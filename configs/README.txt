This directory contains all the configuration files produced by the training module to be used in the efds module. The configuration files are the link between the training and efds modules. 

This directory should contain the subdirectories to each iteration of the training module. Each iteration's subdirectory is labeled by a timestamp ensuring that no iterations get overwritten. 

One iteration of the Viola-Jones algorithm can contain multiple strong classifiers. Each strong classifier will be contained in separate config files labeled config##.txt.

Each config file represents one strong classifier and has the following format:

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
