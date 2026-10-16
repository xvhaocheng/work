# §38 Codex 直接核查的强前作约束

2026-10-08，依据公开原文；仅用于研究新颖性边界，不作专利法律判断。

- [Arm AMBA LTI IHI0089A](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0089A_amba_lti.pdf?hash=091C20253C83E0BB218FD393595E051F343E8A52&la=en&revision=ec4c2359-5838-4f79-86ab-e1354236830a)，§1.1／§1.2.2：译址可早于排序条件，look-aside 数据不经过 SMMU。§6.2.1／§6.2.2：完成不能等待后续软件或其他 Stall 翻译，同4KiB／同上下文的结果共享受完成寿命限制。**推论**：普通缓存命中直通和译址／数据路径分离本身不能立独立创新；这些条款也不证明本原型实现了 LTI 的 LC 或失效语义。
- [PULP axi_xbar 原设计说明](https://pulp-platform.github.io/axi/xbar_parameters/module.axi_xbar.html)，Design Rationale for No Pipelining Inside Crossbar：W 不允许交错、AW 导出 W 顺序、插入 spill 可形成 Coffman 等待环。**推论**：新位置上的读写等待环并不自动成为新的原理；本轮 TC 与父保存环仍须具体证据和同约束普通对照。
- 新补 [US20250217302A1：Adaptive Configuration of Address Translation Cache](https://patents.google.com/patent/US20250217302A1/en)，2025-07-03公开，ATC Backed by External Memory／Example ATC Formats：描述外部 ATC 访问与 ATS 失效、PCIe 写／完成排序的等待环，以及独立接口／VC或内部保存失效请求的处理；还描述利用连续地址压缩 ATC。**推论**：一般“为失效留通路”“独立翻译线路”或“省重复地址”也有明确前史。该外部 ATC／PCIe 场景不是本平台 AXI DMA 的同一个环，不把它当当前 RTL 根因或已完全覆盖候选联合规则。

- [RISC-V IOMMU v20260222：Caching in-memory data structures](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html#_caching_in_memory_data_structures)：IOATC不自动观察普通CPU／DMA对页表的写入，更新需要IOMMU失效命令同步。**推论**：本轮静态normal缓存能复用，并不证明映射更新后也可直接发出数据访问；新增外部结果保存必须核相应失效覆盖。这里核架构语义，不声称本平台实现该规范最新版本。

§38 的结果只能支持选定的普通前置控制没有追平本次后置轨迹。下一须先实现普通已译址源 AR 直接绕过繁忙 TC 的强对照，核权限、页界、结果保留、响应归属与正常回退，见[Codex具体接口规格](codex-next-control.md)。若该普通对照同进度同成本，就不能把位置／联合规则作为独立新机制。若仍有差异，也必须证明来自强前作未覆盖的具体约束；性能残差不等于新颖性。
