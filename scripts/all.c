#include <iostream>
#include "../interface/ResoEff.h"
#include "../src/calcEff.cc"
#include "../src/selection.cc"

using namespace std;

void getSelection(Sample s) {
HighMass h(s);
h.makeSelectedTree();
}

void getEfficiency(Sample s) {
HighMass h(s);
h.plotEfficiency();
}

void all(Sample mysample, const char* func) {
if (strcmp(func,"sel")==0) getSelection(mysample);
else if (strcmp(func,"eff")==0) getEfficiency(mysample);
}

