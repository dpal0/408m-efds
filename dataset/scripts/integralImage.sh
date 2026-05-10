#!/usr/bin/env bash

dir="../project-dataset"

rm -r $dir/integral
cp -r $dir/gray $dir/integral

g++ -std=c++11 IntegralImage.cpp -o IntegralImage.exe #diksha added due to compilation errors 

for file in $dir/integral/*/*/*.txt; do
    ./IntegralImage.exe $file
done

rm IntegralImage.exe
