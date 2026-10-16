# A4：固定 native DMA 接受与完成回执

结论：这个固定 Snitch endpoint 确实有“接受后返回的事务 ID”和“有序累计的完成前缀”。`dmcpy` 返回表示 descriptor 已进入原生请求 FIFO；完成计数的来源是该 job 最后一个写分段的 AXI B 响应退休，二者不能混为一谈。最后 tile 等输入 ID 可以在计数语义上排除后发旧输出；源码没有给出实际发生的提前窗口或净收益。

另有一个必须保留的源码错配：固定 Carfield runtime 的“cfg0 默认串行”注释与固定 Snitch RTL 不一致。RTL 丢掉 runtime 编码的 bit2，最终 `serialize=0`。这不证明本任务已经死锁，也不抹掉 source/API 对照价值；但不能把注释当成当前硬件的串行、无死锁或完整 native 正确性验收。

本席仅补 A3 的这一条关键回执缺口。A3 原文和 87 条已核证据未重做；本席未运行编译、仿真、模型、板测或旧实验，未修改生产源码或全局文档。root 裁决完整 consumer、成本、机制和新意。所有 `F:行号` 等引用绑定文末绝对路径、完整 SHA256 和固定 ref；逐条 machine-readable 证据见 `receipt.json`。

## 1. 固定版本及实际调用链

Carfield `Bender.lock` 明确把 `snitch_cluster` 绑定到 `https://github.com/pulp-platform/snitch_cluster.git` 的 `269830af872fa943d8b079470120d42d62e1cb69`。这是当前补取入口；旧版本表的 `pulp-platform/snitch` URL 不能用于本次固定 ref 的来源证明。先向该旧 URL 的 4 次请求均 404，已如实保留账本；没有跨版本代用。L:510–523。

固定 Carfield wrapper 取 `DMAReqFifoDepth=3`、`DMAAxiReqFifoDepth=3`、`Xdma=9'b100000000`。cluster 把参数传入 `snitch_cc`，后者在 `Xdma` 分支实例化 `axi_dma_tc_snitch_fe`。W:740–762；S:857–865；C:377–410。这仍是一个 Snitch DMA，不能充当当前四 iDMA 的原生任务。

## 2. dmcpy 的承诺点：接受，而非 offer 或完成

F:284–299 的原文顺序是：先等 accelerator response spill 可接收，再 assert `twod_req_valid`，等 `twod_req_ready`，才返回 `next_id` 并确认 accelerator request。T:41–60 明确 `ready=!req_fifo_full`、`push=valid&ready`。

所以返回的数值是这一拍接受 descriptor 前的 `next_id`；同一成功握手使 `next_id` 加一（F:169）。它不等 payload 完成。response spill 和核心响应通路可延迟 CPU 实际看到返回值，因此“接受后返回”不意味着一个固定周期延迟；只拉起 VALID 也不能当成已成功接受。

## 3. 为什么 completed_id 是有序前缀

需要核的是实际 FE，不能只找到一个名字类似的计数器就套语义：

1. FE 的 `twod_req_q` reset 为全零；完整 instruction decode 从未为 `.id` 赋非零值（F:60–79、227–355、361）。这里的软件 transaction ID 是 `next_id`，它没有被写成 descriptor 的 AXI ID。
2. top-level descriptor 按 twod FIFO 顺序输出（T:41–60、72–104），1D descriptor 再进入 backend burst FIFO（K:149–176）。
3. reshaper 只有当前 job 的 `src/dst` 状态；只在当前读写的最后分段可以一起移交时装下一条 FIFO job，AW 的 `.last=w_finish` 只标本 job 的最后写分段，AXI `.id` 从 descriptor 原样带过（R:239–310）。
4. AW 分段进入单个 AW FIFO，按队头 valid/ready 发出；所有 job 的本地 AXI AW ID 都是 0（M:152–169、321–330）。在互连扩大或重映射之前不存在每 job 独立乱序 tag。
5. 每个 AW enqueue 同时把其内部 `.last` 标志入 `last_transaction_queue`。`B_READY` 恒 1，每个 `B_VALID` pop 一项；只有该项 last=1 才产生 `trans_complete`（M:332–369）。**在 AXI 同 ID 写响应按 AW 次序返回的协议条件下**，B 与 last 标志一一对应，完成脉冲就是 job 顺序前缀。这里没有以任意 job done 次数冒充可乱序完成许可。
6. FE 又在每条 1D descriptor 被 backend 接受时排入“它是否 top-level job 最后一条”的 FIFO，在 `oned_trans_complete` 时 pop；只有最后一条完成才累加 `completed_id`（F:140–170）。本 AXPY 为 1D，T:104 直接令该 last 为 1。

上面给出固定 endpoint 的原生排列与回执合同，并明确依赖正常 AXI 同 ID 返回条件；本轮没有全面证明 downstream 互连/目标的所有协议行为。

FE:169–173 的真实计数是两个 reset0 的 32 位加法器：`next_id` 计已接受 job 数，`completed_id` 计已退休 job 数，因此 completed 是“最后完成 ID 加 1”。busy 精确为 `next_id != completed_id`。selector0/1/2/3 分别给 completed/next/busy/!twod_ready（F:320–324）。原 `dma_wait` 用有符号差 `completed-tid>0`；它需要新鲜 ID 与小于 2^31 的模差窗口，不能把任意古老 ID 永久拿来等（U:429–458）。此有限 AXPY episode 不能被当作回绕/错误执行验证。

`idma_tf_id_gen.sv` 虽在源码清单中，但 FE 没有实例化它。它 reset 为 next2/completed1、且跳过部分回绕数值（I:30–76），不能套到这个实际 FE 的 reset0 加法器上。

## 4. done 到底保证什么

`trans_complete = is_last_aw & B_VALID`（M:369），且 B_READY=1。它是 **DMA master 端最后写分段的写响应退休**，不是 descriptor 入队、AW 收下或 W 发完。在正常 AXI/data-path 语义下，写出的数据已从 R 接收路径进入数据 FIFO，再被 W 路径取出（D:176–199、282–343），所以它也不是只有读地址承诺。

但它不证明成功：mover 明确不检查写失败（M:332–335），FE completed 累加不看 B_RESP；data path 的 R_RESP 输入也未参与完成控制。错误响应同样可能推进计数。因此不要用这个回执代替成功/重试合同、host cache handoff、DRAM 持久化、IOMMU 失效完成、quiesce、unmap 或 free；A2 已有硬件 gate 缺口继续成立。

## 5. 最后 tile 的普通对照可表达什么

沿 A3 原发布顺序，最后 RX 和 RY 先接受，前一 tile 的 WY 后接受（A:88–98、127–137）。设它们 ID 是 `q, q+1, q+2`。在输入均完成而该旧输出尚未退休的计数状态，`completed=q+2`、`next=q+3`：输入 ID waits 可以通过，busy 仍为 1。该有序累计计数不会仅因为后发输出仍 pending，就把输入 ID waits 强制扩成 wait-all。

这是一种**允许的回执状态**，不是已观察到的可达时间窗口。若硬件服务/供给恰好使旧输出在最后一次 ID poll 或 barrier 前结束，真实提前窗口就为零；若 poll、barrier 或末尾排空抵消提前，完整净收益也可为零。不能由 buffer 深度或允许 pipeline 直接推出正收益。

两次 ID wait 在此原生前缀语义下有效；因为 RY 后发，单独等后发输入 ID 已覆盖 RX，原生回执边界只需保留一个32位 ID（4B逻辑状态）。这仅给 root 提供强对照的简化事实，不替 root 裁决完整 owner/consumer、编译寄存器成本或候选接受。四个原2048B L1槽和最后统一排空条件仍按 A3 合同处理。

实际分级资源是：twod FIFO 3 descriptors（T:41–60）、backend FIFO 3个1D requests（K:149–176）、AR/AW/R/W FIFO 各深3、data BufferDepth=3、last-AW 标志 FIFO 深6、FE last-1D 标志 FIFO 深16（F:90–160、M:121–228、352–368）。它们单位与阶段不同，不能相加冒充一个总接受容量；也没有以源码值冒充同时 occupancy 或延迟测量。

## 6. cfg0 与 serialize 的固定版本错配

U:273–311 把 `__deserialize` 编到 immediate bit2；cfg0 被注释称为 serialized/deadlock-safe。可实际 F:268 的 `cfg` 只有 `[1:0]`，F:273 接5位 immediate 时丢 bit2，只设 `decouple_rw=cfg[0]`、`is_twod=cfg[1]`（281–282）；top-level struct 没有 serialize 字段（F:70–79）。T:75 把整个 `burst_req_o` 默认为 0，1D/2D分支均不写 `.serialize`（T:87–157），因此传给 backend 的 serialize 恒为0。

K:194–203 在 serialize0 时允许下个 job 无需等待旧 job `trans_complete` 即进入 reshaper；原作者同时留有非符合/死锁风险注释。**确定事实是 cfg flag 无法选择注释声称的串行行为，不能把源注释当 native 保证；没有证据声称本轮已发生死锁、所有输入都错或整个任务无价值。** 没有修 RTL/runtime，也没有拿另一个版本的 serialized 实现补入本链。

## 7. descriptor 边界和控制位置

core8 在调用 DMCPY 之前仍能选择或延后独立 descriptor，包括 RX/RY 交换、在合法 slot owner 条件下调节输入/输出发布顺序。完成/回收 guard 必须单独满足。接受后 descriptor 已压入 native FIFO；这份 FE 没有 cancel/任意 reorder API，native pipeline也不提供绕过队头的 whole-job 调度。A:77–103、127–149；F:265–355；T:41–104；K:149–225。

IOMMU入口看到的是上述选择后产生的 AR/AW burst，软件 `next_id` 不作为每 job tag 暴露，内部 full-job last 标志也不是标准 AXI AW 字段。M:321–369。队头 AW/AR VALID 已拉起后，原 emitter 只在握手时pop，因此源端不能合法换掉那个待接受队头。Carfield把该FPCluster端口送入IOMMU（H:469–488）。

这只说明现有 ingress 不能凭空恢复“尚未发布的 whole descriptor选择”。它不排除一个另行付费的重排缓冲，或在仍未接受且独立的 AR/AW 间进行节流；这些能力必须按真实接口、已承诺义务和资源重新说明，不能从“位置更靠后”自动得到。

## 留未核

- 真正发生的输入完成/旧输出未完成窗口、poll/barrier开销、全部尾部费用与净性能。
- downstream完整协议/成功/可见性/错误恢复，以及原 quiesce/invalidate/cache gate。
- cfg错配应如何处理；本轮只登记事实，没有做版本修补。
- 第二个真实 publisher 与当前四iDMA对应；单DMA/第二publisher缺项事实保持。

## 来源登记

- **B** `Bender.yml`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-Bender.yml`
  - SHA256：`cf49fe2c311fb0a8c6d03d1f7fc9dde2d51dd8f63c1cd5d7505a10bc3dfcdf27`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；274行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/Bender.yml
- **C** `hw/snitch_cluster/src/snitch_cc.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__snitch_cluster__src__snitch_cc.sv`
  - SHA256：`0f46eb4d2dc65a4fc4ae90a6a58306ca39ab0ea4cc8ffa7ffec83f771b2ebe2c`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；978行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/snitch_cluster/src/snitch_cc.sv
- **F** `hw/snitch_dma/src/axi_dma_tc_snitch_fe.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__snitch_dma__src__axi_dma_tc_snitch_fe.sv`
  - SHA256：`a87c5dd9c5b535eb478bd858c5cc2ff99067ff4992b4926494bcd5a1e71608c1`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；363行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/snitch_dma/src/axi_dma_tc_snitch_fe.sv
- **T** `hw/snitch_dma/src/axi_dma_twod_ext.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__snitch_dma__src__axi_dma_twod_ext.sv`
  - SHA256：`4a27cc01f56db3cda532b1e3bc9d88cae01ba37874cdd485608de0c53f9a1230`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；167行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/snitch_dma/src/axi_dma_twod_ext.sv
- **I** `hw/future/src/idma_tf_id_gen.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__future__src__idma_tf_id_gen.sv`
  - SHA256：`83fa6cc09fb5173860eae9a0ea4d69219ec867f036be0e867de9f5d5a54dcec0`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；81行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/future/src/idma_tf_id_gen.sv
- **K** `hw/future/src/dma/axi_dma_backend.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__future__src__dma__axi_dma_backend.sv`
  - SHA256：`11d9833dc919724bf48fe30ea5d0666832bf87b234ae32ae20cb2e272f473afa`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；601行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/future/src/dma/axi_dma_backend.sv
- **M** `hw/future/src/dma/axi_dma_data_mover.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__future__src__dma__axi_dma_data_mover.sv`
  - SHA256：`399a48c1b4c131d1524f564508c72c9bb8eeb054db275930e9f06ab4abeba107`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；371行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/future/src/dma/axi_dma_data_mover.sv
- **R** `hw/future/src/dma/axi_dma_burst_reshaper.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__future__src__dma__axi_dma_burst_reshaper.sv`
  - SHA256：`78cc5a61fad2a540ce31c1a68319a0ab6bfafbaec160bd4cb8673c263dd48097`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；350行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/future/src/dma/axi_dma_burst_reshaper.sv
- **D** `hw/future/src/dma/axi_dma_data_path.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/raw/snitch-hw__future__src__dma__axi_dma_data_path.sv`
  - SHA256：`9c2b2a7f10aa69fa679ba7f2318e016bf9d17335e4ae119cdc18fb577be480eb`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；391行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/future/src/dma/axi_dma_data_path.sv
- **A** `apps/omp/basic/axpy/axpy_snitch.c`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c`
  - SHA256：`0738c631e9b08685c1ac0f6d1feb0a1ce3f38b17f7805bc29a6ac1b54df12307`
  - ref：`47339e040c43ab4d44d9a870bc307251084a1ba4 + isolated A2 patch`；161行。
  - 来源：A2隔离修补副本，非未修改upstream原件。
- **U** `sw/tests/bare-metal/snitchd/runtime/runtime.h`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/runtime/runtime.h`
  - SHA256：`9cf65f032b12807a38a855d2c5207b26955e70da7bc22d431c36b878a93896cd`
  - ref：`dbb4ad3380e6e81fa7eac3955e2340752f29dab0`；476行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/carfield/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/runtime/runtime.h
- **W** `hw/snitch_cluster_wrapper.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/snitch_cluster_wrapper.sv`
  - SHA256：`4317d9421afeb8ba4d6473f7c7d09d6aaee7767d2eb0a0dd02e8a7abc905156c`
  - ref：`dbb4ad3380e6e81fa7eac3955e2340752f29dab0`；829行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/carfield/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/snitch_cluster_wrapper.sv
- **L** `Bender.lock`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/Bender.lock`
  - SHA256：`1dc49906059a9aa49bb189586e2bd85d258bf1edfd6324066ac043f95dfa4365`
  - ref：`dbb4ad3380e6e81fa7eac3955e2340752f29dab0`；556行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/carfield/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/Bender.lock
- **S** `hw/snitch_cluster/src/snitch_cluster.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a2-native-sva-binding-20261015/sources/date-raw/snitch-csr-cluster.sv`
  - SHA256：`96db003e09d9494abef3d82850fedd0b23cf241ea159f2474c9872856c755167`
  - ref：`269830af872fa943d8b079470120d42d62e1cb69`；1307行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/snitch_cluster/269830af872fa943d8b079470120d42d62e1cb69/hw/snitch_cluster/src/snitch_cluster.sv
- **H** `hw/cheshire_wrap.sv`
  - 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/cheshire_wrap.sv`
  - SHA256：`c8d78e1397816c2dcf41a06c1e9d7a57397141d728b7e12ddb84ee7342b30da7`
  - ref：`dbb4ad3380e6e81fa7eac3955e2340752f29dab0`；639行。
  - 原文：https://raw.githubusercontent.com/pulp-platform/carfield/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/cheshire_wrap.sv

取源与失败请求均记录于 `raw/retrieval-ledger.json`；包括实际HTTP状态、ref、URL、落盘路径与SHA256。仅对新增产物做JSON/哈希一致性核验，这不属于硬件/应用执行证据。
