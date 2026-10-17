# 给 Root A 的数据接口说明（不修改其脚本）

2026-10-17最终只读核对：Root A 的
`exp/E1b-ds-sweep/import_board_runs.py:636`导出17列`board_results.csv`及
`summary.json`、`incremental_sources.json`、`manifest.json`；
`:637–645`的manifest已带输出hash、输入清单、fixture_only、缺项和
ds_sweep完成范围。`row_spec`约:493把系列写为`board_ds_sweep`，
与Root B图3系列名匹配。

**当前尚无新增接受板测需要导入。** 两端差一层显式比较基座与完成快照
接口：Root B `scripts/data_adapter.py`接收原17列加`base_id`，相邻
`analysis-import.json`绑定CSV hash、已有source/run清单和完成状态。
缺少它们时会明确不导入，不自动猜测基座或把进行中结果当整轮完成。

请Root A在论文可用快照交接时，提供以下信息；无需更改已存原件或
扩展实验身份体系，也不要求重复硬件/哈希采集：

1. 明确本次源码/软件比较基座。与100次完全相同的固定基座可用
   `baseline100-fixed-885ed0a7-45bf76b1-pathv3`；新的P0等版本应使用
   自己的base_id，不能因同是DS0/DS50就沿用。复用已有构建/运行清单
   作为依据。BIT变体与boot仍分别保留，不用base_id代替它们。
2. 在**派生交接副本**中为每个已有长表行加入同一个base_id，保留原
   row_id、原始操作数、身份未知项和所有原17列；原始17列表不覆盖。
3. 最后发布相邻receipt，格式见`merge-notes.md`。`results_sha256`
   绑定派生CSV的实际字节；source_manifest/run_manifest指向已有清单。
   receipt的complete只表示这个已收集快照写完、通过相应检查，
   不代表100点全齐；实际n、缺条件和阶段完成范围仍由原manifest保留。
   fixture_only或未通过/未保留点不能进入正式论文样本。

收到快照后，在Root B运行：

```sh
paper/ispass2027/analysis/build.sh --experiment-root /path/to/root-a/paper/ispass2027/exp
```

该命令只写Root B审查总表与图，不改Root A或共享canonical。已完成点
可中途同步，缺点仍明确留空；“四点曲线完整”与“快照可读”分别标记。
E5的100次阶段统计是同一批原始数据的另一视图，不新增试次，也不能
用来生成请求CDF。本任务没有修改Root A采集器或数据导出程序。
