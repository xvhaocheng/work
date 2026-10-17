# G独立FPGA构建入口

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-18 JST
- Verification Status: PASS（唯一原流程BIT完成，原timing/route/routed边界门通过；未做G板测）
- Version Label: e6-g-build01

Root已直接审阅最小顶层diff、平台pins/profile、完整builder及合成检查，并授权冻结后立即启动唯一构建。
前端与observer已接受的有限验证没有重跑。后续这一次真实FPGA构建已完成SoC synthesis并通过结构门，
实际回执及完整primitive inventory见`synth-stage01/`，不沿用有限wrapper测试作为全系统证据。

| 对象 | 精确身份及范围 |
| --- | --- |
| CVA6 | `cfb9a0a8bb70b32be6036bc112a5189d6e2aa0ab`，parent `0f0403f56aff3f1ee10e18dccea7a9fd6f2f26a4`；仅顶层默认off参数及向peripherals传递 |
| 平台 | `d5cc44834a89d8e1633a028ad82d6e1f26b38831`，parent E5 `2f19f6c2de5530760f521bf771b6217066bc54d5`；精确CVA6/Linux gitlink、lock、独立G DS0 profile |
| IOMMU | `85b157ebe71a623e326e51fff7b7126fe77e8eb9`，未改；DS0直接wire |
| Linux | `65b427946152e3aca27babab0a437821729805d4`只作本BIT的reference，`g_linux_present=false`；后继69d G软件独立归档，不改写本记录 |
| 结构 | Depth4、BTC2、OBS4、16×32桶/32bit hist；不合BTC3/OBS5/M/P/N/PN |
| 构建 | 2026-10-17T15:08:57.646218Z实际启动；同一session43138于16:34:36.952322Z完成，exec exit0 |

`synth-stage01/partial-stage.json`为该同一构建的只读阶段快照：实际fileset generic=1，
G observer1、OBS observer1、PATH stage16，CPU20ns，DS delayer及禁止研究cell均0；
每stage head/tail各2FF、stamps128FF/4slot、hist1024FF/32bins，全16stage通过。
原完整4.7MiB register inventory保存，实际输入/约束/策略没有修改或重试。
该阶段快照是原件，保留其partial状态；最终同次构建资格见下文`qualified01/`，不追改阶段回执。

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
E5 Slice99.28%仅作为面积风险，不沿用其通过结论。失败停止规则保持；后继Linux/CLI已独立接受及构建，板上G40由Root持有实际硬件再执行。
本批由Codex完成，未调用DeepSeek。


## 唯一G BIT最终资格

[qualification](qualified01/qualification.json)、[原最终manifest](qualified01/build-manifest.json)、
[完整原build.log](qualified01/build.log)、[原routed-boundary](qualified01/routed-boundary.json)
与[最终builder receipt](fpga-ds0-build.json)保存；没有新构建、重试或改约束/strategy。
同一session43138 exec exit0，完成时间2026-10-17T16:34:36.952322Z。
原manifest SHA256 `9f1c9e90554310b9e263cd5886ca91d234c1ffaabc590d222abbbc818b46539f`。

| 最终门／资源 | 实际值 |
| --- | --- |
| setup WNS / TNS / failing | 0.177ns / 0 / 0 |
| hold WHS / failing | 0.036ns / 0 |
| pulse WPWS / failing | 0.062ns / 0 |
| fully routed／routing errors | 295368/295368／0 |
| routed CPU／DS delay／forbidden research | 20ns／0／0 |
| Slice | 50808/50950，99.72% |
| LUT／FF | 168566，82.71%／144581，35.47% |
| BRAM Tile | 50/445，11.24% |
| 原DRC | 0 Errors、228 Warnings；原报告完整保留 |

原[timing](qualified01/ariane_xilinx_timing_summary_routed.rpt)、
[route](qualified01/ariane_xilinx_route_status.rpt)、
[DRC](qualified01/ariane_xilinx_drc_routed.rpt)、
[placed utilization](qualified01/ariane_xilinx_utilization_placed.rpt)、
[hierarchical utilization](qualified01/ariane.utilization.rpt)及CDC/clock/methodology/bus-skew原件同目录保存。
这套原门不等于完整IO/CDC signoff，manifest仍明确full_io_cdc_signoff=false；不沿用E5资格。

实际BIT本机路径：
`/home/xvhaocheng/work-a1-e6-mitigation/paper/ispass2027/exp/E6-mitigation/artifacts/g-ds0-build01/src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit`

11443722B，SHA256 `8dd6993f8d75a813758a2d92e6786d4fdbb1d7372c5ab2720dc03f431430a6cc`。
实际MCS／DCP保留在相邻原目录；binary不进Git，未触硬件或写SD。
actual generic／最终synth回执已复制至qualified01；完整primitive inventory与先前
`synth-stage01/e6-g-synth-registers.tsv`逐byte相同，qualification记录两路径／同SHA，避免重复原件。
Root直接复核本次原报告后独占实际load和G40，不把BIT构建完成写作板测通过。
