# start意图的bank/方向资格：最小来源补核

2026-10-16；Sol/high请求，actual metadata unknown。**原请求的bank和读写类型在实际S1 start时可从已有TL锁定字段取得；当前PTW busy位本身不足以安全决定阻挡谁。** 本次只定向核下列已具名源码，没有实现、模型、实验或合同修改。

- 当前 `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:235–244` 以原始AXI ID的 `BankIdBit` 分bank；`:366–376` 把本bank request逐字段复制后仅覆写`.bank`，原IOVA/DID/PID/ttype保持。TC `rv_iommu_trans_ctl_wrap.sv:625–637` 将AW明确记为translated/untranslated W，`:645–662` 将AR明确记为R/RX。故方向是原交易属性，有来源而非由PTW活动猜测。
- 当前 `translation_logic/rv_iommu_tl_wrap.sv:200–214` 的非旁路request spill保存完整`req_data`；`:416` 从锁定的`req_data.ttype`解码`is_store`，`:462–463` 原响应本就沿`req_data.bank`返回。`:769–800` 把同请求的IOVA、上下文、is_store/is_rx及stage交给PTW。因此**在包裹PTW的TL层**可以把start occurrence与`req_data.bank`及`is_store`关联；PTW目前接口中没有bank输入，不能称PTW现成输出已携带bank。
- start必须仍用 `rv_iommu_ptw.sv:315–327` 的当前IDLE、`init_ptw_i && !edge_trigger_q`，并满足`:360–365` 正常S1分支；TL `:373–375` 的ptw_init电平本身不等于新的已确认start。原PTW只输出active/level等，未提供此已确认start事件到共享AR门。可新增一个受资格的start事件导出，再与保持的TL bank/方向一起捕获；不能把busy拉高当start，或仅因ptw_init电平重复续lease。

拟议最小paid intent记录可以是 `{valid/start, bank, is_store, occurrence/epoch, stage/normal-S1资格}`。确切字段位数依单walk/reset协议可合并，例如is_store可只允许normal untranslated W后省去存储；这是实现选择，不能先报成零字段/零状态。必须计入实际start导出、bank/方向的同周期配对或寄存、到门控位置的传线/可能时钟域同步、pending生命周期与reset/timeout处理。不能从现有TL字段存在推出它们已经可被DDR selector读取。

对payload L，TC `rv_iommu_trans_ctl_wrap.sv:346–357` 保留原`ar_data.id`，故已有bank标识原则上可随原ID追踪至后续AR。但门所在位置必须按真实fullID prepend/route解释该位并保留其owner，不能直接把DS/非DS端口号或低位DID当bank。其最终接入/比对逻辑及生命周期仍需付费；本补核不扩展验证整条路由。

**三种命题必须分开：** `L.bank != intent.bank` 可以由上述已有字段加新增导出通路表达；“intent是V目的写翻译、V当前持W owner”还需方向资格及真实owner来源/稳定性；“L与V排空无数据依赖、V源已完整保留”仍是本模型明示的条件S，bank不同并不能在任意应用中证明这件事（跨bank copy也可能有数据依赖）。本次没有native完整前缀、owner反馈接线或依赖证书，因此不将other-bank判定升级为生产无死锁/有效收益证明。条件小模型可以继续使用S，并把其适用范围与paid intent接口分别记录。
