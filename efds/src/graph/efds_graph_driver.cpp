#include <iostream>
extern "C" {
#include "welt_c_fifo.h"
}
#include "efds_graph.h"

using namespace std;

/* Usage: efds_driver.exe <input_dir> <face_out_file> <config_dir>
 *
 *   input_dir     - directory of 24x24 pixel window .txt files
 *   face_out_file - path for the face classification log
 *   config_dir    - directory containing one .txt config per classifier stage
 *
 * Non-face logs are written to nonface_stage0.txt, nonface_stage1.txt, ...
 * in the current working directory.
 */ //diksha
int main(int argc, char **argv) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0]
             << " <input_dir> <face_out_file> <config_dir>" << endl;
        return 1;
    }

    const char *input_dir     = argv[1];
    const char *face_out_file = argv[2];
    const char *config_dir    = argv[3]; //diksha

    efds_graph *graph = new efds_graph(input_dir, face_out_file, config_dir);
    graph->scheduler();
    delete graph;

    return 0;
}