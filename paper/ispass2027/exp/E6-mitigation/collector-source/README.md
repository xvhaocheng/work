# G collector／分析接口候选

实际代码为`../collect_g.py`和`../g_records.py`；[冻结源码身份](component.json)，
[最终源码副本](../collector-validation/run02/source/)，[100项有限检查](../collector-validation/run02/report.json)。
Root已直接审原候选并要求两处聚焦修订，最新实际源码仍为`../collect_g.py`；
[修订身份](revision01.json)、[实际diff](revision01-actual.diff)与[17项针对检查](../collector-validation/revision01/fixture-context.json)
已补充，待Root审。原100项及run02源码快照保留；未运行UART、板测或新G实验。

矩阵严格40点：10个block，每block恰有`solo/a8-gap0/a8-gap1000/a8-gap2000`各一次，
按block%4轮换顺序。只生成`idma_g --device /dev/idma0 --condition <固定值> --timeout-ms 60000`，
外层既有`platform-timeout 90`、Console.command120秒、整批5400秒上限。
无可变gap/预算、内部retry、补采、SD、加载、reboot或自动切BIT；没有prefix重新执行或隐含resume。
错误立即failed并保留已接受前缀与失败原始返回；不齐40不能作为最终数据。

正式运行前检查既有manifest，不扩展身份体系：

- Root实际load为boot_identity_verified，G软件g-v1 release、同boot，实际BIT hash等于passed FPGA artifact。
- software6491/Linux69d/CVA6cfb9 reference-only和已接受driver hash；BIT本身仍保留其原d5cc/65b reference身份。
- FPGA passed、50MHz DS0直连、BTC2/OBS4/Depth4/hist，以及actual generic和合成后G/PATH/hist inventory。
- COPY4096复用旧已passed P0 backend证据的两次AR/AW；不把fixture当backend或板测证据。

每点复用现有KERNEL/BOOT/TAINT/PRE_SEQ→CLI→CASE_EXIT→dmesg→TAINT_AFTER边界。
**先保存完整Console返回为raw-NN.json，再验证**；不吞掉失败CLI、raw status、错误或未知quiet。
要求一次CLI JSON、完整ABI6 input/echo、CAPS、双方W256实际完成字节、各4096/16MiB完成、
prepared/quiet/verified mask与retained0、真实data_errors/quarantine0；HPM只标原software V bracket。
软件errno、decoder mask和hardware raw word[3]分开核对。

fresh sequence=PRE_SEQ+1且CLI software sequence／final G／32word／OBS4/PATH/hist一致。
全批同boot，点间sequence精确+1，不重号或跳号；旧seq原日志保留且不充新sample。
只解析显式post-command dmesg，拒绝新于目标seq、冲突重复、缺32word或CLI/dmesg逐word不一致。
G decoder核完整预算、own时间、gap occupancy、window conservation、carry/open-edge/ID；
最终G valid1、旧batch valid1/status4、16PATH及16hist全部通过才接受本点。

旧OBS4/PATH stock/flow代码机械复用P0 [原件](p0_records.as-reused.py)，
[实际最小diff](legacy-OBS-diff.patch)只替换旧“有A则必须正流量／重叠”要求。
G的A可在V之前／之内／之后完成；window Aissue/done/carry各自保留，空A window合法。
V AXI8192事务、PATH连续性／shared leave关系保持原检查；不把shared count当private之和。
E5 [hist helper](hist_records.as-reused.py)原字节复用，最终强制OBS4/schema1；不接入OBS5。

production入口递归拒绝fixture/synthetic/mock显式标记。
有限测试只能在内部显式`_fixture=True`+FakeConsole运行，输出保留fixture_only=true、
evidence_level=synthetic_protocol_fixture；CLI没有fixture跳过选项。
实际Console模块仅在正式manifest检查通过后的运行路径导入，普通import／--help／plan不触硬件。

[run01](../collector-validation/run01/report.json)为最初91项通过；补充完整stats与严格类型后，
最终[run02](../collector-validation/run02/report.json)100项通过，含：四条件、A before/inside/after、
taint/kernel/seq/final-valid/rawbank/PATH/hist/预算/warmup/data/quiet等正反例；
实际collect函数完整40点FakeConsole，以及第二点boot/seq/command/exit异常立即停止且raw保留。
这些输入均为合成protocol fixture，不能证明实际DMA、BIT或板上性能。

复跑：

```sh
python3 paper/ispass2027/exp/E6-mitigation/collector-validation/verify_g_collector.py \
  --cli-report paper/ispass2027/exp/E6-mitigation/cli-validation/run01/report.json \
  --output paper/ispass2027/exp/E6-mitigation/collector-validation/root-check01
```

Root审阅且G BIT正式qualified／实际加载后，正式接口为：

```sh
python3 paper/ispass2027/exp/E6-mitigation/collect_g.py \
  --output <新40点结果目录> --load-manifest <实际load.json> \
  --fpga-manifest <实际G_BIT_build-manifest.json> \
  --software-manifest <实际g-software01/build-manifest.json> \
  --backend-evidence <既有passed_P0_backend.json> \
  --board-tools <现有paper/ispass2027/board>
```

输出`manifest.json`、完整`raw-01..40.json`、只含已验记录的`records.json`；
每条保留原CLI/OBS4/PATH/hist/Gword，以及单列derived metrics，不把软件计算冒充hardware字段。
分析计划见[analysis-plan.md](analysis-plan.md)，后续分析实现另小批，不自动生成正式结论。

## Root审阅后的两处聚焦修订

生产bindings要求正式原软件manifest字节SHA
`4bee1eadd8d8102e652129833f171ba869e5edd8a1a039973d13559a3a0b2f6c`，
并显式检查OpenSBI e8d21、CVA6 source_reference_not_loaded_bit角色、
CLI prepared/built source与正式ELF来源，以及两份prepared/kernel UAPI一致性。
只有内部明确fixture模式可使用合成软件manifest；上述来源字段检查不因此放宽。

每点raw先保存，随后在parse之前递归拒绝raw顶层及嵌套synthetic标记；原item检查仍保留。
内部fixture模式对manifest、每个raw及每条record均显式标fixture_only和synthetic evidence_level。
这两处不改变40点、BIT/OBS、预算、数据、quiet或driver/Image。

17项聚焦检查复跑（输出须新目录）：

```sh
python3 paper/ispass2027/exp/E6-mitigation/collector-validation/verify_g_collector_revision.py   --cli-report paper/ispass2027/exp/E6-mitigation/cli-validation/run01/report.json   --output paper/ispass2027/exp/E6-mitigation/collector-validation/root-revision01
```

原100项run02原件不覆盖。此次输入、source和boundary mock完整记录在revision01顶层
`fixture-context.json`。production-mode第二点raw-only顶层／嵌套标记两负例只mock bindings和Console，
实际collect/save/guard/parse执行；第一点parse一次，第二点raw保存但不parse，两次command即停止。
未伪造production可用的qualified BIT清单。

这些负例子目录的原as-executed manifest故意为fixture_only=false，以覆盖真实production路径；
它们均status=failed，包顶层明确fixture_only=true、synthetic来源与boundary mock说明。
原失败raw和manifest不改写，任何一份都不能单独作为板证据或进入paper出口。
正式原软件身份与内部fixture全输出标记反例／正例也在17项内。
