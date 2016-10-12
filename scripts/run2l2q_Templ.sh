#check that root trees exist after efficiency step, and smoothing is done

if [ ! -d "2l2q2DTemplate" ]; then
  mkdir -p 2l2q2DTemplate/spin0/1DSlices/
  mkdir -p 2l2q2DTemplate/spin2/1DSlices/
fi

#smoothing
cd /afs/cern.ch/work/c/cayou/HighMass_RunII/CMSSW_7_6_3/src/ZZAnalysis/AnalysisStep/test/CombInputs/TemplateBuilder/
eval `scramv1 runtime -sh` 
./buildTemplate.exe run/2l2q_2Dtempl_spin0.json
./buildTemplate.exe run/2l2q_2Dtempl_spin2.json
./buildTemplate.exe run/2l2q_2Dtempl_spin0_up.json
./buildTemplate.exe run/2l2q_2Dtempl_spin2_up.json
./buildTemplate.exe run/2l2q_2Dtempl_spin0_dn.json
./buildTemplate.exe run/2l2q_2Dtempl_spin2_dn.json
cd -

#normalize
# spin0
root -l -n -b -q normTempl.c\(0,\"nominal\"\) 
root -l -n -b -q normTempl.c\(0,\"JECup\"\) 
root -l -n -b -q normTempl.c\(0,\"JECdn\"\) 
# spin2
root -l -n -b -q normTempl.c\(2,\"nominal\"\)
root -l -n -b -q normTempl.c\(2,\"JECup\"\) 
root -l -n -b -q normTempl.c\(2,\"JECdn\"\)

#check 1D slices
root -l -n -b -q checkTempl.c 
