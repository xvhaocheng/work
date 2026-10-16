# A2：真实 publisher、容量、owner 与条件计时服务

2026-10-15，Codex `gpt-6.1-sol/ultra` 独占接口核查。仅静态源码与既有合同阅读；本席未构建、运行模型、重放原 8-copy 事件或 ACV-2x2、操作板卡、改生产源码或全局文档。机器可读结论、当前源文件 SHA256 和带行号原件摘录见同目录 `publisher-owner.json`。

**结论：普通 2KiB copy 的合法发布入口已经存在，最早控制点是软件尚未读取 `NEXT_ID` 之前；不存在免费、任意长度的硬件 HOLD/ADMIT。真实应用如何给这些 lane 供给、是否自然形成共享选择器的双头，以及完整时间，仍需绑定。** 每个寄存器前端只承诺一个未完成 normal job，不能把深度 8 的 job FIFO 当作八个可并行发布的 job。DONE 退休不是成功校验、cache acquire 或 consumer 完成。

## 1. 必须分开的三个配置

| 绑定 | 实际源码事实 | 可采用的结论 |
| --- | --- | --- |
| 当前板顶层**源码默认值** | `ariane_xilinx.sv:165–172`：DS 50/50、ResearchRole=0、NonblockMode=0；`ariane_peripherals_xilinx.sv:1262–1281,1404–1409`：四 DMA，各 JobFIFO=8、NumAxInFlight=8、MemSysDepth=0、RAWCouplingAvail=0、BufferDepth=3；单 TC | 每 DMA 数据缓冲 3×8=24B；TC 共用 8 AR、8 AW、32 W 拍。这里没有启用 banked 读门或 4KiB 许可。**这不是当前已加载 bitstream 的证明。** |
| 已有 banked/nonblock profile 的条件展开 | 同一外设源码仅在 NonblockMode!=0 且 i<2 时给 DMA0/1 BufferDepth=512；BankedEnable/NonblockEnable 随该 mode 启用；MSI 改为 BT_ONLY | DMA0/1 各 4096B；DMA2/3 仍 24B。每 bank 分到 4 AR、4 AW、16 W 拍，一个 READ 或 WRITE 翻译 owner。2KiB 仍无 full-buffer 许可。不得为本任务改容量或 profile 补正例。 |
| 既有 actual-DMA 条件计时夹具 | `tests/nonblock_actual_dma_top.sv:12–29,42–60`：两个 real DMA、默认512拍、banked harness；`build-command.json` 选 IOMMU vendor AXI，另以 private mux 插普通 DS-first | 可复用其**明确声明的行为服务**和真实 RTL 接口知识。它不是当前板源码默认配置或当前 SVA 应用的执行记录。旧 ACV-2x2 不重跑。 |

板构建源绑定也不同：`cva6/Makefile:228,793–799` 选择 `cva6/vendor/pulp-platform/axi/src/axi_mux.sv`，FPGA `scripts/run.tcl:43` 加载生成列表；旧模型构建记录选择 `cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv` 的 private 副本。两个实现的模块名相同，不可仅凭模块名宣称等价编译绑定。

## 2. 接口、容量、owner 表

| 实体/边界 | 容量与可见内容 | owner 转换、合法动作与限制 | 原件入口 |
| --- | --- | --- | --- |
| 软件 pending / application slot | 当前寄存器 RTL 没有软件 pending 队列；应用/driver 自行保留任务、源/目的 slot 和 generation。配置寄存器是一份 staging cell | 地址可算、调用进入、配置完成都不等于硬件 job 已发布。若 API 要让同 lane 两个 async 调用先返回，必须另建并计费有限软件 pending 与虚拟 receipt；真实 NEXT ID 要到 actual publish 才关联 | FE:325–373,468–559；WR:186–207 |
| lane 寄存器配置 | SRC=0x00、DST=0x08、LEN=0x10、CONF=0x18、STATUS=0x20、NEXT=0x28、DONE=0x30 | normal 2KiB 用 batch disabled、CONF=0、LEN=2048；下一候选可先写 SRC/DST/LEN，但必须在旧 COUNT 已被捕获后，且整个旧 job 中保持 live `ttype` 等不会改变行为的字段稳定。不能修改已 offer 的完整请求 | FE:320–383,519–559；WR:349–352；REG:274–346 |
| NEXT_ID / ID 分配 | 每 lane 独立 64bit functional ID，reset next=2、done=1 | IDLE 的非零 LEN NEXT 读分配 ID并进 COUNT；非 IDLE 普通 NEXT 读不会排队新 job，返回默认0。NEXT 返回是发布/ID收据，不是 backend 已开始或翻译完成；严格区分 cfg response、COUNT 首次VALID、FIFO accept、backend accept | FE:379–383,490–523；ID:29–75 |
| COUNT 首次 offer | 前端一份完整 `burst_req` | COUNT `valid_o=1`；未 ready 必须保持请求，不能后来按 HOLD/cancel 撤回。FIFO握手后进 WAIT。到 backend response 之前不能从该前端发第二个 normal job | FE:519–559 |
| Job FIFO / backend job | JobFIFO物理深度8，非 fall-through；前端从 normal COUNT 发出后 WAIT | 当前前端使一个 lane 的独立 normal job 未完成数上限为1；8深队列不增加这个可达上限。四 lane 各有一份前端；多 lane 是否同时可用须由软件绑定证明 | WR:173–207；FE:519–559 |
| backend burst 与数据缓冲 | NumAxInFlight=8 的 R/W metadata FIFOs；AR和AW各一个 fall-through寄存器；last-flag深度 `BufferDepth+8+0`，默认11、512拍配置520；数据缓冲24B或4096B | 这些是一个 job 的分段/总线义务资源，不是新的 task lanes，也不能把 metadata深度当 payload容量。RAWCouplingAvail=0走 AW bypass；不能套用“首 R 产生 AW credit”的另一个 coupler分支 | BE:110–112,551–624,631–649,811–857；TR:254–282,446–483 |
| 四 DMA identity | DID=10+i，固定PID=20+i；local AXI ID=0。CONF.pv bit3决定是否发PID；ttype bits5:4，cache bits9:6。src/dst PROT=0（unprivileged/data） | 无动态PID或 privileged选择寄存器；pv=0时 substreamID=0。mux prepend后完整 TR ID为0/4/8/12，返回按完整ID路由；软件functionalID不在AXI ID字段 | BOARD:1279–1281；FE:332–366；REG:274–346；MUX:135–181 |
| 上游 DMA→IOMMU mux | 四输入；AR/AW各一个**两项** spill；W owner FIFO=8；W/B/R未开spill | 新鲜选择在锁定 RR arbiter处；AW首次向spill offer时就推入W owner FIFO，不能等外部AW接受才记owner。W严格依owner到WLAST；后到另lane不能抢走held完整头，spill中的前缀也不能反排 | MUX:188–268,275–305,333–386；SPILL:37–95 |
| TC normal header/payload | mode0：8AR/8AW/32W；banked：每bank4AR/4AW/16W；每TC一个主翻译状态机 | AW优先；所选AW或AR头一直保留到 COMPLETE 把物理header交给target demux才pop。一个bank不是READ/WRITE各自多entry result表。TL response接受≠payload结束 | TC:121–164,178–255,625–725；BANK:162–175,314–356 |
| bank / read admission（条件profile） | bank=完整TR ID bit2；DMA0/2共享bank0，DMA1/3共享bank1；bank不是DID | input demux无额外spill，tracker MaxTrans=16；可见新AW也预约读门，signed AW−rawWLAST balance>0时只放owner bank；FullBuffer例外还核DID=10+b。2KiB一律无例外。bank级门不能自动证明四DMA/同bank多source都具有无环进展 | BANK:243–298,314–343；CERT:16–18 |
| TL / PTW / DS | 主TL请求spill两项，主FSM一个正在处理的翻译；banked至多一owner/bank。可选nonblock是另bank的有条件命中旁路，不增加一个独立miss walker。DS AR另有两项spill | 请求spill到slow response才pop；PTW当前PTE读接受并解析后才生成下级地址。DS聚合还含DDTW/PDTW/CQ等，DS source不等于“当前应用指定PTE” | TL:174–214,282–306,1544–1565；PTW:384–407,507–531；DS:77–108 |
| 物理target header / W / 返回 | TC target demux把AW/W/B/AR/R spill均打开，每个spill两项；tracker未显式覆盖时默认MaxTrans=8；bank输出mux无额外spill、W owner FIFO=8 | TC内部物理AW接受、target输出首次AW、下游AW接受、raw WLAST、target WLAST、B捕获、source B接受是不同边界；容量必须都保留。不能只算TC 16W而忽略原DMA数据buffer与targetWspill | TC:432–450,540–568；DEMUX:53–60；BANK:418–440 |
| normal job retirement / public DONE | transport `w_dp_valid_o=b_valid`、`b_ready=w_dp_ready_i`；final-burst B产生backend response；wrapper rsp_ready=1 | WAIT收到response使functionalID retire，随后DONE读匹配。NO_ERROR_HANDLING把response.error固定0；DONE可能只证明错误已退休，不证明copy成功。正常成功需实际R/B响应无错、完整字节与义务守恒 | TR:446–455；BE:519–532,631–648；WR:206–207,319；FE:544–559,619–628 |
| consumer / source reuse / mapping release | 不存在由DONE自动触发的CPU cache/consumer收据 | 必须补cache可见性、消费真实字节与receipt、slot归还、quiet后mapping释放；retire不等于consumer。错误/超时无reset/abort的drain证明时保留owner和映射，不可自由free/unmap | CACHE:91–111；FE:149–150；BE:527–529 |

表中 source key 的精确绝对路径、完整行段和原件在 JSON `sources/evidence`；比如 FE 为 `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv`，BOARD 为 `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`。现有 `.h/.hjson` 的 CONF 文档只列bits0–2，实际手改生成RTL含pv/ttype/cache；适配应以实际reg_top与frontend为准，不能从旧头文件推断没有PV。

## 3. 首个合法控制位置与强普通对照

对一条新 normal 2KiB copy，软件可以在**NEXT读尚未提交**时决定何时发出它、选择当前真正ready的任务或等待公开DONE。通常配置工作为固定初始化（如batch disable与CONF）后每job三次地址/长度写、一次NEXT读，以及一次或多次DONE读。MMIO桥等待、线程协调/锁、poll、供数/compute、CPU调度与cache成本不免费；静态源码不能给出它们的周期。

普通STATUS只反映`~backend_idle_i`（FE:383），不能在前端刚分配ID但尚未进入backend的窗口里充当“无active job”证明；`launch_active`包含HOLD/COUNT/WAIT/GAP/ADMIT但仅LaunchPort开启有效（:278–284）。源码内部`job_quiet`还同时检查前端IDLE、backend idle及无同拍launch（:149–150）。软件最小实现应维护本lane未退休receipt并等DONE，不能只轮询STATUS=0就覆盖请求。

没有可直接用于2KiB的HOLD/ADMIT捷径：controlled HOLD要求**正好64B、64B对齐、全零CONF、batch disabled、backend idle**（FE:253–266）；Research ADMIT只在有效的4KiB对齐batch上工作（FE:153–157,498–504,588–597）。即便buffer有4096B，CERT仍只给**正好4096B且src/dst页对齐**的DEFAULT许可，不能给2KiB增加permit。

默认CONF=0的legalizer在64bit总线上按最大256拍形成**2KiB分段边界**（LEGAL:78–85,139–148），coupled R/W取源/目的更近边界（:197–214,453–469）。因此若两端均2KiB对齐，2048B任务可形成一笔256拍AR和一笔256拍AW；未对齐任务可能多段，不能固定一job=一次lookup。请求总长度可跨页而由legalizer切分，不能把整个job长度直接当成单AXI burst。

以下普通程序必须进入同信息/同预算比较，root决定其最终目标与源任务合法性：

1. **当前用途优先发布**：仅在源依赖和原API顺序允许、两任务均未承诺时，优先发布当前compute真正要用的copy。需要真实应用当前用途信息的现有持有者；不能假定DDR arbiter有这字段。若软件尚未同时看到两个ready任务，就没有该次二选一。
2. **公开完成节流**：一条lane已发的copy不撤回；在公共DONE匹配之后才发布后继，或暂缓另lane未发的未来任务，先等待当前任务真实完成。只能使用copy完成收据，不能冒充免费translation-complete回调；应计等待、poll、协调及丢失重叠。
3. **全局串行发布**：同一时刻仅一个normal job推进，保留原源依赖、所有consumer与slot占用。它是低成本进展对照；在正常有限服务/单DMA依赖可满足时避免多DMA跨owner环。这里没有做故障/任意服务下的活性证明，也没有证明它达成完整时间目标。
4. **当前VALID DS-first/F4**：共享DDR AR选择器已经能从source index识别DS（xbar source3），不需要预知未来PTE或新job信息；在fresh selection优先已有DS，连续四次竞争DS accept后让一个非DS，按原RR选非DS，held头保持到accept。现有private实现控制状态2+3+1+2=8bits；其mux数据宽度、逻辑、连线、时延、面积/Fmax和CPU/应用费用未测。它是一个已可执行的普通构造，非现有生产默认策略，且无法撤销旧spill/held/DDR队列义务。

对同lane双async发布有两个可审查适配：**阻塞submit到lane可接受**（收据用真实NEXT ID，丢失的重叠计费）；或**有界software pending**（调用可先返回虚拟receipt，publish时绑定lane/NEXT ID，wait沿映射找真实DONE，明确容量/slot保持/锁/错误终止）。当前RTL没有第二个已承诺job槽；也不能把应用六槽等同为DMA FIFO。两种适配都必须由应用/driver席确认，不擅自将原HERO发布顺序改成现成平台SVA执行。

## 4. 现有行为DDR能保证到哪里

精确服务来源是 `tests/nonblock_actual_dma_test.cpp:647–719,1291–1400`，不是MIG。必须事前固定：`ptw_latency`、`read_latency_overlap`、`ds_priority`、`write_tail_pause`、所有fixture开关和profile；从共同ready、相同数据/PTE/状态重新生成实际请求，不能回放一条固定后缀trace。

| 方面 | 精确行为与边界 |
| --- | --- |
| 有限读接收 | 一个 `mem_q`，DDR ARREADY仅在`size<16`时为1。accepted AR保存完整ID、addr、len、beat、latency；64bit每拍。16是**行为DDR读队列**的容量，不是整个系统未完成上限；此前还有各级spills/cuts。 |
| 固定head服务 | `read_latency_overlap=false,ds_priority=false`时，只有队头每tick扣其latency，扣到0后的下一tick可首拍；等价于孤立accepted-at-t请求首R为`t+L+1`。之后每tick最多一拍，握手才beat++，RLAST才pop；背压期间不跳过队头。队列中的后继不老化。旧合同L=31是一个显式条件值，不是当前native服务估计。 |
| 可选不同服务 | `read_latency_overlap=true`按accepted_cycle老化；`ds_priority=true`可在无held R且legacy/full-era window允许时，把最老DS跨ID移至队头，保留各ID内顺序。两者会改变服务合同；不能把它们当与FIFO相同的执行。A2若复用FIFO就显式关掉它们。 |
| W/B服务与有限性缺口 | 默认AWREADY/WREADY=1；AW入`caw_q`，W严格写队头并核WLAST长度；WLAST后ID入`cb_q`，之后BVALID持续到真实B握手，BRESP=OKAY。`caw_q/cb_q`是**无显式容量限制的C++ deque**，不是有限DDR写/响应队列模型；实际RTL另有有限credits/cuts，但其组合可达上界还未证明。write_tail_pause为额外测试干预，不可为正例打开。 |
| 真实RTL的结构费用 | conditional system top保留4-source/2-target xbar，MaxMstTrans/MaxSlvTrans=8、CUT_ALL_PORTS、PipelineStages=1、4→6bitID；DS delayer为真实50/50结构。source3包括所有IOMMU DS流量，不限某task的PTE。CPU proxy/配置FSM不代表真实Linux CPU/运行时/consumer成本。 |
| 能输出的保证 | 在全部RTL、输入、控制、返回背压及上述行为服务固定且实际执行完成时，可以给该条件配置的周期/完整守恒；有残余读拍只给工作量，只有同时约束服务、背压与供数才能给有限时间界。服务接收后的FIFO顺序是该模型特性，不是AXI或MIG普遍保证。 |

现有服务不能给出MIG周期、refresh/row/bank/CDC/真实CPU成本或完整SVA应用时间。逻辑C0未完成不是DDR接受了多少AR/R拍的证明；源事件编号不是时钟。原MIG观测仍paused，不借本表恢复旧goal或旧矩阵。

## 5. 最小后继适配合同与 UNKNOWN

最小硬件侧输入记录为：`lane, DID, pv/PID, local/fullAXIID, declared profile/vendor binding, job ordinal, source/destination VA, bytes, CONF, slot generation, source ready predecessors, cfg start/end, NEXT receipt, COUNT firstVALID, jobFIFO fire, backend accept, every burst ordinal, input/target/output firstoffer+accept, raw/target WLAST, source RLAST/B acceptance, public DONE`。每个burst按`lane+job+direction+burst ordinal+fullID`关联，不能只用固定AXIID。owner记录保留spill项、W-owner FIFO、read-gate状态、TL owner/result、PTE依赖、buffer占用，另接软件席的pin/context/cache/consumer/释放收据。

仍缺的决定性条件：

- 真实启用的SVA应用→driver→固定DID/lane与pending语义；此席不把已有平台内核copy工具或关闭的HERO SVM称为当前SVA执行。
- 实际profile与编译源清单、合法2KiB选项、所有必要CPU/consumer和无错误完结路径的绑定。
- 原自然生成时间下，目标位置有至少两个eligible fresh heads，且当时没有held/既有顺序禁止该选择；**不调capacity、phase、额外delay或cache初态去造共现**。
- 若复用行为DDR，必须明确写/B队列组合有限上界或标为建模缺口，固定MMIO/计算/cache/consumer服务，不能只给DDR L。

因此，本席交付**接口与owner静态证书**；新真实竞争、native时间收益、完整SVA应用时间及MIG债务均保持 `UNKNOWN`。这不否定控制位置方法或共享翻译主问题。

## 6. 对单mm、non-PASID、direct DC.FSC重设计的决定性硬件核查

以下仅答root新增的最小重设计问题，不声称现有driver已经实现SVA：

- **身份链成立。** FE:348,360的src/dst PROT=0，经TC:639,665变为`req_data.priv=0`。CONF.pv=0发无PID请求；TL:1118–1122禁止的是pv=1但PDTV=0，以及无PID且无DPE却priv=1，故这个pv=0/priv=0分支不被该检查拒绝。
- **无PC的DC.FSC路径存在。** TL:1154–1166在PDTV=0直接以`DC.FSC.ppn`作`iosatp_ppn`，`DC.ta.pscid`作PSCID；FSC non-Bare启用S1，iohgatp Bare禁用S2。PGD必须提供物理根PPN，不能把内核虚拟`mm->pgd`指针原样写入；软件席处理物理转换与context建立。
- **Sv39能力和三层walk存在。** CAP:423–433硬置Sv39=1、Sv48/Sv57=0；DDTW:379–387在PDTV=0、SXL=0时允许MODE=8并核capability/reserved。PTW:360–364从FSC根与VA[38:30]取首级，后级依真实PTE返回生成。该结论不扩展到其他页表格式或扩展位。
- **用户页权限相容。** TL:1374–1378在S1下要求priv=0访问的叶PTE.U=1；U=1对user允许，SUM约束只作用于priv=1。写额外要求W=1。PTW:494–497要求leaf A=1、R=1，写D=1；没有A/D自动置位。PTW:586–594要求nonleaf A/D/U=0，:598–604要求PTE[63:54]=0。固定实验须用plain Sv39、已建立的普通用户R/W页，并核实A/D状态，不能把FOLL_WRITE/pin一词直接当这些位已验。
- **范围与未核。** 这是单mm/固定映射/无IOPF下的静态可达路径，不是generic PASID SVA支持。仍需软件席绑定双DID独占同mm、PSCID/上下文生存期、真实pgd可见性、pin与写入前准备、页表/数据cache与失效完成、异常/释放协议。新增helper未实现、运行验证为0、完整时间仍UNKNOWN。当前读门、容量和第一offer约束全部照旧，不因SVA改变。

## 7. Root新增 quiescent context barrier：有界反例核验

**有一个静态成立的限定版本。** `IODIR(DID10)+IODIR(DID11)+host VMA → AV marker → 紧随no-AV IOFENCE`，在下列前提下，观察新token且CQH完成**后继条目的fetch进位**，可证明前marker的B=OKAY已经接受，并证明此前指定失效已经在当前同步缓存数组commit。它不是通用IOFENCE完成、PR/PW排空、零故障或native SVA实测证书。Root仍拥有是否采纳方案的决定；本席不运行。

**精确充分条件。** 只有已知的CQ owner提交此bundle，等候期间其他poster不追加、不清CQ错误位、不disable/re-enable或reset CQ；CQB、ring容量和memory不变。令`N=2^(CQB.size+1)`，起点为本bundle的绝对producer序号及其modulo索引，预留全部空槽，bundle长度`k<N`（上述五条要求合法power-of-two容量N至少8，预留五个空槽并保留ring满/空区分），命令连续且无空洞，tail只发布到bundle末尾。marker专用物理cell需8B对齐、8B独占，不能与CQ/DC/PTE/FQ/payload或别的writer共用；以未复用的32bit token作低位，初始化旧值不匹配，并完成初始化/命令/DC/PTE的真实CPU写出及tail发布顺序。marker opcode/func3合法、AV=1、PR/PW/WSI=0、reserved=0；后继合法同类命令AV=0。ATS实际inflight=0且无新ATS。读token有真实可见性/acquire顺序；全64bit期望值是`zero_extend(token)`，因为实现确实写8B而不是规范文字里的4B（CQ:178–190,364–369）。

令marker索引为`m`，后继索引`s=(m+1)&(N-1)`，目标head为`e=(s+1)&(N-1)`。在独占、无后继post且`k<N`的区间，等待`CQH==e`，而非数值`CQH>s`或marker自己的`CQH==s`。CQ REGISTER在最后一拍R接受时就进head、下一状态才DECODE（CQ:276–293）；因此marker自己的head、`head==tail`或token单独可见均不足。进入后继FETCH前，该单FSM必已离开marker WRITE；B_RESP只在BVALID且BRESP=OKAY的握手后回IDLE（:467–502）。前marker若B错误会停ERROR，除非软件清错；所以**最终错误位恰为0不替代“整个区间无人清错/重启”的历史前提**。再核CQCSR无cqmf/cmd_ill/cmd_to；CQH本身是硬件更新的RO寄存器（REGMAP:821–844）。后继head进位只证明后继fetch完成，尚不证明后继自身DECODE完成；本方案所需的前marker证明已经足够，不借它补全通用fence语义。

**失效确实到commit，但只到匹配cache项。** CQ:333–338与426–432在DECODE产生一拍失效；SW wrap与top原样同钟送入TL（SWWRAP:219–220，TOP:537–538,692–693），TL直连DDTC/PDTC/IOTLB。DDTC:101–115计算匹配valid=0，并在:171–180的posedge写回；PDTC:96–114、173–182同样；IOTLB:217–228、317–331使失效优先于同拍update，:464–472写回。因此并非只有未落实的pulse，匹配数组valid在该DECODE结束沿已commit，早于后续marker写。最小non-PASID版本可用各DID的`IODIR.DDT DV=1`（同时清该DID PDTC）及`VMA GV=0,AV=0,PSCV=0`清全部host S1条目，含G=1；若改用PSCV=1，IOTLB:225–229明确排除G=1，范围必须如实缩小。PDTC:96–99对DDT失效仅按DID匹配，未实现DV=0全局分支；因此不把泛化的全局IODIR当本证书覆盖范围。

**不可省略的反例与边界。** (a) token可先在W阶段可见而B仍挂起，单读token不能接受；(b) marker B出错后第三方清CQMF，随后CQH继续，可同时有token和后继head，故必须独占错误恢复；(c)回绕后旧head索引重现或token复用会假匹配，必须上述序号/容量/epoch约束；(d)旧PTW/DDTW或已经锁存的DC/result若还存活，可在失效后把旧内容再次回填，CQ marker不会等待它们，也不清TL当前`dc_q/req_spill`；故“两DMA已真实drain”必须涵盖其完整TL/PTW/result/target/R/W/B生命周期，其他DMA、debug或ATS源停发，不能用单次STATUS=0替代。IOFENCE的PR/PW计数支持仍是CQ:362的TODO，不能靠该命令证明上述drain；(e)FQ独立状态机可稍后写旧DMA故障，CQ marker既不清FQ也不证明没有late fault，功能成功仍须独立故障/字节检查；(f)若PTE/DC先前stores尚在CPU写缓冲，cache失效commit不能使这些stores免费全局可见，必须另有已完成的CPU发布顺序。

Root提出的`CSR_DCACHE=0x701`自M-mode早期关闭且整个bringup保持关闭，是**功能bringup proposal**。本席只额外核到CSR写bit0控制enable（CSR:1096,1809）与WT读控制`~cache_en`走NC（WTCTRL:87–88），NC refill不写cache（WTMISS:367–368）；reset默认enable=1（CSR:1853），故不能把“从boot保持禁用”当现有运行事实。还须证明启动路径没重新开启、关闭前旧line/写缓冲已处理、命令/PTE/DC stores已真正退休，以及consumer也按同一可见性合同运行。该选择不提供cached工作流的性能证据，成本/运行验收仍UNKNOWN。
