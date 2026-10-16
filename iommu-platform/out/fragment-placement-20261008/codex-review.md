# Codex关键审定，§35

本审定依据实际源码与raw，不以模型共识为证据。

1. `pretl_owner_fragment.sv`保存原虚拟父，按返回拍数offset发固定max128拍child，恢复原ID/最终RLAST，每流1父/一次1child。双W序核virtual/physical owner、seq及连续字节范围，不仅owner。
2. `pre_actual_dma_top.sv:543–571`在原始两个DMA的AR之前逐流选择；AW/W/B仍原mux。tr2→tr4 cast依据packed ID位于高位且仅ID宽度有别，显式覆盖ID。完整AR类型148/98bit，但公共flat harness只支持扩展字段子集，不宣称端到端全属性覆盖。
3. pre真实DMA入口各流1父；post保留各流8项virtual关联和原TC队列。工作集/物理服务几何相同，总资源未配平。声明2502/3568bits非实际综合成本。
4. `placement_test.cpp:1491`读取PRE-EDGE真实dbg_dma_issue/done。末尾严格检查每流恰好1次，DONE>ISSUE，全部原完成/数据检查过后才打印。SYSTEM的walker计数来自实际握手而非旧fallback；miss由lookup-hit差计算，本轮lookup完整响应。
5. 两新post/S CSV与§34同启动顺序单父基准逐字节相同，观测无可见轨迹干预。四N CSV等同对应S、FAST0；不反推内部slot高水位。
6. `rv_iommu_tl_wrap.sv:303–306`的N要求跨bank活动PTW窗口，同bank不合格。Codex运行前误把它当可触发强对照，已撤回；模式确传入reset(enable)，不能从FAST0说有效快路径无价值。
7. `codex-metric-audit.json`由Codex读八CSV/log产生，每流32个实际物理AR，ID32/36、len127，源基址80000000/90000000+日志child offset，精确同序匹配。每原父0..15两child共256拍。平均延迟用真实issue→done，不用末DDR W。
8. 27.09%完成耗时下降、同34miss/102PTW是真结果。不是内存流量下降或全部32次hit的服务时间；提前译址、AW反馈、入口容量亦影响。启动顺序是确定性镜像而非随机重复统计。
9. 新pre_parent_ar UNOPTFLAT例示路径经过两个always_comb；局部flow_ready只读寄存state，owner选择不依赖s_ar_ready。四pre运行正常收敛不等于完整路径lint/综合证明；静态正常任务不推广AXI通用异常/失效正确性。
10. 因复用/提前准备已有前作，当前速度胜前置基线不能直接证明独立新规则。如果普通复用＋提前准备同总成本同效，就不能将位置规则作独立创新；如果新约束下有独有决定/进度/成本且核清前作，才可以建立独立机制贡献。旧动态最长前缀负结论保持。

最终902条件通过。首次24条件失败是脚本总数/单流与other解析，修正后仅重解析raw，没有重跑DUT。76生产SV hash未变，原tests/Linux/BIT/SD/板卡无改动。下一普通父翻译复用。
