# MQSim 固定版本私有构建报告

结论：官方固定提交归档获取成功，原样源码在本机完成一次 host-only 构建，退出码 **0**，耗时 **11.184 秒**；没有运行模拟器。

- 固定来源：CMU-SAFARI/MQSim，commit `51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16`。
- 官方归档 URL：https://codeload.github.com/CMU-SAFARI/MQSim/tar.gz/51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16。下载命令、时间、退出码见 `download.json`，原始输出见 `download.log`；curl retry 为0，仅一次获取。
- 原始归档：`MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16.tar.gz`，474013 bytes，SHA256 `503fbdde978338f43349d94128d9f2731c470f2c3f5f2408339ab529e614472b`；另见 `archive.sha256`。
- 提取目录：`MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16/`。
- **在构建及任何修改之前**，完整记录提取出的175个原始文件：`original-source-manifest.json`（含每文件大小/hash及记录时间）、`original-source.sha256`。构建后再次对照，原始文件改变0、缺失0。未修改源码、Makefile或配置。
- 执行目录为提取目录；完整构建命令为 `timeout --signal=TERM --kill-after=5s 180s make -j2`。开始 `2026-10-15T12:56:40.895160+00:00`，结束 `2026-10-15T12:56:52.078924+00:00`。
- 编译器为原始Makefile指定的g++；必要环境、host平台、实际命令与版本检查退出码见 `build-environment.json`；完整工具版本分别见 `gpp-version.txt`、`make-version.txt`、`timeout-version.txt`。未安装依赖。
- 完整stdout/stderr合并日志为 `build.log`。`build-result.json`保存退出码、时间及原始文件完整性比较。完整日志中检索 `warning:` / `error:` 未发现匹配。
- 新生成62个文件（对象文件与可执行文件），逐文件大小与hash见 `build-artifacts.json` / `build-artifacts.sha256`。可执行文件 `MQSim` 大小13916568 bytes，SHA256 `a1b539dad78d82c4b3ee26b1371f6c92b635eaddb2c1a45eac64b87a09ca6015`。

此次只做源码获取、原始清单记录、构建和构建后hash检查；没有执行MQSim（包括help/smoke run），没有改配置、instrumentation、模拟实验、原生DMA/FPGA操作，没有修改生产Linux/RTL/ABI，没有恢复旧实验或MIG1792023314。后续运行与边界取证由root另定合同。本席全部写入限定本 `offline/`，canonical文档留root统一更新。

模型归属：本子任务 requested 为 `gpt-6.1-sol` / `medium`；工具没有提供可核验的实际推理运行元数据，actual model / effort 均为 **unknown**。共享IOMMU Codex-only，本次未调用DeepSeek或其他外部模型。
