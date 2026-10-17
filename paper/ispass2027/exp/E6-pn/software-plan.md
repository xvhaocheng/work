# E6-PN 软件与collector准备方案

## Material Passport

ARS `experiment-agent / plan`；2026-10-18 JST；**供Root审源码/接口，不是实施或执行回执**。
仅本目录三份文档；不改driver/CLI/RTL，不触UART/JTAG/SD，不编译/综合/板测。
来源记号B/C/I/L/T/D与逐字段证据见 `measurement-contract.md`、`tool-source-map.json`。
Root已明确优先保留原pathv3 kernel45bf与原f911 CLI；不预设必须换内核。

## 1. S15先保持原软件、原命令与reset S

新DS0 BIT reset应为S、V/A gap0。S15不写MODE、不额外清缓存，不预跑测量点：
Solo/A1/A8各5次硬件时间，仍为V0、A1、V源/目的各1页、A源/目的各k页，均4KiB页、COPY4096×4096、软件warmup256后autonomous batch。
原`idma_experiment --batch ...`及kernel payload保持相同。原S路径与旧mode0研究路径不能等同；
硬件原路径等价由A1另审，最终时序和S15只验限定的实际行为。
Root已选D1方向：S/P保留原controller与原backend BufferDepth3；N/PN用隔离的历史banked＋full4KiB backend/cert支路，
同BIT全quiet切换并flush，MSI保留原MRIF能力。不能把S接到“关闭N的banked支路”后再称原路径。

**N支路准入要核实原FLAT context，不能假定MSI已禁用。** A1原提案要求N正常4KiB且MSI translation disabled；
但原L `iommu.c:1313–1316`的probe会调用enable_ir，
只要CAP_MSI_FLAT便在`:953–978`给每endpoint设置DC.msiptp=FLAT。C `ariane_soc_pkg.sv:123`
为MSI_BT_MRIF，I `regmap/rv_iommu_regmap.sv:465–467`广告MSI_FLAT。因此不能假定原DC的MSI关闭，
或把“本次不发MSI payload”直接当作“DC禁用MSI”。A1转达Root已接受按normal FLAT context＋实际输入/leaf排除来限定N，
不为了N更改全局MSI广告能力。具体合同需A1冻结可核readback/preflight，或对N bank的MRIF访问提供明确error/fault，
不能删fatal就宣称MRIF已支持，不能透明回退S/P。若不能证明正常本批输入资格则发DMA前拒绝并保留失败，
不能写出虚假N/PN结果；若确需DC变更，必须经内核管理的最小配置/restore方案另审，不能直接/devmem改OS拥有的DC。
当前为源码风险与已选设计边界，不是N支路实现通过或原包四模式已上板的证据。

核心命令（完整wrapper按source map中的原command保留）：

```text
platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor none --aggressor-pages 1 --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor 1 --aggressor-pages 1 --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor 1 --aggressor-pages 8 --victim-pages 1 --chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000
```

参考来自原baseline100的DS0单boot、每格n10，不用旧G1的近似56.5/107/155ms或DS50值：

| S15条件 | baseline100 DS0硬件median ms | 原表稳定row_id尾部 | 允许新n5 median相对偏差 |
| --- | ---: | --- | --- |
| Solo | 56.53868 | `stratum-ds0-middle-69005cc2-Solo.hardware_ms.median` | ≤0.5% |
| A1 | 107.09009 | `stratum-ds0-middle-69005cc2-A1.hardware_ms.median` | ≤0.5% |
| A8 | 118.22441 | `stratum-ds0-middle-69005cc2-A8.hardware_ms.median` | ≤0.5% |

完整row_id前缀为`board_baseline100.baseline100-20261017-01.`。
参考入口：`data/baseline100-summary.json /conditions/{0,1,2}/metrics/hardware_ms/value`；
原记录为`board/runs/baseline100-20261017-01/ds0-middle/records.json`，实际boot
`a816f6c1-b928-4801-98ea-9a55296159a7`，50点中只选这三条件各10点。
稳定row_id中的历史`69005cc2`只是既有标签，不能解析成实际boot；source map逐条保留实际boot/seq/raw命令。
门槛按`abs(median(new5)/reference10_median−1)≤0.005`逐条件检查；是跨boot的功能/性能等价准入规则，
不是统计配对或跨boot总体等价证明。所有15点与失败原样保留，未过不删点补好看数；DS50仅三格都通过后放行同源构建。
S15与后续正式S点分别标phase，不把5+10混成n15，也不拿前者顶替正式矩阵。

## 2. 原软件可承载的独立control helper：待实现API

原UAPI仅0x06 CAPS、0x07软件refill、0x08 HW batch（L `idma.h:77–85`、`idma.c:1058–1068`），
没有模式或gap输入。原CLI T `bench/idma_experiment.c:63–66,86–88`也没有这些参数。
正式优先新增**独立控制小工具**，保留kernel和原测量CLI：

```text
pn-control capabilities
pn-control read                    # cap/mode/status/N frozen counters；不写MODE
pn-control set-mode S|N|P|PN       # quiet才改；同mode写也清IOTLB/PLRU
pn-control set-v-gap 0|1000|4000  # 只DMA0前端，quiet才改
```

这些是API草案，不是现存可运行命令。只映射DMA0的MMIO页`0x50000000`；
T `configs/platform.dts:141–152`给出V/A地址`0x50000000/0x50001000`。
实际software01 `.config:2265,4982`为`CONFIG_DEVMEM=y`、STRICT_DEVMEM关闭；
L `drivers/char/mem.c:82–90,381–395`有物理映射路径，因而不能断言原kernel绝对无法做此控制。
后续helper应`/dev/mem`+O_SYNC映射、8字节对齐的RV64 volatile64 load/store、I/O fence及readback，
不能用可能拆分字节写的通用文件写来冒充全WSTRB；主机编译/实际汇编检查与板端access失败均需如实记录。
当前只核源码与既有config，尚未在板上验证该新bank访问或helper二进制。

**保护来自两层真实合同，不伪称有kernel原子锁：**

1. Root独占已存在的board锁，唯一串行调用控制/原CLI；上一CLI已退出且status/data/drain检查通过，
   没有并发collector、raw/devmem writer或其他DMA producer。此为执行所有权约束，不是内核mutex证明。
2. 新MODE写的**硬件**前置条件须覆盖全部DMA前端/后端、所有已接受AXI债务、TL/PTW、pending response/heldVALID，
   且observer未ARM/active；同沿新VALID亦拒。busy拒写且旧mode/cache/计数保持，readback确认。
   只有通过的原子切换才清IOTLB与相关PLRU（A1设计），不能丢在途响应。

这可以在Root唯一串行执行和硬件全quiet拒写成立时安全复用原kernel。index/data是两次MMIO，
本身没有跨进程原子性；只在CLI退出后读控制bank，下一测量开始前关闭helper。不能在driver读PATH时穿插改index。
如果实际系统存在无法排除的另一个writer/producer，或硬件quiet不能完整覆盖，才提交最小独立control ioctl：
与`idma_experiment_lock`共锁读写、全quarantine拒绝、独立CAP/version与零reserved、不改变旧0x08路径。
当前不占新ioctl号，不先决定升级内核，不用/devmem绕开真实busy拒绝。

## 3. 与A1对齐的最小PN bank（待Root锁定）

复用DMA0原`INDEX +0x70` / `DATA +0x78`，原OBS3 `0..34`和PATH `64..191`不动。
原C observer `dma_observer.sv:95–99,243–245`只允许上述index，因此下面是**新接口**，不是可用的旧寄存器。

| index | 读/写草案 | 语义 |
| --- | --- | --- |
| 192 | 只读CAP/version | PN独立8word能力、支持的mode/gap/可选生命周期捕获；magic值待Root锁定，不能用OBS3冒充PN能力。 |
| 193 | MODE读/写 | 0=S、1=N、2=P、3=PN；只全8字节写、value0..3、全quiet，read为实际模式。 |
| 194 | 只读状态 | 实际busy、拒写、overflow/invalid；位号和clear语义需Root/A1冻结。 |
| 195/196 | 只读V/A N accept | 同原`W_V` gate，ARM清、V close冻，按DID10/11记真实`n_accept`；饱和且overflow使该样本无效。 |
| 197/198/199 | 用户已批准的只读被动时长 | A own cycles / union cycles / valid及overflow；不携带旧composition generation/diagnostic bank，详细边沿合同见下文。 |

实际N事件来源是D `rv_iommu_nonblock_hit.sv:100–107`与TL `rv_iommu_tl_wrap.sv:287,303–306`。
S/P应为0（若能力存在且计数有效），N/PN可能因该点没有合法命中而为0；不能强制每点>0或用probe hit代替。
所有结果保存实际readback，不新增硬件身份序号；继续复用旧observer seq/boot/原manifest身份。

## 4. V gap需要最小前端cooldown，不能用旧研究G/P4M或CPU循环

C `idma_reg64_frontend.sv:124–143,279–287`只有batch `0x80/88/90/98/a0/a8`，没有gap；
`0x48 N_TRANS`是旧转数（reg_pkg`:119`、frontend`:321`），不是gap。
该前端在WAIT任务done后直接回COUNT（`:350–362`），gap0时OBS3每4096任务应为4095个自然refill周期。

用户已明确要求gap0/1000/4000，因此准备必要最小cooldown设计，**不另设普通授权门**：

- 与A1对齐的`DMA0 +0xb0`为idle-only32bit V_GAP，上32bit为0，`+0xb8`为独立gap CAP；cap常数/位定义仍待Root锁定。
  baseline对此地址返回error，不宣称旧硬件可运行。A始终gap0。
- reset gap=0，g=0保留原WAIT→COUNT的逐周期控制；不经过新倒计时态，不改变burst/队列/工作量。
- g>0仅在当前copy真正done之后、下一copy首次offer之前延迟额外g个周期；首发和最后done后不插gap，
  不撤回或延长已held VALID。定义刻意添加`(N−1)g`，原自然间隙仍在；预期总frontend gap为`(N−1)(1+g)`，
  实际有限波形应验证边沿，不能先把公式当板测事实。不能用`W_V−intentional_gap`称独立服务时延。
- gap配置必须在旧`idma_batch_configure`写`0x88=0`后仍保留（L `idma.c:528–531`）；
  软件warmup时batchOFF完全忽略gap，下一次mode/point前Root按计划显式设置，防止g4000泄漏到g0。
- STOP/timeout在cooldown要有界退出，不多发任务、不重复done，保留issued==completed、quiet与quarantine语义。
  不能只加正常路径而让错误清理挂住。使用既有batch魔数不会变更原CLI，但PN CAP必须明确gap扩展。

## 5. 用户已批准A own/共同完整吞吐最小被动捕获

Root已转达用户选择“允许最小被动时长计数（推荐）”，不再保留为未决功能。
原严格OBS3选项只有同`W_V`的A B响应×2048代理和合法AXI±4KiB界，不能给A own或双方whole-work合计。
这仍是新捕获未实现/无效时的原字段边界；不得静默拿proxy填入新时长/完整吞吐字段。

**本批批准的被动A生命周期捕获：** 不移植旧诊断observer，只复用C现有
`cfg ARM/roles/hasA`、`obs_issue/done`、`batch_active`、backend/private quiet信号。
L `idma.c:936–949`先排空W256再ARM，`:953–957`取快照后才`:590–597`启动实际batch A/V，
因此可以明确排除warmup并捕获A首个硬件task issue。前端事件为C frontend`:162–171`的每copy issue/done，而非CPU NEXT_ID返回。

一个有界候选用两64bit运行elapsed counter（首A issue、首任一路issue开始）及两64bit最近done的elapsed latch
（A done、任一路done更新），约256个计数FF，加少量seen/running/frozen/overflow flags；
无事件FIFO/BRAM、无数据通路控制。两duration按严格`last_done_edge−first_issue_edge`定义，与原V窗开闭边界一致。
共同窗口用真实任一路首issue和最后done，不能假定A最终一定晚于V或取独立时长最大值。
看到V目标done后仍持续捕获A，等STOP后A前端/后端/私有债务全部quiet才冻结，驱动已drain、CLI退出后稳定读取。
若timeout/overflow/未见两lane必要边沿/无quiet则invalid；不以软件poll时刻代替最后硬件done。

bank197=A own cycles、198=union cycles、199=valid/overflow（Solo A=N/A，union=V），正式替换此前reserved0；
最终A/V completed_bytes必须与该次ARM后测量batch一一对应。这样才计算A own速度与双方whole-work共同速度，
且仍保留原V窗口proxy。约256FF是设计估计，不是综合资源回执，不能提前声称S时序无变化。
该增量由用户明确批准；生产实现/有限波形仍由A1负责、Root审。页表AR→R不加硬件，时长捕获不影响原S数据路径。

## 6. 正式collector计划、重复与原始证据

S15通过后，DS0先完成四模式S/N/P/PN×A1/A8/A64×10=120点，均gap0；
再做P/PN、A8、gap0/1000/4000各5=30点，独立phase；后者gap0与主矩阵是不同重复，不能重复入账或挑选互替。
同源DS50仅A8四模式各10=40点。加S15共205个预登记点；不扩大矩阵，不默认回切，不增加另一次S基线测量。
具体轮转次序由Root冻结；建议主矩阵block0..9完整mode×Acell、gap phase block0..4完整六组合，按block旋转起点。

每点流程草案：原manifest/load/BIT/kernel/CLI身份复用 → Root锁持有且上一CLI接受 → 必要quiet MODE写/读回、Vgap设置/读回
→ 原完整wrapper一次测量 → CLI退出后稳定读PN bank → 按原parser校数据/排空/OBS3/PATH，再附PN readback和N counters
→ 保存原command/stdout/stderr/exit与完整dmesg、case/block/phase/mode/gap、boot/seq，不覆盖旧文件。
MODE/控制读写在计时外，不在每copy路径；S15省掉MODE写，原kernel不会为新字段多读MMIO。
同mode写也会清PLRU/IOTLB时，正式各点统一执行相同的控制/映射/warmup协议；S15保持原自然协议，不能混称严格同一初始化历史。
原映射会通过IOTINVAL+IOFENCE同步（L `iommu.c:1722–1801`），但不是新增mode write的原子quiet/PLRU reset证书。

collector按manifest确切计划前缀和原命令重绑；模式readback前后不一致、unsupported capability、busy/rejected写、overflow、
错误status/taint、排空/数据/PATH守恒失败均停止并保留全部raw，不静默fallback成S或软件refill，不自动补点。
旧parser的`gap==4095`只能用于gap0；非0须按新前端边沿合同验证，其他合法性与PATH守恒不弱化。
`full==0`内部mean/max=N/A；A carry/closing另列；private残差可为负，shared精确等式用PATH0 leaves。
PN bank读数随原observer seq和boot关联；没有新增generation也不靠文件名推断实际mode。

所有指标先逐trial计算，再每cell报n/median/Type7 Q1/Q3/IQR/min/max。S/N/P/PN对照仅同phase、同DS、同boot、同block配对，
先逐block相除再描述，不以两个median之比冒充paired median。不同DS/不同boot、S15与旧baseline100只作独立描述/准入比较。
gap phase有自身n5，不能与主矩阵A8 gap0的n10合并；A最终工作量随V时间/gap变化按实测完整保留，不强制相等。
各cell的N实际触发强度另报，N=0不能写作同强度旁路已生效；不据历史模型因果链替本次板数据下结论。

## 7. 下一小批接受条件

本三文件交Root审查，生产实现仍待Root分配。有限host/RTL验证应覆盖：原CLI/ABI字节保持；
S/gap0逐周期原路径；gap0/1/1000/4000边沿/单任务/最后任务无额外gap；STOP在WAIT/COUNT/cooldown；
MODE busy/same-edge VALID/partial write/非法value拒绝且旧状态不变；同mode安全flush；N probe hit不计、真实accept单次按DID计、
N饱和overflow invalid；index隔离；CLI退出后frozen读稳定；fixture递归公开拒绝；可选B的warmup排除、A先/后结束与共同分母。
实际对象/CLI编译与后续板测各自留原始回执，不把本次纯读方案称为已验证实现。
