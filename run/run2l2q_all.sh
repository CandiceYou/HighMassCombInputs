cp ../scripts/all.c .
cp ../scripts/run2l2q_Reso.sh .
cp ../scripts/run2l2q_Templ.sh .

if [ ! -d "2l2qSelectedTrees" ]; then 
  mkdir 2l2qSelectedTrees
fi


### Custom selection ###
#<<comment
for model in Custom
do
echo "running $model efficiency"
root -l -n -b -q all.c\(${model},\"/afs/cern.ch/work/c/cayou/public/80Xsamples/2l2qsamples_1010/VBFHiggs500/ZZ2l2qAnalysis.root\",\"sel\"\)
done
mv 2l2qtree*.root 2l2qSelectedTrees
#comment

### Efficiency ###
<<comment
if [ ! -d "2l2qEfficiency" ]; then
  mkdir 2l2qEfficiency
fi

for model in spin0_ggH spin0_VBF spin2 
do
echo "running $model efficiency"
root -l -n -b -q all.c\(${model},"",\"eff\"\)
done

mv efficiency*.* 2l2qEfficiency
mv 2l2qtree*.root 2l2qSelectedTrees
comment

### 2D Templates ###
<<comment
#make selected trees as input for smoothing
for model in spin0_all #spin2 DYjets TTBar Diboson
do
echo "running $model trees"
root -l -n -b -q all.c\(${model},\"\",\"sel\"\)
done
mv 2l2qtree*.root 2l2qSelectedTrees

#smoothing and normalization
echo "running templates"
bash run2l2q_Templ.sh
comment

### Resolution ###
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
