// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vsw_quiet_top.h"
#include "verilated.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
int main(int argc,char**argv){
    try{
        VerilatedContext context;Vsw_quiet_top d{&context};
        d.clk_i=0;d.rst_ni=0;d.eval();
        for(int i=0;i<4;i++){d.clk_i=1;d.eval();d.clk_i=0;d.eval();}
        d.rst_ni=1;d.eval();
        for(int i=0;i<10;i++){
            if(d.equal_o!=3)throw std::runtime_error("original SW gold mismatch");
            if(d.sw_idle_o!=1||d.prog_idle_o!=1)throw std::runtime_error("empty SW/program not idle or disabled not0");
            d.clk_i=1;d.eval();d.clk_i=0;d.eval();
        }
        std::cout<<"initial complete SW gold smoke passed\n";
        return 0;
    }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}
}
