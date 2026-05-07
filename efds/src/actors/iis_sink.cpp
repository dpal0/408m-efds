#include <iostream>
#include "iis_sink.h"

using namespace std;

iis_sink::iis_sink(welt_c_fifo_pointer in_fifo,
                   const char *out_file_name,
                   const char *in_label) {
    in = in_fifo;
    file_name = out_file_name;
    label = in_label;
    reset();
}

bool iis_sink::enable() {
    bool result = false;
    switch (mode) {
        case IIS_SINK_MODE_PROCESS:
            result = (welt_c_fifo_population(in) > 0);
            break;
        case IIS_SINK_MODE_ERROR:
            result = true;
            break;
        default:
            result = false;
            break;
    }
    return result;
}

void iis_sink::invoke() {
    switch (mode) {
        case IIS_SINK_MODE_PROCESS: {
            IIS *token = nullptr;
            welt_c_fifo_read(in, &token);
            if (token == nullptr) {
                cerr << "iis_sink: read null token" << endl;
                mode = IIS_SINK_MODE_ERROR;
                break;
            }

            // Log result
            out_stream << token->k << " " << label << "\n";

            // OWNERSHIP: the sink is end-of-life for this token, so free it.
            delete token->I;   // free the 24x24 PTIS
            delete token;      // free the IIS struct
            break;
        }
        case IIS_SINK_MODE_ERROR:
            break;
        default:
            mode = IIS_SINK_MODE_ERROR;
            break;
    }
}

void iis_sink::reset() {
    if (out_stream.is_open()) {
        out_stream.close();
    }
    out_stream.open(file_name.c_str());
    if (!out_stream.is_open()) {
        cerr << "iis_sink: could not open output file \"" << file_name << "\"" << endl;
        mode = IIS_SINK_MODE_ERROR;
    } else {
        mode = IIS_SINK_MODE_PROCESS;
    }
}

iis_sink::~iis_sink() {
    if (out_stream.is_open()) {
        out_stream.close();
    }
}

void iis_sink::connect(welt_cpp_graph *graph) {
    int port_index = 0;
    int direction = GRAPH_IN_CONN_DIRECTION;
    graph->add_connection(this, port_index, direction);
}