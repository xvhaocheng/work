# Research v1：按用户要求跑完能运行的剩余项

2026-10-02，用户要求“80项能跑的先跑完”。复用现有Linux00090及本次research BIT启动，没有重载、重编译、修改驱动/RTL或实验参数，没有重跑前4项。

## 实际执行

- 临时副本 `/tmp/platform-research-tail80` 仅将原runner选择数改80、选择原ordinal5..84，其余完整计划校验、逐项CLI调用、退出状态、taint门限和采集保持不变。
- `/tmp/research-tail80-01`：ordinal5..66共62项完成（两项cost短试次＋60项E2主试次）；ordinal67 cost-b01-solo CLI exit1，Invalid argument，runner停止exit1、collection_failed0。
- 该Solo JSON显示errno22、stage0、raw_captured0、各phase submitted/completed0。未修改失败判定，未尝试修复。
- ordinal78/83是另外两次完全相同参数的主Solo，明确记为未运行，不冒称失败或成功。
- 第二临时副本 `/tmp/platform-research-pair15` 仅选择ordinal68..84去掉78/83，共15项，原参数与逐项判据不变。`/tmp/research-pair15-01` 15/15完成，runner/host exit0、collection_failed0。

本轮剩余80项的状态：77项CLI及原runner检查通过、1项失败、2项未运行。加原先3项通过＋1项失败，全84项目前80项在线通过、2项失败、2项未运行。E2为63项（短3＋主60），cost非Solo为17项（短2＋主15）。不宣称84/84完成或独立语义验收通过。

## 原始证据与边界

完整UART保留console.raw.log；两段所有JSON/stderr/exit/dmesg/冻结计划及实际临时runner均在collection归档。首次4项证据仍在此前 `out/mac-research-v1-20261002/`，不覆盖。原始日志含hrtimer警告，未清理或声称无内核警告。此次只运行并回传，没有性能分析或根因诊断。

本轮结果目标：Dell `/home/xvhaocheng/work/iommu-platform/out/mac-research-cont-20261002/`。Linux保持运行，主机UART采集结束。不改永久交付runner；临时runner保留其旧usage文字，其实际筛选范围以plan.selected.tsv和本说明为准。

按用户快速执行要求，Codex仅机械调整临时选取范围并执行已有工具；本轮未调用DeepSeek。
