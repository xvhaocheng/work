# §32 Codex 独立强普通基线规格

继续§31，不复制失败实验。旧SourceReadGateEnable只在Mode>=4的late pool生效，该pool固定4KiB aligned/Depth8历史，因此不能直接用于新32KiB错位任务。新实现仅放out，无池Mode0保持生产banked_ctl、之前配置写容量副本、BUF3以及所有逆读/交错诊断关闭。

新增ordinary_copy_owner_gate.sv，位于每DMA源AR与四输入mux之间，仅门控AR VALID/READY，原AW/W/B/R直接透传。SourceReadGateEnable在本轮out复制的owner_actual_dma_top.sv之Mode0分支选择此门；这是新的普通对照接口，不能称旧开关直接有效。

每流接收实际iDMA normal descriptor(src/dst/len)；从virtual mux AW首次offer记录目的AW addr/len/size的8槽ring，每DMA使用32bit序号而非旧固定任务8次总量。虚拟W队头是真四输入mux switch_id类型w_fifo_data，port0/1下bit0选择流；ID追加位2也是stream，BankIdBit3为bank分类，不能混用。来源axi_mux.sv:166/181/330/353，w_fifo存仲裁选择并据此选slave W。

只在virtual和physical Whead都属于该流、相同流完成的virtual/physical W段序号相同、对应目标AW已到physical completion时，允许其源AR；读的源偏移、拍数/size与当前virtual AW对应的目的偏移、拍数/size必须完全相等，范围在descriptor内。这是严格整段准备规则，普通CONF0共同分段可满足。CONF1首R2KiB vs首W1KiB不满足，不能把这种几何限制宣称所有普通方法失败或I创新。

首次AR VALID后使用sticky hold直到AR握手，不撤回VALID；AR payload由原iDMA保持并手写断言核。不同DMA均可先在原端等待，因此不把不合适流的AR先排入共享TC。只允许各流一个normal active job、固定AXI input ID0、64bit INCR；静态有效映射。计入job字段、AW ring/序号及hold/断言状态，不把BUF3的48B叫总硬件成本。禁止硬编码PA猜copy。

当前PoC目标：CONF0、32KiB、dstoffset3072、V先/A先，两项与§31相同数据/完整guard/真实NEXT-DONE/quiet验收；先看是否用小数据FIFO完成。然后才比较译址后固定分片/动态机制。候选I尚未实现、不声称创新/通用无死锁/生产修复。
