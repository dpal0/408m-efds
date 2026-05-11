#!/bin/bash

for dir in test_*/; do
    rm -f "$dir"face.txt \
          "$dir"non-face.txt \
          "$dir"temp_face.txt \
          "$dir"temp_non-face.txt \
          "$dir"nf*.txt \
	  "$DIR"performance*.txt
done
