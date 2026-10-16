# P4M formal60 recovery 交付说明

## 状态

- 原始在线 formal60：旧 runner 24 在线通过；第 25 次 formal-b09-g2000 因 `startup_line_count before=36 after=36 want=37` 检查 exit5；后 35 未跑。原始 run.json 仍为 24 在线 / 25 attempted / 35 unrun / runner exit5，未改写。
- 第 25 次作为离线复核追认保留；不伪称 25 在线通过，不重跑/剔除，不抹掉原始 flags。
- resume35：35 次尚未运行，状态 prepared_not_run；Dell 无接板。
- 本目录由 Codex 准备/应用；本 README 由 DeepSeek 起草；实际执行与接受以 Codex 记录为准。

## 本目录内容

- PLAN-original.json：冻结原计划。
- remaining-plan.json：resume35 范围 flat26..60；先 b09g0、b09g1000，然后 b10..20 原顺序；分配 12/12/11，合并已有 8/8/9 可成 20/20/20。
- audit.json、statistics.json：恢复审计与首 24 样本统计。
- platform-p4m-formal60：新单文件脚本；旧 rootfs 内 runner 未更新；不能直接用旧 collector（固定 60）冒充已适配。
- SHA256SUMS：交付文件校验值。

## 运行前置

- 当前 Linux exact 6.5.0-rc2-00089-gefd9461d6459；path_observe Y；taint0。
- 当前 dmesg maxseq37 完整唯一 startup+batchtime，且 a_begin_ns=3600085892000。
- 原板端 /tmp/p4m-formal60-215406 存在，并保留第 25 次的 JSON、原始 dmesg 和 params.txt。
- 若发现重启、额外试验、seq 改变、证据缺失等不通过则停止人工复查；不能用改门限/重新跑 formal60 绕开。
- 板端继续运行 Linux，使用已有 00089/P4M v7，无需 SD 更新或重启或 BIT 载入；无内核/RTL 修改或构建。

## Mac 操作员步骤（待执行）

- 按既有传输方式将新单文件放到板端 /tmp/platform-p4m-formal60。
- 串口完整保存 stdout/stderr：`sh /tmp/platform-p4m-formal60 resume35 /tmp/p4m-formal60-resume-01`。
- 新输出 dir，不能覆盖旧 dir；不用 tee 管道掩盖退出码；退出立即 `echo $?`。
- 完整保存 35 JSON/err/dmesg/argv/matrix/params/taint/resume35-preflight/串口和 exit。
- 完成后只读归档回 Dell 再合并审计。
- 脚本 35 完成不等于论文 60 正式已审结。
- 既有 Mac 采集器自动上传/参数适配未实现；不声称脚本已传到 Mac/FPGA。

## 主机与离线验证

- tests/test_board_p4m_start.py 41 项通过；tests/test_p4m_formal60_recovery.py 16 项通过；tests/test_p4m_formal60_resume.py 15 项通过，含真实 seq37 归档 BusyBox 预检查。
- shell syntax 0；加强版 offline_revalidate.py 25/25 all_passed=true；global_metadata 通过；PLAN25 前缀通过；taint0；CLI exit0；capture complete；JSON success/errno0；data_errors/quarantined0。
- 证据 tree 和原输入 hash 未变；Codex 机械核对全部 14 份原始交付 SHA 一致；25 份 before-dmesg 错误文件皆空。

## 证据路径

- 原始：[RESULTS.md](../../mac-p4m-formal60-20260920-215406-run-cicggil4/RESULTS.md)。
- 恢复：[审计](audit.json)、[统计](statistics.json)、[主机验证](../validation-final.json)、[原始文件校验值](../input-hashes.json)。
- 完整恢复报告：[p4m-formal60-recovery.md](../../../docs/p4m-formal60-recovery.md)。
- 未验证边界：下载 BIT hash / SD readback / HPM 校准未验证；非本次阻塞。

## 归属

- DeepSeek：起草本 README。
- Codex：应用脚本、执行/接受主机与离线检查、机械核对 SHA、准备 resume35。
- 实际 resume35 板端运行：尚未执行；状态 prepared_not_run。
