This directory contains the demo for the Embedded Face Detection System (EFDS).
It includes the cross-compiled executable (efds_graph_driver.exe) built for the
Raspberry Pi (ARM target), test datasets, per-stage accuracy summaries, and
scripts to run the demo on the Pi.


Directory Contents
------------------
 
efds_graph_driver.exe
    Cross-compiled ARM binary of the EFDS graph driver. Built on
    enee408m.ece.umd.edu using the Raspberry Pi toolchain
    (Toolchain-rpi.cmake, arm-linux-gnueabihf-g++). Transfer this file
    to the Raspberry Pi before running.
 
runme
    Shell script to run the EFDS on the host server (enee408m.ece.umd.edu)
    using a native build. Uses test_data_mixed/ as input and Per_stage_summary/
    configs by default.
 
    Usage:
        ./runme
 
runme_latency_pi.sh
    Shell script intended to run on the Raspberry Pi. Invokes
    efds_graph_driver.exe with the test dataset and config directory,
    and captures timing output (Tpos, Npos, avg latency/face) for per stage latency benchmarking.
 
    Usage (on the Raspberry Pi):
        ./runme_latency_pi.sh <input dir> <config dir>
 
 
test_data_mixed/
    Input dataset of 24x24 pixel sub-window .txt files (both face and
    non-face samples) used to exercise the EFDS end-to-end and debug. Each file
    represents one sub-window in the comma-separated intensity format
    expected by iis_source.
    F: 1, 3, 5, 7
    NF: 2, 4, 6, 8
 
Per_stage_summary/
    Contains one classifier config file per cascade stage (e.g., stage0.txt,
    stage1.txt, ...). These are the output of the training module and are
    read by classifierActor in CONFIGURE mode. The number of config files
    determines the number of cascade stages N at runtime.
 
    Config file format (one weak classifier per line):
        <feature_type> <x> <y> <w> <h> <polarity> <threshold> <alpha>
        ...
        <strong classifier threshold>
This is where you can run the graph with any input directory and change the configuration file and see the outputs of the graph.
data set:

