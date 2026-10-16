> **2026-10-12 用户纠偏后的有效结论：本机是执行服务器，不是本研究的实验平台。** 下文stage104中的本机dmar1／T400／NVMe事实只作参考；“在本机安装AI环境、运行OPT6.7B并进入G0准备”的主线选择已撤销。原实验平台仍为Genesys2上的CVA6＋四iDMA＋RISC-V IOMMU＋Linux；仿真按需辅助，关键验证回现有FPGA。GPU／AI只改变场景优先级。FlexGen暂为工作流来源参考，尚未通过现有平台的需求／能力／成本适配关口。下一步先核可忠实映射的任务和控制位置，而非运行下面的历史服务器合同。[纠偏裁决](../out/platform-scope-correction-20261012/scope-correction.json)／[原报告快照](../out/platform-scope-correction-20261012/before/gpu-ai-deployment-gate-20261012.md)。

# GPU／AI 优先：真实部署核实与前置采集合同 stage104

2026-10-12。用户要求设置goal继续，场景优先GPU／AI加速器，但优先级不构成排他条件。共享翻译干扰、完整有效工作与计成本的方法论文仍为主线。

**裁决：选定一个有来源的AI逐层卸载工作流，进入前置功能与事件采集准备。** 本机GPU与NVMe的共同主机翻译单元已核实；真实程序的任务和完成边界也已取得。当前尚未运行AI，实际并发、资源峰值及翻译相关损失待采集，不能写成G0性能通过或创新方法成立。整体仍在第一阶段“问题与方法立题”，位置为G0之前的具体实例准备。

[稳定根裁决](../out/gpu-ai-deployment-gate-20261012/root-result-acceptance.json)／[下一批采集合同](../out/gpu-ai-deployment-gate-20261012/pre-g0-contract.json)。

## 1. 实际部署：本轮补上的事实

| 对象 | 本机实际记录 | 本轮结论边界 |
|---|---|---|
| GPU | T400 4GB、compute7.5、驱动550.144.03，0000:01:00.0 | 当前加载nonOpen驱动；公开同版本源不是其二进制完整调用证明 |
| NVMe | 10000:e1:00.0，工作目录所在ext4由nvme0n1p3承载 | 预定权重与offload私有目录确实落在该设备；尚未产生模型I/O |
| GPU／NVMe／网卡 | device/iommu均指向dmar1，组13／6／12均DMA-FQ | 同一remapping硬件实例、启用地址重映射；实际cache／walker争用、IOVA和leafsize未测 |
| Intel集成GPU | 指向dmar0 | 同机器不自动等于同翻译单元 |
| 运行条件 | GPU4096MiB，主机MemTotal约134.7GB；所选文件系统可用约1.4TB | 当前解释器未找到所需AI库，无nvcc；GPU峰值／实际pin预算仍须运行核实 |

[实际设备与路由](../out/gpu-ai-deployment-gate-20261012/local/hardware-inventory.json)／[文件系统到PCI设备对应](../out/gpu-ai-deployment-gate-20261012/local/storage-backing.json)／[运行条件](../out/gpu-ai-deployment-gate-20261012/local/runtime-prerequisites.json)。

根直接核Linux v6.8参考源码：设备probe后关联iommu_device，Intel的dmar名字对应DRHD所分配的硬件实例；不是仅凭隔离组推断共享。DMA-FQ是翻译域加延迟批量失效，不能当作严格逐次撤销的F&S配置。上游参考源码不冒称Ubuntu6.8.0-60的逐字二进制来源。[域类型ABI](https://raw.githubusercontent.com/torvalds/linux/v6.8/Documentation/ABI/testing/sysfs-kernel-iommu_groups)、[设备关联源码](https://raw.githubusercontent.com/torvalds/linux/v6.8/drivers/iommu/iommu.c)、[Intel实例源码](https://raw.githubusercontent.com/torvalds/linux/v6.8/drivers/iommu/intel/dmar.c)。

主机页DMA映射的公开550源码经Linux DMA API取得设备DMA地址。GPU内部页表、主机IOMMU和ATS分开记录：多个CUDA stream不等于不同Requester／PASID；当前ATS启用未知。所核CUDA12.4.1的Linux HMM条件包含Open Kernel Modules，本机未满足此项，但显式pinned拷贝不依赖HMM。PCIe peer-to-peer的IOMMU限制也不能直接否定SSD→CPU→GPU中转路径。[NVIDIA映射源码](https://github.com/NVIDIA/open-gpu-kernel-modules/blob/ca09591fbd213d521feaf1856b41e887b2906a01/kernel-open/nvidia/nv-dma.c)、[CUDA固定版本说明](https://docs.nvidia.com/cuda/archive/12.4.1/cuda-c-programming-guide/index.html#iommu-on-linux)、[ATS接口](https://github.com/NVIDIA/open-gpu-kernel-modules/blob/ca09591fbd213d521feaf1856b41e887b2906a01/kernel-open/nvidia-uvm/uvm_ats.h)。

## 2. 选定工作流与完成边界

来源为FlexGen作者论文和官方FlexLLMGen固定commit `004ffef82b46e8dc8685c55d0cdda650bdaf1269`。它有真实的分层权重加载、当前层计算、下一层预取与缓存生命周期；GPU和SSD可以属于同一有依赖的AI流水线，不要求另造独立后台。论文2023评估与固定2024后续代码不是同一镜像，不把原T4 16GB结果移植为本机T400吞吐。[作者原文](https://proceedings.mlr.press/v202/sheng23a/sheng23a.pdf)、[工作流源码](https://github.com/FMInference/FlexLLMGen/blob/004ffef82b46e8dc8685c55d0cdda650bdaf1269/flexllmgen/flex_opt.py)、[搬运后端](https://github.com/FMInference/FlexLLMGen/blob/004ffef82b46e8dc8685c55d0cdda650bdaf1269/flexllmgen/pytorch_backend.py)。

源程序单batch路径预载下一阶段，每阶段经disk queue join与CUDA全局同步再前进；disk后端使用memmap和四个私有pinned relay worker。DISK标签只说明张量home，OS pagecache可消除实际NVMe读取。源CLI先初始化全部权重，再warmup；这两段都可能改变缓存。原论文表21明确披露控制pagecache会改变开销和收益，不能免费驱缓存来制造本机需要SSD的条件。

前置功能任务采用源默认OPT6.7B、实验缩尺batch1／一个GPU batch／请求prompt32／完整新增2token；保留原warmup，关闭debug、cut和dummy替代。源CLI实际上选OPT30B tokenizer，须保存真实tokenizer版本、token IDs和实际长度。所有这些是明确实验设置，不是原论文性能轨迹。

两项普通配置固定为：

- 磁盘权重home：percent `0 0 100 0 100 0`，KV与中间激活在GPU，源overlap开启。
- 主机内存权重home：percent `0 100 100 0 100 0`，同模型、输入、精度、完整完成，源overlap开启。本机容量允许它成为强普通对照，不能为了NVMe并发排除。

权重／offload／结果均指定本批私有runtime子目录，尚未创建运行环境或下载模型。功能终点须是两token真正生成、每项必要数据搬运完成、queue与CUDA静默、worker排空；API提交、预分配输出数组及console文本不代替完成。完整初始化、warmup、pin、供给和排空另列并计总成本。四relay为`8*GB`字节的源级条件量，utils的GB值及真实pin峰值未取，不宣称已测8GiB。

[AI需求卡](../out/gpu-ai-deployment-gate-20261012/ai_workflow/demand-card.json)／[GPU路径卡](../out/gpu-ai-deployment-gate-20261012/gpu_path/path-card.json)。

## 3. 有界假设、普通方法与下一执行关口

根提出待检验问题：在GPU host DMA与真实NVMe DMA确实同走dmar1并重叠时，计入合法驻留、准备、源已有预取和控制成本后，现有按带宽预测的层级重叠是否仍留下可避免的关键路径等待？源论文的阶段max成本模型以理想重叠为前提，源码又有全局同步；模型误差不能自动归因翻译。必须分开媒体、pagefault／缓存、CPU relay、PCIe数据、软件队列、同步和GPU计算。

根保留的条件设计问题是：若上述残余经匹配证据成立，能否在发布前用已有层依赖与有限page-work额度改善有效进度？这里只提出可反驳的研究问题，不接受新规则。仅“翻译优先／token／预取／换成AI”不构成独立创新；MASK等GPU内部MMU前作也不直接等于主机跨设备路径已覆盖。合法大页要核真实IOVA leaf，不能用CPU大页名称代替；已发布copy不能由下一次门控撤回。

下一批执行按[前置采集合同](../out/gpu-ai-deployment-gate-20261012/pre-g0-contract.json)依次落实：

1. 固定隔离运行环境、模型／tokenizer／配置与真权重文件，核最大张量、relay及GPU峰值适配成本。
2. 加最小私有事件记录：逻辑ready、排队／issue／完成、层消费、缓冲所有权、实际字节、两token及最终排空。CPU与GPU时钟分别保存，未校准不能直接相减；GPU指针不冒称IOVA。
3. 两配置各运行一次有限功能／事件采集，确认初始化、warmup和生成各段真实NVMe活动及GPU host copy重叠。未安装、未下载、未运行是本批状态，不是缺用户授权。
4. 以实际记录决定最小性能G0。自然pagecache或CPU驻留消除生成段NVMe时，关闭本seed的“必须磁盘／稳态GPU-NVMe干扰”收益假设，保留功能资产；若仍有可避免等待，再冻结一个失效和相邻判别以及准确普通方法。

压缩若满足同等质量目标，也须在后续公平比较中允许；本功能seed的固定未压缩精度不是创新时人为缩小资源的许可证。内核翻译观测当前无读权限、无枚举到dmar perf source，记录为观测缺口，不启用追踪或改权限，不拿模型内部队列充当真机证据。[根假设与反例](../out/gpu-ai-deployment-gate-20261012/root-hypothesis-and-counterexamples.json)／[控制点核查](../out/gpu-ai-deployment-gate-20261012/root-path-and-control-review.json)／[实际观测权限](../out/gpu-ai-deployment-gate-20261012/local/observation-access.json)。

## 4. 本轮实际执行与验证

两个复用Codex席分别解决GPU路径和AI源码合同，根保留架构、关键原文与最终选择；新增原始材料九项（两席各三、根Linux三），全部保存字节、版本边界和SHA256。根直接核关键调用链和原论文成本／pagecache段落，实际PermissionError一次有界恢复只记录不可访问，不改变权限。

全程Codex，无DeepSeek／Claude；本轮没有依赖安装、模型／tokenizer下载、CUDA／AI运行、性能模型、构建、FPGA或生产接口／驱动／启动配置修改。下一关口已具体化，但G0性能和方法论文尚未完成。旧源码、stage98—103结论、原始产物和脏工作树均保留。

[根直接阅读覆盖](../out/gpu-ai-deployment-gate-20261012/root-source-read-coverage.json)／[GPU来源](../out/gpu-ai-deployment-gate-20261012/gpu_path/source-manifest.json)／[AI来源](../out/gpu-ai-deployment-gate-20261012/ai_workflow/source-manifest.json)／[根Linux来源](../out/gpu-ai-deployment-gate-20261012/root-source-manifest.json)／[实际账本](../out/gpu-ai-deployment-gate-20261012/execution-ledger.json)。文档与原件静态校验另保存于[最终验收](../out/gpu-ai-deployment-gate-20261012/documentation-and-source-acceptance.json)，不当作应用性能验证。
