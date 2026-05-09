
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


@Author Erica Zhang

# Classifier Actor README
## What it Does
classifierActor is an actor of the cascade graph pipeline. This actor classifies image windows as either face or non-face based on the strong classifier that is built from weak classifiers and Haar features. 
This actor takes in an image from input FIFO and outputs to abort_cont FIFO or cont_out FIFO. "abort_out" FIFO is used when image is detected as not face. "cont_out" FIFO is used when image is detected as face. 

## Actor Modes
The actor has CONFIGURE, READ, CLASSIFY, TRUE_MOD, and FALSE_MOD modes.
### CONFIGURE mode
This mode reads from a config file. It reads one strong classifier threshold, Haar Feature values which include x and y position, width (w), height (h), and type of feature, and weak classifier values including weak classifier threshold, polarity, and weight of the error. 
Multiple weak classifiers are created from config file and stored in a vector called wc_vector, which is used to create one strong classifier called "sc". "sc" is later used in CLASSIFY mode. 
Goes to READ mode. 
### READ mode
This mode reads in the token (I,k) from the input FIFO. Then it checks if token is NULL or valid. If NULL, no action will be done. Otherwise, this input token is stored in currToken which will later be used in CLASSIFY, TRUE_MOD or FALSE_MOD. 
Goes to CLASSIFY mode.
### CLASSIFY mode
This mode checks if the "I" or image in the token (I,k) is valid or not. If the image is valid, it converts the image into a integral image. Then it checks if "sc" is valid or not. If it is valid, this function will using "sc" to classify the generated integral image. Strong classifier's classify method returns a boolean value which returns true if the integral image is a face. Otherwise, it will return false. 
If the integral image is a face, mode will be set to TRUE_MOD. Otherwise mode will be set to FALSE_MOD. 
If "sc" or token are invalid program breaks or returns. 
### TRUE_MOD mode
This is when classfied integral image is a face. Checks if currToken is valid or not. If it is invalid, it returns. Otherwise, this function creates a new token and copies the currToken into the new token called copyToken. Then it will write to the cont_out with that copyToken, which will be sent to the next classifierActor stage in the cascade. 
Mode goes to READ. 
### FALSE_MOD mode
This is when classfied integral image is not a face. Checks if currToken is valid or not. If it is invalid, it returns. Otherwise, this function creates a new token and copies the currToken into the new token called copyToken. Then it will write to the abort_out with that copyToken. 
Mode goes to READ. 

## Token 
Stores IIS* tokens. Where I is the image window and k is the image index.

## integral_image.cpp
Takes in a vector<vector<int>> image which will be used to calculate the integral image. Nothing is returned, but values are changed in the input image. 
