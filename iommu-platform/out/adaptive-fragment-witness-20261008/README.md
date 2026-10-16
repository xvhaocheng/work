# §30：单 DMA 读写几何与候选 I 的收益反证

[审定报告](../../docs/post-translation-fragment-witness-20261008.md)／[阶段证据](stage-evidence.json)／[复用模型哈希](reused-artifacts.json)。

- 0 次 SV 构建，1 次 C++ 编译链接，2 次运行，两次 exit 0 / PASS。
- CONF0 源读 1024+1024，CONF1 源读 2048；目的均两段 1024。数据 R/W 均 256 拍。
- W2 physical AW 周期1456，first source AR1899，first R1931。候选 I 的晚准备目的前提在本场景不成立。
- 单 DMA V；A/CPU/batch/预热静默；512 拍普通缓冲。旧模型含 SameBankDmasEnable=1/ReverseReadOrderEnable=1，不能据此证明自然多 DMA 写序。
- I 未实现；没有资源收益、系统加速、生产等价或实板结果。

原始运行：[CONF0命令](conf0-command.json)／[日志](conf0.log)／[结果](conf0-result.json)；[CONF1命令](conf1-command.json)／[日志](conf1.log)／[结果](conf1-result.json)。
驱动：[DeepSeek补丁应用脚本](apply_geometry.py)／[实际实验副本](geometry_test.cpp)／[差异](geometry-source.patch)。
编译：[命令](cpp-build-command.json)／[日志](cpp-build.log)／[结果](cpp-build-result.json)。

独立设计及文献：[Codex候选I规格](codex-rule-i.md)／[新前作笔记](new-primary-notes.md)。
完整 DeepSeek 答复：[首轮](source-answer.md)／[修订](revision-answer.md)；对应完整原响应 source-raw.json / revision-raw.json，任务 source-task.md / revision-task.md。worker 没有执行命令或修改文件；实际执行与审定为 Codex。
