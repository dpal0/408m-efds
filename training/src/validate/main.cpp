#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <dirent.h>
#include <chrono>
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
 * readConfigFile
 * Generates one strong classifier per config file.
 ******************************************************************************/
StrongClassifier readConfigFile (int stage, const string &dir) {
    
    ifstream configFile(dir + "/config" + to_string(stage) + ".txt");

    if(!configFile.is_open()) {
        cerr << "Error: couldnt open config file:" << dir + "/config" + to_string(stage) + ".txt" << endl;
    }

    float sc_threshold, weight, wc_threshold, error;
    int polarity, x, y, w, h, type_int;
    vector<StrongClassifier::wc_weight_pair> wc_vector; 
    

    configFile >>sc_threshold;

    while(configFile >> x >> y >> w >> h >> type_int >> wc_threshold >> polarity >> error >> weight){
        Feature type=(Feature) type_int;
        HaarFeature hf(x,y,w,h,type);
        WeakClassifier wc(hf,wc_threshold,polarity,error);

        StrongClassifier::wc_weight_pair pair(wc, weight);
        wc_vector.push_back(pair);
    }
    configFile.close(); 
    StrongClassifier sc(wc_vector,sc_threshold);
    return sc;
}


/*******************************************************************************
 * makeTrainingReport
 * Generates a brief training report after all stages are complete, writing it to a file in the output directory. The report includes the number of stages, stage configurations, and total training time.
 ******************************************************************************/
vector<double> makeEvalReport (const string &dir, double TPR, double FPR, int TP, int FP, int TN, int FN, double elapsed) {
    std::ofstream evalFile;
    evalFile.open(dir + "/eval_report.txt", std::ios_base::app);

    float precision = (float)TP / (TP + FP);
    float recall = (float)TP / (TP + TN);
    float f1 = (2 * precision * recall) / (precision + recall);
    TPR *= ((double)TP / (TP + FN));
    FPR *= ((double)FP / (FP + TN));

    evalFile << TP << " "  << FP << " "  << TN << " "  << FN << " "  << TPR << " "  << FPR << " "  << precision << " "  << recall << " "  << f1 << " "  << elapsed << endl;

    evalFile.close();

    cout << "\nTP: " << TP << ", FP: " << FP << ", TN: " << TN << ", FN:" << FN << endl;
    cout << "TPR: " << TPR << ", FPR: " << FPR << endl;
    cout << "Precision: " << precision << ", Recall: " << recall << ", F1: " << f1 << endl;
    cout << "\nTime elapsed: " << elapsed << " ns" <<endl;

    vector<double> retVal = {TPR, FPR};
    return retVal;
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
             << " <face_dir> <nonface_dir> <num_stages> <config_dir>" << endl;
        return 1;
    }

    string faceDir = argv[1];
    string nonfaceDir = argv[2];
    int numStages = stoi(argv[3]);
    string configDir = argv[4];

    // Load images
    cout << "Loading training images..." << endl;
    vector<vector<vector<int>>> face_validating = loadImages(faceDir);
    vector<vector<vector<int>>> nonface_validating = loadImages(nonfaceDir);
    vector<vector<vector<int>>> currentPositives = face_validating;
    vector<vector<vector<int>>> currentNegatives = nonface_validating;

    if (face_validating.empty() || nonface_validating.empty())
    {
        cerr << "Error: need at least one face and one non-face image." << endl;
        return 1;
    }

    // evaluation metrics
    vector<double> rates = {1.0, 1.0};

    // Timer to determine performance/speed
    auto overallStart = std::chrono::high_resolution_clock::now();

    // Cascade training loop
    for (int stage = 0; stage < numStages; stage++)
    {

        // timer start
        auto start = std::chrono::high_resolution_clock::now();

        cout << "\n=== Validation stage " << stage + 1 << " / " << numStages
             << " ===" << endl;
        cout << "Positives: " << currentPositives.size()
             << "  Negatives: " << currentNegatives.size() << endl;

        if (currentPositives.empty() || currentNegatives.empty())
        {
            cout << "No more images remain — stopping early at stage "
                 << stage + 1 << endl;
            break;
        }

        // Construct and train the strong classifier
        StrongClassifier sc = readConfigFile (stage, configDir); 

        // nonface_training shrinks each stage: only false positives from the previous stage are passed to the next stage as negatives.
        for (size_t i = 0; i < currentPositives.size(); i++) {
            if (!sc.classify(currentPositives[i]))
                currentPositives.erase(currentPositives.begin() + i);
        }

        for (size_t i = 0; i < currentNegatives.size(); i++) {
            if (!sc.classify(currentNegatives[i]))
                currentNegatives.erase(currentNegatives.begin() + i);
        }

        // timer (one stage) elapsed
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        //start = time(NULL);

        // evaluate training stage
        int TP = currentPositives.size();
        int FP = currentNegatives.size();
        int TN = nonface_validating.size() - currentNegatives.size();
        int FN = face_validating.size() - currentPositives.size();
        rates = makeEvalReport (configDir, rates[0], rates[1], TP, FP, TN, FN, elapsed);

    }

    // timer (overall) elapsed
    auto overallEnd = std::chrono::high_resolution_clock::now();
    auto overallElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(overallEnd-overallStart).count();

    cout << "\nTotal validation time: " << overallElapsed << " ns." << endl;

    cout << "\nTraining complete." << endl
         << endl;

    //makeTrainingReport(outputDir, numStages, overallElapsed, sc_rounds);
    return 0;
}
#endif