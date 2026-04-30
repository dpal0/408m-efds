#ifndef EFDS_GRAPH_H
#define EFDS_GRAPH_H

extern "C" {
#include "welt_c_fifo.h"
#include "welt_c_util.h"
}

#include "welt_cpp_graph.h"
#include "welt_cpp_actor.h"
#include "classifierActor.h"
#include "iis_source.h"
#include "iis_sink.h"

#include <vector>

#define EFDS_FIFO_CAPACITY 8
#define EFDS_TOKEN_SIZE    sizeof(IIS*)

class efds_graph : public welt_cpp_graph {
public:
    /**  Construct the EFDS dataflow graph.
       num_stages - number of cascade stages (N classifier actors)
       dataset_dir - directory containing input image text files
       output_dir - base directory for sink output files
                    face sink writes:    output_dir/face.txt
                    nonface sinks write: output_dir/stage{i}_nonface.txt
       nRows, nCols - sub-window dimensions (assumed uniform across dataset) **/
    efds_graph(int num_stages, const char* dataset_dir, const char* output_dir,
               int nRows, int nCols);

    ~efds_graph() override;

    /* Run the graph*/
    void scheduler() override;
    
private:
    int num_stages;
    /* Actors */
    iis_source* source;
    std::vector<classifierActor*> classifiers;   // size = num_stages
    std::vector<iis_sink*>        nonface_sinks; // size = num_stages
    iis_sink*                     face_sink;
    /* FIFOs.
       chain_fifos[i] is the input to classifiers[i].
       chain_fifos[0]  - source output
       chain_fifos[i+1] - classifiers[i] continue output
       face_fifo - classifiers[num_stages-1] continue output
       abort_fifos[i]  classifiers[i] abort output */
    std::vector<welt_c_fifo_pointer> chain_fifos;  // size = num_stages
    welt_c_fifo_pointer              face_fifo;
    std::vector<welt_c_fifo_pointer> abort_fifos;  // size = num_stages
};

#endif