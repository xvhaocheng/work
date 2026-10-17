# E6-PN：统一原版上的 P/N 最小移植计划

## Material Passport

- Origin Skill/Mode：ARS-Codex `experiment-agent / plan`；2026-10-18 JST。
- Verification Status：**SOURCE-REVIEWED / DESIGN-ONLY**。本批只有定向读源、固定来源核对和方案；生产改动、仿真、综合、BIT、Linux 构建、板点均为 0。
- 用户指定 Codex-only、Sol/ultra；当前工具没有可独立核验的实际模型或原生上下文元数据，不声称已核验，不修改缓存或窗口。
- 新独占顶层 `/home/xvhaocheng/work-e6-pn`，分支 `ispass/e6-pn`，起点 `main@7123bbfe79cca298c36a880206ab85a2d68e6f67`。四个组件独立工作树及完整来源见 [source-map.md](source-map.md)。Root 独占排板；本席不访问硬件，不干预既有构建。
- 本计划响应本次明确授权；不恢复历史机制探索、停止审查、G、E5 或 E3 扩展，也不重跑已完成 DS。

## 1. 目标与目前需要裁决的依赖

只刻画固定 2/14 项容量分区 P、分区局部 PLRU touch，以及已有 composition N 的有限命中旁路在统一基线上的单独与组合行为。运行模式 `mode[1:0]` 为 `0=S, 1=N, 2=P, 3=PN`；P 保护 DID10/V 的 ways0–1，DID11/A 使用 ways2–15。总 IOTLB 仍为 16 项，不改替换算法为 LRU/invalid-first，不加入 DDR/DS 优先、读门控或新调度机制。

**直接把旧 composition 树关闭 P/N 不能满足原版 S 合同。** 实际 donor 把 DMA0/1 的数据缓冲从 3 beat 改为 512 beat、单控制器改双 bank，并将 MSI 配置从 `MSI_BT_MRIF` 改为 `MSI_BT_ONLY`；`NonblockEnable=1` 时，即使运行模式 S 也改变 normal request gate。旧 `production.patch` 只有 P2 的两文件增量，不能单独应用到 85b 原版得到 P/N。实际行号见 source-map §3–5。

首批结论不是“已能按原样移植”。Root已锁定同BIT双支路首选、最小cooldown与N真实accept计数属于本次必要接口；以下区分该决定与仍需审的实现边界：

| 决策 | 最小守约边界 | 本批处理 |
| --- | --- | --- |
| D1：S 与历史 N 的结构依赖 | S/P原 controller+backend3；N/PN历史banked+backend512/真证书；全quiet切换 | Root选定首选；下面给实际插点、资源和MRIF缺口，未经实际diff审不构建 |
| D2：非零 V gap | BTC1 无 gap；新增独立最小 batch cooldown，gap0逐拍恢复原路径 | 用户测量要求已授权，纳入§4接口；不移植旧P4M/G，不用软件循环替代 |
| D3：A 独立硬件时间与共同总吞吐 | OBS3只捕获V窗；新增约256counter FF+flags的被动A own/union捕获 | 用户已明确批准，纳入197–199；不带事件FIFO/旧诊断observer，仍先验具体有限口径 |
| D4：N 真正触发数 | 原 HPM/OBS3 无 N accept 计数 | 用户明确采集项，允许两DID被动饱和计数；准确接口仍先审后写 |

### D1 的推荐实现方向与拒绝项

P先在原controller/TL上引入masked victim与resident-way局部touch；S/N使用full mask。Root选定的同BIT方向如下：

```mermaid
flowchart LR
 F[原寄存器前端与job FIFO] --> M{quiet锁定mode.N}
 M -->|0| B3[原backend3]
 M -->|1| B512[附加backend512与真证书]
 B3 --> X[原DMA汇聚AXI]
 B512 --> X
 X --> C{同一mode.N}
 C -->|0 S/P| S[原single controller]
 C -->|1 N/PN| N[历史banked controller]
 S --> T[共享原TL/缓存/walkers]
 N --> T
```

具体插点为8aac `dma_core_wrap.sv:160–176`原job FIFO的输出`burst_req/be_valid/be_ready`，及273–319的backend request/response/AXI接口。原 frontend、job FIFO、idgen、计数与backend3实例保持；只给DMA0/1额外backend512实例。req valid只给选中backend，ready只返自选中backend；`idma_rsp/valid/ready`、busy、special ttype/rdata和完整AXI五通道一并选择。未选支路输入valid和返回valid为0，不接收虚假B/R；未选输出不得发事务。全路径切换前两backend都empty/idle、共享job FIFO empty、frontend无offer、证书无pending，故没有残留响应需跨支路复用。

IOMMU top同理保留原single controller，另加历史banked controller；共享TL/cache/walker，不复制整IOMMU。mode.N=0时原controller参数、AW/AR/W FIFO几何、MRIF handler和输出响应直达路径保留；banked不接收VALID。mode.N=1才转banked。TL新增normal gate只在runtime N=1生效，N=0原请求arbiter/spill ready和slow response原路径逐拍恢复，不把无输入RR arbiter“看起来没多拍”当证明。P关闭时mask全1/local touch inactive。

**不使用模式相关reset、异步reset脉冲、运行时flush内部DMA FIFO或新增clock/约束。** 两支路只有共同原reset；切换仅在真实全quiet状态完成，FIFO本来已空。有限验证还需检查未选支路静默、后续重新选中不受旧state/id影响。组合mux不会主动增加流水级，但额外扇出/放置可能改变时序；S15是板级必要门。

### MSI/MRIF：具体不兼容与受限回退

全局 `ariane_soc::RVIOMMUCfg.MSITrans`、IOMMU能力CSR、共享TL/MSI walker/cache与S/P原handler保持 `MSI_BT_MRIF`，绝不复制旧全局`nonblock_cfg()->BT_ONLY`。但历史banked本身在143行拒MRIF，348/355未接各bank MRIF输出，总MRIF req/fault在360–363绑0。原trans_ctl收到`is_mrif`后653行进入MRIF，661–663等待handler ready；**只删fatal或把总口绑0会挂，不能这样移植**。

本次首选是明确受限实验域：N/PN只允许固定DID10/11、静态Sv39 S1/Bare S2、normal aligned4KiB COPY，无ATS/PRI/debug/特殊copy，其他DMA闲置。**允许原kernel实际启用的flat MSI context**，逐实际payload IOVA及S1叶子GPA核对不会命中MSI mask/pattern；不能要求`DC.msiptp==0`，否则会拒掉原kernel正常配置。45bf `iommu.c:953–978,1313–1316`在CAP_MSI_FLAT时给endpoint启用FLAT，mask255/pattern IMSIC；历史N helper62–99本就有input/leaf MSI-safe检查。不是MSI context开启就判成MRIF任务。

实际MSI范围命中的payload、MRIF/非ordinary任务或不能证明静态映射排除的配置，在触发前应明确EOPNOTSUPP并保持quiet，由软件显式切S/P后走原路径。这里的“回退”是**任务发出前全quiet选原支路**，不是发现一个已被banked接受的请求后搬去single controller。N fast probe资格拒绝只保证走normal TL，不自动解决normal TL的MRIF response去向；warmup、miss/leaf变化同样不能绕过受限域核对。

Root已原则接受此受限域，但**实际拒绝如何执行尚未锁定**：当前原kernel允许devmem，不能把未来prepare检查写成已经存在、不可绕过的保护。历史banked elaboration对MRIF fatal的处理、实际非法输入拒绝位置和可达错误/排空行为须给Root单独具体小合同，不能只删fatal/接0。不得将bank-local较窄实现伪称全功能MRIF支持，也不因此新增完整MRIF仲裁。若要求N模式内任意合法MSI/MRIF也透明可运行，那是另一个更大实现；本批不做。发生不符合合同的实际输入不能靠屏蔽error放行，后续有限负测必须检查真实拒绝/错误且保留证据。

最小实际拒绝候选（后续N小批，当前不实现）：保留full-MRIF全局cfg与S/P分支，在共享TL的**runtime N=1且来自受限bank请求**进入MSI状态时，将该非ordinary请求转入原fault/error路径，设置`error=1, is_mrif=0, ignore=0, fault_code=TRANS_TYPE_DISALLOWED`，保留原请求DID/IOVA/type/priv字段与DTF规则；DTF=0按原ERROR态等待真实fault_ready后给error response，DTF=1仍须明确error response而不伪成功。给该受限状态门控`msi_ptw_init`，避免同拍又启动无用MSI walk。正常flat context且输入/leaf不匹配MSI范围的payload从不走此拒绝支路。

这样banked仍通过原AXI error slave消费/回应已接受事务，不进入未接handler的MRIF状态；不是把MRIF翻译结果改成普通物理访问。不得仅在bank输出翻error却漏FQ语义。旧banked的MRIF fatal须替换为这一明确受限接口使能/连接检查，同时保留原single controller完整MRIF；不会增加MRIF DS仲裁。候选需Root再核实际状态赋值优先级/DTF/heldfault与error slave排空，再允许实现；不能先声称这已解决任意devmem非法操作。有限负测必须走真实输入MSI范围与叶GPA MSI范围、DTF0/1、FQ反压、R/B反压，得到一次对应error/fault且不挂、不发payload成功；N helper本身hit的MSI排除也保留。iDMA生产backend仍为NO_ERROR_HANDLING，故错误响应只说明IOMMU/AXI拒绝边界，不能承诺新软件无损恢复；这类运行invalid并按既有失败/STOP/drain内存保留规则处理。

### 资源估算（功能容量，下界，不是综合结果）

同BIT保留原backend3后，再加两套backend512，**resident新增数据容量8192B**；活动N相对活动S的两lane数据容量差为8144B，两者不可混说。每新增backend的`MetaFifoDepth=512+8+0=520`，仅2bit last-flag FIFO就各1040bit，合2080bit/260B；另有重复legalizer/FSM、8深read/write metadata FIFO、通道寄存器、两个job证书及mux。新增banked仍合计8 AW/8 AR header与32 W beat（至少256B数据+32B strobes，完整header/控制位另计），原single controller存储同时留存；不重复cache/walkers。两32bit N计数、约256counterFF+flags的A/union时长、control/status/cooldown也单列。

512×8bit byte-lane存储可定向复用历史`f443...`的FPGA RAM选择，原小FIFO仍用原实现；这涉及`idma_stream_fifo.sv`与vendor `stream_fifo.sv/fifo_v3.sv`的参数默认不变补丁，不能遗漏成“只有改BufferDepth”。实际BRAM/LUT/FF/Slice未知，须综合原报告。旧composition已用Slice50556/50950，双支路是否装得下存在实质风险；不凭功能bit下界声称fit，不为fit更改约束/关闭验证，失败按硬门停。

不接受的捷径：全局启用旧 banked+512 再把 S15 通过当作“原路径不变”的证明；无证书放行小 buffer 非 owner 读；复用旧 composition 整体 observer；给所有模式全局改 MSI；为使结果好看加入 DS-first、PLRU invalid-first、旧 G pacing。若 Root 选择明确放宽结构合同，应另记其授权与资源费用，不能在当前合同内默改。S15 是必要板级准入，不能替代结构/功能核对。

## 2. 固定基座、保持项与容量合同

| 组件 | 唯一基座 |
| --- | --- |
| platform | `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273` |
| CVA6 | `8aac3fcd345af7686a55af1867783fd6b91a0069` |
| IOMMU | `85b157ebe71a623e326e51fff7b7126fe77e8eb9` |
| Linux | `45bf76b15e36cf608dff95013d91f8bedb7b1b8e` |

platform gitlink/lock 和 CVA6 的 IOMMU gitlink已核一致。四个新组件工作树均以该精确提交建立同名 `ispass/e6-pn` 分支，不从活动研究 HEAD 或旧 G/E5 树接续。只读旧树帮助定位，实际移植须新提交、子指针与 lock 相符后再构建。

保持：50 MHz / 原 XDC / DS0 直连；DS50 后续只改原 delay 参数为 50/50；四 DMA ID/DID/地址、64-bit AXI、既有适配/SC/AW owner/DS owner/HPM/PTW leaf/边界修复保留。不得升级 vendor 或改 burst 几何。原版配置 `NumOutstandingTrans=8, WFifoDepth=32, IOTLB=16, DDTC=8`；历史 banked 是每 bank4/16，总容量不翻倍，外加 input demux tracker16、lifecycle/arbiter/证书/缓冲成本另列，不能把 tracker16说成16笔并发翻译。

原 BTC1 4KiB前端的gap0路径/旧寄存器和OBS3/PATH depth4保留，只新增下述必要cooldown，不合P0/BTC2、BTC3、OBS4/5、histogram、G或long-descriptor。所有已有overflow/error/stockflow检查保留；在N下若有限真实DUT发现depth4不足或持有关系不兼容，停止报告，不自行放大或屏蔽。

## 3. P 与 N 的最小语义

P 的 fill mask 由**稳定主 PTW/MSI 请求** DID 决定，不能取当前 fast probe DID。局部 history 的 touch 归属由命中的 resident way 决定；只有该分区两 child 均 eligible 的树节点才更新。S/N full mask 的 victim/touch 必须与原版逐项相同。保留所有 tag、PSCID/GSCID、权限和 invalidation 语义；2/14 是替换/分配政策，不宣称安全隔离或 14 项 LRU。历史 A8 在 14 项 quota 内仍可能反复 walk，原负结果保留。

N 原样限定两 lifecycle tag，每 bank 一笔翻译、一个 normal walker。窗口是**另一 bank 的 S1 PTW 活跃**、S2 Bare；只接受真实 DDTC+IOTLB 命中、Sv39 4KiB、canonical IOVA、普通 untranslated R/W、合法 DC/权限及 MSI 范围排除。ATS/PRI/PID/priv/debug、大页、miss、变化的候选/上下文/invalidation 都不被 fast path 接受，走原合法慢/故障路径。请求探测不 touch；真实 `accept_o` 才一次 touch 两 cache。response 反压时保持完整 payload/tag 直到握手，不因模式切换、flush 或软件 STOP 取消。

完整buffer证书只来自真实**frontend→job FIFO** handshake（wrap中的`be_valid_d && be_ready_d`）与selected backend retire（`idma_rsp_valid && idma_rsp_ready`），不是后端reqfire或Linux flag；normal aligned4096、容量>=4096B、一个未退役job。Root已直接核原件；新双backend接线必须保留accepted-job/retire序列匹配、同拍replace口径及异常重叠许可粘性撤销直到reset。不得由Linux传一个“已缓存”标志替代，也不能把假定页对齐当证书。

## 4. 模式接口、切换、flush、计数与最小cooldown（具体布局待 Root 锁定）

建议复用原DMA0 observer `0x70 index / 0x78 data`，独立8-word PN bank **index192..199**；原OBS3 magic/0..34、PATH64..191、BTC1地址及行为不变，不拷D20的128-word entry snapshots或generation。新bank有自己的capability/version，不能让旧Linux把额外内容当已支持的原OBS3字段。

| index | 提议语义 |
| --- | --- |
| 192 | 独立 PN capability/version（建议 magic `0x49444d41504e0001`；尚未锁定） |
| 193 | 实际 MODE，RW低2bit，其余0；0S/1N/2P/3PN |
| 194 | mode quiet/busy、最近一次非法/忙拒绝、计数 overflow；精确 bit 分配随下批控制小合同审定 |
| 195 | V/DID10 真正 N accept，原 V 窗，饱和32bit |
| 196 | A/DID11 真正 N accept，同窗，饱和32bit |
| 197 | A own hardware cycles，RO64；同次ARM后首A issue至最后A done，STOP/drain/quiet后冻结 |
| 198 | union hardware cycles，RO64；同次ARM后任一路首issue至任一路最后done，双路排空后冻结 |
| 199 | A/union捕获valid/冻结/overflow等flags（具体bit由小批接口锁定）；不是身份序号 |

MODE经DATA写只允许全 `WSTRB=0xff`、值0..3。本轮沿原kernel、由Root串行所有权执行；外部helper不是内核私有experiment mutex参与者，不能虚构它持锁。**硬件真实全quiet拒写**是必要条件：所有DMA frontend/job/backend quiet、无offered/pending AXI/W owner/响应、两controller/TL/PTW/cache update/invalidation quiet，OBS未ARM/active、A/union tracker已冻结/无live任务才接受。接受边沿**同时**更新mode、invalid IOTLB tags和清PLRU（同mode写也清），无旧translation/response跨模式；拒绝不改变mode/cache/计数/phase。后续原prepare的映射/IOFENCE/restore/CQ流程不省略。DDTC内容不因P分区而重解释；涉及DDTC变化仍用原iodir invalidation完整完成，不用mode写冒充I/O fence。若后续证实必须增kernel控制接口，先具体提案，不预设其已存在。

Reset默认S/gap0。S15运行原CLI/原参数，除capability/mode readback外不插入模式写、额外cache clear或新workload；主矩阵模式切换后按统一固定的flush→prepare→warmup/ARM顺序，不能让P/PN单独受益于不同冷暖状态。原command、warmup、供给次序和停止/排空规则须由软件核查席冻结。

计数与身份：原 miss/PTW HPM 用原 DID筛选和采样范围；保留原日志，不把其 warmup/drain 范围偷换成 V 窗。N计数复用同次真实 OBS ARM/open/close，采用原 `(open,close]`口径；开点事件不算、终点事件计入。同拍 fast accept 依据真实被接受请求 DID 归属；未知 DID 视非法实验输入并使样本 invalid，不悄悄并入 V/A。32bit 饱和+sticky overflow 使结果 invalid。无额外 seq/generation；现有软件 seq/manifest/command 绑定不变。

### 获批被动A own/union时长

只用原issue/done、batch active、AXI stock/quiet信号，新增约4×64bit counter寄存器加flags：A age/latest-done elapsed、union age/latest-done elapsed。每次原ARM清新样本捕获状态；首A issue启动A age，任一路首issue启动union age；每次真实对应done更新latest-done elapsed，counter饱和或ID/stock非法使invalid。V target done后原OBS3/PATH正常冻结，但这两时长tracker继续旁听，直到原STOP+drain完成、双方无pending/offer/backend/AXI义务才冻结。最终时长止于最后真实done，不把软件STOP发现/读寄存器延迟或尾部quiet等待加进elapsed。

A own throughput分子使用同次ARM后实际accepted并完成的A COPY数量，必须由原batch completed/issued在quiet时核对，与startup/drain的原字段范围逐项绑定；不能拿V窗A响应数当A全程工作。union throughput采用同一union窗内两路真实完成有效bytes之和，不能相加不同分母带宽。首issue恰好与done同拍的边沿规则/无A/overflow/abort/先A后V或先V后A/回包持续/合法poll须有限真实DUT检查；未valid时相应指标N/A。

这是一项被动追加，不更改S的ready/valid/状态、N准入、gap供给或原OBS3/PATH开闭/错误门。ARM后tracker未排空时拒绝clear/reARM/mode写，不用control优先级丢真实回包；显式abort冻结invalid而继续live stock排空。199 flags精确定义随实现小合同交Root审，无新generation/身份体系、无trace FIFO、无PTW AR→R逻辑。

### 必要V batch cooldown

Root已确认用户Vgap1000/4000测量要求授权此最小接口。建议只给DMA0/V的BatchPort加独立 `0xb0 GAP_CYCLES`（低32bit RW，高32bit必须0）及 `0xb8 GAP_CAP`（RO，建议`0x49444d4147415001`）；参数默认关闭，未启用实例保留原未映射error。BTC1的0x80 magic与0x88..a8原语义不改，不复用0x48/N_TRANS，不导入P4M/G控制器/observer。模式cap与gap cap分开；软件同时核cap/readback和manifest，不用名称推断。

gap写仅frontend IDLE、无offered task/queued job/backend busy、全WSTRB且value<=UINT32_MAX接受；active/held时写拒绝并置原batch_error，旧gap值保持。reset=0；`0x88=0`的既有batch disable/reset计数操作不清gap配置（原driver configure先disable），但batchOFF warmup完全忽略gap。所用配置在启动时锁定，运行中不改。

原IDLE/COUNT/WAIT的2bit编码保持，新增剩余2bit状态作cooldown。仅当真实当前task done且后面还有工作、无STOP时进入；首发无gap、最后一次done后不等gap。gap0直接执行原WAIT→COUNT，所有输出/计数/ID按原周期。gap=g>0时done边沿装载g−1，恰好g个cooldown周期后回COUNT；为避免32bit端点溢出，不计算g+1放入32bit计数器。相邻正常task原本有1个COUNT供给周期，因此无反压/停止时observer REFILL总和应为 `(N−1)*(1+g)`，刻意新增等待才是`(N−1)*g`；不能把全部REFILL称为新gap。

cooldown阶段从未offer下一task，STOP优先于到期：结束批次但不再issue、不伪造第二次done，issued/completed保持相等；已进入COUNT且VALID被反压时STOP不得撤回该offer，按原协议接受并完成后停。timeout仍走既有STOP/drain/保留内存的错误路径，不取消已承诺AXI。配置拒绝不影响已有任务继续；合法poll不abort观察。

有限frontend实际DUT须与8aac gold同输入逐拍比较gap0，覆盖普通/batch、最后done、STOP/到期同拍、heldVALID、active配置拒绝、disable保留gap与batchOFF忽略、gap1/1000/4000和UINT32_MAX装载/递减端点（不循环仿真2^32拍，原counter下一态边界独立有限检查）。外加真实backend+IOMMU有界copy回归，核完整字节/AXI/排空。新增gap不改变N准入、P触碰、观察开闭或原S15工作量。

## 5. 可观测量与缺项的必要性

| 要求 | 现有来源/口径 | 本次边界 |
| --- | --- | --- |
| V硬件时间/吞吐 | OBS3 V首issue→第4096 done；有效 COPY bytes=4096×4096；50MHz | 精确窗口时间；读/写/有效copy带宽分别标单位，不把双向字节重复作copy带宽 |
| A V窗内速度 | 原窗内 A B/RLAST、开闭 stock；4KiB真实backend每方向2burst | 可报每2048B响应proxy及有限边界，不当作精确A完整copy计数；不能用含startup/drain的completed_bytes除V窗 |
| A own硬件elapsed/吞吐 | 原A target=2^32、V结束STOP/drain；新197 actual firstAissue→lastAdone | 新增被动捕获已获批；valid后配绑定的实际A全程完成bytes；旧active_cycles/host ktime仍不能代替 |
| 合计吞吐 | 同V窗的V+A响应代理保留；新198 union窗与实际两lane完成bytes | 分别列V窗proxy与union精确有效copy吞吐，分母不可混合；不加各自不同分母throughput |
| miss/PTW | 原DID HPM与日志 | 整条命令范围保留，不等同 walk AR→R latency |
| N实际触发 | 新195/196，仅真实 `n_accept` | **必要最小新增**：baseline没有该量；probe hit不能代替。只两计数/overflow，不带cache内容快照 |
| V PATH /入口AW等待 | 原PATH8字段/段 + count19(V AWwait)/count29(shared AWwait) | AWwait是入口VALID&&!READY，事件重叠，不相加归因；新banking须有限验证实际ID/return仍配对 |
| PTW AR→R累计 | 原OBS3没有请求配对计时 | 需要新逻辑，按授权**跳过**，不为该量增诊断硬件 |

用户已批准这项最小被动A/union捕获及必要cooldown；本批只是具体设计，尚未实现/验证。实际有限raw回执通过前不得把计划口径写成已有有效板测字段。

## 6. 分批文件清单与接受检查

| 批次 | 预计文件（均在独立组件） | 放行证据 |
| --- | --- | --- |
| A：本批 | 只本目录 `port-plan.md / source-map.md / source-check01.json` | 基座gitlink/lock一致；七份实际donor bytes与固定P2提交相同；原文/RTL行号齐全；四组件clean |
| B：P最小移植 | IOMMU `rtl/translation_logic/rv_iommu_iotlb.sv`, `rv_iommu_tl_wrap.sv`，必要top端口 | 原版full-mask touch/victim逐拍gold；固定2/14fill与resident-way局部touch；invalidate/clear同拍与owner互换；不复制research快照 |
| C：N依赖与接线（双支路/受限域原则已选，实际拒绝位置待锁） | IOMMU `rv_iommu_top.sv`, `transaction_controller/rv_iommu_banked_ctl.sv`, `rv_iommu_trans_ctl_wrap.sv`, `translation_logic/rv_iommu_nonblock_hit.sv`, `rv_iommu_tl_wrap.sv`, `rv_iommu_ddtc.sv`, 构建源清单；CVA6 `ariane_peripherals_xilinx.sv`、DMA wrap/证书/`idma_stream_fifo.sv`及定向vendor RAM参数支持 | 真正Naccept/slowfallback/heldresponse/cachetouch一次；ID bank/tag/owner；小buffer禁止非法豁免；真实backend完整copy字节/AR/AW/R/B排空与错误；S完整数据路径gold |
| D：最小控制/计数 | 新小PN控制块+wrapper，`dma_observer.sv`的附加decode/同窗tap与约256counterFF+flags的A/union被动捕获；不搬旧composition observer | busy写拒绝、同拍offer拒绝、合法mode+clear原子、flush无旧response；accept计数开闭边沿/饱和/未知DID；A/union首末done/abort/STOP/drain冻结；原OBS3/PATH gold，S原命令接口不变 |
| D2：最小V cooldown | `idma_reg64_frontend.sv`、wrap参数/只读quiet端口、peripherals V实例使能 | §4所列真实frontend逐拍gold/边界及有界真实backend接线；不重复G39项既有测试冒作新base测试 |
| E：软件与collector（A3专席） | Linux45bf observer/实验调用局部、任务内新CLI/collector/helper（保持旧命令）；不移植旧composition driver整体 | 固定cap/readback、互斥/quiet/restore/taint、原始命令/输出重绑、真实样本部分导出；旧CLI仍运行原路径；新增gap/own-time必须另批审 |
| F：锁定与构建 | component commits → CVA6 IOMMU gitlink → platform lock/gitlink；任务内build/package清单 | 保存完整SHA、选中原字节/patch、编译参数/约束；产物本机保留且不入Git，输出只在独立工作树artifacts |

不整包 cherry-pick donor feature stack：85b→P2差异含E3 queue/delay、research package/observe等12文件1483插入62删除；提取语义小块，所有现有必要修复留在85b。有限验证只测新增有界合同，设明确进程预算/超时，保留失败，不开长仿真或同源重复扫描。全部生产diff和实际raw回执先交Root审，再批准下一批。上表是设计文件面，不代表已获RTL实现或完整BIT启动许可。

### 有限实际DUT的S等价接受

不以模块名或同名参数代替证据。gold固定8aac+85b原字节、candidate为同一有限harness下的新完整连接（真实register frontend/job FIFO/backend3/AXI mux/single controller/TL/cache/PTW/DS接口），用相同合法MMIO与可重放共享DDR/R/B反压输入，对比每周期：task offer/id/done、五通道VALID/READY及被offer payload、translation request/response、PTW/DS请求、cache fill/touch/victim/flush、原OBS3与PATH读取/错误、final memory及drain。比较S时P关闭/N关闭/gap0；P fullmask与N0旁路还要涵盖热hit、冷miss、owner WLAST边界、held R/B/AR/AW及reset后首次任务。

有限矩阵含Solo与A1/A8/A64短完整copy序列（含足够环回）、4KiB/2MiB leaf与原MRIF非ordinary路径gold（若现有有限harness能建合法请求则测真实handler，不以synthetic completion冒充）。首批不要求跑4096长仿真；所有比较cycle和input序列、源码hash、编译命令、原始log/CSV保存。S在N运行后全quiet切回、重暖/flush后的有限流也需与匹配初始状态gold比，inactive支路始终no VALID/无未清stock。N/PN真实backend另核hit/miss/permission/invalidation与完整copy，不把S等价覆盖误报N功能覆盖。

任一S周期差先定位，不能通过允许±cycle误差绕过；实际硬件S15仍独立必须通过。面积/时序不能从仿真等价推出，综合前由Root审实际diff及有限回执。

## 7. 构建与板级硬门

1. **DS0先构建**：仍50MHz、原约束。最终 WNS>=0；setup/hold/pulse failing endpoints均0、routing errors0且fully routed。保留原routed DCP、timing/check_timing/route_status及manifest。
2. 只允许一次历史post-route流程：原routed DCP执行一次 `phys_opt_design -directive Explore`，若不完全路由则至多一次 `route_design -directive Explore`。源码/频率/约束不变，不增false-path/multicycle、不降DRC等级。仍失败立即停止报告；不改约束、不再试策略、不排板。历史+0.022ns只是来源，不保证新构建能过。
3. Root在DS0模式S跑 **Solo/A1/A8各5次，共15**，沿统一DS0原命令/4096次工作量/原供给、真实BIT/kernel/boot身份；各cell以原统一DS0真实数据计算固定hardware median参照。验收 `abs(median_new/median_unified_DS0 - 1) <= 0.005`，每cell独立判定，不跨cell平均。失败即停止，不靠排除点、改变warmup或扩大n掩盖。任何内置错误/数据错误/restore/taint/overflow亦失败。
4. **S15实际板测通过后才允许同源DS50构建**；不能先并行构DS50，也不能用RTL仿真替代S15。DS50只改原delay50/50，完整重复同一时序硬门（最多一次既定post-route流程）。
5. 本席无硬件操作；Root统一安排同Linux/BIT所需点集中采完。新产物在 `/home/xvhaocheng/work-e6-pn/paper/ispass2027/exp/E6-pn/artifacts/`，保留失败/历史，不写主目录out或共享board_results。

## 8. 已授权矩阵与数据发布

| 阶段 | DS | cell | n/每cell | 点数 |
| --- | --- | --- | --- | --- |
| S准入 | 0 | S × Solo/A1/A8，gap0 | 5 | 15 |
| 主矩阵 | 0 | S/N/P/PN × A1/A8/A64，gap0 | 10 | 120 |
| 供给补充（最小cooldown验收后） | 0 | P/PN × A8 × Vgap0/1000/4000 | 5 | 30 |
| 延迟敏感性（S15后） | 50 | S/N/P/PN × A8，gap0 | 10 | 40 |

**严格205个新试次，四phase独立，不复用。** Root已锁S15准入、主矩阵120、供给补充30（含独立gap0）、DS50的40；即使条件相同，也各有实际独立运行与trial ID，不用准入5点填主矩阵、不从主矩阵抽补充gap0。

模式在完整重复block内安排，点数依授权，保存实际顺序/启动/温度等已有元数据，不补点制造显著性。主比较报告V hardware time/throughput、A窗口有效速度及边界、同窗合计、miss/PTW、真实Naccept、V PATH/入口AWwait，均写明计数范围。描述性median/分位数与配对效果按真实block计算；小n不宣称显著性或精确request p99。旧PN慢于P、普通服务模型消差、合法2MiB负结果全部保留，不把新PN默认当收益机制。

逐试次数据落本任务目录，持续同步**真实已验收论文数据**，完整区组或阶段checkpoint冻结原manifest+records快照；`complete=false`、各cell实际n与计划n明确，原日志供追溯但不称为论文数据同步。未完成manifest计数/后续增长的raw不得用旧hash假装最终。采集前固定BIT/内核/代码/实际命令，复用既有manifest身份，无新generation体系。fixture不进入论文导出。

## 9. 本批结论与下一步

P/N历史来源与真实accept/容量依赖已定位；Root已选同BIT双支路、normal flat-context受限域、必要cooldown/accept计数，用户批准最小A/union被动捕获，矩阵严格205。Root指定下一实施批只做P IOTLB+TL必要mask/flush/localhistory及原gold真实DUT有限检查；N受限输入实际拒绝方案后续独立小批。不构建完整BIT直到Root接受实际diff/有限回执。当前无可上板PN产物、无新性能结果，也没有“PN已复现”或“S等价已通过”的主张。

后继小批回执：[P-only实际diff/有限检查](p-validation/README.md)，IOMMU `d2a813d67bcfd0295d8934c3cc027d24d3195d63`直接接85b；该小批不改变本计划的205板点/完整构建硬门。cooldown前端已由Root另分A2独立`ispass/e6-pn-gap`从8aac实施，A1不重复写该文件，接受后再由A1连接仅DMA0使能。
