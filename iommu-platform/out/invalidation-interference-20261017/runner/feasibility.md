# 失效干扰 runner：只读可行性与接口（2026-10-17）

本文件保留实现前的只读阶段记录，以下“未实现”及startup31、研究port置0均属当时的候选描述。后续根席已批准并完成独立top构建；实际接口见`interface.md`，精确身份见`../build/build03/build-manifest.json`。实际实现从`ba8cdd1`导出完全没有Observer／Batch／Launch接口的旧iDMA，BufferDepth=3，DDR由C++固定startup16/beat1；没有通过关闭当前研究接口来称为原版。

CQ 是命令队列；PTE 是页表项；MMIO 是设备寄存器访问；IOFENCE 是命令完成／排序指令。本附件仅检查现有接口，未实现、编译、运行实验或修改生产源码。按根席最新范围，建议采用 **F5 完整原型 IOMMU top＋两个真实 iDMA＋普通互连＋行为 DDR，DS 不插入 delayer**。这是行为仿真平台，不是运行 Linux／真实 CPU 或 FPGA 的时间。

## 结论与工作量

最小 4KiB COPY 场景在约一天内有望完成，前提是测量真实 CQ 获取／解码和这版 RTL 实际实现的 IOFENCE 行为，不额外实现 PR/PW、CPU cache／Linux。只读阶段不能保证首次编译和协议打通的耗时。预计改动为 1 个窄 SV 系统 top、1 个独立 source-list/build 脚本、1 个双 lane 非阻塞 C++ 驱动，以及被动事件输出／归约；IOMMU 源由固定 F5 提取，不退改研究 TL 来伪造原版。

现有 PLRU runner 不能直接执行本任务：它实际构建 85 个 SV 输入＋一个 C++，不包含 `rv_iommu_top`、software interface、regmap 或 CQ handler。它硬编码 `PolicyEnable=1`，inner harness 硬编码 banked，system xbar 硬编码 `CommonDsFirstEnable(1)`；P 模式关闭提前 hit 也不等于原版路径。其只许改 IOTLB 文件的 source guard 必须由新任务独立 source manifest 代替，不能放宽旧轮检查后覆盖历史结果。

来源：`out/plru-ablation-20261017/runner/build.py:36–67,95–119`；`shared-overlay/tests/nonblock_actual_dma_top.sv:796–806`；`shared-overlay/tests/nonblock_actual_dma_system_top.sv:952–977`。实际历史构建身份为 `builds/v0-build01/build-manifest.json`，状态 passed，top=`nonblock_actual_dma_system_top`；这不是新任务的构建结果。

## 固定的原版 IOTLB 与普通修复边界

采用 IOMMU **F5 `85b157ebe71a623e326e51fff7b7126fe77e8eb9`** 的完整 `rv_iommu_top`、TL、SW/CQ、DS。F5 没有 Banked／Nonblock／Policy／E3 新接口。其 IOTLB 与固定上游 c80 的 Git blob **完全相同**：`9cd819ace299b37dabf143524774efa4bd84c75e`；没有 policy_mask 输入，使用完整 16-way tree-PLRU。CQ handler 同样与 c80 相同，blob=`cee8a1c0c8c00b4d204da4c5fc0ad2a5295a2900`。

F5 含分立的 AW 服务顺序、HPM、DS owner、PTW leaf、AXI boundary 普通修复，不能把整个 F5 叫未经修改的上游。尤其 F3 关系到 CQ marker 的 AW/W 归属，F4 修复合法 4KiB leaf；不得为了“原版”偷偷去掉这些必要修复后把故障算作失效成本。IOTLB 本体仍可逐字绑定上游。

不要继承旧 shared-overlay 的 TL／PTW／mux／xbar／rr_arb_tree，也不要继续编译旧 completion pool、banked CTL、nonblock-hit、M gate 或 paired policy wrappers。使用一个固定的 CVA6 AXI/common_cells vendor 家族，避免两个同名 module/package；普通生产 vendor 没有 F4 扩展端口。真实 iDMA／register_interface 的已用 source-list 可复用，但其原 Python main 不执行。

## 现有接口是什么，缺什么

| 项目 | 已有事实 | 最小改动 |
| --- | --- | --- |
| 两个真实 DMA | `dma_core_wrap` 的实际 frontend、legalizer、backend、AXI transport；不是合成的请求序列 | 保留两个实例、DID10/11、真实寄存器 NEXT／DONE；不用 autonomous batch／held launch／research gate |
| 配置访问 | 旧 `cfg_sel` 只有 DMA0/1；C_HOLD 只在真实 R/B 后应答 | 扩为三目标 cfg_target=0/1/2，第三目标连 F5 `prog_req_i`；保留 AW/W 独立握手、R/B error 和 response ID 检查 |
| CPU | `cpu_enable_i` 是固定地址压力流，且随 batch active 开关；不是任意 CPU MMIO | 最小版本不接该合成 CPU；C++ 控制真实 MMIO AXI xactor。没有 CPU/cache/Linux 时间声明 |
| 命令与页表内存 | C++ `mem`／`read64/write64/write_beat` 可存 PTE、CQ、marker | 初始化／更新普通模型内存，命令必须由 CQ 经真实 DS/DDR 读入；若 host 直接写 memory，必须说明是理想一致性存储，不计 CPU store/cache 耗时 |
| 失效端口 | 旧 inner harness 的 iotinval／iodirinval 永远为 0 | 完整 F5 top 内部已经连接真实 CQ→TL invalidation；不要由 C++ 直接脉冲替代真实命令 |
| DS | 旧 system top 只接 DDTW／PTW，CQ/FQ/PQ 等 tie0 | F5 top 已内含真实 DS merge；其 `ds_req_o` 直接接普通 fabric。不能再加第二层 DS merge，且本任务没有 delayer |
| DDR | FIFO R 队列、每到队首 startup31、AR 深度16、不重排；AW/W/B 独立 | 保留服务顺序，但让 CQ 2-beat 读、单 beat marker 写合法；不能沿用“所有 AW 都是 COPY len255／DMA ID”的断言 |
| 观测 | 旧 e3/research taps 含 bank 字段，不能套在原版 TL | 只读暴露原版 lookup hit/miss、PTW 开始／请求、tag invalidation/fill、CQ fetch/decode/marker B、在途和 quiet；不改替换动作 |

精确旧入口：`shared-overlay/tests/nonblock_actual_dma_top.sv:237–273,950–1078`；`shared-overlay/tests/nonblock_banked_top.sv:588–601`；`shared-overlay/tests/nonblock_actual_dma_system_top.sv:444–505,630–633`；`runner/discovery_dma.cpp:111–125,199–221,392–427,510–563,568–585`。

## 建议的 AXI 类型与窄 top 端口

- 地址／数据 64 bit，WSTRB 8 bit，USER 1 bit，物理地址按已有 cfg 保留 56 bit。
- DMA 请求：`AXI_TYPEDEF_EXT_ALL` 的 `axi_tr2_*`，ID2、DID24、SSID-valid1、SSID20；普通 4 输入 `axi_mux` 后为 `axi_tr4_*` ID4。只用端口0/1，端口2/3 tie0，以保持原有来源 ID 位编码，不引入 bank。
- IOMMU translation 请求接 `axi_tr4_req/resp`；completion 与 DS 为普通 AXI ID4；programming 为普通 AXI ID6，与原 DMA config bus 一致，设置 `RVIOMMUCfg.AxiProgIdWidth=6`。不要将 EXT metadata 错接成普通 AXI 字段。
- 普通 xbar 可保留 4 个 slave master 端口和原 CUT／outstanding 参数：slv2=IOMMU completion、slv3=IOMMU DS、其余闲置；DDR 侧 ID6。没有 DS-first、request mask、提前 hit、返回重排或 delayer。
- 对 C++ 暴露 `cfg_target[1:0]`、`cfg_valid/write`、`cfg_addr[63:0]`、`cfg_wdata[63:0]`、`cfg_wstrb[7:0]`、`cfg_size[2:0]` 和 `cfg_ready/error/rdata`。DMA 配置为 64-bit size3；IOMMU MMIO 为 32-bit size2，CQB 用两个32-bit写。32-bit lane 按 addr[2] 放入／提取 AXI DATA／WSTRB，不能假定高半字也在 rdata[31:0]。
- 直接复用 production `dma_core_wrap` 时不传旧 overlay 专有 `FirstPteReadPolicy` 等参数。BatchPort／LaunchPort／ResearchPort=0；仅保留必要的 passive issue/done/quiet。BufferDepth 及 outstanding 作为统一模型参数明列，不依据 mode 切换。
- F5 top 所需 `prog/comp/ds/tr` channel 类型由上述 typedef 给出；ATS 关闭，DTI 输入 tie0。新增 source 项至少包括 field/DTI packages、programming bridge、修改版 AXI2APB/FIFO、regmap 三模块、CQ/SW wrapper 和其禁用支路所需模块。完整固定 source-list 必须在实现时解析清楚，当前85项不构成其编译闭包。

## 真实 CQ 与 IOFENCE 的已知能力

真实入口已经存在：`cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv` 和 `rv_iommu_sw_if_wrap.sv`。已有 VIP `sim/src/rv_iommu_tb/rv_iommu_vip_pkg.sv:553–599` 展示了 CQB 写、CQON 轮询、128-bit command 两个64-bit memory store、CQT doorbell；可作为代码入口，但没有在本轮运行。`tests/test_iommu_s1_flush.c:3–9,237–249` 的 IOFENCE 是 mock，不能充当 RTL 路径。

寄存器：CQB=`0x18/0x1c`，CQH=`0x20`，CQT=`0x24`，CQCSR=`0x48`。命令16字节。CQ handler 读 ARLEN=1、ARSIZE=3；IOTINVAL.VMA 的 AV／PSCV／GV、PSCID／GSCID／VPN由实际 DECODE 传至 TL（CQ:310–339）。准确字段以 `packages/rv_iommu_pkg.sv:398–426` 为准：IOTINVAL 高64位中 ADDR 从 bit10 开始，4KiB 页的 highword 为 `IOVA >> 2`，不是简单 `IOVA >> 12`。

**完成时间的关键限制：** CQ:276–283 在 FETCH 的最后 R beat 就更新 CQH，然后才 DECODE；:362 的 PR/PW support 仍为 TODO。因此 CQH 越过 fence／CQCSR 的 fence bit 都不是通用 IOFENCE 完成证据，不补实现也不能声称它们保证 DMA R/W 排空。

可测的具体完成选择由根席决定：

1. **软件可见 marker 路径**：真实 IOTINVAL→IOFENCE.C(AV=1,PR=PW=0)→AV0 IOFENCE 哨兵。marker 独占一个8字节对齐槽，因为这版实现固定 AWSIZE=3、WSTRB=全1、WDATA={0,32-bit DATA}。看到唯一 token 且 CQH 越过后续哨兵，可以证明前 marker 的 WRITE 已等到 B 后才允许下次 fetch。额外 marker store／哨兵 fetch 成本单列，不能冒充 Linux 原 AV0 fence 的原生开销。
2. **原命令字节＋只读完成观测**：按第一步查到的原 IOFENCE 字节提交，测试 top 只读记录实际 CQ DECODE／回到 IDLE，明确称 test-observed completion。不得把未实现的 PR/PW 保证补写进解释，也不得用软件不可见信号冒充真实驱动已有能力。

A 的该次 COPY 必须已通过真实精确 DONE 和全部写回条件，才能清 PTE／撤销映射。B 仍可进行持久映射访问；不能为让 fence 轻易完成而把 B 也排空。若要求补完 PR/PW 或完整 CPU/Linux/cache 一致性，就超出此约一天的最小路径，应按用户停止条件报告。

## warm B、同／不同域和测量接口

- B 必须经真实 COPY 预热，再以真实 lookup 观察确认稳定；不写 tags、不强制 cache 状态。预热事件单独保留，测量窗口不 reset cache。同域必须相同 PSCID 且共享相同页表根／映射空间；当前旧 fixture 的两根不同，只把 PSCID 数字改相同不是合法同域设置。不同域使用不同 PSCID／各自根，G-stage关闭，PTE G=0。
- **总翻译足迹**按 source／destination 两腿去重计 VPN。总4／8页可用固定1页 source＋3／7页 destination；若采用4／8页 destination ring，应如实写总5／9页。A 每 COPY 的 source／destination 哪些 map/unmap 必须明列；两页都撤销时，按地址命令数和批量对象数也要计两页。
- C++ 采用两个独立 lane 状态机，A 的 map→COPY→DONE→clear→CQ/fence 等待期间，B 继续 poll／refill。共享 MMIO xactor 只仲裁一次事务，不允许 A 的阻塞 fence 函数冻结 B 补发。仍使用真实 NEXT ID／DONE ID、逐字节 COPY 检查和最终 quiet。
- 观测每次正常 translation 的 DID、PSCID、VPN、接受／lookup／hit／miss／PTW；walk/PTE读另按所属请求记账。统计 B 在实际失效后首次访问各页的 hit/miss，并结合 tags 有效位变化／替换记录区分失效与容量淘汰，不把每次 flush 后的所有 miss 都直接归因于 flush。
- CQ 每提交记录提交序号、128-bit字节、CQT生效周期、fetch两beat、DECODE失效脉冲、fence完成与 errors。`CQT-CQH`是 fetch head 意义下的槽占用；另报 handler execution busy／marker待B，不能把早释放的 head 当退休数。
- 完整 A／B 时间保留 ready→配置／NEXT→backend→首R→末B→精确DONE→各自最后完成和共同quiet。另列供给等待／CQ等待／PTW／payloadDDR读写，不把 CPU 模型缺项写成零成本实测。PTE/CQ若用 host 直接一致性 memory store，报告其假设和零模型周期，不声称测得 Linux map/unmap 软件耗时。

详细来源身份、接口和未实现项见同目录 `interfaces.json`。本阶段到此结束；新构建与测量应在来源第一步完成、根席确定具体 IOFENCE 完成口径后再开始。
