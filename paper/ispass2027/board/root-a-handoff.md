# 最新续接 2026-10-17T20:02Z

E8 原DS0新增110/110已于19:58:32Z完成，同boot04afe602、原pathv3、逐点taint0。V2/4在Solo/A1时自身miss/S1PTW为0，A8/A64为4/8；V8的Solo为0，A1为7168、A8/A64为8192（各n5固定）。V8_A1中A自身miss/S1PTW仍为0，保留真实数据供Root B界定模型适用域，不补造因果分解。两份独立50+60论文包均已提交，未叠加旧baseline。19:59Z起唯一板动作是现有DS100一次JTAG加载；成功后采剩余25点，E6合格BIT就绪即在安全边界优先。E6唯一DS0物理构建仍运行，全部必要有限功能检查已通过，无新增构建或重复trace验证。

DS100 load receipt：`/home/xvhaocheng/work-e8-e2-e9/paper/ispass2027/board/runs/e8-ds100-load01-launch/result.json`。E6 build receipt：`/home/xvhaocheng/work-e6-pn/paper/ispass2027/exp/E6-pn/build/dispatch01/result.json`。两者未完成时只等最终事件；不读中间log。E9每次启动的矩阵仍缺原文，未运行。

# 最新续接：2026-10-17T19:37:14.501204+00:00

用户最新要求：更激进简化、不重复日志/同源验证/归档，E6-PN未完成持续推进，不以阶段交付结束。既有时序/S15硬停止仍保留。

- **E6 DS0已实际构建**：A1 PID2229246，19:27:11Z；manifest `/home/xvhaocheng/work-e6-pn/paper/ispass2027/exp/E6-pn/artifacts/ds0-build01/build-manifest.json`，dispatch同树`build/dispatch01/`。只等最终退出结果，不读中间日志。实际输入Root `exp/E6-pn/build/source-manifest.json`，SHA b91327ecc81178ddb93df7d86181e5167c0b35df109f1d962cf6da08e502fea6。CV109b48fd/IOMMU86c2cee6/platform6a388625/AXI95f。
- TL12712已直接审；top完整finite4项/1193checks过；A3实际外围finite05五项/11492cycles过，MODE/忙拒绝/S/N真实复制/自然排空/capture端点正确，199=7。真实Naccept0与195/196=0完全相等，不强加>0门。修过的失败均为测试入口/期望错误，不改冻结RTL。
- DS50独立`build/build_pn_ds50.py`已准备、未执行，Root已读，严格真实S15过后；明确ds50元数据50/50不被模板0覆盖。
- Root已导入S15 collector730d953、glue0024、TL1809、build aab41ac、top finite5da9cff、DS50 4f90be3、正式runner cb053c0（Root HEAD28b8a37附近，最新以git为准）。formal代码已Root完整读，150/30/40，合法N=0保留，gap4095*(g+1)，S15/gate/helper不改。A3分析独立worktree ispass/e6-pn-analysis准备图/统计，A2出口与runner已备。
- 本机现成工具已打包 `board/packages/e6-pn-tools-20261018/artifacts/pn-tools.tar.gz`（542652B）；manifest复用accepted二进制hash，0编译/0上板。PN加载重启后才上传安装/usr/bin/pn-control、pn-config；S15只读、不执行writer。

## 新E8/E2/E9（只用现有BIT，不构建）

- 独立Root工作树 `/home/xvhaocheng/work-e8-e2-e9`，branch ispass/e8-e2-e9，from main225c；稀疏检出。首次普通checkout已中断，其专属partial目录已清理，未改main。今后新worktree用--no-checkout+sparse再read-tree，main有80万tracked。
- E8总135：DS0 V1/A2,4,12,14,32各10=50；DS0 V2,4,8×Solo/A1/A8/A64各5=60；DS100 V1同5A各5=25。预算基于5个campaign：15.7–15.8秒/点，按20秒留裕；含一次DS100启动共约53min，无SD重写，无额外短测。
- **当前唯一板任务** DS0首50：19:23:16Z，PID2226817，`/home/xvhaocheng/work-e8-e2-e9/paper/ispass2027/board/runs/e8-ds0-a-scan01`；完成receipt同`e8-ds0-a-scan01-launch/result.json`。最近见35/50 running无error（非当前完成数保证）。原DS0 e9b6554f/原45bf pathv3/boot04afe602-3051-410f-996f-778edb2fb94c。只复用已就位配置，没有加载/写卡。
- collector `run_a_scan.py`于14ff778启动，原Root board/collect.py parser全部原门；点间stopflag `exp/E8-footprint/E6_PRIORITY`，E6物理就绪时优先交板。不要重复这50点。
- 后续60/25入口已Root读并cherry021d711为fd2c6e5；`collect_e8.py --ds 0 --group victim`=60，`--ds100 --group aggressor`=25。传Root原load+source+software manifest，parser-source显式Rootboard/collect.py；只参数化V页数，原其他门保留。README示例尚指Root目录的脚本/输出，实际脚本在E8 worktree，调用时修路径。
- E8 exporter dbda9f0已Root全读、cherry6110be2；统一17列/统计/同bootblock相邻A差分，Vcycles/A-S1PTW只跨窗描述，不是因果延迟。A2正等首50 result后导出、复制真实records/manifest、push论文数据（不传UART日志），然后通知Root。不要让A2触UART。
- E2原ABI1仅pair、全局实验锁，无法2–3A与各自HPM；已按用户要求写E8分支`exp/E2-device-scaling/change-request.md`并停止新E2，新增0点，不改驱动RTL。旧multi两轮故障保持，不倒填原pathv3能力。
- **E9用户消息截断在“每次”**，async已问每次条件/次数，尚无回复；不猜矩阵，不执行E9。E8已知预算可推进；E9补齐后算总预算，超过8h先汇报等确认。

当前Root仍有小型metadata/helper包待提交推送，E8分支尚待首实际数据后push。Root旧分支远端上次3e5968f，后续cherry尚未push。0完整PN BIT、0 S15实际点，勿称全部通过。

---

# 当前续接 2026-10-17T19:07:49.361026+00:00

板仍原DS0 e9b6554/原pathv3 Linux45bf，boot04afe602-3051-410f-996f-778edb2fb94c，18:39:20Z启动核验/TAINT0，无后续hardware动作或实验采集。不要重复写卡/重跑E2/补旧baseline点。PN完整BIT/真实S15/正式点均0，DS50构建仍硬锁实际S15全过。

Root远端ispass/root-a已到98d147d，有限组件源码+回执已汇合：history5b7、DS9713、TC/IOMMU6082+AXI95f（Root实际13718项）、SW0dbd（Root19704 raw+10功能字节+36源绑定）、observer embedding a324、backend9befa、typedcorewrap30bc（Root203源/raw项、33自然job/19800gold）。同一批的数据不重测。Root最初误认平台typed直接实例已纠正：外围实际_intf，控制ID6/数据ID2；prior typed单位controlID2仅自身有限域。详见E6-pn/interface-callchain-correction01.md。实际Makefile编CVAflattened两个AXI文件，需后续将95f已接受字节精确同步，不能只pinIOMMU vendor。

当前三席（只准备、不触板）：A1在6082后实施两文件TL/MSIPTW只读fullquiet+stable req_data限定restricted_msi，计划已Root接受；之后才IOMMU top。A2在新work-e6-pn-writer/ispass/e6-pn-writer实现独立pn-config，按writer-contract-v1，readonly pn-control/S15旧pin不动。A3在30bc后仅外_intf二参/二输出透传，真实interface控制ID6+高位0x2d/0x35有限验证，typed/adapter原字节，外围/顶层仍只读方案待Root。未来源码汇合/全栈有限验收后才唯一DS0 fullbuild，不启动E3-lite抢先。所有fallback/约束/physopt/stop规则见runtime-contract与README。235/205互斥逐点计划已冻结，均非结果。

旧记录保留；以上段及实际result优先。

# 当前续接 2026-10-17T18:43:36.100043+00:00

原pathv3/原DS0部署实际全部成功，18:39:20Z return0；kernel6.5.0-rc2-ispass2027-pathv3、boot04afe602-3051-410f-996f-778edb2fb94c、TAINT=0。板空闲，不再打开UART或添加基线/E2点。原恢复/upload/SD/boot串行链已结束，未来PN只切合格BIT并保留原Linux。

E6-PN仍无完整BIT/板点。Root已接受history开关留首构、真实backend9befa（109726原raw+85源绑定）、tap e361、独立control ad56、readonly helper共享历史244项、DS9713（168906复核）等有限小批。实际Root回执在E6-pn/root-validation/，不可当整栈或时序通过。A1下一TC/banked/vendor quiet先提精确接口；A2实施已接受九文件SW/program只读idle；A3实施已接受单文件旧observer embedding。均无硬件权限，独立树/分支，Root审后才整合。完整PN DS0首构先于E3-lite；DS50必须实际S15≤0.5%各格通过后再编。MODE合法0/1/2/3/6，235独立点，显著风险首构前放弃history switch则205，不改约束或追加策略搜索。

以下保留历史，以本段和实际result为准。

# 实际板资源 2026-10-17T17:22:17+00:00

G已于16:56:32Z完成40/40（10完整区组），无失败/重试/替代，boot79a6975d-877d-4a9d-8e19-08834275be7a，BIT8dd6/Linux69d/g-v1、逐点taint0/自然完成/数据与观察门均通过。strict原raw重解析出口16:56:34Z return0，正式包E6-mitigation/paper/g-ds0-final01；Root另从40原CLI words以Fraction独立复核10pairs全部匹配。相对本Ggap0，gap1000/2000的V时间下降中位24.4416%/32.8843%，A own16MiB吞吐下降25.6741%/47.9490%，各n10配对；保留合法较快gap0点，不宣称整体净收益。E2 probe上传已于17:16:45Z成功（bytesACK，无额外回读）；SD已于17:28:52Z return0；原DS0/probe加载已于17:36:47Z验证新boot e96e8ccc-aa05-42ce-83f8-714b8a17060d、taint0；当前唯一板动作e2-probe-preflight01，只读检查探针初态/proc，诊断点尚未运行。完整E3唯一resume仍在原设置route，未qualified；不打断。真实G论文图/报告已验收：figures/g-ds0-final01及figures/g-results.md，Root独立重放数值/CSV/PDF/PNG一致。E2两点诊断须新boot及逐阶段Root接受，不能自动连跑。

E6-PN核心任务已按用户新授权启动并行准备：P小批d2a813d已Root直接审源码/gold/逐拍证据并独立15项核对接受；S15 gate 288e7c2由Root实际复跑51项通过，均非板测。A1独占ispass/e6-pn继续IOMMU N核心，A2独占ispass/e6-pn-gap收口原前端cooldown，A3独占ispass/e6-pn-tools实现只读pn-control；三席并行且均无硬件权限。用户已批准最小被动A/共同硬件时长计数。先DS0时序门，再原kernel/CLI的S15逐格≤0.5%回归；通过后才测120+30并允许同源DS50构建/40点。总205独立试次，gap0各phase不复用；不打断已在跑的完整E3构建。详见exp/E6-pn/README.md。

G load/collector/analyzer均已成功结束；当前只有E2 probe preflight活动，load/SD writer均已成功。新boot已验，两点未开始，以各result/正式manifest为准。

以下保留历史，顶部与实际result优先。

# 实际板资源 2026-10-17T16:25:35.318166+00:00

当前运行仍为E5 d934 BIT / Linux65b full-e3-v1、boot bf3063bb-b908-4333-bcb9-54a08511210e。G公共软件上传已于16:24:56Z return0、11,346,554字节ACK；唯一板动作 g-common-software-write01-launch 正按已排有限脚本写SD，不重启、不加载BIT、不启动试次。不要重复打开UART或写SD。完整E3与G将共用Linux69d/platform6491/release g-v1；Root已审精确E3 profile并实跑27项，G collector实跑100+17项通过。两个BIT仍在唯一原策略流程中，未最终qualified，任何加载须先各自满足50MHz、WNS≥0、三类失败端点及routing errors全0。E4时间图和E5桶区间图已Root目视及独立复跑通过；E2默认off timer探针源码1fac已审，专用Image builder仅prepared，尚未编译或跑新E2。

上传及SD写入分别以 board/runs/g-common-upload01-launch/result.json 和 g-common-software-write01-launch/result.json 为准；后者无result时仍在执行，失败则停止。SD成功后仍需Root选择已合格的BIT并验证新boot，不存在自动load或collector后续。当前没有新正式性能试次。

以下为保留历史，以上段落及实际result优先。

# 实际板资源 2026-10-17T16:10:27.480597+00:00

当前唯一板动作是g-common-upload01-launch：E420/20与新软件passed门确认后，只上传当前full-e3-v1匹配SPI与新Ggzip到全新/tmp文件；未写SD或重启，等待传输bytesACK及新profile/后续BIT验收后由Root决定加载。G CLI507/software-entry9 Root实跑通过，原G软件manifest/transport已收ba3df；旧prepared note保持原文，以passed、Image及实际命令为准。E4论文图已接收ae8e2b3，Root直接读plotter、目视PNG、独立复跑CSV/数值/PNG/PDF全字节相同；SVG只自动ID不同，源/数值无变，E5区间附图仍待画。

不要并行打开UART，也不要在upload接收中发送命令。现有匹配模块只对应当前full-e3-v1；新G模块只供未来g-v1使用。

# 最新续接 2026-10-17T16:07:06.794904+00:00

E4/E5已于16:01:33Z完成20/20（10个同boot/block配对），原数据/CLI/PTE/PA/data/unmap/taint0/最终OBS4+PATH+16×32桶全部通过。V硬件时间中位数4KiB 139.19783ms、2MiB106.88277ms；逐block时间下降中位23.2191%、吞吐提升中位30.2406%，V软件HPM bracket的miss/S1 PTW中位8192→0。E4统一CSV3824行，E5同20点/320stage/10240桶，E5不另计新试次。完整论文包E4-pagesize/paper/e4-common-v2-final01、E5-latency/paper/e4-common-v2-final01；Root另以原record硬件cycles/Fraction独立复算10对通过。

当前板卡E5 d934/共同full-e3-v1内核，bootbf3063bb-b908-4333-bcb9-54a08511210e，正式采集已结束，无活动板任务。G正式软件6491/69d于16:02:37Z passed，payload31,460,808B/32MiB余2,093,624B；尚未写SD/启动G。Root准备共用G软件给尚无正式数据的full-E3及G，先审精确profile兼容以减少SD切换。完整E3唯一physopt续编resume01、G唯一BIT原流程仍未最终qualified；保持原约束/策略，失败停不重试。A2正在准备E4图，A1准备G40点collector，E2只读报告已审且新默认off探针待实际patch最终复核，未编诊断Image或跑新E2。

任何旧E2/E4依赖waiter均已完成/失败，不再有效。下一实际板动作尚未dispatch；不要重新跑E4、写旧65b或重复G软件编译。Root已接受CLI507/software-entry9，G Image结果已passed，等待读取/复制正式manifest/transport。

# 新审核补充 2026-10-17T15:53:15.877878+00:00

Root已接收G RUN69d8小批并实跑1176 G/329 CAPS及旧38/57/97/416+6回归，MMIO与nonmulti yield摘要均保持；尚未编G Image，A1只准备严格CLI/builder下一批。E2 irq-diagnosis只读证据2cac260已由Root直接审关键源码和vmlinux EPC、独立重算40源/7样本及calculations原字节一致；确认timer重试/再入现象，AXI根因未证。后续仅默认off的原生counter诊断小批准备，未编诊断Image/未跑新E2。

唯一板动作仍e4-e5-common-v2-load02，collect02有限等待其成功后才采E420；不要重复加载/写SD。

# 当前续接 2026-10-17T15:48:04.863602+00:00

新E2轮 `e2-device-scaling-20261018-02` 于15:41:06Z失败：Linux65b427/full-e3-v1、原DS0，前5点taint0接受；第6点N4_A8再次softlockup并host timeout。调度候选不足以修复。新5点和原失败尾已独立冻结 `E2-device-scaling/checkpoints/e2-prefix-failed02-{snapshot,paper}`（5841行17列，partial、各已测cell n=1），不与旧5点或后续轮混合。旧E4依赖job随E2失败停止，未加载硬件。

Root在旧命令结束后一次CTRL-C观察到shell prompt，仅做两组只读/proc诊断，exit0；timer_list显示nr_hangs10836、nr_retries32547，短恢复窗口不再增长，尚不证明DMA quiet或根因。原字节在board/runs/e2-softlockup-console02。当前唯一板动作 `e4-e5-common-v2-load02-launch`：JTAG重载已合格E5 d934 BIT并启动同一full-e3-v1内核；先验新boot/taint0，再独立E420+E5 hist。无需再次写SD。E2暂停性能采集，独立Codex席只读诊断timer/CSR/SBI路径。

完整E3原build01以143终止、sender未知，原manifest stale running保持原字节；非时序通过或失败。Root已审实际runner/TCL及原约束/route命令并接受5f7a306，唯一检查点续编于15:46:07Z detached启动，out=A2/E3/artifacts/full-e3-ds0-resume01，PID2083297、6小时界限；原physopt DCP只读、原RTL/XDC/RuntimeOptimized不变，不重综合/布局、不重试。最终所有门通过前没有合格完整E3 BIT。G唯一BIT仍离线构建；G RUN候选69d8接受审查中、未编G Image。

以下为历史，以顶部为准。

# 当前续接 2026-10-17T15:33:13.633050+00:00

SD common-v2-software-write01已成功return0；同一部署job正加载原DS0/新共用full-e3-v1内核，load.json尚programming，部署result仍pending。不要重复SD/加载。e2-common-v2-collect01-launch和e4-e5-common-v2-collect01-launch依赖等待均有效；先新boot新90，只有成功才切E5采20+hist。若任何失败，后续停。

Root已收afaa完整E3collector与07eb最新exporter，Root实际30+1 collector、26 exporter checks都通过，均synthetic0板测；正式full-E3还没采，先30点pilot必须等自身FPGA passed。A2仅跟进唯一旧FPGA最终验收不重构建。G Linux首批889已收、Root329通过与native ex-table查过；A1正独立RUN小批，不编G Image。

已收A4 d06c2c8 P045论文图，Root实际视觉检查/精确CSV复算及9文件字节一致复现；新增0试次。A4席结束无法原位续接，Root只加3行机械new-output guard，原producer脚本保存figures/p0-main01/plot_p0_figures.as-executed.py以匹配原manifest SHA，原图/manifest/验证不改。当前graph脚本要求--output全新目录。图/26项最新receipt正随本次推送。

以下历史状态已被覆盖。

# 当前续接 2026-10-17T15:21:42.110277+00:00

唯一板动作：common-v2-software-write01（Root有限common-v2-deploy01-launch已在upload15:20:40Z结果0/bytesACK后自动开始）。不要再手工写SD或重复加载！部署脚本成功后会加载原DS0、验证full-e3-v1新boot；有限e2-common-v2-collect01-launch已等待其result0，自动独立采新90到runs/e2-device-scaling-20261018-02；有限e4-e5-common-v2-collect01-launch已等待新E2完整passed90/结果0，再加载E5d934+同kernel采20+hist到e4-pagesize-20261018-01。所有等待不占UART、任一步失败停，无自动替代。4份dispatcher command/result与board/manifests/*common-v2*是实际入口。旧E2 failed5+尾永久分开，当前旧boot2f31 taint16384只是更新不再采。

Root已收e6d软件、88218E2 profiles、5a61E4package、d0e2Gbuilder。Root E4 known新包board/packages/e4-e5-common-v2-20261018元数据copy与binary硬链，22项Root实跑通过；E2 profiles42 Root实跑通过。Root还未接A2 9b1b9ce collector，要求源码/command/seq/nestedfixture续接修订正返回；new Image不受该host变更影响。

G build15:08:57Z，A1后继a1_g_build在原E6worktree，CVA6cfb9/platformd5cc，原策略/约束，Linux65b仅ref；只读G Linuxb258初批因timeout从原pair300000收敛本协议60000产生新final，等RV64 ex-table/ld/fence证据。RUN未实现，不能提前编G软件。A4 p0_figures新独立main worktree制作已验45点图，不新增数据或改Root结果页。full-E3 FPGA1d587/dfeb仍placement running，不搜索约束。

下方为历史。

# 当前续接（2026-10-17 15:03Z / 10-18 00:03 JST）

板卡仍原DS0/acece boot2f31b9f4、taint16384，已排空，无活动板任务。旧E2 failed5+未接受第6点已冻结，禁止恢复旧轮或混入新90。Root实际复跑multi合作候选65b427全部回归通过（旧38/57/97、416新、6政策、原MMIO和nonmulti yield一致），CLI167通过，E4profile20通过，receipt均在各任务root-validation。A2已获准一次共用Image构建，Linux65b427/platform ec996/CVA6reference1d587/OpenSBI e8d/release6.5.0-rc2-ispass2027-full-e3-v1；out=A2/E3/artifacts/full-e3-software01，等status passed+transport后更新SD。必须用当前acece匹配SPI模块更新，不能先加载新内核模块。新软件供E2原BIT→E4/E5 d934 BIT→full-E3 BIT，仅切BIT。

A3新分支准备E2/E4精确known-profile，新tuple已给但passed package还未生成；原da6包保留。A1原席完成后无法原位续接，新/root/a1_g_build沿原work-a1-e6-*接手；Root已接受0f0403 G observer/wrapper实际全diff/关键测试原件，下一小批上层参数/独立build入口交审后再构建。M不启动。A2 full-E3 FPGA1d587/dfeb唯一构建仍running，50MHz/所有违例和routing0独立验收，不改约束。Root已merge85367、cherry5dc90fb/fa945ec，未merge main。

下方是历史续接，以上为准。

# 当前续接（2026-10-17 14:55Z）

E2首轮已failed，无板任务在跑！接受5点，6号N4_A8因softlockup/taint16384拒绝；原raw与manifest完整保留。Board仍原DS0+acece，所有lane已排空、数据校验通过但tainted，不能继续正式采。Root只读诊断完成，IOMMU IRQ0，timer/softirq source正在核。E2/checkpoints/e2-prefix-failed01-{snapshot,paper}已离线冻结5接受+1未接受尾，不能拿失败硬件时间充正式样本。

A2被明确要求暂缓Image/payload。Root批准候选multi-only合作调度小批：warmup/测量每未完成poll轮cond_resched，先识别Vdone，旧pair/P0/pages/workload原1024策略；不改HZ/PREEMPT/watchdog/taint/RTL/UAPI/STOP/quiet。A2先finite+RV64+实际diff，Root再审，不先上板。CLI/backend首批38ad848待Root归档验收；4788/46df旧软件候选保留但未构建。

新A3 /root/a3_pages_profile接续原work-a3-pagesize，纯host known software profile最小兼容已锁，新tuple未知拒绝。新Linux可能包含调度修复，不能先登记46df。若共用新软件通过，单次更新供E2原BIT→E4/E5 BIT→完整E3BIT，只切BIT；旧da6 ready包不改作后备。E5 source manifest的Linux reference实际acece（顶层无selected_linux）；full-E3 FPGA dfeb的da6是另一条reference，不要混说。

G observer A1正交最终小批18场景+Goff17/hist18+实际wrapper69检查，尚未交最终diff、未编BIT。full-E3 FPGA唯一1d587/dfeb构建仍running。Root不得恢复失败E2或自动重试；新版本实验另起boot/run/身份，旧5点与失败尾独立保留。

# 当前续接（2026-10-17 14:40Z）

P0正式45/45已于14:37:05Z passed，最终包E3/checkpoints/p0-final01（4506行17列CSV、51份来源），subset-results已更新。首9点包只作历史，不叠计。

唯一板任务为e2-ds0-load01-launch（14:38:06Z），加载原DS0+同一p0-multi内核。先前纯host等待session27823已exit143，未启动硬件，不要再等它。E2尚无collector dispatch；Root在本次load result=0且load.json boot_identity_verified后，执行E2 collect_multi.py90点，使用Root E3/p0-common-software-delivery-manifest.json、Root baseline100 source.json与本次load.json，输出runs/e2-device-scaling-20261017-01（new）。所有source_root可限Root工作树。

E2数据途中用已审snapshot_prefix.py与export_prefix.py，完整90点默认出口不放宽；Root实际44有限checks已过。完成后A3准备的六项同block配对报告另审。然后一次更新E4/E5共同软件、加载E5 passedBIT，共采E420+附加hist；包在Root board/packages/e4-e5-20261017，本次当前p0-multi内核只能加载其自身matching SPI模块，不能提前加载e4-e5模块。E4正式software/fpga manifest均使用Root副本。

A2唯一full-E3 DS0 FPGA构建（Depth64，1d587/dfebcc）仍running；ABI5 adapter46df97c已冻结、416新+旧38/57/97有限回归通过，Root已读实际diff/调用链，等最终原件+RV64对象，未编全软件。A1G frontend2c7已接受，Root锁定G observer精确error/control合同，769 reserved0，不加generation；正实施observer/wrapper小批，未授权直接构建BIT。M保持设计。

RootHEAD/远端随当前实质工作同步；主目录与共享汇总文件均不编辑。完整状态与证据入口见status/root-board.md及board/queue.md。下方旧续接只保留历史，以此节为准。

# Root A integration handoff (live)

Status: preparation and builds in progress; this file is updated with actual collection packets as they become available. The final E1b packet contains all 100 new DS25/DS100 trials plus the explicit historical 100 (200 total; n=10 per cell). All Root edits are on `ispass/root-a` in `/home/xvhaocheng/work-root-a`.

## Board ownership

Root A is the sole scheduler. The independently launched DS campaign at `/home/xvhaocheng/work/paper/ispass2027/board/runs/ds-sweep-20261017-01` has completed with returncode=0. Root completed old DS0/pathv3 role swap (30/30 passed) under this worktree `board/runs/e6-role-swap-20261017-01/` and is now transferring the frozen common E2/P0 software. User authorized coordination with its chat on 2026-10-17; it is to finish DS25 then DS100 (50 trials each), preserve failures, and leave DS100/pathv3 for Root. Root's duplicate invocation failed to acquire UART before JTAG; its failure receipts remain under this worktree's board/runs and are not performance data. Do not relaunch either campaign.

Old DS0/pathv3 role swap is complete (30 trials). The next sequence is one frozen acece common software update, approved P0 DS0 BIT + common software E3 (45), then old DS0 BIT + multi software E2 (90). This ordering uses the same two necessary loads and advances the priority small-transfer result. A later E4/E5 software update is separate and already built, pending those trials and E5 BIT qualification. Queue and daily status are the authoritative scheduling entries. Children never use UART/JTAG.

## Data for integration

- `exp/E1b-ds-sweep/checkpoints/baseline100-reference/`: immutable reuse of the 100 historical DS0/DS50 trials, not new measurements. It contains trial CSV and actual sample counts. Later cumulative checkpoints must not be appended together as independent samples.
- `exp/E5-latency/baseline100/trial-stages.csv`: 1600 stage rows from the same 100 historical trials; 160 groups with n=10. Request CDF/p99 are unavailable.
- `exp/E1b-ds-sweep/checkpoints/ds25-progress01/`: 16 actual new DS25 trials plus the explicit 100-trial historical reference (116 total). Each cell has n=3 or4. This cumulative partial packet is for interim synchronization; do not append it alongside a later cumulative packet.
- `exp/E1b-ds-sweep/checkpoints/ds-sweep-final02/`: final complete 200-trial cumulative packet, with 100 new DS25/DS100 samples and explicit historical reference. Select this packet for E1b; see `final-results.md`. `exp/E6-mitigation/paper/e6-role-swap-ds0/` now contains 30 actual new role-swap trials and raw-bound paper CSV. E2 and E3 result packets remain pending.

Root does not edit shared `data/board_results.csv`, `README.md` or `CURRENT_STATUS.md`. Import only one final selected packet for each newly measured matrix; avoid importing the historical reference a second time. Keep the configuration, actual sample n, boot/sequence and command links attached to each row.

## Merge procedure and conflicts

Fetch `origin/ispass/root-a`, integrate it in an independent integration worktree, review the diff and complete checks before the owner merges to main. Root does not merge main, force-push or reset shared branches. The currently active independent DS chat also edits the DS builder, profiles, collector interfaces and package metadata, so those paths may conflict when its changes land on main; preserve actual run receipts and source identities, and deduplicate shared preparation code rather than rewriting history.

Internal source commits are retained on the task-named component branches. CVA6/Linux only have upstream remotes and platform has no remote; do not push experimental branches to unrelated upstreams. Task-local incremental bundles/patches preserve these commits with their explicitly recorded baseline prerequisites. Restore and validate internal component history and platform pins before treating the top-level documentation as a reproducible build.

## Newly authorized follow-on preparation

E4 requires explicit 2MiB mapping support and actual leaf-size proof. E5 requires a versioned passive histogram extension for request distributions. E6 P/N/PN/P4M and the full three workload shapes require the documented changes. The user subsequently authorized necessary RTL/Linux changes for the existing experiment list in isolated branches, followed by validation, compilation and serial board scheduling. These items are now preparation work (E4/A3, E5/A1, remaining E3/A2); historical E6 mitigation is last. E4 leaf queries and E5 passive histogram RTL/Linux have passed their first reviewed implementation batches; full workload implementation is underway. None of these extensions has yet run on board. Freeze the current P0/Multi kernel before this follow-on work, and do not resume unrelated mechanism exploration.

Latest data packet: `exp/E1b-ds-sweep/checkpoints/ds-sweep-final02/` contains DS25 50 + DS100 50 + explicit historical 100 =200 trials. All cells n=10, no failed or replaced measurements. It supersedes all partial cumulative checkpoints. The P0 new DS0 BIT is timing-qualified and common E2/P0 software delivery has passed; use the full FPGA build manifest and software `delivery-manifest.json`, not the short pointer or retained failed initial software build manifest.

The independent DS chat may also import these same new runs to main under its own authorization. Before appending any Root packet, compare actual `(boot_id, observer sequence)` and original condition/DS/BIT rather than row-ID namespaces or copied raw paths: the same trial appears in cumulative snapshots with distinct row IDs. If main already contains those trials, keep its existing rows and merge only missing evidence/analysis; never count Root's copies as additional repeats.

Current runtime (14:10Z): common acece payload upload completed with byte-count ACK; `board/runs/p0-common-software-write01-launch/` is the sole SD-write job. Wait for its returncode0 before loading the P0 DS0 BIT with release`6.5.0-rc2-ispass2027-p0-multi-v1`, then collect45, then original DS0 BIT and E2 90. Never issue a duplicate SD write. E5 BIT is now timing-qualified (WNS+.126, all endpoints/routing errors0), SHA d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c. Its later E4/E5 software is da6b/a686, release`-ispass2027-e4-e5-v1`;20-point collector and Root host checks are ready. The full-E3 depth64 build has separate source/pins and must pass independently.
# 14:22Z续接补充

`p0-common-software-write01-launch/result.json`已returncode0，完成14:17:50Z。P0加载于14:25:44Z完成且boot验证通过，有限等待session76190已结束，45点collector已dispatch到`p0-subset01-launch`，output=`runs/p0-ds0-subset-20261017-01`，这是唯一板任务。首区组9点的论文CSV已冻结为E3/checkpoints/p0-progress01，各n=1。另一个有限等待session27823在45/45正式passed后才自动加载原DS0（e2-ds0-load01-launch）并验证同release，然后dispatch E2（e2-device-scaling01-launch，output=e2-device-scaling-20261017-01）；不要手工重复这两步。失败即停，不补点。E4/E5后续包已在Root`board/packages/e4-e5-20261017/`备好，正式software/fpga manifest用Root现存副本。A3负责E2 immutable prefix实现；A1仅先准备E6 P4M G前端小批，M未开始；A2 full-E3 BIT构建及ABI5驱动并行。

15:04Z实际部署更新：共用软件已passed，原始manifest SHA7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4；payload SHAca75a420c9321e89f5fbbe284e836421ec1b0f25c14a4ce0eca6084b068bff95，gzip11,068,218B。当前acece匹配SPI模块已上传/bytesACK，唯一活动板任务为common-v2-payload-upload01-launch；等待result=0且bytesACK后写SD，再新启动采集。当前无正式实验在跑。

有限部署续接 common-v2-deploy01-launch已dispatch，只等待上述upload结果和bytesACK后执行一次SD写入、原DS0 BIT加载及新kernel boot验证；它在等待期间不占UART，任一步失败即停、无重试、不启动collector。执行源码保存在board/manifests/common-v2-deploy-as-dispatched.py；result=0才说明部署完成，dispatch本身不是完成。

15:12Z准备更新：E2新旧精确软件profile小批88218a9已由Root审全production diff并实跑identity42通过；新90点有限续接e2-common-v2-collect01-launch已排队，仅在common-v2-deploy01-launch result=0、新boot及check-only通过后启动，输出e2-device-scaling-20261018-02；目前仍等待上传/部署，没有提前占UART或执行试次。G独立BIT已于15:08:57.646218Z启动（cfb9a0a8/d5cc448，A1/E6/artifacts/g-ds0-build01），原约束/策略且失败即停，Linux65b仅reference。G Linux下一批固定双方W256自然预热后各4096测量，新协议只与自身gap0对照。完整E3 collector首批因续接源码/命令及递归fixture身份边界要求聚焦修订，未放行上板。

2026-10-17T15:19:03.830410+00:00：Root已审并接收5a61c68共用软件E4包，实跑22项检查全过，正式新包board/packages/e4-e5-common-v2-20261018由Root准备并固定manifest/source hash，元数据原字节复制、二进制仅硬链接。有限后续e4-e5-common-v2-collect01-launch已排队，仅在新E2完整90点passed后加载E5 BIT并采20点+直方图；E2若失败则此后续也停止，不自动继续。当前仍上传新payload，未开始新试次。

2026-10-17T15:27:48.209584+00:00：完整E3 collector修订afaa3ca已由Root接收，实跑30项实际入口模拟串口与独立self-consistent seq-gap拒绝通过；当前采集器SHA dc2802d0ff240dd0801a0db6f49bb7372ee4d71e45f43040b28f78ac4a3c7fcc冻结，均synthetic/0新板测。G Linux首批889f75b已审，Root实跑329项通过并直接核RV64异常表/单次ld/fence证据；独立RUN生命周期小批已放行准备，未编G Image。


## 2026-10-18 JST 更新（2026-10-17T18:05:17.931527+00:00）

E2 timer probe 已完成唯一 N2_A1 诊断对照，原 CLI/数据/排空/taint0 门通过；探针记录182次 handler、21条选取记录，时窗50,192,459周期。N4_A8 于17:53:23Z达到180秒主机超时，持续 softlockup；一次 Ctrl-C 和20秒只读监听均未恢复 shell，未发送 freeze/read，N4探针计数不可取得，不能据 N2 证明根因。原两轮各5点性能 partial 仍分开。当前唯一硬件动作是原 DS0 的 e2-probe-recovery-load01，恢复后准备原 pathv3 软件供 PN S15，不增加 E2 重试。

完整 E3 的唯一原设置续编已于17:45:03Z硬失败：WNS−2.646ns，setup失败3413，hold/pulse失败0，routing errors50014；RTSTAT-13禁止bitgen，无BIT。原route checkpoint/报告/DRC/失败manifest保留，不重试、不改约束。E3-lite只准备单文件时间戳存储写进程拆分和有限gold验证，实际RAM推断/fit/timing未知，任何Vivado任务排在E6-PN首构之后；完整E3正式板点仍0。

E6-PN：P d2a813d、gap dd1a97e、S15 51项以及只读 helper 137项实际主机检查已接受；N 755cd723与独立被动 observer c913dd1已交付待 Root直接审查，均非整栈或板测。最新用户追加 P（不隔离替换历史），保持2/14分区不变；DS0主矩阵150点，加S15 15、独立gap补充30、DS50原四模式40，合计235。先评估开关风险，显著增险则完整构建前关闭并退回205；原约束/一次历史物理优化/失败即停及S15每格≤0.5%门不变。DS50仍须真实S15通过才构建。A1准备history小批，A2准备E3-lite，A3勘查observer接线；三席无硬件权限。


2026-10-17T18:25:03.753260+00:00：原pathv3 transport upload return0/bytesACK于18:22:42Z完成；pn-original-deploy01现正唯一SD写入，之后仅boot身份验证。P_shared组件5b7dfbc已Root读两文件diff/actual gold、27938项原件核验接受并保留首构开关；完整PN尚未构建。独立backend测试首eval默认输入覆盖、AW阻塞分段边界等harness/调度失败原样保留，真实N容量仍在验证，不冒称已过。
