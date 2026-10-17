# Root A 板测与代码线：2026-10-17 JST

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: PARTIAL（E1b新增100点已完成，含历史共200点；E2/E3/E4/E5/E6仍依队列）
- Version Label: root-a-status-v1

## 今日进展

- 已创建独立 worktree `/home/xvhaocheng/work-root-a`，分支 `ispass/root-a`，起点 main `01f981a9c603fa909fdb2fb22450aa7c5197271f`。未切主目录分支，未改共享汇总文件。
- 已分派 A1 接续 DS25/DS100 构建与 E1b 运行包、A2 准备 E3、A3 核查 E2/E5/E4/E6。子席按用户指定的 gpt-6.1-sol / ultra 启动；主会话保持当前模型，不切配置，遵守共享 IOMMU Codex-only；子 agent 不操作板。
- 已审核并将三席的第一批准备分支合入 `ispass/root-a`：E1b 构建/采集/任务内长表，E3 原始能力证据与 P0 方案，E6 DS0 角色互换采集器。Root 工作树的17项调度/采集接口、6项独立长表、15项历史分析拒绝检查、14项角色采集协议检查实际通过；均为主机检查，不记为新板测。
- 已发现并接续原有两个运行中的 DS 构建，输出在主工作区 `iommu-platform/out/ispass2027-20261017-ds25-01`、`...-ds100-01`；不重复构建、不改变其输入。现已验收通过：WNS均+0.177ns，WHS为+0.048/+0.045ns，WPWS均+0.062ns，setup/hold/pulse失败端点与routing errors均0，CPU 50MHz；delayer单元120/145。原始报告与binary清单已合入任务分支。
- 已建立[唯一板测队列](../board/queue.md)。UART 身份回执确认板仍为 DS50/pathv3，boot `b1eaa85f-01d7-4976-9dac-b57cc8af49b7`、taint=0，四 DMA 节点齐全；该初始回执仅代表当时身份；随后发现独立DS会话在12:53:13Z已加载DS25启动旧pathv3。用户已授权联系该会话，由Root统一排程并接续其两档100次结果。本worktree12:56:37Z重复启动被UART锁在触硬件前拒绝，失败原样保留，不重启替代；任何时刻仍仅一个实际板任务。
- 已整理历史统一100次的论文可用 PATH 数据：[逐试次阶段CSV](../exp/E5-latency/baseline100/trial-stages.csv)共1600行、160组，每组10试次。Root 从原始records重算，JSON和CSV完全一致；保留 count/sum/max、boot/sequence、配置和原始命令入口，不生成请求分位数。DS0 A8 的 V读stage1跨试次均值中位数为142.504517周期，最大观察值795周期；这是阶段统计，不是新板测或请求p99。

## 关键数字

- 本轮计划 E1b：两档 × 五条件 × 十次 = 100 次新增板测。
- E3 优先小子集：三种 V 大小 × 三种 A 条件 × 五次 = 45 次；尚未测量。
- 历史基线每点平均采集开销约 15.7 秒，仅作队列预算；不作新硬件性能结果。

## 阻塞与决定项

E3 已直接核实真实 pathv3 batch 在 CLI、driver、RTL 均固定 4096B，并且 V/A 共用 chunk；全局实验锁也不允许通过两个进程拼接。A2 的 `exp/E3-workloads/p0-change-option.md` 给出只覆盖45点的 packed-ring、新独立长度操作、真实字节验证和 batch 地址推进方案。用户已明确回复“批准仅P0最小改动、构建和45点板测”。Root 接受该限定方案，已指派 A2 分小批实施和验证；完整三类负载、其他 RTL 扩展未获此次扩围。DS25/100 仍先使用已写 pathv3 payload，新版本身份和结果另存。

E2 当前只有两条流、一个 `aggressor_id`，三至四路需软件接口扩展；E4 已确认实验driver逐4KiB建图，2MiB需显式软件映射及实际leaf验证，[请求](../exp/E4-pagesize/change-request.md)保持未实施。E5 baseline v3 仅导出 sum/count/max 等聚合，不导出逐请求样本或直方图，只能复用运行中已有的 mean/max。E6 角色互换的动态角色 ARM 与 PATH mux 已直接核读，现有 BIT/driver 可以执行，独立 collector 已通过16项主机协议检查。E4/E5 需要的内核、driver 或 RTL 变化及 E6 缓解构建仍只提交 change-request，等待用户决定。

E2 实际冻结硬件 `ariane_soc::RVIOMMUCfg.NumHpmCounters=31`，6槽是软件会话限制。Root 已接受先做有界 multi HPM 软件接口：四个DID×三event共12槽，保留旧pair/Solo六槽及安全恢复语义；不改 RTL。多干扰者驱动批次须在 P0 驱动接口稳定后审查整合，V精确硬件窗口保留，未被PATH选中的设备不得冒充已有逐设备PATH观测。

P0 前端三处RTL变更已直接审查，79项有限真实frontend验证通过；独立DS0新BIT正在构建。P0驱动共用旧mapping/cache/排空路径，38项实际函数模型检查和RISC-V对象编译通过；已与通过68场景迭代及实际对象编译的multi HPM源码合入内部Linux稳定提交 `f81c82c24a8c6833c14a1d5c6fa315a8b26e1345`。新payload等待multi driver整合后统一生成，不提前写卡。上述检查不等于新板测或cache/RTL事件性能证明。

新P0/E2/E6采集器的共享计数采用[同窗PATH stage0完成数](../board/observer-shared-counts.md)，保留私有入口和各stage的独立守恒，避免把mux spill库存边界误判为DMA错误。Root对既有100试次实际回放，200条读/写共享等式全部匹配；新几何仍不假定私有/共享残差恒为0。原E1b采集器当前不变，若实际触发解析问题保留完整raw后处理，不重测替换。

软件配置集中排程已明确：旧pathv3先完成E1b两档和旧DS0上的角色互换，然后只更新一次共同新payload，继续原DS0 BIT的E2及P0新DS0 BIT的小子集。这样角色对照保留与历史baseline相同Linux，不回写旧镜像。P0以外的三形态、页大小、直方图和缓解扩展均不随编译许可顺带实施。

## 待合并内容

最终仅推送任务分支。共享结果表的新增行由实验目录 CSV 提供，README/CURRENT_STATUS 更新建议在最终交接中列明；本会话不直接编辑这些共享文件，也不自行合 main。

用户已明确允许实验途中持续同步**已采集、论文可用的数据**；逐试次数据、任务内统一CSV和阶段统计在有真实样本后分批提交，明确n及未完成条件，不等整轮结束。日志保留作追溯，不以推送日志冒充同步论文数据。准备分支已推送 `origin/ispass/root-a`；BIT/payload 留在本机。当前唯一活动DS campaign在主工作区`board/runs/ds-sweep-20261017-01/`，Root不改其活动文件，只在稳定边界冻结本任务CSV；原会话两档结束后保持DS100并交还板卡。

## 后续授权与编译进展（2026-10-17 13:03Z）

用户已补充授权未来清单中的必要RTL/Linux改动由agent在独立branch实现、编译，随后统一排队上板。原E4/E5及其余负载的只读请求状态据此转为准备：E4交A3、E5交A1、E3其余形态交A2，均先完成当前关键交付，再给出有界设计和实际代码审查；E6历史缓解最后准备，不恢复新机制探索。先前等待决定的段落保留为当时状态，以本更新为准。

Root已直接审查E2 multi最终驱动/ABI/CLI、未知静默状态不标drained与STOP前in-flight修正，实际源码hash吻合最终有限测试：旧pair/P0 38场景有序MMIO trace与稳定M完全相同，新multi57有限场景和45CLI场景通过，真实RV64 idma.o/iommu-hpm.o编译通过，multi栈1312B。Linux稳定提交`acece3a3ab7d72ba1f936c1984b82a4a55f28d46`已被接受，平台`52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0`固定其gitlink和lock；A2已开始一次共同payload编译，含原三程序、P0和多设备CLI，release后缀`-ispass2027-p0-multi-v1`。主机验证不代替板测。

## 首批新增论文数据（2026-10-17 13:04Z）

已只读冻结实际DS25采集的前16个成功试次到`exp/E1b-ds-sweep/checkpoints/ds25-progress01/`，原始manifest/records字节另存as-observed，按实际n=3或4提供逐试次长表、中位数及Q1/Q3/IQR；显式引用历史100次后总116试次。当前四档矩阵仍partial，DS100尚未采集；未填补缺点、未绘部分曲线、未重测替换。该累积CSV用于阶段同步，不能与后续累计快照重复追加。新试次的BIT为已验收DS25，软件仍是原pathv3；原始命令/boot/sequence/来源完整保留。

## 共用软件与合法长度backend验收（2026-10-17 13:09Z）

共用内核/Image/payload已编译完成。初次构建在最后SPI模块link前因缺少`scripts/module.lds`失败；A2按现有恢复流程补`modules_prepare`，只编模块，原failed build-manifest/log保持不变，修复后新delivery-manifest为passed，正式release匹配。payload为29,363,656字节、32MiB分区余4,190,776字节；Root已用实际delivery通过E2 collector的纯主机版本/冻结CLI验证。E2采集器已整合，Root29项协议检查和90点合成矩阵通过，非板数据。

P0真实固定backend的13个有限case/87个descriptor通过：合法64/1024/4096B在规定对齐环中每descriptor为1/1/2个AR和AW，实际字节/目的内容/越界guards/已接收请求排空一致。Root直接核实际SV/C++与生产调用参数，并将13个summary逐字段核对原始events。额外零长负向probe发现既有backend响应在backpressure下消失，run02明确failed并原样保留；P0的CLI、driver和batch RTL三层实际代码均拒0，故仅接受限定合法域，不声称整个backend通过，不改生产backend。NO_ERROR_HANDLING下SLVERR测试仅证明模拟响应下守恒和排空，不证明错误会上传软件。

## DS25整档论文数据（2026-10-17 13:14Z）

DS25全部50/50点通过，Solo/A1/A8/A16/A64各10。最新任务内CSV及Q1/Q3/IQR见`exp/E1b-ds-sweep/checkpoints/ds25-stage-50/`；含历史引用后共150试次，四档仍partial。V硬件时间中位数依次56.50850/107.13621/133.18369/140.80433/146.57918ms，吞吐中位数283.143/149.343/120.135/113.633/109.156MiB/s。以同boot、同区组A1为分母，A8/A16/A64额外慢化中位数24.3199%/31.4304%/36.8189%，各10配对；它们是DS25敏感性数据，不替代DS0主结果。原campaign已切入DS100加载，未写SD。

P0新DS0 BIT已最终通过：WNS+0.177ns、WHS+0.035ns、WPWS+0.062ns，setup/hold/pulse失败端点均0，257848条可布线net全部布线、routing errors0，CPU周期20ns。Root直接核原始timing/route与routed边界：direct DS wire、delay cells0、forbidden research cells0。BIT SHA为`1056d7795ffc5673b1aaab29b09ad3e1654a299b481457bf0b8067d18f23afa7`；软件仍固定acece3/52c5，正式delivery passed。E2/P0均已具备排队产物，未提前使用板卡。

## E4/E5首批接受（2026-10-17 13:25Z）

E4只读真实leaf查询内部提交`e61315397bd2bc925f74d76f7b8173f2b3256943`已接受，Root直接审完整代码、实际生产函数模型与final sourcehash。65有限case/7232 query（non-PIE ASan+UBSan）和RV64三个对象通过；普通PIE ASan启动异常仍记failed，原始错误与压缩log入口保留，根因未定。旧allocator/map/unmap/fetch未改；同IOVA两leaf交替的既有空表保留行为已记录，不将no-leaf当全部页表内存已回收。第二批将接有界映射/checked清理与ABI4；正式E4尚未上板。

E5首批被动RTL内部提交`6c5c666d45979d3a03fb98cff89ba7ad095f6125`已接受，Root核3文件完整diff、真实full事件、桶编码/MMIO/饱和粘错与状态传播。923拍HistEnable0与基座的35旧计数+128旧PATH字段逐拍相同；HistEnable1有限场景与192段桶/full守恒、98年龄边界、carry/clear/abort/timer/容量通过。首BIT改为共享已验收P0基座CVA6 8aeb、Linux acece，DS0，Depth4；16段×32桶×32bit，observer v4，P0 frontend字节不变。Root已授权先固定完整platform pins后立即构建DS0，不等Linux后批。新RTL从不驱动DMA/AXI ready/valid，实际面积/时序和板测仍待完成，不把有限模型当硬件结果。

完整E3的真实长descriptor探针已确认旧Depth4在16/64KiB下可能overflow（64KiB实测AW库存9，不能直接当8）；A2正在给源级上界和实际工作集设计，不降低当前P0的检查。当前DS100已启动原pathv3，boot `5cc1560a-b07e-4956-b1f2-e95e3cced13b`，首次快照见15个成功试次，整档继续。

## DS100中途数据及E5输出目录纠正（2026-10-17 13:29Z）

已冻结DS100前30成功试次，各条件n=6，见`exp/E1b-ds-sweep/checkpoints/ds100-progress01/`；连同DS25全50，本轮80新增；显式引用历史100后总180，四档矩阵仍partial，无缺点填补或部分图。该包与此前累计快照不是独立新样本，合并须按实际boot/sequence去重。

E5首次13:27:03Z构建错误指向共享main/out；Root发现后要求立即按worktree隔离规则纠正。经argv/cwd确认后停止其唯一PGID，20个当时进程全部退出、最终active=[]，session3258退出143。原manifest、logs、执行wrapper及取消原因按字节保存，明确为cancelled_output_scope而非时序失败；原输出树不移动。相同完整冻结源码/RTL/约束于13:28:41Z从A1独立worktree `exp/E5-latency/artifacts/e5-ds0-build01/`重新构建，session70397。当前仍仅DS100在板运行。

## E1b完成与后续并行准备（2026-10-17 13:43Z）

DS25/DS100全部100个新增试次通过，原campaign returncode=0，无替换试次。最终累积论文包为`exp/E1b-ds-sweep/checkpoints/ds-sweep-final02/`，含历史DS0/DS50共200试次、每条件n=10；接受长表、真实来源、统计及9个PDF/PNG/SVG图已生成。Root重验原命令绑定并检查图面；DS0仍是论文主结果。以同boot/区组A1为分母，DS0的A8/A16/A64额外慢化中位数10.3962%/13.7155%/16.3512%，DS100为76.0753%/88.7220%/98.1181%。完整表见`exp/E1b-ds-sweep/final-results.md`，IQR为描述范围，不作置信区间。旧累计包及独立DS会话可能已入main的同一批试次均须去重。

首次最终导出在绘图时发现系统Python缺matplotlib，既有冻结文件原样保留本机，不当采集失败；任务局部venv安装matplotlib3.10.9后另建final02，不重测。Root已于13:38:55Z开始原DS0 BIT+旧pathv3加载，接续E6角色互换30点，随后一次更新已通过的共同软件，顺序E2 90→P0 45。

E5 Linux提交`22975965c89285f5510cb89fd3a3c865b91c0d36`已接受，Root直接核实际diff和prepare/finish/batch/PATH调用链：仅v4读取512桶，冻结窗外输出，最终任一失效使CLI/PATH拒绝。48实际函数host断言、RV64对象已过；Root另实际执行10项parser拒绝/守恒检查通过。后续与E4合并一次共同软件编译，当前acece正式包冻结。E5独立DS0 RTL构建继续，未有新hist板数据。

完整E3已接受限定单COPY、CONF deburst/decouple=0、同S/D offset的最多33 burst（stage0保守34）容量证明，专用Depth64、OBS5/INFO格式2，保留现有E5 default4/OBS4不改。A2先做前端小批；三形态新协议用等16MiB有效数据和V一完整几何环预热，明确mapped/实际页集及A实际工作量；不与旧P045混池。E4实际leaf查询已审，第二批显式map/checked清理/ABI4正验证，当前未测。

## 角色互换完成及软件切换（2026-10-17 13:54Z）

E6原DS0/旧pathv3角色互换V1/A0共30/30通过，Solo/A1/A8各10，无失败或替换。硬件时间中位数56.56996/107.05280/118.23629ms，吞吐282.836/149.459/135.322MiB/s；同boot/区组A8/A1比中位数1.10434084（额外慢化10.4341%）。与历史V0各条件中位数的独立boot描述差异绝对值均<0.06%，不作跨boot配对或显著性断言。已从原始命令重算生成`exp/E6-mitigation/paper/e6-role-swap-ds0/paper.csv`（17列、2560行）及summary，完整角色结果见`role-swap-results.md`。实际boot为`09e1dddf-43ff-479e-921e-2bafd1e3277d`。

板卡已转共同软件传输；小型pathv3 SPI模块archive已完成UART传输，现传10,520,154字节压缩payload。未加载模块或写SD，待上传完整后沿既有updater直接更新，不加远端hash或SD回读。之后新Linux配原DS0 BIT测E2 90，再只换P0 DS0 BIT测45点。

E4驱动/ABI4实际提交`e3ca456a15d1952b97a2d5bde3a1021e27511893`已直接审核接受：89有限生命周期场景、原38+57回归、45 CLI和实际RV64对象通过；未知静默、短unmap/残留映射保持整lane retain/quarantine。E5合并后需把E4 caps的PATH判别改用v3/v4共用函数并验证；A3随后固定共同SHA/平台锁，编译唯一E4/E5软件包。当前acece E2/P0正式包不改。

## 共同软件已构建与小子集优先（2026-10-17 14:01Z）

E4/E5共同Linux`da6b69fc007ad5e671dc288dd8038d02e3ebe43d`已实际true merge并修正caps_pages的v3/v4 PATH判别。Root直接核完整生产差异，仅包含已审E5逻辑及该1行修订。97 E4、旧38+57、同源码48 observer有限检查和RV64对象已通过，边界分别标明。平台`a68603f0fc668771bc0c9e3a86fcefe289bfa17b`绑定实际组件。一次完整Image/payload构建首次即passed（13:55:49Z–13:56:59Z），release `6.5.0-rc2-ispass2027-e4-e5-v1`，29,363,656B，余4,190,776B，含六CLI及同版恢复模块。Root已核正式manifest和源码/输入身份；当前acece上板包保持不动。

完整E3 frontend提交`2db78c0927d6e70b819b46651df86e82ab139710`已接受，Root读完整RTL、实际C++与双gold wrapper，核commit源与编译snapshot字节相同；206情境/102,190 COPY逐地址检查，旧BTC2 69场景、BTC1 27场景逐拍gold，STOP/active锁与tile/NIC边界原文保留。此为有限前端证据，不是AXI/缓存/时序/板测证明。A2继续独立OBS5/Depth64容量批，尚未构建完整E3 BIT。

排程按用户尽早看小传输量级的要求优化：当前共同acece软件传输写入后，先P0新DS0 BIT的45点E3，再原DS0 BIT的90点E2。仍只需两次必要BIT加载，同一内核不回切，先前E2在前的记录据此更新。E4 20点与E5 hist待两批后接共同新软件及合格E5 BIT，不额外排独立E5 50点。

## E5最终FPGA与共同采集验收（2026-10-17 14:10Z）

E5独立DS0 BIT最终passed，Root直接核原timing159行、route原文及utilization：WNS+0.126ns、WHS+0.047ns、WPWS+0.062ns，setup/hold/pulse失败端点全0，290520/290520 routable nets全完成、routing errors0，CPU20ns、DS0 direct wire、delay/forbidden cells0。BIT SHA `d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c`，本机产物与完整原始报告入口为`exp/E5-latency/build/completed02/acceptance.json`。相对P0原实现，LUT149400→166334、regs126265→142751，Slice50355→50581/50950（99.28%），BRAM50/DSP27不变；这是全设计同工具/约束的布局资源对照，不是单模块精确归因。后续Depth64须自身实现验收。

Root完整审E4 20点collector、实际CLI/leaf proofs/共享PATH0等式/全部hist gate/同boot完整前缀恢复与递归合成身份门，整合后实际运行38协议、14身份和11递归拒绝检查全部通过；均0板数据、未导UART。E5离线分析原日志/最终CLI/PATH/512桶重绑、三向计划点绑定与递归合成拒绝已审，Root9项实际检查全过；请求分位仅nearest-rank桶区间，试次端点用Type7描述，不池化A请求。

完整E3容量提交`1d587125eee690e623e6ffae13e70f9bbe989adc`已审：Root核4文件完整diff及实际测试源SHA等于commit，默认4的v3/v4逐拍gold、Depth64全port/role/channel/stage的64条full99且area=sum、年龄/overflow/wrap证据保留，top连接有限证据不冒称SoC编译。固定平台`dfebcc0425bab25567e8514d156650a608530be1`及独立builder已审，允许启动唯一full-E3 DS0构建。显式Vivado generic64在预合成记录，版本标签明示source-declared，Linux da6仅reference、ABI5软件尚未实现，不用E5验收冒替。

acece共同压缩镜像10,520,154B已在913.29秒传入并正常bytes-ack，现`p0-common-software-write01`更新SD；模块已加载的旧boot不再作正式测量。写入成功后按新顺序P045→E290，下一正式boot要求新release且taint0。没有追加远端hash或SD回读。

## P0加载与后续准备（2026-10-17 14:22Z）

共同软件SD更新于14:17:50Z返回0，P0新DS0 BIT加载在14:17:52Z启动，仍在正常启动过程；未记为已接受试次。唯一有限续接将在boot身份验证成功后启动45点collector，随后仍是同Linux切原DS0测E2。E4/E5纯host包已复制元数据并硬链接本机产物到`board/packages/e4-e5-20261017/`，无板操作。E4完整论文出口Root29项有限检查及OBS4限定2项通过；共享hist helper显式接受OBS5/INFO2/Depth64，Root26项parser/analysis/collector检查通过，E4父采集器仍锁OBS4。

Root已直接读ABI5首批实际geometry/offset/prefix helper、固定64/544B UAPI及OBS4/5驱动差异，接受有界几何和日志合同，ABI5 ioctl尚未完成，不称软件已就绪。A2继续独立驱动小批，full-E3 DS0硬件在独立编译。A3准备E2不可变partial出口，必须保留真实N/90与原running/failed状态，原完整出口不放宽。E6来源包已审；按用户后续RTL/Linux准备授权，先做P4M G最小前端，V/A各固定4096×4KiB自然完成、分别计完整时间，M的S/N/P/PN仅保留设计。该准备不抢占E4/完整E3顺序，不恢复机制探索。
