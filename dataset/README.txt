Producing the Dataset:

To produce this dataset, the scripts used are in the scripts/ subdirectory with instructions on how to run in the scripts/README.txt file. 

Contents:

the project-dataset/ contains all the images (face and non-face) to be used within this project. There are three representations of each image:

(1) The original image in a *.png file. These images are stored in the orig/ subdirectory.
(2) The grayscale textual representation in a *.txt file. These images are stored in the gray/ subdirectory.
(3) The integral image textual representation in a *.txt file. These images are stored in the integral/ subdirectory.

Each subdirectory contains the exact same layout. This layout is as follows:

(1) training/ subdirectory contains 60% of the images and is used for training the model.
(2) validation/ subdirectory contains 20% of the images and is used for fine-tuning the model.
(3) testing/ subdirectory contains 20% of the images and is used for running real-time tests on the EFDS module.

Furthermore, within each of those subdirectories, there are two more subdirectories:

(1) face/ subdirectory contains each individual image file that contains a face. The images’ representation follows the outer directory categories (i.e. original image, grayscale text, integral text).
(2) non-face/ subdirectory contains each individual image file that does not contain a face. As with the face/ subdirectory, the images’ representation follows the outer directory categories.

Usage:

For the training module, the integral textual images are utilized (dataset/project-dataset/integral/*).
For the EFDS module, the grayscale textual images are utilized (dataset/project-dataset/gray/*).
