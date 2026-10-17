# Root A：FPGA 板测队列

最新 2026-10-17T20:12Z：E6-PN 首个DS0 BIT实际通过，构建19:27:11–20:09:58Z（42m47s），WNS+0.177ns、WHS+0.039ns、WPWS+0.062ns，三类失败端点/routing errors均0；原设置首次即过，无物理补试或约束修改。BIT37554105，完整原manifest已复制到E6-pn/build/completed-ds0-01，source b91327ec保持原件。E8 DS100于20:10:49Z在点间让板，17/25真实接受、preempted，新增总127/135，余8未跑，不补造。当前唯一硬件动作是E6 PN DS0加载PID2305006；有限续接PID2305198仅在加载成功后上传既有工具、运行原命令S15并离线判门，任一步失败即停，无MODE/GAP写。DS50尚未启动，必须真实S15通过。

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: UNVERIFIED（下表区分构建、准备与实际采集）
- Version Label: root-a-board-queue-v1

本轮板卡唯一调度与执行者为 Root A。子 agent 只准备代码、构建和分析；任何时刻只有一个 JTAG/UART/板测任务。各工具继续使用既有 UART 独占锁。此文件仅在 `ispass/root-a` worktree 维护，不修改主目录或共享结果表。


## 推进方式调整（用户要求，2026-10-18 JST）

停止反复读取编译中间日志与重复验证同源已通过的组件。剩余IOMMU顶层和外围接线并行完成，按固定接口自主协调，集中做必要集成检查后立即进入唯一DS0首构。Root直接审关键生产diff及实际结果，不再逐小批重放全部原trace；重复源码/日志归档不阻塞编译，必要原件保留本机。

完整编译以进程退出/结果文件为完成信号；失败后定位相关错误日志，成功后读取最终时序、布线及BIT结果。保留原硬门与失败停止，不增加新验收矩阵。板卡当前空闲，原pathv3配置有效；DS50仍须真实S15先过。

E8新增运行：19:23:16Z开始原DS0 V1/A2,4,12,14,32各10，50正式点；独立工作树`/home/xvhaocheng/work-e8-e2-e9`，UART独占。全部E8为135新点，预算约53分钟；原pathv3无法运行新E2多干扰者矩阵，已停止该项，不改代码/构建。E9缺每boot矩阵待用户补齐。E6回归就绪在点间优先，不中断正在执行的单点。

## 当前资源与顺序

DS25/DS100 构建已完成并验收：两档WNS均+0.177ns，CPU 50MHz，setup/hold/pulse失败端点及routing errors均0。独立会话“构建并采集 DS 延迟曲线”的两档100次已全部完成；Root只读复核并冻结最终200试次包（含历史100）。用户授权协调后已统一排程。Root本地12:56:37Z重复启动被UART锁在触硬件前拒绝，失败原样保留，不计试次。

E4/E5已于16:01:33Z完成20/20（10个同boot/block配对），原数据/CLI/PTE/PA/data/unmap/taint0/最终OBS4+PATH+16×32桶全部通过。V硬件时间中位数4KiB 139.19783ms、2MiB106.88277ms；逐block时间下降中位23.2191%、吞吐提升中位30.2406%，V软件HPM bracket的miss/S1 PTW中位8192→0。E4统一CSV3824行，E5同20点/320stage/10240桶，E5不另计新试次。完整论文包E4-pagesize/paper/e4-common-v2-final01、E5-latency/paper/e4-common-v2-final01；Root另以原record硬件cycles/Fraction独立复算10对通过。

G已于16:56:32Z完成40/40（10完整区组），无失败/重试/替代，boot79a6975d-877d-4a9d-8e19-08834275be7a，BIT8dd6/Linux69d/g-v1、逐点taint0/自然完成/数据与观察门均通过。strict原raw重解析出口16:56:34Z return0，正式包E6-mitigation/paper/g-ds0-final01；Root另从40原CLI words以Fraction独立复核10pairs全部匹配。相对本Ggap0，gap1000/2000的V时间下降中位24.4416%/32.8843%，A own16MiB吞吐下降25.6741%/47.9490%，各n10配对；保留合法较快gap0点，不宣称整体净收益。

E2 timer probe 已完成唯一 N2_A1 诊断对照，原 CLI/数据/排空/taint0 门通过；探针记录182次 handler、21条选取记录，时窗50,192,459周期。N4_A8 于17:53:23Z达到180秒主机超时，持续 softlockup；一次 Ctrl-C 和20秒只读监听均未恢复 shell，未发送 freeze/read，N4探针计数不可取得，不能据 N2 证明根因。原两轮各5点性能 partial 仍分开。原pathv3部署已于18:39:20Z全部成功：SD写入返回0，原DS0 BIT加载/启动验证返回0，kernel6.5.0-rc2-ispass2027-pathv3、boot04afe602-3051-410f-996f-778edb2fb94c、TAINT=0。板卡空闲，无采集/后续硬件动作；等待合格PN BIT。未增加原基线点或E2重试。

完整 E3 的唯一原设置续编已于17:45:03Z硬失败：WNS−2.646ns，setup失败3413，hold/pulse失败0，routing errors50014；RTSTAT-13禁止bitgen，无BIT。原route checkpoint/报告/DRC/失败manifest保留，不重试、不改约束。E3-lite只准备单文件时间戳存储写进程拆分和有限gold验证，实际RAM推断/fit/timing未知，任何Vivado任务排在E6-PN首构之后；完整E3正式板点仍0。

E6-PN：五种合法配置S/N/P隔离/PN/P共享历史的合同已冻结（raw MODE0/1/2/3/6）。Root直接源码与实际原件核验已接受P/N/history、原前端gap、S15离线门、被动时长、原事实tap、新独立MODE控制、真实双backend及DS transport保护小批。真实N backend在WREADY0/AW可走时接收完整4096B；Root原raw重放109726项，另源绑定85项通过。只读helper已适配P共享历史且实际244项/71情境通过，无MODE/GAP写操作。真实MSIPTW首拍后24拍指定尾响应空窗始终nonquiet；Root实际重放168906项通过，默认关闭不实例17FF tracker。各结果仅有限组件证据，尚无完整PN构建/板点。

历史开关只有现有local-touch输入门，无新增IOTLB状态；目前未见显著增险证据，纳入首DS0构建计划，物理风险仍待原设置构建。DS0主150+S15 15+独立gap30+DS50原四模式40，共235；如首构前发现显著时序风险，退回205且说明。原时序硬门/最多一次历史route后物理优化/S15每格≤0.5%不变，DS50只在实际S15过后构建。当前三席并行：TC/vendor6082/95f和SW/program0dbd已Root有限接受；A1实施TL/MSIPTW完整只读quiet与稳定debug来源修正，A2新增独立受限pn-config writer，A3补实际外层_intf透传（typed30bc已有限接受，原“平台直接typed实例”判断已纠正，实际control-ID6新验证另行保留）；三席不触硬件，均须Root实际审查后整栈集成。

E2两轮各接受5/90后在N4_A8失败，性能采集暂停，独立诊断；不会原样重试、换版本混池或放宽taint门。

| 原优先级 | 任务 | 状态 | 采集规模 | 预计板时 / 条件 |
| --- | --- | --- | --- | --- |
| 1 | E3 DS0 小子集 | 完成45/45，最终包每cell n=5 | 3 个 V 大小 × Solo/A1/A8 × 5 = 45 次 | 已完成；小传输A8/A1增幅约14%，4KiB约10.4% |
| 2 | E1b DS25 | 完成50/50，五条件各10，全部接受；阶段CSV已冻结 | Solo/A1/A8/A16/A64 各 10 次 = 50 次 | 约 20–30 分钟，须 50MHz、WNS≥0、违例端点和 routing errors 全零 |
| 3 | E1b DS100 | 完成50/50，五条件各10，全部接受；最终四档200试次包已冻结 | 同上 50 次 | 已完成；两档仅换 BIT，复用已写 pathv3 |
| 4 | E2 DS0 设备数 | 两轮各接受5/90；N4_A8均失败，暂停采集、独立诊断 | 1/2/3 个干扰设备、每设备 1/8 页，加同总足迹6页对照，共9条件各10次 | 共90次，约25–40分钟，不改既有DS0 BIT |
| 5 | E3 DS0 论文子集 | 完整版本route硬失败、无BIT；E3-lite仅离线准备，实际RAM/fit未知 | 六种大小及三类流量组合；新协议等16MiB有效字节 | 用户上限6小时；先30点pilot按实际用时定稿，不与P045混池 |
| 6 | E4 DS0 页大小 | 20/20完成；真实leaf/data/直方图全部通过 | 4KiB 对2MiB，各10次，20/20完成 | 同boot十对、同负载、仅改变IOMMU leaf；已产正式CSV与图 |
| 7 | E6 DS0 角色互换 | 完成30/30，Solo/A1/A8各10；原DS0 BIT及旧pathv3 | V=DMA1、A=DMA0；Solo/A1/A8各10次 | 已完成；同boot/区组A8/A1中位数1.10434084，旧payload不再加测 |
| 附加 | E5 延迟分布 | 被动直方图随E420/20完成，正式桶CSV与原桶区间图已接受 | 完成同一E420点的16阶段×32桶；不另计20新试次 | 保留carry/overflow/最终有效性，量化分位所在桶区间 |
| 新核心任务 | E6-PN 统一基座 | 三席并行小批，N/被动observer有限组件已接受；DS0 S15门通过后才DS50 | 含P共享历史：S15+主150+补30+DS50 40，共235；风险回退205 | 采集粗估62分钟另加部署；PN首构早于E3-lite，须时序及原命令S回归 |
| 已完成 | E6 G固定gap | 40/40及论文图/数据已验收 | 四条件各10；双方独立W256+4096自然完成 | 已完成，协议与PN原后台A分开 |

板时暂以既有 DS0 50 次约 785 秒（约 15.7 秒/点）加配置切换与日志开销粗估，不是硬件运行时间或保证。构建耗时不计为占板。能力受阻的任务保持未测，可推进其余合法任务。

E3 P0 已获用户明确回复“批准仅P0最小改动、构建和45点板测”。在新代码/BIT 就绪前继续 E1b；E1b 保持实际旧 pathv3 payload，不提前换成 P0 内核。新 P0 使用紧密页环与新独立长度操作，旧 ABI1/4KiB 行为保留。全三形态、E4/E5 RTL 扩展及缓解构建不包含在此授权内。

核实 E2/P0 都需新软件后，按“同一Linux配置集中测完”的规则调整实际排程：**旧pathv3上的DS25全50点→DS100全50点→旧DS0上的E6角色互换30点→一次更新新共同payload→P0 DS0新BIT的E3 45点→原DS0 BIT的E2 90点**。角色互换因此在更新软件前完成，保留与历史baseline完全相同的Linux，避免后续回写旧镜像。E3完整三形态、E4大页及E5直方图按后续明确授权并行准备，验收后单独入队。

## 完成记录

历史 G1 和统一 100 次已完成，保留原始包和身份，不为本队列重复测量。本轮已冻结DS25前16个实际成功试次，见 `exp/E1b-ds-sweep/checkpoints/ds25-progress01/`；该快照含16个新增试次及显式引用的100个历史试次，状态partial，各条件n=3或4，无填补、无部分曲线图。活动campaign继续采集，快照不可与后续累计包重复追加。

## Git 与交接

Root 分支 `ispass/root-a`，worktree `/home/xvhaocheng/work-root-a`。A1/A2/A3 各自使用独立 `ispass/a1-ds-sweep`、`ispass/a2-workloads`、`ispass/a3-capabilities` 分支。Root 审核并整合到任务分支；最终推送供合并，不自行合入 main。共享 `data/board_results.csv`、`README.md`、`CURRENT_STATUS.md` 的拟合并内容写在任务结果和状态里。

## 后续修改授权更新（2026-10-17 13:03Z）

用户明确补充：“未来还有实验需要修改rtl 和linux. 都可以加载agent进行, 先在单独branch里面修改然后编译. 然后轮流上班测试收集数据”。据此，现有论文实验清单中的必要RTL/Linux改动无需再因原只读/测试程序限制逐项等待：A1在backend证据后准备E5被动直方图，A3在E2采集器后准备E4显式2MiB映射，A2在当前共同软件后准备E3其余负载形态；E6历史缓解移至最后准备。各批先由Root审设计与实际代码，固定源码后验证/编译，验收通过才加入唯一板队列，不混改正在构建的源树。当前P0/Multi软件固定Linux `acece3a3ab7d72ba1f936c1984b82a4a55f28d46`、平台`52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0`，一次共同payload已开始编译。此授权不恢复新的机制探索，也不允许子agent自行争用板卡。

DS25全50点已完成且无采集失败，最新累积论文包为`exp/E1b-ds-sweep/checkpoints/ds25-stage-50/`（含DS25新50+历史100，共150试次）。每条件n=10；四档矩阵仍partial，等待DS100。旧`ds25-progress01`仅为历史快照，不与本包重复追加。P0新DS0 BIT最终验收已通过：50MHz，WNS+0.177ns、WHS+0.035ns、WPWS+0.062ns，三类失败端点与routing errors均0。E2/P0硬件与共用软件已全部就绪，按旧payload集中顺序等待上板。

DS100中途快照为`exp/E1b-ds-sweep/checkpoints/ds100-progress01/`：DS25 50+DS100 30共80新增试次，含历史100后总180；DS100各条件n=6，整档未完。该累计快照替代此前阶段包作当前查看，合并时只选最终包，不叠加计数。

E5 hist构建首次输出误放共享main/out，已在早期停止该独占20进程树，旧session退出143，非timing失败；原manifest/log与取消/无遗留进程证据保存，原输出未移动。合规新构建于13:28:41Z在A1独立worktree `exp/E5-latency/artifacts/e5-ds0-build01/`启动，固定SHA/RTL/约束不变；后台编译不占板。

14:01Z排程更新：共同软件写入后直接加载P0新DS0 BIT先跑45点，再换原DS0 BIT跑E2 90；仍是同一Linux、两次必要BIT加载，切换数不增加，提前回答小传输干扰量级。E4/E5共同Linux已冻结`da6b69fc007ad5e671dc288dd8038d02e3ebe43d`，平台`a68603f0fc668771bc0c9e3a86fcefe289bfa17b`；唯一新软件构建已passed，release `6.5.0-rc2-ispass2027-e4-e5-v1`，payload29,363,656B。E5 BIT已进入routing，尚未最终验收。该后续软件不替换当前正在传输的acece正式包。

E5最终BIT于14:06:29Z通过：WNS+0.126ns/WHS+0.047ns/WPWS+0.062ns，全部失败端点0，290520/290520可布线net全路由、routing errors0，DS0直接wire、CPU20ns。正式完整manifest和原报告在`exp/E5-latency/build/completed02/`；BIT SHA `d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c`。完整E3的1d587容量批和固定dfebcc builder已审，允许独立构建；尚未记为该版本时序通过。

full-E3 DS0唯一构建已于14:10:50Z启动，`exp/E3-workloads/artifacts/full-e3-ds0-build01/`（A2独立worktree），当前running；CVA6 1d587、平台dfebcc、显式Depth64，Linux da6只作reference。构建不占板，当前板卡仍仅SD update。

14:22Z更新：SD updater成功返回0后，Root有限续接已在14:17:52Z启动P0 BIT加载；另一个有限续接只在boot身份成功后启动45点collector，无第二上板任务。E4/E5只读运行包已复制元数据并硬链接冻结产物到`board/packages/e4-e5-20261017/`，没有启动硬件或重复hash。E2 immutable-prefix两个纯host出口已交A3实施，完整90点出口仍严格；E6只先准备G(P4M)最小批，P/N/PN的M移植未启动。

首区组论文数据见`exp/E3-workloads/checkpoints/p0-progress01/`：V64/1024/4096B的同区组A8/A1额外慢化分别14.2461%/13.8811%/10.3991%，各n=1、running/partial；不将其当完整45点结果，不与最终包相加。Root有限续接session27823只在P0正式passed且45/45后才加载原DS0、验证同内核并启动E2；任一步失败即停，无自动替代试次。

14:40Z更新：P0 45/45全部通过，最终包含首9点，不叠计。纯host续接27823以143结束、未启动E2，原因未建立；Root确认P0成功后已于14:38:06Z手动dispatch唯一E2加载。先前“自动续接E2”的描述已失效，后续由Root在启动验证后执行collector。软件formal delivery原字节已复制到Root E3/p0-common-software-delivery-manifest.json，E2将用此副本，便于source_root限定Root工作树。

14:55Z实际失败更新：E2旧90点轮failed/accepted5，见E2/checkpoints/e2-prefix-failed01-paper及softlockup-diagnosis.md。原第6点硬件V262.105ms而软件收尾23.014s，不作为性能样本。Root已要求不关watchdog、不改HZ/PREEMPT、不放宽taint；仅评审multi未完成轮合作调度候选，失败原因仍待验证。新软件若验证通过将是新source/boot/run，不与本前5点合并。

00:03 JST更新：multi合作候选65b427已通过Root实际回归和源码审核；ec996平台/同release-full-e3-v1共同Image正在唯一构建。旧5点partial与失败尾不并入新90点。G observer0f0403经Root直接审查接受，A1后继只准备上层启用和build入口，尚未编G BIT；E4 known-profile首批Root20检查通过。板卡仍tainted/排空、无正式采集。

15:04Z实际部署更新：共用软件已passed，原始manifest SHA7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4；payload SHAca75a420c9321e89f5fbbe284e836421ec1b0f25c14a4ce0eca6084b068bff95，gzip11,068,218B。当前acece匹配SPI模块已上传/bytesACK，唯一活动板任务为common-v2-payload-upload01-launch；等待result=0且bytesACK后写SD，再新启动采集。当前无正式实验在跑。

有限部署续接 common-v2-deploy01-launch已dispatch，只等待上述upload结果和bytesACK后执行一次SD写入、原DS0 BIT加载及新kernel boot验证；它在等待期间不占UART，任一步失败即停、无重试、不启动collector。执行源码保存在board/manifests/common-v2-deploy-as-dispatched.py；result=0才说明部署完成，dispatch本身不是完成。

15:12Z准备更新：E2新旧精确软件profile小批88218a9已由Root审全production diff并实跑identity42通过；新90点有限续接e2-common-v2-collect01-launch已排队，仅在common-v2-deploy01-launch result=0、新boot及check-only通过后启动，输出e2-device-scaling-20261018-02；目前仍等待上传/部署，没有提前占UART或执行试次。G独立BIT已于15:08:57.646218Z启动（cfb9a0a8/d5cc448，A1/E6/artifacts/g-ds0-build01），原约束/策略且失败即停，Linux65b仅reference。G Linux下一批固定双方W256自然预热后各4096测量，新协议只与自身gap0对照。完整E3 collector首批因续接源码/命令及递归fixture身份边界要求聚焦修订，未放行上板。

2026-10-17T15:19:03.830410+00:00：Root已审并接收5a61c68共用软件E4包，实跑22项检查全过，正式新包board/packages/e4-e5-common-v2-20261018由Root准备并固定manifest/source hash，元数据原字节复制、二进制仅硬链接。有限后续e4-e5-common-v2-collect01-launch已排队，仅在新E2完整90点passed后加载E5 BIT并采20点+直方图；E2若失败则此后续也停止，不自动继续。当前仍上传新payload，未开始新试次。

2026-10-17T15:21:42.110277+00:00实际资源：唯一上板操作为common-v2-software-write01；upload已成功。有限部署→新E2→E4/E5严格依赖各自result0；Root不再手工重复这些动作。G BIT和完整E3 BIT仅离线构建；E3 P045点图由A4离线准备，不新增试次。

2026-10-17T15:27:48.209584+00:00：完整E3 collector修订afaa3ca已由Root接收，实跑30项实际入口模拟串口与独立self-consistent seq-gap拒绝通过；当前采集器SHA dc2802d0ff240dd0801a0db6f49bb7372ee4d71e45f43040b28f78ac4a3c7fcc冻结，均synthetic/0新板测。G Linux首批889f75b已审，Root实跑329项通过并直接核RV64异常表/单次ld/fence证据；独立RUN生命周期小批已放行准备，未编G Image。

2026-10-17T15:33:13.633116+00:00：共同payload的唯一SD写入已成功（session.exit_code0）；同一部署正在加载原DS0/新内核，未开始90点。既定E2→E4/E5依赖队列保持，勿重复操作。P045两张离线论文图已验，原45不另计。full-E3 collector/exporter主机验收完成，FPGA尚未通过不能排实测。

2026-10-17T15:48:04.863602+00:00：以上“当前资源与顺序”为最新，以下时间记录只保留历史；新旧E2部分样本不得当成完成90点。

2026-10-17T15:53:15.877878+00:00：Root已接收G RUN69d8小批并实跑1176 G/329 CAPS及旧38/57/97/416+6回归，MMIO与nonmulti yield摘要均保持；尚未编G Image，A1只准备严格CLI/builder下一批。E2 irq-diagnosis只读证据2cac260已由Root直接审关键源码和vmlinux EPC、独立重算40源/7样本及calculations原字节一致；确认timer重试/再入现象，AXI根因未证。后续仅默认off的原生counter诊断小批准备，未编诊断Image/未跑新E2。

2026-10-17T16:10:27.480597+00:00：当前唯一板动作是g-common-upload01-launch：E420/20与新软件passed门确认后，只上传当前full-e3-v1匹配SPI与新Ggzip到全新/tmp文件；未写SD或重启，等待传输bytesACK及新profile/后续BIT验收后由Root决定加载。G CLI507/software-entry9 Root实跑通过，原G软件manifest/transport已收ba3df；旧prepared note保持原文，以passed、Image及实际命令为准。E4论文图已接收ae8e2b3，Root直接读plotter、目视PNG、独立复跑CSV/数值/PNG/PDF全字节相同；SVG只自动ID不同，源/数值无变，E5区间附图仍待画。

E6-PN首轮DS0已于19:27:11Z实际启动（PID2229246，source SHA b91327ec）；CVA109b/IOMMU86c2，真实顶层/外围elab已通过，功能有限检查并行。构建本机入口见`exp/E6-pn/build/dispatch-reference01.json`；不轮询中间日志，等待完成/失败事件。E8原DS0采集继续占唯一板，PN无板点。

E8首50点已于19:36:23Z完整通过：A2/4的V miss/S1PTW均0；A12/14/32均2，分别n10一致。旧baseline另boot的A8已有2，不能把本轮A12称全局阈值。论文数据位于独立ispass/e8-e2-e9的`exp/E8-footprint/paper/e8-ds0-a-scan01/`，50新增点、17122长表标量行，不上传UART日志。原DS0同boot的V2/4/8×Solo/A1/A8/A64共60已于19:37:38Z开始，PID2235534；E6物理构建继续，真实外围有限检查已全部通过。

2026-10-17T20:02Z：E8 原DS0新增110/110已于19:58:32Z完成，同boot04afe602、原pathv3、逐点taint0。V2/4在Solo/A1时自身miss/S1PTW为0，A8/A64为4/8；V8的Solo为0，A1为7168、A8/A64为8192（各n5固定）。V8_A1中A自身miss/S1PTW仍为0，保留真实数据供Root B界定模型适用域，不补造因果分解。两份独立50+60论文包均已提交，未叠加旧baseline。19:59Z起唯一板动作是现有DS100一次JTAG加载；成功后采剩余25点，E6合格BIT就绪即在安全边界优先。E6唯一DS0物理构建仍运行，全部必要有限功能检查已通过，无新增构建或重复trace验证。
