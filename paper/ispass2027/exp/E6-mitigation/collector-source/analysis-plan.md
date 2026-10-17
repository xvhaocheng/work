# G固定40点分析合同候选

只分析通过Root验收的本G独立协议40点：10区组×Solo/A8gap0/1000/2000，
V/A各W256软件COPY gap0自然完成并排空，再各4096×4K自然完成；Solo无A。
相同50MHz DS0、OBS4/Depth4/hist，同一次已验boot与软件/实际BIT组合。
不与P0／旧pair或历史P4M启动混杂数据混池，不据合成fixture声称性能结论。

最终分析入口须重读原raw命令与绑定manifest，复用`g_records.parse`逐点重验，
要求生产fixture标记全无、collection status passed、精确40点／10完整区组、同boot连续seq，
拒绝手工改cached record、缺点、重号、跨boot、failed或quiet未知。
任何失败保留原日志／状态；不丢弃失败点后继续拼成40点，也不自动补采或调gap。

论文主要比较按区组配对，保留10个独立区组值：

- V固定16MiB吞吐=16/(raw G word10/50000000) MiB/s；V自身issue→done周期。
- A固定16MiB吞吐只用raw word13对应自身whole时间；Solo A为n/a。
- V slowdown=本条件V own cycles／同block Solo V own cycles。
- gap1000/2000的限速效果以同block本G A8gap0为对照：V own cycles比与A whole throughput比均报告。
- 同时报V与A完整工作量、gap occupancy raw word15=4095×gap、软件whole elapsed；不得只报V改善。

窗口行为与whole吞吐独立展示：raw word16为V window，17/18为其中A issue/done，
19/20为边界A in-flight，21为strict carry-done，22为open-edge issue，23..26保留IDs。
A可在V前／内／后完成，空窗口不等于未完成A工作；不以V窗口作A16MiB分母，
不把carry和open-edge相加为新的请求或补全未观测工作。
raw27..30的own issue/done timestamp用于核对相对时序，不替代实际完整预算。

HPM两lane统计仍只覆盖旧非原子software V bracket，按原字段报告；
不把A HPM称为A whole counter或用软件V elapsed代替hardware whole time。
软件snapshot/drain开销单独保存，可报告，不能混进hardware吞吐定义。

PATH与hist沿用E5已验口径：16stage各保留full/carry/start/end、sum/max和32bin原计数。
均值sum/full只针对完整window事务；carry不自动填延迟。hist分位数只能报告所在bucket区间，
不把bin中点或上界当精确延迟。每条件独立重复数是10区组，事务数不是n；
若合并事务hist则明示权重、full分母与各区组范围，不与区组不确定性混称。

首版输出建议为逐点raw/derived表、10个配对区组ratio表、条件summary与16stage histogram表。
分布／区组统计复用已有E3/E5分析工具口径，具体summary/区间实现随下一分析小批交Root审。
本合同不预设G改善方向、幅度或显著性，不开展新gap、预算或机制搜索。
