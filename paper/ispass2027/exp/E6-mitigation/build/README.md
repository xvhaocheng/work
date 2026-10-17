# G独立FPGA构建入口

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-18 JST
- Verification Status: PARTIAL（source及真实合成结构门通过；BIT实现中，未最终验收）
- Version Label: e6-g-build01

Root已直接审阅最小顶层diff、平台pins/profile、完整builder及合成检查，并授权冻结后立即启动唯一构建。
前端与observer已接受的有限验证没有重跑。后续这一次真实FPGA构建已完成SoC synthesis并通过结构门，
实际回执及完整primitive inventory见`synth-stage01/`，不沿用有限wrapper测试作为全系统证据。

| 对象 | 精确身份及范围 |
| --- | --- |
| CVA6 | `cfb9a0a8bb70b32be6036bc112a5189d6e2aa0ab`，parent `0f0403f56aff3f1ee10e18dccea7a9fd6f2f26a4`；仅顶层默认off参数及向peripherals传递 |
| 平台 | `d5cc44834a89d8e1633a028ad82d6e1f26b38831`，parent E5 `2f19f6c2de5530760f521bf771b6217066bc54d5`；精确CVA6/Linux gitlink、lock、独立G DS0 profile |
| IOMMU | `85b157ebe71a623e326e51fff7b7126fe77e8eb9`，未改；DS0直接wire |
| Linux | `65b427946152e3aca27babab0a437821729805d4`只作reference；未构建或实现G软件，`g_linux_present=false` |
| 结构 | Depth4、BTC2、OBS4、16×32桶/32bit hist；不合BTC3/OBS5/M/P/N/PN |
| 构建 | 2026-10-17T15:08:57.646218Z实际启动；session43138，尚无BIT验收 |

`synth-stage01/partial-stage.json`为该同一构建的只读阶段快照：实际fileset generic=1，
G observer1、OBS observer1、PATH stage16，CPU20ns，DS delayer及禁止研究cell均0；
每stage head/tail各2FF、stamps128FF/4slot、hist1024FF/32bins，全16stage通过。
原完整4.7MiB register inventory保存，实际输入/约束/策略没有修改或重试。
当前仅实现阶段进行中；尚未通过最终placement/routing/时序/面积，不是BIT验收或板测。

`g-fpga-source.json`冻结所有递归组件及top-generic/synth-verifier overlay哈希。
`build-entry.json`冻结本批代码、恢复材料哈希与未执行边界；其`source_checked_no_build`是启动前记录。
`dispatch01.json`另记随后实际启动，不改写原冻结回执。
`cva6-g-build.patch/.bundle`从0f0403f起；`platform-g-build.patch/.bundle`从E5平台2f19起；各自verify原件同时保留。
内部组件只保存本地分支/bundle，不推第三方上游。

构建命令在独立worktree `/home/xvhaocheng/work-a1-e6-mitigation`执行：

```text
python3 paper/ispass2027/exp/E6-mitigation/build_g_fpga.py --out paper/ispass2027/exp/E6-mitigation/artifacts/g-ds0-build01
```

实际manifest/log位于该output；输出已存在后入口拒绝复用。没有重试入口、硬件、UART、JTAG或SD动作。
binary只保存在本机artifacts，不进Git。此次未修改E5或A2冻结归档。

`source/build_baseline.as-reused.py`逐字节复用E5已用完整builder；routed verifier也逐字节保留。
实际导出的`run.tcl`只新增：fileset generic `IOMMUGapEnable=1`及真实pre-synth回执；
`open_run synth_1`后执行结构检查。原50MHz约束、retiming、RuntimeOptimized place/route均不变。
新源码默认G关闭，启用只发生于该独立构建export。

`verify_g_synth.tcl`要求真实合成G observer存在、observer及16个PATH stage存在；每stage两位head/tail、
四个32bit stamps、32个32bit histogram桶，并保存实际sequential primitive清单。
同时要求CPU20ns、DS delayer和禁止研究cell均0。OBS4版本绑定冻结decode和实际合成hierarchy，
不冒称设备MMIO读取。若工具优化不能保留这些可核验结构，本次停止并保留原失败，不放宽门限重试。

原最终门保持WNS≥0、setup/hold/pulse failing均0、routing errors0及routed CPU20ns/DS0边界。
E5 Slice99.28%仅作为面积风险，不沿用其通过结论。失败停止，完整G数据工作及Linux/CLI仍需后续小批审阅。
本批由Codex完成，未调用DeepSeek。
