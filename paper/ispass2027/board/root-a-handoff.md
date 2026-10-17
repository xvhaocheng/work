# Root A integration handoff (live)

Status: preparation and builds in progress; this file is updated with actual collection packets as they become available. The latest immutable DS25 snapshot contains all 50 actual new trials (n=10 per condition); DS100 is in progress. All Root edits are on `ispass/root-a` in `/home/xvhaocheng/work-root-a`.

## Board ownership

Root A is the sole scheduler. The independently launched DS campaign at `/home/xvhaocheng/work/paper/ispass2027/board/runs/ds-sweep-20261017-01` is the only active board task. User authorized coordination with its chat on 2026-10-17; it is to finish DS25 then DS100 (50 trials each), preserve failures, and leave DS100/pathv3 for Root. Root's duplicate invocation failed to acquire UART before JTAG; its failure receipts remain under this worktree's board/runs and are not performance data. Do not relaunch either campaign.

The next sequence is old DS0 BIT + old pathv3 role swap (30 trials), one common software update, old DS0 BIT + multi software E2 (90), then approved P0 DS0 BIT + common software E3 (45). Queue and daily status are the authoritative scheduling entries. Children never use UART/JTAG.

## Data for integration

- `exp/E1b-ds-sweep/checkpoints/baseline100-reference/`: immutable reuse of the 100 historical DS0/DS50 trials, not new measurements. It contains trial CSV and actual sample counts. Later cumulative checkpoints must not be appended together as independent samples.
- `exp/E5-latency/baseline100/trial-stages.csv`: 1600 stage rows from the same 100 historical trials; 160 groups with n=10. Request CDF/p99 are unavailable.
- `exp/E1b-ds-sweep/checkpoints/ds25-progress01/`: 16 actual new DS25 trials plus the explicit 100-trial historical reference (116 total). Each cell has n=3 or4. This cumulative partial packet is for interim synchronization; do not append it alongside a later cumulative packet.
- Complete E1b, E2, E3 and role-swap result packets remain pending. Logs and timing reports provide provenance; they do not replace paper data.

Root does not edit shared `data/board_results.csv`, `README.md` or `CURRENT_STATUS.md`. Import only one final selected packet for each newly measured matrix; avoid importing the historical reference a second time. Keep the configuration, actual sample n, boot/sequence and command links attached to each row.

## Merge procedure and conflicts

Fetch `origin/ispass/root-a`, integrate it in an independent integration worktree, review the diff and complete checks before the owner merges to main. Root does not merge main, force-push or reset shared branches. The currently active independent DS chat also edits the DS builder, profiles, collector interfaces and package metadata, so those paths may conflict when its changes land on main; preserve actual run receipts and source identities, and deduplicate shared preparation code rather than rewriting history.

Internal source commits are retained on the task-named component branches. CVA6/Linux only have upstream remotes and platform has no remote; do not push experimental branches to unrelated upstreams. Task-local incremental bundles/patches preserve these commits with their explicitly recorded baseline prerequisites. Restore and validate internal component history and platform pins before treating the top-level documentation as a reproducible build.

## Newly authorized follow-on preparation

E4 requires explicit 2MiB mapping support and actual leaf-size proof. E5 requires a versioned passive histogram extension for request distributions. E6 P/N/PN/P4M and the full three workload shapes require the documented changes. The user subsequently authorized necessary RTL/Linux changes for the existing experiment list in isolated branches, followed by validation, compilation and serial board scheduling. These items are now preparation work (E4/A3, E5/A1, remaining E3/A2); historical E6 mitigation is last. None has yet been implemented or run. Freeze the current P0/Multi kernel before this follow-on work, and do not resume unrelated mechanism exploration.

Latest data packet: `exp/E1b-ds-sweep/checkpoints/ds25-stage-50/` contains DS25 50 plus explicit historical 100 =150 trials. It supersedes the 16-trial progress packet for cumulative import. No DS100 data has yet been frozen. The P0 new DS0 BIT is timing-qualified and the common E2/P0 software delivery has passed; use the full FPGA build manifest and the software `delivery-manifest.json`, not the short pointer or the retained failed initial software build manifest.
