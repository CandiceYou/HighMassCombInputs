# HighMassCombInputs-4l Resolution

Desciption of all scripts -

* readData.cc :
Merge all the input files and re-divide them into datasets according to a chosen mass window around the selected mass points.
xMin and xMax defines the fitting range for individual masses, this is according to a preliminary 2nd order polynomial fit on resolution width.
* indfit.cc:
read the datasets produced by readData.cc run a doubleCB fitting for the resolution of each mass point individually.

* readParam_single.sh:
read the fitting output files for all mass points produced by indfit.cc, generate an array and insert into plotVar_fitInd_tmpl.C

* plotVar_fitInd_tmpl.C:
template file for plotVar_fitInd.C
make plots of the 6 dCB parameters and fit with piece-wise 2nd order polynomial.

* readParam_ind.sh:
extract the fitting of the 6 dCB parameters as a function of mass, and insert into simfit_tmpl.cc

* simfit_tmpl.cc:
template file for simfit.cc
fit the resolution for all mass points together using the mass dependence extracted from readParam_ind.sh.
This produce a "closure test" for the resolution in full mass range.

Output files and sanity checks-

* singleMassFit: This directory has all plots and fitting parameter files for the resolution fitting of individual points.They can be used to check if any mass point selected has too few events and become an outlier for next steps, or if the sample itself has some problem and the doubleCB function cannot describe the resolution.

* params: This directory has all plots and fitting parameter files for the fitting of dCB parameters as a function of mass. You can use these to check if the piece-wise 2nd order polynomial describe the dependence correctly and if the joining points are selected properly.

* fixParamFit: This directory has all plots for the final closure test. The resolution is calculated succesfully if all mass points show a good fitting.

How to run -
* Change the input file directory in readData.cc.
* Select the pole masses of your samples in the inputfiles[] array.Select the mass points you wish to fit in the massBin[] array. Keep both of them small when you do a test run.
* You can then run all the steps by ./all.sh

What to tune -
* The mass window for fitting of each mass point, defined in readData.cc. A wider window gives more event for each dataset and a more stable overall fit, but less accurate for the specific mass.
* The mass points. Mass points are selected at smaller steps for low mass and larger steps for high mass.
* The fitting range. Needs to be small enough for a accurate description, but need to include the full tail. The left tail has been assigned to a larger range than the right tail.
* The joint point for piece-wise function. This depends on the mass range and the distribution.
