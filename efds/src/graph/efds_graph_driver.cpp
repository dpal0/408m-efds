#include <iostream>
#include <cstdlib>
extern "C" {
#include "welt_c_fifo.h"
}
#include "efds_graph.h"

using namespace std;

/* Usage: efds_driver.exe <input_dir> <face_out_file> [num_stages]
 *
 *   input_dir     - directory of 24x24 pixel window .txt files
 *   face_out_file - path for the face classification log
 *   num_stages    - (optional) number of cascade stages, default 3
 *
 * Non-face logs are written to nonface_stage0.txt, nonface_stage1.txt, ...
 * in the current working directory.
 */
int main(int argc, char **argv) {
    if (argc < 3 || argc > 4) {
        cerr << "Usage: " << argv[0]
             << " <input_dir> <face_out_file> [num_stages]" << endl;
        return 1;
    }

    const char *input_dir     = argv[1];
    const char *face_out_file = argv[2];
    int         num_stages    = 3;          /* default */

    if (argc == 4) {
        num_stages = atoi(argv[3]);
        if (num_stages < 1) {
            cerr << "efds_driver: num_stages must be >= 1" << endl;
            return 1;
        }
    }

    efds_graph *graph = new efds_graph(input_dir, face_out_file, num_stages);
    graph->scheduler();
    delete graph;

    return 0;
}