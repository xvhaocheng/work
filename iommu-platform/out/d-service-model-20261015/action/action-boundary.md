## Material Passport

- 任务：`d-service-model-20261015`／source action boundary；日期：2026-10-15；作者：Codex。
- 输入：复用 `out/d-source-gate-20261015/mechanism/{mechanism.md,sources.json}`，定向直接核当前 RTL/现有 M0 服务声明。请求 Sol/high；实际模型与强度元数据不可得，均记 unknown。
- 输出是根席条件模型的约束输入，不裁决全局新意、不要求先有原生实现或实测反号。未跑构建、旧实验、DMA或外部模型；未改生产源码。

## 结论

可以把 P/PN 投影为“同一分区、串行普通 TL；PN 额外允许另一 bank 的合格缓存 hit 在 owner walk 已启动时经有限 N 槽提前翻译”。它直接支持普通 R 和 W。**纯 device→host 写在 AW/CTL 接口上可用 N，不需要 COPY 完整读缓冲证书；但本核查没有建立写专用 fixture、实际 producer 或原生 SPDK/SSD 的 P/PN 链。** 单共享读写服务台也不是当前 M0 事实，必须独立声明。

## 不可省略的动作边界

| 边界 | 必须保留的最小语义 | 直接源码位置（缩写在 JSON 对应完整路径、当前 SHA256） |
|---|---|---|
| 相同 P/PN | runtime `(N,partition)=(0,1)/(1,1)`；同编译硬件、同容量及分区。不能把 P 偷换成另一个编译 `NonblockEnable=0` 的入口 | M0:3359–3368；TL:284–286 |
| 普通路径释放 | 主 TL 以 slow response 成功握手释放当前请求并回 IDLE；**P 此后可以翻译下一候选，同时上一个 owner 的 payload/B 还未结束**。CTL bank 的翻译头在下游 AW/AR 握手释放，而非等整 job 完成 | TL:174–176,1544–1567；CTL:674–725 |
| 窗口 | start 时须 owner 在 PT、PTW active、S1-only、候选另 bank，且无明确排除条件。window **仅 gate start**；开始后 owner walk 结束，不自动取消 DC/LEAF/OUT | TL:303–306；N:47–48,100–112 |
| 付费 N | 一个 N 槽，先 DC 后 LEAF，两次有序状态推进；复用真实缓存端口，相关主路 hit 被屏蔽。只有 accept 触摸替换状态。OUT 阻止普通新入口；REJECT 不握手，等 normal_fire/候选消失重置，不能每拍免费重试 | N:51–55,100–112,143–172；TL:249–277,284–286 |
| 合格与许可 | 实际候选 DC 命中有效；Sv39/S1-only、canonical、普通 R/W、4KiB 非 superpage、U、方向对应 r/w、MSI 安全及其余排除项；候选/DDTP/DC 稳定、无 invalidate。VALID/DC hit 均非许可；只有 LEAF 的 accept 令 TL request 握手 | N:56–58,84–103；TL:286 |
| 响应到完成 | accept→OUT→共享 RR response→匹配 bank AWAIT_RESP→COMPLETE→下游 AW/AR；再有真实 W/data、B 或 R 最后 beat、provider/consumer 的完成定义。早 TL 不直接计作早服务/早 quiet | TL:346–352；BANK:379–408；CTL:178–180,219–239,332–362,674–725 |
| 有限资源与顺序 | 保留每 bank 单翻译头、AW 优先 AR、有限队列和 W ownership，响应及请求 held VALID 稳定；公共 RR 不偏向 PN。N 不取消 owner walk、不重排已提交头、不新增免费服务口/优先级 | BANK:158–165,243–275,379–408,418–440；ARB:11–14；LOCK:34–51 |

N 的缓存查找状态非零是源码事实；本轮未校准任何周期。事件模型可以设正的符号成本并做敏感性分析。若压缩为统一 probe cost，要保留其占用、屏蔽主路、拒绝和 OUT 延迟的影响，并称“条件抽象”。要称 RTL 等价，还须展开逐拍端口、响应仲裁、缓存更新/替换、队列/ID 和 PTE 反馈；单个 free-hit 事件不够。

## 纯写的具体边界

CTL:628–642 的 AW 选择只要求 AW 有效及 W credit 未溢出；CTL:646–647 的 fullbuffer 证书只用于 AR 的 read-admission 豁免。N:56–57,88–99 明确允许 UNTRANSLATED_W 并检查 w 权限。因此独立纯写不应被强加 COPY 的源读缓冲证明；仍要提供合法且保持稳定的 AW、实际可获得的写数据、有限 W buffer/credit、mux 的 W 所有权和最终 B/consumer 完成。AW 可以早于 W 数据就绪，故早翻译也可能没有早 payload。

这只证明动作接口适用。把 SPDK 完成事件投影成此类写流、其依赖/ready 时间、bank/DID 与页权限映射以及 provider/consumer 终点，须由根席来源合同显式给出；不能从“源允许写”推出“现有实际 producer 已如此发写”。

## 服务台假设：当前 M0 与研究投影分开

M0:653–677 同时驱动 AW/W ready、AR ready，并单独处理 B；M0:699–719 的 head-serial 是 **read mem_q**；M0:1291–1399 中 AW/W/B 使用独立 caw_q/cb_q。一个“共享 DDR AXI 口”不证明读写为一个互斥服务器。早 payload **read** 排在 PTE read 前存在具体路径；早纯写直接推迟 PTE read，在该独立队列声明中没有相同证据。不能把前一种因果无标记移到后一种。

允许研究“真实或拟议共享资源使读写竞争”的条件原理模型；统一服务器、非抢占、setup/transfer 时间、读写切换、队列深度/排队次序均作为声明假设，对 P/PN一致计费。无需先完成原生实现或取得实测反号才能建这个模型。它能证明的是给定假设下的因果与边界，不能声称当前 M0 重现、真实 DRAM 校准或 RTL 逐周期等价。现材料也不证明真实 DRAM 永无读写耦合。

若用固定“剩余 walk 时间”，须说明排除了早 payload 对后续 PTE 的反馈；要研究这条负效应，就需按 PTE 依赖给共享服务请求，不能一边冻结 walk 完成时间、一边声称完整计入竞争。PTW active 只表示非 IDLE，不等于该拍 DDR 正服务 PTE（PTW:97–100,384–397）。

## 普通 P 对照必须有的机会

1. P 在 slow response 后立即利用正常入口，而非等 owner job/B；正常仲裁服务所有已合法 ready 的头。
2. 两动作获得相同的事前已知 producer 结构、页/分区准备、ready/依赖和公开发行能力。合法提前准备、可用 outstanding credit 与提交 VALID 前的公开排序，按实际接口允许范围纳入；准备/观测/切换/clear/warm 均非免费。
3. RR/held VALID 与 W 归属对两者相同；不能给 PN 私有 future-hit 真值或任意优先级，也不能人为禁止 P 的普通下游重叠。普通控制的额外调度能力同样要有实际接口，不任意改硬件 RR。
4. “同 RTL runtime P/PN”与“同总资源普通构造”分开登记。N 槽、逻辑、probe占用全部列成本；等资源 P 若复用额外存储/控制，必须给出可执行接口与使用方法。不能因为现有 runtime P 未使用 N 槽，就自动宣称已击败所有等资源控制；也不要求击败所有想象程序。

源文件当前完整 SHA256、精确行范围、关键原文及与既有 source register 的匹配结果见 `action-boundary.json`。所有列入文件当前 SHA 均与复用登记一致；这不绑定旧日志/旧二进制。本席只提供源约束，模型参数、来源投影和最终准入由根席接受。
