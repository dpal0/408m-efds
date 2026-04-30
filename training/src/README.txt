This directory contains the source code for the training module. It includes: 

(1) HaarFeature.h: creates and calculates the Haar (rectangle) Features values.

(2) WeakClassifier.h: Trains integral image on all HaarFeatures to determine the best Haar Feature, threshold, polarity, and weight.  

(3) StrongClassifier.h: Trains integral image with weak classifiers to determine the best linear combination of weak classifiers.

(4) main.cpp: Loads in images for training, trains strong classifiers (the number of weak classifiers per strong classifier and the number of strong classifiers are set by the developer), and returns the resulting config files.

(5) makeme: Builds the cpp files into an executable.

(6) runme: Runs the training module. Arguments are specified within the runme script and should be modified depending on the desired inputs.
