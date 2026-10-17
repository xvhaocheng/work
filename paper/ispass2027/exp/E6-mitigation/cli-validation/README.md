# G静态CLI有限验证

实际platform `6491dfd76fc92dc020d17166b327da93605f58b1`，parent `d5cc448`；仅新增
`bench/idma_g.c`并将Linux gitlink/lock指向Root已接受的`69d8b9d4`。driver本批未改。
[完整实际patch](../software-source/platform-g-software.patch)、[恢复bundle](../software-source/platform-g-software.bundle)、
[精确组件/软件输入](../software-source/component.json)、[原源码副本](../software-source/bench/idma_g.c)。

CLI仅允许四个`--condition solo|a8-gap0|a8-gap1000|a8-gap2000`；无参数为solo。
`--device`只能`/dev/idma0`，timeout严格unsigned decimal且1..60000ms。
无随机gap、可变预算、重试、补采或fallback。`--caps`仅一次只读CAPS ioctl；不执行RUN。
它要求完整ABI6/BTC2/OBS4/GAP/G-bank/HPM/RUN_READY能力与固定预算。
这只是软件／硬件接口检查，不表示板测或HPM校准通过。

新独立G warmup所有条件相同：参与lane各256个gap0软件COPY4096自然完成并排空，
再ARM各4096个测量COPY4096自然完成。Solo A页／预算／gap为0；A条件固定DMA1、8页。
比较使用本G a8-gap0；不与旧pair、P0或历史baseline混池。

每次错误仍先输出单行JSON再exit1：`input`完整96B输入、`caps_return/caps_errno/caps_buffer`、
`run_return/run_errno/result_buffer`分别保留。RUN即使EINTR或未copy输出也保留post-ioctl缓冲区，
原始status、software decoder errors、old observer status和32个硬件word彼此分开。
32word用固定16位hex字符串，避免丢失64bit精度；所有统计项（包括未参与A的实际零值）保留。
失败不会伪造hardware errorbits。collector后续独立处理taint/seq/boot与最终valid。

`derived`独立于原始字段：clock=50000000Hz，单位MiB/s，V=16MiB/(raw[10]/50MHz)，
A=16MiB/(raw[13]/50MHz)；Solo A及失败／CAPS时为null。A窗口raw[17/18]和carry/raw[21]、
open-edge/raw[22]原样保留，不用于A whole分母。HPM仍只对应原非原子software V bracket。

[507项实际main检查原报告](run01/report.json)通过，包含四条件、1/60000ms、A在V前／内／后完成、
全部96个CAPS字节逐一变异、输入echo／status／decoder／预算／warmup／masks／data／quarantine变异、
旧kernel/失败/正返回/EINTR不重试，以及64bit原始值与每个stats字段保留检查。
测试使用真实CLI main、link wrappers及UBSan；假syscall计数要求各至多一次，无设备访问。

实际静态RV64 ELF为517224B，SHA256
`a6e475fc24592292bbb9af53742dc18a3650ba889678ef181c7b3605659a82a9`，
readelf证明ELF64/RISC-V且无INTERP；binary只在本机保存，Git保存源码／命令／原始输出。
这些不是DMA、Image、全SoC或板测验证。

复跑（输出须新目录）：

```sh
python3 paper/ispass2027/exp/E6-mitigation/cli-validation/verify_g_cli.py \
  --platform /home/xvhaocheng/work-a1-e6-platform \
  --linux /home/xvhaocheng/work-a1-e6-linux \
  --output paper/ispass2027/exp/E6-mitigation/cli-validation/root-cli01
```

[软件入口有限检查原报告](software-entry01.json)共9项通过：精确source/gitlinks/helper/common inputs、
实际rootfs逐entry保留（mtime依原writer归零）、七个既有binary逐字节相等、单一G executable、
已有G／重复路径拒绝、非法jobs/out拒绝、failed包不可finalize且不产生src/Image。
报告对应构建仅为主机cpio测试；没有运行正式prepare/finalize。

```sh
python3 paper/ispass2027/exp/E6-mitigation/cli-validation/verify_software_entry.py \
  --cli-run paper/ispass2027/exp/E6-mitigation/cli-validation/root-cli01 \
  --output paper/ispass2027/exp/E6-mitigation/artifacts/root-software-entry01
```

原helper/原full-E3 builder保存在`../source/*as-reused.py`；本批代码与验证均由Codex完成。
