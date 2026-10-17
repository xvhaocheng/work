# 问题发现 fixture 可执行性核查（只读设计）

## Material Passport

- 日期：2026-10-17 JST；ARS experiment-agent / plan 的有界源码可执行性输入。
- 本席实际工作：定向读源码并写本文件；模型/仿真/编译/生产或旧 runner 修改均 0。
- 建议席配置为 Sol/high；实际独立运行元数据 unknown，不能用建议代替观测。
- 本文是供 root 选择的方案，不是冻结合同、执行授权或新性能结果；无需先有创新机制才能发现问题。

## 结论与证据等级

**源码上可做**：正常 batch-OFF、aligned4096 COPY，固定源、有限目的环，预先建立全部映射；每 lane 只留一个 parent，DONE 后普通 MMIO 补发，不改 PTE/IOTLB、不加启动 delay，可以形成连续有限任务供给。实际是否持续重叠、重叠比例及翻译竞争须新运行测得；单线程 MMIO 补发自然有空洞，不能承诺两 lane 每周期都忙。

**尚未具备的部分**：当前 paired 入口只运行一对 COPY，且计数硬断言每 lane 恰好一次；不能把“pair 改持续供给”记作已有结果（runner/paired_dma.cpp:2391–2413、2425–2437、2459–2491）。

**可复用的源码骨架**：原 actual-DMA warm 逻辑已经有 A-first、V-first poll/refill、A-second poll/refill、精确 NEXT/DONE 和最终 A drain，但旧 A 同时轮转源/目的，旧停止规则按 V 完成截断 A；它不是本提案固定源、双方固定工作量的完成实验（tests/nonblock_actual_dma_test.cpp:2091–2114、2117–2122、2155–2215）。原文件开头明确只模拟 driver 顺序而非 CPU/板级时间（:18–48）；本文未另读 Linux 来升级来源主张。

已执行旧结果的范围仍以根 CURRENT_STATUS 和 first-pte-paired-local 报告为准；首 PTE 版本关闭不构成对正常任务序列问题发现的禁令。M 不进入此发现实验。

下文缩写均相对 `iommu-platform/`：

- `R` = `out/first-pte-paired-local-20261016/runner/paired_dma.cpp`。
- `C` = `out/first-pte-paired-local-20261016/combined/paired_dma.cpp`。
- `S` = `out/first-pte-paired-local-20261016/combined/overlay/tests/nonblock_actual_dma_system_top.sv`。
- `D` = 同 overlay 的 `tests/nonblock_actual_dma_top.sv`；`B` = `tests/nonblock_banked_top.sv`。
- `T` = 同 overlay 的 `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv`。

## 必须分别命名的普通配置

| 名称 | N runtime | P runtime | 说明 |
| --- | ---: | ---: | --- |
| S-runtime | 0 | 0 | 原接口的 ordinary 模式名；不能自动称 stock RTL |
| N | 1 | 0 | 允许非阻塞命中，完整替换 mask |
| P | 0 | 1 | 普通静态分区，阻塞运行策略 |
| PN | 1 | 1 | 普通静态分区 + 非阻塞命中 |

原模式真值表在 `tests/nonblock_actual_dma_test.cpp:3359–3368`；本提案若选 P/PN，可用同一 nonblock-capable、PolicyEnable=1 elaboration，reset 前设 runtime 两位并全程固定。`T:143–144` 要求 PolicyEnable 构建仍有 NonblockEnable=1、16-entry IOTLB，不能为了运行 P 将该 elaboration 能力关掉。`T:315–335` 的 runtime enable 控制 N；`:663–679` 的 P mask 为 DID10 两项 `0003`、其余十四项 `fffc`。

**现有 build 的 `--policy 0` 是 PairPolicy=0 的 PN+DS-first/F4，绝非 stock**：build.py:90–91 固定 `-GPolicyEnable=1`；`R:2465–2466` 固定 partition=true、reset(true)；`S:923–924` 固定 CommonDsFirstEnable(1)。PairPolicy=0 只关闭 M/O 特殊动作。真正 stock elaboration 还涉及 TouchEnable/PartitionLocalHistory 差异（`T:663–666`），不得仅令 runtime 两位为 0 就改名 stock。

DS-first/F4 是 RTL DRAM AR 选择器策略：当前 DS VALID 有优先权；连续四次 DS grant 后若有其他请求，给非 DS 机会；不是预测预约，也不是 DDR R-return 重排（overlay `cva6/vendor/pulp-platform/common_cells/src/rr_arb_tree.sv:155–185`）。原 C++ `ds_priority` 会旋转 DDR 队列，必须继续 false（`R:683–699、2462`）。

建议若做最小发现，将 **P 与 PN 作为两个已实现强普通**，共同固定 DS-first/F4=on、PairPolicy=0、原 DDR FIFO 服务 startup31、DS50/50。只变一个行为维度——目的地址再用距离。若 root 改选 S/N/P/PN 或 DS-first on/off，须从 ≤8 新配置总预算重新分配，不能隐加矩阵。

M 必须静态/运行均关闭：`S:923、943` 当前以 PairPolicy==1 使能；O 的 PairPolicy2/3/4 也不进入，普通 read legalizer 维持 0（`D:357、418、797`）。不把 DS-first/F4 混称 M，不把 O 分段混称普通未分段。

## 最多四个来源绑定的再用条件（供选择，未冻结）

共同来源绑定：沿 `R:83–95、533–583` 已有固定 4KiB 页/映射初始化，以及 `R:2095–2118、2179–2198` 的合法 batch-OFF 环推进与补发动作；这证明 fixture 有动作接口，不证明真实应用地址再用分布。**固定源 + 仅目的环是本轮需改测试代码的投影**，不能声称旧 driver 正是此工作负载。

| 条件 | A 目的环 R_A | A 不同有效页（源+目的） | 地址再次使用 |
| --- | ---: | ---: | --- |
| c1 | 1 | 2 | 每个 parent 都重用同一目的页 |
| c2 | 8 | 9 | 每 8 个 A parent 环回 |
| c3 | 16 | 17 | 每 16 个 A parent 环回，足迹超过 A 的十四项分区 |
| c4 | 32 | 33 | 每 32 个 A parent 环回，较长再用距离 |

这些数字只是安全、便于解释的候选边界，不是已预测的 miss 次数；共享 FSM、替换、触碰及合法供给的实际交织决定结果。若选全部四项 × P/PN = 8 配置，不能再加 cold/hot 或单独 solo 运行。历史冷/热结果不能充当不同工作负载的 solo 参照；只报告这八项的行为与完整任务差异。

安全布局建议：V 保留 `src IOVA/PA=0x10000000/0x80000000`、`dst=0x10001000/0x80001000`。A 固定 `src=0x20000000/0x90000000`；目的 p 为 `IOVA=0x20001000+p*4096`、`PA=0x90100000+p*4096`，0≤p<R_A。该公式要替换旧“先 a_pages 个源、再 a_pages 个目的”映射，**不能只改 submit 而忘了 PTE**（`R:559–566`）。

最大 A L0 只用索引0–32，同一个现有 L0 内；V/A 根及 L1/L0 保留 `0xA0020000/0xA0040000/0xA0050000` 与 `0xA0030000/0xA0060000/0xA0070000`；DDT `0xA0010000`（`R:537–567`，`B:588–592`）。载荷、表、CPU scratch `0x81000000/0x81001000` 均分离，落在真实 decode `[0x80000000,0xc0000000)`（`S:895–902`）。全部页 4KiB 对齐、PTE A/D 预置，运行期间不变更映射、失效或手工改内部状态。

共同工作量候选：双方各 64 个 4096B COPY，读/写各 262144B/lane；V 目的环恒1，A 按本地 parent 序号 j 的 `p=j%R_A` 推进，最大环两次完整经过。初始化并维护的有效载荷物理页并集为 `R_A+3` 页；页表页另记。若保留旧未用源页初始化，应另记而非冒充有效工作。64 是固定有限任务设计建议，不是旧 64-job batch 实验的复用结果。

共同供给候选：quiet 后 t0，配置 A、V，再 A NEXT、V NEXT；此后单线程循环 V poll→若 exact ID DONE 且未达64则完整配置并 NEXT→A 同样处理→正常 tick；每 lane 最多一个 active parent。双方各自达到64后停止补发，继续等另一方，最后 quiet。不使用“V完就截断A”、STOP、自主 batch、定时到达/额外 delay、launch hold 或根据 cache 观察来挑地址。完整配置/poll 自身消耗的真实 fixture 周期计入，不用理想即时供给替换。

## 最少需要的独立测试变化（未实施）

1. 在新目录复制 runner/overlay 并新建入口；不改 R、C 或生产。原 build.py:32–33 只准输出旧 task tree，故新构建副本必须改独立输出边界，允许显式 P/PN runtime 选择；其余源列表/manifest 保留。当前 main 只接受 cold/hot/policy/trace（`R:2496–2507`），不得假装已接受 ring/count 参数。
2. 新 setup 按上表建立固定源和目的环；只初始化两个源，给每目的页不同旧 sentinel，记录静态映射表哈希。reset 前一次初始化，不在 parent 间 poison 或改数据来生成状态；源在运行期间恒定。
3. 新供给器保存每 parent `{lane,j,src,dst,NEXT_ID,cfg_begin/end,next_begin/end,poll_done_cycle}`。沿用 `program_single` 的 CONF=0/SRC/DST/NB=4096（`R:1503–1514`）和真实 AXI cfg FSM（`D:950–1028`）。逐 lane exact DONE 后才发下一 parent，启动顺序、poll优先级和配额跨全部配置相同。
4. 改 paired 的单次报告/断言为 N-copy，并保留硬件 issue/done 上限和单active断言（`R:943–983`）。每 parent 保留512返回beat、4096读/写字节、2个256beat AW/2B和最后B≤DONE；全局 TL request=response、ID/LAST、B ID、无响应错误。不能只将总数乘64就不验证逐 parent。
5. 将 AW 目的范围 guard 从 V 单页/A旧环改为精确静态映射+当前 parent 目的；源 guard 缩为两个固定源。原守卫在 `R:1315–1330、1385–1400`；DDR宽度/INCR/256beat AW guard 在 `R:2367–2373` 保留。固定源重复写相同数据会让“只验最后内存”漏掉中间坏写，因此 W handshake 时按 pending AW 的实际地址/beat 检查每个 strobe字节等于固定源，并逐 parent 守恒；最终再验所有触及目的和两源不变。此即时 W数据检查是**需新增**，旧 `R:1338–1357` 只计数并写入 mem，旧最终数据检查在 `R:2382–2387`。
6. 每 lane 独立 watchdog 以该 lane issue/R/W/B/DONE 等实际进度刷新，另有全任务硬上限；不可让 A 进度掩盖 V 卡死。quiet 继续要求 TL无inflight、双DMA job_quiet、CPU quiet及 mem/AW/B 队列全空（`R:1695–1729`）。保留 FAIL/timeout 与原始事件，不丢失败配置、不以失败补点。

## 观察与计时：现有足够什么，最少还缺什么

**无需新增 DUT 端口即可确认持续双parent重叠**：系统 top 已有双lane issue/done、job_quiet、banked TL req/resp、PTW active/R、IOTLB aggregate hit、DDR AR/AW/R/W/B（`S:135–210`）。C++按硬件 issue/done更新busy，统计双busy周期、各lane空洞和每parent队列长度；原 warm已有此计数方式（`R:905–918`）。busy overlap只证明未完成任务重叠，不能单独证明翻译端口同周期竞争。

**定位行为所需最少只读增强**：将 IOTLB lookup/hit 加上当次所属 bank/DID/VPN，记录 per-lane hit/miss；既有输出只有 aggregate，不能拿 main_bank无条件给 N probe归属（`S:209–210、251–252`，`T:675–685`）。若要区分读/写 miss，再加当次 lookup类型；端口必须绑定实际 normal/fast lookup选择，不驱动DUT。现有 TL req/resp bank可计响应等待，已有转译AW阻塞标志（`S:1422–1438`）及 DDR/PTW事件可先复用。未承诺这些新归属端口已实现。

`C:2374–2463` 的扩展观察器有backend接收、本地AR/RLAST、DS selector等，只读但含first-PTE/O专属断言及单次标记；不可整段盲用作多parent发现统计。要复用则每真实新parent重置局部计数，并正确处理同周期DONE→新backend，不让观察器顺序制造假失败。最小方案可从 R 的通用回调重建。

真实端点：t0=初始化/可选公开前缀结束后的quiet、**第一次任何配置之前**（`R:2286–2289、2485–2487`）；每lane最终硬件DONE−t0为T_lane；双方最后DONE−t0为T_data；最终quiet−t0为T_return（现行定义 `R:2438–2442`）。另报逐parent ready（第0个=t0，其后=前个硬件DONE）、issue、DONE、CPU观察DONE/补发延迟及末B；不得把软件poll完成代替硬件DONE。用全时代事件，不用旧 win_open 首V窗口裁短尾部（`R:1001–1013、2317–2329`）。

所有初始冷填充和后续环回都保留；若另分warm/steady区间只能作为同一完整轨迹的标注，不能删除启动或挑相位。全部状态由上述合法外部COPY序列自然产生，无任意启动delay和内部cache改写。此层仅为局部RTL+行为DDR+真实DMA/MMIO，不是原生CPU/真实DDR/FPGA周期结果（runner/README.md:39–42）。
