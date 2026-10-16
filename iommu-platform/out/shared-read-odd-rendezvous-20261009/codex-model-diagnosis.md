# §77 Codex：加入窗口与内存轮询模型

本阶段只在 out 夹具中修改普通控制与模型；没有修改 production/Linux、综合、BIT 或板卡。三项 build01 与五项 build02 DUT 均 exit0，日志打印完整 copy 通过；最终 CSV/SHA 验收另记，不以此文替代验收。

## 普通夹具修正

OddRendezvous 从作业开始就要求 phase0/mode3 的新 qW 数据全部 resident；其后不再将最小闭包 J 强制进位为偶数。原有 firstVALID AW/W 不修改。rV=1 时 q2 下一承诺端点2大于 resident1，故不能在捕获 J 的同沿新建 promise2；J1可以合法成立。

仅这两门不够：没有 cutover 的旧最后一拍路径会要求 qW[254,256) 先 offer，而新 resident 门要求 R256 先到。已将完整 WRITE 成功证书下的 phase0 终端纳入现有两拍 private parking 例外，仍保留 lead<2/B3，不增加 payload。这属于普通 resident-W 实现边界，不是新算法。

## 原轮询模型为何藏掉窗口

multi_test.cpp 的 drive_ddr 原来每次探测 ready ID 就推进 r_rr_last_id，随后可能因 memory-r-interval 未到而撤销这次 NEW VALID。轮询指针因此按空闲 tick 前进，而非按实际 R 服务前进。held R 并未撤销，旧模型仍可满足 AXI 的协议检查；它不能代表预期的服务轮询公平性。

原 run01：V ID33 firstR3065、old LAST7097，差4032=63×64；A ID37 AR3076，latency31 使其3108已 ready，却到7161才返回1拍。两个候选在 idle tick 交替，每个64周期服务槽落在同一奇偶相位，全部选中 V。这不是 A 尚未获许可，也不是 A 的 AR 未被接受。

Codex 将 last-ID 更新移至实际 f_ddr_r 握手、mem_q.pop_front 前，使用真实完整6bit ID。两种 rotation 的 !ddr_r_held 保护及每 ID 最老请求选择都保留；不会绕过已经 VALID 的 V 响应。n_rr_selections 仍只是候选探测计数，不是服务次数。

## 实际新窗口

build02 run04/05/08 分别为 interval64 有背压/B64、interval64 无背压/B0、interval63 有背压/B64。三个都记录 resident_valid=1、resident_cycles=4、rV=rA=J1、rawWV1/rawWA0、promiseV1/promiseA0、E64、phase2。旧 V physical R 仍 busy；新规则在这里可以尝试原子绑定，当前 §77 仍未绑定。

run06 不触发 cutover，完成两份独立拷贝；run07 interval0 无窗口，A短前缀完成时V已12。窗口只是这些实际服务条件下可达，不是所有负载都有。

## 修正历史结论的范围

§75/§76 的旧数据、源/可执行快照与所有失败记录保留。它们的 byte/held-VALID 检查结果不因本次模型诊断自动失效，但 interval>0 下的零窗口与公平服务推论须限定为旧模型。没有声称 interval0 全部 bit-exact，也没有将旧耗时改成新模型数据。模型修复不是 IOMMU production bug，不是论文机制贡献。

Codex 亲自设计/诊断/源核/执行；独立关键席核了 J1 同沿、terminal 环和轮询相位证据，未执行实验。
