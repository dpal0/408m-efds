This the main testing directory for the efds

Here we did test runs with different config files and generated eval reports

It contains three sub directories for unit testing of the graph, source actor and sink actor.

The rest of them are train test runs

----

test04_train5 to test10_train4

These tests use different training components to classify the images.

For example: 

1. test04_train5 uses configs/train5/config#.txt files to create a cascade graph with classifier Actors from stages 0 to 4 to classify the images in project-dataset/gray/testing/face/ and project-dataset/gray/testing/non-face/ files.

2. efds_graph_driver is ran and outputs a face.txt and non-face.txt file which contains the classfication of the images in the project-dataset/gray/testing/face/ and project-dataset/gray/testing/non-face/ files. Where the output classification from the graph for classifying the images in project-dataset/gray/testing/face/ are stored in face.txt and simiarly with non-face.txt.

Since some of the images are aborted and classfied early compared to others because it is classified as non-face, then it may appear earlier and out of order in the output files in face.txt and non-face.txt. Therefore, these files must be sorted before running evalTrain_driver to obtain the evaluation metrics of the output face.txt and non-face.txt files.

test04_train5 to test10_train4 have the same functionality, but they use different config files to create their classifier actors and graph. All of these tests uses runme_face, runme_noface, and runme_eval to generate face.txt, non-face.txt, and evalEfds.txt.

runme_face -> generate face.txt
runme_noface -> generate non-face.txt
runme_eval -> generate face.txt, non-face.txt, and evalEfds.txt

Each test has face.txt, non-face.txt, and evalEfds.txt generated. These generated files can also be found in configs/train#/ folder which are named: outputFace.txt, outputNonFace.txt, evalEfds.txt.
