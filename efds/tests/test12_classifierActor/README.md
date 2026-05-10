
Copyright (c) 1997-2023
Maryland DSPCAD Research Group, The University of Maryland at College Park 

Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the above
copyright notice and the following two paragraphs appear in all copies
of this software.

IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.


@author Erica Zhang

# classActor_driver
This driver generates multiple classifier actors using the configs/config#.txt in this test directory. For each generated classifier actor, it will generate vector images by reading the face images in face/000#.txt and non-face images in non-face/000#.txt files. Then, each generated classifier actor will be used to classify the face and non-face files and return true if face is detected and false if not face is detected. This detection is used to print whether the current image that was classified to be either face or not face. After this current classifier actor stage is done with reading all the images, it gives a summary of the number of faces and non-faces that passed or failed. 
CMakeLists.txt, build, and runme are used to compile and test the output values.
dxtest can be used to test this unit test. 
# Example Output from driver
```txt
_________Test Config0____________
Face 1 detected as Face
Face 2 detected as Face
Face 3 detected as Face
Face 4 detected as Face
NonFace 1 detected as NonFace
NonFace 2 detected as Face
NonFace 3 detected as Face
NonFace 4 detected as NonFace
Config 0  ____Summary____
all faces passed
number of nonfaces failed: 2
_________Test Config1____________
Face 1 detected as Face
Face 2 detected as Face
Face 3 detected as Face
Face 4 detected as Face
NonFace 1 detected as Face
NonFace 2 detected as Face
NonFace 3 detected as Face
NonFace 4 detected as Face
Config 1  ____Summary____
all faces passed
number of nonfaces failed: 4
```
# How to Run dxtest 
1st: rm -rf autotest*
2nd: dxtest

# Makeme
makeme  configures and builds the test files using cmake and make. After makeme is completed, the executable file classActor_driver is created inside build directory. 

# Runme
runme is used run the executable classActor_driver. 