# D-BOUNDARY-SOURCE-2：本机 composition 生命周期核查

## Material Passport

- 日期：2026-10-15；方法：ARS-Codex / bounded source verification（fact-check，源码核查）。
- 证据状态：SOURCE_ONLY；0 次构建、模型运行、设备 ioctl、FPGA/板测、旧实验重跑；未改生产源码或 canonical docs。
- 独占范围：本机 `linux/drivers/misc/idma.c` 的 composition 路径、真实 UAPI/CLI、分配与释放辅助函数；为排除明显混淆，定向读 E3 两轮及 SVA_RUN 的生命周期入口。
- 建议/请求：gpt-6.1-sol / high；实际 model/effort：unknown（本席无运行元数据凭证）。没有使用 DeepSeek、Claude 或再派子席。
- 本轮不裁决 D 的全局优先级；D/I/A5/A2 既有成果、Oct15–28 窗口与 MIG1792023314 paused 保留。

**结论：当前 composition 提供的是“一次 ioctl 完成一个受控实验”的边界，没有提供“同一持久应用池中，上一 task 完成后由应用选下一 task 的 P/PN 模式”的正常控制接口。** 如果把连续两次 composition ioctl 当两 episode，第二次必须重新分配、初始化、映射和 warm，而且第一次成功路径已自动恢复 S/DID10；不能省略这些工作，把它直接移植成持久应用池正例。这个结论限于所核本机接口，不证明外部应用没有这种边界，不证明整个 D 思想族无价值。

## 可核查的接口与调用链

路径前缀均为 `/home/xvhaocheng/work/iommu-platform/`；逐段关键原文见 [source-evidence.md](source-evidence.md)，hash 与实际阅读范围见 [source-manifest.json](source-manifest.json)。

1. **现成 CLI 一进程一次试验。** `bench/idma_composition.c:3–14,393–415,461–527`：命令行选 mode/warm/pages/gap；创建 request，open `/dev/idma0`，恰好一次 `IDMA_IOC_RUN_COMPOSITION`，校验返回、close。`scripts/rootfs.py:8–24` 有打包条目；这只证明源码打包意图，不证明当前板端已装此二进制。
2. **入口是特权实验接口。** `linux/include/uapi/linux/idma.h:256–273` 明称 same-BIT composition experiment；`linux/drivers/misc/idma.c:2780–2816` 检查 `CAP_SYS_RAWIO`、ABI、mode 0..3、DID10/11、V1页/A1/8/16页、gap0/1000/4000、warm≤4096，然后唯一调用 `idma_run_session2(... true, NULL, NULL, false, &p->p4m, NULL, NULL, p)`。
3. **锁覆盖完整资源与运行生命周期。** `idma.c:151–154,2357–2458,2737–2748`：`streams[2] = {}` 是每次调用的局部状态；全局 `idma_experiment_lock` 从准备前一直持有到 cleanup 之后。不同 fd/进程的该路径会串行；两 DMA 的内部并行不等于多进程各自持 session。`idma.c:215–225,2932–2937` 的 open 只存设备指针，release 空，不创建 per-fd buffer/session/pool。
4. **完整成功路径次序。** `idma.c:2418–2458,2484–2507,2519–2646,2655–2744`：gap/observer/batch preflight → composition configure → V stream_prepare → 可选 A stream_prepare → HPM acquire → cache flush → 请求 warm → warm A drain（batch）→ V/A batch configure → gap readback → observer arm/snapshot → 测量 → STOP/drain/collect → HPM release → cache flush/verify → observer/P4M capture → batch disable → composition capture → restore S/DID10 → stream cleanup → unlock → 返回输出。并非每个 COPY 都重跑准备。
5. **stream_prepare 真正分配并改写内存。** `idma.c:320–400`：取得现有 device domain（不在这里创建 domain），每 stream 两次 `dma_alloc_attrs(...FORCE_CONTIGUOUS)`；遍历整个 src/dst 写 pattern/反 pattern；取得 PA；扫描空闲 IOVA；逐页显式 `iommu_map` src/dst。`idma.c:229–300,333–347`：cleanup 逐页 unmap，再两次 `dma_free_attrs`。每次重新选择/映射，不等于每次数值地址必不同；分配器可能复用，PA/IOVA 同一性没有保证。
6. **warm 的真实语义。** `idma.c:411–467,667–749,2501–2550`：warm 通过 MMIO COPY、NEXT_ID 与 DONE 反馈推进，V 达到请求数量时结束；A 条数取决于完成反馈，再 drain 最后在途 A。warm=0 是 ABI 可表达请求，batch 路径不会提交 warm V/A；**它不授权改变固定 M0 warm256、gap0、64/64 和既定 guard 来追正例**。当前 fixed M0 的合同仍以父任务为准。
7. **模式边界在准备之前。** `idma.c:2436–2458` configure 早于 stream_prepare；驱动在配置 next mode 时尚未取得本次 buffer 的 PA/IOVA。`idma.c:2343–2355,2692–2702` 最后 capture 后等 quiet word5==1，再 configure(mode0,DID10)。驱动依赖 quiet/config/clear/ack，硬件含义由 root 的 RTL 核查负责；本席不把 source 注释当硬件证明，也不等同 M0 全局 reset。

## 四种时间尺度不能混用

| 尺度 | 实际工作/寿命 | 源码依据 |
|---|---|---|
| 每设备 probe | 分配 `idma_device`、映射 MMIO、设 DMA mask、读取 DID、注册 misc；此处没有 stream pool | `idma.c:2939–2988` |
| 每进程/每 open fd | CLI 创建输入/结果对象与参数；open 保存公共设备指针，close 不负责 pool | `bench/idma_composition.c:461–499,514–527`；`idma.c:215–225` |
| 每 composition ioctl/session/测量 batch | 新建 V/A stream；配置、alloc/init/map、warm、一个测量窗口、drain/verify/capture/restore/unmap/free | `idma.c:2357–2816` 的已读范围见 manifest |
| 每软件 warm COPY | 仅写 src/dst/chunk/conf、读 NEXT_ID、poll DONE、更新 ring offset；不重新 alloc/map/flush | `idma.c:411–467,667–749` |
| 每硬件测量 COPY | 本 ioctl 只配置 autonomous batch 并成对触发，轮内复用本次 buffer；不向用户回调选 mode | `idma.c:505–607,822–915,2544–2594` |

`idma.c:335,402–408,751–769,956–966,2393–2395` 的 retain/quarantine 是未安全排空时保留 buffer 至 reboot、阻止以后试验的故障路径，不是正常可复用 pool。

## 两个容易误认成目标边界的接口

- **E3 pair 确有单次调用内的持久映射。** UAPI `idma.h:94–110`；`idma.c:2717–2731` 第二轮保留 stream，但先 `idma_stream_reseed` 全量重写两 buffer、重置 offset/counters，再回 `run_round` 做 HPM acquire/flush/warm。`idma.c:2831–2859 → 2751–2757` 调用时 composition 是 NULL，改变的是 E3 delay 实验参数，不给用户在两轮间输入 P/PN。不能把“有两轮复用”夸大成任意任务 control。
- **SVA_RUN 接受用户 arena，但不能据此造持久映射池。** `idma_sva.h:8,49–77` 明称 private synchronous fixed-task research ABI；没有 P/PN 字段。`idma_sva_task.h:13–16,62–70,308–457` 默认 opt-in 禁用，受 profile/单线程等检查；每次新建 session、pin user pages、分配 research domain/映射/attach、执行、restore/unmap/release/unpin，成功返回不留该 DMA binding。用户虚拟内存理论上可由别的调用者保留（不能宣称 ABI 强制释放用户内存），但现成 `bench/idma_sva_task.c:436–498` 每 repeat 都 mmap/init/consume/munmap；SVA 的实际执行/profile 未在本轮验证，也不恢复 A2。

## 最有希望的真实边界与信息/费用

选定 card：[boundary-card.json](boundary-card.json) 的 `native-composition-between-successful-ioctls`。它是**实际 ABI 可顺序调用的实验边界**：上一 ioctl 已完成、下一 mode 可由 caller 选择；现成 CLI 不实现持续 resubmit loop，保留同一 fd 连续调用是接口层推论，未执行。

- source owner：特权调用进程拥有 request/结果；驱动在 mutex 下拥有 V/A 两实验 stream；各 stream 只在本 ioctl 中持其 src/dst/显式 IOVA mapping。调用者不提交数据 buffer、task 描述符或 pool handle。设备对象/其既有 domain 的寿命比试验长，不等于 stream pool。
- 最早信息：用户在 ioctl 前知道 mode/pages/chunk/gap/warm/transfer 数和上一已返回结果；它没有本次新分配 PA 的输入控制。驱动在分配后才拿 PA、find_iova 后拿 IOVA，此时 mode 已配置。composition 结果结构没有显式 mapping/pool 句柄。不能声称用户当时已知真实 DRAM bank/row/refresh；也不能把 kernel 内部信息自动当用户已可用观测。
- 已知固定工作量：双 lane、V1/A8 时四次显式 DMA alloc，总 buffer 18×4096=73728B，初始化写相同数量字节，显式额外 map/unmap 各18页；测量 V 的 COPY 是 N×4096B read 与同量 write。`dma_alloc_attrs` 内部可能有额外映射/清零等成本，本席未展开，不能以上数当完整内部代价。warm A 和测量 A 数量受反馈/STOP影响。
- 未知费用：排队等待、DMA allocator/contiguous backing、内部 DMA map、IOVA/PTE/失效、CPU 写回/flush、warm、MMIO、各 guard/quiet、capture、restore、verify、free、真实 CPU/设备互扰和保留下来的 DRAM 状态；本轮均无实测时间。quiet/config/clear 的硬件覆盖留 root。
- 时间字段：`victim_elapsed_ns` 起点在 `idma.c:2587`，终点在 window 返回的 V 最后软件观测完成；它不含准备和后处理。`drain_ns` 仅括住 `idma_drain(v/a)`（2621–2630），不含随后 collect/verify/capture/restore/free。没有从 CLI 启动到结果返回的完整应用费用字段。run deadline 在 flush 后、warm 前建立（2495–2496），既不是完整 ioctl 时间也不是单独测量预算。

## 明确未核与下一必要输入

1. 本机源码/CLI 证明控制可表达，不证明当前运行内核、BIT、设备、PA/时钟或 board rootfs 对应这些字节。不能以当前 hash 回填历史二进制 identity。
2. 未核一般应用的长期 task/pool completion-to-resubmit 原生轨迹；不把外部 SPDK 当本机已运行应用。与另席 SPDK 来源任务不重叠。
3. 若 root 仅需裁决本机接口门，证据已足，不应再扫整个仓库；如需把映射/HPM 费用进一步落到调用链，最多定向下一两文件：`linux/drivers/iommu/riscv/iommu.c`（实际 map/unmap 与页表/失效）和 `linux/drivers/iommu/riscv/iommu-hpm.c`（HPM acquire/release；路径存在，本轮未读），仍需实际测量才有时间值。DMA allocator 内部费用另属未核，不假定免费。
4. 本席交回的否定范围是“当前 composition 不支持目标持久应用边界”，而非“新关系被证明不存在”；能否改用另一个具名正常来源、是否仍有信息差以及 D 优先级由 root 裁决。

本轮否定不包含“硬件无法增加 keepalive adapter”，也不授权实现 adapter 或采集器；不因此重跑旧32/64或64/64测量。root 已独立承担 quiet/config/clear 的原文核查和最终优先级裁决。
