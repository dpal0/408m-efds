#include <string>
#include "EvalMetrics.h"
using namespace std;

int main(int argc, char **argv) {

    // number of cascades/iterations to compute
    int cascades = stoi(argv[1]);

    // inputs used to generate ground truth data
    int faces = stoi(argv[2]);
    int nonfaces = stoi(argv[3]);

    // output file to save results to
    char* out_filename = argv[4];

    EvalMetrics* metrics = new EvalMetrics(cascades, faces, nonfaces);
    metrics->calculateCascade(out_filename);
    delete metrics;
}
