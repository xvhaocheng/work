# §61：数据已经齐备才形成 WRITE firstVALID 的强普通对照

Codex架构：§60 peer字节进度准入10完整copy通过、source R仍256，各目标256W。F1 peer平均5600 vs旧共同6097.5；F128 peer5163.5 vs旧5657.5。但须排除普通data-first/store-and-forward qW，不把相对child-groups barrier约8.7%直接命名独立创新。

新shared_write_progress_i改2bit enum：0原共同groups网格，1保持§60 peer actual raw_written准入，2普通每个新qW必须有全部自身payload，即source_returned[f]/8>=q_next_end[f]，形成firstVALID前判断，已firstVALID后不撤回。两新策略都停用groups数比较、允许logical terminal R进入最多2拍private存量；q2和真实当前WRITE授权仍必须。mode2/read-frontier仍按各成功WRITE许可min裁sourceAR。策略2不需要peer反馈，不强求共同F/WR前沿裁WRITE child：尚无数据的V qW2不会占头挡A已resident qW1，A可先完成首原B建立次页许可；仍保留native每页actualAW合法。

强对照同B3/qW2、相同独立授权/sourceR256、normal属性、可开/关普通WR预取。策略2保证每个已firstVALID W头的数据已经存在；策略1通过peer消费信用可提前形成W且在R库存到2时证明旧W头数据闭包，可能更能隐藏AW/TC延迟。若两者同效同成本，peer规则不能独立立贡献；若提前承诺有持续进度/性能收益，须核credit/order-aware/collective最强前作和完整cost。

普通C++只改+write-progress接受0|1|2/default0，unsigned字段真实驱动2bit，不用bool ternary吞掉2。集中contract仍只natural/shared2048/physical-bypass2/keepq1/nohandover/pause，WRprefetch可0|1。JSON新增policy独立于bool opt_in，观察actual仍来自RTL qchild形成sticky。保留所有原copy/guard/ sourceR/WRITE权限/DONE验收；原0、1精确对应轨迹回归，不另造框架。仅out，未知动态失效/fault/多job/未对齐/面积边界不变。
