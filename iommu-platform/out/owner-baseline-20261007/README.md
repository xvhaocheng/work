# 普通准入强对照，2026-10-07

- [Codex预先设计与两次追加运行理由](codex-test-spec.md)
- [最终机制/创新审定](codex-review.md)
- [8/8验收](acceptance.json)、[比较](comparison.json)、[实际命令和JSON](runs.json)
- [源页重遍历事件](source-rewalk-evidence.json)、hot-events-analysis.json、cold-events-analysis.json
- [仅3个tests文件实际改动](actual-test-changes.patch)；before/保留原文件
- build-command.json / build.log / build-state.json；首次不支持输入默认值的elaboration失败在build-initial-*保留
- cpp-measure-only-build.log是追加flag的C++重编/链接，没有再次生成SV模型
- production-source-hashes.json：被当前模型编译的生产SV前后hash一致
- evidence-task/answer/raw：DeepSeek接口证据；cpp-task/answer/raw及cpp-replacements.json：owner开关草案
- measure-only-cpp-task/answer/raw及measure-only-replacements.json：同预热的后级额度开关草案
- 各case.log / case-events.csv是原始结果，未覆盖之前阶段模型/数据

不是C实现，不是C性能失败；当前停止旧C首选创新推荐。普通walk方法已在本冷页模型保留FAST并略胜P。新颖性/通用进度/板端收益均未验收。

