# configs

This directory contains all the trained models produced by the training module to be used in the efds module. Each training model contains configuration files, an evaluation report and a training report. The configuration files are the link between the training and efds modules. 

This directory should contain the subdirectories to each iteration of the training module. Any new subdirectory is labeled by a timestamp ensuring that no iterations get overwritten and must be rewritten to its desired directory name.

# config file

In terms of the configuration files, one iteration of the Viola-Jones algorithm can contain multiple strong classifiers. Each strong classifier will be contained in separate config files labeled config##.txt. Each config file represents one strong classifier and has the following format:

Line 1: strong classifier threshold
Line 2-end: 1 weak classifier per line. Each line contains:
- haar feature x
- haar feature y
- haar feature width
- haar feature height
- haar feature type
- weak classifier threshold
- weak classifier polarity
- haar feature weight
- weak classifier weight

# evaluation report

In terms of the evaluation report, each line represents the metrics for one cascade of the model. Each line has the following format:
- True Positive (int)
- False Positive (int)
- True Negative (int)
- False Negative (int)
- True Positive Rate (double)
- False Positive Rate (double)
- Precision (double)
- Recall (double)
- F1 Score (double)
- time to run in nanoseconds (double)

# training report

In terms of the training report, this is a human readable textfile contains:
- the number of strong classifiers
- the number of weak classifiers per strong classifier
- the total training time
- the timestamp when training occured
