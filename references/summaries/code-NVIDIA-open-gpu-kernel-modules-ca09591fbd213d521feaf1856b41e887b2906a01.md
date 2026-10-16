# NVIDIA/open-gpu-kernel-modules/ca09591fbd213d521feaf1856b41e887b2906a01〔正式题名待整理〕

稳定ID：`code-NVIDIA-open-gpu-kernel-modules-ca09591fbd213d521feaf1856b41e887b2906a01` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/NVIDIA/open-gpu-kernel-modules/blob/ca09591fbd213d521feaf1856b41e887b2906a01/kernel-open/nvidia-uvm/uvm_ats.h)
- [来源2](https://github.com/NVIDIA/open-gpu-kernel-modules/blob/ca09591fbd213d521feaf1856b41e887b2906a01/kernel-open/nvidia/nv-dma.c)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/gpu-ai-deployment-gate-20261012.md:25](../../iommu-platform/docs/gpu-ai-deployment-gate-20261012.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/gpu-ai-deployment-gate-20261012.md:25](../../iommu-platform/docs/gpu-ai-deployment-gate-20261012.md)：

主机页DMA映射的公开550源码经Linux DMA API取得设备DMA地址。GPU内部页表、主机IOMMU和ATS分开记录：多个CUDA stream不等于不同Requester／PASID；当前ATS启用未知。所核CUDA12.4.1的Linux HMM条件包含Open Kernel Modules，本机未满足此项，但显式pinned拷贝不依赖HMM。PCIe peer-to-peer的IOMMU限制也不能直接否定SSD→CPU→GPU中转路径。[NVIDIA映射源码](https://github.com/NVIDIA/open-gpu-kernel-modules/blob/ca09591fbd213d521feaf1856b41e887b2906a01/kernel-open/nvidia/nv-dma.c)、[CUDA固定版本说明](https://docs.nvidia.com/cuda/archive/12.4.1/cuda-c-programming-guide/index.html#iommu-on-linux)、[ATS接口](https://github.com/NVIDIA/open-gpu-kernel-modules/blob/ca09591fbd213d521feaf1856b41e887b2906a01/kernel-open/nvidia-uvm/uvm_ats.h)。

[返回总目录](../README.md)
