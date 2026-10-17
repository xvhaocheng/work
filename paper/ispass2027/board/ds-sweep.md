# E1b：DS附加延迟四点曲线

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17
- Verification Status: ANALYZED（构建与实际100次采集已检查；未独立重跑）
- Version Label: ds_sweep_v1

用户本轮明确授权构建DS25、DS100并按顺序采集。已有DS0/DS50的正式100次保留不重测；
新两档每档Solo/A1/A8/A16/A64各10次，共新增100次。构建、启动、采集和统计分别记录，
未完成阶段不记作通过。[配套实验包](packages/ds-sweep-20261017/README.md)。

## 固定输入与执行

两档均复用[既有冻结源码](manifests/source-manifest.json)及[同一软件写卡回执](runs/baseline-software-write03/command-275284c8602d40c4a6a1712da181a211.json)。
平台/CVA6/IOMMU/Linux/OpenSBI commit见[本轮source](packages/ds-sweep-20261017/source.json)。
不修改生产checkout、约束或实现策略；导出的诊断顶层只将输入/输出延迟分别设为25/25、100/100。
四DMA、16项原PLRU、observer v3、批量供给和无研究扩展与DS50保持一致。
DS表示IOMMU数据结构访存通路的附加延迟，输入、输出各为横轴周期数，不是运行时开关或纯PTW成本。
DS0保留原直接连线；正延迟点启用诊断delayer，因此零点同时涉及该通路结构差异。

实际构建命令（两项独立并行，无自动重试）：

```sh
python3 paper/ispass2027/board/build_baseline.py fpga --variant ds25 --source paper/ispass2027/board/manifests/source-manifest.json --out iommu-platform/out/ispass2027-20261017-ds25-01
python3 paper/ispass2027/board/build_baseline.py fpga --variant ds100 --source paper/ispass2027/board/manifests/source-manifest.json --out iommu-platform/out/ispass2027-20261017-ds100-01
```

每个构建先保存manifest，包含固定源码、实际顶层/构建脚本、参数、命令和环境；
结束后记录BIT哈希、routed时序、routing errors、50MHz时钟和delayer网表单元数。
WNS必须≥0，setup/hold/pulse失败端点与routing errors必须为0；未通过即停止，不改约束或策略。
CPU时钟和delayer计数在同次构建收尾读取routed checkpoint，不另加上板验收步骤。

采集使用同一已写SD的`6.5.0-rc2-ispass2027-pathv3`，只切BIT，不重写或回读SD。
先DS25全部50次，再DS100全部50次；每档十区组，区组内条件循环移位。
V=DMA0、A=DMA1，V工作集1页，4KiB×4096，预热256，与已有正式基线一致。
保留采集器内置数据/完成/排空、boot/内核及16条同sequence PATH检查。
失败原样保留并停止，不补采、不挑样本。

## 统计口径

硬件时间来自原始cycles/50000；每试次吞吐为16MiB除以硬件秒数。
分别报告十试次的中位数、Q1、Q3与IQR，四分位数采用线性插值（type 7）。
曲线采用同一DS、同一区组的`T_V(Ak)/T_V(A1)-1`，每点十个配对比值取中位数，
误差棒为Q1到Q3，不是置信区间。DS50的历史前后两段按原block 0—4/5—9配对，
如实列出两个boot，不将十个配对称为十次独立重启。
旧报告的“两个中位数之比”保留原口径，不与本曲线的“区组配对比值中位数”混用。

完整原始命令、load、campaign及结果见本轮独立runs；
统一长表通过增量导入器重建，曲线和四档五条件统计由同一分析脚本产生。

## 完成结果与证据

两档构建均通过，均50MHz；WNS均+0.177ns，WHS分别+0.048/+0.045ns，
WPWS均+0.062ns，setup/hold/pulse失败端点与routing errors均0。
Delayer网表单元数120/145，无研究单元。原始[DS25构建](packages/ds-sweep-20261017/metadata/ds25/build-manifest.json)、
[DS100构建](packages/ds-sweep-20261017/metadata/ds100/build-manifest.json)记录全部输入及BIT哈希。

[Campaign](runs/ds-sweep-20261017-01/campaign.json)为completed，
[有限任务回执](runs/ds-sweep-20261017-01-launch/result.json)returncode=0。
DS25→DS100各50次、五条件各10次，失败0、重试0、补采0，未重写SD。
实际boot为`0a1ae461-d2a5-4b43-8580-4544f313e058`、`5cc1560a-b07e-4956-b1f2-e95e3cced13b`。
结束时间为2026-10-17 22:35:10 JST，结束时保持DS100/原pathv3，
[UART释放回执](runs/ds-sweep-20261017-01/board-release.json)已保存；本会话此后没有板上操作。

[新导入包](../data/board_imports/ds-sweep-20261017-01/summary.json)确认100个唯一实际试次，
`ds_sweep.complete=true`，无失败/重复或缺项。新增10140个标量行，
实板长表现共27544行；旧基线与G1不改写。来源重建检查通过。
四档共200次、120个区组配对比值，完整硬件时间/吞吐的中位数、Q1/Q3/IQR及源行ID在
[统计报告](../data/ds-sweep-summary.md)和[JSON](../data/ds-sweep-summary.json)。
作图[入口与依赖](../analysis/README.md)保存重建命令，PDF/PNG/SVG共9份；
报告及图的逐字节重建检查通过，三张PNG已由Codex目视核对。

| 条件 | DS0额外慢化 % | DS25 % | DS50 % | DS100 % |
| --- | ---: | ---: | ---: | ---: |
| A8 | 10.3962 | 24.3199 | 44.7831 | 76.0753 |
| A16 | 13.7155 | 31.4304 | 52.0975 | 88.7220 |
| A64 | 16.3512 | 36.8189 | 57.6313 | 98.1181 |

分段斜率（额外慢化百分点/附加周期；0→25、25→50、50→100）分别为：
A8=0.557/0.819/0.626，A16=0.709/0.827/0.732，A64=0.819/0.832/0.810。
**初步描述：A64接近线性；A8/A16在高延迟段边际增长趋缓，但尚未形成平台式饱和。**
这只是四个实测配置的描述，不是总体显著性或纯PTW成本归因；DS0与正延迟档的通路结构差异保留。

本轮构建和采集无失败。协调消息曾报告另一个入口在UART锁处被拒绝；
该入口不属于本campaign，未纳入本轮100次或宣称为额外测量，本会话未对其做重试。
本任务没有使用DeepSeek；Codex完成构建、采集、数据分析及验证。
