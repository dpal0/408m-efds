#!/usr/bin/env bash

dir_pd="../../project-dataset"
dir="../../dataset"

mv $dir_pd $dir

mkdir $dir/orig
mv $dir/testing $dir/orig
mv $dir/training $dir/orig

dir="$dir/orig"
mkdir $dir/validation
mkdir $dir/validation/face
mkdir $dir/validation/non-face

for ((i=1458; i<=1944; i++)); do
	origFile="$dir/training/face/$i.png"
	printf -v offset "%04d" "$((i-1457))"
	newFile="$dir/validation/face/$offset.png"
	mv "$origFile" "$newFile"
done

for ((i=2793; i<=3639; i++)); do
	origFile="$dir/training/non-face/$i.png"
	printf -v offset "%04d" "$((i-2792))"
	newFile="$dir/validation/non-face/$offset.png"
	mv "$origFile" "$newFile"
done
