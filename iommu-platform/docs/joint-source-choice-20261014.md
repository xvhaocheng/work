# 合并来源与选择差异首关：当前入口不准入A/B（2026-10-14）

执行批准方案并登记实际goal1791943603，无额外token预算。只核原`short-mn-a1`正常入口：N1、A/V各1页、gap0、16次V测量、16次V warmup；没有运行该旧case或任何新A/B。

**当前入口不适合作为已经成立的F1正例。** 原先可疑的512/3异构缓冲不在两个活跃source之间；原窗口的A工作量也随V完成和软件STOP变化。首次VALID前真实共同可选状态及独有选择仍unknown，F1全族不关闭。

本轮获得四项具体边界：

1. 软件仅启动DMA0/1；绑定硬件下两者都为512拍。3拍buffer属于未由该入口启动的DMA2/3。
2. batch逐个提交4KiB copy，等待当前copy最后B对应的backend完成后才提交下一job。Job FIFO深度8不能当8个并发copy。
3. NormalTL合流在TL normal_gate之前首次VALID就锁定；后来两bank同时VALID可能已有winner。不能把后来的并存当成可重选。
4. V固定16次，A target2^32且在软件观察V完成后STOP/drain。候选改变完成时间时，A接受数和字节可能改变，原V窗口不能直接证明相同工作量下双方完整收益。

来源调用顺序、所有权及完整事件链见[机制卡与依赖图](../out/joint-source-choice-20261014/root/source-choice-card.md)，[软件原文](../out/joint-source-choice-20261014/source-origin/source-evidence.md)和[root原文/SHA](../out/joint-source-choice-20261014/root/source-snapshot.json)分别保存直接证据。当前软件未绑定旧Image/tool binary，当前加载FPGA身份也未核，全部结论限定源码合同与钉住历史RTL参考。

warmup在mode/config/clear之后；软件warmup后无显式IoTLB/DDTC invalidate，RTL arm/open/close不直接产生policy_clear。但这不等于已证明测量全hit、无PTE或预测历史有效；没有新时间轨迹，也不补warmup/清cache/扫相位。

具体普通覆盖只接受两项：held／单头状态没有新的合法排序；必要历史无效时F1回退RR，O-RR复现本次选择。它们不证明全任务周期或费用相等。成熟历史且真正双头的差异未证，九冻结普通合同继续保留，不宣称owner/debt/前作覆盖整个F1。

源码字节式为V逻辑copy量65536B、A量`nA*4096B`，payload R/W另各一份，warmup/启动/尾部/PTE及CPU成本分别核；nA与完整退休时间均未测。所有权关联、CDC、事件/history端口、预测算术、选择延迟等新增费用保留，完整数字成本、面积/Fmax/应用null。

本轮停止在合并关口，没有A/B执行合同、预测器、四条件、RTL或板操作。后继必须一次性给出同一正常来源的真实首次双头、已拥有释放依赖、固定完整工作量及适用普通规则外差异；没有证据包不再自动开启同类审查，不改变本入口制造正例。不同承诺点或新有限来源另立合同，本轮不自动切换。

root及唯一必要新独占source-origin席均Codex-only6.1-sol/ultra/native；无旧席恢复、DeepSeek/Claude/Astra，无旧实验/build/validator/trace重解析或压力/容量修改。稳定裁决、五文档同步与必要新交付核验后才actual complete。原窗口累计，论文未完成。[完整导航](../out/joint-source-choice-20261014/README.md)／[稳定裁决](../out/joint-source-choice-20261014/root-result-acceptance.json)／[下一门](../out/joint-source-choice-20261014/next-goal-contract.json)。
