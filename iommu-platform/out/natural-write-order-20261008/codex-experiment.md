# Codex独立实验设计与计数器风险假设（2026-10-08，§31）

## 目标与实验边界
沿候选I继续查自然多DMA写序/源读的依赖；不重跑§30两项。独立out实验副本、只读标量观察端口，不修改生产或当前tests文件。两个真实iDMA、samebank、PoolMode0、BUF3，关闭ReverseReadOrder/InterleaveAw/SourceReadGate/Frontier/HeaderPark，删除build宏TEST_COMPLETION_GATE，使用实际rv_iommu_banked_ctl。DDR仍原单序列服务模型，不是MIG复现。CONF1独立读写分段；2048或32768B、目的offset3072、两个启动顺序，共最多四项首次运行。没有CPU/batch/预热。先观察几何与停滞层次，不实施I。

## 生产对应关系
ariane_peripherals_xilinx.sv:1228 的真实四输入mux低ID宽2、NoSlvPorts4；:1409以BankIdBit=DmaAxiIdWidth=2选bank。生产DMA0/2同bank0，DMA1/3同bank1。
当前两活跃测试port0/1在SameBankDmasEnable1下用BankIdBit3，ID标签0/4同时落bank0；相当于重新选择同bank的两流，但尚未实现literal生产port0/2、DID10/12映射。不能称四真DMA实验或完整生产等价。
生产:1270缓冲配置：banked模式下DMA0/1各512拍，DMA2/3各3拍；实验两流均3拍是资源配置探索，并非当前生产同bank混合512/3原样复刻。生产明确只论证冻结4KiB aligned job，长job与错位目的超出该原型保证；不能用新例解释既有4KiB CONF0 PN结果。

## 独立风险C0（在worker返回与新仿真之前记录，未证实）
- FPGA配置 ariane_soc_pkg.sv:111 NumOutstandingTrans8/WFifoDepth32，测试同8。
- banked_ctl:164每bankNumOutstandingTrans4；输出mux:434 MaxWTrans8。上游4DMA mux MaxWTrans8。
- trans_ctl_wrap:175 w_cnt_t宽clog2(4)+1=3 bit，:245 counter WIDTH3。counter内部delta_counter多一位，非sticky overflow_o取内部MSB；外部q_o仅低3位。
- 第8个未完成physicalAW在一个bank若合法被接受，内部counter=8，外部w_cnt_q=0且w_cnt_ovf=1；:220与:360要求w_cnt_q非零才Wready/Wvalid，从而可能无法返回到7。
- IDLE:628依据w_cnt_ovf禁止AW，在overflow已经发生之后才生效；不是显式w_cnt_q>=perbankNumOutstandingTrans。
- 长拷贝可能在AW优先期间排入8个地址，原2×4KiB工作最多4AW没有触及此边界。

以上只是源码推演。还须确认实际AWready限制是否使8项不可能同时进入；如果观测只有<=7而停滞在其他位置，不能使用C0作诊断。若成立应先用正确计数/准入边界验证，再讨论I；增加一个bit或显式full判定是工程修复，不是论文新算法。

必要观察：每bank w_cnt_q / w_cnt_ovf及八项physicalAW减WLAST事件；源AR是否已出、RREADY/WVALID是否被该计数零阻断。新增读口只放本轮system_top副本。所有失败日志保留，不伪造PASS或完成JSON。修复若必要应以独立复制TC模块用于对照先验证，生产应用须经源审查/实际接受。

## I机会的实际判别
R已经提交，后一段对应目的在正确虚拟/物理W序中暂不可消费，且当前源R尾部阻住另一DMA必要读，才构成候选I针对的问题。只在源AR之前发生TC队头停滞，或者仅计数器错误，不能当I奏效证据。普通动态分片同信息也表达同规则，元数据取得/保持与strong fixed fragment/尾部预约的成本必须计入；创新尚未成立。
