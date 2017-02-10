#check that root trees exist after efficiency step, and smoothing is done
cp ../scripts/checkTempl_1Dslice_2Dsys.c .
cp ../scripts/normTempl.c .
cp ../scripts/

if [ ! -d "2l2q2DTemplate" ]; then
  mkdir -p 2l2q2DTemplate/spin0/1DSlices/
#  mkdir -p 2l2q2DTemplate/spin2/1DSlices/
fi

#smoothing
cp ../smConfig/*.json ../../TemplateBuilder/run 
cd ../../TemplateBuilder/
eval `scramv1 runtime -sh` 
./buildTemplate.exe run/2l2q_2Dtempl_spin0.json
./buildTemplate.exe run/2l2q_2Dtempl_spin0_up.json
./buildTemplate.exe run/2l2q_2Dtempl_spin0_dn.json
#./buildTemplate.exe run/2l2q_2Dtempl_spin2.json
#./buildTemplate.exe run/2l2q_2Dtempl_spin2_up.json
#./buildTemplate.exe run/2l2q_2Dtempl_spin2_dn.json
cd -

#normalize
# spin0
root -l -n -b -q normTempl.c\(0,\"nominal\"\) 
root -l -n -b -q normTempl.c\(0,\"JECup\"\) 
root -l -n -b -q normTempl.c\(0,\"JECdn\"\) 
# spin2
#root -l -n -b -q normTempl.c\(2,\"nominal\"\)
#root -l -n -b -q normTempl.c\(2,\"JECup\"\) 
#root -l -n -b -q normTempl.c\(2,\"JECdn\"\)

#check 1D slices, make alternative templates
root -l -b -q checkTempl_1Dslice_2Dsys.c \(0\)   #spin0
#root -l -b -q checkTempl_1Dslice_2Dsys.c \(2\)  #spin2
