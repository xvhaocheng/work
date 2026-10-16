# LR1 准确前作与 native 边界

具名主前作已经覆盖“撤权后只对相关旧请求取得排空保证”这一抽象动作，普通源阶段 join 也能在相同目标证明上提前归还源页。不能把 whole-copy 的保守等待或当前 RTL/driver 缺项当成普通方的必然限制。此处裁决协议覆盖和原文边界；候选的真实值/生命周期/成本最终由 root 接受。

## RISC-V IOMMU

[v20260222 原文](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) 的 IOTINVAL.VMA 允许 address+PSCID 失效并同步 PTE 更新；实现可做更宽失效。IOFENCE.C 完成保证先前 CQ 命令已完成并提交，普通 cqh 前移本身不作这个保证。PR/PW 对 IOMMU 已处理的旧读/写请求请求全局排序点；尚未被 IOMMU 处理的在途请求仍需互连相关封闭措施。只读源的回收需完整读排序，原文不提供 source/job/PA-generation reader 注册。原件及精确行号见 [manifest](critical-sources.json) 的 RISC-V 项。

因此，native flags0 不能凭命令完成当作 DMA 排空。若公共目标封闭与真实 R/旧义务已单独证明完成，则 phase 可采用 PR=PW=0 做 CQ 完成；排序安全来自已收费的目标证明。给普通程序保留这个选项，不强迫无关 DMA/globalquiet。

## Intel VT-d

[完整 Architecture Specification Rev.5.0，August 2024](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf) §6.5.2.3、§6.5.4 明确：domain-selective 排空保证覆盖指定 domain；page-selective 保证覆盖该 domain 中 untranslated 地址与指定范围重叠的 non-committed 请求。排空对象已被 remapper 处理、还排在 Root-Complex 内，目的为主存；范围是保证下界，不能据此承诺实现从不做更宽处理或其他 DMA 零延迟影响。

读 drained 的完成点是 Root-Complex 已从 memory 取完数据，不能当作 DMA 已收齐 4096B。AT=10 translated 请求要结合 DevTLB/ATS；raw、heldVALID 和未处理请求不属于上述已有 remapped 排空保证。queued DR/DW 的完成点为下一 Invalidation Wait 完成；硬件版本与能力决定 flag 行为，Rev.5.0 对 MajorVersion≥2 的 queued invalidation 描述为总是排空 relevant 请求。精确原文：PDF 物理 p136–137，印刷 6-45/6-46，text:8130–8186；排空 flag 在 p117，text:6862–6875。

[13th Gen IOTLB_REG 页面](https://edc.intel.com/content/www/xl/es/design/publications/13th-generation-core-processors-datasheet-volume-2-of-2/iotlb-invalidate-register-iotlb-reg-0-0-0-vtdbar-offset-508_1/) 是寄存器入口；网页关键段的重取和本机取回超时。本席成功保存完整官方 PDF（12,811,634B），scope 判断以上述 PDF 原文为准，不靠网页片段推断 page drain。原件、编号 extract 与 SHA256 均在 [critical-sources.json](critical-sources.json)。

## 本机关键源

| 关键源 | 本次亲核的事实 | 对裁决的含义 |
|---|---|---|
| `rtl/software_interface/rv_iommu_cq_handler.sv:312–317,348–385` | 失效地址/context字段已向 iotinval_o 传出；362 明写 PR/PW R/W counter support TODO | 不能假定 native 已有标准 DMA 排空；也不能把实现标准 PR/PW 的维护修复当研究新动作 |
| `linux/drivers/iommu/riscv/iommu.c:359–363,560–565` | 默认 IOFENCE 构造没有 PR/PW；sync 调普通构造器并同步提交 | 只说明本版本路径，不能推出所有普通程序必须等全部目的写或全系统静默 |
| 同文件 `1730–1771` | 当前 first-stage PSCID 以及按范围设置 ADDR/逐页提交代码被注释 | 精确失效基线的功能补齐应计工程成本；标准接口已有这个能力，不把补齐本身当创新 |

本机 IOMMU HEAD `57eb5621802ef9ed8d77c684b9a03205c6630391`，Linux HEAD `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`。关键原文 extract/hash 保留；本席没有修改这些生产源。

## 接受／否决条件

若候选只是“相同的付费注册、封入口、真实整页 readreceipt、旧义务退休，然后撤权/同步/失效/排序/ACK”，独立 phase 已给出完整同动作控制流程，不能据此宣称独有控制动作或独有提前归还收益；成本差须另有原始实现证据。因为 VT-d 原文已有 domain/page 相关排空，不能把相关旧读局部排空的抽象能力作为独立新颖性。

若候选有上述普通流程与准确前作未提供的具体信息产生机制或完整实现成本差，必须保持相同撤权需求、注册完整性、真实 R、旧新 generation 和错误生命周期，再用相同预算的完整 ordinary 程序比较。当前 API 布尔位不证明这种差异，逻辑 paired 成本相等也不证明实测面积相等。论文主贡献、性能与 native 实现均不由本席的字符串策略或源摘要自动成立。
