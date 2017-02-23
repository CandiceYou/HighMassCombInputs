#include <iostream>
#include "../../interface/ResoEff.h"
#include "../../src/calcEff.cc"
#include "../../src/selection.cc"
#include "../../src/calcReso.cc"

using namespace std;

void getReso(Sample s=spin0_all,const char* cType="resolved") {
HighMass h(s);
h.readResoDataset(cType);
h.fitReso(cType);
}


