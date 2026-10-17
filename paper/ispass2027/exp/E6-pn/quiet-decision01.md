# Root决定：补齐模式切换的DS事务完成保护

2026-10-18 JST。此决定属于现有P/N移植的必要配置保护，不是新增论文观测指标；尚无实现验收或物理结果。

Root已直接读IOMMU `5b7dfbc70e458244a8e5682e5ae3f8571320254c` 的 `rv_iommu_msiptw.sv:163–170,242–305`：Basic-Translate读发ARLEN=1的两拍请求，RREADY常1；成功首拍可使BT FSM回IDLE，未要求RLAST。MRIF首拍交接也会先回BT IDLE。因此，FSM idle加当前VALID均0不能排除两R拍之间仍有旧尾拍。现 `rv_iommu_ds_if.sv` 的spill/Wowner保护不能单独证明所有外部读已完成。

接受的提案方向是：保留原MSIPTW/PTW和DS传输行为，仅给新增quiet判断增加最小被动生命周期跟踪。优先复用已有可靠库存；若无，则在DS外部边界用真实AR握手→RLAST握手、AW握手→B握手各一个小计数器，结合全部内外spill VALID、Wowner empty、producer生命周期及控制队列状态。两个8bit计数器加sticky错误是上限建议，不是已综合资源数或新增AXI credit限制。

计数器不控制数据READY/VALID，不改服务顺序，不进入旧observer或新增论文诊断bank。PnEnable关闭时编译移除。同步增减净算；无对应请求的完成或不可表示的增加均置sticky错误，之后quiet不能为真，直到全局reset。不得wrap到0后授权模式切换。当前请求/返回VALID必须独立排除，不能只测试计数为0。内部spill也必须纳入，以覆盖尚未到外部的请求和已从外部返回但未交付producer的响应。

这解决的是已接受外部事务的库存边界；CQ已取未执行命令、fence、FQ/MSI/MRIF内部工作等仍须由其实际状态提供quiet证明。不能用head==tail、单一TC in_flight或CPU独占代替硬件事实，也不能把capture_busy反馈到observer的all_quiet。

A1先交准确边界、原始调用链和真实两拍MSI返回间隙的有限DUT方案，再接受生产小批。Root将直接审实际diff与测试证据。全局capability、原S路径、50MHz约束、首构优先级和既定时序/回归停止条件均沿用本次任务合同。
