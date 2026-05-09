This directory provides a quick end-to-end demonstration of the EFDS pipeline.
It contains a single convenience script (runme) that runs the compiled classifier
against the face validation subset of the MIT CBCL dataset.


Usage
-------------

1. Build the project from efds/src/:

       ./buildsetup      # first-time CMake setup only
       ./makeme          # compile and install

   After a successful build, efds_graph_driver.exe will be installed into this
   directory (efds/src/demo/).

2. The following paths must exist relative to the repo root (three levels up
   from this directory):

       dataset/project-dataset/gray/validation/face/   # input image windows
       configs/train5/                                  # classifier stage configs

   These are not included in the repository. Obtain the dataset and trained
   config files separately and place them at those paths.


Running the Demo
----------------

From this directory

    ./runme


Output
------

face.txt
    One line per image window classified as a face:

        <sequence_index> F

nonface_stage<N>.txt
    One file per classifier stage, logging windows rejected at that stage:

        <sequence_index> NF

Timing and face-count statistics are printed to stdout on completion.

These output files can be passed directly to EvalMetrics (see
efds/src/evaluation/) to compute Recall, Precision, F1-Score, and FPR.
