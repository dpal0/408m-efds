#include "iis_source.h"

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#define MAX_FIFO_COUNT 1

iis_source::iis_source(welt_c_fifo_pointer out_fifo,
                       const char* dataset_dir_in,
                       int nRows_in, int nCols_in) {
    this->out         = out_fifo;
    this->dataset_dir = string(dataset_dir_in);
    this->nRows       = nRows_in;
    this->nCols       = nCols_in;
    this->file_index  = 0;
    this->grid_ptr    = nullptr;
    this->mode        = IIS_SOURCE_MODE_READ;

    this->actor_set_max_port_count(MAX_FIFO_COUNT);

    /* Populate file_names from dataset_dir. */
    scan_directory();

    /* If the directory is empty, jump straight to INACTIVE so that
       enable() returns false and the scheduler doesn't spin on us. */
    if (file_names.empty()) {
        cerr << "iis_source warning: no .txt files found in '"
             << dataset_dir << "'" << endl;
        mode = IIS_SOURCE_MODE_INACTIVE;
    }
}

iis_source::~iis_source() {
    if (grid_ptr != nullptr) {
        delete grid_ptr;
        grid_ptr = nullptr;
    }
}

void iis_source::scan_directory() {
    file_names.clear();

    DIR* dir = opendir(dataset_dir.c_str());
    if (dir == nullptr) {
        cerr << "iis_source error: could not open directory '"
             << dataset_dir << "'" << endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name(entry->d_name);
        /* Skip "." and ".." */
        if (name == "." || name == "..") continue;

        /* Keep only .txt files. */
        if (name.size() < 4) continue;
        string ext = name.substr(name.size() - 4);
        if (ext != ".txt") continue;

        file_names.push_back(name);
    }
    closedir(dir);

    /* Lexicographic order so k=1,2,... is deterministic. */
    sort(file_names.begin(), file_names.end());
}

bool iis_source::enable() {
    bool result = false;
    switch (mode) {
        case IIS_SOURCE_MODE_READ:
            /* Need room on the output FIFO and at least one file left. */
            result = (welt_c_fifo_population(out) 
                      welt_c_fifo_capacity(out)) &&
                     (file_index < (int)file_names.size());
            break;
        case IIS_SOURCE_MODE_INACTIVE:
            result = false;
            break;
        default:
            result = false;
            break;
    }
    return result;
}

void iis_source::invoke() {
    switch (mode) {
        case IIS_SOURCE_MODE_READ: {
            /* Build full path to next file. */
            string full_path = dataset_dir + "/" + file_names[file_index];

            ifstream fin(full_path.c_str());
            if (!fin.is_open()) {
                cerr << "iis_source error: could not open file '"
                     << full_path << "'" << endl;
                file_index++;
                /* If that was the last file, switch to INACTIVE. */
                if (file_index >= (int)file_names.size()) {
                    mode = IIS_SOURCE_MODE_INACTIVE;
                }
                break;
            }

            /* Parse the comma-separated text image into a 2D int grid. */
            if (grid_ptr != nullptr) {
                delete grid_ptr;
                grid_ptr = nullptr;
            }
            grid_ptr = new vector<vector<int>>();
            grid_ptr->reserve(nRows);

            string line;
            while (getline(fin, line)) {
                if (line.empty()) continue;
                vector<int> row;
                row.reserve(nCols);
                stringstream ss(line);
                string cell;
                while (getline(ss, cell, ',')) {
                    row.push_back(atoi(cell.c_str()));
                }
                grid_ptr->push_back(row);
            }
            fin.close();

            /* Sanity-check dimensions. */
            if ((int)grid_ptr->size() != nRows) {
                cerr << "iis_source warning: file '" << full_path
                     << "' has " << grid_ptr->size()
                     << " rows, expected " << nRows << endl;
            }

            /* Build a cv::Mat from the parsed grid (CV_8U grayscale).
               This is heap-allocated; downstream actors / sink are
               responsible for delete-ing it. */
            cv::Mat* img = new cv::Mat(nRows, nCols, CV_8UC1);
            for (int r = 0; r < nRows && r < (int)grid_ptr->size(); r++) {
                const vector<int>& row = (*grid_ptr)[r];
                for (int c = 0; c < nCols && c < (int)row.size(); c++) {
                    img->at<unsigned char>(r, c) =
                        (unsigned char)row[c];
                }
            }

            /* Wrap into an IIS with k = file_index + 1 (1-based). */
            IIS* token = new IIS();
            token->I = img;
            token->k = file_index + 1;

            /* Write the IIS pointer onto the output FIFO. */
            welt_c_fifo_write(out, &token);

            /* Advance to next file. */
            file_index++;
            if (file_index >= (int)file_names.size()) {
                mode = IIS_SOURCE_MODE_INACTIVE;
            }

            /* Free the temp grid buffer; the cv::Mat holds the pixels now. */
            delete grid_ptr;
            grid_ptr = nullptr;
            break;
        }
        case IIS_SOURCE_MODE_INACTIVE:
            /* No-op. */
            break;
        default:
            break;
    }
}

void iis_source::reset() {
    file_index = 0;
    if (grid_ptr != nullptr) {
        delete grid_ptr;
        grid_ptr = nullptr;
    }
    mode = file_names.empty() ? IIS_SOURCE_MODE_INACTIVE
                              : IIS_SOURCE_MODE_READ;
}

void iis_source::connect(welt_cpp_graph* graph) {
    int direction  = GRAPH_OUT_CONN_DIRECTION;
    int port_index = 0;
    graph->add_connection(this, port_index, direction);
}