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

`p0-common-software-write01-launch/result.json`已returncode0，完成14:17:50Z。`p0-ds0-load01-launch`于14:17:52Z启动，当前唯一硬件进程。Root有限等待session76190在加载成功且release精确匹配后自动dispatch`p0-subset01-launch`，output=`runs/p0-ds0-subset-20261017-01`；不要手工重复load或collector。加载后45点→原DS0/E2 90。E4/E5后续包已在Root`board/packages/e4-e5-20261017/`备好，正式software/fpga manifest用Root现存副本。A3负责E2 immutable prefix实现；A1仅先准备E6 P4M G前端小批，M未开始；A2 full-E3 BIT构建及ABI5驱动并行。
