# B1-STRIPE-COST-1：HERO 单条 B2 源码证据卡

**结论：原定 s=0、t=0 计算 B0／提前准备 B2、t=1 提交 B2 的切片成立。** B0 和 B2 复用 `b_ptrs[0]`；释放点是 t0 计算的隐式 barrier。源码只证明地址可提前计算与合法槽复用，**没有证明普通预热/PTE cache 后存在强制驱逐或阻挡完整结果的残余等待**。实际路径是 copy-based device1；SVM device0 分支为 `if(0)`。这是一张正常源程序的条件映射卡，不是当前平台共享 SID/IOMMU 的实测轨迹。[S1:164–228,369–404]

本席独占范围：源码事件、几何、源级普通残余可判定范围；不做实现成本布局、全局排名或后继授权。采用 ARS-Codex deep-research 的 bounded source-verification/fact-check 角色，不启动完整 pipeline。请求 Sol/high；实际模型及强度缺独立运行元数据，均为 `unknown`。Codex-only，未使用 DeepSeek/Claude。

## 1. 来源与身份

S1/S2/S3 均为已保存的 HERO 源码快照，当前 SHA-256 与输入清单一致；完整绝对路径、hash、阅读行界见 `source-manifest.json`。代码事实证据等级为“主源码快照”，不赋予板上测量等级；论文 venue/作者 COI 检查不适用于这三份源码。本席没有新增论文、网络检索或原生运行。

- S1 = `.../hero/openmp-examples/mm-large/mm-large.c`，sha256 `8b16b51169bb31b7eb290eb5f309ddfbba25fa94e3fae3b327baadf491360186`。
- S2 = `.../hero/support/libhero-target/inc/hero-target.h`，sha256 `24266b241f40dffdc35ad74ee574a5fcd3134e045161d463c63bb84a4df4d594`。
- S3 = `.../hero/support/libhero-target/pulp/hero-target.c`，sha256 `c56ac379b7058ebe47fd11189a7ae37bfc02dd9a36aecca8946ec694abc948b6`。

`main` 实际调用 `target device(1)`，map(to:A,B)/map(from:C)，标签为 copy-based；`device(0)` 的 SVM 路径放在永假分支。目标侧参数 `a/b/c` 与 host malloc 返回值的具体关系由未核的 offload/runtime 决定，不能直接认定本卡 `b` 为 Linux host 原 VA，更不能自行绑定当前四 iDMA/IOMMU 的 DID/SID/SSID、页表或时间轴。[S1:307–311,367–404]

## 2. 默认几何：B2 是一维连续 16 KiB 搬运

无命令行参数时 height=256；stripe_height 从128按条件减半，32时等于32 KiB仍继续减半，最终16；width=height=256，n_stripes=16，`S=stripe_size_b=256*16*4=16384`。A/B/C各256 KiB，SPM为各两个16 KiB分配，总96 KiB的请求分配量；不据此推定物理 bank、可用容量或实际成功。[S1:49–75,284–310]

B的“vertical stripe”是输出列角色；源码实际使用 `b_ptrs[!b_idx][j*width+k]`，配一维 `hero_memcpy_host2dev_async`，不是一组带步长的2D散段。B2地址范围为 `[b+32768,b+49152)`，对应B的存储行32…47，每行1024B。[S1:171–179,219–226; S3:58–70]

令P为实际页大小、x为目标侧B2首地址，则相同P下页数为 `floor((x+S-1)/P)-floor(x/P)+1`。下面只是共同映射条件，真实x、PA、页粒度、权限及页表层级未知：

| 条件 | B2结果覆盖数 | 可核限制 |
| --- | ---: | --- |
| 4 KiB，b页对齐 | 4 | B2首址同样页对齐，页号q+8…q+11 |
| 4 KiB，b页内偏移δ∈1…4095 | 5 | 页号q+8…q+12；最后一页覆盖δ字节 |
| 单个可行大页覆盖整个B2 | 1 | 必须已有合法大页leaf、IOVA/PA及权限/属性匹配，且span不跨该leaf；源码malloc未提供这些保证 |
| span跨大页边界或混合leaf | 依实际leaf | 大页不是任意把4/5项合并；必须由真实页表确定 |

在典型2 MiB leaf的条件例中：B2首址在leaf内偏移≤2 MiB−16 KiB时可单leaf覆盖；越界则至少两段。单stripe无需自身2 MiB对齐，但包含它的映射必须满足所用体系的大页对齐/覆盖要求。malloc既不证明大页存在，也不证明大页不可用；只读本组三文件无法确认当前映射。[S1:307–311；本节公式为算术投影]

同一4 KiB映射下，B1与B2因相邻且S为4P：b对齐时页集不交；b不对齐时共享一个边界页。普通B1搬运可能顺带使B2首leaf热，但是否发生、何时发生与是否保留均未知。逐一穷举δ=0…4095的几何算术检查已通过；不是DMA仿真。

## 3. a/r 和源码顺序

`a_info`：目标函数入口已持有b、width/height/stripe_height，可计算B2范围。**现有代码没有此时提交B2或prepare**；可知不等于已提交。[S1:44–50]

本卡冻结的可插入准备窗口：`a0` 在s0,t0的显式barrier（L208）通过之后、该轮 `omp for` 计算之前；`r0` 在t0 `omp for` 的隐式barrier完成之后、t1切换b_idx之前。这里利用无nowait的OpenMP for语义；源码没有给barrier的周期持续时间。允许在窗口内某时刻a实际发prepare，但该hook不存在，a、准备完成时间、MMIO/CPU开销及r0的实测时间都为unknown。[S1:208–234]

| 源顺序事件 | 索引／槽／所有者 | 证据 |
| --- | --- | --- |
| 初始化 | a_idx=b_idx=c_idx=0；每个a/b/c指针数组是SPM地址；job数组为线程firstprivate | S1:52–83 |
| 首次搬运 | thread0提交A0→a_ptrs[0]；thread1提交B0→b_ptrs[0] | S1:93–111 |
| s0外循环 | thread0令a_idx=1，提交A1→a_ptrs[1]，wait A0；thread2的C分支因s=0不执行 | S1:114–162 |
| s0,t0前段 | thread1令b_idx=1，提交B1→b_ptrs[1]，wait旧B0 job，即b_dma[0] | S1:165–205 |
| 显式barrier通过／a0 | A0、B0等待完成；A1/B1可能仍在途。此后计算读a_ptrs[0]、b_ptrs[0]，写c_ptrs[0]的首16列 | S1:208–228 |
| r0 | t0全部计算线程经过for隐式barrier；B0最后本地读已结束，b_ptrs[0]可复用 | S1:214–234 |
| s0,t1原生提交 | thread1令b_idx=0；ext_addr=b+2S；B2→b_ptrs[0]；覆盖b_dma[0]为新job；随后wait B1（b_dma[1]） | S1:165–202 |
| s0,t1计算 | 读B1于b_ptrs[1]；B2可在b_ptrs[0]搬运；本地C0写第16…31列 | S1:208–228 |
| s0,t2前段 | b_idx=1，提交B3→b_ptrs[1]，wait B2（b_dma[0]）；barrier后计算读B2于b_ptrs[0] | S1:165–228 |
| s0,t2结束／t3 | for隐式barrier释放B2本地数据最后用途；t3 B4可以复用b_ptrs[0] | S1:165–234 |

以上只有每线程程序顺序及屏障依赖；A/B不同线程的异步提交之间不构造额外总序。C首次外送是s1的 `c_dma[0]`（s1先翻c_idx为1，发送!c_idx=0）；不在本卡[a0,r0]内。[S1:143–161]

## 4. 真实owner与最后用途

- **SPM数据owner**：B0加载完成后，在t0由计算线程共同读取b_ptrs[0]；r0后该槽才能被B2覆写。B1在b_ptrs[1]，可能由DMA写入，t1 wait/barrier后转为计算读。B2在t1提交前没有SPM payload owner；提交后DMA写b_ptrs[0]，t2 wait/barrier后转为计算读，t2末释放给B4。共享b_idx负责选择槽，不能拿job ID代替SPM地址。[S1:52–83,165–228]
- **job ID owner**：thread1的firstprivate b_dma数组保存其B作业。旧b_dma[0]在t0 L202已wait；该32 bit字段可回收作handle，不必等r0。可是数据槽仍由B0计算占用。S2 job仅含uint32_t id；S3写MMIO源/目的/长度后读tf_id启动并返回；wait用高4位stream、低28位transfer ID轮询done。源码没有未来B2结果表、PPN数组或可免费借用的多端口存储。[S1:56–58,79–83,179–202; S2:72–74; S3:27–70,134–138]
- **翻译结果owner**：当前三文件不表示IOMMU结果owner。若新增一次性prepare，必须额外指定所有pending/ready/fault结果的有限归属；不把可回收ID等同于4/5个页结果。SPM释放是payload许可，不等同于翻译交接握手。每页结果可在本次B2 DMA最后一次需要该结果的物理请求被合法接收后释放，具体burst边界、重放/在途义务不在本组三文件中，时间为unknown。`hero_dma_wait(B2)`仅提供源码层较晚的完整搬运完成端点；不能据此强迫所有结果保留到t2计算或整矩阵完成。[S2:83–117; S3:50–70,134–138]
- B矩阵条带后续s还会重新加载。那是后续作业代的用途，不是本次s0 B2 job自动包含的义务；若跨代缓存或复用须单独计owner/context/有效性。本次B2计算影响C0的列32…47；完整应用还须其余C结果、第二末C等待、最终C阻塞搬运、并行区结束及六SPM释放，host随后compare(c,d)。只缩短本次wait不等于完整应用收益。[S1:114–236,238–273,379]

## 5. [a0,r0]内A/C实际能产生什么

**A**：A0和B0都已在barrier之前wait，不再因t0本地计算产生这些输入的host payload；A1已提交但没有wait，故正常残余最多包含该16 KiB搬运尚未完成的部分，可能已全部完成。是否仍发翻译、多少burst/leaf、何时排入共享资源unknown。A2不会在此窗口提交（直到s1）。不能把两个A槽都算作必须同时钉住的翻译集合。[S1:98,114–138,208–225]

**C**：s0不走C DMA分支；t0把16×16个uint32结果写入本地c_ptrs[0]，共1024B逻辑结果地址量。这里没有C host外送；不把本地SPM写等同外部IOMMU请求。c_ptrs[1]在本窗口无C计算用途。host map(from:C)属于target外围传输，未核runtime不能把它插入本窗口。[S1:143–162,217–226,369–371]

**B与合计**：B1是另一条可在途16 KiB输入。若只计本卡明确的外部source span，a0时可能剩余A1/B1最多合计32 KiB payload，实际剩余字节0…32768、实际同时性与顺序unknown。原生源码没有B2预热流；新增普通纯翻译不会提前覆盖SPM。三个PE提交A/B/C也不是三个独立DMA硬件publisher；S3显示共同MMIO DMA配置块，未核其跨核提交序列化与实际stream分配，不能从源码补成可靠逐请求并发轨迹。[S1:94–107,128,179,208; S3:27–43,58–85]

只在“同一上下文、全部4 KiB leaf、忽略其他来源、仅外部source”的共同投影下，A1页集为4/5页，B1∪B2为8/9页，总集合大小为12…14页；B1/B2不对齐时共享一页，**不是15页**。这只是可能涉及的地址并集，不是同时必须驻留的翻译数，更不能由它推出容量溢出。A0/B0已完成payload的条目允许普通对照合法回收。真实IOTLB结构/关联/替换、其他页面与上下文不在本席证据中。

## 6. 普通预热之后可裁决的范围

| 待核主张 | 本席裁决／证据 |
| --- | --- |
| 地址必须等SPM释放后才可知 | 否：入口参数给定即可算b+2S；真实提交较晚是数据owner约束，不是地址未知。S1:44–50,165–179 |
| 正常代码已在[a0,r0]预热B2 | 否：t0只提交B1；本例prepare hook不存在。S1:171–179 |
| 普通预热后B2必然被A/C驱逐 | **不成立为源码事实**：C无外部DMA，A仅A1可能剩余；页表、请求顺序、缓存结构/初态缺失。源码没有驱逐观测。S1:114–228 |
| B1会帮助预热B2 | 只有4 KiB不对齐且相同context时首leaf重叠这一几何可能；仍须时序、缓存插入与保持条件 |
| 预热后仍有leaf walk/PTE DRAM成本 | unknown：取决于命中、leaf粒度、PTE cache、静态映射/大页；本组三文件不能排除成本为0 |
| B2的慢翻译一定延长完整矩阵时间 | unknown：没有native trace/周期/关键路径；B2在t1搬运可被B1计算掩蔽。S1:165–234 |
| 没有源码强制驱逐就证明B1无价值 | 否：证据不足仅限制该正例声明；不能外推全族或完整集成成本 |

若允许普通预热且条目/有效结果保持到消费，或者真实大页/PTE cache把剩余工作化解，则本源码不留下一个**已证**必须依赖额外一次性结果池才能完成的需求。反过来，如要确立该单条带正例，最少须给同一合法source实例的：目标侧B2地址和映射leaf/context、prewarm完成与消费间真实或明确source-bound请求/缓存服务合同、有效结果丢失原因、B2重走与完整结果之间的未被计算掩蔽依赖。它们目前缺失，不自动启动采集、模型、实验或硬件。

## 7. 核验与限制

已核：S1/S2/S3 hash与先前清单一致；默认参数算术；全部4096种4 KiB页内偏移下4/5页、B1/B2重叠与8/9页并集。核验是源码/算术检查，不是编译、运行源程序、功能模型或性能试验。

未核：upstream commit/快照完整性之外的原仓库身份、编译器生成代码、运行时调度/跨核MMIO提交原子性、真实内存映射/大页/权限、实际IOMMU SID/SSID、burst与cache替换、MMIO与计算时间、有效译后结果实际存储/端口、错误失效/取消。源码满足正常地址/依赖卡的边界；它单独不足以接受“普通预热后仍有关键残余”或“无免费结果表所以新接口收益成立”。

执行账：0联网、0生产文件编辑、0构建、0旧实验重跑、0 native/设备/硬件；只写本独占目录三份交付物。没有读取另一席答案。AI辅助核查由Codex执行，模型/强度独立元数据unknown。
