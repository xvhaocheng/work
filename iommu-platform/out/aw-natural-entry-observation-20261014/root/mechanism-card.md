# 自然入口观测卡

唯一问题：原供给是否自然形成B当前job两次输出AW预留、A当前job零次预留，并同时有未承诺的B物理载荷队头L和A当前读PTW的P。候选动作和普通定义不改，本轮只做入口事实，不执行预测/ADMIT/WAIT干预。

原配置：每源唯一DEFAULT4096B、原512beat私有缓冲/onePTW/16IoTLB、accepted decouple_rw0/deburst0、两路串行NEXT、无warmup/batch/CPU、原N及完整walk。真实四输入upstream mux，两个live源；原DS50/50、行为DDR head-only31/queue16/原DSpriority。唯一新能力是被动记录，不是新的改善方法。

原输入、完整source/job、bank/functionalID、held VALID、R/W/B字节及退休都从新记录核验；无法把同bank或super_last直接当依赖/完成。相对普通的选择差异和净收益没有执行机会；原普通卡未改。

结果：AW条件1695拍，加入A读P后429拍，再加入B物理L后0拍。必要AW源前驱自然存在，但不是完整候选入口。这一冷、sole-copy固定前缀停止；不构造预热/延迟去修正它。完整家族开放，论文未完成。

进入收益比较仍须新的原始合法完整L/P入口、双方全工作和全部费用；微benchmark门为false。模拟只证明其声明DDR条件，不代替FPGA/GPU需求。
