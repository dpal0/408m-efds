#ifndef CLASSIFIER_ACTOR_H
#define CLASSIFIER_ACTOR_H
extern "C"{
#include "welt_c_fifo.h"
#include "welt_c_util.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include <opencv2/opencv.hpp>
#include "StrongClassifier.h"
#define CONFIGURE 1
#define READ 2
#define CLASSIFY 3
#define TRUE_MOD 4
#define FALSE_MOD 5
struct IIS{
    cv::Mat *I; //image window
    int k; //image index
};
class classifierActor: public welt_cpp_actor{
public: 
    classifierActor(welt_c_fifo_pointer in,welt_c_fifo_pointer abort_out, welt_c_fifo_pointer cont_out, int stage);
    bool enable();
    void invoke();
    void reset();
    void connect(welt_cpp_graph *graph) override;
    ~classifierActor() override;
private: 
    welt_c_fifo_pointer in; //input from prev classifier actor. 
                            // Format: (image window pointer I,image index k)
    welt_c_fifo_pointer cont_out; //output to next classifier actor
    welt_c_fifo_pointer abort_out; //when in false mode
    int stage; //stage num
    StrongClassifier *sc; //strong classifier object
    int mode; //actor mode. 
    IIS *currToken; //current input token (I,K)
};
StrongClassifier* readConfigFile(int stage);
#endif