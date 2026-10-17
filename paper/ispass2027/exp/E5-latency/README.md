# E5 被动分段直方图

当前没有E5板测或论文分布数据。已接受的真实RTL有限验证、驱动真实函数host测试与RISC-V driver.o均通过；
唯一活动完整DS0构建在本工作树 `artifacts/e5-ds0-build01/`，最终实现验收仍待完成。
设计/原文采样口径见 `design.md`，组件patch与最小增量bundle见 `metadata/`，构建/取消修正入口见 `build/README.md`。

生产来源为已接受的P0/multi，保留Depth4、原错误检查与P0 frontend字节。
Linux小批供Root与E4整合统一payload；A1不访问硬件。

## 采集端接口

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

`rtl-validation/runs/run03/`：真实生产stage/path/observer，923拍全部旧字段等价、18集成场景、98年龄边界等。
`driver-validation/runs/run02/`：48项真实函数host-MMIO fixture，`driver-validation/object01/`：RISC-V目标编译0warnings。
`test_hist_records.py` 的10项测试以明确标为fixture_only的实际驱动mock输出为输入，正向及错误门均通过；
原始输出见 `parser-validation.log`。这些host日志与512行生成检查不是板测，也不进入论文数据。
