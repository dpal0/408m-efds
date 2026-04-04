The dataset in this directory is based on a copy of the MIT CBCL dataset that
was obtained on March 21, 2021 from
https://github.com/paveyry/FaceDetection/tree/master/learning-tests 

The dataset version here is a reorganized version of the MIT CBCL dataset,
where the images are partitioned into subsets for training and testing.  In
particular, 1944 face images and 3639 non-face images are provided as training
samples, and 485 face images and 909 non-face images are provided as testing
samples. 

To partition the original dataset into training and testing subsets, the
following command was used to randomly select files from the current working
directory:

ls | shuf -n  <ARG1>  | xargs -i mv {} <ARG2>

where <ARG1> denotes the number of files to be selected (an integer), and
<ARG2>  denotes the directory path in which to store the selected files.

Each image is already converted to be of size 24 x 24 pixels (this is how the
images in the original dataset are provided). The images are in PNG format,
meaning that all of the images are RGB images.

More details on the MIT CBCL dataset can be found here:
http://www.ai.mit.edu/projects/cbcl.old/software-datasets/FaceData2.html

Release created on Tue Mar 31 23:58:22 EDT 2026
