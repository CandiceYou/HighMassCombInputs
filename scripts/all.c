#include <iostream>
#include "../interface/ResoEff.h"
#include "../src/calcEff.cc"
#include "../src/selection.cc"

using namespace std;

void getSelection(Sample s, char* fin) {
HighMass h(s);
h.makeSelectedTree(fin);
}

void getEfficiency(Sample s) {
HighMass h(s);
h.plotEfficiency();
}

void all(Sample mysample, char* fin,const char* func) {
if (strcmp(func,"sel")==0) getSelection(mysample,fin);
else if (strcmp(func,"eff")==0) getEfficiency(mysample);
}

