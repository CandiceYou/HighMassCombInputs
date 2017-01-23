for subdir in singleMassFit fixParamFit params 
do
mkdir $subdir
done

rm params/individual_fit_param.txt

for ch in {2e2mu,4mu,4e}
do
root -l -n -q -b readData.cc\(\"$ch\"\) indfit.cc\(\"$ch\"\)
#bash readParam_single.sh $ch
#root -l -n -b -q "plotVar_fitInd_$ch.C"
#bash readParam_ind.sh $ch
#root -l -n -q -b readData.cc\(\"$ch\"\) simfit.cc\(\"$ch\"\)
done
