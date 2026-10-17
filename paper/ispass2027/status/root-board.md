# Root A 板测与代码线：2026-10-18 JST

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Verification Status: PARTIAL（E1b、E3 P0、E6角色及E4/E5已完成；E2/完整E3/G按队列继续）
- Updated: 2026-10-17T17:01:42.623645+00:00 / 2026-10-18 JST

E4/E5已于16:01:33Z完成20/20（10个同boot/block配对），原数据/CLI/PTE/PA/data/unmap/taint0/最终OBS4+PATH+16×32桶全部通过。V硬件时间中位数4KiB 139.19783ms、2MiB106.88277ms；逐block时间下降中位23.2191%、吞吐提升中位30.2406%，V软件HPM bracket的miss/S1 PTW中位8192→0。E4统一CSV3824行，E5同20点/320stage/10240桶，E5不另计新试次。完整论文包E4-pagesize/paper/e4-common-v2-final01、E5-latency/paper/e4-common-v2-final01；Root另以原record硬件cycles/Fraction独立复算10对通过。

G已于16:56:32Z完成40/40（10完整区组），无失败/重试/替代，boot79a6975d-877d-4a9d-8e19-08834275be7a，BIT8dd6/Linux69d/g-v1、逐点taint0/自然完成/数据与观察门均通过。strict原raw重解析出口16:56:34Z return0，正式包E6-mitigation/paper/g-ds0-final01；Root另从40原CLI words以Fraction独立复核10pairs全部匹配。相对本Ggap0，gap1000/2000的V时间下降中位24.4416%/32.8843%，A own16MiB吞吐下降25.6741%/47.9490%，各n10配对；保留合法较快gap0点，不宣称整体净收益。当前唯一板动作e2-probe-upload01-launch，只向当前G boot的新/tmp文件上传匹配G SPI与已编probe gzip，不写SD/重启/跑点；完成后Root据E3是否最终qualified再选下一加载。完整E3唯一resume仍未qualified；A2并行冻结真实G图表/结果报告。E2两点诊断包/Image/host23已接受，尚无新E2板点。

E2两个版本均在第6点N4_A8失败，各5点分别冻结，不混池；timer重试/再入现象有证据，CPU AXI根因未定。65b候选不能称修复成功。

## 已完成的论文数据

| 实验 | 新试次 | 完成与关键结果 | 论文入口 |
| --- | --- | --- | --- |
| E1b DS曲线 | 100 | DS25/DS100各50、五条件各10；两档WNS均+0.177ns，失败端点/routing errors均0。含历史DS0/DS50后最终200试次。 | [完整数据与9个图](../exp/E1b-ds-sweep/final-results.md) |
| E3 P0小子集 | 45 | DS0，64B/1KiB/4KiB×Solo/A1/A8×5，45/45全部通过；14:25:47Z–14:37:05Z，无失败或替换。 | [最终45点与统一CSV](../exp/E3-workloads/checkpoints/p0-final01/README.md) |
| E6角色互换 | 30 | 原DS0/旧pathv3，V1/A0，Solo/A1/A8各10；同boot/block A8/A1时间比中位数1.10434084。 | [角色结果](../exp/E6-mitigation/role-swap-results.md) |
| E4页大小＋E5分布 | 20（共享） | 20/20通过；2MiB较4KiB配对时间下降23.2191%、吞吐提升30.2406%；真实512桶/试次。 | [E4完整数据](../exp/E4-pagesize/paper/e4-common-v2-final01/README.md)、[E5同20点](../exp/E5-latency/paper/e4-common-v2-final01/README.md) |
| E6 G固定gap | 40 | 40/40、四条件各10；gap1000/2000的V时间下降24.4416%/32.8843%，A own吞吐下降25.6741%/47.9490%。 | [严格40点长表](../exp/E6-mitigation/paper/g-ds0-final01/long-table.csv)、[原word独立复核](../exp/E6-mitigation/root-validation/g-final01.json) |

E3关键口径是同boot、同区组、同V大小的A8/A1配对：

| V descriptor | n | 额外时间中位数 | Q1–Q3 | 吞吐损失中位数 |
| --- | --- | --- | --- | --- |
| 64B | 5 | 14.2461% | 14.1921%–14.2633% | 12.4697% |
| 1KiB | 5 | 13.8811% | 13.8760%–13.9157% | 12.1892% |
| 4KiB | 5 | 10.4000% | 10.3991%–10.4506% | 9.4203% |

**一句话判断：DS0上小传输更敏感，A8/A1增幅约14%，4KiB约10.4%，增加约3–4个百分点。** 进一步同区组跨大小配对的时间增幅差中位数为64B 3.8469、1KiB 3.5157个百分点，各5对。逐block数值、公式和case定位见最终包`size-effects.json`。这些是描述性统计，IQR不是置信区间，不作统计显著性或纯翻译归因。每点固定4096个V descriptor，三大小有效工作分别256KiB/4MiB/16MiB，只在同大小内算慢化；A1已含带宽/仲裁共跑代价。HPM是非原子软件区间，A完整工作含启动/排空；DMA2/3无PATH时不能补造逐设备内部等待。

DS口径始终是IOMMU数据结构访存的附加延迟，DS0为主结果，DS25/50/100只作敏感性分析。E1b同boot/block A8/A1、A16/A1、A64/A1时间增幅中位数：DS0为10.3962%/13.7155%/16.3512%，DS100为76.0753%/88.7220%/98.1181%。E6角色与历史V0各条件中位数的绝对描述差异<0.06%，不作跨boot配对。

所有阶段快照均为同一campaign的累计视图。E1b早期16/50/80新增点与最终200包、E3首9点与最终45包都不得叠计；以实际boot+observer sequence及原命令hash识别重叠。独立DS会话可能已将同一批100新点并入main，后续汇总必须去重。

## 在准备和等待的任务

| 任务 | 实际状态 | 下一步 |
| --- | --- | --- |
| E2 设备数 | 两个版本各接受5点，第6点N4_A8均失败；新65b轮host timeout，独立partial包不混池。 | 暂停性能采集；独立probe Image已通过，待Root审最多两点诊断包；不关闭watchdog/taint门，不原样重试。 |
| E4 页大小+E5分布 | 已20/20完成，两个严格出口均通过，10对Fraction复算通过。 | 时间图和stage1原桶区间图均已接受、复跑；不为E5增加重复板点。 |
| 完整E3 | CVA6 1d587/platform dfebcc，Depth64的原DS0构建exit143；唯一physopt续编15:46:07Z启动，未qualified。正式G软件69d/6491已通过并纳入精确profile。 | collector/exporter已审；pilot30调用及6小时预算门已审。自身BIT验收、G新boot通过后先30点再据实际wall决定续150。 |
| E6 P4M G | 从E5 6c5基座独立准备；frontend2c7、observer0f0403与上层cfb9已审；G唯一BIT15:08:57Z开始构建。 | 40/40已完成、严格出口与原word独立复核通过；图表/报告并行收尾，V/A各16MiB自然完成。 |
| E6 P/N/PN M | 完成历史来源/容量/接口设计与负结果追溯，未启动移植。 | 维持最后优先级，不恢复机制探索。 |

E4/E5冻结包在`board/packages/e4-e5-20261017/`：小型元数据原字节复制，BIT/Image/payload/SPI模块仅本机硬链接，不进Git。E5最终BIT SHA d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c，50MHz，WNS+0.126/WHS+0.047/WPWS+0.062ns，失败端点与routing errors均0，290520/290520 routable nets全完成。Slice50581/50950（99.28%）；LUT166334、regs142751、BRAM50、DSP27。完整E3和G必须各自验收，不能沿用E5通过结论。

## 执行边界与异常

Root worktree `/home/xvhaocheng/work-root-a`，分支`ispass/root-a`。不在主目录切分支，不修改共享README/CURRENT_STATUS/data表，不自行合main、不强推或重置。已接受代码/验证/论文数据持续推送任务分支。正式最终汇总应选择各任务最新完整包，补共享索引和结果表；原始数据、失败与旧结论保留。

用户已授权现有论文清单必要RTL/Linux改动在独立分支实现、编译、验收后串行上板，并授权途中同步论文可用真实数据。当前无需用户决定；时序失败会停止该版本并报告，不自行改约束或搜索实现策略。现阶段均为Codex工作，未调用DeepSeek；历史报告中的模型归属保留原文。

已保留的实际异常：Root重复DS启动在触硬件前被UART锁拒绝；E5首次输出误放共享out后取消独占进程树（非timing失败）；初P0软件最后SPI link缺module.lds，原失败保留、仅模块恢复后正式delivery通过；合法长度backend通过，零长度backpressure负例仍failed且CLI/driver/RTL三层拒零；普通PIE ASan启动失败未冒称通过，non-PIE有限检查另存。上述不作为性能试次或被替换点。

P0结束前的纯主机续接session27823以143终止，原因未建立，未启动E2硬件；独立P0 collector仍45/45成功。Root确认原result=0后才手动dispatch唯一E2加载，未重复任何试次，记录见`board/runs/e3-to-e2-continuation01.json`。下一步E2 collector由Root在boot验证后启动，不再依赖该已结束等待。

E2失败与候选修复详情见[诊断记录](../exp/E2-device-scaling/softlockup-diagnosis.md)，已接受5点的[partial长表](../exp/E2-device-scaling/checkpoints/e2-prefix-failed01-paper/README.md)保留。新软件若及时验收，计划一套共同内核供E2/E4/full-E3，仅依次切BIT；A3正在准备精确known-profile兼容，旧da6包不改，未知新tuple拒绝。

## 最新验收与共同软件（00:03 JST）

Linux65b427相对46df只改multi轮询合作点：N2/N3/N4每未完成轮请求cond_resched，先检查V完成；旧pair/P0/pages/workload仍为原1024门。Root直接审生产diff/调用链，实跑旧38/57/97、ABI5 416与新增6政策情境全部通过；旧MMIO摘要9128914394721286305:498206、nonmulti调度摘要3976756370770541329:357一致。此候选不证明或保证消除softlockup，保留watchdog/taint/超时门，必须新boot/新run实测。

新共用软件tuple固定Linux65b427946152e3aca27babab0a437821729805d4、platform ec99648e3c9a1e48c1e9721e3662bb632eb439bd、OpenSBI e8d21bfdc2dd753314627968e496c3a1188f6dfb、release6.5.0-rc2-ispass2027-full-e3-v1；CVA6 1d587仅软件构建reference，不能冒充E2/E4实际BIT来源。构建已于15:03:30Z passed（所有命令exit0），payload29,363,656B，headroom4,190,776B；SD写入已返回0，当前正加载原DS0并验证新启动。A3已交并由Root实跑20项known-profile/package检查通过，正在准备新tuple的E2/E4有限兼容；未知tuple继续在UART前拒绝。旧da6 ready包原样保留。

15:04Z实际部署更新：共用软件已passed，原始manifest SHA7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4；payload SHAca75a420c9321e89f5fbbe284e836421ec1b0f25c14a4ce0eca6084b068bff95，gzip11,068,218B。当前acece匹配SPI模块已上传/bytesACK，唯一活动板任务为common-v2-payload-upload01-launch；等待result=0且bytesACK后写SD，再新启动采集。当前无正式实验在跑。

有限部署续接 common-v2-deploy01-launch已dispatch，只等待上述upload结果和bytesACK后执行一次SD写入、原DS0 BIT加载及新kernel boot验证；它在等待期间不占UART，任一步失败即停、无重试、不启动collector。执行源码保存在board/manifests/common-v2-deploy-as-dispatched.py；result=0才说明部署完成，dispatch本身不是完成。

15:12Z准备更新：E2新旧精确软件profile小批88218a9已由Root审全production diff并实跑identity42通过；新90点有限续接e2-common-v2-collect01-launch已排队，仅在common-v2-deploy01-launch result=0、新boot及check-only通过后启动，输出e2-device-scaling-20261018-02；目前仍等待上传/部署，没有提前占UART或执行试次。G独立BIT已于15:08:57.646218Z启动（cfb9a0a8/d5cc448，A1/E6/artifacts/g-ds0-build01），原约束/策略且失败即停，Linux65b仅reference。G Linux下一批固定双方W256自然预热后各4096测量，新协议只与自身gap0对照。完整E3 collector首批因续接源码/命令及递归fixture身份边界要求聚焦修订，未放行上板。

2026-10-17T15:19:03.830410+00:00：Root已审并接收5a61c68共用软件E4包，实跑22项检查全过，正式新包board/packages/e4-e5-common-v2-20261018由Root准备并固定manifest/source hash，元数据原字节复制、二进制仅硬链接。有限后续e4-e5-common-v2-collect01-launch已排队，仅在新E2完整90点passed后加载E5 BIT并采20点+直方图；E2若失败则此后续也停止，不自动继续。当前仍上传新payload，未开始新试次。

2026-10-17T15:27:48.209584+00:00：完整E3 collector修订afaa3ca已由Root接收，实跑30项实际入口模拟串口与独立self-consistent seq-gap拒绝通过；当前采集器SHA dc2802d0ff240dd0801a0db6f49bb7372ee4d71e45f43040b28f78ac4a3c7fcc冻结，均synthetic/0新板测。G Linux首批889f75b已审，Root实跑329项通过并直接核RV64异常表/单次ld/fence证据；独立RUN生命周期小批已放行准备，未编G Image。

2026-10-17T15:33:13.632922+00:00：E3 P0图已完成，见exp/E3-workloads/figures/p0-main01（2张PDF/SVG/PNG，15配对/10对比；Root视觉检查和独立paper.csv/Fraction复算通过，9个输出字节相同重现）。45个原试次再利用、新增0；原producer脚本/图/manifest保留，当前plotter只许新output防覆盖。full-E3 exporter warmup_only标签修订已收，Root最新26项真实host入口检查passed，全synthetic/0新板测。

2026-10-17T15:53:15.877878+00:00：Root已接收G RUN69d8小批并实跑1176 G/329 CAPS及旧38/57/97/416+6回归，MMIO与nonmulti yield摘要均保持；尚未编G Image，A1只准备严格CLI/builder下一批。E2 irq-diagnosis只读证据2cac260已由Root直接审关键源码和vmlinux EPC、独立重算40源/7样本及calculations原字节一致；确认timer重试/再入现象，AXI根因未证。后续仅默认off的原生counter诊断小批准备，未编诊断Image/未跑新E2。

2026-10-17T16:10:27.480597+00:00：当前唯一板动作是g-common-upload01-launch：E420/20与新软件passed门确认后，只上传当前full-e3-v1匹配SPI与新Ggzip到全新/tmp文件；未写SD或重启，等待传输bytesACK及新profile/后续BIT验收后由Root决定加载。G CLI507/software-entry9 Root实跑通过，原G软件manifest/transport已收ba3df；旧prepared note保持原文，以passed、Image及实际命令为准。E4论文图已接收ae8e2b3，Root直接读plotter、目视PNG、独立复跑CSV/数值/PNG/PDF全字节相同；SVG只自动ID不同，源/数值无变，E5区间附图仍待画。
