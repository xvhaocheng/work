// Test-only package boundary. The selected cv64a6_imafdc_sv39_config_pkg.sv:43
// sets CVA6ConfigFPGAEn=0. No CPU or SoC RTL is instantiated by this test.
package ariane_pkg;
  localparam bit FPGA_EN = 1'b0;
endpackage
