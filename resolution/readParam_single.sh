#!/bin/bash          

ch=$1

MassStr=""
paramIndex=0
massIndex=0

#for mass in {115,120}
#for mass in {115,120,125,126,130,135,140,145,150,155,160,170,175,180,200,230,250,350,400,450,500,600,750,800,900,1000} 
for mass in {120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,680,700,720,740,760,780,800,820,840,860,880,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2800,3000}
do
MassStr+="$mass,"
((massIndex++))
done
echo "$MassStr"

for var in {mean,sigma,a1,a2,n1,n2}
do
((paramIndex++))
ParamStr[$paramIndex]=""
ErrStr[$paramIndex]=""

#for mass in {115,120}
#for mass in {115,120,125,126,130,135,140,145,150,155,160,170,175,180,200,230,250,350,400,450,500,600,750,800,900,1000}
for mass in {120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,680,700,720,740,760,780,800,820,840,860,880,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2800,3000}

do
ParamStr[$paramIndex]+="`awk -v ORS="," '/'$var'_CB/{print $3}' singleMassFit/SingleMassFit_ResoParam_MH${mass}_${ch}.txt`"
ErrStr[$paramIndex]+="`awk -v ORS="," '/'$var'_CB/{print $5}' singleMassFit/SingleMassFit_ResoParam_MH${mass}_${ch}.txt`"
done

echo ${var}
echo "${ParamStr[$paramIndex]}"
echo "${var}_Error"
echo "${ErrStr[$paramIndex]}"

done

echo "Number of mass points = $massIndex"
echo "Number of parameters = $paramIndex"

sed -e 's|<massArray>|'$MassStr'|g' -e 's|<nArray>|'$massIndex'|g'  -e 's|<meanArray>|'${ParamStr[1]}'|g'  -e 's|<sigmaArray>|'${ParamStr[2]}'|g'  -e 's|<a1Array>|'${ParamStr[3]}'|g'  -e 's|<a2Array>|'${ParamStr[4]}'|g'  -e 's|<n1Array>|'${ParamStr[5]}'|g'  -e 's|<n2Array>|'${ParamStr[6]}'|g'  -e 's|<meanErrArray>|'${ErrStr[1]}'|g'  -e 's|<sigmaErrArray>|'${ErrStr[2]}'|g' -e 's|<a1ErrArray>|'${ErrStr[3]}'|g' -e 's|<a2ErrArray>|'${ErrStr[4]}'|g' -e 's|<n1ErrArray>|'${ErrStr[5]}'|g' -e 's|<n2ErrArray>|'${ErrStr[6]}'|g' -e 's|<channel>|'$ch'|g' < plotVar_fitInd_tmpl.C > plotVar_fitInd_${ch}.C


