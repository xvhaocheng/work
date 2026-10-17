# 使用实例：ISPASS 2027 G1与统一基线

本页是通用上板skill的一个实验实例，只有当前任务要求G1复现或该投稿基线时才用。
它不限制其他未来实验，也不因读取本页而重新启动已完成的实验。

实际身份、命令、运行进展和结果以
[G1与统一基线报告](../../../g1-reproduce.md)、
[构建说明](../../../BUILD.md)及对应实验包为准；不在此重复数值结果或哈希。

成套入口：[G1包](../../../packages/g1-20261017/README.md)已保存通过的独立九次结果及失败/恢复链；[baseline100包](../../../packages/baseline100-20261017/README.md)的正式100次全部完成，两DS下五条件各10次，[campaign.json](../../../runs/baseline100-20261017-01/campaign.json)为completed、launch returncode 0。该流程已实际执行完成。每包manifest关联结果、`source.json`中的commit、`metadata/`恢复材料，以及本机`artifacts/`中的image/BIT；Git保存元数据、结果和恢复材料，二进制本机留存。实际写SD使用`fw_payload.bin`，单独Linux `Image`是组件。G1历史dirty构建的完整执行树单一commit仍为`unknown`，不以当前HEAD补齐。

执行沿用已有流程直接传输、更新、启动和采集；配套导航不增加重复哈希、SD回读或独立确认，也不重跑已完成的G1。

该实例约定：

- G1使用历史batch DS50 BIT和00082软件，Solo/A1/A8各3次；参考硬件时间为
  56.5/107/155ms，允许中位数相对偏差±2%。已完成G1不默认重跑。
- 新基线使用独立修复底座、批量供给与被动PATH；DS0直连、DS50为附加延迟诊断，
  两者共用同一新payload。该版本关系不推广为所有实验的软件兼容保证。
- 后续默认AB：DS50十区组共50次全部完成，再切DS0十区组共50次；每个BIT
  只加载一次，两配置共用已写好的同一payload，切换BIT不重写SD。各区组包含
  Solo/A1/A8/A16/A64。G1九次不计入正式矩阵。
- 本轮已完成的[campaign](../../../runs/baseline100-20261017-01/campaign.json)
  保留实际ABA：DS50前25、DS0中50、DS50后25，三子run均passed，对应三次boot；
  无新增失败或补采，也未因默认流程改为AB而重测。
- 此采集器固定V=DMA0、A=DMA1、4KiB、4096次、预热256；要求本轮新sequence、
  数据与完成/排空检查，且新基线需要16条同sequence PATH记录。
- 默认[run_baseline100.py](../../../run_baseline100.py)依次调用
  `ds50-all`、`ds0-all`各50次，无自动重试。
  板仍处于本轮[最后有效DS50 boot](../../../runs/baseline100-20261017-01/ds50-last-load/load.json)时，
  可传`--initial-load-manifest`复用该启动记录，省去首次编程/启动，不重写SD。
  `collect.py`也保留`g1`、`ds50-first`、`ds0-middle`、`ds50-last`的历史兼容；
  `--resume-run`仅用于该采集器的适用恢复场景。其他实验使用自身命令和采集器。
- 历史G1的±2%性能门槛不适用于新基线或其他未来实验。

设备数、负载形态、传输大小、页大小和延迟分布的扩展在该投稿任务中只列待补，
是否执行由后续用户任务和当前规则决定。
