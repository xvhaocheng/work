# 本轮用户收窄：路由后的映射接纳／复制

2026-10-16 JST。本文件是当前用户异步回复的任务摘要与关键原话，不是完整逐字备份，也不是已验证事实。

用户关键原话：

> 这次固定源码后，候选一应收窄为：在路由决定之后，哪些源内存区值得建立并保留目标设备映射，哪些应继续逐次映射，哪些应复制到目标设备的紧凑池。

> 它不再以“接收包交给CPU后就必须撤销NIC权限”为前提——所选驱动的普通RX映射本来就可以跨多个包的生命周期保留。

> DMA_PMD已经按domain首次使用建立大页映射，因此“按需映射，而非全部预映射”本身不是新增贡献。

用户明确的本次分析范围：

- 冻结官方Linux v6.12，不把本地6.5加提交当v6.12；源码支持的组合为 mlx5e A RX → samples/bpf/xdp_fwd_kern.c xdp_fwd_prog/FIB → B mlx5e跨设备XDP_REDIRECT，或PASS中的本机目的UDP socket。
- 普通单buffer、线性RX、非AF_XDP，本地UDP足够大用户buffer、非MSG_PEEK；不把多buffer/clone/tap/GRO聚合混入同一证明。若实际链不能排除这些须标条件，不伪造正常native运行。
- A池级常驻映射、B逐次额外map/TX/unmap、最后引用、回pool/离pool/物理释放分开；XDP_TX与跨设备REDIRECT分别计费。
- T：部署明确允许相关专用池范围给A/B按规定权限持续映射至退役，仍需buffer引用、在途DMA、信任变化、整页元数据/空洞授权。只分析合同，不宣称当前部署已选择T。
- S：细粒度原始对象隔离或物理授权结束的条件分支，当前实际mlx5e部署需求未建立；不把strict实际unmap同步当逐包强制unmap。
- 两设备翻译型DMA、不同domain、共享IOMMU服务是待验证部署投影，不能用本平台16项IOTLB参数填入。
- O0原RX池+B逐次映射；O1目标持久紧凑池+复制；O2 DMA_PMD按domain首次使用建图后保留；O3同信息普通复制/短map/持久map选择，含包长/源区历史/累计费用。普通对照允许正常批处理、缓存与同接口，不人为削弱。
- 重点补核DAMN ASPLOS2018（https://dants.github.io/papers/damn-asplos-2018.pdf）：设备/权限分配接口、常驻池、读取点复制、零复制IP转发与XDP适配边界。此前shadowing/Mosaic/Ingens/fbufs核读复用，不重复全文。
- 有限判别：固定RX分配与路由规则后，源区→目标domain再用关系是否令O2首次建图、固定copy阈值和普通累计费用规则作实质不同选择。实际未来再用次数r_g只能作事后盈亏分析，在线只能用当时付费可知信息；不同domain/区间互扰须有真实费用依据。
- 全生命周期核账：源大页取得/保留/集成、目标池初始化/退役、关联表/IOVA/PTE、复制/清理/同步、回调适配和错误归属、最终释放。别名映射不是两份物理内存，较早回源池不是总内存减少。CPU/带宽/内存/完成约束分别报告。
- 若普通累计费用规则等价，当前实例可止于普通部署建议；不把手工公式当方法论文。不预认新意、共享IOMMU收益或主线。
- 不开发系统，不实现新池、不启动完整性能模型；上一轮整页转交协议后继不按旧合同执行。候选二暂不展开；B1结题与证据触发、I/D/A5A2、MIG暂停及10月15—28日窗口保持。

新引用待核，不以用户粘贴为证据：

- DMA_PMD镜像：https://lkml.rescloud.iu.edu/2610.0/06299.html
- DAMN原文：https://dants.github.io/papers/damn-asplos-2018.pdf
- DMA_PMD评审：https://lists.openwall.net/netdev/2026/10/11/76
- AF_XDP：https://docs.kernel.org/networking/af_xdp.html （只在决定普通替代能力时定向核，不强制扩展本次范围）
