# O-envelope-derived：在线输入合同与停止范围

actualgoal1791952824；root，gpt-6.1-sol/ultra。源码及手推，不是已实现硬件或已发生的调度。本卡只核父轮两score的直接在线落地，不新增双rollout。

## 问题与继承边界

在physical payload AR进入下游的首次VALID之前，选择ADMIT当前数据L，或WAIT使当前PTE P先进入同一共享服务。目标为当前两任务各自真实R/W/B和正常job退休的最晚完成时间，次比较双方完成时间之和。原一PTW、IoTLB16、队列/ID/held/命中旁路、同ID顺序、read-admission、fullbuffer、错误终结和后续供给保持。

父轮在附加条件下得到：

`F_admit=max(D,a+E)`；`F_wait=max(D+g+b,g+E)`。

该式来自B→A的完整流式W顺序、可用AW/credit、跨策略相同W服务及相同末B尾z。它不是所有实际R/W/B状态的通用公式；完整CA/CB与max/sum仍需保留。公共z仅能在明确相同的条件域抵消。

## 观察、预测和安全次序

1. 如果physical AR已向下游首次VALID/held，继续原承诺直到握手；不得根据新score撤回、更换地址或ID。native已承诺DS/其他通道同样保持。
2. 新选择只用仍存活的physical packet/select与它所属的原current job；当前慢walk用自身req_data/PTW owner。TC已前进的req metadata、event-only issue_id不能冒充L身份。
3. 全部native许可/原容量/同ID/native W owner条件优先。只允许阻挡尚未首次VALID的L，不能重新授权AW/W或截断完整walk。
4. 关联、normal/error资格、history端点或epoch无效；计数/时间/score溢出；history未成熟；当前义务不能落入声明包络；或者所需AW可用性不能证明/不能由明确预测覆盖：回退currentDS+RR。不得免费读未来PTE/尚未接受请求/隐藏DDR状态。
5. 信息有效时比较两策略的预测max，再比较sum，再年龄/RR；一次越过持续等待的另一合法头后必须服务它。相同packet的WAIT直到P合法推进/条件失效再重新按原时序处理；不因重算撤回held VALID。年龄/越过记录及选择延迟计费。

这只是完整调度合同，没有新增RTL。currentDS+RR指普通控制的回退行为，不声称该新增gate已免费存在于生产实现。

## 本轮实际核到的输入与缺口

输出W FIFO保存bank index，在AW首次转发VALID时预留槽，晚于该槽的W可在真实AW握手前推进。FIFO的存在可证明预留W顺序，不能证明所有AW已经接受、整份copy已写就绪或双方B尾相同。

可构造便宜的当前job前缀记录：normal、aligned、DEFAULT、deburst=0的4KiB/64bit copy，各job两份256beat AW；每bank2bit记录0/1/2/invalid，合计4bit。它证明“已预留的B槽在未来A槽之前”，不预测A未来AW许可时刻。参见[w-order-certificate.md](w-order-certificate.md)。

对父式的**直接current-full-AW证书版本**，要求双方整份当前copy的raw AW已真正接受、physical AW均已准备，且L仍待首次VALID、P仍为另一copy必要普通翻译。输入demux的W bank排他与DEFAULT的真实R→W依赖使该入口不可达。此版本全回退，停止。

上述是检验一个显式更严格的直接映射，不能把它倒写为父轮“任何AW预测都必须当前已全部接受”的要求。父轮允许AW许可在数据需要时可用；对合法的部分前缀/未来AW预测版本，缺少策略相关AW准备/许可及独立B完成项，故本轮没有接受完整收益或产生实际非平凡选择。它需要另立明确修订合同，不在本轮暗中扩成更大预测器。

## 创新、收益与下一门

- 接受：真实W预留/接受分离、当前完整AW证书的可达性界限、廉价部分顺序证书接口与分项成本。
- 停止：untouched两score直接映射到“两份copy当前已完整写就绪”的在线版本。不能据此关闭所有W包络、未来AW预测、F1或完整准入家族。
- 普通覆盖：父轮O-envelope-derived已统一覆盖其纸面窄域，保留；本轮不是新增已发表前作覆盖，不拼接各场景事后最优。
- 性能：无实际prefix/history/服务law/字节与RWB完成/净收益，microbenchmark不准入。面积、Fmax、完整新增总位数/端口/周期和应用收益为null。
- 下一工作只能是明确修订：让原input W owner的释放、peer AW普通翻译准备和两任务独立B终结进入包络，或交付新的同源完整证据包。不能只再扩写原两score、重跑旧域或自动恢复旧分支。
