#ifndef INTEGRAL_IMAGE_H
#define INTEGRAL_IMAGE_H

extern "C"{
#include "welt_c_util.h"
#include "welt_c_fifo.h"
}
#include welt_cpp_actor.h"
#include welt_cpp_graph.h"
#include "iis.h"

class IntegralImageActor: public welt_cpp_actor{

public: 
	IntegralImageActor(welt_c_fifo_pointer img_in,
			   welt_c_fifo_pointer img_out;
			   int index);
	~IntergralImageActor();

	IIS compute(IIS input_image_subwindow);
	bool enable() override;
	void invoke() override;
	void reset() override;
	void connect(welt_c_fifo_graph *graph) override;
private:
	void buildIntegralImage(PTIS& subwindow);

};

#endif

