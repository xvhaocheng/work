# E5 同点附加与有界离线区间分析

Root 已接受本接口（2026-10-17）。E5不新增独立50点上板计划：优先附加E4的20个4KiB/2MiB同负载点，
使用本次E5 DS0 OBS4/INFO1/Depth4 BIT与共同E4/E5 Linux。独立 `collect_e5.py` 仅留作备选，当前不排队。
未来完整E3 OBS5/INFO2/Depth64由A2另批明确支持，当前v4硬件/信息格式/容量合同不改。

## 输入和接受边界

纯函数 `rebind_trial(raw_command, cached_item, expected_kernel)` 重绑实际原始命令：

- 外部UART command exit、实际CASE_EXIT、boot/kernel/taint、PRE_SEQ以及唯一CLI JSON都必须通过，CLI对象与原记录逐字段相同。
- 从显式post-command完整dmesg重新解析最终batch/time/AXI三段和16条PATH，与缓存逐字段相同；全部valid/status有效。
- 重新解析INFO1及全部512桶，与原attachment逐字段相同；桶/full、sum/max边界守恒，任何缺项/错序列/错误立即停止。
- 函数接受已有身份，不创建新的运行身份体系。CLI读取既有run manifest/records与原命令，输出新的任务内分析目录；
  原始文件哈希进入分析回执，拒绝覆盖输出。running/partial数据应先按既有方案冻结snapshot，不对后续仍变化的字节发布旧hash。

`trial_intervals(item, attachment, context)` 返回16个stage行。`describe_trials(rows, group_fields)` 只做跨试次描述。
E4尚未定稿时，保持通用validated-item+attachment接口；分组字段由E4真实配置显式传入，不猜测映射粒度/负载。
默认拒绝fixture公开发布。单测可显式内部fixture并在输出永久标为fixture_only，不写入论文可用数据。

## 每试次统计

每段使用真实full计数，p50/p95/p99的秩固定为 **nearest-rank `ceil(p*full)`**，
找累计真实桶计数第一次达到该秩的桶，报告原始闭区间 `[lo,hi]`、bin与rank。
桶内不插值，不把bucket上界/中点称为精确p99。末桶仍为 `[2^30,2^32-1]`。
full=0时n_valid=0，所有分位区间及mean为N/A；原max保持原字段（合法空总体为0）。

保留原PATH sum、full、max，mean以sum/full计算并同时保留整数分子/分母，可追到精确样本均值。
A的总体明确只是 **whole/full窗口内完成子总体**，不是所有A请求；carry、opening和closing分列，不补齐截断请求。
V也沿用同一个full口径，不把各段分位数相加为descriptor端到端尾延迟。

## 跨试次统计

每组必须按真实负载/映射配置分开，另自动分role/channel/stage。报告n_trials、n_full_trials、total_full、total_carry、opening/closing总量。
每个分位区间的lo和hi端点分别计算跨试次min/Q1/median/Q3/max/IQR，四分位按Type7；每个有效试次权重相同。
这些是 **试次级区间端点的描述统计**，不是跨请求分位数或请求CDF。n_full_trials不等于total_full。
精确样本mean与原max也仅以试次级描述统计报告。默认不pool A、不构造合并请求分布，不给桶内推测值。

## 有限接受检查

原始驱动mock日志只作显式内部fixture：验证已知桶边界/nearest-rank、空full、极端尾桶、真实sum/full、carry保留、
跨试次Type7/n计数、重复trial拒绝、raw缓存篡改/最终PATH失败/缺桶/错误版本拒绝、fixture默认禁止发布。
它不证明板测分布；实际论文数据只能来自Root后续同点采集的accepted原始记录。
