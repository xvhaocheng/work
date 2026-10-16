# P4M 21 次预试执行记录

2026-09-20，在已启动的 Linux 6.5.0-rc2-00088-g9f8da9e7a6a0 / P4M v7 BIT 上执行，未重启、重写 SD 或重编译。

- 用户指定 7 条件各 3 次，4096 次 V 传输、256 次预热、4 KiB、V0/A1 固定，50 MHz / DS 50/50。
- 沿用 Dell docs/p4m-short-results-review.md 的三 block 顺序，详见 run.json 和 linux_p4m_pilot21.sh。每 block 一个 Solo；legacy batch 与 P4M gap0 相邻，第二 block 反转先后。
- 21 次全部运行且未重试：18 个 idma.p4m.v1，3 个 idma.batch.v1；每次 CLI exit=0、success=true、result.status=0、V completed=4096；参与 DMA 的 data_errors/quarantined=0，前后 taint=0。
- final_exit=0、collection_failed=0、trials_done=21、PILOT21_HOST_EXIT=0；恢复 printk 设置、退出 root、关闭串口。
- path_observe=Y。为保留全部早期观测，每次打印当时的完整 dmesg，最终再打印一次。原始 UART 因此有重复记录；observations-unique.log 仅按完整行去重并选择观测/状态/警告，不替代原始 UART。历史短试 seq1..4 保留；本轮 observer seq5..25 对应试次1..21。
- records.json 从 UART 提取完整原始 JSON，使用 Python 整数保留 64 位 magic，未经 JavaScript 数值转换。
- 普通 batch 的硬件窗口来自 idma_batch_obs part=time；不能把软件耗时当作硬件窗口。P4M 的 A 进度是同一硬件窗口完整 copy 的完成量，而整轮 HPM/miss/PTW 不属于同一窗口。
- 第三 block 的 P4M A8 gap0 原始样本有别于前两次：V cycles=7248503、V miss/S1PTW=2/2、A 整轮 miss/S1PTW=5141/5141。数据检查仍通过；保留，不删除或补跑，不在此确定原因。
- 按用户要求未做独立验收/主机回归；仅检查新脚本语法、程序内置状态及结果采集完整性。下载哈希和 SD 回读沿用未核验状态，不宣称产物哈希验收通过。
- 仅完成预试，未启动正式采样或 gap4000 等额外条件。

DeepSeek 起草 runner 和结果报告；Codex 审阅、套用交接顺序、增加逐次日志保留，实际执行与回传。最终指标与建议见 RESULTS.md（另行补充）。
