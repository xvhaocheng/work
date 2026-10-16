# composition route-opt net3 172：停止在第 53 项

状态：`stopped`。本轮**不是正式 172 通过**，也不是收益或机制结论。

## 计数与阶段

- planned 172；attempted 53；completed 52；failed 1；not run 119。
- short 16/16 技术完成；noise 12/12 技术完成；explore 24/144 完成，失败 `explore-b1-g1000-a8-mpn`。
- 证据：[summary.json](summary.json)、[manifest.json](manifest.json)、[plan.json](plan.json)、[boot.json](boot.json)、[STOPPED.txt](STOPPED.txt)。

## 配置覆盖

- short：mode 0/1/2/3 × a_pages 0/1/8/16，transfers 16，warmup 16，v_gap 0。
- noise：mode S，a_pages 8，pairs 1..6，slots 0/1，transfers 4096，warmup 256，v_gap 0。
- explore 完成：b1-g0 a0/a1/a8/a16 四 mode；b1-g1000 a0 四 mode；b1-g1000 a1 四 mode。
- 失败：`cases/explore-b1-g1000-a8-mpn/`。失败 meta：[meta.json](cases/explore-b1-g1000-a8-mpn/meta.json)。
- 未运行：失败时点之后全部计划项，共 119。

## 失败记录

- `STOPPED.txt` 记录 SSH 密码命令超时、SSH transport rc=255、CLI stdout 非 JSON、`boot id changed: None`。
- 失败 case 未取得 CLI JSON、exit、boot.after、taint.after；`boot id changed: None` 不是重启证据。
- CLI 参数 timeout=15s；SSH helper 现场超时 300s。两者不是同一层计时。
- Mac en11 carrier active，ping 2/0；UART 已接回，旧正确 115200 参数下被动 3s 与换行后 8s 均无字节；新建 TCP22 连接 5 秒超时；UART 身份查询 8 秒超时，累计接收 0 字节。
- 未证实链路故障，未证实 RTL 死锁。

## 边界

- 未完成探索不得写成收益。
- 科学阴性（fast==0、miss==0）不是技术失败；本轮停止记录属于技术/传输失败类记录缺失。
- 现场记录已回传；同一已验收BIT经JTAG重载后Linux和SSH已恢复，内核net3、boot_id `476dcf54-3dfb-441f-b95c-3440589736f5`、taint0、carrier1。UART启动日志15838字节；本请求累计JTAG2、SD0，科学尝试仍53。恢复结果：[result.json](../composition-routeopt-net3-20261005-03-rescue/result.json)。

DeepSeek 核对已有记录并起草本报告；Codex 直接检查关键驱动源码、实际故障记录并执行恢复。


下一步：保留UART持续记录，再针对第53项定位卡住的位置；本轮不自动续跑119项，不自动重新编译。
