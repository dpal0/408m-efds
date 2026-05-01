#ifndef IIS_SOURCE_H
#define IIS_SOURCE_H

#include <string>
#include <vector>
#include "welt_cpp_actor.h"
#include "welt_c_fifo.h"
#include "welt_cpp_graph.h"
#include "iis.h"

#define IIS_SOURCE_MODE_PROCESS  0
#define IIS_SOURCE_MODE_COMPLETE 1
#define IIS_SOURCE_MODE_ERROR    2

class iis_source : public welt_cpp_actor {
public:
    iis_source(welt_c_fifo_pointer out_fifo, const char *dir_name);
    ~iis_source();
    bool enable();
    void invoke();
    void reset();
    void connect(welt_cpp_graph *graph);

private:
    welt_c_fifo_pointer out;
    std::string dir_name;
    std::vector<std::string> file_list;  // full paths, sorted
    size_t next_index;                    // index into file_list of next file to read
    int next_k;                           // 1-based image index for IIS

    // Helper: parse one txt file into a newly allocated PTIS. Returns nullptr on error.
    PTIS *load_ptis(const std::string &path);
};

#endif