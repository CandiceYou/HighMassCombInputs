#include <iostream>
#include "../../interface/ResoEff.h"
#include "../../src/calcEff.cc"
#include "../../src/selection.cc"
#include "../../src/calcReso.cc"
#include "../../src/checkReso.cc"

using namespace std;

void resoClosure(Sample s=spin0_all , const char* cType="resolved") {
HighMass h(s);
h.checkReso(cType);
}


