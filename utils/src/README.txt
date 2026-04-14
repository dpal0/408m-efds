This directory contains scripts that should be run to pre process the dataset. 
They should be run once when the dataset is first imported and should not need 
to be run again. They are run in this order:

./splitData.sh
./rgbToGray.sh
./integralImage.sh

The splitData.sh splits the training dataset proportionally so that the dataset
is 60% training, 20% validation, and 20% testing.

The rgbToGray.sh copies the entire dataset into a new subdirectory called 
gray/. It then converts every image from RGB to grayscale.

The integralImage.sh copies the entire dataset inot a new subdirectory called
integral/. It then converts every image from grayscale to integral image.
