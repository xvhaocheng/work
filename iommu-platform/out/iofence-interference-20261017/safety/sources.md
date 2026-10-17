# 来源、行号与必要原句

本页为 README 的可导航源入口。路径缩写以本页位置为基准：`OLD` = `../../dead-translation-20261017`，`SPECOLD` = `../../invalidation-interference-20261017/sources/spec-rtl`。`source.json` 保存所有关键文件的绝对路径与 SHA256。未引用二手教程作为规范证据。

## RISC-V 官方规范

- 已核在线官方仓库：[riscv-non-isa/riscv-iommu](https://github.com/riscv-non-isa/riscv-iommu)。主页说明 Ratified。官方 published PDF [v20260222](https://docs.riscv.org/reference/iommu/v20260222/_attachments/riscv-iommu.pdf) 的既有原件、文本与来源 metadata 在 `SPECOLD/spec/`；本项未重新渲染 PDF。
- [当前 main API 原响应](official/spec-main.json)：commit `97e23d078a50822bb247c564af31c2645b4c36ca`，commit time `2026-10-14T16:31:12Z`。
- [tags 原响应](official/spec-tags.json)：区分 `v1.0.1` 标签与 `20260222` dated 标签，不把二者 SHA 混写。本文固定已发布 dated tag 的原始文件。
- [在线取回回执](official/fetch-pinned.json)：固定 dated tag 和当次 main 两份关键 `.adoc` 逐字节相同；HTTP200、实际 UTC 时间和 hash 已记录。
- 主规范 [queues 原文](official/ratified-20260222/iommu_in_memory_queues.adoc) 的 [固定官方 URL](https://github.com/riscv-non-isa/riscv-iommu/blob/054005f4ffe30838a4ffe144a01ea3933ffea6f2/src/iommu_in_memory_queues.adoc#L326-L429)。§4.1.2，印刷页50–51；`SPECOLD/spec/riscv-iommu-v20260222.txt:2422–2492` 可核章节与印刷页界。

| 原文行 | 支持的判断 |
| --- | --- |
| queues 330–340 | IOFENCE 字段只有 AV/WSI/PR/PW/DATA/ADDR，无 domain/device scope。 |
| 344–356 | 普通 CQH 与 IOFENCE CQH 的完成含义不同，timeout 保持在 fence。 |
| 368–376 | PR/PW 要排序先前已处理的设备读/写，到全局顺序点。 |
| 386–397 | 回收前撤权、适当 IOTINVAL 和对应读写排序。 |
| 399–406 | 尚未 processed 的请求需要互连特定操作才能保证观察。 |
| 408–411 | 主存保证，I/O memory 由实现/协议定义。 |
| 414–420 | AV marker 在完成时写；写 fault 时 CQH 不越过该 fence。 |
| 528–536 | fence 等待先前 ATS invalidation 真正完成。 |
| 183–190,208–277 | PSCID/GSCID、host/guest 和两级失效的含义；不能等同于 DID。 |

唯一必要的英文短摘录（queues:399–401）是 **“requests that have been already processed by the IOMMU”**。本文不把这句话擅自改写为 comp AR/AW 已握手，也不推成所有在途请求。

[software guidelines 原文](official/ratified-20260222/iommu_sw_guidelines.adoc)，[固定官方 URL](https://github.com/riscv-non-isa/riscv-iommu/blob/054005f4ffe30838a4ffe144a01ea3933ffea6f2/src/iommu_sw_guidelines.adoc#L123-L310)：

| 原文行 | 章节与印刷页 | 支持的判断 |
| --- | --- | --- |
| 123–136 | §7.3，p89 | 先使页表更新全局可见，再失效；IOFENCE 命令/请求排序。 |
| 143–187 | §§7.3.1–2，p89–90 | DDT/PDT 改变涉及目录及对应翻译缓存失效，重绑不能只换软件表。 |
| 210–231 | §7.3.4，p90–91 | 二阶段改变按 GPA/GSCID；影响 DC 内已翻译字段时也须失效 DC。 |
| 233–251 | §7.3.5，p91 | 一阶段需区分 second-stage Bare 与非 Bare。 |
| 279–310 | §7.3.7，p92–93 | DevATC、PASID、先 IOATC 后 DevATC 的次序。 |

## Linux 官方源：固定 commit 58161ead29e50d70a317ea26b472e0c7508e69da

官方 [DMA howto](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/Documentation/core-api/dma-api-howto.rst#L601-L602) 已重新取回到 [本地原文](official/linux-dma-api-howto.rst)：`601–602` 要求 DMA finished 后 unmap，`666` 对 scatterlist 重申。当前 [docs.kernel.org 渲染页](https://docs.kernel.org/core-api/dma-api-howto.html) 也已在线读取；本地精确行号仍固定上述 commit，不把动态页面充当版本锁。

其余原字节复用 `OLD/sources/linux/A-mainline/`，原取回记录、commit 与 blob/hash 见 `OLD/sources/linux/manifest.json`：

| 固定文件 | 行号 | 支持的判断 |
| --- | --- | --- |
| [dma-iommu.c](../../dead-translation-20261017/sources/linux/A-mainline/drivers/iommu/dma-iommu.c) | 806–843 | strict 页表清除→同步→IOVA 归还；FQ 与立即释放不同。 |
| [riscv/iommu.c](../../dead-translation-20261017/sources/linux/A-mainline/drivers/iommu/riscv/iommu.c) | 517–535 | IOFENCE sync helper 和成功等待、错误日志。 |
| 同上 | 1005–1112 | 按 PSCID 发失效，遍历本域相关 IOMMU，再逐个 IOFENCE。 |
| 同上 | 1268–1274 | 同步后释放 gather 的 page-table freelist。 |
| [iommu-bits.h](../../dead-translation-20261017/sources/linux/A-mainline/drivers/iommu/riscv/iommu-bits.h) | 766–771 | 普通 IOFENCE helper 设置 PR=PW=1、AV=0。 |
| [既有 findings](../../dead-translation-20261017/sources/linux/findings.md) | 17–32,57–68 | 上述调用链及边界导航；本项已直接核关键源，非仅引用摘要。 |

## 冻结 RTL 与历史独立模型

共同路径前缀 `OLD/frozen/cva6/corev_apu/riscv-iommu/rtl/`，完整路径及 hash 在 `source.json`：

| 文件 | 行号 | 直接证据 |
| --- | --- | --- |
| software_interface/rv_iommu_cq_handler.sv | 241–260 | CQ 不等 TC 空闲便可开始取命令。 |
| 同上 | 276–283 | REGISTER 末读 beat 直接推进 CQH，然后进入 DECODE。 |
| 同上 | 310–339 | IOTINVAL 直接发缓存失效脉冲。 |
| 同上 | 348–394 | PR/PW TODO 在362；后面没有 payload drain 等待。 |
| transaction_controller/rv_iommu_trans_ctl_wrap.sv | 97,645–678 | in_flight=非IDLE；先寄存翻译、后等 comp ready。 |
| 同上 | 286–311 | comp PA 来自已寄存翻译响应。 |
| translation_logic/rv_iommu_tl_wrap.sv | 1351–1373 | COMPLETE 中才发送 normal response。 |
| 同上 | 550–552,573–599 | 失效接IOTLB，PTW active 未接；PTW update 独立。 |
| translation_logic/rv_iommu_ptw.sv | 35–84,393–395,478–480 | 无 inval 输入；PTE返回后可完成 fill。 |
| translation_logic/rv_iommu_iotlb.sv | 204–305,305–318 | 失效优先同拍；后续 fill 仍可置 valid。 |

历史模型与 overlay 的固定来源：

| 文件 | 行号 | 直接证据 |
| --- | --- | --- |
| [runner/invalidation_dma.cpp](../../dead-translation-20261017/runner/invalidation_dma.cpp) | 388–410 | 快照及计数来自 comp 已接受 AR/AW，含同拍。 |
| 同上 | 432–471 | 写数据、DDR RLAST/B 和 payload 完成计数。 |
| 同上 | 693–694 | 终局 cohort 守恒检查；不等于每次 fence 覆盖证明。 |
| [CQ overlay](../../dead-translation-20261017/runner/rtl-overlay/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv) | 291–298,414–426,532–539 | PRPW延迟CQH、AV/ATS范围外、WAIT_PRPW。 |
| [历史 runner README](../../dead-translation-20261017/runner/README.md) | 3–9 | 版本、只测 leaf、保守DDR点及无Linux运行等限制。 |

来源定性：RISC-V规范是架构要求的权威一手来源；Linux与RTL是具名版本行为的一手可检查源码；历史仿真只支持其输入与模型范围。安全充分性证明与反例为本项条件推导，不冒充规范逐字结论、实测事故或 peer-reviewed 定理。这里没有期刊、DOI、作者利益冲突核验的适用对象。

浏览工具已成功读取官方仓库、原始 queues 与 kernel 文档；数个猜测的 docs.riscv 路径不可访问，`src/iommu.adoc` 为404，GitHub tags经浏览工具受限。随后使用官方 GitHub HTTPS API/固定raw URL取得真实内容，未依赖猜测页面或二手缓存；回执见 `official/fetch-initial.json`、`fetch-pinned.json`。实际 main 和 dated tag 查询均已保留，不把网页失败误称为已核全文。
