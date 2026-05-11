# training/src

(1) HaarFeature.h: creates and calculates the Haar (rectangle) Features values.

(2) WeakClassifier.h: Trains integral image on all HaarFeatures to determine the best Haar Feature, threshold, polarity, and weight.  

(3) StrongClassifier.h: Trains integral image with weak classifiers to determine the best linear combination of weak classifiers.

(4) main.cpp: Loads in images for training, trains strong classifiers (the number of weak classifiers per strong classifier and the number of strong classifiers are set by the developer), and returns the resulting config files.

# subdirectories

(1) train: This directory contains scripts to train the classifiers. It was used to created multiple config files quickly.

(2) validate : This directory contains scripts to check the accuracy of the classifiers on given dataset.
