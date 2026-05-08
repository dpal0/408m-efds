
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

# evalMetric_driver
This driver tests the EvalMetrics.cpp with multiple edge cases using the test#.txt files shown in the tests/test11_EvalMetric/ directory. EvalMetric's calculateCascade() function takes in an output.txt file which is the file that is generated from the cascade graph after multiple images are classfied. Output.txt file looks something like this:
```txt
1 F
2 NF
3 NF
4 F
```
test#.txt files in this directory represents the output.txt files which will be put into the calculateCascade to determine true positive, true negative, false positive, false negative values, recall, precision, F1-score, and FPR values.
test1.txt = shows normal output file from graph
test2.txt = shows all classification matches ground truth from graph
test3.txt = shows all classification does not match ground truth from graph
test4.txt = shows mixed order of classification from graph for example:
```txt
1 F
4 F
2 NF
3 NF
```
where 4 F is not supposed to be in line 2 but it should be in line 4 and same with the 2 NF and 3 NF which are not in correct order.
test5.txt = shows incorrect classification from the graph. Correct classification is either F or NF. So incorrect classification would be any other strings that are not F or NF. 
test6.txt = shows missing outputs from the graph. For example, if an 4 images are sent through the graph, then output.txt file should include 4 classifications. So this tests when instead of 4 classficiations, we get less than 4 classifications. 
test7.txt = shows duplicate outputs from the graph. 
test8.txt = shows when ground truth is all true or all faces. 
test8.txt = shows when ground truth is all false or all non faces. 

# runme
runme is used to compile and execute the evalMetric_driver. It stores its output in the diagnostics.txt. This will be used to compare the actual output when using dxtest. 


