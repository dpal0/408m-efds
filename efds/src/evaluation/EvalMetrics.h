#ifndef eval_metrics_h
#define eval_metrics_h

#include <fstream>
using namespace std;

class EvalMetrics {

public:

    // constructor
    EvalMetrics (int cascades, int faces, int nonfaces);

    // destructor
    ~EvalMetrics ();

    void calculateCascade (char* out_filename);
    
private:

    void groundTruth ();

    int cascades;
    int faces;
    int nonfaces;

    bool* truth;
};

#endif


