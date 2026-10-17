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
