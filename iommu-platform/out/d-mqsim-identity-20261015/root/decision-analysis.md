## Material Passport

2026-10-15；ARS-Codex experiment-agent run/有界正确性核验。完成固定输入身份定位与私有修复回归；不是系统综述、新论文新意或原生数据正确性认证。共享IOMMU任务Codex-only，既有全局安排保持。

## 研究结论

**原MQSim少完成的根因已确认并作最小私有修复；四组原冻结输入均完整完成，但四个合法host+模拟PCIe排空边界上活动GC全为0。停止扩展这版cache-off覆写→跨任务活动GC残留假设，不调seed/cache/相位/长度追正例。** 这是已修私有语义变体上的四输入结果，不否定所有SSD服务状态、FTL布局、cache或其他工作负载，不自动改主线为I/A。

D仍需要有来源的跨边界状态及可计费的信息价值。下一有界候选转为已核DRAM行/刷新来源，先检查真实规则下普通前史、严格模型排空、显式warm/等待怎样保留或洗掉状态，再决定是否接共享资源P/PN模型。无需预先取得原生P/PN或实测反号，但模型假设、O_host与O_phys、已有观测和内部真值必须分开。本轮没有新增选策模型、策略训练或新意通过。

## 根因与实际修复

一次初始身份重放完整复现上轮所有原有事件与结果XML，UID将缺失CID53定位为第1777次生成、LBA2912/LPA182的用户写child2841。它被GC屏障暂存，释放时调用仅处理MAPPING的AMU局部handler，立即return后直接delete，未进自己的TSU/PHY/cache/CQE路径。源码与完整事件同时支持根因；不是由模型共识或最终空表推断。[根因报告](diagnosis.md)／[原请求摘录](missing-request-events.json)／[稳定身份接受](identity-acceptance.json)。

私有变体仅改LPA release一个语义函数：先从指定LPA两等待索引摘出原child到局部借用list，解锁，再走正常Translate/分配/保护/TSU/真实program/owner/CQE。保留owner原指针及原数据，不delete/rebuild，不手动补成功或绕overfull，不改GC movement/erase协议。额外三处只读记录给出正常用户分配、PHY命令发起及GC候选未完成user-program计数。源码、合同、patch和binary都在实际build/run前绑定hash。[正式修复合同](repair-contract.json)／[root代码审查](repair-review.json)／[私有diff](../implementation/repaired/repair.patch)。

| 原冻结请求数 | 修复前host完成 | 修复后host/各自真实program完成 | 屏障进入/重投 | 满plane等待/恢复 | 排空边界GC |
| --- | --- | --- | --- | --- | --- |
| 512 | 512 | 512/512 | 0/0 | 0/0 | 0 |
| 2048 | 2047 | 2048/2048 | 1/1 | 37/37 | 0 |
| 4096 | 4095 | 4096/4096 | 2/2 | 91/91 | 0 |
| 8192 | 8190 | 8192/8192 | 4/4 | 205/205 | 0 |

root对所有14848个用户请求逐一核唯一生成→SQE→设备请求→原child→一次分配/提交/发起/实际PHY完成→正常owner remove/CQE/host消费；没有把GC搬页完成当用户新写完成。无锁内分配、无GC候选尚有未完成用户program、无异步mapping wait/CACHE越界；overfull等旧路径未作语义修改，按实际锁迹与真实分配逐次核验，不能用“最终配对”替代未越锁检查。PPA按block erase代次区分复用，不要求跨全程数值唯一。

旧512对照的原有事件和XML完全不变。独立只读核账检查748项启动前hash，拒绝4种负变异：无自身PHY的伪完成、锁内分配、GC时仍有user program、同child重复提交。3组覆写在修复后会改变GC与时序，不能将其与少完成旧运行作性能比较。[四组完整核验](repair-verification.json)／[同一第1777请求的新链](repaired-target-events.json)。所有ns来自示例NAND配置，不是本机SSD测量。

## 接受边界与后继

接受的是四冻结整页USERIO write-only/cache-off、单stream/单channel-chip-die-plane配置内的模型因果完成及局部修复。不是通用上游修复；混合R/W顺序、partial、cache writeback、多资源、suspend和实际payload字节均未验。原模型host不保存真实数据，真实program事件不能被写成端到端数据内容校验。本轮修模拟器不构成论文创新。原所有源字节、8运行、1调试重放及其旧未建立裁决原样保留；这是后继新证据，不回填历史成功。

实际新增执行：5次私有构建（4次随观测证据需求变化，1次语义修复），1次初始身份重放，4次私有回归；root审查后亲自执行，独立核账不运行模型。无M0/CS1/旧硬件实验、原生DMA、生产Linux/RTL/UAPI修改、collector、FPGA或板操作。MIG1792023314仍paused，A5/A2和独立工程成果保留，10月15—28日立题窗口不改。请求模型与实际运行元数据分开，不把请求配置当实运行认证。

[下一最小来源合同](next-minimal-contract.json)优先验证DRAM行/刷新在显式warm后的保留与洗去，尚未执行。已有D最接近工作核查继续复用；若进一步主张新状态接口或信息价值，再定向补最接近的接口/调度前作，不把MQSim/Ramulator2本身当全部前作，也不因某项传感器不存在就宣称创新。
