# A5：固定 HERO AXPY 的翻译地址路径与并行证据边界

结论：**一个 Snitch DMA 不等于没有内部并行；但一次 AXPY 拷贝也不等于两条同时过 IOMMU 的地址腿。** 集群内 DMA AXI crossbar 先按地址分流：L1 TCDM 地址和其 alias 直接走本地存储，只有外侧地址走 SoCDMAOut。继承 A3 已接受的 FPCluster 外侧连接后，RX/RY 的 host IOVA 读地址 AR、WY 的 host IOVA 写地址 AW 进入同一个 IOMMU；相应 L1 写、读地址留在集群内。AR/AW 各有 FIFO、各自驱动 VALID，且固定 serialize=0 允许跨 descriptor 在旧完成前继续移交，因而不能用“单 DMA”排除跨拷贝并行。**这些原件没有证明默认 AXPY 曾在 IOMMU 内同时保有两个翻译请求，更没有证明实际仲裁竞争、HOL 或时间收益。**

本席只复用 A3/A4 与五份既有 RTL 原件，未扩源、取网络、编译、仿真、板测、改生产代码或补 hook。以下“过 IOMMU”指接到 `dev_tr_req_i` 的结构路径；实际翻译模式、有效 context/PTE 与成功执行不由这份接线证明。

## 1. 地址路径表

| 动作与地址腿 | 原生通道 | 集群内去向 | IOMMU 判定 |
| --- | --- | --- | --- |
| RX[t]：host x IOVA → L1 X bank，源地址 | AR | 不命中本地规则时 SoCDMAOut → FPCluster 外侧端口 | 接到 IOMMU 翻译请求端口；真正执行有效页表翻译仍待运行/配置证据 |
| RX[t]：同一拷贝的 L1 X bank 目的地址 | AW | TCDMDMA → `i_axi_to_mem_dma` | 本地，不进入外侧 IOMMU |
| RY[t]：host y IOVA → L1 Y bank，源地址 | AR | 不命中本地规则时 SoCDMAOut → FPCluster 外侧端口 | 同 RX 的远端路径 |
| RY[t]：同一拷贝的 L1 Y bank 目的地址 | AW | TCDMDMA → `i_axi_to_mem_dma` | 本地，不进入外侧 IOMMU |
| WY[t]：L1 Y bank → host y IOVA，源地址 | AR | TCDMDMA → `i_axi_to_mem_dma` | 本地，不进入外侧 IOMMU |
| WY[t]：同一拷贝的 host y IOVA 目的地址 | AW | 不命中本地规则时 SoCDMAOut → FPCluster 外侧端口 | 接到同一 IOMMU 翻译请求端口；真实翻译、成功/可见性未核 |
| K[t]：计算核使用四个 L1 bank 的数据 | 本地 TCDM consumer | 继承 A3 的 L1 FMA consumer 合同 | 不是上述 host IOVA DMA 翻译请求；未据此外推所有 CPU/取指流量 |

表中工作负载地址角色继承 A3 `source-contract.json` 的 `workload` 与 `shared_translation_scope`；不是新跑应用得到的地址轨迹。表只涵盖这些具名 bulk-copy 动作，不把整个平台全部流量归成它们。AXI R/W 是对应地址请求的数据通道，不另算一个地址翻译请求。

输入 copy 的远端 R 接收与它的 L1 写响应 B 退休是两个不同阶段。固定 DMA 的 job 完成回执看最后写 B（继承 A4；M:332–369），而 RX/RY 的这个 B 来自本地 TCDM 写腿。因此“外侧 RX/RY 读返回结束”不能单独替代“输入已写好 L1”的回执，更不能直接放行 L1 consumer。也不能反向宣称真实运行一定存在可利用的 R/B 时间差。

S:250 给 `TCDMSize=NrBanks*TCDMDepth*(NarrowDataWidth/8)`。W:724–726、742、745、807–808、817 固定为 32 banks × 512 words × 8B = 128KiB、base `0x51000000`、alias enable=1/base `0x18000000`。结合 S:454–469，TCDM 规则边界为 `[0x51000000,0x51020000)` 和 `[0x18000000,0x18020000)`。S:575–599 也有 ZeroMemory 及其 alias 规则；所以 host IOVA 必须避开**全部集群本地规则**才能按表进入外侧路径。本席未补核这次运行的具体 IOVA 数值，也未验证 AXI crossbar 实现细节；区间按这些 start/end 接口约定表述。

root 另已亲核 A3 的 S2 `axpy_main.c`:20、110：应用实参有 `iova + 0x200000000`（`BYPASS_OFFSET`）的源码地址关系。本席仅继承这条 root 证据，未另读第六份源码；它不是具体运行 IOVA，也不能因为常量名称就认定绕过 IOMMU或已经原生启用。A2 三个硬件 hook 默认拒绝的运行门槛仍保留。

决定性路由原文是 S:575–580 的 `default: SoCDMAOut` 与 `idx: TCDMDMA`，S:588–593 为 TCDM alias；S:603–631 把这些规则连接至 `i_axi_dma_xbar`；S:665–675 把 TCDMDMA 接入 `i_axi_to_mem_dma`；S:545–551 则把 SoCDMAOut 接出 `wide_out_req_o`。DMA 本身只接集群 xbar 的一个 SDMAMst 输入（S:951–953）。

## 2. 固定 context 标签、接口门槛与未核项

H:469–488 表明所有非 FPCluster 外部 master bypass；FPCluster 的 pre-MMU 请求被接到 `axi_iommu_req`。读写方向都强制 `stream_id=0`、`ss_id_valid=1`、`substream_id=0`。H:514–525 把它接到 `riscv_iommu.dev_tr_req_i`，完成请求接回 FPCluster post-MMU；IOMMU 隐式内存访问另走 IOMMUMstIdx，不是第二个 AXPY publisher。H:492–494 还固定 `.InclPC(0)`、`.InclBC(0)`；本席只记录实参，没有再读 IOMMU 内部去外推其全部语义。

因此，RX/RY 的远端读与 WY 的远端写是**相同接口 context 标签**，没有源码依据把它们说成两个独立 device/process context；软件 transaction ID 也不能替换成 context 标签。A4 已接受的 native ID0、有序完成前缀与 cfg bit2 截断事实照旧继承，不在此重做证明。

“接到翻译端口”仍要求地址确实从本地分流之外送出。进一步声称一次原生翻译成功，须有有效 IOMMU 工作模式、与固定标签匹配的 context/page-table 配置、映射/权限/生命周期和成功响应证据。A3 已登记独立页表 IOVA 软件代码及硬件 quiesce/invalidate/cache gate 未实现、运行被阻挡的边界；本席未亲核这些 driver 文件，未补齐 gate，不能把此接线当成当前 Dell 原生执行验收。FPCluster 平台中间组合连接继承 A3/A4 已接受合同，本席没有扩读 Carfield top 文件重新证明全平台。

## 3. 两个请求：支持到哪一级

| 层级 | 本轮判断 | 原文边界 |
| --- | --- | --- |
| 一个 DMA 有读/写两套地址发出通道 | 静态成立 | M:121–138 AR FIFO；152–169 AW FIFO；285–296 各方向 refill；321–346 各自 empty 驱动 VALID、各自 READY pop |
| 固定配置要求所有旧 job B 退休后才能移交下一 job | 不成立 | 继承 A4 serialize=0；K:194–203 中非 serialize 分支 `burst_valid=~burst_req_empty`，不等待旧 `trans_complete` |
| 一次 RX/RY/WY 的两地址腿同时竞争 IOMMU | 不能据此成立 | 每次 copy 只有 host IOVA 一侧是外侧翻译地址；另一侧本地 |
| 不同 copy 的远端 AR 与远端 AW 可以在结构上并行提供 | 原件不含互斥发出门，具有静态并行通道能力；须有跨 descriptor 流水及可用容量等条件 | M 独立 emitter + K 非串行移交 + S 地址分流；不是端到端可达性/正确性证明 |
| 同方向不同 copy 可以有多个地址在途 | 源码有 FIFO 与非串行移交容量，未测占用 | 不能把 FIFO 深度、xbar outstanding 参数或 VALID 当成真实同时接受数 |
| 默认 AXPY 在同一时段向 IOMMU 提供/成功接受了两个远端翻译请求 | 未核 | 无 AR/AW handshake、队列占用或请求身份的执行轨迹；内部接收/串行化/仲裁实现本席未扩读 |
| 两请求竞争某个共享翻译资源，产生 HOL、延迟传播或净时间收益 | 未核 | 同时在途也不自动推出争用；须定位实际有限共享资源、服务/阻塞与完成时间证据 |

K:189–202 的原作者非串行/死锁风险注释保留；不把“允许继续移交”转成已经正常运行、已经死锁、或默认输入一定可达某个窗口的结论。跨方向共用 native AXI ID0 也不把 AR/AW 合成一条总顺序；本席不据它重新推导完整下游协议。

另一个必须保留的门槛是**同 ID、不同写目的的集群 xbar 顺序约束**。输入 copy 的 AW 去本地 TCDMDMA，输出 copy 的 AW 去 SoCDMAOut，native AW ID 都为 0。S:303–316 固定 `UniqueIds=0`、`AxiIdUsedSlvPorts=WideIdWidthIn`；W:728 引用 `snitch_cluster_pkg::WideIdWidthIn` package 参数，本轮判断不使用其具体数值；W:798–799 给 `WideMaxMstTrans=WideMaxSlvTrans=4`，S:603–631 实例化/接线。配置本身没有展示 crossbar 如何实现同 ID 跨目的写响应排序；它可能限制后发 WY 的外侧 AW 何时出现，不能仅凭旧 descriptor 已接受就断言旧输出 AW 已到 IOMMU。本席未扩读 `axi_xbar` 实现，因此不裁决这项限制是否实际启用、释放点或影响周期。

root 的正/相邻反切面可以只固定**同一已接受 descriptor 前缀**，分别条件化输入本地写完成前缀已/未覆盖，并保持旧输出未退休；不要额外假设两切面拥有同一外侧 AW 握手前缀。这里仅支持“远端 R 完成不等于本地 B 完成”和“同 SID 不能按 context 把两输入与输出分开”；不证明两切面的全部 IOMMU 可见信号不可区分，也不证明默认执行自然出现任一切面。

## 4. A4 的精确窄纠正

A4 `receipt.md` §7 的“Carfield 把该 FPCluster 端口送入 IOMMU”作为**外侧端口**接线判断保留，但应补：“集群内部的 L1 TCDM/alias 地址腿先被 DMA xbar 截走，只有外侧 host IOVA 地址腿进入此端口。”不能把原句扩读成每个 copy 的 AR 与 AW 都进入 IOMMU。

A4 `receipt.json` 的 `claims` 中控制位置判断（finding 始于 `Before native DMCPY issue/acceptance`）同样应限定为“已经被集群本地地址路由分流后，IOMMU 看到的外侧 burst”。A3 `shared_translation_scope.what_is_shared` 中 x/y input 和 y output 共用翻译路径只适用于其 host IOVA 腿。单 DMA/单 publisher 事实仍成立；它不能单独否定同一 DMA 内部、跨 copy 的读写并行或有限资源争用，也不能证明这些已经发生。旧产物未修改。

## 5. 来源与接受边界

所有五份原件重新算 SHA256，与 A4 `receipt.json` 完全一致。机器产物 `translation-legs.json` 保存绝对路径、固定 ref、完整哈希、精确行界及必要原文；只核新增产物与原件身份，不把此检查看成硬件测试。

- **S** `/home/xvhaocheng/work/iommu-platform/out/a2-native-sva-binding-20261015/sources/date-raw/snitch-csr-cluster.sv`；Snitch ref `269830af872fa943d8b079470120d42d62e1cb69`；SHA256 `96db003e09d9494abef3d82850fedd0b23cf241ea159f2474c9872856c755167`。
- **W** `/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/snitch_cluster_wrapper.sv`；Carfield ref `dbb4ad3380e6e81fa7eac3955e2340752f29dab0`；SHA256 `4317d9421afeb8ba4d6473f7c7d09d6aaee7767d2eb0a0dd02e8a7abc905156c`。
- **H** 同 Carfield 源根的 `hw/cheshire_wrap.sv`；SHA256 `c8d78e1397816c2dcf41a06c1e9d7a57397141d728b7e12ddb84ee7342b30da7`。
- **M** `/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__future__src__dma__axi_dma_data_mover.sv`；同 Snitch ref；SHA256 `399a48c1b4c131d1524f564508c72c9bb8eeb054db275930e9f06ab4abeba107`。
- **K** 同 A4 raw 目录的 `snitch-hw__future__src__dma__axi_dma_backend.sv`；同 Snitch ref；SHA256 `11d9833dc919724bf48fe30ea5d0666832bf87b234ae32ae20cb2e272f473afa`。

决定性剩余缺口：默认 AXPY 的真实 host IOVA 与本地规则避让；IOMMU 工作模式/有效 context 与成功执行；真实远端请求同存及仲裁服务证据；相应完整时间。它们不妨碍 root 仅形成公开完成前缀和 owner/alias guard 的协议合同，但禁止把协议前缀写成已经出现的硬件竞争或性能正例。
