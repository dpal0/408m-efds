/*
 * test_classifier_actor.cpp
 *
 * Driver for classifierActor.
 *
 * Usage:
 *   ./test_classifier_actor.exe <input_dir> <config_file> <output_log>
 *
 * - input_dir   : directory of PTIS .txt image files (same format as iis_source)
 * - config_file : strong classifier config file (same format classifierActor reads)
 * - output_log  : file to write results (face/non-face per image index k)
 *
 * The driver wires up:
 *   iis_source --> classifierActor --> [cont_fifo | abort_fifo]
 *
 * After the scheduler finishes, it drains both output fifos and logs results.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

extern "C" {
#include "welt_c_fifo.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "iis.h"
#include "iis_source.h"
#include "classifierActor.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0]
             << " <input_dir> <config_file> <output_log>" << endl;
        return 1;
    }
    const char *input_dir   = argv[1];
    const char *config_file = argv[2];
    const char *output_log  = argv[3];

    // ---- FIFOs ----
    // in_fifo:   iis_source  --> classifierActor.in
    // cont_fifo: classifierActor.cont_out  (face)
    // abort_fifo:classifierActor.abort_out (non-face)
    const int FIFO_CAP = 16;
    welt_c_fifo_pointer in_fifo    = welt_c_fifo_new(FIFO_CAP, sizeof(IIS *), 0);
    welt_c_fifo_pointer cont_fifo  = welt_c_fifo_new(FIFO_CAP, sizeof(IIS *), 0);
    welt_c_fifo_pointer abort_fifo = welt_c_fifo_new(FIFO_CAP, sizeof(IIS *), 0);

    // ---- Actors ----
    iis_source      *src = new iis_source(in_fifo, input_dir);
    classifierActor *clf = new classifierActor(in_fifo, abort_fifo, cont_fifo,
                                               string(config_file));

    vector<welt_cpp_actor *> actors;
    actors.push_back(src);
    actors.push_back(clf);

    // ---- Scheduler ----
    int  safety = 500000;
    bool any_fired;
    do {
        any_fired = false;
        for (size_t i = 0; i < actors.size(); i++) {
            if (actors[i]->enable()) {
                actors[i]->invoke();
                any_fired = true;
            }
        }
        safety--;
    } while (any_fired && safety > 0);

    if (safety == 0) {
        cerr << "test_classifier_actor: scheduler hit safety cap" << endl;
    }

    // ---- Drain output FIFOs and log results ----
    ofstream log(output_log);
    if (!log.is_open()) {
        cerr << "test_classifier_actor: could not open output log: "
             << output_log << endl;
        return 1;
    }

    int face_count    = 0;
    int nonface_count = 0;

    IIS *tok = nullptr;

    // Drain cont_fifo (faces)
    while (welt_c_fifo_population(cont_fifo) > 0) {
        welt_c_fifo_read(cont_fifo, &tok);
        if (tok) {
            log << "k=" << tok->k << " FACE" << endl;
            face_count++;
            delete tok->I;
            delete tok;
            tok = nullptr;
        }
    }

    // Drain abort_fifo (non-faces)
    while (welt_c_fifo_population(abort_fifo) > 0) {
        welt_c_fifo_read(abort_fifo, &tok);
        if (tok) {
            log << "k=" << tok->k << " NON-FACE" << endl;
            nonface_count++;
            delete tok->I;
            delete tok;
            tok = nullptr;
        }
    }

    log << "---" << endl;
    log << "FACE:     " << face_count    << endl;
    log << "NON-FACE: " << nonface_count << endl;
    log.close();

    cout << "test_classifier_actor: done." << endl;
    cout << "  FACE:     " << face_count    << endl;
    cout << "  NON-FACE: " << nonface_count << endl;
    cout << "  See " << output_log << " for per-image results." << endl;

    // ---- Cleanup ----
    delete src;
    delete clf;
    welt_c_fifo_free(in_fifo);
    welt_c_fifo_free(cont_fifo);
    welt_c_fifo_free(abort_fifo);

    return 0;
}