# §75 Codex 源码与证据接受

Codex直接审读ShortPrefix状态/真实ownREAD许可/WRITE resident门/完整payload检查/原始terminal失败，运行四build及29DUT后接受当前窄夹具。DeepSeek给驱动与audit提案，由Codex应用并执行；两次验收口径错误（不存在pr CSV、末个raw父长度≠总长度）保留并已交DeepSeek修订，未放宽整作业字节检查。独立Codex关键席只审并发条件，无测试或文件编辑。

- 实际audit75.py exit0，29DUT：27完整copy PASS＋2旧route-not-bound终端拒绝，另7CLI非法不算DUT。
- 四build源码8+exe实际文件与SHA完整；新source与current一致，旧失败source/exe保留，不用被覆盖obj冒充旧版本。
- 每个成功CSV的source AR与rfirst/rlast完成匹配；实际逐R握手counter与len+1总工作量一致；目标W各256、logical r/W各2048，完整源/目标及未写区域CHECK保留。没有逐拍pr CSV，不宣称存在。
- 27PASS中18次ShortPrefix严格窗口均0。J8时A rawW闭合、V已r46是实际负例；不否定rawW未全闭合的更宽窗口或其他负载。
- QW66实例输出逐29项核对确为V0/A1；跨页F1时末A父255与整A作业256分开，未当成数据缺拍。
- run22/run23修复E=N后都independent/decided、R512；run24/06、run25/05、run29/上一阶段08的CSV逐字一致，旧metric没有额外差异（上一阶段SOURCE仅四新刺激字段）。

本阶段没有加入会员的RTL，没有性能创新证据。因为经典stream-merging/piggyback和pending read-combine已覆盖一般思路，不能把短补读单独作为贡献。如果更宽原子加入门在同权限/不可撤/总资源下有前作与强对照不能解释的能力与收益，才可形成机制贡献；理论条件见codex-mechanism-verdict.md。

新增functional FF1、observer FF134、payload0；未综合，不能声称总面积相同。无production/Linux/BIT/JTAG/SD/Mac或board动作。数据、快照、失败、提案和真实provider usage均保存。
