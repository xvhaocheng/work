# SDH1 frozen functional contract, 2026-10-14

问题：生产TC以bank为翻译生命周期，N限定异bank。当前W所需的另一完整源AR可能在同bank normal miss之后；直接放宽bank条件会把PA交给错误TC。源码边界不是一个评分不足问题。

新增动作：原raw AR中每完整源的最老请求，且与active miss异源；准确W头同job/匹配copy字节区间、其physical AW已经握手、此前W独立可排空、真实N受限权限hit成立时，占用现有单个N OUT并用(source,slot,generation)连接原header。物理AR接受前不移除header，不改变normal owner。同ID前序、任何已发布VALID及已接受请求不能覆盖。lookup失效/normal完成优先，正式权限/guard保留。

私有原型接口：原两bank各四AR槽；普通队列/normal owner/一辅助结果槽；请求full native IDs0/4/8/12；job与copy区间、W头和actual AW账本；缓存/权限输入是显式已观察记录。不会将初始化的hit记录称原生已建立命中或免费预热。参考原型验证协议状态和真实payload值，**不是native RTL、真实DDTC/cache/PTW模拟或性能模型**。Python对象/日志不是免费硬件存储的证明。

root冻结限制：辅助槽已占用时不可再grant；invalid/normal完成待办时不能新probe。grant后PA/ID/header held至physical ARaccept，失效完成必须等待这项义务而不能撤VALID。原型每normal walk最多一次protected grant，明确为保守有限公平策略，同资源普通可采用相同策略。生产gen宽度G与回绕排空规则尚须实现，原型用单调整数防止伪造零成本。

进入性能条件：候选相对可执行generic single-PTW nonblocking+owner存在具体安全动作差异，或完整可实现控制/资源成本优势及有符号全完成余量。若完整普通同路径同信息复现，则保留普通功能原型，0性能条件。area/Fmax、native完成时间、CPU/GPU收益均null。

次候选：原DMA0/1各4096B，两个目的AW在首次向IOMMU VALID之前门控；source端原两AW spill不是IOMMU许可。读收据须覆盖整页/错误，而非一段RLAST；可信物理对象/映射epoch+所有读者先保存原值，再开放AW并完成W/B/consumer。纯payload swap读写各8192B，scratch各12288B；首先归DMA协作，不算IOMMU方法收益。

不重跑旧实验/模型/验证器/构建、不恢复关闭分支/旧席、不改变压力/容量/walker/供给时相。goal1791986539无额外预算，最多两工作日，五页同步及新检查通过后才能complete；论文未完成。
