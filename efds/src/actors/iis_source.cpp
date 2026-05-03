
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <sys/types.h>
extern "C" {
#include "welt_c_fifo.h"
#include "welt_cpp_graph.h"
#include "welt_cpp_actor.h"
}
#include "iis.h"
#include "iis_sink.h"
#include "iis_source.h"

using namespace std;

iis_source::iis_source(welt_c_fifo_pointer out_fifo, const char *in_dir_name) {
    out = out_fifo;
    dir_name = in_dir_name;
    next_index = 0;
    next_k = 1;
    reset();
}

bool iis_source::enable() {
    bool result = false;
    switch (mode) {
        case IIS_SOURCE_MODE_PROCESS:
            // We need space in the output FIFO to produce a token
            result = (welt_c_fifo_population(out) < welt_c_fifo_capacity(out));
            break;
        case IIS_SOURCE_MODE_COMPLETE:
        case IIS_SOURCE_MODE_ERROR:
            result = true;
            break;
        default:
            result = false;
            break;
    }
    return result;
}

void iis_source::invoke() {
    switch (mode) {
        case IIS_SOURCE_MODE_PROCESS: {
            // Read the next file into a fresh PTIS
            const string &path = file_list[next_index];
            PTIS *p = load_ptis(path);
            if (p == nullptr) {
                cerr << "iis_source: failed to load " << path << endl;
                mode = IIS_SOURCE_MODE_ERROR;
                break;
            }

            // Build the IIS token on the heap
            IIS *token = new IIS();
            token->I = p;
            token->k = next_k;

            // Write the pointer to the FIFO
            welt_c_fifo_write(out, &token);

            // Advance
            next_index++;
            next_k++;

            // If we just wrote the last file, transition to COMPLETE
            if (next_index >= file_list.size()) {
                mode = IIS_SOURCE_MODE_COMPLETE;
            }
            break;
        }
        case IIS_SOURCE_MODE_COMPLETE:
        case IIS_SOURCE_MODE_ERROR:
            // Nothing to do, stay put
            break;
        default:
            mode = IIS_SOURCE_MODE_ERROR;
            break;
    }
}

void iis_source::reset() {
    file_list.clear();
    next_index = 0;
    next_k = 1;

    DIR *dir = opendir(dir_name.c_str());
    if (dir == nullptr) {
        cerr << "iis_source: could not open directory \"" << dir_name << "\"" << endl;
        mode = IIS_SOURCE_MODE_ERROR;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;
        // Skip "." and ".." and anything that isn't a .txt file
        if (name == "." || name == "..") continue;
        if (name.size() < 4) continue;
        if (name.substr(name.size() - 4) != ".txt") continue;
        file_list.push_back(dir_name + "/" + name);
    }
    closedir(dir);

    // Sort for deterministic ordering across runs
    sort(file_list.begin(), file_list.end());

    if (file_list.empty()) {
        cerr << "iis_source: no .txt files found in \"" << dir_name << "\"" << endl;
        mode = IIS_SOURCE_MODE_COMPLETE;
    } else {
        mode = IIS_SOURCE_MODE_PROCESS;
    }
}

iis_source::~iis_source() {
    // file_list is a vector of std::string, cleans itself up.
    // We don't own any tokens still in flight — sinks are responsible for those.
}

void iis_source::connect(welt_cpp_graph *graph) {
    int port_index = 0;
    int direction = GRAPH_OUT_CONN_DIRECTION;
    graph->add_connection(this, port_index, direction);
}

PTIS *iis_source::load_ptis(const string &path) {
    ifstream in_stream(path.c_str());
    if (!in_stream.is_open()) {
        return nullptr;
    }

    PTIS *p = new PTIS();  // constructor already sized to 24x24 zeros
    for (int r = 0; r < 24; r++) {
        for (int c = 0; c < 24; c++) {
            int val;
            if (!(in_stream >> val)) {
                cerr << "iis_source: not enough values in " << path
                     << " (got to r=" << r << " c=" << c << ")" << endl;
                delete p;
                return nullptr;
            }
            p->pix[r][c] = val;
        }
    }
    in_stream.close();
    return p;
}