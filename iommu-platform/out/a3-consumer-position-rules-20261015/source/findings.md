# A3：AXPY 真实消费者与发布位置源码合同

AXPY 能给出一条真实设备实现的需求证据：Snitch 计算核确实从四个固定 L1 缓冲取数，core 8 负责两路输入和一路输出的 DMA 发布，并承担槽位回收。**它不能单独证明功能必须用 DMA，也不是当前四 iDMA 的共享 IOMMU 性能正例。** 现有 runtime 返回事务 ID 并提供 `dma_wait(tid)`；普通固件保存 ID、放松 `wait_all` 的机会必须进入对照。本轮只核源码、写合同；无模型、编译、测试、板端或旧实验操作，新意由 root 裁决。

本文的 `S#:起止行` 引用全部绑定文末源登记表的**绝对路径与完整 SHA256**；JSON 中每条证据都再次写入路径、行号与 SHA256。源码修补副本与原公开版本分开：S1–S3 基于 hero-tools `47339e040c43ab4d44d9a870bc307251084a1ba4` 加 A2 隔离修补；它不是已证明的论文实验快照。S13:4–5。

## 1. 什么消费者是真的，什么约束仍未给出

- 构建选择 `carfield_snitch_cluster`，host main 与设备 kernel 分开编译。S3:9–17。
- 当前设备实现的计算消费者是 0–7 号核：每核读 64 个 FP32 x、读写 64 个 y，指针直接固定为 `l1_buf_x/y[itr%2][core_idx]`，一元素一条 `fmadd.s`。core 8 只发布 DMA；8 个计算核不能计成 8 个 DMA 源。S1:43–50、77–124、131–135。
- `x_dma/y_dma` 是被映射的原数据存储位置，DMA 直接从它们取输入并把 y 写回。最终外部消费者是 host CPU 的 `consume()`，它读取 x/y、核独立 expected；不是另一个只能在设备端完成的后继算子。S2:24–39、73–116。
- 主程序自身已有 `expected[i] = fmaf(alpha, x_ref[i], y_ref[i])`。因此当前功能合同允许普通 CPU 直接计算同样的 y；源码没有规定 CPU 时间/能耗预算、设备专属操作或下一设备消费者来排除它。若任务额外明确“必须由 Snitch 在 L1 位置计算/交付”或 CPU 资源上限，则这个附加约束必须在比较前冻结，并计代价，不能从既有 DMA 实现倒推为功能必需。S2:81–85、112–116。
- 保留并映射原 buffer 可避免额外 host 中间搬运；保留本身不能满足**当前 kernel** 的 L1 指针读取。当前验证器额外保留 x_ref/y_ref/expected 并在冷/热间恢复，是独立校验安排，不能借此声称所有普通部署都必须付这三份参考存储/恢复费用。源码未决定一个重写后的设备直接远程读实现是否可行或更快。S2:73–116；S1:110–117。

默认 `n=32768` 的源码推导是：每块 512 floats/2048B，64 块；每次 offload 128 个输入 descriptor、64 个输出，共 192 个，393216B 数据载荷。四个 L1 槽合计 8192B，映射的 x/y 用户数据共 262144B/64 个 4KiB 页；这些是静态计数，不是容量、接受量或时间测量。S1:43–50、79–98、135；S2:18–20、44、70–77。

## 2. 四个 L1 槽的 owner 和回收条件

这里的 owner 是由发布者、barrier 与使用顺序维护的**隐含访问所有权**。源码没有为每个槽建立独立 owner 元数据；它也不同于驱动的 open/mm/映射 owner。S1:49–50、77–149；S9:9–17、104–110。

| 槽（每槽 2048B） | 使用 tile | 填充 owner | 计算消费者 | 后继消费者 | 最小合法回收事件 |
|---|---|---|---|---|---|
| X0 `l1_buf_x[0][8][64]` | 偶数 | core8 发布的输入 DMA 写 | 核0–7各读本核64元素 | 无，x不改 | 此块全部计算读完成 |
| X1 `l1_buf_x[1][8][64]` | 奇数 | 同上 | 同上 | 无 | 同上 |
| Y0 `l1_buf_y[0][8][64]` | 偶数 | core8 发布的输入 DMA 写 | 核0–7读旧y并写FMA结果 | 输出 DMA 读槽并写回映射y | 输出 DMA 不再读取槽；整笔完成是保守充分事件 |
| Y1 `l1_buf_y[1][8][64]` | 奇数 | 同上 | 同上 | 同上 | 同上 |

所有声明/角色：S1:43–50、77–124、127–145。输出“已读完 L1 源”的更早事件没有在当前应用中被暴露，不能擅自把输出发布/发 AW 当槽已可复用。当前程序用下一轮顶端全局等待，强制此前所有 DMA 结束后再把同奇偶槽填给 t+2；这比 X 槽所需条件更强。S1:88–99、127–145。

令 RX[t]/RY[t] 为取当前块输入，K[t] 为8核计算，WY[t] 为输出：

1. RX[t] 与 RY[t] 完成后，K[t] 才能读槽。
2. K[t] 所有核完成后，WY[t] 才能读取更新后的 y。
3. K[t] 完成后，X[t%2] 已可用于 t+2；它不必等 WY[t]。
4. Y[t%2] 需等 WY[t] 不再读它，才可被 t+2 的输入覆盖。
5. 最后 WY 完成、dispatcher DONE、CPU cache handoff、host consumer、撤映射/失效完成/unpin/free 均要闭合，不能以循环退出代替。

第1–4项来自 S1:88–135 的读写位置/跨槽不别名关系；第5项代码边界为 S1:141–156、S5:65–73、S10:491–509、S2:108–138、S9:28–60/137–150。其硬件完成强度仍受第6节 gate 限制。

## 3. `wait_all` 造成的额外依赖与现成 ID 能力

当前程序的发布图是：初始 `RX0,RY0`；每轮先 `wait_all`、barrier，再并行进行 `RX[t+1],RY[t+1]` 与 K[t]，底部 barrier 后发布 WY[t]。因此 t+1 轮的全局等待把 WY[t] 也挡到了 K[t+1] 前。K[t+1] 只读相反奇偶槽和不重叠的用户 tile，数据 DAG 并不要求等 WY[t] 的 host 写回。这个**自动生成的软件依赖**是源码事实/依赖推导，性能影响未测。S1:77–103、110–145。

原 runtime 不能被简写成“只有 busy、没有 job ID”：

| 能力 | 原文位置 | 精确行为 |
|---|---|---|
| `dma_txid_t`/issue返回ID | S4:238–286 | `typedef uint32_t dma_txid_t`；`dmcpyi` 结果放 `a0`/`reg_txid` 并返回 |
| `dma_wait(tid)` | S4:429–442 | `dmstati` selector 0，注释为 completed_id；循环直到 `completed_id - tid > 0` |
| `dma_wait_all()` | S4:445–458 | `dmstati` selector 2，读 busy，直到 0 |
| AXPY实际调用 | S1:79–80、97–98、135 | 五个静态 issue 位置均传最后参数0，S4:282把它编码成cfg0；全部丢弃返回ID |
| AXPY实际等待 | S1:90、145 | 轮首及修补后的尾部只调用 `wait_all` |

原文摘录：S4:273–285 包括 `register uint32_t reg_txid asm ("a0");` / `return reg_txid;`；S4:429–440 包括 `static inline void dma_wait(dma_txid_t tid)` / `sub t0, t0, %0` / `blez t0, 1b`。完整有关原文保留在 JSON 的 `ordinary_transaction_id_interface.evidence`。

**首选有界普通版本（root提出，本席核owner/API）：只改最后tile入口。** 保留此前所有迭代的 `wait_all`；在倒数第二轮保存其未来预取调用返回的 `id_x_last/id_y_last`，最后轮仅以 `dma_wait(id_x_last)` 和 `dma_wait(id_y_last)` 等当前输入；最后统一 `wait_all`、fence和barrier完全保留。逻辑上只新增两个32位ID/8B元数据，物理寄存器/溢出成本未量；四槽容量不变。此版本以至少2块为前提，只有1块则保留原等待，不从不存在的预取ID取值。S1:79–80、85–103、141–149；S4:238–286/429–458。

其owner证明是：最后块用 `(L-1)%2`，旧输出 WY[L-2] 用相反的 `(L-2)%2`；最后轮 `rest=0` 没有未来预取，绝不会覆写仍可能被旧输出读取的槽；当前输入完成后可读当前槽，底部barrier后再发最后输出，尾部全局等待仍收齐两块输出，host DONE和free没有提前。S1:85–99、110–149。**一般中间tile若只把wait-all换成input-ID而继续原样预取，则可能把RY[t+2]写进WY[t]仍读取的Y槽，不能这样机械替换。** 泛化版本必须另给Y槽reuse guard，并保留全部8核barrier。最后tile版本正是利用不再发生下一次槽覆盖来免除这一额外改动。

当前硬件的 completed_id 若是有序前缀，等待某ID必须包含此前全部job，不能当作任意乱序完成许可。本轮只直接核得已公开的返回ID/按ID等待函数，没有取得其固定DMA RTL计数产生、更新/回绕/错误和物理总线完成语义；在已核来源中该点精确留未核，不继续扫子模块。普通方案可按原生API的公开语义参加源级比较，仍须硬件验收；本轮不声称已观察最后输入完成与旧输出未完成的同时窗口，亦不声称净收益。不能因原应用没使用ID，就把放松等待集合归成新增付费接口的收益。

## 4. 合法改顺序的位置，以及何时已太晚

- **core8 issue 前可改：** RX 与 RY 的独立填充次序可交换；也可延后下一块预取，等 K[t] 完成后先发 WY[t] 再发后续输入。前提始终是槽空闲、所需输入完成后才计算、输出读完后才覆盖。保存ID后可按第3节把输入就绪等待与旧输出槽回收等待分开。这些是普通设备固件控制，尚未实现/运行，也没有零开销假设。S1:77–103、127–149；S4:238–286/429–458。
- **host launch 前可改：** 决定何时启动整次 offload、是否用CPU、映射和存活多久。现有同步target/mailbox API没有在一次 offload 中给 host 每tile重新排序的入口；修改设备 core8 发布代码是另一个已具名普通位置。S1:17–29；S2:99–116；S10:485–509。
- **已接受以后：** 仅源端延后下一条，不能收回已接受DMA的义务，不能提前覆盖源/目的或撤销其有效映射。当前 helper仅提供启动与等待，没有在此已核边界提供取消API。函数进入、VALID高或“发过描述符”都不等于硬件接受，准确握手/承诺点本轮未重审；新控制器若部署更下游，仍须证明它实际还能选择尚未承诺的动作。S4:240–286/429–458；S9:28–60。
- **K已读槽或WY已用槽后：** 换输入、覆盖y或卸掉页都会越过consumer/owner边界。最终DONE后的CPU等待也不能倒补先前缺失的DMA完成/cache交接。S1:110–149；S2:108–116。

## 5. 共享翻译范围：单DMA有多种请求，不等于多个DMA源

固定 cluster 对 Xdma 明确断言：`// Make sure we only have one DMA in the system.` / `` `ASSERT_INIT(NumberDMA, $onehot0(Xdma)) ``，至多一个DMA扩展实例；有 Xdma 的核连接一个 `SDMAMst`。AXPY只有core8执行issue。S6:951–955、1304–1305；S1:77–99、131–135。

Carfield只把 FPCluster 端口送入 riscv_iommu，其余外部 master旁路；AR/AW stream_id和substream_id均固定0。S7:469–488、514–525。x、y输入及y输出确实经过同一翻译路径，可以研究这个单源序列，但源码调用顺序本身不证明它们已形成并发翻译争用、HOL或容量瓶颈。当前链没有第二个独立DMA任务/publisher；也没有把当前四iDMA接到这条Snitch消费者链上。**跨端点适配仍是独立工作**，须绑定publisher、consumer、DID/domain、缓存、accepted资源和完成所有权，不能把两份已存在源码拼成同一已运行任务。

这不否定AXPY作为来源证据的价值：它给了“两个独立输入＋原地y计算＋输出＋双缓冲回收”的实际结构，以及源端普通控制能动哪些序。它不能提供四DMA干扰的性能正例、独有收益或新意裁决。

## 6. 普通公平对照与尚缺的输入

| 对照 | 此任务必须给的机会 | 要计入的成本/边界 |
|---|---|---|
| CPU直接FMA | 按现有数值/结果consumer合同，CPU可在原y算出结果；若禁用须事先明确CPU预算或设备独有消费者 | n次FMA、CPU占用、缓存/内存访问、共同校验；不强收费根本没用的DMA映射 |
| 普通独立页表＋正常缓存 | 原实现就采用pin＋独立IOVA；相同页/权限/生命周期下允许合法cache/映射复用 | pin、两别名PTE、失效收据、数据搬运、缓存交接、consumer、排空/unmap/dirty-unpin；不得强加borrowed路径独有的CPU PTE限制 |
| 保留原buffer | 现有DMA已直接访问mapped x/y；可保留/映射存活的原分配，不强造pack/staging费用 | 真实驻留与owner时间；重复冷/热跑需要的原y保留一致计费；保留不等于填好L1 |
| 公开DONE节流/串行＋保存ID普通固件 | host串行整次offload；core8普通序列化/重排issue；已有ID等待必须比较 | 失去的重叠、poll/指令/barrier、排空/cache/owner时间；不把DONE当零成本或任意OOO许可 |

证据：CPU S2:24–39/81–85；页表/缓存 S9:87–125/168–180、S2:90–127；保留buffer S2:73–116、S1:79–80/110–117/135；DONE/ID S10:491–509、S5:65–73、S4:429–458。

**硬件实现仍未闭合。** gate 的 quiesce、invalidate_complete、cache_sync 全部返回 `-EOPNOTSUPP`，region_add在pin/PTE前调用quiesce并退出。尾DMA wait、独立consumer和软件owner修补不能把这个硬件缺口变为已部署。S8:5–14；S9:87–97；S11:3、36。现有源码与实编成果复用，本轮没有重跑旧验证。

下一决定只需先补：①真实部署是否有必须的设备位置/CPU预算；②一个具名第二独立publisher及其与本consumer链共享IOMMU的对应；③固定endpoint的完成ID、接受和物理排空/可见性语义；④与最后tile双ID普通版本（保留尾等待）、带reuse guard的泛化固件、普通页表cache、保留buffer及DONE串行的完整成本比较。缺这些时，本链只能准入源码需求/控制位置推导，不能准入新的共享IOMMU性能正例。

## 源登记（路径＋SHA256）

- **S1** [axpy_snitch.c](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c`
  - SHA256：`0738c631e9b08685c1ac0f6d1feb0a1ce3f38b17f7805bc29a6ac1b54df12307`
  - 来源：47339e040c43ab4d44d9a870bc307251084a1ba4 + isolated A2 patch；161行。
- **S2** [axpy_main.c](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_main.c)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_main.c`
  - SHA256：`a92e882dc405450615186f1d97d45f644908d36ab931d16d732958cb53d9e95e`
  - 来源：47339e040c43ab4d44d9a870bc307251084a1ba4 + isolated A2 patch；143行。
- **S3** [Makefile](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/Makefile)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/Makefile`
  - SHA256：`a06318488be7694377f88ce20230d537139fcdb6e0f34e51dce51ef5851cb283`
  - 来源：47339e040c43ab4d44d9a870bc307251084a1ba4 + isolated A2 patch；20行。
- **S4** [runtime.h](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/runtime/runtime.h)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/runtime/runtime.h`
  - SHA256：`9cf65f032b12807a38a855d2c5207b26955e70da7bc22d431c36b878a93896cd`
  - 来源：dbb4ad3380e6e81fa7eac3955e2340752f29dab0；476行。
- **S5** [omptarget.c](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/omptarget.c)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/omptarget.c`
  - SHA256：`5f117eba73497de86f77778eb94f1f33aa81f47c161096a9dafa632371422ddb`
  - 来源：dbb4ad3380e6e81fa7eac3955e2340752f29dab0；97行。
- **S6** [snitch-csr-cluster.sv](/home/xvhaocheng/work/iommu-platform/out/a2-native-sva-binding-20261015/sources/date-raw/snitch-csr-cluster.sv)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-native-sva-binding-20261015/sources/date-raw/snitch-csr-cluster.sv`
  - SHA256：`96db003e09d9494abef3d82850fedd0b23cf241ea159f2474c9872856c755167`
  - 来源：269830af872fa943d8b079470120d42d62e1cb69；1307行。
- **S7** [cheshire_wrap.sv](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/cheshire_wrap.sv)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/cheshire_wrap.sv`
  - SHA256：`c8d78e1397816c2dcf41a06c1e9d7a57397141d728b7e12ddb84ee7342b30da7`
  - 来源：dbb4ad3380e6e81fa7eac3955e2340752f29dab0；639行。
- **S8** [hero_platform_gate.c](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/sw/hero-driver/common/hero_platform_gate.c)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/sw/hero-driver/common/hero_platform_gate.c`
  - SHA256：`3f752b912dab027ae9449a8e09324755c8b5cc5a44c73f034c9ab08bbd2a314d`
  - 来源：47339e040c43ab4d44d9a870bc307251084a1ba4 + new isolated A2 gate；14行。
- **S9** [hero_iommu.c](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/sw/hero-driver/common/hero_iommu.c)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/source/hero-tools/sw/hero-driver/common/hero_iommu.c`
  - SHA256：`9306a94fec06b43ff5e43cdf4fded3c26a0e169daf8a9805d6181122993598f0`
  - 来源：47339e040c43ab4d44d9a870bc307251084a1ba4 + isolated A2 patch；182行。
- **S10** [llvm-hero-rtl.cpp](/home/xvhaocheng/work/iommu-platform/out/a2-native-sva-binding-20261015/sources/date-raw/llvm-hero-rtl.cpp)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-native-sva-binding-20261015/sources/date-raw/llvm-hero-rtl.cpp`
  - SHA256：`e8470a0863fb7a942a1409e42dfe214cc288fcfedb94e3e98c809fddee6f9572`
  - 来源：d8cffb8d7294483b45732283719e250116fbe6c0；526行。
- **S11** [README.md](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/README.md)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/README.md`
  - SHA256：`d190cbdd31182df28cc7bad1d68cccf485071f6688cf1fdfab560aa25b1d9475`
  - 来源：existing local evidence；36行。
- **S12** [date-software.md](/home/xvhaocheng/work/iommu-platform/out/a2-native-sva-binding-20261015/sources/date-software.md)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-native-sva-binding-20261015/sources/date-software.md`
  - SHA256：`553bb9124495e50fb0355352a6b17381e4db35020c0db1660e0a24f8568739c9`
  - 来源：existing local evidence；67行。
- **S13** [version-lock.json](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/config/version-lock.json)
  - 路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/hero/config/version-lock.json`
  - SHA256：`addc218ab5c8f692b91843068ac83fa0d26dc9e834d965c582bc2e63a2c3b64c`
  - 来源：existing local evidence；291行。
