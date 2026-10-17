# E1b：DS附加延迟四点曲线

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17
- Verification Status: ANALYZED（构建通过；板测未执行）
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

## 当前状态

两 BIT 构建已通过，均为50MHz、WNS +0.177ns，setup/hold/pulse失败端点和routing errors均0。
DS25的WHS/WPWS为+0.048/+0.062ns，DS100为+0.045/+0.062ns；routed delayer单元数120/145，
禁止研究单元均0。实际清单与原始报告保存在配套包metadata/ds25和metadata/ds100；
包状态为ready_for_collection，本机已配套保存两BIT与共用payload/Image。
尚未启动DS25/DS100正式采集，未形成四点曲线。
两档冻结源码与DS50的差异已核对；17项采集/调度替身检查及6项独立导入检查已通过，
这不替代构建或板测验收。Root执行命令、独立长表和交接见
[E1b任务目录](../exp/E1b-ds-sweep/README.md)。
