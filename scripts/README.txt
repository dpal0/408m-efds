

run_efds.sh
-----------
Runs the EFDS graph driver on a single input directory using a given config
and writes the classification results to an output directory.

Usage:
  ./run_efds.sh <input_dir> <config_dir> <output_dir>

Arguments:
  input_dir   Directory of grayscale images to classify (e.g. testing/face)
  config_dir  Config directory containing config0.txt, config1.txt, ... files
  output_dir  Directory where results will be written (created if missing)

Output files (written to output_dir):
  face.txt        Sorted classification results for all input images
  performance.txt Driver performance stats (timing, counts)
  nf_stage#.txt   Images aborted at nf_stage#

Example:
  ./run_efds.sh ../dataset/project-dataset/gray/testing/face \
               ../configs/train0 \
               ./output/train0
