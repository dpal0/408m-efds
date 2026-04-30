This is team 4’s facial detection project. There are two main modules: 

(1) training/ module. This module constructs the machine learning algorithm used for facial detection. The Viola-Jones Algorithm was used. This algorithm uses an image’s integral representation, parses the image into Haar-like features, and classifies the image using a linear combination of weighted weak and strong classifiers.

(2) efds/ module. This module contains the code for running the algorithm in real-time. It can be run in the native environment or in the cross-compilation platform (Raspberry Pi).

There are two more directories in this project overview:

(3) dataset/ directory contains the complete set of image files used to train and validate the training module and to test the efds module.

(4) configs/ directory contains the configuration files. These files are the link between the training and efds modules. Each file represents a strong classifier that was computed by the training module and will be used in the efds module.
