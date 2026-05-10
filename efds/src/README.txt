This directory contains all source code for the Embedded Face Detection System.
It is organized into three subdirectories: actors/, graph/, and evaluation/.


Usage
-----

From this directory (efds/src/):

    ./buildsetup      # first-time CMake setup
    ./makeme          # compile and install



Subdirectories
--------------

actors/
    Contains the dataflow actors and supporting headers.

    iis_source.h/.cpp       - Source actor. Reads 24x24 pixel window .txt files
                              from a directory and emits IIS tokens into the pipeline.
    iis_sink.h/.cpp         - Sink actor. Receives IIS tokens and writes results
                              (face or non-face) to an output file.
    classifierActor.h/.cpp  - Cascade classifier actor. One instance per stage.
                              Reads a config file, classifies an image window using
                              Haar features and a strong classifier, and routes the
                              token to either cont_out (face) or abort_out (non-face).
    integral_image.h/.cpp   - Computes the integral image in-place on a 2D pixel
                              vector. Used by classifierActor during classification.
    test_source_sink.cpp    - Standalone test for the source and sink actors.

graph/
    Contains the graph definition and the main driver executable.

    efds_graph.h/.cpp       - Defines efds_graph, which wires together the source
                              actor, N classifier actors (one per config file found
                              in the config directory), and the face/non-face sinks.
                              The number of stages is determined at runtime.
    efds_graph_driver.cpp   - main() entry point. Constructs the graph, runs the
                              scheduler, and prints timing and face-count stats.
    runme                   - Convenience shell script to invoke efds_graph_driver.exe
                              with default dataset and config paths.
    face.txt / nonface_stage*.txt
                            - Sample output files from a previous run.

evaluation/
    EvalMetrics.h/.cpp      - Reads a face output file and computes detection metrics:
                              Recall, Precision, F1-Score, and FPR against a ground
                              truth vector.


Top-level Files
---------------

iis.h           - Defines the IIS token: a PTIS* image pointer and int sequence index.
ptis.h          - Defines the PTIS struct representing a raw 24x24 pixel sub-window.
CMakeLists.txt  - Build config. Builds libefds.a from all actor and graph sources,
                  and compiles efds_graph_driver.exe and test_source_sink.exe.
buildsetup      - Run once to configure CMake (first time only).
makeme          - Compiles, builds libefds.a, and installs executables.


Output File Format
------------------

Face and non-face log files use the same format:

    1 F
    2 NF
    3 F

Each line is: <sequence_index> <F|NF>
This format is consumed directly by EvalMetrics for performance evaluation.
