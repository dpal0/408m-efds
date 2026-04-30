#ifndef IIS_SOURCE_H
#define IIS_SOURCE_H

extern "C" {
#include "welt_c_fifo.h"
#include "welt_c_util.h"
}

#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "classifierActor.h"   /* for the IIS struct definition */

//#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#define IIS_SOURCE_MODE_READ     1
#define IIS_SOURCE_MODE_INACTIVE 2

/* Source actor that reads grayscale image text files from a directory
   and emits them as IIS* tokens onto its output FIFO.

   File format: same as enee408mtxt2im / enee408mim2txt — comma-separated
   integer pixel values, one image row per line.

   Files in the directory are read in lexicographic order. The first file
   produces an IIS with k=1, the second k=2, and so on. */
class iis_source : public welt_cpp_actor {
public:
    iis_source(welt_c_fifo_pointer out_fifo,
               const char* dataset_dir,
               int nRows, int nCols);
    ~iis_source() override;

    bool enable() override;
    void invoke() override;
    void reset() override;
    void connect(welt_cpp_graph* graph) override;

private:
    /* Output FIFO. */
    welt_c_fifo_pointer out;

    /* Dataset directory and the list of files within it that we will read. */
    std::string dataset_dir;
    std::vector<std::string> file_names;

    /* Image dimensions (assumed uniform across the dataset). */
    int nRows;
    int nCols;

    /* Index into file_names for the next file to read. Doubles as the
       k value once incremented (k = file_index + 1 after a read). */
    int file_index;

    /* Internal buffer used while parsing a text image. */
    std::vector<std::vector<int>>* grid_ptr;

    /* Actor mode: READ or INACTIVE. */
    int mode;

    /* Helper: populate file_names with the .txt files found in dataset_dir,
       sorted lexicographically. Called once during construction. */
    void scan_directory();
};

#endif