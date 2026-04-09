#!/usr/bin/env bash

dir="../../dataset"
rm -r $dir/integral
cp -r $dir/gray $dir/integral

g++ integralImage.cpp -o integralImage.exe

for file in $dir/integral/*/*/*.txt; do
	./integralImage.exe $file
done

rm integralImage.exe
