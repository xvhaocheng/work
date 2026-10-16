# §45 root运行验收

2026-10-08。root Codex实际应用DeepSeek脚本与一次修订，直接审查保存primitive、两个neighbor实现、Dijkstra与报告字段后，运行一次脚本。作者分工：root设计合同/证明/执行/接受，DeepSeek起草普通Python实现；native两席进行关键证明和primary质疑，不执行模型。

## 1. 真实执行

`execution-ledger.json`：math_model_runs=1，exit0，elapsed_seconds=0.7781777381896973，stderr空。6450=5族×86预算×15服务额度；每族1290。另120代表路径与5特例是同一执行中的重叠辅助求解，不计成新6450试验。所有数据是数学图，非DUT/FPGA实验。各无解配置确实耗尽heap；成功配置早停，不将尚未定距节点称不可达。

root随后只读取JSON核数量、字段统计、工作下界和代表路径，没有重新运行模型。全部6450完成可达性与root最大gap判别一致，mismatch=0；所有可完成配置min_work=30。两请求读不能共享/合并，无PWC，各固定15步，这个下界不能外推一般cache场景。

## 2. 最小q随B的实际输出（折叠不改变q的相邻段）

| 具体保存族 | B范围→最小可完成q |
|---|---|
| whole_restart | 0..85→15 |
| full_current（5bit通用tag） | 0..34→15；35..37→7；38..78→4；79..81→3；82..85→1 |
| semantic_restart（3bit tag） | 0..32→15；33..35→7；36..85→4 |
| mixed_indexed（4bit固定链tag） | 0..33→15；34..36→7；37..77→4；78..80→3；81..85→1 |
| semantic_chunks（3bit固定链tag） | 0..32→15；33..35→7；36..76→4；77..85→3 |

B只计single suspended frame，C、active、owner/valid、在途与terminal共同成本另计；是列明codec上界，不是总面积/最小状态曲线。q是PTE服务笔数而非cycle。

## 3. 原始路径人工复核

- semantic_restart，B36/q4：每请求0→4→8→12→15，单轮4/4/4/3笔；frame33/33/36bit，无回退，双方共30笔。
- semantic_chunks，B77/q3：每请求0→3→4→7→8→11→12→15，3/1/3/1/3/1/3；frame47/33/77/33/77/36bit，全部≤77，无回退，双方共30笔。
- semantic_restart，B36/q3：全部可达保存节点只有初态两个turn编码，搜索耗尽，无完成。输出cycle使用A/V各1笔后whole reset；两席手工给出的各3笔完整G后discard同样合法，但未伪称JSON包含那个3笔环。
- whole_restart，B0/q14：无完成，heap耗尽；q15：先A15再V15，30笔完成。

每个成功路径中的真实frame符合B、推进量合计30、双方终态15；第一方完成后第二方连续完成属合同允许，不借此躲过first-completion缺口证明。

## 4. 接受范围与停止

root接受受限max-gap iff与普通单调checkpoint策略达到30笔下界。两席补充q2专用80bit点只手工审定，未进入五族枚举。五族不穷尽所有codec/cache/联合编码。存在合法完成策略不表示所有公平策略完成；静态normal模型没有执行真实fault、动态映射或到达。

因为相同原文字段的普通方法已达到本合同最低PTE工作，就不能把固定链调度或phase编码作为独立创新。本轮DUT/build/BIT/JTAG/板卡/SD=0，production RTL/Linux/tests未改。真实最新完整copy仍§43，不重跑旧DUT。
