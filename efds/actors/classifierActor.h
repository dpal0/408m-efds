#ifndef CLASSIFIER_ACTOR_H
#define CLASSIFIER_ACTOR_H

extern "C"{
#include "welt_c_util.h"
#include "welt_c_fifo.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "iis.h"
#include "StrongClassifier.h"
#include <string>

#define CLASSIFIER_ACTOR_MODE_CONFIGURE 0
#define CLASSIFIER_ACTOR_MODE_READ      1
#define CLASSIFIER_ACTOR_MODE_CLASSIFIY 2
#define CLASSIFIER_ACTOR_MODE_TRUE      3
#define CLASSIFIER_ACTOR_MODE_FLASE     4


//TO DO: Figure out private fields, config files from the classifier

class ClassifierActor: public welt_cpp_actor {
public:
	ClassifierActor(welt_c_fifo_pointer input,
			welt_c_fifo_pointer continue_out,
			welt_c_fifo_pointer abort_out); //Constructor takes config files for this classifier instance
	
	~ClassifierActor();

	bool enable() override;
	void invoke() override;

	void reset() override;

	void connect() override;

private:
	welt_c_fifo_pointer input;
	welt_c_fifo_pointer output;
};

#endif 
