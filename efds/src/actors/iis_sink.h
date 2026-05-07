#ifndef IIS_SINK_H
#define IIS_SINK_H

#include <fstream>
#include <string>
#include "welt_cpp_actor.h"
#include "welt_c_fifo.h"
#include "welt_cpp_graph.h"
#include "iis.h"

#define IIS_SINK_MODE_PROCESS 0
#define IIS_SINK_MODE_ERROR   1

class iis_sink : public welt_cpp_actor {
public:
    // label is what gets written next to each k (e.g., "stage1_reject", "face")
    iis_sink(welt_c_fifo_pointer in_fifo,
             const char *out_file_name,
             const char *label);
    ~iis_sink();
    bool enable();
    void invoke();
    void reset();
    void connect(welt_cpp_graph *graph);

private:
    welt_c_fifo_pointer in;
    std::string file_name;
    std::string label;
    std::ofstream out_stream;
};

#endif