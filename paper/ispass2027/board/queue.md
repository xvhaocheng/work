# Root A：FPGA 板测队列

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: UNVERIFIED（下表区分构建、准备与实际采集）
- Version Label: root-a-board-queue-v1

本轮板卡唯一调度与执行者为 Root A。子 agent 只准备代码、构建和分析；任何时刻只有一个 JTAG/UART/板测任务。各工具继续使用既有 UART 独占锁。此文件仅在 `ispass/root-a` worktree 维护，不修改主目录或共享结果表。

## 当前资源与顺序

DS25/DS100 构建已完成并验收：两档WNS均+0.177ns，CPU 50MHz，setup/hold/pulse失败端点及routing errors均0。独立会话“构建并采集 DS 延迟曲线”的两档100次已全部完成；Root只读复核并冻结最终200试次包（含历史100）。用户授权协调后已统一排程。Root本地12:56:37Z重复启动被UART锁在触硬件前拒绝，失败原样保留，不计试次。

E4/E5已于16:01:33Z完成20/20（10个同boot/block配对），原数据/CLI/PTE/PA/data/unmap/taint0/最终OBS4+PATH+16×32桶全部通过。V硬件时间中位数4KiB 139.19783ms、2MiB106.88277ms；逐block时间下降中位23.2191%、吞吐提升中位30.2406%，V软件HPM bracket的miss/S1 PTW中位8192→0。E4统一CSV3824行，E5同20点/320stage/10240桶，E5不另计新试次。完整论文包E4-pagesize/paper/e4-common-v2-final01、E5-latency/paper/e4-common-v2-final01；Root另以原record硬件cycles/Fraction独立复算10对通过。

当前唯一板任务 g-ds0-20261018-01 正式G40采集：2026-10-17T16:45:49.663115+00:00观测已接受9/40，status=running，无补采/重试。G BIT8dd6993f已成功加载，新boot 79a6975d-877d-4a9d-8e19-08834275be7a、Linux69d/g-v1、taint0验证通过。独立G最终资格为50MHz、WNS+0.177/WHS+0.036/WPWS+0.062ns，setup/hold/pulse失败端点/routing errors全0，295368/295368 nets全路由。完整E3唯一resume仍未qualified，因此G先就绪先测完，之后共用同G内核只切BIT。G collector Root100+17、分析41+两campaign ID检查均通过；完整40之前不宣称每条件n10或正式性能汇总。E2默认off probe Image已通过，Root审两点命令包并独立实跑23host检查通过，运行包/压缩payload/SPI均已准备，仅离线、尚未上板。

E2两轮各接受5/90后在N4_A8失败，性能采集暂停，独立诊断；不会原样重试、换版本混池或放宽taint门。

| 原优先级 | 任务 | 状态 | 采集规模 | 预计板时 / 条件 |
| --- | --- | --- | --- | --- |
| 1 | E3 DS0 小子集 | 完成45/45，最终包每cell n=5 | 3 个 V 大小 × Solo/A1/A8 × 5 = 45 次 | 已完成；小传输A8/A1增幅约14%，4KiB约10.4% |
| 2 | E1b DS25 | 完成50/50，五条件各10，全部接受；阶段CSV已冻结 | Solo/A1/A8/A16/A64 各 10 次 = 50 次 | 约 20–30 分钟，须 50MHz、WNS≥0、违例端点和 routing errors 全零 |
| 3 | E1b DS100 | 完成50/50，五条件各10，全部接受；最终四档200试次包已冻结 | 同上 50 次 | 已完成；两档仅换 BIT，复用已写 pathv3 |
| 4 | E2 DS0 设备数 | 两轮各接受5/90；N4_A8均失败，暂停采集、独立诊断 | 1/2/3 个干扰设备、每设备 1/8 页，加同总足迹6页对照，共9条件各10次 | 共90次，约25–40分钟，不改既有DS0 BIT |
| 5 | E3 DS0 论文子集 | BTC3/Depth64已审，原构建exit143，15:46:07Z唯一physopt续编；collector/exporter已审 | 六种大小及三类流量组合；新协议等16MiB有效字节 | 用户上限6小时；先30点pilot按实际用时定稿，不与P045混池 |
| 6 | E4 DS0 页大小 | 20/20完成；真实leaf/data/直方图全部通过 | 4KiB 对2MiB，各10次，20/20完成 | 同boot十对、同负载、仅改变IOMMU leaf；已产正式CSV与图 |
| 7 | E6 DS0 角色互换 | 完成30/30，Solo/A1/A8各10；原DS0 BIT及旧pathv3 | V=DMA1、A=DMA0；Solo/A1/A8各10次 | 已完成；同boot/区组A8/A1中位数1.10434084，旧payload不再加测 |
| 附加 | E5 延迟分布 | 被动直方图随E420/20完成，正式桶CSV与原桶区间图已接受 | 完成同一E420点的16阶段×32桶；不另计20新试次 | 保留carry/overflow/最终有效性，量化分位所在桶区间 |
| 最后准备 | E6 P/N/PN/P4M 缓解 | 原始来源和设计已审，先分配P4M G前端小批；M保持设计 | G正式软件/40点collector已审，独立BIT待最终qualified；未测 | G采用两lane各固定4096个4KiB任务；若先qualified可利用E3构建等待时间测完40点 |

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
