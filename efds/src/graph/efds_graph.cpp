#include <iostream>
#include <string>
#include <dirent.h>
#include <algorithm>
#include <stdexcept> //diksha
extern "C" {
#include "welt_c_fifo.h"
}
#include "welt_cpp_graph.h"
#include "efds_graph.h"
#include "iis_source.h"
#include "iis_sink.h"
#include "classifierActor.h"


using namespace std;

efds_graph::efds_graph(const char *input_dir,
                       const char *face_out_file,
                       const char *config_dir) //diksha
{
    /* Scan config_dir for .txt files; sort so order is deterministic */ //diksha
    vector<string> config_files;
    DIR *dir = opendir(config_dir); //diksha
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) { //diksha
            string name = entry->d_name;
            if (name.size() > 4 && name.substr(name.size() - 4) == ".txt" && name.substr(0, 6) == "config") {
                config_files.push_back(string(config_dir) + "/" + name); //diksha
            }
        }
        closedir(dir); //diksha
    } else { //diksha
        throw std::runtime_error(string("efds_graph: cannot open config directory: ") + config_dir); //diksha
    } //diksha
    sort(config_files.begin(), config_files.end());
    num_stages = (int)config_files.size();
    face_count = 0;

    /* ---- FIFOs ---------------------------------------------------- */
    /* cont_fifos has num_stages+1 entries:
     *   cont_fifos[0]           : source output -> classifier[0] input
     *   cont_fifos[i] (i>0)     : classifier[i-1] cont_out -> classifier[i] input
     *   cont_fifos[num_stages]  : classifier[num_stages-1] cont_out -> face_sink
     *
     * abort_fifos has num_stages entries:
     *   abort_fifos[i] : classifier[i] abort_out -> nonface_sink[i]
     */
    int token_size = sizeof(IIS *);

    for (int i = 0; i <= num_stages; i++) {
        cont_fifos.push_back(
            welt_c_fifo_new(EFDS_FIFO_CAPACITY, token_size, 0));
    }
    for (int i = 0; i < num_stages; i++) {
        abort_fifos.push_back(
            welt_c_fifo_new(EFDS_FIFO_CAPACITY, token_size, 0));
    }

    /* ---- Source ---------------------------------------------------- */
    src = new iis_source(cont_fifos[0], input_dir);

    /* ---- Classifiers (one per .txt file found in config_dir) ------- */
    for (int i = 0; i < num_stages; i++) {
        classifiers.push_back(
            new classifierActor(cont_fifos[i],
                                abort_fifos[i],
                                cont_fifos[i + 1],
                                config_files[i])); //diksha
    }

    /* ---- Face sink ------------------------------------------------- */
    face_sink = new iis_sink(cont_fifos[num_stages], face_out_file, "F");

    /* ---- Non-face sinks (one per stage) ---------------------------- */
    for (int i = 0; i < num_stages; i++) {
        //string out_name = "nonface_stage" + to_string(i) + ".txt";
        nonface_sinks.push_back(
            new iis_sink(abort_fifos[i],face_out_file, "NF"));
    }
}

void efds_graph::scheduler() {
    /* Drive execution until the source is exhausted and all FIFOs
     * are drained.  We fire each actor whenever it is enabled,
     * sweeping through source -> classifiers -> sinks in order.
     * The outer loop keeps iterating as long as at least one actor
     * fired during a full sweep (progress check), so we stop cleanly
     * once everything is drained.
     */
    bool progress = true;
    while (progress) {
        progress = false;

        /* Source */
        if (src->enable()) {
            src->invoke();
            progress = true;
        }

        /* Classifiers in pipeline order */
        for (int i = 0; i < num_stages; i++) {
            if (classifiers[i]->enable()) {
                classifiers[i]->invoke();
                progress = true;
            }
        }

        /* Face sink */
        if (face_sink->enable()) {
            face_sink->invoke();
            face_count++; //For time performance
            progress = true;
        }

        /* Non-face sinks */
        for (int i = 0; i < num_stages; i++) {
            if (nonface_sinks[i]->enable()) {
                nonface_sinks[i]->invoke();
                progress = true;
            }
        }
    }
}

efds_graph::~efds_graph() {
    delete src;

    for (int i = 0; i < num_stages; i++) {
        delete classifiers[i];
    }

    delete face_sink;

    for (int i = 0; i < num_stages; i++) {
        delete nonface_sinks[i];
    }

    for (int i = 0; i <= num_stages; i++) {
        welt_c_fifo_free(cont_fifos[i]);
    }
    for (int i = 0; i < num_stages; i++) {
        welt_c_fifo_free(abort_fifos[i]);
    }
}

