# E2：原 pathv3 的能力限制（本轮不修改、不运行）

本轮固定原Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e` 与现有BIT。该ABI只支持一个干扰者，不能执行用户要求的2–3个同时批量干扰者，也不能分别取得所有干扰者的miss/PTW。依照本轮要求停止E2，不改驱动或RTL、不构建。

实际原软件源码位于`/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-software01/`：`inputs/bench/idma_experiment.c:124–129`只填写一对V/A；`src/linux/include/uapi/linux/idma.h:48–74`只有一个aggressor_id和两份stats；`src/linux/drivers/misc/idma.c:847–850`为streams[2]，`:868–1030`全局experiment_lock使多进程也串行，`:1058–1068`不支持后来的multi ioctl。`src/linux/drivers/iommu/riscv/iommu-hpm.c:11,76,188–194`只分配两endpoint×三事件。原CVA6 `8aac3fc` 的PATH只跟踪V和一个选定A，物理4DMA不等于逐设备观测。

未来若恢复此项，需要独立批准/指定可用的multi内核与采集接口；不能把后来ABI3软件称为当前pathv3能力。既有multi两轮在N4_A8发生softlockup，原记录仍保留于Root的`exp/E2-device-scaling/softlockup-diagnosis.md`，不重试、不混入新模型验证数据。本轮E8继续，E2新增板点为0。
