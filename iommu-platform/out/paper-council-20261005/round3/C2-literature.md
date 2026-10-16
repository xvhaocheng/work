# C2 R3 原始来源：允许结果、完成边界与强简单对手

日期：2026-10-05。只核对与 C2-L 收缩、C2-O 去留直接相关的原始材料；没有执行原文 artifact、测试或本地构建。PDF 页码从 1 起；行号指本次网页解析位置，不是本地版本证明。这里引用的 GitHub main 未固定 commit，后续实现应另行固定。

## O-L1：RISC-V IOMMU 固定版规范

[v20260222 In-memory queue interface](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)。定位 §Page-Table cache invalidation 与 §Command-queue Fence。

- L63–70：IOATC 可能不自动观察软件 PTE 修改；IOTINVAL 同步相应表项更新。
- L91–94：允许更大范围失效；未执行覆盖性失效时允许旧/新翻译。支持 O1，不能据单个模型选择强制所有合法实现一致。
- L99–101：普通 CQ head 前进对执行完成是 **“not a guarantee”**；IOFENCE 完成的 head 语义不同。支持 O2。
- L107–113：PR/PW 作用对象是 **“already been processed”** 的先前读/写，到全局排序点；未处理流量另需互连 fencing。支持 O3/O4/O6 的区别。
- L114–116：AV 写回是完成通知方式之一；后续使用仍要遵守平台内存观察顺序，不能把软件随意读标志假设为充分同步。
- L135–139：ATS 失效可等待 completion 或 timeout，IOFENCE 对超时有错误路径。本轮基础模型排除 ATS，但不声称规范/参考模型不知道异步完成。

O1–O6 是 C2 自行构造的教学/验证实例，不是规范现成 litmus，不是板上观察。特别是 O6 的 indistinguishability 是模型推导，不能归给规范作者。

## O-L2：官方 reference model——不仅是地址函数

- [README 原文](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/README.md)，已读 L53–140 的 API。
- L77–80 定义 `iommu_to_hb_do_global_observability_sync(PR,PW)`：testbench 负责相应全局可见性动作，函数返回才作已完成假设。
- L120–140 给翻译入口、ATS invalidation completion、timeout 和命令推进接口。
- 直接源码：[iommu_command_queue.c](https://raw.githubusercontent.com/riscv-non-isa/riscv-iommu/main/iommu_ref_model/libiommu/src/iommu_command_queue.c)。已核对 `do_iofence_c`（网页 L528–590）：L537–548 等 ATS；L552–559 超时错误；L566–567 调全局可见性 callback；其后处理 AV/中断。不是仅凭 README 猜测接线。
- 对手构造的依据：简单 testbench scoreboard 可以接这些既有接口。未运行此模型、未查全体测试覆盖，因此不宣称它已经验证当前 RTL 所有竞态或天然代表规范全部允许行为。

## O-L3：TransForm

[TransForm: Formally Specifying Transistency Models and Synthesizing Enhanced Litmus Tests](https://arxiv.org/pdf/2008.03578)，Naorin Hossain、Caroline Trippel、Margaret Martonosi，直接读取 14 页原始预印本。

- 已核对第 1 页贡献范围、第 7 页 §III-B2/C（网页 L671–746），第 9 页 §IV（L952–976），第 11–13 页 §VI（L1150–1264）。
- 明确包含页表写、隐式 walk、TLB 失效关系，以及带最小性/有界覆盖条件的 enhanced litmus 合成。失效后用旧映射已有禁止实例。
- 其已讨论 COATCheck；本轮未另读 COATCheck 原文，不把转引当独立全文核查。
- `find(IOMMU)` 无命中不证明它不能扩展到 IOMMU，也不证明 C2-O 新颖。C2-O 必须与直接扩展的关系模型比较。

## O-L4：Relaxed virtual memory in Armv8-A

[ESOP 2022 作者原文，31 页](https://www.cl.cam.ac.uk/~pes20/RelaxedVM-Arm/RelaxedVM-Arm-esop2022.pdf)。另有 240 页扩展版，但本轮没有通读该扩展版。

- 已读第 1–3 页范围/贡献，第 7–12 页关于 TLB 缓存、失效与同步的关键段落。
- 已有架构允许结果包络、翻译并发关系、litmus 与 Isla 计算工具；模型覆盖多级/多阶段，也明确列出不覆盖的功能。
- 这是比“通用 differential fuzzing”更直接的强先例。C2-O 不能将“允许旧/新结果＋部分序＋litmus”当新方法。
- 本轮没有验证其与某个 DMA/IOFENCE 接口完全同构，也不能由 SMMU 关键词无命中宣称未解决相关问题。

## O-L5：ShareMMU 补核定量部分

[NVIDIA 原始发布页](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)；[camera-ready PDF](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf?VersionId=5BjALNe5CpmAmE5xIkXa6I31Ml7zg1ZO)。直接 URL 一次失败后，经发布页文件链接成功读取；没有下载/运行 artifact。

- 复核第 8–9 页 §V-C7/8、Fig.11（网页 L730–797）：撤销瞬态、旧 hit 完成、miss 等待、drain 后改权限，已有有限硬件时间与最终推进论证。
- 新读第 11–12 页 §VII-A/B 开头（L1040–1105）及评价段落定位：性能开销、权限变化工作负载与敏感性；未在这些部分找到独立 DMA deadline 和撤销 deadline 的联合准入可行域。
- 这个有限“未找到”不支持无人做过的结论。它仅把已知重叠与仍需证明的差异分开；不因 deadline 字面无命中就宣布没有定量保证。

## 本轮未执行与未确定

没有复现任何作者结果；没有证实当前平台安全漏洞；没有比较 oracle 与 scoreboard 的实际覆盖或成本；没有给 C2-L 新调度找出击败强简单方案的反例。当前文献足以压低创新主张，尚不足以证明任何新方向已经可发表。
