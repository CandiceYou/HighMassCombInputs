#!/bin/bash          


ch=$1
paramIndex=0
for var in {mean,sigma,a1,a2,n1,n2}
do
((paramIndex++))
FitParamStr[$paramIndex]=""

indParam_p0[$paramIndex]="`awk  '/'$ch'_'$var'_param_0/{print $2}' params/individual_fit_param.txt`"
indParam_p1[$paramIndex]="`awk  '/'$ch'_'$var'_param_1/{print $2}' params/individual_fit_param.txt`"
indParam_p2[$paramIndex]="`awk  '/'$ch'_'$var'_param_2/{print $2}' params/individual_fit_param.txt`"
indParam_p3[$paramIndex]="`awk  '/'$ch'_'$var'_param_3/{print $2}' params/individual_fit_param.txt`"
indParam_p4[$paramIndex]="`awk  '/'$ch'_'$var'_param_4/{print $2}' params/individual_fit_param.txt`"
indParam_p5[$paramIndex]="`awk  '/'$ch'_'$var'_param_5/{print $2}' params/individual_fit_param.txt`"
indParam_p6[$paramIndex]="`awk  '/'$ch'_'$var'_param_6/{print $2}' params/individual_fit_param.txt`"


indFitParamStr[$paramIndex]=${indParam_p0[$paramIndex]}","${indParam_p1[$paramIndex]}","${indParam_p2[$paramIndex]}","${indParam_p3[$paramIndex]}","${indParam_p4[$paramIndex]}","${indParam_p5[$paramIndex]}","${indParam_p6[$paramIndex]}


echo "${var}_indFitParam"
echo "${indFitParamStr[$paramIndex]}"

done

sed  -e 's|<mean_param>|'${indFitParamStr[1]}'|g' -e 's|<sigma_param>|'${indFitParamStr[2]}'|g' -e 's|<a1_param>|'${indFitParamStr[3]}'|g' -e 's|<a2_param>|'${indFitParamStr[4]}'|g' -e 's|<n1_param>|'${indFitParamStr[5]}'|g' -e 's|<n2_param>|'${indFitParamStr[6]}'|g'  <simfit_tmpl.cc >temp2.cc

mv temp2.cc simfit.cc


