#!/usr/bin/env bash

dir="../project-dataset"

rm -r $dir/integral
cp -r $dir/gray $dir/integral

g++ IntegralImage.cpp -o IntegralImage.exe

for file in $dir/integral/*/*/*.txt; do
    ./IntegralImage.exe $file
done

rm IntegralImage.exe
