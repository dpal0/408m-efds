dir="../../dataset"

rm -r $dir/gray
cp -r $dir/orig $dir/gray

cd $dir/gray

for outerDir in "training" "testing" "validation"; do
	cd $outerDir
	for innerDir in "face" "non-face"; do
		cd $innerDir
		for file in *.png; do
			enee408mim2txt $file grayscale
		done
		rm *.png
		cd ..
	done
	cd ..
done


