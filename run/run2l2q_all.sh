# any tree that already appears in 2l2qSelectedTrees will not be generated again, if the you want to redo the tree, please delete first!

cp ../scripts/all.c .
cp ../scripts/run2l2q_Reso.sh .
cp ../scripts/run2l2q_Templ.sh .

if [ ! -d "2l2qSelectedTrees" ]; then 
  mkdir 2l2qSelectedTrees
fi


### Custom selection ###
#when sample type is "Custom", you can insert any input file, the output selected file is 2l2qtree_testSample.root
<<comment
root -l -n -b -q all.c\(Custom,\"/afs/cern.ch/work/c/cayou/public/80Xsamples/2l2qsamples_1010/VBFHiggs500/ZZ2l2qAnalysis.root\",\"sel\"\)
comment

### Efficiency ###
<<comment
if [ ! -d "2l2qEfficiency" ]; then
  mkdir 2l2qEfficiency
fi

for model in spin0_VBF spin0_ggH #spin2 
do
echo "running $model efficiency"
root -l -n -b -q all.c\(${model},\"\",\"eff\"\)
done

mv efficiency*.* 2l2qEfficiency
mv 2l2qtree*.root 2l2qSelectedTrees
comment

### 2D Templates ###
#make selected trees as input for smoothing, for spin0 ggH and VBF are combined into spin0_all
for model in spin0_all DYjets TTBar Diboson data #spin2
do
echo "running $model trees"
root -l -n -b -q all.c\(${model},\"\",\"sel\"\)
done
mv 2l2qtree*.root 2l2qSelectedTrees

#smoothing and normalization
#spin-2 part is commented out in run2l2q_Templ.sh, uncomment when needed
echo "running templates"
bash run2l2q_Templ.sh

### Resolution ###
#currently using only spin-0 resolution for both spin-0 and spin-2
<<comment
for model in spin0_all 
do
echo "running $model resolution"
bash run2l2q_Reso.sh ${model}
mv 2l2qResolution_${model}/2l2qtree*.root 2l2qSelectedTrees
done

echo "Done! Deleting.."
rm all.c run2l2q_Reso.sh run2l2q_Templ.sh
comment
