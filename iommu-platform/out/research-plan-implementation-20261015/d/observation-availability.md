# D首批：观测可得性与信息时机

2026-10-15。决策只发生在quiet边界，为下一COPY任务选P/PN。完整SHA与源摘录见[assessment.json](/home/xvhaocheng/work/iommu-platform/out/research-plan-implementation-20261015/d/assessment.json)。本轮没有实现观测器或新增采样。

## 1. O0须是决策时实际知道的信息

| 信息 | 现状/来源 | 本次事前决策能否用 |
|---|---|---|
| 源/目的页环、映射/context、DID、任务字节/次数、既有模式、gap、CPU条件、服务配置 | M0登记及配置代码已知；板端request/metadata也保存 | 能；所有baseline同获，不能隐藏A1/A8或DS配置制造不可分 |
| 板端分DID普通lookup/hit/miss、fast、fill、gap、PSCID、IOTLB首尾快照、config/generation | 已有composition窗口ABI，[字段:72](/home/xvhaocheng/work/iommu-platform/scripts/composition_plan.py:72) | 当次窗口结束后可读；用于下一同类任务还需真实重复来源、历史取得/失效费用与稳定性条件 |
| 板端pending/AW入口反压/完成计数 | 原V窗口指标；事件重叠，非互斥原因 | 仅按窗口和时机使用；不是PTW阻塞原因，[边界:129](/home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md:129) |
| 板端HPM miss/S1walk等 | 旧工具明确`raw_only, not a validated metric` | 不能冒充已校准因果或banked-mode性能指标，[口径:454](/home/xvhaocheng/work/iommu-platform/scripts/composition_plan.py:454) |
| M0完整JSON的全局lookup/hit、PTW活跃/返回、fast、TL accept/resp、payload统计及双方完成 | 已有producer输出，[C++:982](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:982)、[JSON:3031](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:3031) | 是该动作执行后的结果；可作标签/核对，不可用于同一次事前选择 |
| 同预算可得的复用、合并/扇出、占用突发与任务结构 | 事前已知映射/页环必须给；其余并非当前ABI现成字段 | 逐项核实际硬件/历史来源与费用；不能把DOA的GPU计数器当成本平台已有，也不强造平台没有的MSHR语义 |

普通lookup不包含fast路径，分母须分别报告。`O0`不以“miss均31”替代完整观测；不能将P与PN的事后结果误当作两个动作前不可区分状态。

## 2. 模型真值可见，不等于部署可见

完整CSV覆盖`t0→quiet`：DMA issue/done、DDR AR/AW/W/B/R、PTW起止/AR/R、fill及fast，[写出代码:998](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:998)。源头是测试层级观察线及C++队列，不是现有板端observer的导出功能。

| 候选量/身份 | 已有入口 | 尚缺的证明 |
|---|---|---|
| payload与DS端口 | DDR tag32=V payload、36=A payload、48=DS；[解码:31](/home/xvhaocheng/work/iommu-platform/scripts/analyze_full_batch_g0.py:31) | tag48不是通用的“某DID的PTW”；DS_if合并多类结构请求 |
| DS pending与另一路payload R的联合时序 | 以同tag有序AR—R配对构造模型离线区间；已有匹配算法[96](/home/xvhaocheng/work/iommu-platform/scripts/analyze_full_batch_g0.py:96) | 需核请求类型/owner和同一服务链；不能推出MIG内部队列或全部等待原因 |
| PTW活跃与V等待的联合时序 | `full_ptw_start/end`带`dbg_main_bank`；DMA issue/done和DDR事件可配对 | bank来自当前`req_data.bank`；须证明所需期间绑定稳定。DMA未完成也不等于翻译未完成；已有trace缺逐请求lookup/hit时序 |
| 后来证实为hit的V请求等待 | 当前JSON只有聚合lookup/hit，fast另计 | 需要有身份请求关联及真实可见时刻。不可把后来hit真值交给更早决策 |

实际测试探针定义见[system_top:1392](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:1392)。旧耦合日志已明确无DID追踪，不能将活跃PTW猜成A：[诊断原文:106](/home/xvhaocheng/work/iommu-platform/docs/nonblock-dependency-diagnosis-20261004.md:106)。

## 3. 部署缺项及候选最小增量

当前板端V9缺PTW占有及页表AR/R独立时序；不是把旧E3信号直接接来就已有同合同测量：[历史原件:131](/home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md:131)。目前以下均为未知：

- 能否在同一实际配置取得稳定owner/context、请求类型和PTW—payload关联；每事件采样边界与漏记/溢出行为。
- 一个付费小联合计数或短窗口摘要是否足够；能否比完整trace便宜，是否及时用于下一任务。
- 新观测的面积、时序、CPU读取/日志争用及状态维护费用；当前仿真层级tap不能作零硬件成本证明。
- 任务是否真实重复、旧窗口能否预测下一窗口；相位变化和过期观测的处理。

候选增量先限制为一个已结束窗口的跨owner联合时序摘要；不能区分时再比较一次有预算的现有P/PN完整试选或弃权。共现/次序信号不是可相加的因果份额。没有实现或性能主张。

## 4. 首批信息门

先列每项观测的源、身份、窗口、首次可见时刻和取得费用；给baseline同样信息。若来源不明、需未来真值、现有便宜信息已足够，或普通完整试选同费用有效，则相应“新增观测必要/有净值”主张不通过。不为补齐表格自动启动collector、旧板gap矩阵或paused MIG。
