# E8：现有 BIT 足迹扫描

状态：准备采集。只用既有 DS0/DS100 BIT 和 SD 上原 pathv3；不构建、不改驱动/RTL。E6 回归就绪时在单点之间让板。

新点共135：DS0 V1×A{2,4,12,14,32}各10（50点）；DS0 V{2,4,8}×A{Solo,1,8,64}各5（60点）；DS100 V1×A{2,4,12,14,32}各5（25点）。旧A1/8/16/64引用原200点，不重复采集或叠计。固定4KiB×4096、预热256，原V0/A1批量协议，保留原数据/排空/taint/同序号OBS及16条PATH检查。每个配置内区组轮换，失败即停、不补点。

## 运行前板时预算

取现有campaign开始/结束时间（UTC），包含UART与CLI开销：

| 来源 | 点数 | 时长/秒 | 秒/点 |
| --- | ---: | ---: | ---: |
| baseline100 ds50-first | 25 | 288.457135 | 11.5383 |
| baseline100 ds0-middle | 50 | 785.073761 | 15.7015 |
| baseline100 ds50-last | 25 | 288.642422 | 11.5457 |
| ds-sweep ds25-all | 50 | 786.718930 | 15.7344 |
| ds-sweep ds100-all | 50 | 788.355260 | 15.7671 |

原件：`/home/xvhaocheng/work-root-a/paper/ispass2027/board/runs/baseline100-20261017-01/*/manifest.json`；ds-sweep使用`exp/E1b-ds-sweep/checkpoints/ds-sweep-final02/raw/*/as-observed/manifest.json`，不用被UART锁拒绝的另一次campaign。baseline100三次load各约470.9–471.1秒。

按20秒/点预留：E8采集45分钟；复用当前已加载DS0，仅切DS100一次约7分51秒，合计约53分钟。若两档各从未加载状态开始，则约61分钟。E2若接口可用，额外60点约20分钟；实际原pathv3不支持，故不运行。E9消息停在“每次”，点数待补；其板时为3次新启动约23分33秒，加每个正式点20秒，不能将未知矩阵称为已预算。已明确E8部分远低于8小时；E9若使总预算超过8小时，先汇报等待确认。

`run_a_scan.py`先采V1：只读取现有load/source记录，直接调用原`board/collect.py`解析器，不改原断言或CLI JSON；每点立即保存完整record和原UART路径。`--stop-file`只在点间交板，已采前缀保留。V多页入口单独准备，避免等待整包才运行。既有身份和二进制清单仍以Root的baseline100、ds-sweep和原软件恢复包为准；不增加哈希/SD回读/短测关口。
