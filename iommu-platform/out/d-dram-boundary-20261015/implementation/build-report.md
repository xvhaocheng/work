# Ramulator 2 私有host构建与执行草案

原版库、私有被动观测库和两个driver均已编译成功；所有模拟case执行数为0。原MIG1792023314继续paused。只写本implementation目录，未改生产Linux/RTL/UAPI或原模型参数。

请求模型/强度：gpt-6.1-sol / medium；实际运行元数据unknown。当前共享IOMMU任务Codex-only，未使用DeepSeek。

## 官方源和工具

- 原版源目录：`ramulator2-be93be78055d922aa1d4d33e15bcc8f2b0c61a9d/`；原库为该目录`libramulator.so`；官方exe为`build/ramulator2`。
- 官方精确commit archive：`https://codeload.github.com/CMU-SAFARI/ramulator2/tar.gz/be93be78055d922aa1d4d33e15bcc8f2b0c61a9d`；SHA256 `f6a79d68dbae0289a3d1a53a7df51ce9270307cabae70e68eef8b4c654b6b688`。165原始文件构建前hash完整记录于`receipts/source-prebuild.json`。
- 最初普通configure因本机无cmake而在子进程启动前失败（wrapper exit1、CMake exit不适用）；root随后授权恢复。
- 私有工具`tooling/cmake-3.28.4-linux-x86_64/bin/cmake`，来自官方Kitware v3.28.4 release。tar SHA256 `1f74731c80cbba3263c64fca6f6af0fb8dd1d06365425e404f79564773080d11`与同release SHA-256.txt一致。无系统安装、无全局环境更改。URL/实际版本/交叉hash见`receipts/cmake-release-download.json`、`cmake-checksum-download.json`、`cmake-tool.json`。
- 实际g++11.4.0、make4.3、git2.34.1等完整版本见`receipts/tools.json`。
- 官方CMake固定依赖及resolved commit：yaml-cpp0.7.0=`0579ae3d976091d7d664aa9d2527e0d0cff25763`，spdlog1.11.0=`ad0e89cbfb4d0c1ce4d097e134eb7be67baebb36`，argparse2.9=`997da9255618311d1fcb0135ce86022729d1f1cb`。依赖构建前全部文件hash见`receipts/dependencies-prebuild.json`。

## 实际编译结果

- 原版configure恢复exit0，4.08s；一次正常build -j2 exit0，146.85s。库SHA256 `a816b3cc3850bd50cc4f28aed9998c1b6d130d4244e428b39883cee1be8d83cf`。完整日志、命令、工具退出码及artifacthash在`receipts/configure-recovery.*`、`build.*`、`original-build-artifacts.json`。
- 私有源在`instrumented-source/`。只增被动audit：5队列、cap、刷新时钟和next、DDR4各层state/rowmap/ready/history/future只读快照；priority_send原接受结果、issue真实返回后、REFab_end状态更新后事件。未改原条件、调度、队列容量或时序值。其构建前792文件hash见`receipts/instrumented-prebuild.json`。
- 私有configure禁FetchContent联网更新，使用私有复制的同一固定依赖，exit0；一次库build --target ramulator -j2 exit0，146.66s。见`receipts/instrumented-configure.*`、`instrumented-build.*`。
- driver先出现yaml-cpp iterator_value不能直接赋Node的编译错误，改Clone后又暴露YAML ostream/Clone未由动态库导出的链接错误；最终显式链接该官方build生成的yaml-cpp静态库，两个driver均exit0。全部真实尝试分别保存`receipts/driver-baseline-compile.*`、`driver-baseline-corrected-compile.*`、`driver-baseline-final-compile.*`、`driver-instrumented-compile.*`。未以失败尝试代替成功声明。
- 原165文件及私有构建前全部源/依赖文件编译后hash均未变；标准unified patch包含5个既有文件和2新增audit文件，原CMake末尾无换行标记已处理，`patch --dry-run` exit0。见`receipts/patch-dry-run.*`。

## 待root审查的执行资产

- `boundary-driver.cpp`：同一源编译原版baseline（无audit符号依赖）与私有instrumented两个driver；Factory/GEM5连接初始化；直接memory.send/tick，不用is_finished。输入来自冻结contract，只尝试一次send。callback只计唯一ID/metadata，统一完整tick返回后判断drain；校验所有有clock的component与driver tick一致、next_refresh>当前clock、五队列/future/Refreshing为0且当前已接受请求全callback。
- `boundary-config.yaml`：冻结DDR4_8Gb_x8/DDR4_2400R、1channel/2rank/64bit、Generic/FRFCFS/AllBank/ClosedRowPolicy cap4/RoBaRaCoCh/clock1；原TraceRecorder保留。只为输出路径替换插件path。
- `run-cases.py`：root明确放行后才使用`--execute-root-reviewed-contract`。固定8instrumented+1original history_cap_warm，无额外输入/扫描；每进程30s、首失败停止不重试；冻结合同hash校验，执行前绑定源输入/实际两库和driver，保存stdout/stderr/退出码和每case文件hash。不应在本席调用。
- 输出是`events.yaml`。fullstate在cold、每次send前后、drain转换、REFabissue/end边界、final；其余post_tick保留5队列/clock/next/future_count/refreshing。逐请求send与callback带id/address/role/send_after_tick，顶层epoch cold-zero。原TraceRecorder为`official-trace.ch0`。
- 最终完整执行前绑定：`receipts/pre-run-manifest.json`，含原模型、私有源码、依赖、driver/config、冻结contract/predictions、真实库和driver hash及各次构建收据。首次baseline draft hash是在编译进行中记录，已明确说明；最终修正源码hash已在成功编译前固化于`driver-before-final-compile.json`，两库各自source/dep hash均在编译前固化。

当前结论仅为构建完成与实现准备；没有任何DRAM残留、洗去或passivity执行结论。root须先审查最终patch/driver与manifest后决定运行。
