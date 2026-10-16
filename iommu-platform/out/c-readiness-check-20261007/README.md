# C 可达性小判别：2026-10-07

- [Codex预先定义的假设/否证条件](codex-hypotheses.md)
- [Codex最终机制与创新审定](codex-review.md)
- [5/5验收、关键路径与hash](acceptance.json)
- [完整运行命令/日志JSON](runs.json)；[小比较表](comparison.json)
- [新冷目的四组离线事件整理](cold-events-analysis.json)；[首walk干扰精确事件](first-walk-interference.json)
- [旧两CSV修订整理](old-events-revised-analysis.json)；初版空尾段计数错误保留在old-events-analysis.json
- [实际单文件测试补丁](fixture-applied.patch)，原C++和旧binary均保留；[C++单次编译/链接日志](cpp-build.log)
- DeepSeek证据任务：evidence-task.txt / evidence-raw.json / evidence-answer.md
- DeepSeek指定夹具草案：fixture-task.txt / fixture-raw.json / fixture-answer.md
- DeepSeek定向修订：revision-task.txt / revision-raw.json / revision-answer.md；[补丁格式失败记录](patch-format-failures.txt)
- analyze_readiness.py为实际解析程序；initial/revised副本保留作者草案，实际版由Codex另纠正PTW bank诊断误用port名称。CSV不是完整run，离线整理不发PASS。

同目录各case.log、case-events.csv为原数据。仅测试侧变更；没有候选C实现、生产RTL/Linux、BIT构建或板卡操作。5次为路径门槛小测试，不是强方法完整比较或统计性能收益。

