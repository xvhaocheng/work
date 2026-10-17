# E5 被动分段直方图

当前没有E5板测或论文分布数据。已接受的真实RTL有限验证、驱动真实函数host测试与RISC-V driver.o均通过；
唯一活动完整DS0构建在本工作树 `artifacts/e5-ds0-build01/`，最终实现验收仍待完成。
设计/原文采样口径见 `design.md`，组件patch与最小增量bundle见 `metadata/`，构建/取消修正入口见 `build/README.md`。

生产来源为已接受的P0/multi，保留Depth4、原错误检查与P0 frontend字节。
Linux小批供Root与E4整合统一payload；A1不访问硬件。

## 采集端接口

当前板时方案为E4的20个4KiB/2MiB同负载点附加hist，A3的新E4 collector调用下述纯helper。
不排专门E5的50点。区间分析合同见 `analysis-design.md`。

备选独立入口是 `collect_e5.py`，只为本批E5 OBS4/Depth4 DS0 BIT附加hist验证。
它调用当前工作树的既有 `board/collect.py`，固定沿用原 `ds0-all` 计划：
Solo/A1/A8/A16/A64各10次、N4096/W256、4KiB，不增加或改变DMA命令，不加载BIT/写SD/自动重试。
普通DS0/DS25/DS50/DS100及历史v3仍调用原collector，不受本模块影响。

完整构建验收与共同软件包发布后，Root在自己的工作树调用：

```sh
python3 paper/ispass2027/exp/E5-latency/collect_e5.py \
  --package E4_E5_FINAL_PACKAGE \
  --load-manifest E5_VERIFIED_LOAD_JSON \
  --fpga-manifest E5_FINAL_BUILD_MANIFEST_JSON \
  --output paper/ispass2027/board/runs/E5_NEW_RUN/ds0-all \
  --check-only
```

三个大写参数必须换成Root已发布的共同包目录、已验证当前boot的load.json及本次最终FPGA build-manifest.json。
`--check-only`只读主机回执，成功后去掉该选项即进入原collector；`--port`与显式`--resume-run`按原接口透传。
默认board-dir是运行脚本所在工作树的 `paper/ispass2027/board`，也可显式 `--board-dir` 指向Root实际入口。

身份门复用现有manifest：共同包status/kernel/source.json五个完整commit；package ds0_bit hash必须同时等于
本次已验收CVA6 `6c5c666d45979d3a03fb98cff89ba7ad095f6125` 构建产物hash与已验证load/boot的BIT身份。
要求50MHz、WNS及失败端点/routing/routed边界通过、OBS4/INFO1/Depth4/16×32×32；旧v3或后续OBS5/Depth64明确拒绝。
BIT/Image/payload不重复hash。运行manifest保留原collector身份，并附wrapper/validator源码hash和所复用回执的路径/hash；
新raw、records和manifest都写入Root显式新output，证据不会跨根写进A1。

`hist_records.py` 是纯解析/验证模块，没有串口、JTAG、执行器或共享表写入。
Root现有每种负载解析器仍先验证CLI、实际BIT/Linux/load身份、boot、taint、sequence、数据/排空和完整工作量；
然后在接受并保存该试次 **之前** 调用：

```python
item = existing_workload_parser(raw['stdout'], ...)
item['histogram'] = hist_records.attach_from_output(raw['stdout'], item)
```

这里沿用现有 `item['sequence']`、`item['observer']`、`item['path']` 字段，旧4KiB、P0和multi可以共用此附加校验，
但各自完整工作量/角色验证必须仍由原解析器负责。旧v3没有hist信息会明确失败，不能静默退化为E5合格样本。
必须使用显式 `CASE_EXIT=0` 后、`TAINT_AFTER=0` 前的完整dmesg转储；不从可能截断/交错的实时console拣选行。

验证要求同一实际sequence的v4 info、16段×32个有界桶、全部hist及最终batch/PATH有效、sum(bins)==full，
且实际PATH sum/max落在桶边界允许范围；缺段/缺桶/错序列/冲突重复/overflow/格式失配均停止。
历史sequence不混入，等值重复只保留一个stage且单独记重复数，不增加样本。
新驱动逐段流式打印；后段失败时早先行可能仍是valid=1，因此最终batch/PATH全局有效是强制接受条件。

`bin_rows(item, item['histogram'])` 返回一条已接受试次的512个桶行，沿用boot/sequence、case/block/condition、DS和原始命令路径，
附cycle上下界、真实count、full/carry/start/end及PATH sum/max。它不写文件，也不创建新身份体系。
调用方把这些行写到E5任务内输出，并关联既有冻结run snapshot/manifest；不得覆盖历史快照或写共享data表。

桶count是交易数，不能作为独立试次n。必须明确实际试次数、完整性与窗口截断；CDF来自真实桶计数，
分位数只能给桶边界范围，不能把PATH均值造作p95/p99或相加各段分位数。50MHz下每周期20ns。

## 有限验证入口

`analyze_hist.py` 仅用标准库。对E4已冻结且完整的20试次输入，Root可执行：

```sh
python3 paper/ispass2027/exp/E5-latency/analyze_hist.py \
  --records E4_FROZEN_RUN/records.json \
  --manifest E4_FROZEN_RUN/manifest.json \
  --source-root ROOT_OF_FROZEN_EVIDENCE \
  --group-by mapping_page_bytes \
  --output paper/ispass2027/exp/E5-latency/analysis/E4_NEW_CHECKPOINT
```

相对原始证据路径均从显式source-root解析，绝对路径必须仍在该根内；输出不得已存在。
部分进度须先冻结derived snapshot、同步真实recorded_cases，再加`--allow-partial`，报告actual n/complete=false。
逐试次16段结果在`trial-stage-intervals.csv`，真实512桶长表在`histogram-bins.csv`，跨试次端点统计在`summary.json`，
原始manifest/records/load/command JSON的哈希入口在`analysis-manifest.json`。不写共享data、不覆盖快照。

纯函数 `rebind_trial(raw, item, expected_kernel)` 从原stdout重取同`item['record']['schema']`的完整CLI JSON、
最终time/axi_v/axi_a和16条PATH、512桶并与缓存逐字段核对；支持E4外层包装schema而不硬编码CLI schema。
`trial_intervals(item, attachment, context)` 产生每stage nearest-rank区间及原sum/full/max；
`describe_trials(rows, group_fields)` 按显式配置和role/channel/stage统计真实试次数，默认不pool A。
nearest-rank和跨试次Type7口径分开；保留mean整数分子/分母，零full的n_valid=0且分位区间/mean为空，
合法原max=0仍保留。分位边界/均值的跨试次统计都不能解释为合并请求分布。

公开CLI不提供fixture放行选项。测试仅用显式内部fixture参数，生成的临时报告永久标记fixture_only且测试后删除。
递归继承manifest/load/raw/cached/decoded CLI任意层级的明确fixture或合成来源标记；内部报告与全部行/组均标记fixture并有source_board_cases=0。
E4使用与A3锁定的20个dict计划点(block/condition/mapping_page_bytes)，case及三字段必须逐条匹配计划前缀，
页大小同时重绑原CLI input.mapping_page_bytes，不允许对调或重标条件。
`test_analyze_hist.py` 当前九项检查涵盖raw重绑/缓存篡改、最近秩/末桶、空full/carry/精确样本mean、
Type7/真实n、重复trial拒绝、完整任务内CSV、各层级递归fixture拒绝/传播和E4计划/三向映射绑定；
主证据`analysis-validation04.log/.json`含实际REJECTED原因和对应源码哈希，03保留前一通过，旧`analysis-validation.log`保留f36版本，
`analysis-validation02.log`保留新增拒绝门已生效但旧单测诊断文字正则未同步的失败，未抹去历史。
`protocol-compat01/receipt.json` 是A3实际冻结E4 collector/CLI协议的一次端到端兼容：
leaf4k/leaf2m/closing_spill三个显式synthetic样本全部通过内部raw重绑，公开全部拒绝；
以A3真实20点计划对一个内部fixture prefix生成16段/512桶报告，全部fixture且source_board_cases=0。
完整原协议源码/CLI/fixture/分析输入与输出都保留，未重复执行A3驱动/CLI回归，也未做板测。

`rtl-validation/runs/run03/`：真实生产stage/path/observer，923拍全部旧字段等价、18集成场景、98年龄边界等。
`driver-validation/runs/run02/`：48项真实函数host-MMIO fixture，`driver-validation/object01/`：RISC-V目标编译0warnings。
`test_hist_records.py` 的10项测试以明确标为fixture_only的实际驱动mock输出为输入，正向及错误门均通过；
原始输出见 `parser-validation.log`。这些host日志与512行生成检查不是板测，也不进入论文数据。
`test_collect_e5.py` 的4项host fixture检查覆盖旧工作量参数原样传递、仅主机check-only、错误BIT/kernel/source/版本/depth及未验收时序拒绝，
以及采集manifest的包装器/validator来源记录；原始输出见 `collector-validation.log`，没有导入或执行硬件控制。
