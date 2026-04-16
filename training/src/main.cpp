#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <dirent.h>
#include "integral_image.h"
#include "HaarFeature.h"
#include "StrongClassifier.h"

using namespace std;

/* The dataset uses 24x24 sub-windows. */
static const int IMG_ROWS = 24;
static const int IMG_COLS = 24;

/*******************************************************************************
 * loadTXT
 * Loads a single image from a .txt file containing IMG_ROWS rows of
 * IMG_COLS comma-separated pixel values (0-255).
 * Returns true on success, false if the file cannot be read or is the wrong size.
 ******************************************************************************/
bool loadTXT(const string& path, vector<vector<int>>& img) {
    ifstream f(path);
    if (!f.is_open()) {
        cerr << "loadTXT: cannot open " << path << endl;
        return false;
    }

    img.assign(IMG_ROWS, vector<int>(IMG_COLS));

    string line;
    for (int r = 0; r < IMG_ROWS; r++) {
        if (!getline(f, line)) {
            cerr << "loadTXT: not enough rows in " << path << endl;
            return false;
        }
        stringstream ss(line);
        string token;
        for (int c = 0; c < IMG_COLS; c++) {
            if (!getline(ss, token, ',')) {
                cerr << "loadTXT: not enough columns in row " << r
                     << " of " << path << endl;
                return false;
            }
            img[r][c] = stoi(token);
        }
    }

    return true;
}


/*******************************************************************************
 * loadImages
 * Loads all .txt files from a directory, returning one 2D pixel grid per file.
 ******************************************************************************/
vector<vector<vector<int>>> loadImages(const string& dir) {
    vector<vector<vector<int>>> images;
    DIR* d = opendir(dir.c_str());
    if (!d) {
        cerr << "loadImages: cannot open directory " << dir << endl;
        return images;
    }
    struct dirent* entry;
    while ((entry = readdir(d)) != nullptr) {
        string name = entry->d_name;
        if (name.size() < 4 || name.substr(name.size() - 4) != ".txt")
            continue;
        string path = dir + "/" + name;
        vector<vector<int>> img;
        if (loadTXT(path, img))
            images.push_back(img);
    }
    closedir(d);
    cout << "Loaded " << images.size() << " images from " << dir << endl;
    return images;
}

/*******************************************************************************
 * computeIntegralImages
 * Computes an IntegralImage for each loaded image.
 ******************************************************************************/
vector<IntegralImage> computeIntegralImages(
        const vector<vector<vector<int>>>& images) {
    vector<IntegralImage> iis;
    iis.reserve(images.size());
    for (const auto& img : images)
        iis.push_back(img);
    return iis;
}

/*******************************************************************************
 * generateFeatures
 * Returns all valid Haar features for an IMG_ROWS x IMG_COLS window.
 * For each feature type, iterates over all valid (x, y, w, h) combinations.
 * Width/height constraints per type:
 *   TWO_RECT_VERTICAL   — h divisible by 2
 *   TWO_RECT_HORIZONTAL — w divisible by 2
 *   THREE_RECT_VERTICAL — h divisible by 3
 *   THREE_RECT_HORIZONTAL — w divisible by 3
 *   FOUR_RECT           — w and h both divisible by 2
 ******************************************************************************/
vector<HaarFeature> generateFeatures() {
    vector<HaarFeature> features;

    for (int y = 0; y < IMG_ROWS; y++) {
        for (int x = 0; x < IMG_COLS; x++) {
            for (int h = 1; y + h <= IMG_ROWS; h++) {
                for (int w = 1; x + w <= IMG_COLS; w++) {

                    if (h % 2 == 0) {
                        HaarFeature feat(x, y, w, h, TWO_RECT_VERTICAL);
                        features.push_back(feat);
                    }
                    if (w % 2 == 0) {
                        HaarFeature feat(x, y, w, h, TWO_RECT_HORIZONTAL);
                        features.push_back(feat);
                    }
                    if (h % 3 == 0) {
                        HaarFeature feat(x, y, w, h, THREE_RECT_VERTICAL);
                        features.push_back(feat);
                    }
                    if (w % 3 == 0) {
                        HaarFeature feat(x, y, w, h, THREE_RECT_HORIZONTAL);
                        features.push_back(feat);
                    }
                    if (h % 2 == 0 && w % 2 == 0) {
                        HaarFeature feat(x, y, w, h, FOUR_RECT);
                        features.push_back(feat);
                    }
                }
            }
        }
    }

    cout << "Generated " << features.size() << " candidate features." << endl;
    return features;
}

/*******************************************************************************
 * main
 * Usage: ./train <face_dir> <nonface_dir> <num_stages> <output_dir>
 *
 * Runs cascade training and writes one config file per stage to output_dir.
 ******************************************************************************/
int main(int argc, char** argv) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0]
             << " <face_dir> <nonface_dir> <num_stages> <output_dir>" << endl;
        return 1;
    }

    string faceDir    = argv[1];
    string nonfaceDir = argv[2];
    int    numStages  = stoi(argv[3]);
    string outputDir  = argv[4];

    // Load images
    cout << "Loading training images..." << endl;
    vector<vector<vector<int>>> faceImgs    = loadImages(faceDir);
    vector<vector<vector<int>>> nonfaceImgs = loadImages(nonfaceDir);

    if (faceImgs.empty() || nonfaceImgs.empty()) {
        cerr << "Error: need at least one face and one non-face image." << endl;
        return 1;
    }

    // Precompute integral images
    cout << "Computing integral images..." << endl;
    vector<IntegralImage> face_training    = computeIntegralImages(faceImgs);
    vector<IntegralImage> nonface_training = computeIntegralImages(nonfaceImgs);

    // Generate all candidate Haar features 
    cout << "Generating candidate features..." << endl;
    vector<HaarFeature> features = generateFeatures();

    //  Cascade training loop 
    // nonface_training shrinks each stage: only false positives from the previous
    // stage are passed to the next stage as negatives.
    vector<IntegralImage> currentNegatives = nonface_training;

    for (int stage = 0; stage < numStages; stage++) {
        cout << "\n=== Training stage " << stage + 1 << " / " << numStages
             << " ===" << endl;
        cout << "Positives: " << face_training.size()
             << "  Negatives: " << currentNegatives.size() << endl;

        if (currentNegatives.empty()) {
            cout << "No false positives remain — stopping early at stage "
                 << stage + 1 << endl;
            break;
        }

        // Construct and train the strong classifier
        StrongClassifier sc;
        int rounds = 5;
        sc.train(features, face_training, currentNegatives, rounds);

        //currentNegatives starts as a copy of nonface_training, 
        //and then shrinks each stage to only the false positives from the previous stage.

        // TODO: write the config file for this stage
        // string configPath = outputDir + "/classifier_" + to_string(stage+1) + ".cfg";
        // sc.writeConfig(configPath); //change 

    }

    cout << "\nTraining complete." << endl;
    return 0;
}
