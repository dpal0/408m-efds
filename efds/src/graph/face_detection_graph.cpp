#include "efds_graph.h"

#include <cstdio>
#include <cstring>

efds_graph::efds_graph(int num_stages, const char* dataset_dir, const char* output_dir, int nRows, int nCols): num_stages(num_stages)
{
    /* ---------- 1. Allocate FIFOs ----------
       One input FIFO per classifier stage, plus one for the face sink,
       plus one abort FIFO per stage. */
    chain_fifos.resize(num_stages);
    abort_fifos.resize(num_stages);

    for (int i = 0; i < num_stages; i++) {
        chain_fifos[i] = welt_c_fifo_new(EFDS_FIFO_CAPACITY,
                                         EFDS_TOKEN_SIZE, i);
        abort_fifos[i] = welt_c_fifo_new(EFDS_FIFO_CAPACITY,
                                         EFDS_TOKEN_SIZE,
                                         num_stages + i);
    }
    face_fifo = welt_c_fifo_new(EFDS_FIFO_CAPACITY,
                                EFDS_TOKEN_SIZE, 2 * num_stages);

    /* ---------- 2. Construct the source actor ----------
       The source produces IIS* tokens onto chain_fifos[0]. */
    source = new iis_source(chain_fifos[0], dataset_dir, nRows, nCols);

    /* ---------- 3. Construct the classifier actors ----------
       Stage i:  reads from chain_fifos[i]
                 writes abort to abort_fifos[i]
                 writes continue to chain_fifos[i+1]   (or face_fifo if last) */
    classifiers.resize(num_stages);
    for (int i = 0; i < num_stages; i++) {
        welt_c_fifo_pointer cont_out = (i == num_stages - 1)
                                       ? face_fifo
                                       : chain_fifos[i + 1];
        classifiers[i] = new classifierActor(chain_fifos[i],
                                             abort_fifos[i],
                                             cont_out,
                                             i);
    }

    /* ---------- 4. Construct the sinks ----------
       One nonface sink per stage; one face sink at the end. */
    nonface_sinks.resize(num_stages);
    char path_buf[512];
    for (int i = 0; i < num_stages; i++) {
        std::snprintf(path_buf, sizeof(path_buf),
                      "%s/stage%d_nonface.txt", output_dir, i);
        nonface_sinks[i] = new iis_sink(abort_fifos[i],
                                        path_buf,
                                        /*is_face=*/false);
    }
    std::snprintf(path_buf, sizeof(path_buf), "%s/face.txt", output_dir);
    face_sink = new iis_sink(face_fifo, path_buf, /*is_face=*/true);

    /* ---------- 5. Register actors with the graph ----------
       The order doesn't affect correctness for a self-timed scheduler,
       but listing source first and sinks last is conventional. */
    actors.push_back(source);
    for (int i = 0; i < num_stages; i++) {
        actors.push_back(classifiers[i]);
    }
    for (int i = 0; i < num_stages; i++) {
        actors.push_back(nonface_sinks[i]);
    }
    actors.push_back(face_sink);
}

efds_graph::~efds_graph() {
    /* Delete actors. The base class may also do this depending on
       the Welter version; if it does, drop these deletes. */
    delete source;
    for (auto* c : classifiers) delete c;
    for (auto* s : nonface_sinks) delete s;
    delete face_sink;

    /* Free FIFOs. */
    for (auto& f : chain_fifos) welt_c_fifo_free(f);
    for (auto& f : abort_fifos) welt_c_fifo_free(f);
    welt_c_fifo_free(face_fifo);
}

void efds_graph::scheduler() {
    /* Self-timed scheduler: keep firing any enabled actor until none
       are enabled. For a linear cascade with a finite source, this
       drains everything once the source goes inactive. */
    bool progress = true;
    while (progress) {
        progress = false;
        for (auto* a : actors) {
            if (a->enable()) {
                a->invoke();
                progress = true;
            }
        }
    }
}