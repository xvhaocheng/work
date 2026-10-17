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
