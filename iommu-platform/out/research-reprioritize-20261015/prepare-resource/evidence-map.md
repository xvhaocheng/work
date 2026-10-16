# B1/Q 原始证据导航

本轮定向读取；不是全文阅读声明。`source-manifest.json` 保存路径、实际读取区间、SHA256 和证据级别。代码只证明源级能力，论文结果只属于原配置，旧接受只证明对应冻结版本。

| ID | 原件与本轮定位（平台根相对路径） | 支持 / 不支持 |
|---|---|---|
| S1 | `out/workflow-stream-contract-20261012/source-evidence/hero/openmp-examples/mm-large/mm-large.c:44–70,78–228,238–273,280–318` | 真实双缓冲、已知地址/未释放槽、A/B/C供给、最终释放、默认16KiB算术；不证明当前多DMA共享或周期 |
| S2 | `out/workflow-stream-contract-20261012/source-evidence/hero/support/libhero-target/inc/hero-target.h:65–74` | `hero_dma_job_t`只有uint32_t id；不是多页prepared描述符 |
| S3 | `out/workflow-stream-contract-20261012/source-evidence/hero/support/libhero-target/pulp/hero-target.c:58–85,134–137` | 写地址长度后读tf_id启动；wait按id/stream；没有提前纯prepare API |
| S4 | `cva6/corev_apu/tb/ariane_soc_pkg.sv:110–132` | 16 IOTLB，InclDbg=1，InclATS=0，56bit PA；不能推出实际运行BIT一致 |
| S5 | `cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_dbg_if_ctl.sv:47–109` | 纯翻译key/权限类型，结果PPN/fault、恒ready和生命周期 |
| S6 | `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:170–213,1329–1335,1520–1567` | shared arbiter、normal优先、debug/normal结果路径、fault sink；不证明免费并行 |
| S7 | `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:674–725` | 正常response恒ready；先保存结果后等内部地址交接；内部交接不是DDR外部握手 |
| S8 | `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:274–280,315–405,607–650` | 单拍PTE、firstVALID/AR接受/R退休、错误归属；不是已有任意抢占接口 |
| S9 | `out/innovation-frontier-reset-20261008/continuation-primary/rocket-2022.scala:156–175,195–218,524–566,593–599` | 固定2022原源的request destination、S/G状态、地址重建；不是当前IOMMU暂停性能证据 |
| P1 | `out/shared-translation-literature-20261006/papers/socdma2018.txt:272–304,334–348,375–412` | ICCD2018 §IV-A/IV-C，进度限定纯预取、短burst锁定/重放、完整元数据；不证明当前B接口所有细节已发表 |
| P2 | `out/shared-translation-literature-20261006/papers/mask2018.txt:820–865,867–925` | MASK §5.2–5.4 token/旁路/内存优先与quota；不声称片上全cost相等 |
| P3 | `out/shared-translation-literature-20261006/papers/riscvsva2025.txt:180–230,262–299` | DATE2025 PTE LLC/计时口径，先flush后map，原平台收益；不移植百分比 |
| P4 | `out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:100–125,355–390` | DWS owned walker/idle stealing，一个foreign whole walk；不是一PTE bound |
| P5 | `out/research-reappraisal-20261015/analysis/adjacent/axi-rtanalysis.txt:103–135,175–230,590–610,650–690` | TC2023 §2.1–2.3/Lemma10/§3.7：任意任务内到达、R/W独立、不满pipeline、有界PS服务、响应公式 |
| P6 | `out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt:20–40,75–100` | 同指令组完成、工作量优先已有；本轮未重读完整算法 |
| R1 | `out/independent-directions-20261015/ownership/assessment.md:9–99,111,118` | 六项旧版本边界、B1强对照；属于已接受综合，不替代关键源码 |
| R2 | `out/independent-directions-20261015/ownership/primary-protocol-notes.md:3–14` | 复用ATS正式语义的已有定向核读；本席未重新抓网页或PCI-SIG全文 |
| R3 | `out/r2-ownership-cost-20261013/root-result-acceptance.json:6–20,46–49`; `out/rcs1-completion-set-20261014/root-result-acceptance.json:3–33`; `out/local-read-reclaim-20261015/root-result-acceptance.json:5–20` | 冻结版本的逻辑/性能边界；不代表全族普通等价或原生收益 |
| R4 | `out/innovation-frontier-reset-20261008/continuation-prior-agent.md:39–100` | 已核专利、具体CKPT/GR构造、错误/firstVALID边界；专利本轮未重读 |
| R5 | `docs/translation-progress-frontier-20261008.md:5–27,53–67` | 旧固定五族前沿适用范围；不重新运行、不推广持续到达 |
| R6 | `docs/completion-obligation-design-20261014.md:7–27`; `docs/blocking-replacement-20261014.md:9–29`; `docs/source-drain-hit-20261014.md:9–29,43–61` | CO1/BR1/SDH1窄关闭、正常owner和W生产者必要性；不扩大为架构覆盖 |
| R7 | `out/d-boundary-source-20261015/root/decision-analysis.md:7–57` | D版本降优先级与本轮授权/暂停；不自动提升B/Q/A/I |
| R8 | `out/research-novelty-assessment-20261015/analysis-priors.md:9–33,43–67` | C2-P1补充近邻与标准递推边界；ICRT/REALM/2026NoC本文仅复用已核报告 |

重要未核项：当前 HERO 的真实映射/时序/独立 publisher；O-DBG 软件执行；完整 B1 采纳/取消/失效实现；候选与 O-PAQ-D 的物理存储/端口差；准备引发 demand 暴露延迟的实际片段；当前有限返回造成 TC2023 特定流水前提失效的完整正常实例；所有新完整性能值。它们不能填0，也不必全部齐全才允许一个明确来源与假设的条件模型。

本轮没有下载/浏览新文献。已核论文的原始 URL 由 source manifest 对应条目保留。没有全文/全领域覆盖声明。
