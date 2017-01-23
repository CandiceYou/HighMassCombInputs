#!/bin/bash          

cat=$1

MassStr=""
paramIndex=0
massIndex=0

for file in singleMassFit/${cat}/SingleMassFit_ResoParam_MH*_${cat}.txt
do
if [ "${massIndex}" -ne "0" ] 
  then MassStr+=","
fi
MassStr+="${file//[!0-9]/}"
((massIndex++))
done
echo "$MassStr"

for var in {mean,sigma,a1,a2,n1,n2}
do
((paramIndex++))
ParamStr[$paramIndex]=""
ErrStr[$paramIndex]=""

for file in singleMassFit/${cat}/SingleMassFit_ResoParam_MH*_${cat}.txt
do
#echo $file
ParamStr[$paramIndex]+="`awk -v ORS="," '/'$var'_CB/{print $3}' $file`"
ErrStr[$paramIndex]+="`awk -v ORS="," '/'$var'_CB/{print $5}' $file`"
done

echo ${var}
echo "${ParamStr[$paramIndex]}"
echo "${var}_Error"
echo "${ErrStr[$paramIndex]}"

done

echo "Number of mass points = $massIndex"
echo "Number of parameters = $paramIndex"

sed -e 's|<massArray>|'$MassStr'|g' -e 's|<nArray>|'$massIndex'|g'  -e 's|<meanArray>|'${ParamStr[1]}'|g'  -e 's|<sigmaArray>|'${ParamStr[2]}'|g'  -e 's|<a1Array>|'${ParamStr[3]}'|g'  -e 's|<a2Array>|'${ParamStr[4]}'|g'  -e 's|<n1Array>|'${ParamStr[5]}'|g'  -e 's|<n2Array>|'${ParamStr[6]}'|g'  -e 's|<meanErrArray>|'${ErrStr[1]}'|g'  -e 's|<sigmaErrArray>|'${ErrStr[2]}'|g' -e 's|<a1ErrArray>|'${ErrStr[3]}'|g' -e 's|<a2ErrArray>|'${ErrStr[4]}'|g' -e 's|<n1ErrArray>|'${ErrStr[5]}'|g' -e 's|<n2ErrArray>|'${ErrStr[6]}'|g' < plotVar_fitInd_tmpl.C > plotVar_fitInd.C

