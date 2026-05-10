EFDS - Embedded Face Detection System
======================================

This is the main deliverable for the ENEE 408M course project. It implements
the Viola-Jones face detection algorithm as a dataflow graph designed to run
on a Raspberry Pi. The system takes in image sub-windows, runs them through a
cascade of classifiers, and outputs whether each sub-window is a face or not.

efds/training produces config files that are stored in efds/configs that tell the EFDS how to classify

This project is built with CMake and cross-compiled for the Raspberry Pi using
the ARM toolchain. It also compiles natively on the course server
(enee408m.ece.umd.edu) for testing.


How it works
------------
Images come in as sub-windows through a source actor. Each sub-window passes
through a chain of classifier actors — one per cascade stage. At each stage,
the classifier either rejects the sub-window as a non-face (abort path) or
passes it forward to the next stage (continue path). Only sub-windows that
make it through every stage are considered faces. This early-rejection design
is what makes Viola-Jones fast enough to run on embedded hardware.

The number of classifier stages is determined automatically at runtime by how
many config files are present in the config directory.


Directory Structure
-------------------

src/
    All source code for the dataflow graph and actors and CMakeLists to build the 
    actors and tests as well.
tests/
    Contains unit tests for the classifier actor, source and sink actors and train runs and the 
    graph using different config files.  

demo/
    A full end-to-end demo script. Run this to see the system detect faces on
    a complete input. Good for showing the system working on the Pi.

CMakeLists.txt
    Top-level CMake config. Sets up include paths for the Welter C++ framework
    headers and pulls in the src/ subdirectory build.

drivers/
	Where the drivers for unit testing, evaluation, and train runs live.

Build Instructions
------------------
From the efds/src directory:

    ./buildsetup      # run once to configure CMake (first time only)
    ./makeme          # compiles, builds libefds.a, and installs test binaries

For cross-compilation targeting the Raspberry Pi, use the ARM toolchain as
configured in the course DICE environment.


Dependencies
------------
- Welter C++ framework (welt_cpp_actor, welt_cpp_graph, welt_c_fifo, etc.)
  Headers: $UXWELTER and $UXWELTERC
  Libraries: $WELTERGEN
- CMake
- ARM cross-compilation toolchain (for Pi deployment)
- DICE toolchain on enee408m.ece.umd.edu
