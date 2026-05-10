EFDS Source
===========

This directory contains all source code for the Embedded Face Detection System (EFDS),
implemented as a dataflow graph using the Welter-C++ actor framework.

Directories
-----------

actors/
    Dataflow actor implementations: iis_source, iis_sink, classifierActor,
    and integral_image. Also contains shared token type headers iis.h and
    ptis.h, which define the data structures passed between actors on FIFOs.

graph/
    Graph definition (efds_graph.h/.cpp) that wires together the source,
    N classifier actors, and sink actors into a full cascade pipeline.

drivers/
    Standalone driver executables for testing and evaluation:
    test_source_sink, classifier_actor_driver, efds_graph_driver,
    and evalTrain_driver.

evaluation/
    EvalMetrics.h/.cpp for computing detection metrics (Recall, Precision,
    F1-Score, FPR) against ground truth.

What Gets Built
---------------

libefds.a
    Static library containing all actor and graph object code.
    Installed to efds/

test_source_sink.exe
    Installed to efds/tests/Source_and_sink_tests/

classifier_actor_driver.exe
    Installed to efds/tests/Classifier_actor_tests/

efds_graph_driver.exe
    Installed to efds/tests/ and efds/demo/

evalTrain_driver.exe
    Installed to efds/tests/

Dependencies
------------

The build requires the Welter-C++ framework. The following environment
variables must be set before building (configured via source .dspcadrc-408m):

    UXWELTER  - Welter C++ header directory
    UXWELTERC - Welter C header directory
    WELTERGEN - Welter compiled library directory

Usage
-----

From this directory (efds/src/):

    ./buildsetup      # Run once to configure CMake (first time only)
    ./makeme          # Compile, build libefds.a, and install executables

To run the test suite after building, use dxtest from the efds/ directory.
