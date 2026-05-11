#ifndef eval_metrics_h
#define eval_metrics_h
#include <iostream>
#include <fstream>
using namespace std;

class EvalMetrics {

public:

    // constructor
    EvalMetrics (int cascades, int faces, int nonfaces);

    // destructor
    ~EvalMetrics ();

    void calculateCascade (const char* out_filename);
    
    // Accumulate counts from a file containing only one ground-truth class.
    // Pass is_face_file=true if every window in the file is a real face,
    // false if every window is a real non-face.
    void accumulate (const char* out_filename, bool is_face_file);
 
    // Print combined TP/FP/TN/FN and derived metrics across all
    // accumulate() calls made so far.
    void report ();

private:

    void groundTruth ();

    int cascades;
    int faces;
    int nonfaces;

     // Running confusion-matrix totals across accumulate() calls.
    int TP_total;
    int FP_total;
    int TN_total;
    int FN_total;
    
    bool* truth;
};

#endif


