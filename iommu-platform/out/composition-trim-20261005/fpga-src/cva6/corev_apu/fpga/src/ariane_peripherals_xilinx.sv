// Copyright 2018 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// Xilinx Peripherals

`include "axi/assign.svh"
`include "axi/typedef.svh"
`include "axi-iommu/assign.svh"
`include "axi-iommu/typedef.svh"
`include "register_interface/assign.svh"
`include "register_interface/typedef.svh"

module ariane_peripherals #(
    parameter int AxiAddrWidth = -1,
    parameter int AxiDataWidth = -1,
    parameter int AxiIdWidth   = -1,
    parameter int AxiUserWidth =  1,
    parameter bit InclUART     =  1,
    parameter bit InclSPI      =  0,
    parameter bit InclEthernet =  0,
    parameter bit InclGPIO     =  0,
    parameter bit InclTimer    =  1,
    parameter bit InclDMA      =  0,
    parameter bit InclIOMMU    =  0,
    parameter bit IOMMULegacyE3Observer = 1,
    parameter int unsigned IOMMUResearchRole = 0,
    parameter int unsigned IOMMUNonblockMode = 0,
    parameter int unsigned IOMMUDSFixedDelayInput  = 0,
    parameter int unsigned IOMMUDSFixedDelayOutput = 0
) (
    input  logic       clk_i           , // Clock
    input  logic       clk_200MHz_i    ,
    input  logic       rst_ni          , // Asynchronous reset active low
    AXI_BUS.Slave      plic            ,
    AXI_BUS.Slave      uart            ,
    AXI_BUS.Slave      spi             ,
    AXI_BUS.Slave      gpio            ,
    AXI_BUS.Slave      ethernet        ,
    AXI_BUS.Slave      timer           ,
    AXI_BUS.Slave      dma_cfg         , // DMA Engine configuration IF     (XBAR  => DMA  )
    AXI_BUS.Master     iommu_comp      , // IOMMU Completion IF             (IOMMU => XBAR )
    AXI_BUS.Master     iommu_ds        , // IOMMU Data Structures IF        (IOMMU => XBAR )
    AXI_BUS.Slave      iommu_prog      , // IOMMU Programming IF            (XBAR  => IOMMU)
    output logic [1:0] irq_o           ,
    // UART
    input  logic       rx_i            ,
    output logic       tx_o            ,
    // Ethernet
    input  logic       eth_clk_i       ,
    input  wire        eth_rxck        ,
    input  wire        eth_rxctl       ,
    input  wire [3:0]  eth_rxd         ,
    output wire        eth_txck        ,
    output wire        eth_txctl       ,
    output wire [3:0]  eth_txd         ,
    output wire        eth_rst_n       ,
    input  logic       phy_tx_clk_i    , // 125 MHz Clock
    // MDIO Interface
    inout  wire        eth_mdio        ,
    output logic       eth_mdc         ,
    // SPI
    output logic       spi_clk_o       ,
    output logic       spi_mosi        ,
    input  logic       spi_miso        ,
    output logic       spi_ss          ,
    // SD Card
    input  logic       sd_clk_i        ,
    output logic [7:0] leds_o          ,
    input  logic [7:0] dip_switches_i
);

    // ---------------
    // 1. PLIC
    // ---------------
    logic [ariane_soc::NumSources-1:0] irq_sources;

    // Unused interrupt sources
    assign irq_sources[(ariane_soc::NumSources-1):(ariane_soc::RVIOMMUCfg.NumIntVec+7)] = '0;

    REG_BUS #(
        .ADDR_WIDTH ( 32 ),
        .DATA_WIDTH ( 32 )
    ) reg_bus (clk_i);

    logic         plic_penable;
    logic         plic_pwrite;
    logic [31:0]  plic_paddr;
    logic         plic_psel;
    logic [31:0]  plic_pwdata;
    logic [31:0]  plic_prdata;
    logic         plic_pready;
    logic         plic_pslverr;

    axi2apb_64_32 #(
        .AXI4_ADDRESS_WIDTH ( AxiAddrWidth  ),
        .AXI4_RDATA_WIDTH   ( AxiDataWidth  ),
        .AXI4_WDATA_WIDTH   ( AxiDataWidth  ),
        .AXI4_ID_WIDTH      ( AxiIdWidth    ),
        .AXI4_USER_WIDTH    ( AxiUserWidth  ),
        .BUFF_DEPTH_SLAVE   ( 2             ),
        .APB_ADDR_WIDTH     ( 32            )
    ) i_axi2apb_64_32_plic (
        .ACLK      ( clk_i          ),
        .ARESETn   ( rst_ni         ),
        .test_en_i ( 1'b0           ),
        .AWID_i    ( plic.aw_id     ),
        .AWADDR_i  ( plic.aw_addr   ),
        .AWLEN_i   ( plic.aw_len    ),
        .AWSIZE_i  ( plic.aw_size   ),
        .AWBURST_i ( plic.aw_burst  ),
        .AWLOCK_i  ( plic.aw_lock   ),
        .AWCACHE_i ( plic.aw_cache  ),
        .AWPROT_i  ( plic.aw_prot   ),
        .AWREGION_i( plic.aw_region ),
        .AWUSER_i  ( plic.aw_user   ),
        .AWQOS_i   ( plic.aw_qos    ),
        .AWVALID_i ( plic.aw_valid  ),
        .AWREADY_o ( plic.aw_ready  ),
        .WDATA_i   ( plic.w_data    ),
        .WSTRB_i   ( plic.w_strb    ),
        .WLAST_i   ( plic.w_last    ),
        .WUSER_i   ( plic.w_user    ),
        .WVALID_i  ( plic.w_valid   ),
        .WREADY_o  ( plic.w_ready   ),
        .BID_o     ( plic.b_id      ),
        .BRESP_o   ( plic.b_resp    ),
        .BVALID_o  ( plic.b_valid   ),
        .BUSER_o   ( plic.b_user    ),
        .BREADY_i  ( plic.b_ready   ),
        .ARID_i    ( plic.ar_id     ),
        .ARADDR_i  ( plic.ar_addr   ),
        .ARLEN_i   ( plic.ar_len    ),
        .ARSIZE_i  ( plic.ar_size   ),
        .ARBURST_i ( plic.ar_burst  ),
        .ARLOCK_i  ( plic.ar_lock   ),
        .ARCACHE_i ( plic.ar_cache  ),
        .ARPROT_i  ( plic.ar_prot   ),
        .ARREGION_i( plic.ar_region ),
        .ARUSER_i  ( plic.ar_user   ),
        .ARQOS_i   ( plic.ar_qos    ),
        .ARVALID_i ( plic.ar_valid  ),
        .ARREADY_o ( plic.ar_ready  ),
        .RID_o     ( plic.r_id      ),
        .RDATA_o   ( plic.r_data    ),
        .RRESP_o   ( plic.r_resp    ),
        .RLAST_o   ( plic.r_last    ),
        .RUSER_o   ( plic.r_user    ),
        .RVALID_o  ( plic.r_valid   ),
        .RREADY_i  ( plic.r_ready   ),
        .PENABLE   ( plic_penable   ),
        .PWRITE    ( plic_pwrite    ),
        .PADDR     ( plic_paddr     ),
        .PSEL      ( plic_psel      ),
        .PWDATA    ( plic_pwdata    ),
        .PRDATA    ( plic_prdata    ),
        .PREADY    ( plic_pready    ),
        .PSLVERR   ( plic_pslverr   )
    );

    apb_to_reg i_apb_to_reg (
        .clk_i     ( clk_i        ),
        .rst_ni    ( rst_ni       ),
        .penable_i ( plic_penable ),
        .pwrite_i  ( plic_pwrite  ),
        .paddr_i   ( plic_paddr   ),
        .psel_i    ( plic_psel    ),
        .pwdata_i  ( plic_pwdata  ),
        .prdata_o  ( plic_prdata  ),
        .pready_o  ( plic_pready  ),
        .pslverr_o ( plic_pslverr ),
        .reg_o     ( reg_bus      )
    );

    // define reg type according to REG_BUS above
    `REG_BUS_TYPEDEF_ALL(plic, logic[31:0], logic[31:0], logic[3:0])
    plic_req_t plic_req;
    plic_rsp_t plic_rsp;

    // assign REG_BUS.out to (req_t, rsp_t) pair
    `REG_BUS_ASSIGN_TO_REQ(plic_req, reg_bus)
    `REG_BUS_ASSIGN_FROM_RSP(reg_bus, plic_rsp)

    plic_top #(
      .N_SOURCE    ( ariane_soc::NumSources  ),
      .N_TARGET    ( ariane_soc::NumTargets  ),
      .MAX_PRIO    ( ariane_soc::MaxPriority ),
      .reg_req_t   ( plic_req_t              ),
      .reg_rsp_t   ( plic_rsp_t              )
    ) i_plic (
      .clk_i,
      .rst_ni,
      .req_i         ( plic_req    ),
      .resp_o        ( plic_rsp    ),
      .le_i          ( '0          ), // 0:level 1:edge
      .irq_sources_i ( irq_sources ),
      .eip_targets_o ( irq_o       )
    );

    // ---------------
    // 2. UART
    // ---------------
    logic         uart_penable;
    logic         uart_pwrite;
    logic [31:0]  uart_paddr;
    logic         uart_psel;
    logic [31:0]  uart_pwdata;
    logic [31:0]  uart_prdata;
    logic         uart_pready;
    logic         uart_pslverr;

    axi2apb_64_32 #(
        .AXI4_ADDRESS_WIDTH ( AxiAddrWidth ),
        .AXI4_RDATA_WIDTH   ( AxiDataWidth ),
        .AXI4_WDATA_WIDTH   ( AxiDataWidth ),
        .AXI4_ID_WIDTH      ( AxiIdWidth   ),
        .AXI4_USER_WIDTH    ( AxiUserWidth ),
        .BUFF_DEPTH_SLAVE   ( 2            ),
        .APB_ADDR_WIDTH     ( 32           )
    ) i_axi2apb_64_32_uart (
        .ACLK      ( clk_i          ),
        .ARESETn   ( rst_ni         ),
        .test_en_i ( 1'b0           ),
        .AWID_i    ( uart.aw_id     ),
        .AWADDR_i  ( uart.aw_addr   ),
        .AWLEN_i   ( uart.aw_len    ),
        .AWSIZE_i  ( uart.aw_size   ),
        .AWBURST_i ( uart.aw_burst  ),
        .AWLOCK_i  ( uart.aw_lock   ),
        .AWCACHE_i ( uart.aw_cache  ),
        .AWPROT_i  ( uart.aw_prot   ),
        .AWREGION_i( uart.aw_region ),
        .AWUSER_i  ( uart.aw_user   ),
        .AWQOS_i   ( uart.aw_qos    ),
        .AWVALID_i ( uart.aw_valid  ),
        .AWREADY_o ( uart.aw_ready  ),
        .WDATA_i   ( uart.w_data    ),
        .WSTRB_i   ( uart.w_strb    ),
        .WLAST_i   ( uart.w_last    ),
        .WUSER_i   ( uart.w_user    ),
        .WVALID_i  ( uart.w_valid   ),
        .WREADY_o  ( uart.w_ready   ),
        .BID_o     ( uart.b_id      ),
        .BRESP_o   ( uart.b_resp    ),
        .BVALID_o  ( uart.b_valid   ),
        .BUSER_o   ( uart.b_user    ),
        .BREADY_i  ( uart.b_ready   ),
        .ARID_i    ( uart.ar_id     ),
        .ARADDR_i  ( uart.ar_addr   ),
        .ARLEN_i   ( uart.ar_len    ),
        .ARSIZE_i  ( uart.ar_size   ),
        .ARBURST_i ( uart.ar_burst  ),
        .ARLOCK_i  ( uart.ar_lock   ),
        .ARCACHE_i ( uart.ar_cache  ),
        .ARPROT_i  ( uart.ar_prot   ),
        .ARREGION_i( uart.ar_region ),
        .ARUSER_i  ( uart.ar_user   ),
        .ARQOS_i   ( uart.ar_qos    ),
        .ARVALID_i ( uart.ar_valid  ),
        .ARREADY_o ( uart.ar_ready  ),
        .RID_o     ( uart.r_id      ),
        .RDATA_o   ( uart.r_data    ),
        .RRESP_o   ( uart.r_resp    ),
        .RLAST_o   ( uart.r_last    ),
        .RUSER_o   ( uart.r_user    ),
        .RVALID_o  ( uart.r_valid   ),
        .RREADY_i  ( uart.r_ready   ),
        .PENABLE   ( uart_penable   ),
        .PWRITE    ( uart_pwrite    ),
        .PADDR     ( uart_paddr     ),
        .PSEL      ( uart_psel      ),
        .PWDATA    ( uart_pwdata    ),
        .PRDATA    ( uart_prdata    ),
        .PREADY    ( uart_pready    ),
        .PSLVERR   ( uart_pslverr   )
    );

    if (InclUART) begin : gen_uart
        apb_uart i_apb_uart (
            .CLK     ( clk_i           ),
            .RSTN    ( rst_ni          ),
            .PSEL    ( uart_psel       ),
            .PENABLE ( uart_penable    ),
            .PWRITE  ( uart_pwrite     ),
            .PADDR   ( uart_paddr[4:2] ),
            .PWDATA  ( uart_pwdata     ),
            .PRDATA  ( uart_prdata     ),
            .PREADY  ( uart_pready     ),
            .PSLVERR ( uart_pslverr    ),
            .INT     ( irq_sources[0]  ),
            .OUT1N   (                 ), // keep open
            .OUT2N   (                 ), // keep open
            .RTSN    (                 ), // no flow control
            .DTRN    (                 ), // no flow control
            .CTSN    ( 1'b0            ),
            .DSRN    ( 1'b0            ),
            .DCDN    ( 1'b0            ),
            .RIN     ( 1'b0            ),
            .SIN     ( rx_i            ),
            .SOUT    ( tx_o            )
        );
    end else begin
        /* pragma translate_off */
        `ifndef VERILATOR
        mock_uart i_mock_uart (
            .clk_i     ( clk_i        ),
            .rst_ni    ( rst_ni       ),
            .penable_i ( uart_penable ),
            .pwrite_i  ( uart_pwrite  ),
            .paddr_i   ( uart_paddr   ),
            .psel_i    ( uart_psel    ),
            .pwdata_i  ( uart_pwdata  ),
            .prdata_o  ( uart_prdata  ),
            .pready_o  ( uart_pready  ),
            .pslverr_o ( uart_pslverr )
        );
        `endif
        /* pragma translate_on */
    end

    // ---------------
    // 3. SPI
    // ---------------
    assign spi.b_user = 1'b0;
    assign spi.r_user = 1'b0;

    if (InclSPI) begin : gen_spi
        logic [31:0] s_axi_spi_awaddr;
        logic [7:0]  s_axi_spi_awlen;
        logic [2:0]  s_axi_spi_awsize;
        logic [1:0]  s_axi_spi_awburst;
        logic [0:0]  s_axi_spi_awlock;
        logic [3:0]  s_axi_spi_awcache;
        logic [2:0]  s_axi_spi_awprot;
        logic [3:0]  s_axi_spi_awregion;
        logic [3:0]  s_axi_spi_awqos;
        logic        s_axi_spi_awvalid;
        logic        s_axi_spi_awready;
        logic [31:0] s_axi_spi_wdata;
        logic [3:0]  s_axi_spi_wstrb;
        logic        s_axi_spi_wlast;
        logic        s_axi_spi_wvalid;
        logic        s_axi_spi_wready;
        logic [1:0]  s_axi_spi_bresp;
        logic        s_axi_spi_bvalid;
        logic        s_axi_spi_bready;
        logic [31:0] s_axi_spi_araddr;
        logic [7:0]  s_axi_spi_arlen;
        logic [2:0]  s_axi_spi_arsize;
        logic [1:0]  s_axi_spi_arburst;
        logic [0:0]  s_axi_spi_arlock;
        logic [3:0]  s_axi_spi_arcache;
        logic [2:0]  s_axi_spi_arprot;
        logic [3:0]  s_axi_spi_arregion;
        logic [3:0]  s_axi_spi_arqos;
        logic        s_axi_spi_arvalid;
        logic        s_axi_spi_arready;
        logic [31:0] s_axi_spi_rdata;
        logic [1:0]  s_axi_spi_rresp;
        logic        s_axi_spi_rlast;
        logic        s_axi_spi_rvalid;
        logic        s_axi_spi_rready;

        xlnx_axi_dwidth_converter i_xlnx_axi_dwidth_converter_spi (
            .s_axi_aclk     ( clk_i              ),
            .s_axi_aresetn  ( rst_ni             ),

            .s_axi_awid     ( spi.aw_id          ),
            .s_axi_awaddr   ( spi.aw_addr[31:0]  ),
            .s_axi_awlen    ( spi.aw_len         ),
            .s_axi_awsize   ( spi.aw_size        ),
            .s_axi_awburst  ( spi.aw_burst       ),
            .s_axi_awlock   ( spi.aw_lock        ),
            .s_axi_awcache  ( spi.aw_cache       ),
            .s_axi_awprot   ( spi.aw_prot        ),
            .s_axi_awregion ( spi.aw_region      ),
            .s_axi_awqos    ( spi.aw_qos         ),
            .s_axi_awvalid  ( spi.aw_valid       ),
            .s_axi_awready  ( spi.aw_ready       ),
            .s_axi_wdata    ( spi.w_data         ),
            .s_axi_wstrb    ( spi.w_strb         ),
            .s_axi_wlast    ( spi.w_last         ),
            .s_axi_wvalid   ( spi.w_valid        ),
            .s_axi_wready   ( spi.w_ready        ),
            .s_axi_bid      ( spi.b_id           ),
            .s_axi_bresp    ( spi.b_resp         ),
            .s_axi_bvalid   ( spi.b_valid        ),
            .s_axi_bready   ( spi.b_ready        ),
            .s_axi_arid     ( spi.ar_id          ),
            .s_axi_araddr   ( spi.ar_addr[31:0]  ),
            .s_axi_arlen    ( spi.ar_len         ),
            .s_axi_arsize   ( spi.ar_size        ),
            .s_axi_arburst  ( spi.ar_burst       ),
            .s_axi_arlock   ( spi.ar_lock        ),
            .s_axi_arcache  ( spi.ar_cache       ),
            .s_axi_arprot   ( spi.ar_prot        ),
            .s_axi_arregion ( spi.ar_region      ),
            .s_axi_arqos    ( spi.ar_qos         ),
            .s_axi_arvalid  ( spi.ar_valid       ),
            .s_axi_arready  ( spi.ar_ready       ),
            .s_axi_rid      ( spi.r_id           ),
            .s_axi_rdata    ( spi.r_data         ),
            .s_axi_rresp    ( spi.r_resp         ),
            .s_axi_rlast    ( spi.r_last         ),
            .s_axi_rvalid   ( spi.r_valid        ),
            .s_axi_rready   ( spi.r_ready        ),

            .m_axi_awaddr   ( s_axi_spi_awaddr   ),
            .m_axi_awlen    ( s_axi_spi_awlen    ),
            .m_axi_awsize   ( s_axi_spi_awsize   ),
            .m_axi_awburst  ( s_axi_spi_awburst  ),
            .m_axi_awlock   ( s_axi_spi_awlock   ),
            .m_axi_awcache  ( s_axi_spi_awcache  ),
            .m_axi_awprot   ( s_axi_spi_awprot   ),
            .m_axi_awregion ( s_axi_spi_awregion ),
            .m_axi_awqos    ( s_axi_spi_awqos    ),
            .m_axi_awvalid  ( s_axi_spi_awvalid  ),
            .m_axi_awready  ( s_axi_spi_awready  ),
            .m_axi_wdata    ( s_axi_spi_wdata    ),
            .m_axi_wstrb    ( s_axi_spi_wstrb    ),
            .m_axi_wlast    ( s_axi_spi_wlast    ),
            .m_axi_wvalid   ( s_axi_spi_wvalid   ),
            .m_axi_wready   ( s_axi_spi_wready   ),
            .m_axi_bresp    ( s_axi_spi_bresp    ),
            .m_axi_bvalid   ( s_axi_spi_bvalid   ),
            .m_axi_bready   ( s_axi_spi_bready   ),
            .m_axi_araddr   ( s_axi_spi_araddr   ),
            .m_axi_arlen    ( s_axi_spi_arlen    ),
            .m_axi_arsize   ( s_axi_spi_arsize   ),
            .m_axi_arburst  ( s_axi_spi_arburst  ),
            .m_axi_arlock   ( s_axi_spi_arlock   ),
            .m_axi_arcache  ( s_axi_spi_arcache  ),
            .m_axi_arprot   ( s_axi_spi_arprot   ),
            .m_axi_arregion ( s_axi_spi_arregion ),
            .m_axi_arqos    ( s_axi_spi_arqos    ),
            .m_axi_arvalid  ( s_axi_spi_arvalid  ),
            .m_axi_arready  ( s_axi_spi_arready  ),
            .m_axi_rdata    ( s_axi_spi_rdata    ),
            .m_axi_rresp    ( s_axi_spi_rresp    ),
            .m_axi_rlast    ( s_axi_spi_rlast    ),
            .m_axi_rvalid   ( s_axi_spi_rvalid   ),
            .m_axi_rready   ( s_axi_spi_rready   )
        );

        xlnx_axi_quad_spi i_xlnx_axi_quad_spi (
            .ext_spi_clk    ( clk_i                  ),
            .s_axi4_aclk    ( clk_i                  ),
            .s_axi4_aresetn ( rst_ni                 ),
            .s_axi4_awaddr  ( s_axi_spi_awaddr[23:0] ),
            .s_axi4_awlen   ( s_axi_spi_awlen        ),
            .s_axi4_awsize  ( s_axi_spi_awsize       ),
            .s_axi4_awburst ( s_axi_spi_awburst      ),
            .s_axi4_awlock  ( s_axi_spi_awlock       ),
            .s_axi4_awcache ( s_axi_spi_awcache      ),
            .s_axi4_awprot  ( s_axi_spi_awprot       ),
            .s_axi4_awvalid ( s_axi_spi_awvalid      ),
            .s_axi4_awready ( s_axi_spi_awready      ),
            .s_axi4_wdata   ( s_axi_spi_wdata        ),
            .s_axi4_wstrb   ( s_axi_spi_wstrb        ),
            .s_axi4_wlast   ( s_axi_spi_wlast        ),
            .s_axi4_wvalid  ( s_axi_spi_wvalid       ),
            .s_axi4_wready  ( s_axi_spi_wready       ),
            .s_axi4_bresp   ( s_axi_spi_bresp        ),
            .s_axi4_bvalid  ( s_axi_spi_bvalid       ),
            .s_axi4_bready  ( s_axi_spi_bready       ),
            .s_axi4_araddr  ( s_axi_spi_araddr[23:0] ),
            .s_axi4_arlen   ( s_axi_spi_arlen        ),
            .s_axi4_arsize  ( s_axi_spi_arsize       ),
            .s_axi4_arburst ( s_axi_spi_arburst      ),
            .s_axi4_arlock  ( s_axi_spi_arlock       ),
            .s_axi4_arcache ( s_axi_spi_arcache      ),
            .s_axi4_arprot  ( s_axi_spi_arprot       ),
            .s_axi4_arvalid ( s_axi_spi_arvalid      ),
            .s_axi4_arready ( s_axi_spi_arready      ),
            .s_axi4_rdata   ( s_axi_spi_rdata        ),
            .s_axi4_rresp   ( s_axi_spi_rresp        ),
            .s_axi4_rlast   ( s_axi_spi_rlast        ),
            .s_axi4_rvalid  ( s_axi_spi_rvalid       ),
            .s_axi4_rready  ( s_axi_spi_rready       ),
            .io0_i          ( '0                     ),
            .io0_o          ( spi_mosi               ),
            .io0_t          (                        ),
            .io1_i          ( spi_miso               ),
            .io1_o          (                        ),
            .io1_t          (                        ),
            .ss_i           ( '0                     ),
            .ss_o           ( spi_ss                 ),
            .ss_t           (                        ),
            .sck_o          ( spi_clk_o              ),
            .sck_i          ( '0                     ),
            .sck_t          (                        ),
            .ip2intc_irpt   ( irq_sources[1]         )
        );
    end else begin
        assign spi_clk_o = 1'b0;
        assign spi_mosi = 1'b0;
        assign spi_ss = 1'b0;

        // assign irq_sources [1] = 1'b0;
        assign spi.aw_ready = 1'b1;
        assign spi.ar_ready = 1'b1;
        assign spi.w_ready = 1'b1;

        assign spi.b_valid = spi.aw_valid;
        assign spi.b_id = spi.aw_id;
        assign spi.b_resp = axi_pkg::RESP_SLVERR;
        assign spi.b_user = '0;

        assign spi.r_valid = spi.ar_valid;
        assign spi.r_resp = axi_pkg::RESP_SLVERR;
        assign spi.r_data = 'hdeadbeef;
        assign spi.r_last = 1'b1;
    end


    // ---------------
    // 4. Ethernet
    // ---------------

    if (InclEthernet) begin : gen_ethernet

    logic                    clk_200_int, clk_rgmii, clk_rgmii_quad;
    logic                    eth_en, eth_we, eth_int_n, eth_pme_n, eth_mdio_i, eth_mdio_o, eth_mdio_oe;
    logic [AxiAddrWidth-1:0] eth_addr;
    logic [AxiDataWidth-1:0] eth_wrdata, eth_rdata;
    logic [AxiDataWidth/8-1:0] eth_be;

    axi2mem #(
        .AXI_ID_WIDTH   ( AxiIdWidth       ),
        .AXI_ADDR_WIDTH ( AxiAddrWidth     ),
        .AXI_DATA_WIDTH ( AxiDataWidth     ),
        .AXI_USER_WIDTH ( AxiUserWidth     )
    ) i_axi2rom (
        .clk_i  ( clk_i                   ),
        .rst_ni ( rst_ni                  ),
        .slave  ( ethernet                ),
        .req_o  ( eth_en                  ),
        .we_o   ( eth_we                  ),
        .addr_o ( eth_addr                ),
        .be_o   ( eth_be                  ),
        .data_o ( eth_wrdata              ),
        .data_i ( eth_rdata               )
    );

    framing_top eth_rgmii (
       .msoc_clk(clk_i),
       .core_lsu_addr(eth_addr[14:0]),
       .core_lsu_wdata(eth_wrdata),
       .core_lsu_be(eth_be),
       .ce_d(eth_en),
       .we_d(eth_en & eth_we),
       .framing_sel(eth_en),
       .framing_rdata(eth_rdata),
       .rst_int(!rst_ni),
       .clk_int(phy_tx_clk_i), // 125 MHz in-phase
       .clk90_int(eth_clk_i),    // 125 MHz quadrature
       .clk_200_int(clk_200MHz_i),
       /*
        * Ethernet: 1000BASE-T RGMII
        */
       .phy_rx_clk(eth_rxck),
       .phy_rxd(eth_rxd),
       .phy_rx_ctl(eth_rxctl),
       .phy_tx_clk(eth_txck),
       .phy_txd(eth_txd),
       .phy_tx_ctl(eth_txctl),
       .phy_reset_n(eth_rst_n),
       .phy_int_n(eth_int_n),
       .phy_pme_n(eth_pme_n),
       .phy_mdc(eth_mdc),
       .phy_mdio_i(eth_mdio_i),
       .phy_mdio_o(eth_mdio_o),
       .phy_mdio_oe(eth_mdio_oe),
       .eth_irq(irq_sources[2])
    );

       IOBUF #(
          .DRIVE(12), // Specify the output drive strength
          .IBUF_LOW_PWR("TRUE"),  // Low Power - "TRUE", High Performance = "FALSE"
          .IOSTANDARD("DEFAULT"), // Specify the I/O standard
          .SLEW("SLOW") // Specify the output slew rate
       ) IOBUF_inst (
          .O(eth_mdio_i),     // Buffer output
          .IO(eth_mdio),   // Buffer inout port (connect directly to top-level port)
          .I(eth_mdio_o),     // Buffer input
          .T(~eth_mdio_oe)      // 3-state enable input, high=input, low=output
       );

    end else begin
        assign irq_sources [2] = 1'b0;
        assign ethernet.aw_ready = 1'b1;
        assign ethernet.ar_ready = 1'b1;
        assign ethernet.w_ready = 1'b1;

        assign ethernet.b_valid = ethernet.aw_valid;
        assign ethernet.b_id = ethernet.aw_id;
        assign ethernet.b_resp = axi_pkg::RESP_SLVERR;
        assign ethernet.b_user = '0;

        assign ethernet.r_valid = ethernet.ar_valid;
        assign ethernet.r_resp = axi_pkg::RESP_SLVERR;
        assign ethernet.r_data = 'hdeadbeef;
        assign ethernet.r_last = 1'b1;
    end

    // 5. GPIO
    assign gpio.b_user = 1'b0;
    assign gpio.r_user = 1'b0;

    if (InclGPIO) begin : gen_gpio

        logic [31:0] s_axi_gpio_awaddr;
        logic [7:0]  s_axi_gpio_awlen;
        logic [2:0]  s_axi_gpio_awsize;
        logic [1:0]  s_axi_gpio_awburst;
        logic [3:0]  s_axi_gpio_awcache;
        logic        s_axi_gpio_awvalid;
        logic        s_axi_gpio_awready;
        logic [31:0] s_axi_gpio_wdata;
        logic [3:0]  s_axi_gpio_wstrb;
        logic        s_axi_gpio_wvalid;
        logic        s_axi_gpio_wready;
        logic [1:0]  s_axi_gpio_bresp;
        logic        s_axi_gpio_bvalid;
        logic        s_axi_gpio_bready;
        logic [31:0] s_axi_gpio_araddr;
        logic [7:0]  s_axi_gpio_arlen;
        logic [2:0]  s_axi_gpio_arsize;
        logic [1:0]  s_axi_gpio_arburst;
        logic [3:0]  s_axi_gpio_arcache;
        logic        s_axi_gpio_arvalid;
        logic        s_axi_gpio_arready;
        logic [31:0] s_axi_gpio_rdata;
        logic [1:0]  s_axi_gpio_rresp;
        logic        s_axi_gpio_rlast;
        logic        s_axi_gpio_rvalid;
        logic        s_axi_gpio_rready;

        // system-bus is 64-bit, convert down to 32 bit
        xlnx_axi_dwidth_converter i_xlnx_axi_dwidth_converter_gpio (
            .s_axi_aclk     ( clk_i              ),
            .s_axi_aresetn  ( rst_ni             ),
            .s_axi_awid     ( gpio.aw_id         ),
            .s_axi_awaddr   ( gpio.aw_addr[31:0] ),
            .s_axi_awlen    ( gpio.aw_len        ),
            .s_axi_awsize   ( gpio.aw_size       ),
            .s_axi_awburst  ( gpio.aw_burst      ),
            .s_axi_awlock   ( gpio.aw_lock       ),
            .s_axi_awcache  ( gpio.aw_cache      ),
            .s_axi_awprot   ( gpio.aw_prot       ),
            .s_axi_awregion ( gpio.aw_region     ),
            .s_axi_awqos    ( gpio.aw_qos        ),
            .s_axi_awvalid  ( gpio.aw_valid      ),
            .s_axi_awready  ( gpio.aw_ready      ),
            .s_axi_wdata    ( gpio.w_data        ),
            .s_axi_wstrb    ( gpio.w_strb        ),
            .s_axi_wlast    ( gpio.w_last        ),
            .s_axi_wvalid   ( gpio.w_valid       ),
            .s_axi_wready   ( gpio.w_ready       ),
            .s_axi_bid      ( gpio.b_id          ),
            .s_axi_bresp    ( gpio.b_resp        ),
            .s_axi_bvalid   ( gpio.b_valid       ),
            .s_axi_bready   ( gpio.b_ready       ),
            .s_axi_arid     ( gpio.ar_id         ),
            .s_axi_araddr   ( gpio.ar_addr[31:0] ),
            .s_axi_arlen    ( gpio.ar_len        ),
            .s_axi_arsize   ( gpio.ar_size       ),
            .s_axi_arburst  ( gpio.ar_burst      ),
            .s_axi_arlock   ( gpio.ar_lock       ),
            .s_axi_arcache  ( gpio.ar_cache      ),
            .s_axi_arprot   ( gpio.ar_prot       ),
            .s_axi_arregion ( gpio.ar_region     ),
            .s_axi_arqos    ( gpio.ar_qos        ),
            .s_axi_arvalid  ( gpio.ar_valid      ),
            .s_axi_arready  ( gpio.ar_ready      ),
            .s_axi_rid      ( gpio.r_id          ),
            .s_axi_rdata    ( gpio.r_data        ),
            .s_axi_rresp    ( gpio.r_resp        ),
            .s_axi_rlast    ( gpio.r_last        ),
            .s_axi_rvalid   ( gpio.r_valid       ),
            .s_axi_rready   ( gpio.r_ready       ),

            .m_axi_awaddr   ( s_axi_gpio_awaddr  ),
            .m_axi_awlen    ( s_axi_gpio_awlen   ),
            .m_axi_awsize   ( s_axi_gpio_awsize  ),
            .m_axi_awburst  ( s_axi_gpio_awburst ),
            .m_axi_awlock   (                    ),
            .m_axi_awcache  ( s_axi_gpio_awcache ),
            .m_axi_awprot   (                    ),
            .m_axi_awregion (                    ),
            .m_axi_awqos    (                    ),
            .m_axi_awvalid  ( s_axi_gpio_awvalid ),
            .m_axi_awready  ( s_axi_gpio_awready ),
            .m_axi_wdata    ( s_axi_gpio_wdata   ),
            .m_axi_wstrb    ( s_axi_gpio_wstrb   ),
            .m_axi_wlast    (                    ),
            .m_axi_wvalid   ( s_axi_gpio_wvalid  ),
            .m_axi_wready   ( s_axi_gpio_wready  ),
            .m_axi_bresp    ( s_axi_gpio_bresp   ),
            .m_axi_bvalid   ( s_axi_gpio_bvalid  ),
            .m_axi_bready   ( s_axi_gpio_bready  ),
            .m_axi_araddr   ( s_axi_gpio_araddr  ),
            .m_axi_arlen    ( s_axi_gpio_arlen   ),
            .m_axi_arsize   ( s_axi_gpio_arsize  ),
            .m_axi_arburst  ( s_axi_gpio_arburst ),
            .m_axi_arlock   (                    ),
            .m_axi_arcache  ( s_axi_gpio_arcache ),
            .m_axi_arprot   (                    ),
            .m_axi_arregion (                    ),
            .m_axi_arqos    (                    ),
            .m_axi_arvalid  ( s_axi_gpio_arvalid ),
            .m_axi_arready  ( s_axi_gpio_arready ),
            .m_axi_rdata    ( s_axi_gpio_rdata   ),
            .m_axi_rresp    ( s_axi_gpio_rresp   ),
            .m_axi_rlast    ( s_axi_gpio_rlast   ),
            .m_axi_rvalid   ( s_axi_gpio_rvalid  ),
            .m_axi_rready   ( s_axi_gpio_rready  )
        );

        xlnx_axi_gpio i_xlnx_axi_gpio (
            .s_axi_aclk    ( clk_i                  ),
            .s_axi_aresetn ( rst_ni                 ),
            .s_axi_awaddr  ( s_axi_gpio_awaddr[8:0] ),
            .s_axi_awvalid ( s_axi_gpio_awvalid     ),
            .s_axi_awready ( s_axi_gpio_awready     ),
            .s_axi_wdata   ( s_axi_gpio_wdata       ),
            .s_axi_wstrb   ( s_axi_gpio_wstrb       ),
            .s_axi_wvalid  ( s_axi_gpio_wvalid      ),
            .s_axi_wready  ( s_axi_gpio_wready      ),
            .s_axi_bresp   ( s_axi_gpio_bresp       ),
            .s_axi_bvalid  ( s_axi_gpio_bvalid      ),
            .s_axi_bready  ( s_axi_gpio_bready      ),
            .s_axi_araddr  ( s_axi_gpio_araddr[8:0] ),
            .s_axi_arvalid ( s_axi_gpio_arvalid     ),
            .s_axi_arready ( s_axi_gpio_arready     ),
            .s_axi_rdata   ( s_axi_gpio_rdata       ),
            .s_axi_rresp   ( s_axi_gpio_rresp       ),
            .s_axi_rvalid  ( s_axi_gpio_rvalid      ),
            .s_axi_rready  ( s_axi_gpio_rready      ),
            .gpio_io_i     ( '0                     ),
            .gpio_io_o     ( leds_o                 ),
            .gpio_io_t     (                        ),
            .gpio2_io_i    ( dip_switches_i         )
        );

        assign s_axi_gpio_rlast = 1'b1;

    end

    // 6. Timer
    if (InclTimer) begin : gen_timer
        logic         timer_penable;
        logic         timer_pwrite;
        logic [31:0]  timer_paddr;
        logic         timer_psel;
        logic [31:0]  timer_pwdata;
        logic [31:0]  timer_prdata;
        logic         timer_pready;
        logic         timer_pslverr;

        axi2apb_64_32 #(
            .AXI4_ADDRESS_WIDTH ( AxiAddrWidth ),
            .AXI4_RDATA_WIDTH   ( AxiDataWidth ),
            .AXI4_WDATA_WIDTH   ( AxiDataWidth ),
            .AXI4_ID_WIDTH      ( AxiIdWidth   ),
            .AXI4_USER_WIDTH    ( AxiUserWidth ),
            .BUFF_DEPTH_SLAVE   ( 2            ),
            .APB_ADDR_WIDTH     ( 32           )
        ) i_axi2apb_64_32_timer (
            .ACLK      ( clk_i           ),
            .ARESETn   ( rst_ni          ),
            .test_en_i ( 1'b0            ),
            .AWID_i    ( timer.aw_id     ),
            .AWADDR_i  ( timer.aw_addr   ),
            .AWLEN_i   ( timer.aw_len    ),
            .AWSIZE_i  ( timer.aw_size   ),
            .AWBURST_i ( timer.aw_burst  ),
            .AWLOCK_i  ( timer.aw_lock   ),
            .AWCACHE_i ( timer.aw_cache  ),
            .AWPROT_i  ( timer.aw_prot   ),
            .AWREGION_i( timer.aw_region ),
            .AWUSER_i  ( timer.aw_user   ),
            .AWQOS_i   ( timer.aw_qos    ),
            .AWVALID_i ( timer.aw_valid  ),
            .AWREADY_o ( timer.aw_ready  ),
            .WDATA_i   ( timer.w_data    ),
            .WSTRB_i   ( timer.w_strb    ),
            .WLAST_i   ( timer.w_last    ),
            .WUSER_i   ( timer.w_user    ),
            .WVALID_i  ( timer.w_valid   ),
            .WREADY_o  ( timer.w_ready   ),
            .BID_o     ( timer.b_id      ),
            .BRESP_o   ( timer.b_resp    ),
            .BVALID_o  ( timer.b_valid   ),
            .BUSER_o   ( timer.b_user    ),
            .BREADY_i  ( timer.b_ready   ),
            .ARID_i    ( timer.ar_id     ),
            .ARADDR_i  ( timer.ar_addr   ),
            .ARLEN_i   ( timer.ar_len    ),
            .ARSIZE_i  ( timer.ar_size   ),
            .ARBURST_i ( timer.ar_burst  ),
            .ARLOCK_i  ( timer.ar_lock   ),
            .ARCACHE_i ( timer.ar_cache  ),
            .ARPROT_i  ( timer.ar_prot   ),
            .ARREGION_i( timer.ar_region ),
            .ARUSER_i  ( timer.ar_user   ),
            .ARQOS_i   ( timer.ar_qos    ),
            .ARVALID_i ( timer.ar_valid  ),
            .ARREADY_o ( timer.ar_ready  ),
            .RID_o     ( timer.r_id      ),
            .RDATA_o   ( timer.r_data    ),
            .RRESP_o   ( timer.r_resp    ),
            .RLAST_o   ( timer.r_last    ),
            .RUSER_o   ( timer.r_user    ),
            .RVALID_o  ( timer.r_valid   ),
            .RREADY_i  ( timer.r_ready   ),
            .PENABLE   ( timer_penable   ),
            .PWRITE    ( timer_pwrite    ),
            .PADDR     ( timer_paddr     ),
            .PSEL      ( timer_psel      ),
            .PWDATA    ( timer_pwdata    ),
            .PRDATA    ( timer_prdata    ),
            .PREADY    ( timer_pready    ),
            .PSLVERR   ( timer_pslverr   )
        );

        apb_timer #(
                .APB_ADDR_WIDTH ( 32 ),
                .TIMER_CNT      ( 2  )
        ) i_timer (
            .HCLK    ( clk_i            ),
            .HRESETn ( rst_ni           ),
            .PSEL    ( timer_psel       ),
            .PENABLE ( timer_penable    ),
            .PWRITE  ( timer_pwrite     ),
            .PADDR   ( timer_paddr      ),
            .PWDATA  ( timer_pwdata     ),
            .PRDATA  ( timer_prdata     ),
            .PREADY  ( timer_pready     ),
            .PSLVERR ( timer_pslverr    ),
            .irq_o   ( irq_sources[6:3] )
        );
    end

    //-------------------------------------
    // Direct Memory Access Engine & IOMMU
    //-------------------------------------
    /*
        Two peripheral master interfaces (XBAR slave ports)
        - IOMMU Data Structures IF for implicit accesses
        - IOMMU Completion IF to forward allowed requests
        Two peripheral slave interfaces (XBAR master ports)
        - IOMMU Programming IF for IOMMU configuration
        - iDMA Programming IF for iDMA configuration
    */

    typedef logic [AxiAddrWidth-1:0]                iommu_addr_t;
    typedef logic [AxiDataWidth-1:0]                iommu_data_t;
    typedef logic [(AxiDataWidth/8)-1:0]            iommu_strb_t;
    typedef logic [AxiUserWidth-1:0]                iommu_user_t;
    typedef logic [ariane_soc::IdWidth-1:0]         iommu_id_t;
    typedef logic [ariane_soc::IdWidthSlave-1:0]    iommu_slv_id_t;

    localparam int unsigned NumDMADevices = 4;
    localparam int unsigned DmaAxiIdWidth =
        ariane_soc::IdWidth - $clog2(NumDMADevices);

    // AXI Bus between DMA devices and IOMMU TR IF
    // Extended with AXI support for untranslated transactions
    `AXI_TYPEDEF_EXT_ALL(axi_tr, iommu_addr_t, iommu_id_t, 
                            iommu_data_t, iommu_strb_t, iommu_user_t, 
                            rv_iommu::device_id_t, logic, rv_iommu::process_id_t)
    axi_tr_req_t axi_iommu_tr_req;
    axi_tr_resp_t axi_iommu_tr_resp;
    `AXI_TYPEDEF_ALL(axi_comp, iommu_addr_t, iommu_id_t,
                            iommu_data_t, iommu_strb_t, iommu_user_t)
    axi_comp_req_t  axi_iommu_comp_req;
    axi_comp_resp_t axi_iommu_comp_resp;
    rv_iommu::e3_ctl_t e3_ctl;
    rv_iommu::e3_tl_t e3_tl;
    rv_iommu::research_t research;
    logic nonblock_fire, nonblock_trans_accept;
    logic [1:0] composition_mode;
    logic [23:0] composition_did;
    logic composition_clear, composition_iommu_quiet;
    logic [15:0] nonblock_diag;
    rv_iommu::device_id_t nonblock_did;
    rv_iommu::ttype_t nonblock_type;
    logic [7:0] e3_control, e3_index;
    logic [63:0] e3_data;
    logic e3_error;
    if (!InclDMA) begin : gen_no_e3_control
        assign e3_control = 0;
        assign e3_index = 0;
        assign composition_mode = 0;
        assign composition_did = 24'd10;
        assign composition_clear = 0;
    end

    typedef logic [DmaAxiIdWidth-1:0] dma_iommu_id_t;
    `AXI_TYPEDEF_EXT_ALL(axi_dma_tr, iommu_addr_t, dma_iommu_id_t,
                            iommu_data_t, iommu_strb_t, iommu_user_t,
                            rv_iommu::device_id_t, logic, rv_iommu::process_id_t)
    axi_dma_tr_req_t  [NumDMADevices-1:0] axi_dma_tr_req;
    axi_dma_tr_resp_t [NumDMADevices-1:0] axi_dma_tr_resp;
    // Shared DMA full-buffer status.  Width mirrors NumDMADevices;
    // only the low two bits are consumed by fullbuffer_jobs_i.
    logic [NumDMADevices-1:0] fullbuffer_jobs;

    // AXI Bus between System Interconnect and IOMMU Programming IF
    `AXI_TYPEDEF_ALL(axi_prog, iommu_addr_t, iommu_slv_id_t, 
                     iommu_data_t, iommu_strb_t, iommu_user_t)
    axi_prog_req_t axi_iommu_prog_req;
    axi_prog_resp_t axi_iommu_prog_resp;
    `AXI_ASSIGN_TO_REQ(axi_iommu_prog_req, iommu_prog)
    `AXI_ASSIGN_FROM_RESP(iommu_prog, axi_iommu_prog_resp)

    //------------
    // DMA Engine
    //------------
    if (InclDMA) begin : gen_dma
        logic [NumDMADevices-1:0] obs_issue, obs_done, obs_poll, batch_active;
        logic [NumDMADevices-1:0] batch_gap_active, job_quiet;
        logic [63:0] obs_issue_id [NumDMADevices], obs_done_id [NumDMADevices], obs_poll_id [NumDMADevices];
        logic [NumDMADevices-1:0] obs_cfg_valid, obs_cfg_write;
        logic [7:0] obs_cfg_addr [NumDMADevices];
        logic [63:0] obs_cfg_wdata [NumDMADevices], legacy_cfg_rdata, launch_cfg_rdata, obs_cfg_rdata;
        logic [7:0] obs_cfg_wstrb [NumDMADevices];
        logic [NumDMADevices-1:0] launch_hold, launch_go, launch_cancel;
        logic [NumDMADevices-1:0] launch_held, launch_active, launch_cancelled, launch_error;
        logic [NumDMADevices-1:0] obs_arvalid, obs_awvalid, obs_arready, obs_awready;
        logic [NumDMADevices-1:0] obs_rlast, obs_b, obs_bad_id;
        logic [12:0] research_index;
        logic [63:0] research_data, research_wdata;
        logic [63:0] research_observer_data, research_policy_data;
        logic policy_enable, policy_allow, policy_abort, research_reverse;
        logic research_policy_access;
        logic [63:0] nonblock_data, composition_data;
        logic composition_access;
        logic [12:0] composition_index;
        logic research_write, research_dma_quiet, research_arm, research_open, research_close;
        logic [127:0] research_entries [16], research_entries_next [16];
        for (genvar r = 0; r < 16; r++) begin : gen_research_entries
            assign research_entries[r] = research.entries[r];
            assign research_entries_next[r] = research.entries_next[r];
        end
        for (genvar o = 0; o < NumDMADevices; o++) begin : gen_dma_observation
            assign obs_arvalid[o] = axi_dma_tr_req[o].ar_valid;
            assign obs_awvalid[o] = axi_dma_tr_req[o].aw_valid;
            assign obs_arready[o] = axi_dma_tr_resp[o].ar_ready;
            assign obs_awready[o] = axi_dma_tr_resp[o].aw_ready;
            assign obs_rlast[o] = axi_dma_tr_resp[o].r_valid && axi_dma_tr_resp[o].r.last && axi_dma_tr_req[o].r_ready;
            assign obs_b[o] = axi_dma_tr_resp[o].b_valid && axi_dma_tr_req[o].b_ready;
            // Each current iDMA emits AxID=0. Monitor its private mux-side ID.
            assign obs_bad_id[o] = (obs_arvalid[o] && |axi_dma_tr_req[o].ar.id) ||
                (obs_awvalid[o] && |axi_dma_tr_req[o].aw.id) ||
                (axi_dma_tr_resp[o].r_valid && |axi_dma_tr_resp[o].r.id) ||
                (axi_dma_tr_resp[o].b_valid && |axi_dma_tr_resp[o].b.id);
        end
        dma_observer #(.NumPorts(NumDMADevices), .PathEnable(InclIOMMU),
                       .E3Enable(InclIOMMU && IOMMULegacyE3Observer),
                       .ControlledEnable(InclIOMMU && IOMMULegacyE3Observer),
                       .P4MEnable(InclIOMMU), .ResearchEnable(InclIOMMU),
                       .AxiIdWidth(ariane_soc::IdWidth)) i_dma_observer (
            .clk_i, .rst_ni,
            .cfg_valid_i(obs_cfg_valid[0] && !obs_cfg_addr[0][7]),
            .cfg_write_i(obs_cfg_write[0]),
            .cfg_addr_i(obs_cfg_addr[0][6:0]), .cfg_wdata_i(obs_cfg_wdata[0]),
            .cfg_wstrb_i(obs_cfg_wstrb[0]), .cfg_rdata_o(legacy_cfg_rdata),
            .e3_data_i(e3_data), .e3_error_i(e3_error), .e3_control_o(e3_control), .e3_index_o(e3_index),
            .research_data_i(research_data), .research_index_o(research_index),
            .research_write_o(research_write), .research_wdata_o(research_wdata),
            .research_quiet_o(research_dma_quiet), .research_arm_o(research_arm),
            .research_open_o(research_open), .research_close_o(research_close),
            .issue_i(obs_issue), .done_i(obs_done), .poll_i(obs_poll),
            .batch_active_i(batch_active),
            .issue_id_i(obs_issue_id), .done_id_i(obs_done_id), .poll_id_i(obs_poll_id),
            .ar_valid_i(obs_arvalid), .aw_valid_i(obs_awvalid),
            .ar_ready_i(obs_arready), .aw_ready_i(obs_awready),
            .rlast_fire_i(obs_rlast), .b_fire_i(obs_b), .bad_id_i(obs_bad_id),
            .shared_ar_valid_i(axi_iommu_tr_req.ar_valid),
            .shared_aw_valid_i(axi_iommu_tr_req.aw_valid),
            .shared_ar_ready_i(axi_iommu_tr_resp.ar_ready),
            .shared_aw_ready_i(axi_iommu_tr_resp.aw_ready),
            .tr_id_i('{axi_iommu_tr_req.ar.id, axi_iommu_tr_req.aw.id}),
            .out_id_i('{axi_iommu_comp_req.ar.id, axi_iommu_comp_req.aw.id}),
            .return_id_i('{axi_iommu_comp_resp.r.id, axi_iommu_comp_resp.b.id}),
            .out_valid_i({axi_iommu_comp_req.aw_valid, axi_iommu_comp_req.ar_valid}),
            .out_ready_i({axi_iommu_comp_resp.aw_ready, axi_iommu_comp_resp.ar_ready}),
            .return_i({axi_iommu_comp_resp.b_valid && axi_iommu_comp_req.b_ready,
                       axi_iommu_comp_resp.r_valid && axi_iommu_comp_resp.r.last && axi_iommu_comp_req.r_ready})
        );

        assign obs_cfg_rdata = obs_cfg_addr[0][7] ? launch_cfg_rdata : legacy_cfg_rdata;
        if (InclIOMMU) begin : gen_research
            assign research_policy_access = research_index[12:8] == 5'h0E;
            assign composition_access = IOMMUNonblockMode == 3 &&
                research_index >= 13'hD20 && research_index <= 13'hD9F;
            assign composition_index = research_index - 13'hD20;
            assign research_data = composition_access ? composition_data :
                                   IOMMUNonblockMode != 0 && research_index[12:8] == 5'h0D ?
                                   nonblock_data :
                                   research_policy_access ? research_policy_data : research_observer_data;
            // Composition has its own accepted-request counters. Do not also
            // build the legacy diagnostic bank for this profile.
            if (IOMMUNonblockMode != 0 && IOMMUNonblockMode != 3) begin : gen_nonblock_observer
                dma_nonblock_observer #(.Mode(IOMMUNonblockMode)) i_nonblock_obs (
                    .clk(clk_i), .rst(!rst_ni), .arm(research_arm),
                    .open(research_open), .close(research_close),
                    .index(research_index[7:0]), .rdata(nonblock_data),
                    .accept(nonblock_trans_accept), .fast(nonblock_fire),
                    .did(nonblock_did), .ttype(nonblock_type),
                    .diag_events(nonblock_diag)
                );
            end else begin : gen_no_nonblock_observer
                assign nonblock_data = '0;
            end
            if (IOMMUNonblockMode == 3) begin : gen_composition
                dma_composition_control i_composition (
                    .clk_i, .rst_ni,
                    .cfg_write_i(research_write && composition_access),
                    .index_i(composition_index[6:0]), .wdata_i(research_wdata),
                    .rdata_o(composition_data),
                    .quiet_i(composition_iommu_quiet && research_dma_quiet &&
                             (&job_quiet) && !(|obs_issue) &&
                             !(|launch_active) && !(|launch_held)),
                    .supported_i(research.supported),
                    .arm_i(research_arm), .open_i(research_open), .close_i(research_close),
                    .mode_o(composition_mode), .protected_did_o(composition_did),
                    .clear_o(composition_clear),
                    .lookup_i(research.lookup), .lookup_hit_i(research.hit),
                    .normal_i(research.normal), .stage1_i(research.stage1),
                    .stage2_i(research.stage2), .lookup_did_i(research.did),
                    .lookup_pscid_i(research.pscid),
                    .accept_i(nonblock_trans_accept), .fast_i(nonblock_fire),
                    .fast_did_i(nonblock_did), .gap_active_i(batch_gap_active[1:0]),
                    .update_i(research.update), .invalidate_i(research.invalidate),
                    .update_did_i(research.did), .replace_mask_i(research.replace_mask),
                    .entries_i(research_entries), .entries_next_i(research_entries_next),
                    .plru_i(research.plru)
                );
            end else begin : gen_no_composition
                assign composition_mode = 0;
                assign composition_did = 24'd10;
                assign composition_clear = 0;
                assign composition_data = 0;
            end
            // The old E2/COST event recorder is not read by composition's
            // ioctl. Keep the shared index/window interface above, but avoid
            // a second snapshot/event store and its translation-to-RAM path.
            if (IOMMUNonblockMode != 3) begin : gen_legacy_research
                dma_research_observer #(.ResearchRole(IOMMUResearchRole)) i_research (
                    .clk_i, .rst_ni, .cfg_valid_i(research_write && !research_policy_access && !composition_access),
                    .cfg_write_i(research_write && !research_policy_access && !composition_access), .index_i(research_index),
                    .wdata_i(research_wdata), .rdata_o(research_observer_data),
                    .reverse_order_o(research_reverse),
                    .quiet_i(research_dma_quiet && e3_ctl.quiet && !e3_tl.ptw_active),
                    .supported_i(research.supported),
                    .entries_i(research_entries), .entries_next_i(research_entries_next),
                    .plru_i(research.plru), .update_i(research.update),
                    .invalidate_i(research.invalidate), .replace_mask_i(research.replace_mask),
                    .update_did_i(research.did), .lookup_i(research.lookup),
                    .lookup_hit_i(research.hit), .lookup_did_i(research.did),
                    .lookup_iova_i(research.iova), .lookup_pscid_i(research.pscid),
                    .lookup_gscid_i(research.gscid), .lookup_stage1_i(research.stage1),
                    .lookup_stage2_i(research.stage2), .ctl_state_i(e3_ctl.state),
                    .ctl_did_i(e3_ctl.current_did), .select_fire_i(e3_ctl.select_valid),
                    .select_did_i(e3_ctl.select_did),
                    .response_error_i((e3_ctl.response_fire && e3_ctl.response_error) ||
                                      (research.update && !research.normal)),
                    .issue_i(obs_issue[1:0]), .done_i(obs_done[1:0]),
                    .issue_id_i('{obs_issue_id[0], obs_issue_id[1]}),
                    .done_id_i('{obs_done_id[0], obs_done_id[1]}),
                    .p4m_arm_i(research_arm), .p4m_open_i(research_open),
                    .p4m_close_i(research_close)
                );
            end else begin : gen_no_legacy_research
                assign research_observer_data = '0;
                assign research_reverse = 1'b0;
            end
            if (IOMMUResearchRole == 2) begin : gen_policy
                dma_research_policy i_policy (
                    .clk_i, .rst_ni,
                    .cfg_valid_i(research_write && research_policy_access),
                    .cfg_write_i(research_write && research_policy_access),
                    .index_i(research_index), .wdata_i(research_wdata),
                    .rdata_o(research_policy_data),
                    .quiet_i(research_dma_quiet && e3_ctl.quiet && !e3_tl.ptw_active),
                    .supported_i(research.supported),
                    .v_active_i(batch_active[0]), .v_done_i(obs_done[0]),
                    .a_issue_i(obs_issue[1]), .a_done_i(obs_done[1]),
                    .ctl_state_i(e3_ctl.state), .ctl_did_i(e3_ctl.current_did),
                    .select_fire_i(e3_ctl.select_valid), .select_did_i(e3_ctl.select_did),
                    .lookup_i(research.lookup), .lookup_hit_i(research.hit),
                    .lookup_did_i(research.did), .ptw_r_fire_i(e3_tl.ptw_r_fire),
                    .response_error_i((e3_ctl.response_fire && e3_ctl.response_error) ||
                                      (research.update && !research.normal)),
                    .policy_enable_o(policy_enable), .policy_allow_o(policy_allow),
                    .policy_abort_o(policy_abort)
                );
            end else begin : gen_no_policy
                assign research_policy_data = '0;
                assign policy_enable = 1'b0;
                assign policy_allow = 1'b0;
                assign policy_abort = 1'b0;
            end
        end else begin : gen_no_research
            assign research_data = '0;
            assign composition_mode = 0;
            assign composition_did = 24'd10;
            assign composition_clear = 0;
            assign composition_access = 0;
            assign composition_index = 0;
            assign composition_data = 0;
            assign nonblock_data = '0;
            assign research_policy_access = 1'b0;
            assign research_observer_data = '0;
            assign research_policy_data = '0;
            assign research_reverse = 1'b0;
            assign policy_enable = 1'b0;
            assign policy_allow = 1'b0;
            assign policy_abort = 1'b0;
        end
        if (InclIOMMU) begin : gen_launcher
            dma_controlled_launcher #(.NumPorts(NumDMADevices)) i_launcher (
                .clk_i, .rst_ni,
                .cfg_valid_i(obs_cfg_valid[0] && obs_cfg_addr[0][7]),
                .cfg_write_i(obs_cfg_write[0]),
                .cfg_addr_i(obs_cfg_addr[0]),
                .cfg_wdata_i(obs_cfg_wdata[0]),
                .cfg_wstrb_i(obs_cfg_wstrb[0]),
                .cfg_rdata_o(launch_cfg_rdata),
                .frontend_held_i(launch_held),
                .frontend_active_i(launch_active),
                .frontend_cancelled_i(launch_cancelled),
                .frontend_error_i(launch_error),
                .obs_issue_i(obs_issue), .obs_done_i(obs_done),
                .obs_issue_id_i(obs_issue_id), .obs_done_id_i(obs_done_id),
                .selected_i(e3_ctl.select_valid),
                .select_write_i(e3_ctl.select_write),
                .select_id_i(e3_ctl.select_id),
                .select_did_i(e3_ctl.select_did),
                .select_iova_i(e3_ctl.select_iova),
                .launch_hold_o(launch_hold),
                .launch_go_o(launch_go),
                .launch_cancel_o(launch_cancel)
            );
        end else begin : gen_no_launcher
            assign launch_hold = '0;
            assign launch_go = '0;
            assign launch_cancel = '0;
            assign launch_cfg_rdata = '0;
        end

        if (IOMMULegacyE3Observer) begin : gen_e3_observer
            dma_e3_observer i_e3_observer (
                .clk_i,.rst_ni,.arm_i(e3_control[1]),.clear_i(e3_control[2]),
                .active_i(e3_control[3]),.open_i(e3_control[4]),.close_i(e3_control[5]),
                .abort_i(e3_control[6]),.delay128_i(e3_control[7]),
                .ctl_i(e3_ctl),.tl_i(e3_tl),.index_i(e3_index),.data_o(e3_data),.error_o(e3_error),
                .tr_fire_i({axi_iommu_tr_req.aw_valid && axi_iommu_tr_resp.aw_ready,
                            axi_iommu_tr_req.ar_valid && axi_iommu_tr_resp.ar_ready}),
                .tr_id_i('{axi_iommu_tr_req.ar.id,axi_iommu_tr_req.aw.id}),
                .tr_address_i('{64'(axi_iommu_tr_req.ar.addr),64'(axi_iommu_tr_req.aw.addr)}),
                .out_valid_i({axi_iommu_comp_req.aw_valid,axi_iommu_comp_req.ar_valid}),
                .out_ready_i({axi_iommu_comp_resp.aw_ready,axi_iommu_comp_resp.ar_ready}),
                .out_id_i('{axi_iommu_comp_req.ar.id,axi_iommu_comp_req.aw.id}),
                .return_i({axi_iommu_comp_resp.b_valid && axi_iommu_comp_req.b_ready,
                           axi_iommu_comp_resp.r_valid && axi_iommu_comp_resp.r.last && axi_iommu_comp_req.r_ready}),
                .return_id_i('{axi_iommu_comp_resp.r.id,axi_iommu_comp_resp.b.id}),
                .wlast_i(axi_iommu_comp_req.w_valid && axi_iommu_comp_req.w.last && axi_iommu_comp_resp.w_ready)
            );
        end else begin : gen_no_e3_observer
            assign e3_data = '0;
            assign e3_error = 1'b0;
        end

        AXI_BUS #(
            .AXI_ADDR_WIDTH ( AxiAddrWidth             ),
            .AXI_DATA_WIDTH ( AxiDataWidth             ),
            .AXI_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
            .AXI_USER_WIDTH ( AxiUserWidth             )
        ) dma_cfg_bus [NumDMADevices-1:0]();

        axi_demux_intf #(
            .AXI_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
            .ATOP_SUPPORT   ( 1'b0                     ),
            .AXI_ADDR_WIDTH ( AxiAddrWidth             ),
            .AXI_DATA_WIDTH ( AxiDataWidth             ),
            .AXI_USER_WIDTH ( AxiUserWidth             ),
            .NO_MST_PORTS   ( NumDMADevices            ),
            .MAX_TRANS      ( 8                        )
        ) i_dma_cfg_demux (
            .clk_i,
            .rst_ni,
            .test_i           ( 1'b0                ),
            .slv_aw_select_i  ( dma_cfg.aw_addr[13:12] ),
            .slv_ar_select_i  ( dma_cfg.ar_addr[13:12] ),
            .slv              ( dma_cfg             ),
            .mst              ( dma_cfg_bus         )
        );

        // AXI Bus between iDMAs and IOMMU TR IF
        // Extended with IOMMU AXI signals
        AXI_BUS_EXT #(
            .AXI_ADDR_WIDTH ( AxiAddrWidth          ),
            .AXI_DATA_WIDTH ( AxiDataWidth          ),
            .AXI_ID_WIDTH   ( DmaAxiIdWidth         ),
            .AXI_USER_WIDTH ( AxiUserWidth          ),
            .AXI_SID_WIDTH  ( rv_iommu::DevIdWidth  ),
            .AXI_SSID_WIDTH ( rv_iommu::ProcIdWidth )
        ) idma_axi_master [NumDMADevices-1:0]();

        for (genvar i = 0; i < NumDMADevices; i++) begin : gen_idma_axi_assign
            `AXI_ASSIGN_TO_REQ_EXT(axi_dma_tr_req[i], idma_axi_master[i])
            `AXI_ASSIGN_FROM_RESP(idma_axi_master[i], axi_dma_tr_resp[i])
        end

        axi_mux #(
            .SlvAxiIDWidth ( DmaAxiIdWidth        ),
            .slv_aw_chan_t ( axi_dma_tr_aw_chan_t ),
            .mst_aw_chan_t ( axi_tr_aw_chan_t     ),
            .w_chan_t      ( axi_dma_tr_w_chan_t  ),
            .slv_b_chan_t  ( axi_dma_tr_b_chan_t  ),
            .mst_b_chan_t  ( axi_tr_b_chan_t      ),
            .slv_ar_chan_t ( axi_dma_tr_ar_chan_t ),
            .mst_ar_chan_t ( axi_tr_ar_chan_t     ),
            .slv_r_chan_t  ( axi_dma_tr_r_chan_t  ),
            .mst_r_chan_t  ( axi_tr_r_chan_t      ),
            .slv_req_t     ( axi_dma_tr_req_t     ),
            .slv_resp_t    ( axi_dma_tr_resp_t    ),
            .mst_req_t     ( axi_tr_req_t         ),
            .mst_resp_t    ( axi_tr_resp_t        ),
            .NoSlvPorts    ( NumDMADevices        ),
            .MaxWTrans     ( 8                    )
        ) i_dma_iommu_tr_mux (
            .clk_i,
            .rst_ni,
            .test_i      ( 1'b0              ),
            .slv_reqs_i  ( axi_dma_tr_req    ),
            .slv_resps_o ( axi_dma_tr_resp   ),
            .mst_req_o   ( axi_iommu_tr_req  ),
            .mst_resp_i  ( axi_iommu_tr_resp )
        );

        for (genvar i = 0; i < NumDMADevices; i++) begin : gen_dma_inst
            dma_core_wrap_intf #(
                .AXI_ADDR_WIDTH     ( AxiAddrWidth             ),
                .AXI_DATA_WIDTH     ( AxiDataWidth             ),
                .AXI_ID_WIDTH       ( DmaAxiIdWidth            ),
                .AXI_SLV_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
                .AXI_USER_WIDTH     ( AxiUserWidth             ),
                .JOB_FIFO_DEPTH     ( 8                        ),
                // N v2: a frozen batch job is one aligned 4 KiB copy. Keep
                // its full source data until the paired write can drain, so
                // a read-ahead burst cannot block the shared R return path.
                // S and N use the same capacity; other DMA/legacy modes keep
                // the existing three-beat buffer. Larger/multi-job streams
                // are outside this prototype's dependency argument.
                .BUFFER_DEPTH       ( IOMMUNonblockMode != 0 && i < 2 ? 512 : 3 ),
                .NUM_AX_IN_FLIGHT   ( 8                        ),
                .MEM_SYS_DEPTH      ( 0                        ),
                .RAW_COUPLING_AVAIL ( 0                        ),
                .IS_TWO_D           ( 0                        ),
                .BATCH_PORT (1'b1),
                .OBSERVER_PORT      ( i == 0                   ),
                .LAUNCH_PORT        ( InclIOMMU && i < 2       ),
                .RESEARCH_PORT      ( InclIOMMU && IOMMUResearchRole != 0 && i == 1 ),

                .DEVICE_ID          ( 24'(10 + i)              ),
                .PROCESS_ID         ( 20'(20 + i)              ),
                .AxID               ( '0                       )
            ) i_dma (
                .obs_issue_o(obs_issue[i]), .obs_done_o(obs_done[i]), .obs_poll_o(obs_poll[i]),
                .batch_active_o(batch_active[i]),
                .batch_gap_active_o(batch_gap_active[i]), .job_quiet_o(job_quiet[i]),
                .fullbuffer_job_o(fullbuffer_jobs[i]),
                .policy_enable_i(i == 1 && policy_enable),
                .policy_allow_i(i == 1 && policy_allow),
                .policy_abort_i(i == 1 && policy_abort),
                .batch_reverse_i(i == 1 && research_reverse),
                .obs_issue_id_o(obs_issue_id[i]), .obs_done_id_o(obs_done_id[i]), .obs_poll_id_o(obs_poll_id[i]),
                .obs_cfg_valid_o(obs_cfg_valid[i]), .obs_cfg_write_o(obs_cfg_write[i]),
                .obs_cfg_addr_o(obs_cfg_addr[i]), .obs_cfg_wdata_o(obs_cfg_wdata[i]),
                .obs_cfg_wstrb_o(obs_cfg_wstrb[i]), .obs_cfg_rdata_i(obs_cfg_rdata),
                .launch_hold_i(launch_hold[i]),
                .launch_go_i(launch_go[i]),
                .launch_cancel_i(launch_cancel[i]),
                .launch_held_o(launch_held[i]),
                .launch_active_o(launch_active[i]),
                .launch_cancelled_o(launch_cancelled[i]),
                .launch_error_o(launch_error[i]),
                .clk_i          ( clk_i              ),
                .rst_ni         ( rst_ni             ),
                .testmode_i     ( 1'b0               ),
                // slave port
                .axi_slave      ( dma_cfg_bus[i]     ),
                // master port
                .axi_master     ( idma_axi_master[i] )
            );
        end
    end

	//--------------
    // No DMA Engine
    //--------------
    //
    // When no DMA engine is included, TR AXI Bus request xVALID/xREADY wires are set to zero
    // AXI transactions directed to the DMA config port are responded with error.
    else begin : gen_dma_disabled

        assign fullbuffer_jobs = '0;
		// AXI Bus between System Interconnect (Mst) and iDMA Configuration Port (Slv)
		ariane_axi_soc::req_slv_t axi_dma_cfg_req;
		ariane_axi_soc::resp_slv_t axi_dma_cfg_resp;
		`AXI_ASSIGN_TO_REQ(axi_dma_cfg_req, dma_cfg)
		`AXI_ASSIGN_FROM_RESP(dma_cfg, axi_dma_cfg_resp)

        axi_err_slv #(
			.AxiIdWidth ( ariane_soc::IdWidthSlave   ),
			.axi_req_t  ( ariane_axi_soc::req_slv_t  ),
			.axi_resp_t ( ariane_axi_soc::resp_slv_t )
        ) i_idma_err_slv (
			.clk_i      ( clk_i    				),
			.rst_ni     ( rst_ni   				),
			.slv_req_i  ( axi_dma_cfg_req       ),
			.slv_resp_o ( axi_dma_cfg_resp      ),
			.test_i     ( 1'b0     				)
        );

		// Set TR IF request wires to a known state
		assign axi_iommu_tr_req.ar_valid = 1'b0;
        assign axi_iommu_tr_req.aw_valid = 1'b0;
        assign axi_iommu_tr_req.w_valid  = 1'b0;
        assign axi_iommu_tr_req.b_ready  = 1'b1;
        assign axi_iommu_tr_req.r_ready  = 1'b1;
    end

    //--------------------------------------------
    // RISC-V Input/Output Memory Management Unit
    //--------------------------------------------
    if (InclIOMMU) begin : gen_iommu

        // AXI Bus between IOMMU Memory IF and System Interconnect
        `AXI_TYPEDEF_ALL(axi_ds, iommu_addr_t, iommu_id_t, 
                            iommu_data_t, iommu_strb_t, iommu_user_t)
        axi_ds_req_t  axi_iommu_ds_req;
        axi_ds_resp_t axi_iommu_ds_resp;
        axi_ds_req_t  axi_iommu_ds_delayed_req;
        axi_ds_resp_t axi_iommu_ds_delayed_resp;

        axi_delayer #(
            .aw_chan_t          ( axi_ds_aw_chan_t          ),
            .w_chan_t           ( axi_ds_w_chan_t           ),
            .b_chan_t           ( axi_ds_b_chan_t           ),
            .ar_chan_t          ( axi_ds_ar_chan_t          ),
            .r_chan_t           ( axi_ds_r_chan_t           ),
            .req_t              ( axi_ds_req_t              ),
            .resp_t             ( axi_ds_resp_t             ),
            .StallRandomInput   ( 1'b0                      ),
            .StallRandomOutput  ( 1'b0                      ),
            .FixedDelayInput    ( IOMMUDSFixedDelayInput    ),
            .FixedDelayOutput   ( IOMMUDSFixedDelayOutput   )
        ) i_iommu_ds_delayer (
            .clk_i      ( clk_i                     ),
            .rst_ni     ( rst_ni                    ),
            .slv_req_i  ( axi_iommu_ds_req          ),
            .slv_resp_o ( axi_iommu_ds_resp         ),
            .mst_req_o  ( axi_iommu_ds_delayed_req  ),
            .mst_resp_i ( axi_iommu_ds_delayed_resp )
        );

        `AXI_ASSIGN_FROM_REQ(iommu_ds, axi_iommu_ds_delayed_req)
        `AXI_ASSIGN_TO_RESP(axi_iommu_ds_delayed_resp, iommu_ds)

        // AXI Bus between IOMMU Completion IF and System Interconnect
        `AXI_ASSIGN_FROM_REQ(iommu_comp, axi_iommu_comp_req)
        `AXI_ASSIGN_TO_RESP(axi_iommu_comp_resp, iommu_comp)

        //--------------
        // RISC-V IOMMU
        //--------------
        // Keep extended 64-byte device contexts compatible with the existing
        // Linux image. The banked prototype excludes MRIF, not flat MSI.
        function automatic rv_iommu_cfg::rv_iommu_cfg_t nonblock_cfg();
            rv_iommu_cfg::rv_iommu_cfg_t cfg = ariane_soc::RVIOMMUCfg;
            if (IOMMUNonblockMode != 0) cfg.MSITrans = rv_iommu_cfg::MSI_BT_ONLY;
            return cfg;
        endfunction
        initial begin
            if (IOMMUNonblockMode > 3 ||
                (IOMMUNonblockMode != 0 && (IOMMUResearchRole != 0 || IOMMULegacyE3Observer)))
                $fatal(1, "Banked prototype cannot use legacy E3/COST observers");
        end
        rv_iommu_top #(
            .RVIOMMUCfg   (nonblock_cfg()),
            .BankedEnable(IOMMUNonblockMode != 0),
            .NonblockEnable(IOMMUNonblockMode != 0),
            .PolicyEnable(IOMMUNonblockMode == 3),
            .BankIdBit(DmaAxiIdWidth),
            .E3Enable(InclDMA),

            .axi_tr_aw_chan_t       (axi_tr_aw_chan_t),
            .axi_tr_w_chan_t        (axi_tr_w_chan_t),
            .axi_tr_b_chan_t        (axi_tr_b_chan_t),
            .axi_tr_ar_chan_t       (axi_tr_ar_chan_t),
            .axi_tr_r_chan_t        (axi_tr_r_chan_t),
            .axi_tr_req_t           (axi_tr_req_t),
            .axi_tr_resp_t          (axi_tr_resp_t),
            
            .axi_comp_aw_chan_t     (axi_comp_aw_chan_t),
            .axi_comp_w_chan_t      (axi_comp_w_chan_t),
            .axi_comp_b_chan_t      (axi_comp_b_chan_t),
            .axi_comp_ar_chan_t     (axi_comp_ar_chan_t),
            .axi_comp_r_chan_t      (axi_comp_r_chan_t),
            .axi_comp_req_t         (axi_comp_req_t),
            .axi_comp_resp_t        (axi_comp_resp_t),

            .axi_ds_aw_chan_t       (axi_ds_aw_chan_t),
            .axi_ds_w_chan_t        (axi_ds_w_chan_t),
            .axi_ds_b_chan_t        (axi_ds_b_chan_t),
            .axi_ds_ar_chan_t       (axi_ds_ar_chan_t),
            .axi_ds_r_chan_t        (axi_ds_r_chan_t),
            .axi_ds_req_t           (axi_ds_req_t),
            .axi_ds_resp_t          (axi_ds_resp_t),
            
            .axi_prog_aw_chan_t     (axi_prog_aw_chan_t),
            .axi_prog_w_chan_t      (axi_prog_w_chan_t),
            .axi_prog_b_chan_t      (axi_prog_b_chan_t),
            .axi_prog_ar_chan_t     (axi_prog_ar_chan_t),
            .axi_prog_r_chan_t      (axi_prog_r_chan_t),
            .axi_prog_req_t         (axi_prog_req_t),
            .axi_prog_resp_t        (axi_prog_resp_t)
        ) i_riscv_iommu (
            .clk_i        (clk_i),
            .rst_ni       (rst_ni),
            .nonblock_enable_i(IOMMUNonblockMode == 3 ? composition_mode[0] :
                               IOMMUNonblockMode == 2),
            .policy_partition_i(IOMMUNonblockMode == 3 && composition_mode[1]),
            .policy_clear_i(composition_clear), .policy_did_i(composition_did),
            .policy_quiet_o(composition_iommu_quiet),
            .tr_req_i     (axi_iommu_tr_req),
            .tr_resp_o    (axi_iommu_tr_resp),
            .comp_resp_i  (axi_iommu_comp_resp),
            .comp_req_o   (axi_iommu_comp_req),
            .ds_resp_i    (axi_iommu_ds_resp),
            .ds_req_o     (axi_iommu_ds_req),
            .prog_req_i   (axi_iommu_prog_req),
            .prog_resp_o  (axi_iommu_prog_resp),
            .wsi_wires_o  (irq_sources[(ariane_soc::RVIOMMUCfg.NumIntVec-1)+7:7]),
            .e3_enable_i(e3_control[0]), .e3_arm_i(e3_control[1]),
            .e3_active_i(e3_control[3]), .e3_delay128_i(e3_control[7]),
            .e3_ctl_o(e3_ctl), .e3_tl_o(e3_tl), .research_o(research),
            .nonblock_fire_o(nonblock_fire), .trans_accept_o(nonblock_trans_accept),
            .nonblock_diag_o(nonblock_diag),
            // Only DMA0/1 in this prototype have the whole-copy buffers.
            .fullbuffer_jobs_i(IOMMUNonblockMode != 0 ? fullbuffer_jobs[1:0] : 2'b00),
            .trans_did_o(nonblock_did), .trans_type_o(nonblock_type)
        );

    //----------
    // No IOMMU:
    //----------
    //
    //	When the IOMMU is not included, translation requests are bypassed directly to the XBAR.
    //	AXI transactions performed to the IOMMU programmming IF are responded with error.
    //	All memory IF request xVALID/xREADY wires are set to zero.
    end else begin : gen_iommu_disabled

        assign research = '0;
        assign nonblock_fire = 1'b0;
        assign composition_iommu_quiet = 1'b0;
        assign nonblock_diag = '0;
        assign nonblock_trans_accept = 1'b0;
        assign nonblock_did = '0;
        assign nonblock_type = '0;

        axi_err_slv #(
            .AxiIdWidth ( ariane_soc::IdWidthSlave   ),
            .axi_req_t  ( ariane_axi_soc::req_slv_t  ),
            .axi_resp_t ( ariane_axi_soc::resp_slv_t )
        ) i_iommu_err_slv (
            .clk_i      ( clk_i               ),
            .rst_ni     ( rst_ni              ),
            .test_i     ( 1'b0                ),
            .slv_req_i  ( axi_iommu_prog_req  ),
            .slv_resp_o ( axi_iommu_prog_resp )
        );

        // Connect directly the device to the System Interconnect
        // TR IF req => Comp IF req
		`AXI_ASSIGN_FROM_REQ(iommu_comp, axi_iommu_tr_req)

        // Comp IF resp => TR IF resp
		`AXI_ASSIGN_TO_RESP(axi_iommu_tr_resp, iommu_comp)

		// Set memory IF request xVALID/xREADY wires to a known state
        assign iommu_ds.aw_valid  = 1'b0;
        assign iommu_ds.w_valid   = 1'b0;
        assign iommu_ds.b_ready   = 1'b1;
        assign iommu_ds.ar_valid  = 1'b0;
        assign iommu_ds.r_ready   = 1'b1;

        assign irq_sources[(ariane_soc::RVIOMMUCfg.NumIntVec-1)+7:7] = '0;

    end

endmodule
