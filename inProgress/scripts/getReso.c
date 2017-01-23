#include <iostream>
#include "../../interface/ResoEff.h"
#include "../../src/calcEff.cc"
#include "../../src/selection.cc"
#include "../../src/calcReso.cc"

using namespace std;

void getReso(Sample s=spin0_all) {
HighMass h(s);
h.readResoDataset("resolved");
h.fitReso("resolved");
h.readResoDataset("merged");
h.fitReso("merged");
}


