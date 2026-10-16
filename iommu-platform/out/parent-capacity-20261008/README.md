# §34 实际证据入口

- [结论与下一受控比较规格](../../docs/parent-capacity-boundary-20261008.md)／[Codex关键源审定](codex-review.md)。
- [汇总JSON](summary.json)／[汇总表](summary.md)：8份记录PASS，明确6 fresh＋2 reused-reference；480项条件全部通过、6份新CSV与对应8槽逐字节相同。汇总脚本退出0。
- 实际验收入口：`python3 out/parent-capacity-20261008/summarize-revised.py`（在平台根目录）。DeepSeek草案与局部修订、Codex同ID真实打印顺序检查；只读已有raw，不重新仿真。`summarize.py`保留为首版失败草案，非最终入口。
- [末W独立核验](endpoint-audit.json)：各容量V先17530／20213，A先镜像；reset-relative DDR数据端点，非DONE时长／稳态吞吐。
- 三个`build-depth{1,2,4}-command.json/-result.json/.log`：真实SV+C++构建exit0，分别6.31／6.32／6.33秒；旧depth8未重编／重跑。
- 六项新`depth{1,2,4}-{v,a}-32768-command.json/-result.json/.log/.csv`，两项`depth8-*`复用§33固定基准；reference标记和原始argv保留，路径指向原运行，不假造新调用。
- [派生RTL](posttl_owner_fragment.sv)／[系统副本](system_top.sv)／[准备脚本](prepare.py)；仅父physical保存槽深度改变，virtual source关联、AW环仍8，iDMA与TC容量保留。
- [生产76SV哈希检查](production-check.json)／[阶段与源码／binary SHA账本](stage-evidence.json)。无原tests／生产RTL／Linux／BIT／SD／板卡动作，保留原C0工程问题未应用生产修复。
- DeepSeek三次思考high/max393216完整返回，实际总usage793120 provider tokens（非Codex消费）；原任务／原答／原始response保存在worker-*、revision-*、final-*。DeepSeek未执行／写文件。Codex亲自设计关键容量测试、核源与AMD2009技术披露、应用脚本与最终审定。
- 失败记录保留：首版argv格式AttributeError，修订稿Markdown counts层级KeyError；均为汇总脚本，非RTL或DUT失败。Codex第一次Delete+Add同路径patch被工具拒绝后采用锚定围栏提取，没有丢失原稿。
- 独立创新仍未成立；preTL／postTL及预翻译强对照只形成规格，未实现／执行。本轮不是P/N/PN结果、四DMA/MIG或实板验证。
