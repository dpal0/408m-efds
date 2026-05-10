EFDS Tests
==========

This directory contains the test suite and evaluation runs for the EFDS.
Executables are built and installed here via makeme in efds/src/.

Directory Structure
-------------------

ITS Suites (run with dxtest)
    These follow the standard DICE ITS convention with runme scripts
    and can be exercised using the dxtest utility.

    Source_and_sink_tests/
        ITS suite for the iis_source and iis_sink actors.
        Driver: test_source_sink.exe

    Classifier_actor_tests/
        ITS suite for the classifierActor.
        Driver: classifier_actor_driver.exe

    Graph_tests/
        ITS suite for the full EFDS graph pipeline.
        Driver: efds_graph_driver.exe

    test_evalMetrics/ and test-utils-evalmetrics/
        ITS suite for the EvalMetrics utility.
        Driver: evalTrain_driver.exe

Evaluation Runs (not ITS)
    These are design space exploration runs using different classifier
    configurations. Each has a runme_eval script instead of a runme,
    and is not intended to be exercised with dxtest.

    test_train0/ - test_train6/
        Evaluation runs with different config files to explore
        trade-offs in detection rate, false positive rate, and
        processing speed.

    train_narrow/
        Evaluation run with a narrow (more restrictive) cascade config.

    train_normal/
        Evaluation run with a normal cascade config.

Installed Executables
---------------------

    test_source_sink.exe       - used by Source_and_sink_tests/
    classifier_actor_driver.exe - used by Classifier_actor_tests/  (in Classifier_actor_tests/)
    efds_graph_driver.exe      - used by Graph_tests/ and evaluation runs
    evalTrain_driver.exe       - used by test_evalMetrics/

Utilities
---------

    cleanup.sh
        Removes generated output files (face.txt, non-face.txt,
        temp_face.txt, temp_non-face.txt, nf*.txt) from all test_*/
        subdirectories. Run from this directory:

            ./cleanup.sh
