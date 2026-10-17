# invalidation_dma_top 接口（固定F5＋ba8cdd1；2026-10-17）

本接口用于独立测试副本；不修改生产代码。IOMMU固定F5 `85b157e`，真实iDMA／CVA6依赖固定 `ba8cdd1`，无研究frontend、banked、P/PN、early-hit、DS-first F4或DS delayer。F5保留已披露的必要正确性修复，包括PTW 4KiB leaf及DS AW/W owner修复；不将整个F5称为未经修改的上游。顶名 `invalidation_dma_top`。

实际fixture采用DDTP.mode3两层DDT和32B base DC，DID限定10/11（均<64）。固定F5的一层入口硬编码64B步长，而两层leaf采用32B；两层首级索引对更高DID另有切片差异，本任务不覆盖。额外nonleaf DDT读在所有模式共同存在并照实计入，不改RTL或使用非规范稀疏DC布局。

## 时钟与MMIO

- 输入 `clk_i`、`rst_ni`。
- 输入 `cfg_target[1:0]`：0=B/DMA0(DID10)，1=A/DMA1(DID11)，2=IOMMU programming。
- 输入 `cfg_valid`、`cfg_write`、`cfg_addr[63:0]`、`cfg_wdata[63:0]`、`cfg_wstrb[7:0]`、`cfg_size[2:0]`。
- 输出 `cfg_ready`、`cfg_rdata[63:0]`、`cfg_error`。请求保持至ready；ready表示真实AXI R/B已被内部FSM捕获。ready后撤valid至少一拍，C_HOLD才回IDLE。每次只允许一个MMIO事务，A/B驱动状态机不得阻塞整条host循环。
- DMA普通寄存器使用size=3、64-bit DATA、WSTRB=ff。IOMMU32-bit MMIO使用size=2，DATA=`uint64_t(value)<<((addr&7)*8)`，WSTRB=`0x0f<<(addr&7)`；读值按相同lane提取。CQB分0x18/0x1c两个32-bit写。不可把高半字读写当低32位。

## DDR标量端口

AXI地址64、数据64、ID6、USER1。无额外SV memory latency；由C++服务模型提供本轮固定startup16/beat1（旧PLRU donor为31，不沿用其报告字段）。

- 输出 `mem_aw_valid`、`mem_aw_id[5:0]`、`mem_aw_addr[63:0]`、`mem_aw_len[7:0]`、`mem_aw_size[2:0]`、`mem_aw_burst[1:0]`、`mem_aw_lock`、`mem_aw_cache[3:0]`、`mem_aw_prot[2:0]`、`mem_aw_qos[3:0]`、`mem_aw_region[3:0]`、`mem_aw_atop[5:0]`、`mem_aw_user`；输入 `mem_aw_ready`。
- 输出 `mem_w_valid`、`mem_w_data[63:0]`、`mem_w_strb[7:0]`、`mem_w_last`、`mem_w_user`；输入 `mem_w_ready`。
- 输入 `mem_b_valid`、`mem_b_id[5:0]`、`mem_b_resp[1:0]`、`mem_b_user`；输出 `mem_b_ready`。
- 输出 `mem_ar_valid`、`mem_ar_id[5:0]`、`mem_ar_addr[63:0]`、`mem_ar_len[7:0]`、`mem_ar_size[2:0]`、`mem_ar_burst[1:0]`、`mem_ar_lock`、`mem_ar_cache[3:0]`、`mem_ar_prot[2:0]`、`mem_ar_qos[3:0]`、`mem_ar_region[3:0]`、`mem_ar_user`；输入 `mem_ar_ready`。
- 输入 `mem_r_valid`、`mem_r_id[5:0]`、`mem_r_data[63:0]`、`mem_r_resp[1:0]`、`mem_r_last`、`mem_r_user`；输出 `mem_r_ready`。

普通4-slot xbar保留master来源编码：completion在slv2，DS在slv3，0/1闲置。因此DDR ID[5:4]=2属于payload、=3属于DS；payload ID[2]区分DMA1/0。所有AW/W/B都必须一般化：CQ marker为单beat8B，不能套COPY的len255断言。

## 只读debug输出（在上升沿前取样）

| 输出 | 宽度 | 定义 |
| --- | --- | --- |
| `dbg_tl_req_fire`、`dbg_tl_req_did`、`dbg_tl_req_iova` | 1/24/64 | CTL→TL真实valid&&ready及其请求身份 |
| `dbg_lookup`、`dbg_lookup_hit` | 1/1 | 原版TL一次PT lookup episode的首拍及当拍hit；原版内部lookup可能持续多拍，不能每拍算一次miss |
| `dbg_lookup_vpn`、`dbg_lookup_pscid`、`dbg_lookup_did` | 27/20/24 | 当次lookup身份，host Sv39 VPN |
| `dbg_ptw_start`、`dbg_walk_did` | 1/24 | PTW真实接受init条件为`state_q==IDLE && init_ptw_i && !edge_trigger_q`；walk所属当前TL请求 |
| `dbg_tl_state`、`dbg_ptw_state`、`dbg_ptw_init`、`dbg_ptw_edge_trigger` | 3/2/1/1 | 原TL/PTW实际状态与init、edge latch，只读用于watchdog归因 |
| `dbg_ptw_read_fire` | 1 | PTW自己的DS AR valid&&ready，不包括DDTW/CQ |
| `dbg_inval`、`dbg_inval_av`、`dbg_inval_pscv`、`dbg_inval_gv` | 各1 | 真实CQ送出的IOTINVAL.VMA脉冲和限定字段 |
| `dbg_inval_pscid`、`dbg_inval_vpn` | 20/27 | 真实失效PSCID/VPN |
| `dbg_cq_head`、`dbg_cq_tail`、`dbg_cq_csr` | 各32 | 实际regmap/CQ寄存器 |
| `dbg_cq_state` | 3 | 原handler枚举：IDLE0/FETCH1/REGISTER2/DECODE3/WRITE4/ERROR5 |
| `dbg_cq_opcode`、`dbg_cq_func3`、`dbg_cq_decode` | 7/3/1 | 实际cmd_q opcode/func3，decode=state3 |
| `dbg_cq_read_fire`、`dbg_cq_write_b_fire` | 各1 | CQ自身AR接受／marker B接受 |
| `dbg_dma_issue[1:0]`、`dbg_dma_done[1:0]` | 各2 | 旧wrap真实frontend issue／完成脉冲；位0=B，位1=A，无frontend改动 |
| `dbg_fault` | 1 | 真实IOMMU fault或CQ error；需直接失败，不把fault视为正常walk |
| `dbg_fault_cause` | 12 | 原top的`fault_data.cause_code`（`rv_iommu::cause_t`）；在fault_valid对应周期解释，CQ error另看CQCSR |
| `dbg_iommu_in_flight`、`dbg_dma_quiet[1:0]` | 1/2 | 原top在途指示及旧DMA frontend/backend均idle |
| `dbg_tags_valid`、`dbg_tags_en1`、`dbg_tags_en2`、`dbg_tags_global` | 各16 | 原版IOTLB当前每way属性 |
| `dbg_tags_vpn`、`dbg_tags_pscid` | 432/320 | way i从bits `[i*27+:27]`／`[i*20+:20]`取值；SV端口扁平packed，Verilator C++为VlWide，可写辅助按bit提取 |

全部debug仅从固定实例内部做层次只读观察，保留原IOTLB本体字节；不以force/set寄存器注入命令。CQH在FETCH后提前推进，不能视为decode/execute完成。主测量按根席决定的IOTINVAL＋两条AV0 IOFENCE；marker＋sentinel仅smoke，marker须独占8-byte对齐槽。
