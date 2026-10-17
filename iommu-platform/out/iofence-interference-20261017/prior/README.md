# Scoped drain / fence 前作门：FAIL（本版本停止）

2026-10-17。ARS deep-research `fact-check`，Codex-only。Intel 已明确提供按 domain / domain 内地址限定的数据 drain 完成保证，足以否决“首次新增此语义”的简单接口版本；root 已直接核读决定页并接受第三门失败。没有机制实现、RTL运行或板测。本结论不否定尚未提出并验证的实现成本差异。

| 官方来源与核读位置 | 谁的未完成数据必须处理、何时完成 | 对候选的覆盖与边界 |
| --- | --- | --- |
| [Intel VT-d Rev 5.0，2024-08](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf)，§6.5.2.3 印刷6-25/26，§6.5.4 印刷6-45/46；PDF116/117/136/137 | G/DID 指定全局、domain、domain内地址。DR/DW 的相应未提交主存请求须在下一 `inv_wait_dsc` 完成前 drain；读为 Root Complex 完成取数，写为CPU可见。 | **已覆盖 domain/address 范围的完成语义。** DID 是 domain ID。`only guarantees` 是最低保证，**不承诺实现绝不额外等待无关domain**。限已处理的 untranslated 请求；ATS translated请求另依端点失效完成规则，不能混为同一接口。 |
| [Arm SMMUv3 IHI0070 G.b，2025-04-30](https://documentation-service.arm.com/static/6813b2bbefb0f21122c144c2)，§3.21.1 p131/132、§4.4.2.2 p185、§4.7.3 p207（PDF页同） | TLBI 可按 ASID/VMID 等命中条目；后续 CMD_SYNC 确保相关事务在其 Shareability domain 全局可见。按被命中翻译条目关联 payload，而非仅删除缓存。 | **已有 scoped invalidation completion 的紧邻语义。** CMD_SYNC 同时等待队列此前所有命令；不能推成按SID的独立队列完成或保证不受无关流量拖延。stage2仅影响stage1描述符访问时的例外见p131，未把所有递归payload一概算作需完成。 |
| [RISC-V v1.0.1，2026-02-22](https://docs.riscv.org/reference/iommu/v20260222/_attachments/riscv-iommu.pdf)，§4.1.2 印刷50/51，PDF54/55；及 [main固定版本](https://github.com/riscv-non-isa/riscv-iommu/blob/97e23d078a50822bb247c564af31c2645b4c36ca/src/iommu_in_memory_queues.adoc) L325–414 | IOFENCE.C 完成保证此前CQ命令完成；PR/PW要求此前IOMMU已处理的全部相应主存请求到达全局排序点，命令无DID/PSCID/GSCID选择字段。 | 本轮未见按domain选择PR/PW。IOTINVAL自身的PSCID/GSCID/地址选择不缩窄PR/PW。尚未到IOMMU的设备请求另需互连同步。 |
| [RISC-V main扩展章](https://github.com/riscv-non-isa/riscv-iommu/blob/97e23d078a50822bb247c564af31c2645b4c36ca/src/iommu_extensions.adoc)，L1–228 | 已核读扩展表及QOSID/NL/S定义；S扩展IOTINVAL地址范围，NL扩展非叶失效，QOSID标注资源身份。 | 未提供IOFENCE domain选择。独立Svats文件/全部提案未核读，不能据搜索无命中声称不存在；Intel决定证据后停止扩展检索。 |

决定性短引（Intel，15词）：“only guarantees draining of non-committed read/write requests to the domain specified in the invalidation request.” 原文见[官方PDF第137页](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf#page=137)；本机抽取文本L8177及决定页截图保留在sources/。完成时点见L8169–8174，page范围见L8180–8183，DR/DW/DID字段见L6862–6880，wait descriptor见§6.5.2.9印刷6-35/36。这是规范最低保证，未测量硬件隔离性能。

完整来源、版本、实际下载时间/哈希、核读范围与复用身份见 [source-manifest.json](source-manifest.json)；[查询与失败记录](queries/search-log.json)。三组各四个主查询，另直接打开官方URL。Intel浏览器取件因体积失败、Arm E.a浏览器超时，但终端下载的两家文件均为真实PDF且关键页已渲染核看。RISC-V未版本化新URL返回404；本轮直接核读既有官方v1.0.1原件，未冒称该404下载成功。软件scoped I/O sync仅完成一次有界检索，没有核读到可追加的直接等价论文，不作“没有前作”的负面断言。

本报告为AI辅助核查；规范是各架构语义的一手权威，并非性能实测或独立同行评审证据。不主张已穷举全部规范、厂商实现或论文；停止范围只覆盖上述简单语义版本。

公开归档提供本报告、官方链接、版本／SHA256和读取位置；Intel／Arm整份PDF、全文抽取与页面截图保留本机，不随成果提交。RISC-V开放规范源码与查询记录按来源保存。
