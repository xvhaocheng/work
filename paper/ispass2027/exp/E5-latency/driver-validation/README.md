# E5 v4 驱动读取：真实函数有限验证

Linux组件 `22975965c89285f5510cb89fd3a3c865b91c0d36` 是已接受P0/multi
`acece3a3ab7d72ba1f936c1984b82a4a55f28d46` 的直接后继，只改 `drivers/misc/idma.c` 的observer区域
以及multi capability的path magic判别；三份现有UAPI原字节不变。

主证据 `runs/run02/summary.json`：真实抽取的5个驱动函数在mock冻结MMIO/日志环境中48项断言通过。
此处是 **fixture_only host测试**，日志里的计数和时长是测试输入，不是FPGA/板测或论文数据。
原驱动整文件、实际抽取函数、完整harness、UAPI、命令及输入哈希均保留。
run01仅因测试include路径缺少`include/`而编译失败；修正测试路径后run02通过，未改生产代码。

覆盖：v1普通/v2无path batch/v3旧PATH兼容、v4 prepare与multi CAPS_SELECTED_PATH一致、错误magic/DID/忙状态、
v4每试次准确512桶+1info单遍读、16条hist记录、sum/full不符、无A却非空、overflow、格式/保留位、
高32bit损坏、既有run error与-EIO返回；异常64bit原值仍保留日志。

驱动在原finish drain/verify之后、旧全局batch/PATH日志之前读新桶。
每个hist行的valid表示该段在已知状态下通过检查；若后段才失败，先前行不会倒写。
**接受一次试次必须同时核命令成功、最终batch valid=1、全部16条旧PATH valid=1及全部16条hist valid=1**；
不能仅靠某条hist行valid=1接纳。任何新错误都会使最终batch/PATH全局valid=0并返回-EIO。
HIST_INFO只读一次，每个桶只读一次，使用672byte格式缓冲和单个值/总和，没有4KiB栈数组。

`object01/manifest.json` 是独立O目录实际RISC-V `drivers/misc/idma.o` 交叉编译，passed、0 warnings。
配置原样取自已通过的P0/multi软件构建，仅生成prepare头和目标object；未编译Image/rootfs/payload。
全部新输出位于本工作树 `../artifacts/driver-object01/`；日志和配置副本在此任务目录。

```sh
python3 paper/ispass2027/exp/E5-latency/driver-validation/run_driver.py \
  --linux /home/xvhaocheng/work-a1-e5-linux \
  --out paper/ispass2027/exp/E5-latency/driver-validation/runs/NEW_RUN
```

最小 `../metadata/linux-e5.bundle` 需要acece prerequisite，已在真实Linux仓库verify。
该小批供Root与E4合入后统一软件构建，不另外生成需独立写SD的新payload。
