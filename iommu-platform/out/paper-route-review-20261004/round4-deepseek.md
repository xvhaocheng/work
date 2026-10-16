# Round4 Reviewer 模拟：体系结构、OS/系统、系统安全三份独立意见

> 裁决口径：当前阶段我允许给出拒稿意见。以下不是承诺可发表，也不新增研究路线。  
> 文件缩写：`R3C` = `out/paper-route-review-20261004/round3-codex-reconstruction.md`；`R3D` = `out/paper-route-review-20261004/round3-deepseek.md`；`VR` = `out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md`；`LR` = `docs/research-literature-review-20261004.md`。

## 0. 共同前提与自查：Round3 接口示意不是已确认安全设计

Round3 DeepSeek 自己已声明：所有实现、寄存器、ioctl、RTL 连接均为未编译、未上板提案（`R3D:3`）。因此以下都只能当“待验证门禁”，不能当事实：

1. `part_did_i [7:0]`、`iommu_mode_o [1:0]`、`iommu_mode_generation_o [7:0]` 只是提案（`R3D:87-93`）。实际 DID 宽度、mode/generation 宽度、是否与既有 ABI 冲突，未从给定证据确认。
2. “DID0=A、DID1=V”“只用两个 DID”“`owner_part = alloc_owner_did_i[0]` 按 parity 分半”是实验假设（`R3D:90,113,118`），不是已验证硬件属性。真实活跃 DID 集合、DID 是否稳定、是否有其他 DID 活跃，未确认（`R3D:427`）。
3. 8-bit generation 自增（`R3D:92,141`）没有回绕、ABA、跨模式比较语义；不能当已确认安全机制。
4. `iotlb_flush_req <= 1'b1` 单拍 flush（`R3D:142`）与 Linux ioctl 中先写 mode 再 `legal_flush`（`R3D:198-203`）没有 flush 完成握手、顺序证明、失败回滚、旧请求 drain 证明。不能当“静止切换已正确”。
5. 当前 IOTLB lookup 按 PSCID/GSCID/global/stage 匹配，`update_i` 不含 DID；分配身份必须从真实主查表请求稳定 DID 传递，不能拿 TL 当拍探测 DID 当 fill owner；不得改权限匹配（`R3C:25`）。所以“按 DID 8/8 分区”与实际匹配域是否一致，未证明。
6. v5 已明确：S/N 使用不同 BIT、不同 fresh boot（`VR:27,45,220`）；`original` 复用旧基线（`VR:219`）；快速 accept 是 accepted requests，不是 hit rate，也不是全部 miss（`VR:170-171`）；A8 三轮 V 吞吐 104.263/92.987/93.048，全部保留（`VR:151`）。这些限制直接否定当前因果/显著性主张。

---

## 1. Reviewer 1：体系结构 / 新颖性 reviewer

**独立裁决：主 A 当前应 Reject；后备 B 不应作为主线，当前也不应作为安全/QoS 新机制发表。**

### 1.1 为什么不值得发表

主 A 的两个机制组件都是已知技术：

- P 是 cache partition / coloring / 静态容量分配；N 是 hit-under-miss / non-blocking cache。HPCA2017 已明确 private TLB 与 shared L2 TLB 支持 non-blocking hit-under-miss（`LR:19`）；MemCoRe 已做 FPGA 快速节流、token bucket、address-aware（`LR:25`）；HyperTRIO 已涉及翻译结构分区与 HOL/2D walk（`LR:26`）；Bermuda 已做共享 IOTLB 干扰表征并提出 stream-ID QoS/page coloring（`LR:17`）。
- Round3 DeepSeek 自己也承认：P/N 本身不是新机制，P+N 是已知组合，主 A 有很大概率只是复述缓存常识（`R3D:40,63-68,70-73`）。
- 当前唯一可能的新贡献候选很弱：在 RISC-V IOMMU 的 IOTLB tag 实际按 PSCID/GSCID/global/stage 匹配、update 不含 DID 的结构下，按稳定 DID 做 8/8 分配是否真的保护实际匹配域；以及 N 的 hit 服务与 P 的替换保护是否在同一单 FSM/单响应口下互补（`R3D:70-72`）。但这最多是设计空间/表征/负面边界，且尚未验证。

因此，“连续热 V 是否掩盖容量隔离需求”作为 RQ 单独看，回答“冲突时保护有用、hit-under-miss 有用”几乎必然接近缓存常识。除非给出非平凡交互边界和条目级证据，否则不值得作为机制论文。

### 1.2 证据够不够

不够，且当前证据不能支撑主要假设：

- H1：P 的收益取决于 V 两次访问之间 A 插入的翻译工作集（`R3D:57`）。当前没有 P 实现，没有 per-entry 替换来源/首次 miss 证据，H1 未测。
- H2：同 BIT 下 N 让 hit 前进，P 减少替换；P+N 若优于单项，必须看到 V 条目存活改善和真实快速服务事件（`R3D:58`）。当前只有 N 候选/probe/DC/leaf/fast accept 非零，没有 P，没有 P+N 增量，H2 未测。
- v5 只能证明 N 机制在 A8/A16 可被真实触发：A8 candidate/probe/DC/leaf/fast 非零，A16 非零并有 3 个 leaf_lookup_miss；S 在 A8/A16 有 candidate_in_window 但 probe_starts=0、fast=0（`VR:180-181,196-199,203-206`）。
- 性能层面 N/S A8 约 +4.2%、A16 约 -2.7%、A1/Solo 近似持平，均为描述性；N/原版 A8 约 -14.52%、A16 约 -22.09%，N 相对原版仍低（`VR:87-89,151,160-164`）。不能宣称 N 改善。
- 统计上 n=3，不提供显著性；A8 三轮差异明显；任意 2%/5%/10% 阈值已被撤回，必须先测配对 S/S 散度再冻结 MDE（`R3C:8`；`R3D:33,245,356`）。

### 1.3 Alternative explanation

- N/S 差异可由不同 BIT、不同 fresh boot、布局、资源、时序、缓冲深度、启动相位解释，而非纯 runtime enable（`VR:27,45,160,220`）。
- original→S 同时含分 bank、缓冲/layout、启动条件差异，不能作纯布局因果（`VR:159`）。
- fast accept 非零不证明 V 得到真实收益；candidate_in_window 不证明 probe；没有 probe 时不能推出 accept（`VR:203-206`）。
- P 若有效，可能是因为 V 获得专用半树而减少冲突，不必然是“按 DID 保护了实际匹配域”；也可能只是 PLRU 根限制导致分配偏向。
- P+N 若看似优于单项，可能只是 max(P,N) 的移动瓶颈，或把等待搬到同一单 FSM/单响应口。
- B 的跨域暂停可由软件 ioctl、CQ/fence 排空、诱发 walk 或全局 flush 解释，不必然是失效生命周期独立效应（`R3D:275-279,286,297`）。

### 1.4 Baseline 公平性

当前不公平：

- S/N 不同 BIT、不同 boot（`VR:27,45,220`），不能作同 BIT runtime enable 因果。
- original 复用 06:07 旧基线，不是同轮同条件三臂（`VR:219`）。
- original 受旧 legacy3 限制，S/N 为 512 缓冲（`VR:159`），资源/布局不可比。
- P 的 8/8 静态分配会改变 V 可用条目数；必须证明“同总量/同资源/不加优先级”的合理 N 与同容量静态 P 才公平，否则 reviewer 会判为调参或容量偷换。文献已警告弱基线会误导（`LR:21,30-34`）。
- N 不能是弱实现：若不是有限非阻塞、公平选择、合法 hit 不因 miss 记录满而全局停，baseline 不公平（`LR:71`）。
- 同窗口 A 吞吐与 V 吞吐共用窗口，A 进度会随 V 完成时间变化；不能把二者相等外推为独立总线利用率（`VR:115,136`）。

### 1.5 方法 / cache ownership 正确性

- DID 宽度、固定 DID0/DID1、按 parity 分配都不是已确认事实（`R3D:90,113,118`）。若真实 DID 不是 0/1 或不止两个活跃 DID，P 必须禁用或显式扩展，不能 silently parity（`R3D:113`）。
- IOTLB tag 不含 DID，lookup 按 PSCID/GSCID/global/stage；update_i 不含 DID；fill owner 不能拿 TL 当拍探测 DID（`R3C:25`）。因此必须证明 V/A 的匹配域真正分离，否则“DID 分区保护 V”不成立。
- 8/8 必须在 PLRU 选择根处限制半树且不改变权限匹配/失效语义（`R3C:25,27`）；具体代码路径未给出，需要编译/静态确认。
- mode 切换必须全静止并合法 flush，不能留“名义分区、旧条目跨区”污染（`R3C:27`）。当前 flush 单拍提案与 ioctl 顺序不足以证明。

### 1.6 Round5 最小阻断问题（体系结构）

1. 实际 RTL 中 `nonblock_enable_i`、`banked_ctl in_flight_o/e3.quiet`、`TL ptw_active`、`research_dma_quiet`、fast pending/response/backpressure 的层次、宽度、时钟域必须由编译/静态检查确认（`R3D:424`；`R3C:28`）。若不存在可组合的静止谓词，主 A 同 BIT 切换不成立。
2. PLRU 根限制半树的精确代码路径必须由源码/综合确认；否则 P 不能声称 8/8（`R3C:27`）。
3. DID 实际宽度、活跃 DID 集合、DID0/DID1 是否可保证，必须确认；不能继续把 parity 当事实（`R3D:113,118,427`）。
4. 必须证明 DID 分区与 PSCID/GSCID/global/stage 匹配域不冲突；若冲突，删除“DID 分区保护”主张（`R3C:25`）。

### 1.7 必须降级的主张

- “同 BIT 四模式可安全切换”——未编译未上板，降级为提案。
- “P 保护实际匹配域”——未验证，且 tag 不含 DID，降级为待测假设。
- “N 改善性能”——v5 不支持，只能写“机制在 A8/A16 可被触发”。
- “连续热 V 掩盖容量隔离需求”——不能外推，只能保留为受控 E2 式边界待测。
- “hit-under-miss / 分区 / token bucket / 纳秒级”不是创新（`LR:42`）。

---

## 2. Reviewer 2：OS / 系统 / 实验与 Artifact reviewer

**独立裁决：主 A 当前 Reject；后备 B 当前只配 Linux-only 负面对照，不能作主线。**

### 2.1 为什么不值得发表

- 主 A 的核心控制条件“同 BIT 可切换四模式”尚未实现。Round3 说存在 `nonblock_enable_i` 是前置，但存在信号不等于同一 BIT 内行为等价 S、资源不变、切换安全（`R3C:6,29`；`R3D:31,36`）。
- 当前 ABI 未确认：现有 `nonblock_enable_i` 在 peripherals 连 `IOMMUNonblockMode==2`；新控制 ABI 必须独立识别，不能伪装旧 role2，不能放宽旧 validator（`R3C:29`；`R3D:104,107`）。
- Linux ioctl 只是提案：`idma_research.h`、`idma_ioctl_iommu_mode`、MMIO、ioctl 基址、失效接口均未确认（`R3D:169-183,424-431`）。
- 静止切换检查不完整：不能只信 CPU idle；必须检查硬件 `banked_ctl in_flight_o/e3.quiet`、`TL ptw_active`、DMA quiet，并完整检查 fast pending/response/backpressure（`R3C:28`；`R3D:123-136`）。
- 性能证据不具因果性：S/N 不同 BIT、不同 boot；N/S A8 正、A16 负；A8 三轮差异大；无配对 S/S MDE；不能做统计显著（`VR:87-89,144-151,220,238`）。
- 后备 B：当前无选择性 A 域失效接口确认；若只有全局 flush，B 立即停（`R3D:303`）。空命令若复现同量暂停，判纯软件开销，B 停（`R3D:328,337`）。

### 2.2 证据够不够

不够：

- 同 BIT 四模式无编译产物、无 bitstream hash、无模块、无读回（`R3D:211-215`）。
- mode/generation 读回未验证；模式切换成功/失败未记录（`R3D:151`）。
- flush 后无跨区污染未验证；权限/匹配语义未改变未验证（`R3D:247,252-253`）。
- P 不改变首次 miss/替换来源未验证；N 不改变真实 fast 服务未验证（`R3D:243,253`）。
- B 的 ioctl、空命令、选择性失效、CQ/fence 排空、诱发 walk 分离均未测（`R3D:306-313`）。

### 2.3 Alternative explanation

- N/S 差异可由不同 BIT/boot、布局、时序、缓冲深度解释（`VR:27,45,160,220`）。
- original→S 可由分 bank、缓冲/layout、启动条件解释（`VR:159`）。
- A8 三轮 104.263/92.987/93.048，可能是运行变异而非 N 效应（`VR:151`）。
- short-a8 有 fast accept 但短组不并入长组；不能把短组正计数当性能（`VR:109,223`）。
- S 有 `candidate_in_window` 但无 probe，不能反推 S 无候选（`VR:199,203-206`）。
- B 的 V 暂停可由 ioctl 软件成本、CQ/fence 排空、诱发 walk、全局 flush 侧效应解释，不必然是单 FSM（`R3D:275-279,337-340`）。

### 2.4 Baseline 公平性

必须满足，当前未满足：

- 同一 BIT、同一 boot、同一仪器、同一时钟、同一完整 4 KiB copy 动点、A/V 固定工作量（`R3D:226-229`；`R3C:17-18`）。
- 主表只保留 S/N/P/P+N；原版只作整体开销锚，不混入因果表（`R3D:219`；`R3C:20`）。
- A 进度用同一 V 硬件窗口内有效完成字节，不用不同 V 窗口 A 字节直接比较（`R3D:228`）。
- 双 DMA 方向作验证条件，不作新因子；CPU-only 不进入主表（`R3D:229-230`）。
- MDE 必须先测同条件配对 S/S 散度再冻结，不预设 2/5/10（`R3D:245`；`R3C:8`）。
- B 必须有：无命令、空命令同次数、合法 A 域选择性失效、全局 flush 仅作非法对照、CPU-only 软件开销单独测（`R3D:315-322`）。

### 2.5 ABI / 静止切换 / flush 时序正确性

当前不够格：

- 新 ABI 必须独立识别，不能伪装 `IOMMUNonblockMode==2`，不能放宽旧 validator（`R3C:29`）。
- ioctl 中 `idma_iommu_wait_hw_quiescent` 必须读 RTL quiet，不是 CPU idle（`R3D:198`）。但 quiet 层次/宽度未确认（`R3D:424`）。
- `mode_switch_ok` 只是示意，未证明包含完整 fast pending/response/backpressure、DMA quiet、outstanding、PTW active（`R3D:128-136`；`R3C:28`）。
- `iotlb_flush_req` 单拍（`R3D:142`）与 ioctl `idma_iommu_legal_flush`（`R3D:202`）无 flush 完成等待；可能 ioctl 返回后旧条目仍被使用。
- generation 8-bit 自增（`R3D:92,141`）未定义回绕/ABA；不能在安全或统计上依赖。
- 模式切换期间必须无新请求进入；否则旧请求/新 mode 混用，generation 记录也不清。

### 2.6 Round5 最小阻断问题（OS/系统）

1. 编译并静态确认：`nonblock_enable_i`、`in_flight_o`、`e3.quiet`、`ptw_active`、`research_dma_quiet`、fast pending/response/backpressure 的精确层次、宽度、时钟域。若不能组成完整静止谓词，停止同 BIT 主张。
2. 只读确认 Linux `idma.c` 现有 ioctl 基址、MMIO 映射、是否有选择性失效接口；若只有全局 flush，B 停（`R3D:303,426`）。
3. 确认新 ABI 与旧 role2 validator 不冲突；模式/generation 读回格式必须固定并可记录。
4. 编译产出 bitstream + 模块，记录 hash、boot id、Linux 版本；否则不能声称可执行产物（`R3D:211-215,261`）。
5. 上板必须先做四模式正确性/读回/合法 flush/无跨区污染验收；否则不进入性能表（`R3D:259-267`）。

### 2.7 必须降级的主张

- “一次 SD 更新装入新驱动/工具即可”——可接受，但当前未编译，不能当已交付（`R3D:214`）。
- “模式只在静止边界切换”——当前只是提案，不是已验证行为。
- “generation 可识别切换”——8-bit 自增未定义回绕，降级。
- “B 跨域暂停独立于软件成本”——未做空命令对照，降级。

---

## 3. Reviewer 3：系统安全 / QoS / 威胁模型 reviewer

**独立裁决：主 A 当前 Reject；后备 B 当前 Reject。不能把 QoS 干扰包装成安全隔离失败，也不能把 Round3 接口示意当完整安全设计。**

### 3.1 为什么主 A 不值得作为安全论文发表

- 主 A 没有威胁模型。当前叙事是“保护 V 服务”“缓存保护×命中先服务”，这是 QoS/性能隔离，不是安全边界。
- IOTLB tag 不含 DID，lookup 按 PSCID/GSCID/global/stage；update_i 不含 DID；不能改权限匹配（`R3C:25`）。因此“按 DID 做 8/8 分区”与实际 lookup 匹配域可能不一致。若 A/V 共享 PSCID/GSCID/global/stage，DID 分区可能不改变命中/权限路径，形成“分配按 DID、命中不按 DID”的所有权错配。没有证明前，不能称安全隔离。
- 固定 DID0/DID1、按 parity 分配、8-bit generation、flush 时序都不是已确认事实（`R3D:90,113,118,92,141-142`）。这些是接口草案，不是安全设计。
- 没有定义：谁可写 mode？设备是否可访问该 ABI？ioctl 权限？切换失败回滚？generation 回绕？旧条目跨区？恶意 A 能否触发？合法 A 的失效是否算攻击？当前都缺。
- 已知文献已覆盖：IOTLB invalidation 成本、TLB shootdown、QoSID 只定义身份关联不保证内部调度/公平/时延（`LR:23`）；专利已公开优先级/利用率/老化（`LR:24`）；Bermuda 已做共享 IOTLB 干扰与 QoS 调节提案（`LR:17`）。不能声称新安全威胁。

### 3.2 为什么后备 B 不值得作为安全论文发表

- B 的前提是“合法 A 域选择性失效、V 映射不变”。若这是合法 QoS 行为，V 服务暂停是性能隔离问题，不是漏洞（`R3D:394-398`）。
- 失效成本本身已有大量工作；rIOMMU、Price of Safety、Overcoming IOTLB wall 被列为削弱新颖性来源（`R3D:288-297`）。B 只能争非常窄的负面边界。
- 若平台只支持全局 flush，全局 flush 会破坏 V 映射，实验前提不成立，B 死（`R3D:303,337`）。
- 空命令对照很可能把效应打成软件开销；若空命令复现同量暂停，B 停（`R3D:328,337`）。
- 必须区分：ioctl 软件路径、CQ/fence 排空、诱发 walk 间接竞争、单 FSM/单响应口传播（`R3D:275-279`）。当前均未分离。

### 3.3 证据够不够

不够：

- 选择性 A 域失效接口未确认（`R3D:303,426`）。
- 空命令、真实选择性失效、全局非法对照、CPU-only 软件开销均未测（`R3D:315-322`）。
- quiet/outstanding 观测不足；若无法把软件发起与硬件 quiet 分开，B 停（`R3D:337`）。
- 没有威胁模型，无法定义攻击者能力、目标、安全属性、失败后果。

### 3.4 Alternative explanation

- V 暂停可由 Linux ioctl/软件成本解释。
- 可由 CQ/fence 排空/outstanding 解释。
- 可由失效诱发的 A walk 间接竞争解释，属普通竞争，不是失效生命周期独立效应（`R3D:285,297,338`）。
- 可由全局 flush 副作用解释；但全局 flush 非法，不能当 baseline。
- 可由 A 同窗有效字节/V 完成时间窗口偏差解释。

### 3.5 Baseline 公平性

B 的唯一公平对照是：

- 无命令；
- 空命令，同次数、同 CPU 路径；
- 合法 A 域选择性失效，V 映射不变；
- 全局 flush 只作非法对照，不作 baseline；
- CPU-only 软件开销单独测；
- A 同窗有效字节、V 固定工作量（`R3D:315-322`）。

缺一即不能归因。

### 3.6 ABI / 威胁模型 / cache ownership 正确性

- 主 A 的 mode ABI 必须是特权控制，不能由设备直接写；当前提案没有权限/角色/回滚/失败状态说明。
- 静止切换必须证明：mode write 只在硬件 quiet 后发生；flush 完成前无新翻译；generation 更新与 mode 读回原子；旧条目不能跨区命中。
- cache ownership：DID 不在 tag；fill owner 必须来自真实主查表稳定 DID，不能拿 TL 探测 DID（`R3C:25`）。parity 分配未验证；若其他 DID 活跃，P 必须禁用/扩展（`R3D:113`）。
- 权限匹配不能因 P/N 改变；失效语义不能改（`R3C:25,27`）。当前未证明。
- 后备 B 的 ABI 必须确认“合法 A 域选择性失效”是否存在；若不存在，停止。

### 3.7 Round5 最小阻断问题（安全）

1. 明确威胁模型：A 是 untrusted 还是合法 QoS 域？若合法 QoS，删除“安全隔离失败”措辞。
2. 证明 DID 分区与 PSCID/GSCID/global/stage 匹配域不冲突；若冲突，P 不能称安全隔离。
3. 证明 mode ABI 特权、不可由设备访问、切换失败安全回滚、generation 无 ABA、flush 完成前无新翻译。
4. B 先只读确认选择性失效接口；若只有全局 flush，B 停。
5. B 必须空命令同次数同 CPU 路径对照；空命令复现则停。

### 3.8 必须降级的主张

- “缓存保护提供安全隔离”——降级为 QoS/性能隔离候选，当前无安全证据。
- “DID 8/8 分区保护匹配域”——未验证，且 tag 不含 DID。
- “A 域合法失效造成跨域安全失败”——降级为 QoS 干扰待测；合法 QoS 降级不是漏洞。
- “Round3 接口是安全设计”——降级为接口草案，不是完整安全设计。

---

## 4. 主 A 与后备 B 的逐项审查矩阵

| 维度 | 主 A 当前状态 | 后备 B 当前状态 | Round5 最小阻断 |
|---|---|---|---|
| 值得发表性 | 机制已知，P+N 大概率缓存常识；最多负面/表征，当前 Reject | 失效成本已知，合法 QoS 暂停非漏洞；当前 Reject | 不新增机制；先过门禁，否则停 |
| 证据 | v5 只证 N 可触发，不证收益；P/P+N 未实现；无 per-entry 证据 | 未确认选择性接口；无空命令对照 | A：同 BIT 四模式 + 条目级证据；B：只读接口 + 空命令 |
| Alternative | 不同 BIT/boot/layout/时序/窗口/计数语义 | 软件开销/CQ 排空/诱发 walk/全局 flush | 分别消除 |
| Baseline 公平 | S/N 不同 BIT/boot；original 复用旧基线；P 容量变化 | 无命令/空命令/选择性失效/CPU-only 未做 | A：同 BIT 同 boot；B：同路径空命令 |
| 方法正确性 | P 定义、N 定义、P+N vs max 未验证 | 失效类型/范围/outstanding 未分离 | A：机制门禁；B：三类延迟分离 |
| 威胁模型 | 缺失；QoS 当安全 | 合法失效算不算攻击未定义 | 先写清，否则降级 |
| ABI | 未编译；旧 role2 混淆风险 | 选择性失效 ABI 未知 | 编译确认独立识别 |
| 静止切换 | 谓词不完整；flush 无完成握手 | 不适用 | 上板验证无跨区污染 |
| cache ownership | DID 不在 tag；parity 未验证 | 不适用 | 证明匹配域或删除安全主张 |
| 8-bit generation | 未定义回绕 | 不适用 | 确认宽度/回绕/读回 |
| flush 时序 | 单拍 flush 不足 | 全局 flush 非法 | 完成握手 + 无新请求 |

---

## 5. 最终阻断清单（Round5 必须关闭）

### P0：不关闭就不允许进入性能/发表主张

1. **同 BIT 四模式与 ABI 正确性**  
   - 证据：`R3C:6,29`；`R3D:87-107,169-183,424`。  
   - 阻断：新 ABI 是否独立于 `IOMMUNonblockMode==2`；旧 validator 是否被放宽；模式/generation 读回格式。  
   - 批次：下一批编译/静态检查可解决；读回行为需上板。

2. **完整静止谓词**  
   - 证据：`R3C:28`；`R3D:123-136,424`。  
   - 阻断：`in_flight_o/e3.quiet/ptw_active/research_dma_quiet/fast pending/response/backpressure` 是否同层次、同域、可组合。  
   - 批次：编译/静态确认；动态安全需上板。

3. **flush 完成与无跨区污染**  
   - 证据：`R3D:142,202,247,252-253`。  
   - 阻断：单拍 flush、ioctl 写后不等完成、旧条目跨区。  
   - 批次：设计/编译确认握手；上板验证污染。

4. **DID 宽度、固定 0/1、parity、cache ownership**  
   - 证据：`R3D:90,113,118,427`；`R3C:25`。  
   - 阻断：DID 不在 tag；按 parity 分配与实际匹配域可能错配；其他 DID 活跃时 P 不成立。  
   - 批次：编译/静态确认宽度与路径；匹配域行为需上板或第二平台。

5. **generation 语义**  
   - 证据：`R3D:92,141`。  
   - 阻断：8-bit 回绕/ABA/跨模式比较。  
   - 批次：编译确认宽度；语义测试上板。

6. **P 必须改变首次 miss/替换来源**  
   - 证据：`R3D:243,253,264`；`VR:170-171`。  
   - 阻断：fast accept 不是 hit rate；candidate 不是 probe；无 per-entry ownership/替换来源。  
   - 批次：仅上板 + 条目级观测。

7. **N 必须改变真实 fast accepted service，而非仅计数**  
   - 证据：`R3D:243,253,265`；`VR:170-181`。  
   - 阻断：v5 只证明可触发，不证明 V 收益。  
   - 批次：上板。

8. **P+N vs max(P,N)**  
   - 证据：`R3D:242,254`。  
   - 阻断：无增量则停止“组合必要”主张。  
   - 批次：上板。

9. **Baseline 公平与统计**  
   - 证据：`VR:27,45,151,159,219,220,238`；`R3D:245-246`。  
   - 阻断：不同 BIT/boot、original 复用、n=3、A8 变异、无 MDE。  
   - 批次：同 BIT 同 boot 上板；MDE 需先测 S/S。

10. **后备 B 的选择性失效接口**  
    - 证据：`R3D:303,337,426`。  
    - 阻断：若只有全局 flush，B 停；空命令复现，B 停。  
    - 批次：Linux 只读确认 + 上板空命令对照；无接口则只能第二平台。

11. **安全威胁模型**  
    - 证据：`R3D:394-398`。  
    - 阻断：合法 QoS 降级不是漏洞；DID 分区未证安全。  
    - 批次：文档/设计澄清；安全泛化只能第二平台。

### P1：主张降级

- “同 BIT 四模式已可切换” → 未编译未上板提案。
- “P 保护实际匹配域” → 待测假设，且 tag 不含 DID。
- “N 改善性能” → v5 不支持，只可说机制可触发。
- “连续热 V 掩盖风险” → 不可外推，只可保留受控 E2 式边界。
- “B 是安全失败” → 降级为 QoS/性能干扰待测。
- “DID0/DID1、parity、8-bit generation、flush 时序已确认” → 全部降级为未确认接口。
- “hit-under-miss / 分区 / token bucket / 纳秒级” → 明确不是创新（`LR:42`）。

---

## 6. 哪些问题下一批编译可解决，哪些只能上板，哪些只能第二平台

### 下一批编译/静态检查可解决

- 实际 RTL 端口层次、宽度、时钟域：`nonblock_enable_i`、`in_flight_o`、`e3.quiet`、`ptw_active`、`research_dma_quiet`、fast pending/response/backpressure（`R3D:424`；`R3C:28`）。
- PLRU 根限制半树的具体代码路径（`R3C:27`；`R3D:111`）。
- DID 实际宽度、活跃 DID 集合、tag/update_i 匹配字段（`R3C:25`；`R3D:90,118,427`）。
- Linux `idma.c` ioctl 基址、MMIO、是否存在选择性失效接口（`R3D:426`）。
- 新 ABI 与旧 role2/validator 是否冲突（`R3C:29`）。
- mode/generation 寄存器宽度、编码、回绕定义（`R3D:91-92,141`）。
- bitstream 与模块能否编译，资源/Fmax 记录（`R3D:211-215,237,261`）。

### 只能上板解决

- 模式切换时完整静止、flush 完成、无跨区污染、权限/匹配语义不变（`R3D:247,252-253,259-267`）。
- P 是否真实改变 V 首次 miss/替换来源/条目存活（`R3D:243,264`）。
- N 是否真实产生 V 合法 hit 的 fast accepted service，而非仅计数（`R3D:243,265`；`VR:170-181`）。
- P+N 是否超过 max(P,N) 与配对散度（`R3D:242,254`）。
- S/S 配对散度、MDE 冻结、同 BIT 同 boot 配对 ABAB（`R3D:244-245`）。
- B 的空命令 vs 真实选择性失效、CQ/fence 排空、诱发 walk 分离（`R3D:315-322,337-340`）。
- 双 DMA 方向、连续热 V vs 间歇 V；若硬件 gap 不能安全复用，只能 E2 式前缀+探测（`R3D:222-223`）。

### 只能第二平台/第二实现解决

- 安全隔离、DID 分区保护、跨域 QoS 干扰的外部有效性；单 FPGA 单 IOMMU 不能泛化。
- 若当前平台只有全局 flush、无合法 A 域选择性失效，B 必须停或换平台；不能在当前平台伪造（`R3D:303,337`）。
- 不同 IOMMU 实现、不同 PSCID/GSCID/global/stage 匹配语义下的结论。
- 侧信道/共享翻译安全主张的第二实现或模型验证。
- 任何“架构级规则”的跨平台可复现性。

---

## 7. 最终立场

- **主 A：当前应 Reject 作为机制/论文主张。** 若能过 P0 的 1-8 门禁，且结果显示 P 改变条目级替换来源、N 改变真实 fast 服务、P+N 超过配对散度，才值得 Codex 重新评估是否作为表征/设计空间工作；仍不承诺可发表。
- **后备 B：当前应 Reject 作为安全/QoS 主线。** 只能先做 Linux 只读确认和空命令对照；若只有全局 flush 或空命令复现，立即停。
- **Round3 接口示意：不是完整安全设计。** DID 宽度、DID0/DID1、parity 分配、8-bit generation、flush 时序均不能作为已确认事实；它们只能是 Round5 编译/上板门禁。
- **不新增研究 idea。** 本轮只给拒稿意见、必须降级的主张、最小阻断清单以及“编译可解 / 上板可解 / 第二平台可解”的分类。