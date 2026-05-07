#ifndef EFDS_GRAPH_H
#define EFDS_GRAPH_H

#include <vector>
#include <string>

extern "C" {
#include "welt_c_fifo.h"
}
#include "welt_cpp_graph.h"
#include "iis_source.h"
#include "iis_sink.h"
#include "classifierActor.h"

#define EFDS_FIFO_CAPACITY 4

class efds_graph : public welt_cpp_graph {
public:
    efds_graph(const char *input_dir,
               const char *face_out_file,
               const char *config_dir); //diksha

    void scheduler();
    ~efds_graph();

private:
    /* Actors */
    iis_source                    *src;
    std::vector<classifierActor*>  classifiers;   // num_stages entries
    iis_sink                      *face_sink;
    std::vector<iis_sink*>         nonface_sinks; // one per stage

    /* FIFOs */
    /* src_to_stage[0]     : source -> classifier[0]          */
    /* src_to_stage[i]     : classifier[i-1].cont -> classifier[i], i>0 */
    /* cont_fifos[N-1].cont -> face_sink                      */
    std::vector<welt_c_fifo_pointer> cont_fifos;  // num_stages+1 entries
    std::vector<welt_c_fifo_pointer> abort_fifos; // num_stages entries

    int num_stages;
};

#endif /* EFDS_GRAPH_H */