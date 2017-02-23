model=$1

if [ ! -d "2l2qResolution_${model}" ]; then 
  mkdir 2l2qResolution_${model}
fi

cd 2l2qResolution_${model}

mkdir -p singleMassFit/resolved
mkdir -p singleMassFit/merged
mkdir -p params
mkdir -p closureTest/resolved
mkdir -p closureTest/merged

cp ../../scripts/readParam_single.sh .
cp ../../scripts/readParam_ind.sh .
cp ../../scripts/plotVar_fitInd_tmpl.C .
cp ../../scripts/getReso.c .
cp ../../scripts/resoClosure.c .
cp ../../src/checkReso_tmpl.cc .

#fit dCB parameters, and closure test
for cat in "merged" "resolved"
do

#make dataset and single mass fit, for both resolved and merged
root -l -n -b -q getReso.c\(${model},\"$cat\"\)
mv 2l2qtree*.root ../2l2qSelectedTrees

echo " calculating resolution for ${cat}"
rm params/individual_fit_param_${cat}.txt
bash readParam_single.sh $cat
root -l -n -b -q plotVar_fitInd.C\(\"$cat\"\)
bash readParam_ind.sh $cat
root -l -n -b -q resoClosure.c\(${model},\"$cat\"\)
done

rm checkReso_tmpl.cc
rm plotVar_fitInd_tmpl.C
rm readParam_single.sh
rm readParam_ind.sh
rm plotVar_fitInd.C
rm getReso.c 
rm resoClosure.c 
rm resoDataset_*.root
