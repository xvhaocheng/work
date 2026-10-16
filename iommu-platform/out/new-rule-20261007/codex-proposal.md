# Codex 独立规则草案（2026-10-07）

作者：主会话 Codex。依据共享源码/因果证据提出，在读取本轮 DeepSeek 答案前保存；双方任务共享事实和候选轴，因此不是完全盲审。本文没有通过创新验收，没有实现或测试。

## 草案：先预约页表，再受限兑现旁路

一句话：N 可以先完成缓存命中的翻译，但它产生的新数据读不能抢在当前待取 PTE 进入共享存储入口之前；放行后，每次真实 PTE 进展只允许有限量的数据承诺，等待变长不再产生额外许可。

动机：现有 FIFO 模型中，PN 的 V 数据读在 2763 到达 DDR，叶 PTE 在 2766 才到达；已进入服务的 256-beat 数据读使 PTE 返回到 3100。MASK 的页表优先能调度已经到达的请求，不能撤销三周期前已经承诺的不可抢占数据读。该事实支持“优先级生效边界前的预约”这个问题，不证明新机制优于 MASK。

### 明确规则

新增元数据：主 walk 代号、当前 PTE 阶段代号、该 PTE 是否在最终共享 AR 入口完成握手、该阶段已预留的旁路读 beat 数、尚未返回的旁路读 beat 数、旁路来源标签。新增 PREPARED 状态保存已完成的物理地址翻译；只使用现有每 bank 单一持有响应时，不能假设获得多个 lookahead 槽位。

```
walk/PTE 阶段开始:
    ordered = false
    used = 0               // 不清除旧旁路请求的 outstanding

对应 PTE 在共享 AR 入口握手:
    ordered = true

尝试把 PREPARED 的 N 读变成物理 AR offer:
    if conservative_drain_required(read):
        保留原系统必须的排空路径，单独记账
    else if !walk_active:
        按普通路径提交
    else if ordered && used + beats <= Q && outstanding + beats <= Q:
        used += beats      // 在首次拉高 VALID 前预留，不能等 ready 才记账
        outstanding += beats
        提交 AR，VALID/地址/长度保持到握手
    else:
        保持 PREPARED，尚未产生物理 AR VALID

旁路 R beat 实际返回:
    outstanding -= 1       // 不补 used

PTE 实际返回并处理出后继阶段:
    开始后继阶段；只有这里允许重新计量 used

异常/失效:
    禁止产生新的承诺；已接受的 AXI 请求必须继续排空
    PREPARED 的失效处理必须与原翻译完成/失效契约一致
```

`Q` 是一次阶段允许承诺的 beat 预算，不是“有 Q 就绝不伤害 PTW”的保证。初版不分裂 burst：若 Q 小于原 burst，就整个延后；不能把改变 DMA job 长度或软件间隔当作同一规则。已预留但尚未握手的 AR 也消耗预算。旧阶段的 outstanding 跨阶段保留，防止一个长读被下一阶段重复借贷。

`conservative_drain_required` 不能仅用输入侧 `w_owner_q` 或相同 AXI ID 推断：必须覆盖物理侧已承诺 AW 所需的源读。保守实现可豁免已承诺物理 AW 所在 bank 的读；它会放过多余流量，因而预算仅约束可延后的旁路读。若无法明确排空依赖，该候选不能直接改生产 RTL。

### 相同命中、不同决定

S0：候选已缓存命中、owner/队列大小相同，PTE 还在 DS 延迟链内。候选先准备但不承诺物理数据读。

S1：候选条件相同，PTE 已在共享存储入口被接收，且有预算。候选可以承诺物理数据读。

区别来自 PTE 是否越过优先级生效边界，而不是 V/A 身份、hit/miss、单纯队列长度或等待年龄。

### 保证和不保证

1. 在所有相关读共用的 FIFO 服务模型下，非豁免旁路读不能在 AR 接收顺序上越过当前 PTE；它仍可能阻挡下一条 PTE。
2. 对非豁免请求，每个 PTE 等待阶段新承诺至多 Q beat，所有阶段合计未返回量也至多 Q；等待时间本身不增加许可。
3. MIG 跨 ID 重排时，AR 接收顺序不等于服务顺序。没有共同可控仲裁/服务保证时，第一项不能推出 PTE 延迟保护；预算也不建立硬实时上界。
4. 原有输入 owner guard、fullbuffer 例外、AXI 规则均不删除。排空例外的流量和影响必须单独报告，不能冒充预算保护。
5. 不保证 PN 比 P/N 更快：可能把 N 的收益全部消掉，也可能仅移动瓶颈。

### 来源接口

- `rv_iommu_nonblock_hit.sv:47–48,100–107,143–172`：现有旁路探测/命中接受。不要只临时关 accept，否则 LEAF 会掉进 REJECT。
- `rv_iommu_tl_wrap.sv:282–330`：现有 N window 和 normal gate；无法提供共享存储端的 PTE 接收证明。
- `rv_iommu_trans_ctl_wrap.sv:329–357,714–725`：现有 COMPLETE 把持有翻译直接变成物理 AR/AW；需要拆开 PREPARED/ISSUE。
- `rv_iommu_ptw.sv:385–417`：AR 发出/接收与 PTE 返回；只有返回后才知道叶或后继。
- `tests/nonblock_actual_dma_system_top.sv:362–385`、生产 `axi_delayer`：PTE 存在可见性延迟；不能把固定 50 周期当作 MIG 延迟上界。
- banked controller 输入 owner 与共享物理侧 owner 不是同一组信息，需实际接口设计。当前 FPGA 几乎满 Slice，不允许凭空宣称只增几个计数器、零时序代价。

### 最强前作与否决条件

已知组件：MASK 的页表/数据协同优先、普通准入/令牌/欠账、MMU-aware DMA 的分段翻译驻留、数据依赖和 credit、预留/服务调度本身均已有。新候选仅是具体的“PTE 跨边界确认 + 按真实 PTE 进展计量旁路承诺”的组合。搜索未发现同式规则不能证明原创。

1. 若强前作已有未来 PTE 预约/同式进展预算，机制新颖性否决。
2. 若在相同合法状态和可用接口下，普通页表优先加固定总 credit 取得同等选择和收益，否决新规则，只保留工程修复。
3. 若只有固定 50/50 延迟+严格 FIFO 才有效，实际 MIG 无同等可控边界，则平台论文主线不成立。
4. 若原 burst 太大、排空豁免太多，规则退化成全部关闭 N，没有优于关闭 N 的联合收益，则停止。
5. 先只做既有实际 DMA 模型里的最小对照：同工作集/输入，P、PN、PN+普通 PTE 优先、PN+普通固定 credit、PN+预约、PN+预约与进展预算。取得区别后才考虑 RTL；当前没有实施。

审定：可以作为明确可证伪的规则提案；现在只能称研究候选，不能称“前人没有”或已可立项创新。
