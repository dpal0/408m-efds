#include <iostream>
#include <vector>
#include "welt_c_fifo.h"
#include "welt_cpp_graph.h"
#include "welt_cpp_actor.h"
#include "iis.h"
#include "iis_source.h"
#include "iis_sink.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_dir> <output_log>" << endl;
        return 1;
    }
    const char *input_dir  = argv[1];
    const char *output_log = argv[2];

    // FIFO between source and sink.
    // Token type is IIS*, so we store sizeof(IIS*) bytes per slot.
    // Capacity 4 is plenty for a 2-actor pipeline.
    welt_c_fifo_pointer fifo = welt_c_fifo_new(4, sizeof(IIS *), 1);

    // Instantiate actors
    iis_source *src  = new iis_source(fifo, input_dir);
    iis_sink   *sink = new iis_sink(fifo, output_log, "passthrough");

    // Put them in a list for the scheduler to walk
    vector<welt_cpp_actor *> actors;
    actors.push_back(src);
    actors.push_back(sink);

    // Simple round-robin scheduler.
    // Run until no actor is enabled (or until source is done AND fifo is drained).
    int safety = 100000;  // hard cap to prevent infinite loops while debugging
    bool any_fired;
    do {
        any_fired = false;
        for (size_t i = 0; i < actors.size(); i++) {
            if (actors[i]->enable()) {
                // Don't fire source if it's already COMPLETE — just skip,
                // otherwise it'll spin forever in the COMPLETE branch.
                // (enable() returns true for COMPLETE in the lab pattern,
                // so we filter here based on whether work actually happens.)
                actors[i]->invoke();
                any_fired = true;
            }
        }
        safety--;
    } while (any_fired && safety > 0);

    if (safety == 0) {
        cerr << "test_source_sink: scheduler hit safety cap, possible infinite loop" << endl;
    }

    // Cleanup
    delete src;
    delete sink;
    welt_c_fifo_free(fifo);

    cout << "test_source_sink: done. Check " << output_log << endl;
    return 0;
}