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

# Evaluation Metrics
This is used to evaluate true positives, true negatives, false positives, false negatives, TPR/Recall value, Precision value, FPR, and F1-score value of an output.txt file. 
"calculateCascade" method takes in a output.txt file and reads each line and its classification to check whether the ouput.txt classification is correct or not when compared to the ground truth vector's value. From the comparison, the true positive, true negatives, false positives, and false negatives are incremented. After obtaining the total of the true positive, true negatives, false positives, and false negatives, Recall, precision,F1-score, and FPR values are calculated based on the formulas: 
Recall=TP/(TP+FN)
Precision=TP/(TP+FP)
F1-Score=2*(Precision*Recall)/(Precision+Recall)
FPR=FP/(FP+TN)
## Output.txt file format
Here is an example of Output.txt file format:
```txt
1 F
2 NF
3 NF
4 F
```
## groundTruth()
Is a method that creates a boolean ground truth array based on the number of face and non-face images. 
Where true=face image, false=non-face image. 
First part of the array stores face labels and the last part of the array stores non-face labels. 

