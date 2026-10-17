#include "Vdma_observer.h"
#include <verilated.h>
double sc_time_stamp(){return 0.;}
int main(int argc,char **argv){Verilated::commandArgs(argc,argv);Vdma_observer d;d.eval();return 0;}
