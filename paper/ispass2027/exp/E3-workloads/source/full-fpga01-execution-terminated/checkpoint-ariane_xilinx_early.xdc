set_property SRC_FILE_INFO {cfile:/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-ds0-build01/src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/constraints/xlnx_mig_7_ddr3.xdc rfile:../../xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/constraints/xlnx_mig_7_ddr3.xdc id:1 order:EARLY scoped_inst:i_ddr rxprname:$PPRDIR/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/constraints/xlnx_mig_7_ddr3.xdc} [current_design]
set_property SRC_FILE_INFO {cfile:/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-ds0-build01/src/cva6/corev_apu/fpga/xilinx/xlnx_clk_gen/xlnx_clk_gen.gen/sources_1/ip/xlnx_clk_gen/xlnx_clk_gen.xdc rfile:../../xilinx/xlnx_clk_gen/xlnx_clk_gen.gen/sources_1/ip/xlnx_clk_gen/xlnx_clk_gen.xdc id:2 order:EARLY scoped_inst:i_xlnx_clk_gen/inst rxprname:$PPRDIR/xilinx/xlnx_clk_gen/xlnx_clk_gen.gen/sources_1/ip/xlnx_clk_gen/xlnx_clk_gen.xdc} [current_design]
set_property SRC_FILE_INFO {cfile:/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-ds0-build01/src/cva6/corev_apu/fpga/xilinx/xlnx_axi_gpio/xlnx_axi_gpio.gen/sources_1/ip/xlnx_axi_gpio/xlnx_axi_gpio.xdc rfile:../../xilinx/xlnx_axi_gpio/xlnx_axi_gpio.gen/sources_1/ip/xlnx_axi_gpio/xlnx_axi_gpio.xdc id:3 order:EARLY scoped_inst:i_ariane_peripherals/gen_gpio.i_xlnx_axi_gpio/U0 rxprname:$PPRDIR/xilinx/xlnx_axi_gpio/xlnx_axi_gpio.gen/sources_1/ip/xlnx_axi_gpio/xlnx_axi_gpio.xdc} [current_design]
set_property SRC_FILE_INFO {cfile:/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-ds0-build01/src/cva6/corev_apu/fpga/xilinx/xlnx_axi_quad_spi/xlnx_axi_quad_spi.gen/sources_1/ip/xlnx_axi_quad_spi/xlnx_axi_quad_spi.xdc rfile:../../xilinx/xlnx_axi_quad_spi/xlnx_axi_quad_spi.gen/sources_1/ip/xlnx_axi_quad_spi/xlnx_axi_quad_spi.xdc id:4 order:EARLY scoped_inst:i_ariane_peripherals/gen_spi.i_xlnx_axi_quad_spi/U0 rxprname:$PPRDIR/xilinx/xlnx_axi_quad_spi/xlnx_axi_quad_spi.gen/sources_1/ip/xlnx_axi_quad_spi/xlnx_axi_quad_spi.xdc} [current_design]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:CLKIN1:CLKFBOUT:pll_clk3_out:clk_pll_i::1:0:CLKIN1:CLKOUT0:pll_clk3_out:mmcm_ps_clk_bufg_in::1:0:CLKIN1:CLKOUT1:pll_clk3_out:clk_div2_bufg_in [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_ddr3_infrastructure/gen_mmcm.mmcm_i]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:CLKIN1:CLKFBOUT:sys_clk_p:pll_clkfbout::1:0:CLKIN1:CLKOUT0:sys_clk_p:freq_refclk::1:0:CLKIN1:CLKOUT1:sys_clk_p:mem_refclk::1:0:CLKIN1:CLKOUT2:sys_clk_p:sync_pulse::1:0:CLKIN1:CLKOUT3:sys_clk_p:pll_clk3_out [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_ddr3_infrastructure/plle2_i]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:CLKIN1:CLKFBOUT:sys_clk_p:mmcm_clkfbout::1:0:CLKIN1:CLKOUT1:sys_clk_p:clk_ref_mmcm_400 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_iodelay_ctrl/clk_ref_mmcm_gen.mmcm_i]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:FREQREFCLK:ICLK:freq_refclk:u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/iserdes_clk::1:0:ICLK:ICLKDIV:u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/iserdes_clk:iserdes_clkdiv [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_in_gen.phaser_in]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:MEMREFCLK:OCLK:mem_refclk:oserdes_clk::1:0:OCLK:OCLKDIV:oserdes_clk:oserdes_clkdiv [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_out]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:FREQREFCLK:ICLK:freq_refclk:u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/iserdes_clk::1:0:ICLK:ICLKDIV:u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/iserdes_clk:iserdes_clkdiv_1 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_in_gen.phaser_in]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:MEMREFCLK:OCLK:mem_refclk:oserdes_clk_1::1:0:OCLK:OCLKDIV:oserdes_clk_1:oserdes_clkdiv_1 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_out]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:FREQREFCLK:ICLK:freq_refclk:u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/iserdes_clk::1:0:ICLK:ICLKDIV:u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/iserdes_clk:iserdes_clkdiv_2 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_in_gen.phaser_in]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:MEMREFCLK:OCLK:mem_refclk:oserdes_clk_2::1:0:OCLK:OCLKDIV:oserdes_clk_2:oserdes_clkdiv_2 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_out]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:FREQREFCLK:ICLK:freq_refclk:u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/iserdes_clk::1:0:ICLK:ICLKDIV:u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/iserdes_clk:iserdes_clkdiv_3 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_in_gen.phaser_in]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:MEMREFCLK:OCLK:mem_refclk:oserdes_clk_3::1:0:OCLK:OCLKDIV:oserdes_clk_3:oserdes_clkdiv_3 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_out]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:MEMREFCLK:OCLK:mem_refclk:oserdes_clk_4::1:0:OCLK:OCLKDIV:oserdes_clk_4:oserdes_clkdiv_4 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_out]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:MEMREFCLK:OCLK:mem_refclk:oserdes_clk_5::1:0:OCLK:OCLKDIV:oserdes_clk_5:oserdes_clkdiv_5 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_out]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:MEMREFCLK:OCLK:mem_refclk:oserdes_clk_6::1:0:OCLK:OCLKDIV:oserdes_clk_6:oserdes_clkdiv_6 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_out]
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:MEMREFCLK:OCLK:mem_refclk:oserdes_clk_7::1:0:OCLK:OCLKDIV:oserdes_clk_7:oserdes_clkdiv_7 [get_cells i_ddr/u_xlnx_mig_7_ddr3_mig/u_memc_ui_top_axi/mem_intfc0/ddr_phy_top0/u_ddr_mc_phy_wrapper/u_ddr_mc_phy/ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_out]
current_instance i_xlnx_clk_gen/inst
set_property src_info {type:PI file:{} line:-1 export:NONE save:INPUT read:READ} [current_design]
set_property TOOL_DERIVED_CLK_NAMES 1:0:CLKIN1:CLKFBOUT:i_xlnx_clk_gen/inst/clk_in1:clkfbout_xlnx_clk_gen::1:0:CLKIN1:CLKOUT0:i_xlnx_clk_gen/inst/clk_in1:clk_out1_xlnx_clk_gen::1:0:CLKIN1:CLKOUT1:i_xlnx_clk_gen/inst/clk_in1:clk_out2_xlnx_clk_gen::1:0:CLKIN1:CLKOUT2:i_xlnx_clk_gen/inst/clk_in1:clk_out3_xlnx_clk_gen::1:0:CLKIN1:CLKOUT3:i_xlnx_clk_gen/inst/clk_in1:clk_out4_xlnx_clk_gen [get_cells mmcm_adv_inst]
set_property src_info {type:SCOPED_XDC file:1 line:1 export:INPUT save:INPUT read:READ} [current_design]
##################################################################################################
set_property src_info {type:SCOPED_XDC file:1 line:2 export:INPUT save:INPUT read:READ} [current_design]
##
set_property src_info {type:SCOPED_XDC file:1 line:3 export:INPUT save:INPUT read:READ} [current_design]
##  Xilinx, Inc. 2010            www.xilinx.com
set_property src_info {type:SCOPED_XDC file:1 line:4 export:INPUT save:INPUT read:READ} [current_design]
##  Sat Oct 17 23:16:41 2026
set_property src_info {type:SCOPED_XDC file:1 line:5 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:6 export:INPUT save:INPUT read:READ} [current_design]
##  Generated by MIG Version 4.2
set_property src_info {type:SCOPED_XDC file:1 line:7 export:INPUT save:INPUT read:READ} [current_design]
##
set_property src_info {type:SCOPED_XDC file:1 line:8 export:INPUT save:INPUT read:READ} [current_design]
##################################################################################################
set_property src_info {type:SCOPED_XDC file:1 line:9 export:INPUT save:INPUT read:READ} [current_design]
##  File name :       xlnx_mig_7_ddr3.xdc
set_property src_info {type:SCOPED_XDC file:1 line:10 export:INPUT save:INPUT read:READ} [current_design]
##  Details :     Constraints file
set_property src_info {type:SCOPED_XDC file:1 line:11 export:INPUT save:INPUT read:READ} [current_design]
##                    FPGA Family:       KINTEX7
set_property src_info {type:SCOPED_XDC file:1 line:12 export:INPUT save:INPUT read:READ} [current_design]
##                    FPGA Part:         XC7K325T-FFG900
set_property src_info {type:SCOPED_XDC file:1 line:13 export:INPUT save:INPUT read:READ} [current_design]
##                    Speedgrade:        -2
set_property src_info {type:SCOPED_XDC file:1 line:14 export:INPUT save:INPUT read:READ} [current_design]
##                    Design Entry:      VERILOG
set_property src_info {type:SCOPED_XDC file:1 line:15 export:INPUT save:INPUT read:READ} [current_design]
##                    Frequency:         800 MHz
set_property src_info {type:SCOPED_XDC file:1 line:16 export:INPUT save:INPUT read:READ} [current_design]
##                    Time Period:       1250 ps
set_property src_info {type:SCOPED_XDC file:1 line:17 export:INPUT save:INPUT read:READ} [current_design]
##################################################################################################
set_property src_info {type:SCOPED_XDC file:1 line:18 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:19 export:INPUT save:INPUT read:READ} [current_design]
##################################################################################################
set_property src_info {type:SCOPED_XDC file:1 line:20 export:INPUT save:INPUT read:READ} [current_design]
## Controller 0
set_property src_info {type:SCOPED_XDC file:1 line:21 export:INPUT save:INPUT read:READ} [current_design]
## Memory Device: DDR3_SDRAM->Components->MT41J256m16XX-107
set_property src_info {type:SCOPED_XDC file:1 line:22 export:INPUT save:INPUT read:READ} [current_design]
## Data Width: 32
set_property src_info {type:SCOPED_XDC file:1 line:23 export:INPUT save:INPUT read:READ} [current_design]
## Time Period: 1250
set_property src_info {type:SCOPED_XDC file:1 line:24 export:INPUT save:INPUT read:READ} [current_design]
## Data Mask: 1
set_property src_info {type:SCOPED_XDC file:1 line:25 export:INPUT save:INPUT read:READ} [current_design]
##################################################################################################
set_property src_info {type:SCOPED_XDC file:1 line:26 export:INPUT save:INPUT read:READ} [current_design]

current_instance -quiet
set_property src_info {type:SCOPED_XDC file:1 line:27 export:INPUT save:INPUT read:READ} [current_design]
set_property IO_BUFFER_TYPE NONE [get_ports {ddr3_ck_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:28 export:INPUT save:INPUT read:READ} [current_design]
set_property IO_BUFFER_TYPE NONE [get_ports {ddr3_ck_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:29 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:30 export:INPUT save:INPUT read:READ} [current_design]
create_clock -period 5.000 [get_ports sys_clk_p]
set_property src_info {type:SCOPED_XDC file:1 line:31 export:INPUT save:INPUT read:READ} [current_design]
# Note: CLK_REF FALSE Constraint.
set_property src_info {type:SCOPED_XDC file:1 line:32 export:INPUT save:INPUT read:READ} [current_design]
# CLK_REF is a 200  MHz clock source used to drive IODELAY CTRL logic (via an
set_property src_info {type:SCOPED_XDC file:1 line:33 export:INPUT save:INPUT read:READ} [current_design]
# additional MMCM). This clock need not utilized CLOCK_DEDICADE_ROUTE (as they
set_property src_info {type:SCOPED_XDC file:1 line:34 export:INPUT save:INPUT read:READ} [current_design]
# are limited in number), hence the FALSE value set.
set_property src_info {type:SCOPED_XDC file:1 line:35 export:INPUT save:INPUT read:READ} [current_design]
# Please refer to UG 586 for details on clocking resources.
current_instance i_ddr
set_property src_info {type:SCOPED_XDC file:1 line:36 export:INPUT save:INPUT read:READ} [current_design]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_pins -hierarchical *clk_ref_mmcm_gen.mmcm_i*CLKIN1]
set_property src_info {type:SCOPED_XDC file:1 line:37 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:38 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:39 export:INPUT save:INPUT read:READ} [current_design]
############## NET - IOSTANDARD ##################
set_property src_info {type:SCOPED_XDC file:1 line:40 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:41 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:42 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L1N_T0_34
current_instance -quiet
set_property src_info {type:SCOPED_XDC file:1 line:43 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:44 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:45 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:46 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD3 [get_ports {ddr3_dq[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:47 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:48 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L2P_T0_34
set_property src_info {type:SCOPED_XDC file:1 line:49 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:50 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:51 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:52 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC2 [get_ports {ddr3_dq[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:53 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:54 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L2N_T0_34
set_property src_info {type:SCOPED_XDC file:1 line:55 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:56 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:57 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:58 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC1 [get_ports {ddr3_dq[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:59 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:60 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L4P_T0_34
set_property src_info {type:SCOPED_XDC file:1 line:61 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:62 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:63 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:64 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC5 [get_ports {ddr3_dq[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:65 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:66 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L4N_T0_34
set_property src_info {type:SCOPED_XDC file:1 line:67 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[4]}]
set_property src_info {type:SCOPED_XDC file:1 line:68 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[4]}]
set_property src_info {type:SCOPED_XDC file:1 line:69 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[4]}]
set_property src_info {type:SCOPED_XDC file:1 line:70 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC4 [get_ports {ddr3_dq[4]}]
set_property src_info {type:SCOPED_XDC file:1 line:71 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:72 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L5P_T0_34
set_property src_info {type:SCOPED_XDC file:1 line:73 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[5]}]
set_property src_info {type:SCOPED_XDC file:1 line:74 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[5]}]
set_property src_info {type:SCOPED_XDC file:1 line:75 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[5]}]
set_property src_info {type:SCOPED_XDC file:1 line:76 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD6 [get_ports {ddr3_dq[5]}]
set_property src_info {type:SCOPED_XDC file:1 line:77 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:78 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L5N_T0_34
set_property src_info {type:SCOPED_XDC file:1 line:79 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[6]}]
set_property src_info {type:SCOPED_XDC file:1 line:80 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[6]}]
set_property src_info {type:SCOPED_XDC file:1 line:81 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[6]}]
set_property src_info {type:SCOPED_XDC file:1 line:82 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AE6 [get_ports {ddr3_dq[6]}]
set_property src_info {type:SCOPED_XDC file:1 line:83 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:84 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L6P_T0_34
set_property src_info {type:SCOPED_XDC file:1 line:85 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[7]}]
set_property src_info {type:SCOPED_XDC file:1 line:86 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[7]}]
set_property src_info {type:SCOPED_XDC file:1 line:87 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[7]}]
set_property src_info {type:SCOPED_XDC file:1 line:88 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC7 [get_ports {ddr3_dq[7]}]
set_property src_info {type:SCOPED_XDC file:1 line:89 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:90 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L7N_T1_34
set_property src_info {type:SCOPED_XDC file:1 line:91 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[8]}]
set_property src_info {type:SCOPED_XDC file:1 line:92 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[8]}]
set_property src_info {type:SCOPED_XDC file:1 line:93 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[8]}]
set_property src_info {type:SCOPED_XDC file:1 line:94 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AF2 [get_ports {ddr3_dq[8]}]
set_property src_info {type:SCOPED_XDC file:1 line:95 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:96 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L8P_T1_34
set_property src_info {type:SCOPED_XDC file:1 line:97 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[9]}]
set_property src_info {type:SCOPED_XDC file:1 line:98 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[9]}]
set_property src_info {type:SCOPED_XDC file:1 line:99 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[9]}]
set_property src_info {type:SCOPED_XDC file:1 line:100 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AE1 [get_ports {ddr3_dq[9]}]
set_property src_info {type:SCOPED_XDC file:1 line:101 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:102 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L8N_T1_34
set_property src_info {type:SCOPED_XDC file:1 line:103 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[10]}]
set_property src_info {type:SCOPED_XDC file:1 line:104 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[10]}]
set_property src_info {type:SCOPED_XDC file:1 line:105 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[10]}]
set_property src_info {type:SCOPED_XDC file:1 line:106 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AF1 [get_ports {ddr3_dq[10]}]
set_property src_info {type:SCOPED_XDC file:1 line:107 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:108 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L10P_T1_34
set_property src_info {type:SCOPED_XDC file:1 line:109 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[11]}]
set_property src_info {type:SCOPED_XDC file:1 line:110 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[11]}]
set_property src_info {type:SCOPED_XDC file:1 line:111 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[11]}]
set_property src_info {type:SCOPED_XDC file:1 line:112 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AE4 [get_ports {ddr3_dq[11]}]
set_property src_info {type:SCOPED_XDC file:1 line:113 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:114 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L10N_T1_34
set_property src_info {type:SCOPED_XDC file:1 line:115 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[12]}]
set_property src_info {type:SCOPED_XDC file:1 line:116 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[12]}]
set_property src_info {type:SCOPED_XDC file:1 line:117 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[12]}]
set_property src_info {type:SCOPED_XDC file:1 line:118 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AE3 [get_ports {ddr3_dq[12]}]
set_property src_info {type:SCOPED_XDC file:1 line:119 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:120 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L11P_T1_SRCC_34
set_property src_info {type:SCOPED_XDC file:1 line:121 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[13]}]
set_property src_info {type:SCOPED_XDC file:1 line:122 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[13]}]
set_property src_info {type:SCOPED_XDC file:1 line:123 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[13]}]
set_property src_info {type:SCOPED_XDC file:1 line:124 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AE5 [get_ports {ddr3_dq[13]}]
set_property src_info {type:SCOPED_XDC file:1 line:125 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:126 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L11N_T1_SRCC_34
set_property src_info {type:SCOPED_XDC file:1 line:127 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[14]}]
set_property src_info {type:SCOPED_XDC file:1 line:128 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[14]}]
set_property src_info {type:SCOPED_XDC file:1 line:129 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[14]}]
set_property src_info {type:SCOPED_XDC file:1 line:130 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AF5 [get_ports {ddr3_dq[14]}]
set_property src_info {type:SCOPED_XDC file:1 line:131 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:132 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L12P_T1_MRCC_34
set_property src_info {type:SCOPED_XDC file:1 line:133 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[15]}]
set_property src_info {type:SCOPED_XDC file:1 line:134 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[15]}]
set_property src_info {type:SCOPED_XDC file:1 line:135 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[15]}]
set_property src_info {type:SCOPED_XDC file:1 line:136 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AF6 [get_ports {ddr3_dq[15]}]
set_property src_info {type:SCOPED_XDC file:1 line:137 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:138 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L13N_T2_MRCC_34
set_property src_info {type:SCOPED_XDC file:1 line:139 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[16]}]
set_property src_info {type:SCOPED_XDC file:1 line:140 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[16]}]
set_property src_info {type:SCOPED_XDC file:1 line:141 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[16]}]
set_property src_info {type:SCOPED_XDC file:1 line:142 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AJ4 [get_ports {ddr3_dq[16]}]
set_property src_info {type:SCOPED_XDC file:1 line:143 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:144 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L14P_T2_SRCC_34
set_property src_info {type:SCOPED_XDC file:1 line:145 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[17]}]
set_property src_info {type:SCOPED_XDC file:1 line:146 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[17]}]
set_property src_info {type:SCOPED_XDC file:1 line:147 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[17]}]
set_property src_info {type:SCOPED_XDC file:1 line:148 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AH6 [get_ports {ddr3_dq[17]}]
set_property src_info {type:SCOPED_XDC file:1 line:149 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:150 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L14N_T2_SRCC_34
set_property src_info {type:SCOPED_XDC file:1 line:151 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[18]}]
set_property src_info {type:SCOPED_XDC file:1 line:152 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[18]}]
set_property src_info {type:SCOPED_XDC file:1 line:153 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[18]}]
set_property src_info {type:SCOPED_XDC file:1 line:154 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AH5 [get_ports {ddr3_dq[18]}]
set_property src_info {type:SCOPED_XDC file:1 line:155 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:156 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L16P_T2_34
set_property src_info {type:SCOPED_XDC file:1 line:157 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[19]}]
set_property src_info {type:SCOPED_XDC file:1 line:158 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[19]}]
set_property src_info {type:SCOPED_XDC file:1 line:159 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[19]}]
set_property src_info {type:SCOPED_XDC file:1 line:160 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AH2 [get_ports {ddr3_dq[19]}]
set_property src_info {type:SCOPED_XDC file:1 line:161 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:162 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L16N_T2_34
set_property src_info {type:SCOPED_XDC file:1 line:163 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[20]}]
set_property src_info {type:SCOPED_XDC file:1 line:164 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[20]}]
set_property src_info {type:SCOPED_XDC file:1 line:165 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[20]}]
set_property src_info {type:SCOPED_XDC file:1 line:166 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AJ2 [get_ports {ddr3_dq[20]}]
set_property src_info {type:SCOPED_XDC file:1 line:167 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:168 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L17P_T2_34
set_property src_info {type:SCOPED_XDC file:1 line:169 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[21]}]
set_property src_info {type:SCOPED_XDC file:1 line:170 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[21]}]
set_property src_info {type:SCOPED_XDC file:1 line:171 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[21]}]
set_property src_info {type:SCOPED_XDC file:1 line:172 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AJ1 [get_ports {ddr3_dq[21]}]
set_property src_info {type:SCOPED_XDC file:1 line:173 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:174 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L17N_T2_34
set_property src_info {type:SCOPED_XDC file:1 line:175 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[22]}]
set_property src_info {type:SCOPED_XDC file:1 line:176 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[22]}]
set_property src_info {type:SCOPED_XDC file:1 line:177 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[22]}]
set_property src_info {type:SCOPED_XDC file:1 line:178 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AK1 [get_ports {ddr3_dq[22]}]
set_property src_info {type:SCOPED_XDC file:1 line:179 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:180 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L18P_T2_34
set_property src_info {type:SCOPED_XDC file:1 line:181 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[23]}]
set_property src_info {type:SCOPED_XDC file:1 line:182 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[23]}]
set_property src_info {type:SCOPED_XDC file:1 line:183 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[23]}]
set_property src_info {type:SCOPED_XDC file:1 line:184 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AJ3 [get_ports {ddr3_dq[23]}]
set_property src_info {type:SCOPED_XDC file:1 line:185 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:186 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L20P_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:187 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[24]}]
set_property src_info {type:SCOPED_XDC file:1 line:188 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[24]}]
set_property src_info {type:SCOPED_XDC file:1 line:189 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[24]}]
set_property src_info {type:SCOPED_XDC file:1 line:190 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AF7 [get_ports {ddr3_dq[24]}]
set_property src_info {type:SCOPED_XDC file:1 line:191 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:192 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L20N_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:193 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[25]}]
set_property src_info {type:SCOPED_XDC file:1 line:194 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[25]}]
set_property src_info {type:SCOPED_XDC file:1 line:195 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[25]}]
set_property src_info {type:SCOPED_XDC file:1 line:196 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AG7 [get_ports {ddr3_dq[25]}]
set_property src_info {type:SCOPED_XDC file:1 line:197 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:198 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L22P_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:199 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[26]}]
set_property src_info {type:SCOPED_XDC file:1 line:200 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[26]}]
set_property src_info {type:SCOPED_XDC file:1 line:201 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[26]}]
set_property src_info {type:SCOPED_XDC file:1 line:202 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AJ6 [get_ports {ddr3_dq[26]}]
set_property src_info {type:SCOPED_XDC file:1 line:203 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:204 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L22N_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:205 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[27]}]
set_property src_info {type:SCOPED_XDC file:1 line:206 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[27]}]
set_property src_info {type:SCOPED_XDC file:1 line:207 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[27]}]
set_property src_info {type:SCOPED_XDC file:1 line:208 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AK6 [get_ports {ddr3_dq[27]}]
set_property src_info {type:SCOPED_XDC file:1 line:209 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:210 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L23P_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:211 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[28]}]
set_property src_info {type:SCOPED_XDC file:1 line:212 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[28]}]
set_property src_info {type:SCOPED_XDC file:1 line:213 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[28]}]
set_property src_info {type:SCOPED_XDC file:1 line:214 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AJ8 [get_ports {ddr3_dq[28]}]
set_property src_info {type:SCOPED_XDC file:1 line:215 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:216 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L23N_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:217 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[29]}]
set_property src_info {type:SCOPED_XDC file:1 line:218 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[29]}]
set_property src_info {type:SCOPED_XDC file:1 line:219 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[29]}]
set_property src_info {type:SCOPED_XDC file:1 line:220 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AK8 [get_ports {ddr3_dq[29]}]
set_property src_info {type:SCOPED_XDC file:1 line:221 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:222 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L24P_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:223 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[30]}]
set_property src_info {type:SCOPED_XDC file:1 line:224 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[30]}]
set_property src_info {type:SCOPED_XDC file:1 line:225 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[30]}]
set_property src_info {type:SCOPED_XDC file:1 line:226 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AK5 [get_ports {ddr3_dq[30]}]
set_property src_info {type:SCOPED_XDC file:1 line:227 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:228 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L24N_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:229 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dq[31]}]
set_property src_info {type:SCOPED_XDC file:1 line:230 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dq[31]}]
set_property src_info {type:SCOPED_XDC file:1 line:231 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddr3_dq[31]}]
set_property src_info {type:SCOPED_XDC file:1 line:232 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AK4 [get_ports {ddr3_dq[31]}]
set_property src_info {type:SCOPED_XDC file:1 line:233 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:234 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L16N_T2_33
set_property src_info {type:SCOPED_XDC file:1 line:235 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[14]}]
set_property src_info {type:SCOPED_XDC file:1 line:236 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[14]}]
set_property src_info {type:SCOPED_XDC file:1 line:237 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[14]}]
set_property src_info {type:SCOPED_XDC file:1 line:238 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AH9 [get_ports {ddr3_addr[14]}]
set_property src_info {type:SCOPED_XDC file:1 line:239 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:240 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L1P_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:241 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[13]}]
set_property src_info {type:SCOPED_XDC file:1 line:242 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[13]}]
set_property src_info {type:SCOPED_XDC file:1 line:243 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[13]}]
set_property src_info {type:SCOPED_XDC file:1 line:244 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AA12 [get_ports {ddr3_addr[13]}]
set_property src_info {type:SCOPED_XDC file:1 line:245 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:246 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L1N_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:247 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[12]}]
set_property src_info {type:SCOPED_XDC file:1 line:248 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[12]}]
set_property src_info {type:SCOPED_XDC file:1 line:249 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[12]}]
set_property src_info {type:SCOPED_XDC file:1 line:250 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AB12 [get_ports {ddr3_addr[12]}]
set_property src_info {type:SCOPED_XDC file:1 line:251 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:252 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L2P_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:253 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[11]}]
set_property src_info {type:SCOPED_XDC file:1 line:254 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[11]}]
set_property src_info {type:SCOPED_XDC file:1 line:255 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[11]}]
set_property src_info {type:SCOPED_XDC file:1 line:256 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AA8 [get_ports {ddr3_addr[11]}]
set_property src_info {type:SCOPED_XDC file:1 line:257 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:258 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L2N_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:259 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[10]}]
set_property src_info {type:SCOPED_XDC file:1 line:260 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[10]}]
set_property src_info {type:SCOPED_XDC file:1 line:261 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[10]}]
set_property src_info {type:SCOPED_XDC file:1 line:262 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AB8 [get_ports {ddr3_addr[10]}]
set_property src_info {type:SCOPED_XDC file:1 line:263 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:264 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L4P_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:265 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[9]}]
set_property src_info {type:SCOPED_XDC file:1 line:266 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[9]}]
set_property src_info {type:SCOPED_XDC file:1 line:267 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[9]}]
set_property src_info {type:SCOPED_XDC file:1 line:268 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN Y11 [get_ports {ddr3_addr[9]}]
set_property src_info {type:SCOPED_XDC file:1 line:269 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:270 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L4N_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:271 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[8]}]
set_property src_info {type:SCOPED_XDC file:1 line:272 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[8]}]
set_property src_info {type:SCOPED_XDC file:1 line:273 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[8]}]
set_property src_info {type:SCOPED_XDC file:1 line:274 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN Y10 [get_ports {ddr3_addr[8]}]
set_property src_info {type:SCOPED_XDC file:1 line:275 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:276 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L5P_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:277 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[7]}]
set_property src_info {type:SCOPED_XDC file:1 line:278 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[7]}]
set_property src_info {type:SCOPED_XDC file:1 line:279 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[7]}]
set_property src_info {type:SCOPED_XDC file:1 line:280 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AA11 [get_ports {ddr3_addr[7]}]
set_property src_info {type:SCOPED_XDC file:1 line:281 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:282 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L5N_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:283 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[6]}]
set_property src_info {type:SCOPED_XDC file:1 line:284 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[6]}]
set_property src_info {type:SCOPED_XDC file:1 line:285 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[6]}]
set_property src_info {type:SCOPED_XDC file:1 line:286 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AA10 [get_ports {ddr3_addr[6]}]
set_property src_info {type:SCOPED_XDC file:1 line:287 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:288 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L6P_T0_33
set_property src_info {type:SCOPED_XDC file:1 line:289 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[5]}]
set_property src_info {type:SCOPED_XDC file:1 line:290 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[5]}]
set_property src_info {type:SCOPED_XDC file:1 line:291 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[5]}]
set_property src_info {type:SCOPED_XDC file:1 line:292 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AA13 [get_ports {ddr3_addr[5]}]
set_property src_info {type:SCOPED_XDC file:1 line:293 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:294 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L10P_T1_33
set_property src_info {type:SCOPED_XDC file:1 line:295 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[4]}]
set_property src_info {type:SCOPED_XDC file:1 line:296 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[4]}]
set_property src_info {type:SCOPED_XDC file:1 line:297 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[4]}]
set_property src_info {type:SCOPED_XDC file:1 line:298 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD9 [get_ports {ddr3_addr[4]}]
set_property src_info {type:SCOPED_XDC file:1 line:299 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:300 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L7N_T1_33
set_property src_info {type:SCOPED_XDC file:1 line:301 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:302 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:303 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:304 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC10 [get_ports {ddr3_addr[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:305 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:306 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L8P_T1_33
set_property src_info {type:SCOPED_XDC file:1 line:307 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:308 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:309 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:310 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD8 [get_ports {ddr3_addr[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:311 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:312 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L8N_T1_33
set_property src_info {type:SCOPED_XDC file:1 line:313 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:314 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:315 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:316 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AE8 [get_ports {ddr3_addr[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:317 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:318 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L9P_T1_DQS_33
set_property src_info {type:SCOPED_XDC file:1 line:319 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_addr[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:320 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_addr[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:321 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_addr[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:322 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC12 [get_ports {ddr3_addr[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:323 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:324 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L9N_T1_DQS_33
set_property src_info {type:SCOPED_XDC file:1 line:325 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_ba[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:326 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_ba[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:327 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_ba[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:328 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC11 [get_ports {ddr3_ba[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:329 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:330 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L7P_T1_33
set_property src_info {type:SCOPED_XDC file:1 line:331 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_ba[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:332 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_ba[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:333 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_ba[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:334 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AB10 [get_ports {ddr3_ba[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:335 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:336 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L10N_T1_33
set_property src_info {type:SCOPED_XDC file:1 line:337 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_ba[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:338 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_ba[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:339 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_ba[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:340 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AE9 [get_ports {ddr3_ba[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:341 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:342 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L11P_T1_SRCC_33
set_property src_info {type:SCOPED_XDC file:1 line:343 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports ddr3_ras_n]
set_property src_info {type:SCOPED_XDC file:1 line:344 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports ddr3_ras_n]
set_property src_info {type:SCOPED_XDC file:1 line:345 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports ddr3_ras_n]
set_property src_info {type:SCOPED_XDC file:1 line:346 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AE11 [get_ports ddr3_ras_n]
set_property src_info {type:SCOPED_XDC file:1 line:347 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:348 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L11N_T1_SRCC_33
set_property src_info {type:SCOPED_XDC file:1 line:349 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports ddr3_cas_n]
set_property src_info {type:SCOPED_XDC file:1 line:350 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports ddr3_cas_n]
set_property src_info {type:SCOPED_XDC file:1 line:351 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports ddr3_cas_n]
set_property src_info {type:SCOPED_XDC file:1 line:352 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AF11 [get_ports ddr3_cas_n]
set_property src_info {type:SCOPED_XDC file:1 line:353 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:354 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L24P_T3_33
set_property src_info {type:SCOPED_XDC file:1 line:355 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports ddr3_we_n]
set_property src_info {type:SCOPED_XDC file:1 line:356 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports ddr3_we_n]
set_property src_info {type:SCOPED_XDC file:1 line:357 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports ddr3_we_n]
set_property src_info {type:SCOPED_XDC file:1 line:358 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AG13 [get_ports ddr3_we_n]
set_property src_info {type:SCOPED_XDC file:1 line:359 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:360 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L12N_T1_MRCC_34
set_property src_info {type:SCOPED_XDC file:1 line:361 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports ddr3_reset_n]
set_property src_info {type:SCOPED_XDC file:1 line:362 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports ddr3_reset_n]
set_property src_info {type:SCOPED_XDC file:1 line:363 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD LVCMOS15 [get_ports ddr3_reset_n]
set_property src_info {type:SCOPED_XDC file:1 line:364 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AG5 [get_ports ddr3_reset_n]
set_property src_info {type:SCOPED_XDC file:1 line:365 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:366 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L15P_T2_DQS_33
set_property src_info {type:SCOPED_XDC file:1 line:367 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_cke[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:368 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_cke[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:369 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_cke[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:370 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AJ9 [get_ports {ddr3_cke[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:371 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:372 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L15N_T2_DQS_33
set_property src_info {type:SCOPED_XDC file:1 line:373 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_odt[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:374 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_odt[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:375 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_odt[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:376 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AK9 [get_ports {ddr3_odt[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:377 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:378 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L24N_T3_33
set_property src_info {type:SCOPED_XDC file:1 line:379 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_cs_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:380 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_cs_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:381 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_cs_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:382 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AH12 [get_ports {ddr3_cs_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:383 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:384 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L1P_T0_34
set_property src_info {type:SCOPED_XDC file:1 line:385 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:386 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dm[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:387 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:388 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD4 [get_ports {ddr3_dm[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:389 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:390 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L7P_T1_34
set_property src_info {type:SCOPED_XDC file:1 line:391 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:392 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dm[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:393 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:394 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AF3 [get_ports {ddr3_dm[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:395 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:396 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L13P_T2_MRCC_34
set_property src_info {type:SCOPED_XDC file:1 line:397 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:398 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dm[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:399 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:400 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AH4 [get_ports {ddr3_dm[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:401 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:402 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L19P_T3_34
set_property src_info {type:SCOPED_XDC file:1 line:403 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dm[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:404 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dm[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:405 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD SSTL15 [get_ports {ddr3_dm[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:406 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AF8 [get_ports {ddr3_dm[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:407 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:408 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L12P_T1_MRCC_33
set_property src_info {type:SCOPED_XDC file:1 line:409 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO DONTCARE [get_ports sys_clk_p]
set_property src_info {type:SCOPED_XDC file:1 line:410 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15 [get_ports sys_clk_p]
set_property src_info {type:SCOPED_XDC file:1 line:412 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:413 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L12N_T1_MRCC_33
set_property src_info {type:SCOPED_XDC file:1 line:415 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15 [get_ports sys_clk_n]
set_property src_info {type:SCOPED_XDC file:1 line:416 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD12 [get_ports sys_clk_p]
set_property src_info {type:SCOPED_XDC file:1 line:416 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD11 [get_ports sys_clk_n]
set_property src_info {type:SCOPED_XDC file:1 line:417 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:418 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L3P_T0_DQS_34
set_property src_info {type:SCOPED_XDC file:1 line:419 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:420 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dqs_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:421 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:423 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:424 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L3N_T0_DQS_34
set_property src_info {type:SCOPED_XDC file:1 line:426 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dqs_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:427 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:428 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD2 [get_ports {ddr3_dqs_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:428 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AD1 [get_ports {ddr3_dqs_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:429 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:430 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L9P_T1_DQS_34
set_property src_info {type:SCOPED_XDC file:1 line:431 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:432 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dqs_p[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:433 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:435 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:436 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L9N_T1_DQS_34
set_property src_info {type:SCOPED_XDC file:1 line:438 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dqs_n[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:439 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:440 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AG4 [get_ports {ddr3_dqs_p[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:440 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AG3 [get_ports {ddr3_dqs_n[1]}]
set_property src_info {type:SCOPED_XDC file:1 line:441 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:442 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L15P_T2_DQS_34
set_property src_info {type:SCOPED_XDC file:1 line:443 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:444 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dqs_p[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:445 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:447 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:448 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L15N_T2_DQS_34
set_property src_info {type:SCOPED_XDC file:1 line:450 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dqs_n[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:451 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:452 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AG2 [get_ports {ddr3_dqs_p[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:452 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AH1 [get_ports {ddr3_dqs_n[2]}]
set_property src_info {type:SCOPED_XDC file:1 line:453 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:454 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L21P_T3_DQS_34
set_property src_info {type:SCOPED_XDC file:1 line:455 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_dqs_p[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:456 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dqs_p[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:457 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_p[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:459 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:460 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L21N_T3_DQS_34
set_property src_info {type:SCOPED_XDC file:1 line:462 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_dqs_n[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:463 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddr3_dqs_n[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:464 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AH7 [get_ports {ddr3_dqs_p[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:464 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AJ7 [get_ports {ddr3_dqs_n[3]}]
set_property src_info {type:SCOPED_XDC file:1 line:465 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:466 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L3P_T0_DQS_33
set_property src_info {type:SCOPED_XDC file:1 line:467 export:INPUT save:INPUT read:READ} [current_design]
set_property VCCAUX_IO HIGH [get_ports {ddr3_ck_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:468 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_ck_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:469 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {ddr3_ck_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:471 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:472 export:INPUT save:INPUT read:READ} [current_design]
# PadFunction: IO_L3N_T0_DQS_33
set_property src_info {type:SCOPED_XDC file:1 line:474 export:INPUT save:INPUT read:READ} [current_design]
set_property SLEW FAST [get_ports {ddr3_ck_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:475 export:INPUT save:INPUT read:READ} [current_design]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {ddr3_ck_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:476 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AB9 [get_ports {ddr3_ck_p[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:476 export:INPUT save:INPUT read:READ} [current_design]
set_property PACKAGE_PIN AC9 [get_ports {ddr3_ck_n[0]}]
set_property src_info {type:SCOPED_XDC file:1 line:477 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:478 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:479 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:480 export:INPUT save:INPUT read:READ} [current_design]

current_instance i_ddr
set_property src_info {type:SCOPED_XDC file:1 line:481 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_OUT_PHY_X1Y7 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_out}]
set_property src_info {type:SCOPED_XDC file:1 line:482 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_OUT_PHY_X1Y6 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_out}]
set_property src_info {type:SCOPED_XDC file:1 line:483 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_OUT_PHY_X1Y5 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_out}]
set_property src_info {type:SCOPED_XDC file:1 line:484 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_OUT_PHY_X1Y4 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_out}]
set_property src_info {type:SCOPED_XDC file:1 line:485 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_OUT_PHY_X1Y11 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_out}]
set_property src_info {type:SCOPED_XDC file:1 line:486 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_OUT_PHY_X1Y10 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_out}]
set_property src_info {type:SCOPED_XDC file:1 line:487 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_OUT_PHY_X1Y9 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_out}]
set_property src_info {type:SCOPED_XDC file:1 line:488 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_OUT_PHY_X1Y8 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_out}]
set_property src_info {type:SCOPED_XDC file:1 line:489 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:490 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:491 export:INPUT save:INPUT read:READ} [current_design]
## set_property LOC PHASER_IN_PHY_X1Y7 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_in_gen.phaser_in}]
set_property src_info {type:SCOPED_XDC file:1 line:492 export:INPUT save:INPUT read:READ} [current_design]
## set_property LOC PHASER_IN_PHY_X1Y6 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_in_gen.phaser_in}]
set_property src_info {type:SCOPED_XDC file:1 line:493 export:INPUT save:INPUT read:READ} [current_design]
## set_property LOC PHASER_IN_PHY_X1Y5 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_in_gen.phaser_in}]
set_property src_info {type:SCOPED_XDC file:1 line:494 export:INPUT save:INPUT read:READ} [current_design]
## set_property LOC PHASER_IN_PHY_X1Y4 [get_cells  -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_in_gen.phaser_in}]
set_property src_info {type:SCOPED_XDC file:1 line:495 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_IN_PHY_X1Y11 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/phaser_in_gen.phaser_in}]
set_property src_info {type:SCOPED_XDC file:1 line:496 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_IN_PHY_X1Y10 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/phaser_in_gen.phaser_in}]
set_property src_info {type:SCOPED_XDC file:1 line:497 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_IN_PHY_X1Y9 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/phaser_in_gen.phaser_in}]
set_property src_info {type:SCOPED_XDC file:1 line:498 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_IN_PHY_X1Y8 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/phaser_in_gen.phaser_in}]
set_property src_info {type:SCOPED_XDC file:1 line:499 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:500 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:501 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:502 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:503 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:504 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OUT_FIFO_X1Y7 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/out_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:505 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OUT_FIFO_X1Y6 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/out_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:506 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OUT_FIFO_X1Y5 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/out_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:507 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OUT_FIFO_X1Y4 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/out_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:508 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OUT_FIFO_X1Y11 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/out_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:509 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OUT_FIFO_X1Y10 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/out_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:510 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OUT_FIFO_X1Y9 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/out_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:511 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OUT_FIFO_X1Y8 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/out_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:512 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:513 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:514 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC IN_FIFO_X1Y11 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/in_fifo_gen.in_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:515 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC IN_FIFO_X1Y10 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/in_fifo_gen.in_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:516 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC IN_FIFO_X1Y9 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/in_fifo_gen.in_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:517 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC IN_FIFO_X1Y8 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/in_fifo_gen.in_fifo}]
set_property src_info {type:SCOPED_XDC file:1 line:518 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:519 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:520 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHY_CONTROL_X1Y1 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/phy_control_i}]
set_property src_info {type:SCOPED_XDC file:1 line:521 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHY_CONTROL_X1Y2 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/phy_control_i}]
set_property src_info {type:SCOPED_XDC file:1 line:522 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:523 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:524 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_REF_X1Y1 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_1.u_ddr_phy_4lanes/phaser_ref_i}]
set_property src_info {type:SCOPED_XDC file:1 line:525 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PHASER_REF_X1Y2 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/phaser_ref_i}]
set_property src_info {type:SCOPED_XDC file:1 line:526 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:527 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:528 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OLOGIC_X1Y143 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_D.ddr_byte_lane_D/ddr_byte_group_io/*slave_ts}]
set_property src_info {type:SCOPED_XDC file:1 line:529 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OLOGIC_X1Y131 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_C.ddr_byte_lane_C/ddr_byte_group_io/*slave_ts}]
set_property src_info {type:SCOPED_XDC file:1 line:530 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OLOGIC_X1Y119 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_B.ddr_byte_lane_B/ddr_byte_group_io/*slave_ts}]
set_property src_info {type:SCOPED_XDC file:1 line:531 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC OLOGIC_X1Y107 [get_cells -hier -filter {NAME =~ */ddr_phy_4lanes_0.u_ddr_phy_4lanes/ddr_byte_lane_A.ddr_byte_lane_A/ddr_byte_group_io/*slave_ts}]
set_property src_info {type:SCOPED_XDC file:1 line:532 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:533 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:534 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:535 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC PLLE2_ADV_X1Y1 [get_cells -hier -filter {NAME =~ */u_ddr3_infrastructure/plle2_i}]
set_property src_info {type:SCOPED_XDC file:1 line:536 export:INPUT save:INPUT read:READ} [current_design]
set_property LOC MMCME2_ADV_X1Y1 [get_cells -hier -filter {NAME =~ */u_ddr3_infrastructure/gen_mmcm.mmcm_i}]
set_property src_info {type:SCOPED_XDC file:1 line:537 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:538 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:539 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:540 export:INPUT save:INPUT read:READ} [current_design]
set_multicycle_path -setup -from [get_cells -hier -filter {NAME =~ */mc0/mc_read_idle_r_reg}] -to [get_cells -hier -filter {NAME =~ */input_[?].iserdes_dq_.iserdesdq}] 6
set_property src_info {type:SCOPED_XDC file:1 line:543 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:544 export:INPUT save:INPUT read:READ} [current_design]
set_multicycle_path -hold -from [get_cells -hier -filter {NAME =~ */mc0/mc_read_idle_r_reg}] -to [get_cells -hier -filter {NAME =~ */input_[?].iserdes_dq_.iserdesdq}] 5
set_property src_info {type:SCOPED_XDC file:1 line:547 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:548 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -through [get_pins -filter {NAME =~ */DQSFOUND} -of [get_cells -hier -filter {REF_NAME == PHASER_IN_PHY}]]
set_property src_info {type:SCOPED_XDC file:1 line:549 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:550 export:INPUT save:INPUT read:READ} [current_design]
set_multicycle_path -setup -start -through [get_pins -filter {NAME =~ */OSERDESRST} -of [get_cells -hier -filter {REF_NAME == PHASER_OUT_PHY}]] 2
set_property src_info {type:SCOPED_XDC file:1 line:551 export:INPUT save:INPUT read:READ} [current_design]
set_multicycle_path -hold -start -through [get_pins -filter {NAME =~ */OSERDESRST} -of [get_cells -hier -filter {REF_NAME == PHASER_OUT_PHY}]] 1
set_property src_info {type:SCOPED_XDC file:1 line:552 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:553 export:INPUT save:INPUT read:READ} [current_design]
#set_max_delay -datapath_only -from [get_cells -hier -filter {NAME =~ *temp_mon_enabled.u_tempmon/* && IS_SEQUENTIAL}] -to [get_cells -hier -filter {NAME =~ *temp_mon_enabled.u_tempmon/device_temp_sync_r1*}] 20
set_property src_info {type:SCOPED_XDC file:1 line:554 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -to [get_pins -hier -include_replicated_objects -filter {NAME =~ *temp_mon_enabled.u_tempmon/device_temp_sync_r1_reg[*]/D}] 20.000
set_property src_info {type:SCOPED_XDC file:1 line:555 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath_only -from [get_cells -hier *rstdiv0_sync_r1_reg*] -to [get_pins -filter {NAME =~ */RESET} -of [get_cells -hier -filter {REF_NAME == PHY_CONTROL}]] 5.000
set_property src_info {type:SCOPED_XDC file:1 line:556 export:INPUT save:INPUT read:READ} [current_design]
#set_false_path -through [get_pins -hier -filter {NAME =~ */u_iodelay_ctrl/sys_rst}]
set_property src_info {type:SCOPED_XDC file:1 line:557 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -through [get_nets -hier -filter {NAME =~ */u_iodelay_ctrl/sys_rst_i}]
set_property src_info {type:SCOPED_XDC file:1 line:558 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:559 export:INPUT save:INPUT read:READ} [current_design]
set_max_delay -datapath_only -from [get_cells -hier -filter {NAME =~ *ddr3_infrastructure/rstdiv0_sync_r1_reg*}] -to [get_cells -hier -filter {NAME =~ *temp_mon_enabled.u_tempmon/xadc_supplied_temperature.rst_r1*}] 20.000
set_property src_info {type:SCOPED_XDC file:1 line:560 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:1 line:561 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:2 line:1 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:2 line:2 export:INPUT save:INPUT read:READ} [current_design]
# file: xlnx_clk_gen.xdc
set_property src_info {type:SCOPED_XDC file:2 line:3 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:2 line:4 export:INPUT save:INPUT read:READ} [current_design]
# (c) Copyright 2008 - 2013 Xilinx, Inc. All rights reserved.
set_property src_info {type:SCOPED_XDC file:2 line:5 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:2 line:6 export:INPUT save:INPUT read:READ} [current_design]
# This file contains confidential and proprietary information
set_property src_info {type:SCOPED_XDC file:2 line:7 export:INPUT save:INPUT read:READ} [current_design]
# of Xilinx, Inc. and is protected under U.S. and
set_property src_info {type:SCOPED_XDC file:2 line:8 export:INPUT save:INPUT read:READ} [current_design]
# international copyright and other intellectual property
set_property src_info {type:SCOPED_XDC file:2 line:9 export:INPUT save:INPUT read:READ} [current_design]
# laws.
set_property src_info {type:SCOPED_XDC file:2 line:10 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:2 line:11 export:INPUT save:INPUT read:READ} [current_design]
# DISCLAIMER
set_property src_info {type:SCOPED_XDC file:2 line:12 export:INPUT save:INPUT read:READ} [current_design]
# This disclaimer is not a license and does not grant any
set_property src_info {type:SCOPED_XDC file:2 line:13 export:INPUT save:INPUT read:READ} [current_design]
# rights to the materials distributed herewith. Except as
set_property src_info {type:SCOPED_XDC file:2 line:14 export:INPUT save:INPUT read:READ} [current_design]
# otherwise provided in a valid license issued to you by
set_property src_info {type:SCOPED_XDC file:2 line:15 export:INPUT save:INPUT read:READ} [current_design]
# Xilinx, and to the maximum extent permitted by applicable
set_property src_info {type:SCOPED_XDC file:2 line:16 export:INPUT save:INPUT read:READ} [current_design]
# law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
set_property src_info {type:SCOPED_XDC file:2 line:17 export:INPUT save:INPUT read:READ} [current_design]
# WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
set_property src_info {type:SCOPED_XDC file:2 line:18 export:INPUT save:INPUT read:READ} [current_design]
# AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
set_property src_info {type:SCOPED_XDC file:2 line:19 export:INPUT save:INPUT read:READ} [current_design]
# BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
set_property src_info {type:SCOPED_XDC file:2 line:20 export:INPUT save:INPUT read:READ} [current_design]
# INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
set_property src_info {type:SCOPED_XDC file:2 line:21 export:INPUT save:INPUT read:READ} [current_design]
# (2) Xilinx shall not be liable (whether in contract or tort,
set_property src_info {type:SCOPED_XDC file:2 line:22 export:INPUT save:INPUT read:READ} [current_design]
# including negligence, or under any other theory of
set_property src_info {type:SCOPED_XDC file:2 line:23 export:INPUT save:INPUT read:READ} [current_design]
# liability) for any loss or damage of any kind or nature
set_property src_info {type:SCOPED_XDC file:2 line:24 export:INPUT save:INPUT read:READ} [current_design]
# related to, arising under or in connection with these
set_property src_info {type:SCOPED_XDC file:2 line:25 export:INPUT save:INPUT read:READ} [current_design]
# materials, including for any direct, or any indirect,
set_property src_info {type:SCOPED_XDC file:2 line:26 export:INPUT save:INPUT read:READ} [current_design]
# special, incidental, or consequential loss or damage
set_property src_info {type:SCOPED_XDC file:2 line:27 export:INPUT save:INPUT read:READ} [current_design]
# (including loss of data, profits, goodwill, or any type of
set_property src_info {type:SCOPED_XDC file:2 line:28 export:INPUT save:INPUT read:READ} [current_design]
# loss or damage suffered as a result of any action brought
set_property src_info {type:SCOPED_XDC file:2 line:29 export:INPUT save:INPUT read:READ} [current_design]
# by a third party) even if such damage or loss was
set_property src_info {type:SCOPED_XDC file:2 line:30 export:INPUT save:INPUT read:READ} [current_design]
# reasonably foreseeable or Xilinx had been advised of the
set_property src_info {type:SCOPED_XDC file:2 line:31 export:INPUT save:INPUT read:READ} [current_design]
# possibility of the same.
set_property src_info {type:SCOPED_XDC file:2 line:32 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:2 line:33 export:INPUT save:INPUT read:READ} [current_design]
# CRITICAL APPLICATIONS
set_property src_info {type:SCOPED_XDC file:2 line:34 export:INPUT save:INPUT read:READ} [current_design]
# Xilinx products are not designed or intended to be fail-
set_property src_info {type:SCOPED_XDC file:2 line:35 export:INPUT save:INPUT read:READ} [current_design]
# safe, or for use in any application requiring fail-safe
set_property src_info {type:SCOPED_XDC file:2 line:36 export:INPUT save:INPUT read:READ} [current_design]
# performance, such as life-support or safety devices or
set_property src_info {type:SCOPED_XDC file:2 line:37 export:INPUT save:INPUT read:READ} [current_design]
# systems, Class III medical devices, nuclear facilities,
set_property src_info {type:SCOPED_XDC file:2 line:38 export:INPUT save:INPUT read:READ} [current_design]
# applications related to the deployment of airbags, or any
set_property src_info {type:SCOPED_XDC file:2 line:39 export:INPUT save:INPUT read:READ} [current_design]
# other applications that could lead to death, personal
set_property src_info {type:SCOPED_XDC file:2 line:40 export:INPUT save:INPUT read:READ} [current_design]
# injury, or severe property or environmental damage
set_property src_info {type:SCOPED_XDC file:2 line:41 export:INPUT save:INPUT read:READ} [current_design]
# (individually and collectively, "Critical
set_property src_info {type:SCOPED_XDC file:2 line:42 export:INPUT save:INPUT read:READ} [current_design]
# Applications"). Customer assumes the sole risk and
set_property src_info {type:SCOPED_XDC file:2 line:43 export:INPUT save:INPUT read:READ} [current_design]
# liability of any use of Xilinx products in Critical
set_property src_info {type:SCOPED_XDC file:2 line:44 export:INPUT save:INPUT read:READ} [current_design]
# Applications, subject only to applicable laws and
set_property src_info {type:SCOPED_XDC file:2 line:45 export:INPUT save:INPUT read:READ} [current_design]
# regulations governing limitations on product liability.
set_property src_info {type:SCOPED_XDC file:2 line:46 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:2 line:47 export:INPUT save:INPUT read:READ} [current_design]
# THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
set_property src_info {type:SCOPED_XDC file:2 line:48 export:INPUT save:INPUT read:READ} [current_design]
# PART OF THIS FILE AT ALL TIMES.
set_property src_info {type:SCOPED_XDC file:2 line:49 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:2 line:50 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:2 line:51 export:INPUT save:INPUT read:READ} [current_design]
# Input clock periods. These duplicate the values entered for the
set_property src_info {type:SCOPED_XDC file:2 line:52 export:INPUT save:INPUT read:READ} [current_design]
# input clocks. You can use these to time your system. If required
set_property src_info {type:SCOPED_XDC file:2 line:53 export:INPUT save:INPUT read:READ} [current_design]
# commented constraints can be used in the top level xdc
set_property src_info {type:SCOPED_XDC file:2 line:54 export:INPUT save:INPUT read:READ} [current_design]
#----------------------------------------------------------------
set_property src_info {type:SCOPED_XDC file:2 line:55 export:INPUT save:INPUT read:READ} [current_design]
# Connect to input port when clock capable pin is selected for input
current_instance -quiet
current_instance i_xlnx_clk_gen/inst
set_property src_info {type:SCOPED_XDC file:2 line:56 export:INPUT save:INPUT read:READ} [current_design]
create_clock -period 5.000 [get_ports -scoped_to_current_instance clk_in1]
set_property src_info {type:SCOPED_XDC file:2 line:57 export:INPUT save:INPUT read:READ} [current_design]
set_input_jitter [get_clocks -of_objects [get_ports -scoped_to_current_instance clk_in1]] 0.050
set_property src_info {type:SCOPED_XDC file:2 line:58 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:2 line:59 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:2 line:60 export:INPUT save:INPUT read:READ} [current_design]
set_property PHASESHIFT_MODE WAVEFORM [get_cells -hierarchical *adv*]
set_property src_info {type:SCOPED_XDC file:2 line:61 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:3 line:1 export:INPUT save:INPUT read:READ} [current_design]
# (c) Copyright 2009 - 2013 Xilinx, Inc. All rights reserved.
set_property src_info {type:SCOPED_XDC file:3 line:2 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:3 line:3 export:INPUT save:INPUT read:READ} [current_design]
# This file contains confidential and proprietary information
set_property src_info {type:SCOPED_XDC file:3 line:4 export:INPUT save:INPUT read:READ} [current_design]
# of Xilinx, Inc. and is protected under U.S. and
set_property src_info {type:SCOPED_XDC file:3 line:5 export:INPUT save:INPUT read:READ} [current_design]
# international copyright and other intellectual property
set_property src_info {type:SCOPED_XDC file:3 line:6 export:INPUT save:INPUT read:READ} [current_design]
# laws.
set_property src_info {type:SCOPED_XDC file:3 line:7 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:3 line:8 export:INPUT save:INPUT read:READ} [current_design]
# DISCLAIMER
set_property src_info {type:SCOPED_XDC file:3 line:9 export:INPUT save:INPUT read:READ} [current_design]
# This disclaimer is not a license and does not grant any
set_property src_info {type:SCOPED_XDC file:3 line:10 export:INPUT save:INPUT read:READ} [current_design]
# rights to the materials distributed herewith. Except as
set_property src_info {type:SCOPED_XDC file:3 line:11 export:INPUT save:INPUT read:READ} [current_design]
# otherwise provided in a valid license issued to you by
set_property src_info {type:SCOPED_XDC file:3 line:12 export:INPUT save:INPUT read:READ} [current_design]
# Xilinx, and to the maximum extent permitted by applicable
set_property src_info {type:SCOPED_XDC file:3 line:13 export:INPUT save:INPUT read:READ} [current_design]
# law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
set_property src_info {type:SCOPED_XDC file:3 line:14 export:INPUT save:INPUT read:READ} [current_design]
# WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
set_property src_info {type:SCOPED_XDC file:3 line:15 export:INPUT save:INPUT read:READ} [current_design]
# AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
set_property src_info {type:SCOPED_XDC file:3 line:16 export:INPUT save:INPUT read:READ} [current_design]
# BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
set_property src_info {type:SCOPED_XDC file:3 line:17 export:INPUT save:INPUT read:READ} [current_design]
# INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
set_property src_info {type:SCOPED_XDC file:3 line:18 export:INPUT save:INPUT read:READ} [current_design]
# (2) Xilinx shall not be liable (whether in contract or tort,
set_property src_info {type:SCOPED_XDC file:3 line:19 export:INPUT save:INPUT read:READ} [current_design]
# including negligence, or under any other theory of
set_property src_info {type:SCOPED_XDC file:3 line:20 export:INPUT save:INPUT read:READ} [current_design]
# liability) for any loss or damage of any kind or nature
set_property src_info {type:SCOPED_XDC file:3 line:21 export:INPUT save:INPUT read:READ} [current_design]
# related to, arising under or in connection with these
set_property src_info {type:SCOPED_XDC file:3 line:22 export:INPUT save:INPUT read:READ} [current_design]
# materials, including for any direct, or any indirect,
set_property src_info {type:SCOPED_XDC file:3 line:23 export:INPUT save:INPUT read:READ} [current_design]
# special, incidental, or consequential loss or damage
set_property src_info {type:SCOPED_XDC file:3 line:24 export:INPUT save:INPUT read:READ} [current_design]
# (including loss of data, profits, goodwill, or any type of
set_property src_info {type:SCOPED_XDC file:3 line:25 export:INPUT save:INPUT read:READ} [current_design]
# loss or damage suffered as a result of any action brought
set_property src_info {type:SCOPED_XDC file:3 line:26 export:INPUT save:INPUT read:READ} [current_design]
# by a third party) even if such damage or loss was
set_property src_info {type:SCOPED_XDC file:3 line:27 export:INPUT save:INPUT read:READ} [current_design]
# reasonably foreseeable or Xilinx had been advised of the
set_property src_info {type:SCOPED_XDC file:3 line:28 export:INPUT save:INPUT read:READ} [current_design]
# possibility of the same.
set_property src_info {type:SCOPED_XDC file:3 line:29 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:3 line:30 export:INPUT save:INPUT read:READ} [current_design]
# CRITICAL APPLICATIONS
set_property src_info {type:SCOPED_XDC file:3 line:31 export:INPUT save:INPUT read:READ} [current_design]
# Xilinx products are not designed or intended to be fail-
set_property src_info {type:SCOPED_XDC file:3 line:32 export:INPUT save:INPUT read:READ} [current_design]
# safe, or for use in any application requiring fail-safe
set_property src_info {type:SCOPED_XDC file:3 line:33 export:INPUT save:INPUT read:READ} [current_design]
# performance, such as life-support or safety devices or
set_property src_info {type:SCOPED_XDC file:3 line:34 export:INPUT save:INPUT read:READ} [current_design]
# systems, Class III medical devices, nuclear facilities,
set_property src_info {type:SCOPED_XDC file:3 line:35 export:INPUT save:INPUT read:READ} [current_design]
# applications related to the deployment of airbags, or any
set_property src_info {type:SCOPED_XDC file:3 line:36 export:INPUT save:INPUT read:READ} [current_design]
# other applications that could lead to death, personal
set_property src_info {type:SCOPED_XDC file:3 line:37 export:INPUT save:INPUT read:READ} [current_design]
# injury, or severe property or environmental damage
set_property src_info {type:SCOPED_XDC file:3 line:38 export:INPUT save:INPUT read:READ} [current_design]
# (individually and collectively, "Critical
set_property src_info {type:SCOPED_XDC file:3 line:39 export:INPUT save:INPUT read:READ} [current_design]
# Applications"). Customer assumes the sole risk and
set_property src_info {type:SCOPED_XDC file:3 line:40 export:INPUT save:INPUT read:READ} [current_design]
# liability of any use of Xilinx products in Critical
set_property src_info {type:SCOPED_XDC file:3 line:41 export:INPUT save:INPUT read:READ} [current_design]
# Applications, subject only to applicable laws and
set_property src_info {type:SCOPED_XDC file:3 line:42 export:INPUT save:INPUT read:READ} [current_design]
# regulations governing limitations on product liability.
set_property src_info {type:SCOPED_XDC file:3 line:43 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:3 line:44 export:INPUT save:INPUT read:READ} [current_design]
# THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
set_property src_info {type:SCOPED_XDC file:3 line:45 export:INPUT save:INPUT read:READ} [current_design]
# PART OF THIS FILE AT ALL TIMES.
set_property src_info {type:SCOPED_XDC file:3 line:46 export:INPUT save:INPUT read:READ} [current_design]

current_instance -quiet
current_instance i_ariane_peripherals/gen_gpio.i_xlnx_axi_gpio/U0
set_property src_info {type:SCOPED_XDC file:3 line:47 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -to [get_pins -hier *cdc_to*/D]
set_property src_info {type:SCOPED_XDC file:3 line:48 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:3 line:49 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:1 export:INPUT save:INPUT read:READ} [current_design]
# file: xlnx_axi_quad_spi.xdc
set_property src_info {type:SCOPED_XDC file:4 line:2 export:INPUT save:INPUT read:READ} [current_design]
# (c) Copyright 2009 - 2012 Xilinx, Inc. All rights reserved.
set_property src_info {type:SCOPED_XDC file:4 line:3 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:4 line:4 export:INPUT save:INPUT read:READ} [current_design]
# This file contains confidential and proprietary information
set_property src_info {type:SCOPED_XDC file:4 line:5 export:INPUT save:INPUT read:READ} [current_design]
# of Xilinx, Inc. and is protected under U.S. and
set_property src_info {type:SCOPED_XDC file:4 line:6 export:INPUT save:INPUT read:READ} [current_design]
# international copyright and other intellectual property
set_property src_info {type:SCOPED_XDC file:4 line:7 export:INPUT save:INPUT read:READ} [current_design]
# laws.
set_property src_info {type:SCOPED_XDC file:4 line:8 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:4 line:9 export:INPUT save:INPUT read:READ} [current_design]
# DISCLAIMER
set_property src_info {type:SCOPED_XDC file:4 line:10 export:INPUT save:INPUT read:READ} [current_design]
# This disclaimer is not a license and does not grant any
set_property src_info {type:SCOPED_XDC file:4 line:11 export:INPUT save:INPUT read:READ} [current_design]
# rights to the materials distributed herewith. Except as
set_property src_info {type:SCOPED_XDC file:4 line:12 export:INPUT save:INPUT read:READ} [current_design]
# otherwise provided in a valid license issued to you by
set_property src_info {type:SCOPED_XDC file:4 line:13 export:INPUT save:INPUT read:READ} [current_design]
# Xilinx, and to the maximum extent permitted by applicable
set_property src_info {type:SCOPED_XDC file:4 line:14 export:INPUT save:INPUT read:READ} [current_design]
# law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
set_property src_info {type:SCOPED_XDC file:4 line:15 export:INPUT save:INPUT read:READ} [current_design]
# WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
set_property src_info {type:SCOPED_XDC file:4 line:16 export:INPUT save:INPUT read:READ} [current_design]
# AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
set_property src_info {type:SCOPED_XDC file:4 line:17 export:INPUT save:INPUT read:READ} [current_design]
# BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
set_property src_info {type:SCOPED_XDC file:4 line:18 export:INPUT save:INPUT read:READ} [current_design]
# INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
set_property src_info {type:SCOPED_XDC file:4 line:19 export:INPUT save:INPUT read:READ} [current_design]
# (2) Xilinx shall not be liable (whether in contract or tort,
set_property src_info {type:SCOPED_XDC file:4 line:20 export:INPUT save:INPUT read:READ} [current_design]
# including negligence, or under any other theory of
set_property src_info {type:SCOPED_XDC file:4 line:21 export:INPUT save:INPUT read:READ} [current_design]
# liability) for any loss or damage of any kind or nature
set_property src_info {type:SCOPED_XDC file:4 line:22 export:INPUT save:INPUT read:READ} [current_design]
# related to, arising under or in connection with these
set_property src_info {type:SCOPED_XDC file:4 line:23 export:INPUT save:INPUT read:READ} [current_design]
# materials, including for any direct, or any indirect,
set_property src_info {type:SCOPED_XDC file:4 line:24 export:INPUT save:INPUT read:READ} [current_design]
# special, incidental, or consequential loss or damage
set_property src_info {type:SCOPED_XDC file:4 line:25 export:INPUT save:INPUT read:READ} [current_design]
# (including loss of data, profits, goodwill, or any type of
set_property src_info {type:SCOPED_XDC file:4 line:26 export:INPUT save:INPUT read:READ} [current_design]
# loss or damage suffered as a result of any action brought
set_property src_info {type:SCOPED_XDC file:4 line:27 export:INPUT save:INPUT read:READ} [current_design]
# by a third party) even if such damage or loss was
set_property src_info {type:SCOPED_XDC file:4 line:28 export:INPUT save:INPUT read:READ} [current_design]
# reasonably foreseeable or Xilinx had been advised of the
set_property src_info {type:SCOPED_XDC file:4 line:29 export:INPUT save:INPUT read:READ} [current_design]
# possibility of the same.
set_property src_info {type:SCOPED_XDC file:4 line:30 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:4 line:31 export:INPUT save:INPUT read:READ} [current_design]
# CRITICAL APPLICATIONS
set_property src_info {type:SCOPED_XDC file:4 line:32 export:INPUT save:INPUT read:READ} [current_design]
# Xilinx products are not designed or intended to be fail-
set_property src_info {type:SCOPED_XDC file:4 line:33 export:INPUT save:INPUT read:READ} [current_design]
# safe, or for use in any application requiring fail-safe
set_property src_info {type:SCOPED_XDC file:4 line:34 export:INPUT save:INPUT read:READ} [current_design]
# performance, such as life-support or safety devices or
set_property src_info {type:SCOPED_XDC file:4 line:35 export:INPUT save:INPUT read:READ} [current_design]
# systems, Class III medical devices, nuclear facilities,
set_property src_info {type:SCOPED_XDC file:4 line:36 export:INPUT save:INPUT read:READ} [current_design]
# applications related to the deployment of airbags, or any
set_property src_info {type:SCOPED_XDC file:4 line:37 export:INPUT save:INPUT read:READ} [current_design]
# other applications that could lead to death, personal
set_property src_info {type:SCOPED_XDC file:4 line:38 export:INPUT save:INPUT read:READ} [current_design]
# injury, or severe property or environmental damage
set_property src_info {type:SCOPED_XDC file:4 line:39 export:INPUT save:INPUT read:READ} [current_design]
# (individually and collectively, "Critical
set_property src_info {type:SCOPED_XDC file:4 line:40 export:INPUT save:INPUT read:READ} [current_design]
# Applications"). Customer assumes the sole risk and
set_property src_info {type:SCOPED_XDC file:4 line:41 export:INPUT save:INPUT read:READ} [current_design]
# liability of any use of Xilinx products in Critical
set_property src_info {type:SCOPED_XDC file:4 line:42 export:INPUT save:INPUT read:READ} [current_design]
# Applications, subject only to applicable laws and
set_property src_info {type:SCOPED_XDC file:4 line:43 export:INPUT save:INPUT read:READ} [current_design]
# regulations governing limitations on product liability.
set_property src_info {type:SCOPED_XDC file:4 line:44 export:INPUT save:INPUT read:READ} [current_design]
#
set_property src_info {type:SCOPED_XDC file:4 line:45 export:INPUT save:INPUT read:READ} [current_design]
# THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
set_property src_info {type:SCOPED_XDC file:4 line:46 export:INPUT save:INPUT read:READ} [current_design]
# PART OF THIS FILE AT ALL TIMES.
set_property src_info {type:SCOPED_XDC file:4 line:47 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:48 export:INPUT save:INPUT read:READ} [current_design]
## IOB constraints ######
current_instance -quiet
current_instance i_ariane_peripherals/gen_spi.i_xlnx_axi_quad_spi/U0
set_property src_info {type:SCOPED_XDC file:4 line:49 export:INPUT save:INPUT read:READ} [current_design]
set_property IOB TRUE [get_cells -hierarchical -filter {NAME =~*IO*_I_REG}]
set_property src_info {type:SCOPED_XDC file:4 line:50 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:51 export:INPUT save:INPUT read:READ} [current_design]
#####################################################################################################
set_property src_info {type:SCOPED_XDC file:4 line:52 export:INPUT save:INPUT read:READ} [current_design]
# The following section list the board specific constraints (with/without STARTUPE2/E3 primitive)   #
set_property src_info {type:SCOPED_XDC file:4 line:53 export:INPUT save:INPUT read:READ} [current_design]
# as per guidance given in product guide.                                                           #
set_property src_info {type:SCOPED_XDC file:4 line:54 export:INPUT save:INPUT read:READ} [current_design]
# User should uncomment, update constraints based on board delays and use                           #
set_property src_info {type:SCOPED_XDC file:4 line:55 export:INPUT save:INPUT read:READ} [current_design]
#####################################################################################################
set_property src_info {type:SCOPED_XDC file:4 line:56 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:57 export:INPUT save:INPUT read:READ} [current_design]
#####################################################################################################
set_property src_info {type:SCOPED_XDC file:4 line:58 export:INPUT save:INPUT read:READ} [current_design]
# STARTUPE2 primitive is not used                                                                   #
set_property src_info {type:SCOPED_XDC file:4 line:59 export:INPUT save:INPUT read:READ} [current_design]
#####################################################################################################
set_property src_info {type:SCOPED_XDC file:4 line:60 export:INPUT save:INPUT read:READ} [current_design]
#### All the delay numbers have to be provided by the user
set_property src_info {type:SCOPED_XDC file:4 line:61 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:62 export:INPUT save:INPUT read:READ} [current_design]
#### CCLK max delay is 6.7 ns ; refer Data sheet
set_property src_info {type:SCOPED_XDC file:4 line:63 export:INPUT save:INPUT read:READ} [current_design]
#### We need to consider the max delay for worst case analysis
set_property src_info {type:SCOPED_XDC file:4 line:64 export:INPUT save:INPUT read:READ} [current_design]
##set cclk_delay 6.7
set_property src_info {type:SCOPED_XDC file:4 line:65 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:66 export:INPUT save:INPUT read:READ} [current_design]
#### Following are the SPI device parameters
set_property src_info {type:SCOPED_XDC file:4 line:67 export:INPUT save:INPUT read:READ} [current_design]
#### Max Tco
set_property src_info {type:SCOPED_XDC file:4 line:68 export:INPUT save:INPUT read:READ} [current_design]
##set tco_max 7
set_property src_info {type:SCOPED_XDC file:4 line:69 export:INPUT save:INPUT read:READ} [current_design]
#### Min Tco
set_property src_info {type:SCOPED_XDC file:4 line:70 export:INPUT save:INPUT read:READ} [current_design]
##set tco_min 1
set_property src_info {type:SCOPED_XDC file:4 line:71 export:INPUT save:INPUT read:READ} [current_design]
#### Setup time requirement
set_property src_info {type:SCOPED_XDC file:4 line:72 export:INPUT save:INPUT read:READ} [current_design]
##set tsu 2
set_property src_info {type:SCOPED_XDC file:4 line:73 export:INPUT save:INPUT read:READ} [current_design]
#### Hold time requirement
set_property src_info {type:SCOPED_XDC file:4 line:74 export:INPUT save:INPUT read:READ} [current_design]
##set th 3
set_property src_info {type:SCOPED_XDC file:4 line:75 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:76 export:INPUT save:INPUT read:READ} [current_design]
#### Following are the board/trace delay numbers
set_property src_info {type:SCOPED_XDC file:4 line:77 export:INPUT save:INPUT read:READ} [current_design]
#### Assumption is that all Data lines are matched
set_property src_info {type:SCOPED_XDC file:4 line:78 export:INPUT save:INPUT read:READ} [current_design]
##set tdata_trace_delay_max 0.25
set_property src_info {type:SCOPED_XDC file:4 line:79 export:INPUT save:INPUT read:READ} [current_design]
##set tdata_trace_delay_min 0.25
set_property src_info {type:SCOPED_XDC file:4 line:80 export:INPUT save:INPUT read:READ} [current_design]
##set tclk_trace_delay_max 0.2
set_property src_info {type:SCOPED_XDC file:4 line:81 export:INPUT save:INPUT read:READ} [current_design]
##set tclk_trace_delay_min 0.2
set_property src_info {type:SCOPED_XDC file:4 line:82 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:83 export:INPUT save:INPUT read:READ} [current_design]
##### End of user provided delay numbers
set_property src_info {type:SCOPED_XDC file:4 line:84 export:INPUT save:INPUT read:READ} [current_design]
####create_generated_clock -name clk_sck -source [get_pins -hierarchical *axi_quad_spi_1/ext_spi_clk] [get_ports <SCK_IO>] -edges {3 5 7}
set_property src_info {type:SCOPED_XDC file:4 line:85 export:INPUT save:INPUT read:READ} [current_design]
##create_generated_clock -name clk_sck -source [get_pins -filter {REF_PIN_NAME==ext_spi_clk} -of [get_cells -hier -filter {REF_NAME=~axi_quad_spi_0}]] [get_ports SCK_IO] -edges {3 5 7}
set_property src_info {type:SCOPED_XDC file:4 line:86 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:87 export:INPUT save:INPUT read:READ} [current_design]
#### Data is captured into FPGA on the second rising edge of ext_spi_clk after the SCK falling edge
set_property src_info {type:SCOPED_XDC file:4 line:88 export:INPUT save:INPUT read:READ} [current_design]
#### Data is driven by the FPGA on every alternate rising_edge of ext_spi_clk
set_property src_info {type:SCOPED_XDC file:4 line:89 export:INPUT save:INPUT read:READ} [current_design]
##set_input_delay -clock clk_sck -max [expr $tco_max + $tdata_trace_delay_max + $tclk_trace_delay_max] [get_ports IO*_IO] -clock_fall;
set_property src_info {type:SCOPED_XDC file:4 line:90 export:INPUT save:INPUT read:READ} [current_design]
##set_input_delay -clock clk_sck -min [expr $tco_min + $tdata_trace_delay_min + $tclk_trace_delay_min] [get_ports IO*_IO] -clock_fall;
set_property src_info {type:SCOPED_XDC file:4 line:91 export:INPUT save:INPUT read:READ} [current_design]
##set_multicycle_path 2 -setup -from clk_sck -to [get_clocks -of_objects [get_pins -hierarchical */ext_spi_clk]]
set_property src_info {type:SCOPED_XDC file:4 line:92 export:INPUT save:INPUT read:READ} [current_design]
##set_multicycle_path 1 -hold -end -from clk_sck -to [get_clocks -of_objects [get_pins -hierarchical */ext_spi_clk]]
set_property src_info {type:SCOPED_XDC file:4 line:93 export:INPUT save:INPUT read:READ} [current_design]

set_property src_info {type:SCOPED_XDC file:4 line:94 export:INPUT save:INPUT read:READ} [current_design]
#### Data is captured into SPI on the following rising edge of SCK
set_property src_info {type:SCOPED_XDC file:4 line:95 export:INPUT save:INPUT read:READ} [current_design]
#### Data is driven by the IP on alternate rising_edge of the ext_spi_clk
set_property src_info {type:SCOPED_XDC file:4 line:96 export:INPUT save:INPUT read:READ} [current_design]
##set_output_delay -clock clk_sck -max [expr $tsu + $tdata_trace_delay_max - $tclk_trace_delay_min] [get_ports IO*_IO];
set_property src_info {type:SCOPED_XDC file:4 line:97 export:INPUT save:INPUT read:READ} [current_design]
##set_output_delay -clock clk_sck -min [expr $tdata_trace_delay_min - $th - $tclk_trace_delay_max] [get_ports IO*_IO];
set_property src_info {type:SCOPED_XDC file:4 line:98 export:INPUT save:INPUT read:READ} [current_design]
##set_multicycle_path 2 -setup -start -from [get_clocks -of_objects [get_pins -hierarchical */ext_spi_clk]] -to clk_sck
set_property src_info {type:SCOPED_XDC file:4 line:99 export:INPUT save:INPUT read:READ} [current_design]
##set_multicycle_path 1 -hold -from [get_clocks -of_objects [get_pins -hierarchical */ext_spi_clk]] -to clk_sck
set_property src_info {type:SCOPED_XDC file:4 line:100 export:INPUT save:INPUT read:READ} [current_design]

