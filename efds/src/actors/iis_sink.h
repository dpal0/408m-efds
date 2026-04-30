#ifndef IIS_SINK_H
#define IIS_SINK_H

extern "C" {
#include "welt_c_fifo.h"
#include "welt_c_util.h"
}

#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "classifierActor.h"   /* for the IIS struct definition */

#include <string>

#define IIS_SINK_MODE_WRITE    1
#define IIS_SINK_MODE_INACTIVE 2

/* Sink actor that consumes IIS* tokens and appends a labeled record
   (one line per token) to an output text file.

   Each consumed token contributes a line of the form:
       <k>F        if is_face is true
       <k>NF       if is_face is false

   The sink is responsible for freeing the IIS struct (and the cv::Mat
   it points to) after consuming each token, since this is a terminal
   actor for the token's lifetime. */
class iis_sink : public welt_cpp_actor {
public:
    iis_sink(welt_c_fifo_pointer in_fifo,
             const char* output_path,
             bool is_face);
    ~iis_sink() override;

    bool enable() override;
    void invoke() override;
    void reset() override;
    void connect(welt_cpp_graph* graph) override;

private:
    /* Input FIFO. */
    welt_c_fifo_pointer in;

    /* Path of the file we append each record to. */
    std::string output_path;

    /* True for the face sink (writes "<k>F"), false for nonface sinks
       (writes "<k>NF"). */
    bool is_face;

    /* Actor mode. */
    int mode;
};

#endif