# Codex：接纳前排空负担规则的普通等价控制

2026-10-08，root亲自推演，不是DUT新运行或新机制验收。

## 为什么“事先限制负担”这句话不足以立新

在§43受限模型中，P是V的前缀；不同bank并没有两套冲突顺序。令Q是已首次offer但尚未物理W退休的完整序列；k_i是Q中设备i最后一笔写的位置。普通控制保存每行owner、长度及已传W计数。想把i已承诺W退完，必须传完Q从head到k_i的全部剩余W。这个值用FIFO前缀求和即可得到，不需要新的调度机制。

若再要求需要从内存返回多少源数据，静态线性copy的source/destination offset映射及已经返回的R计数已由普通fragment保存。对上述目标prefix扣除已得到的数据，就得到必要source payload；已offer读的不能撤销尾部另行保留。§42两个现场的普通continue正好达到这类字节下界。翻译准备、R/W运输、pending B分别是不同的债务，不能把其中一个总数称整个释放周期界。

因此“before offer，只有C_i≤H_i才接纳”在C_i只含这些数量时，可以由普通owner-prefix记账＋保留已VALID请求实现同一准入集合。它与只有自家AW数量的弱方法不同，但这不代表与最强简单方法不同。这里没有实现该控制，也没有测其面积/时序，不能声称零额外面积或性能更好。

## 紙面反例只排除弱统计

`[A2KiB,V4KiB]`与`[V4KiB,A2KiB]`的每owner及全局未传字节相同；A退出前需要的prefix不同。普通k_A记录立即区分它们。不能只拿第一类统计当baseline来宣布新规则成功；两状态真实可达性尚未执行。

## 前作有区别，但代码差异不是论文差异

AXI-REALM冻结版的budget在下游AR/AW握手按整child bytes扣，budget_left只导出及触发isolate；NumPending是静态transaction count。这版没有我们的copy source/dest关联字段，不能说其代码已经实现exact C_i。

但其write buffer先收完整W再发AW（完整WLAST token gate），使**已走过这个buffer的目的AW**不再等待未来source R。它用容量改变服务义务，已是必须比较的强普通方案。buffer不足可能让上游整个child不能推进到WLAST；这不意味着可以从该buffer发一个未收齐数据的AW。相同总payload下short child+full reservation、普通prefix gate都须加入成本对照，不能只比较一个放不下的固定长burst。

## 下一判据

因为FIFO前缀工作可用普通记账获得，单独把它作为新算法不能立独立创新。只有找到**真实翻译相关约束**，使强普通控制不能在相同信息/资源/释放合同下给同样保证或效率，或者证明前作未覆盖的锐资源/延迟界，才可以发展机制或分析贡献。若只有translation-ready bit、普通PTE优先、不同阈值、源页缓存或FIFO位置，就先核既有前作与当前§39控制，不自动再编译。

当前设计状态：两个写序冲突分支已停；纯byte-debt准入归为普通对照，不作为已成立新机制。下一步仅补上述增量的具体可达见证，尚无进入实现的新规则。本阶段不承诺期刊分区。

主源与直接源码：
- [AXI-REALM论文](https://arxiv.org/abs/2501.10161)，§III-A buffer与budget描述；冻结代码不一定等于论文评估版本。
- [冻结budget单元](https://github.com/pulp-platform/axi_rt/blob/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_rt_unit.sv#L278)：278–282,352–386。
- [冻结write buffer](https://github.com/pulp-platform/axi_rt/blob/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_write_buffer.sv#L95)：95–123,150–177。
- 对应本地源码均在../fragment-drain-boundary-20261008/map-agent-sources/；root本轮直接核以上门与counter全文件，以及isolate Normal/Hold/Drain时序。
