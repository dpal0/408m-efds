#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <dirent.h>
#include "../HaarFeature.h"
#include "../WeakClassifier.h"
#include "../StrongClassifier.h"

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
bool loadTXT(const string &path, vector<vector<int>> &img)
{
    ifstream f(path);
    if (!f.is_open())
    {
        cerr << "loadTXT: cannot open " << path << endl;
        return false;
    }

    img.assign(IMG_ROWS, vector<int>(IMG_COLS));

    string line;
    for (int r = 0; r < IMG_ROWS; r++)
    {
        if (!getline(f, line))
        {
            cerr << "loadTXT: not enough rows in " << path << endl;
            return false;
        }
        stringstream ss(line);
        string token;
        for (int c = 0; c < IMG_COLS; c++)
        {
            if (!getline(ss, token, ','))
            {
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
vector<vector<vector<int>>> loadImages(const string &dir)
{
    vector<vector<vector<int>>> images;
    DIR *d = opendir(dir.c_str());
    if (!d)
    {
        cerr << "loadImages: cannot open directory " << dir << endl;
        return images;
    }
    struct dirent *entry;
    while ((entry = readdir(d)) != nullptr)
    {
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
vector<HaarFeature> generateFeatures()
{
    vector<HaarFeature> features;

    for (int y = 0; y < IMG_ROWS; y++)
    {
        for (int x = 0; x < IMG_COLS; x++)
        {
            for (int h = 1; y + h <= IMG_ROWS; h++)
            {
                for (int w = 1; x + w <= IMG_COLS; w++)
                {

                    if (h % 2 == 0)
                    {
                        HaarFeature feat(x, y, w, h, TWO_RECT_VERTICAL);
                        features.push_back(feat);
                    }
                    if (w % 2 == 0)
                    {
                        HaarFeature feat(x, y, w, h, TWO_RECT_HORIZONTAL);
                        features.push_back(feat);
                    }
                    if (h % 3 == 0)
                    {
                        HaarFeature feat(x, y, w, h, THREE_RECT_VERTICAL);
                        features.push_back(feat);
                    }
                    if (w % 3 == 0)
                    {
                        HaarFeature feat(x, y, w, h, THREE_RECT_HORIZONTAL);
                        features.push_back(feat);
                    }
                    if (h % 2 == 0 && w % 2 == 0)
                    {
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
 * makeConfigFile
 * Generates one config file for one strong classifier. For each strong
 * classifier, pulls all the information related to the strong classifier and
 * writes it to a file. The file is setup as follows:
 *   1 line with strong classifier threshold
 *   1 line for each weak classifier with x, y, width, height, feature type,
 *       wc_threshold, polarity, error, and weight
 ******************************************************************************/
void makeConfigFile(StrongClassifier *sc, int stage, const string &dir)
{
    ofstream configFile(dir + "/config" + to_string(stage) + ".txt");

    configFile << sc->getThreshold() << endl;

    vector<StrongClassifier::wc_weight_pair> wc_vector = sc->getWeakClassifiers();

    for (StrongClassifier::wc_weight_pair it : wc_vector)
    {
        WeakClassifier wc = it.wc;
        float weight = it.weight;

        HaarFeature *feature = wc.getHaarFeature();
        float threshold = wc.getThreshold();
        int polarity = wc.getPolarity();
        float error = wc.getError();

        int x = feature->getX();
        int y = feature->getY();
        int w = feature->getWidth();
        int h = feature->getHeight();
        int type = feature->getType();

        configFile << x << " "
                   << y << " "
                   << w << " "
                   << h << " "
                   << type << " "
                   << threshold << " "
                   << polarity << " "
                   << error << " "
                   << weight << endl;
    }

    configFile.close();
}

/*******************************************************************************
 * makeTrainingReport
 * Generates a brief training report after all stages are complete, writing it to a file in the output directory. The report includes the number of stages, stage configurations, and total training time.
 ******************************************************************************/
void makeTrainingReport(const string &dir, int numStages, double elapsed, int sc_rounds[])
{
    ofstream reportFile(dir + "/training_report.txt");

    reportFile << "Training Report for run " << dir << endl;
    reportFile << "=================" << endl;
    reportFile << "Number of Stages: " << numStages << endl;
    reportFile << "Total Training Time: " << elapsed << " seconds" << endl;
    reportFile << "\nStage Configurations:" << endl;

    for (int stage = 0; stage < numStages; stage++)
    {
        reportFile << "Stage " << stage + 1 << ": " << sc_rounds[stage] << " weak classifiers" << endl;
    }

    reportFile.close();
}

/*******************************************************************************
 * main
 * Usage: ./train <face_dir> <nonface_dir> <num_stages> <output_dir>
 *
 * Runs cascade training and writes one config file per stage to output_dir.
 ******************************************************************************/
#ifndef UNIT_TESTING
int main(int argc, char **argv)
{

    // parse command line arguments
    if (argc != 5)
    {
        cerr << "Usage: " << argv[0]
             << " <face_dir> <nonface_dir> <num_stages> <output_dir>" << endl;
        return 1;
    }

    string faceDir = argv[1];
    string nonfaceDir = argv[2];
    string stageConfig = argv[3];
    string outputDir = argv[4];

    int numStages;
    ifstream stageConfigFile(stageConfig);
    if (!stageConfigFile.is_open())
    {
        cerr << "Error: Could not open stage config file: " << stageConfig << endl;
        return 1;
    }
    stageConfigFile >> numStages;
    int sc_rounds[numStages];
    for (int i = 0; i < numStages; i++)
    {
        int rounds;
        stageConfigFile >> rounds;
        sc_rounds[i] = rounds;
    }
    stageConfigFile.close();

    // Load images
    cout << "Loading training images..." << endl;
    vector<vector<vector<int>>> face_training = loadImages(faceDir);
    vector<vector<vector<int>>> nonface_training = loadImages(nonfaceDir);
    vector<vector<vector<int>>> currentNegatives = nonface_training;

    if (face_training.empty() || nonface_training.empty())
    {
        cerr << "Error: need at least one face and one non-face image." << endl;
        return 1;
    }

    // Generate all candidate Haar features
    cout << "Generating candidate features..." << endl;
    vector<HaarFeature> features = generateFeatures();

    // Timer to determine performance/speed
    time_t overallStart = time(NULL);

    // Cascade training loop
    for (int stage = 0; stage < numStages; stage++)
    {

        // timer start
        time_t start = time(NULL);

        cout << "\n=== Training stage " << stage + 1 << " / " << numStages
             << " ===" << endl;
        cout << "Positives: " << face_training.size()
             << "  Negatives: " << currentNegatives.size() << endl;

        if (currentNegatives.empty())
        {
            cout << "No false positives remain — stopping early at stage "
                 << stage + 1 << endl;
            break;
        }

        // Construct and train the strong classifier
        StrongClassifier sc;
        sc.train(features, face_training, currentNegatives, sc_rounds[stage], stage + 1, numStages, 0.99f);

        // nonface_training shrinks each stage: only false positives from the previous stage are passed to the next stage as negatives.
        for (size_t i = 0; i < currentNegatives.size(); i++)
        {
            if (!sc.classify(currentNegatives[i]))
            {
                currentNegatives.erase(currentNegatives.begin() + i);
            }
        }

        // timer (one stage) elapsed
        time_t end = time(NULL);
        double elapsed = difftime(end, start);
        cout << "\nTraining time for stage " << stage + 1 << ": " << elapsed << " seconds." << endl;
        start = time(NULL);

        // write to config file
        makeConfigFile(&sc, stage, outputDir);
    }

    // timer (overall) elapsed
    time_t overallEnd = time(NULL);
    double overallElapsed = difftime(overallEnd, overallStart);
    cout << "\nTotal training time: " << overallElapsed << " seconds." << endl;

    cout << "\nConfig Files stored in: " << outputDir << endl;

    cout << "\nTraining complete." << endl
         << endl;

    makeTrainingReport(outputDir, numStages, overallElapsed, sc_rounds);
    return 0;
}
#endif
