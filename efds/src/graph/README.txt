Running the Driver
------------------

    efds_graph_driver.exe <input_dir> <face_out_file> <config_dir>

    input_dir       Directory of 24x24 .txt image window files to classify.
    face_out_file   File where face detections are logged.
    config_dir      Directory with one .txt config file per classifier stage.

Non-face logs are written to nonface_stage0.txt, nonface_stage1.txt, etc.
in the current working directory.

Usage (also in graph/runme):

./efds_graph_driver.exe ../../../dataset/project-dataset/gray/validation/face/ face.txt ../../../configs/train5

One thing to take care of before runing the grpah on given dataset is to properly set up the data
