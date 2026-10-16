# Mac 本地任务交接：Genesys 2 首次上板

## 2026-10-04 N原型草案审查

同设备上下文初版已否决：不能证明DID11/DID10之间的阻塞解除，原AR队头与年轻V结果的身份交付尚未闭合。修正约束见docs/research-n-prototype-spec-20261004.md；下一批仅闭合跨上下文请求/结果生命周期，不启动无效草案编译。无N实现/构建/板测，未要求更新SD；现00093仍是已确认的secondboot，UART关闭。


## 2026-10-04 method88完成与FPGA存活确认

- method88 已正式完成：88/88 host_exit0，原始已回传。本机归档：`results/experiments/20261004-method88-ssh-01/`。正式报告：`results/experiments/20261004-method88-ssh-01/DELL_RESULTS.md`；文献矩阵：`docs/research-literature-review-20261004.md`。
- 执行事实：kernel `6.5.0-rc2-00093-gc0f5e13ea81f`；boot 1 `93406a68-fc8a-4040-9dc6-ae4e4a670d36`（复用既有启动，含旧 462 历史）；boot 2 `236b6316-cca3-49be-b52b-48f3963fa515`（新启动）；4 short + 84 long。UART 已 live 返回 uname `00093` + bootid second，确认为 second boot，无需重启；串口已关闭。
- 判定：p5 对最强匹配简单对照四格全负，跨方向 False → 固定 A8 主创新路线结束，p5 转为已测试候选/基线；不继续为 p5 找有利条件。
- E2 已完成 64，不是尚无条目替换直接证据：Linux `00093`/role1 新版 64 = 短 4、边界 30、前缀 30，64 clean/exit0；128 target 中 62 个替换后 firstmiss、66 保持 hit，无未决。W7 正反各 5 次保留两 target，W8 正反各 5 次均替换；128 target 非 128 独立试验。证据 `out/e2-results-review-20261003/RESULTS.md`；只限有限布局/实现范围，不能推持续吞吐归因。新 88 不是重做 E2/E3。
- 不要写成：窄幅反转、actual cost 独有收益、每 copy miss 固定、0xE2E 作真实服务分母、p5 漏计 DDT/PDT/PTW、E2 尚无条目替换直接证据、SMMUv3 已实测显著吸收干扰。
- 新 primary 是 Memory Latency Distribution-Driven Regulation，ECRTS2023，不是 RTSS2022；latency CDF 反馈限流已存在；仅把平均吞吐反馈改成 CDF 不自动新颖；IOMMU vs DRAM 差别须具体规则证据。
- 下一动作：本轮无 Linux/RTL 改动、无新构建、无 SD 更新。当前没有 N/P 实现和足够 novelty 差异，不能称已选好新方法或创新成立。当前正在做标准 N 接口/资源/失效/顺序规格与关键 RTL 可行性对照，再做 S/N/P 标准方法强基线/设计取舍实验；若仅常规优化无增量则不编大工程。读现有源码不是多余 hash 回读。用户授权 SSH 自主推进到 manualSD 前为止，但继续实现不授权无科学价值工程，不强迫 needsLinux 作为停工理由。

## 2026-10-04 最新授权与 method88 SSH 执行中

用户授权 Dell 通过 ssh mac 自主执行实验、分析、计划、实现与后续实验，直到需要人工写入 SD；这覆盖此前“仅保留原始数据”限制。不要额外回读、解包验收或重复检查；保留既有内置错误停止，不自动重跑。

本轮结果目录 results/experiments/20261004-method88-ssh-01；host.log/host-exit.json/launch.json 为同名前缀。首批44接入既有role2启动93406a68-fc8a-4040-9dc6-ae4e4a670d36，无初次BIT重载；第二批44使用同BIT的新启动。冻结88参数不改，部署修订在首样本前记录，不能称两次全新启动。完成后保持Linux运行并关闭串口；原始结果回传Dell后分析。尚无本轮科学结论。


## 2026-10-04 最新：两 BIT / Linux00093，526项原始记录已采集

完整原始目录已回传Dell并核对无差异，三份Dell交接已更新并回读确认，回执见results/experiments/20261003-research-two-bit-continued/DELL_RECEIPT.md。

E2 64项、method 462项全部保存，主机续采集进程退出0、COMPLETE.json已生成。原始目录 results/experiments/20261003-research-two-bit-continued/；Dell目标 out/mac-research-two-bit-20261003/，实际回传状态见本轮DELL_RECEIPT.md。原始首次tar-z采集停止现场独立保留于remote initial-transfer-failure/；第1项仅恢复采集，未重跑。role1 boot64d4328b-ba1c-4e35-9044-7409fd158027、role2 boot93406a68-fc8a-4040-9dc6-ae4e4a670d36。保留原runner自动产物，不新增结果分析；按用户最新要求只保留原始数据，不作收益结论。Linux保持运行，主机采集已结束、UART释放。无新增编译、试次或独立验收。

## 2026-10-03 最新：fullstats v1新BIT/00092，21/21在线完成

配套新BIT＋Linux6.5.0-rc2-00092-g5853e425caf8；boot_id=9a14f4f9-514e-4d18-93f2-b987b2e3bce6。cost-fullstats /tmp/research-fullstats-01仅执行一次，21/21原runner通过，final/host退出0、collection_failed0。完整原始结果results/experiments/20261003-research-fullstats，回Dell out/mac-research-fullstats-20261003并核对内容一致，交接已更新。尚未独立离线语义验收/统计分析，不跨版本拼接；没有追加试次或重跑旧E2。Linux保持运行，UART释放。

## 2026-10-02 最新：Solo修复00091，cost-followup 11/11完成

新内核6.5.0-rc2-00091-g9ad05d00cca7、原研究BIT；boot_id=db14364a-c046-4e6a-8ae7-f4dea007724a。固定cost-followup仅运行一次，11/11原runner通过，final/host退出0、collection_failed0，Solo未再EINVAL。完整证据results/experiments/20261002-research-followup，回Dell out/mac-research-followup-20261002。旧84项状态不改，不跨版本拼接；未新增独立验收或分析。Linux保持运行，UART释放。启动曾停SPI初始化，1MHz重载同一BIT后恢复，没有构建或代码改动。

## 2026-10-02 Research继续完成：累计80通过/2失败/2未运行

用户要求80项能跑先跑，本轮新增77项CLI/原runner通过；ordinal67主Solo同样Invalid argument，78/83同参数Solo未运行。原3项通过＋本77项共80通过，另2项失败2项未运行；E2 63项、非Solo cost17项完成，未独立语义验收。只机械改临时runner选取范围，未重跑已尝试项/修改驱动RTL/构建。Linux00090保持运行，UART已释放。完整两段数据和脚本见results/experiments/20261002-research-cont，回Dell out/mac-research-cont-20261002；原首次4项目录保留。

## 2026-10-02 Research v1最新：第4项失败，Linux保持运行

新配套BIT以1MHz成功加载、SD启动Linux00090；执行一次platform-research all /tmp/research-v1-01。前3项E2短试次CLI exit0，第4项short-cost-solo返回Invalid argument/exit1，停止后80项未运行。未重跑、未修复、未新增独立验收。完整原始现场见results/experiments/20261002-research-v1/，回Dell out/mac-research-v1-20261002；交接已更新。Linux保持运行，UART已释放。下方00089运行状态为历史记录。

## 2026-10-02 最新：剩余29次已完成，60份计划结果已齐（跨启动）

本轮 results/experiments/20261002-p4m-recovery/：29/29原runner检查通过，exit0、data_errors0、完成数正确、taint0。先前额外6份已离线复核；总计53在线通过＋7离线追认，共60份、每gap20，不能称为同一启动60次在线通过。用户断电后使用原BIT/SD恢复，新boot_id=e6d264eb-e2a2-43e8-8136-8a7e10363e56，Linux00089。Linux保持运行，UART已关闭，无追加实验或构建。

完整证据已取回，Dell目标out/mac-p4m-remaining29-20261002；回传最终回执见本轮DELL_RECEIPT.md。下面旧“35未跑”条目仅为历史记录。

## 最新：formal60 停于第25次记录检查；Linux 保持运行

本轮 `results/experiments/20260920-215406-run-cicggil4`：计划 gap0/1000/2000 各20，4096/256，20块交错；实际24通过（各8）+第25检查失败+35未运行。runner/host exit5；第25CLIexit0、双方data_errors/quarantined0、taint0。前dmesg seq1..36、后seq2..37，总数仍36，继承检查错误地要求总数+1；原始seq37存在，不是已发现DMA数据错误。不重跑、不修被测代码、不重启Linux，已关闭Mac串口采集。Linux00089/P4Mv7仍复用当前启动。

14项完整证据已回Dell `out/mac-p4m-formal60-20260920-215406-run-cicggil4`，逐字节读回一致；HANDOFF/CURRENT_STATUS/validation已更新。见本run的FAILURE_HANDOFF.md与两份回执。统计保留第25但不算通过，gap2000 n9不是9通过；正式60未完成。后续修脚本/续跑需新授权。

Mac DeepSeek全局桥接1.4.0已安装（41测试、真实工具往返通过）；终端/补丁请求由DeepSeek发起、Codex审核执行，不是独立无审批终端。新MCP工具需重启deepseek刷新，当前可用全局命令入口；详见 `tools/deepseek-global/INSTALLATION.md`。本轮未将FPGA串口交给DeepSeek。

## 最新：00089 start-control 12/12完成并回传

run `results/experiments/20260920-211940-run-46nvmwgi`，UART `results/serial/20260920-211934-console-clug0et5/console.raw.log`；JTAG `results/jtag/20260920-212011-program-blrjlube`。复用P4M v7 BIT，SD00089成功启动，执行交付 `platform-p4m-start all /tmp/p4m-start-01`，4短+8 BPPBBPPB全通过内置检查、退出0、taint/data_errors/quarantined0，无试次重跑。完整原始目录归档105项含根目录；tar -z不支持后改普通tar成功，仅采集兼容性问题。已退出root关闭串口。

12项证据回传Dell `out/mac-p4m-start-20260920-211940-run-46nvmwgi` 并回读一致；HANDOFF/validation更新成功，见本run两份回执。B/P硬件MiB/s中位108.81305/108.85767，四对cycles比0.999413682/0.999699263/0.999995512/0.999888800，8长Vmiss/S1=0/0、A整轮5140/5140、P开窗AID=6。短A1启动跨度较大且dmesg有hrtimer警告，保留不确诊因果。仅描述性接近；正式gap0/1000/2000的重复数和顺序待确认，不能自动继续实验。旧gap500反例保留。

## 最新：00089 start-control 已写 SD，等待用户插回 FPGA

交付 `artifacts/software/p4m-start-control-20260920/delivery`；内核6.5.0-rc2-00089-gefd9461d6459，复用现有migration-4dma-p4m BIT。写卡记录和旧启动区备份 `results/sd/update-wvxp4i78/`，仅第一分区，跳过payload哈希/SD回读，不声称板上通过。下一步开启完整串口采集、按需加载同BIT、确认00089后执行 `platform-p4m-start all /tmp/p4m-start-01`：4短＋8个BPPBBPPB对照，失败停不重跑，所有结果默认回Dell并更新交接。详细可复用步骤见P4M_START_SD.md。

## 最新：2026-09-20 P4M 21 次预试完成

完整 RESULTS.md 已补传；Dell HANDOFF.md / docs/validation.md 已更新并回读一致，见本run的handoff-receipt.json。正式参数仍未冻结，未启动额外实验。

run `results/experiments/20260920-202441-run-9_mfua8v`，UART `results/serial/20260920-202440-console-stavj2fr/console.raw.log`。在既有 00088/P4M v7 启动上运行，4096 transfers/warmup256，7 条件各3次、固定V0/A1，按Dell预定3block顺序，未重试。21/21 CLI exit0、success/status0、V4096/4096、data_errors/quarantined/taint0；总退出0。已退出root并关闭串口。8份核心证据已回传Dell `out/mac-p4m-pilot-20260920-202441-run-9_mfua8v`，回读一致，见upload-receipt.json。报告和交接更新后记录在对应回执中。

本轮observer seq5..25；seq1..4为既往short，不能混入。第三block A8gap0 cycles7248503、Vmiss/S1PTW2/2、A整轮miss/S1PTW5141/5141；前两次A8gap0约775万cycles、Vmiss0、Amiss6160。保留全部样本，不判定最终根因，不宣布batch与P4M等价。按用户偏好未做独立验收或主机回归，未核验产物哈希/SD回读。仅本轮21预试获授权，正式采样未开始。

## 最新：P4M 四组交付短实验已完成并回传

用户明确运行platform-p4m short /tmp/p4m-v7-01，4组Solo/A1gap0/A8gap0/A8gap1000均exit0，V64/64、data_errors/quarantined0、finaltaint0。run results/experiments/20260920-180713-run-l7cin11d，UART results/serial/20260920-180706-console-t_rvh11n/console.raw.log。7项原始证据已回传Dell out/mac-p4m-short-20260920-180713-run-l7cin11d并回读一致，HANDOFF/validation已更新；METRICS.md为补充指标。V推算MiB/s282.30/148.92/103.58/146.58，同窗口A完成0/64/64/32；整轮A miss/S1PTW0/112/72不得与窗口统计混同。没有新增独立验收脚本/回归，没有重跑加测。已退出root关闭串口。正式参数待确定，不能自动扩矩阵。DeepSeek整理与修订表格，Codex核对口径并实际执行/回传。

## 最新：2026-09-20 P4M00088 已启动到登录界面，仅启动

新BIT加载完成，串口实见6.5.0-rc2-00088-g9f8da9e7a6a0及Buildroot登录提示。未登录/未跑short或正式实验，不做独立验收。串口已关闭；run为results/experiments/20260920-175705-run-rsn6mvks，完整启动日志results/serial/20260920-175646-console-affb7y5d/console.raw.log。6项启动证据已回传Dell out/mac-p4m-boot-20260920-175705-run-rsn6mvks，远端回读一致；HANDOFF及validation已更新，回执在run目录。正式参数未冻结，等用户选择实验范围，不擅跑交付中的4组short。DeepSeek重试偏好已写本项目AGENTS.md。

## 最新：2026-09-20 P4M / Linux00088 已写 SD，待接回 FPGA

Dell 新交付 `/home/xvhaocheng/work/iommu-platform/out/p4m-baseline-20260920/delivery` 的8个必要文件已下载到同名本地 artifacts/software 子目录。配套 payload29363656字节、新BIT11443722字节。Linux00088-g9f8da9e7a6a0、CVA6 e822bc9d、profile migration-4dma-p4m；两者必须配套。sd_card.py/fpga.py已加最小版本入口，fetch_p4m.py提供复用下载与元数据读取。未修改底层写卡逻辑、驱动/RTL或板端runner，未跑测试/验收。

登记的disk5容量/分区UUID/偏移确认；旧第一分区备份 results/sd/update-v6_09g7q，写入完成状态 written_not_readback_verified，已sync并diskutil安全推出，第二分区未写。按用户偏好未哈希/回读，不称验收通过。本轮尚未加载新BIT/启动Linux/运行实验，也没有新实验结果待回传。用户已被询问：交付只有4组short且正式参数未冻结，是只启动还是明确运行这4组；默认遵循不跑短验收，不擅扩实验。等待用户把SD接回断电板卡、连接JTAG/串口后继续。

DeepSeek适配请求长时间未返回而终止等待，未获得/采用提案；Codex接手版本适配、审查及实际写卡。旧数据保留。以后实验原始结果仍自动回传Dell。

## 最新：2026-09-18 v6 / Linux00087 实板完成

交付完成：完整16文件已传到 `/home/xvhaocheng/work/iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/`，远端逐文件回读比对通过；Dell HANDOFF.md 与 docs/validation.md 已添加顶部记录并回读确认。本地 upload-receipt.json / handoff-receipt.json 保存回执。最终主机111项/25子测试通过，short/main验收均PASS。归档run/summary保留上传前状态，当前完成状态以此处和本地run/回执为准。不要重新运行或重复上传到同一目录。

已加载 migration-4dma-e3-controlled 新 BIT，STAT0x501079fc，观察器 v6 与 launcher magic 匹配，Linux00087 正常登录。固定 3 短 + 8 正式对全部执行，22 轮成功、418 条完整观测、taint/data_errors/quarantine0，无重试/扩矩阵/重编。短试次独立检查通过后才运行正式阶段。已退出root并关闭串口，未改printk。结果表与解释见 E3_CONTROLLED_RESULTS.md；实验目录 results/experiments/20260918-164635-run-rk48bhte，串口 results/serial/20260918-164542-console-libjqis3/console.raw.log。实时printk有破碎字符，但板端完整dmesg回显完好，原始字节保留，收集器仅用各阶段完整dump按seq取观测。原始JSON已逐项与UART核对，保留uint64精度。

正式cold12/12触发，hot0/4，V miss0，cold A miss/S1 PTW每轮2。4对cold互补的V剩余等待D128−D0为127/129/129/130周期；D0/D0基线两对差−2/0。身份/PTE/前史计数/插入身份一致，但触发前时间仍有差异，仅正式对6两项前置时间完全相同；有限证据支持延迟传播，不自动宣布纯D因果或进入性能/缓解机制实验。完整回传状态以该目录 upload-receipt.json 与 Dell 顶部交接为准；没有回执不能声称回传完成。下方为历史准备状态。

## 最新：2026-09-18 v6 / Linux00087 SD 准备完成

用户要求拷贝到SD。Dell最新e3-controlled-20260918交付已下载19个必需平面文件（未下载不用于本次操作的MCS/Image/旧结果复核）；DeepSeek起草最小版本适配，Codex复核并写卡。登记的disk5容量/分区UUID/偏移确认，备份results/sd/update-vfcfn3na，写第一启动分区，状态written_not_readback_verified，已同步并安全推出。payload 29363656字节，期望SHA 3ec5ff7f5e376e94862f148739c10a0a2f487f7ca5e8afa755e08bf21f40a914，未计算文件哈希/未回读（用户要求）。新BIT b55f5afbba667a7540ed003dd724d078e0e6e4bff5d30fe7e796abba5b7f2ecc 已下载，尚未加载；必须profile migration-4dma-e3-controlled，不能沿用v5。Linux应为6.5.0-rc2-00087-g2de370cd344e。下载scripts/fetch_e3_controlled.py，sd_card.py/fpga.py已支持。仅准备，不重编/不运行实验；等用户接回断电FPGA并连接上电后，先记录串口、加载新BIT，按交付E3_CONTROLLED.md及内置platform-e3-controlled流程执行3短+8正式对、22轮（预期418观测，尚未实测），失败停、无补采，完整结果默认回传Dell。不得复用v5旧采集器验收新controlled schema。当前新交付手册在 artifacts/software/e3-controlled-20260918/delivery/。

## 最新：v5 完整结果已回传 Dell

用户明确要求以后每次结果默认回传；已写项目 AGENTS.md。此次10个证据文件共843692字节已通过 scripts/return_e3_queue.py 回传至 /home/xvhaocheng/work/iommu-platform/out/mac-e3-queue-20260918-130545-run-d01lc9wp，远端逐文件回读比对通过，本地回执在该实验目录 upload-receipt.json。Dell /home/xvhaocheng/work/HANDOFF.md 与 iommu-platform/docs/validation.md 已追加新结果并读取确认，既有内容保留。归档run/summary保留上传前快照，本地run记录最终completed_uploaded；不要重新上传覆盖归档。未重跑实验。DeepSeek起草回传脚本，Codex审查、执行并核对。当前Mac全局AGENTS已重读，连接程序实际initialize返回版本1.3.1，默认393216、thinking enabled/high；以后预算及空输出恢复按新规则。以下“本地保存/未回传”均为历史状态。

## 最新：2026-09-18 E3 v5 实板 26 子轮完成（本地保存）

新 migration-4dma-e3-queue BIT 已加载，STAT0x501079fc，实读外层magic0x49444d414f425305；Linux00086启动。实验 results/experiments/20260918-130545-run-d01lc9wp，UART results/serial/20260918-130407-console-49jwu1pv/console.raw.log。3短+10正式配对均通过，26个不同sequence、494条去重观测，data_errors/quarantine/taint0；已恢复printk、退出root并关闭串口。正式A8 16/16触发、A1 0/4，A8目标V首次miss16/16、插入截断9/16、实际触发旧队头为A共9/16；不能解释为纯D128效应或HOL证明，无补采。结果 E3_QUEUE_RESULTS.md；scripts/linux_e3_queue.sh、collect_e3_queue.py、e3_queue_fields.py及assess_e3.py queue模式。35字段测试、4类坏输入拒绝检查及旧00085回归通过。DeepSeek起草脚本/校验/结果，Codex修正读写方向、lane序号、触发时间语义并执行验证。原始记录未改动，仅重解析；早期“write”失败是主机校验器错误而非实板失败。全部仅本地保存，未回传Dell，未改远端交接或RTL，未重编。下一步若做新机制控制需单独授权；本轮矩阵已结束，不自动重跑。

## 最新：2026-09-18 E3 v5 / 00086 已写卡

用户要求 DeepSeek 阅读新交接、按手册 copy 到 SD。DeepSeek 已返回只读审查，Codex 直接复核并执行。e3-followup-20260918 全交付已下载；原 disk5 身份核对后只写第一分区，备份 results/sd/update-h4tbwue2，最终 written_not_readback_verified，已同步、安全推出。用户要求跳过下载哈希/写后回读；未重新编译，未上板。必须配套新 migration-4dma-e3-queue BIT（已下载，尚未 JTAG 加载），不能用 v4。脚本 fetch_e3_queue.py、sd_card.py、fpga.py 已支持新交付；说明 E3_QUEUE_SD_UPDATE.md。下一步等接回 FPGA，先记录串口再加载新 BIT、确认00086；旧 assess_e3.py 的 bank v1 / schema / 矩阵规则需适配 v5，之后按手册3短+10正式配对，不补采。以下00085内容均为历史。

## 最新：00085诊断回传完成

用户明确回复“同意”后，short/main全部原始JSON、诊断、观测和完整UART已回传dell out/mac-e3-diag-{short,main}-20260918-081411-run-1z6a407q，服务器HANDOFF.md及docs/validation.md已追加结果。未重跑实验；下节“回传待批准”为历史状态，现已解除。远端summary的run为上传前状态快照，不覆盖归档原件，本地run.json记录最终上传完成。

## 最新：00085诊断实板完成，回传待明确批准

现E3 BIT重载、STAT0x501079fc、Linux00085启动。3短+6正式对共18轮通过，data_errors/quarantine/taint0，新增字段及19条观测/轮检查通过。正式A8八轮全触发、A1四轮无触发；A预热全256提交/255完成/1排空、A8末页7相同，但预热miss首512→次312/318。有A1第二轮软件启动间隔16.9236ms；不能称D128效应/修复或HOL证明。结果E3_DIAG_RESULTS.md，实验20260918-081411-run-1z6a407q，已退出root/关闭串口。上传被权限审核明确拦截，尚未回传/未改服务器文档；必须先获用户明确批准上传本批完整UART及诊断数据，不可绕过。DeepSeek字段代码调用失败由Codex接手，后续DeepSeek证据表已返回并经Codex直接核查。

## 最新：00085诊断软件已写卡

e3-diag-20260918已下载，仅payload写入原disk5第一分区，备份results/sd/update-iysskhly，状态written_not_readback_verified，已安全推出。复用现有E3 BIT，不编译/不换BIT。00085尚未上板；说明E3_DIAG_SD_UPDATE.md。后续D0/D0新schema需修改采集器，不能用旧互补D检查；3短+6对(A8,A8,A1,A8,A8,A1)，无触发不补采。DeepSeek提供只读接入审查，Codex复核并执行实际写卡；用户强调已编译，只需写卡。

## 最新：2026-09-17 E3实板采样完成，因果配对条件未满足

新BIT/Linux00084正常，STAT0x501079fc；3短+12正式配对共30子轮正确性/观测检查通过，data_errors/quarantine/taint0。正式10对A8首轮触发0/10，第二轮8/10，两轮均触发0/10；第一轮V miss0/A6160，第二轮8次V2、A5133/5136，明显轮次状态差，不能解释为128周期因果效应。2对A1四轮均未触发。详见E3_STAGE_RESULTS.md，实验20260917-222837-run-p0lbvbv7；脚本linux_e3.sh/assess_e3.py/collect_e3.py。没有补采、扩矩阵或修改RTL，建议先dell复核双轮状态/触发覆盖。已恢复printk、退出root、关闭串口。

## 最新：2026-09-17 E3配对版准备完成

新交付e3-service-20260916已下载，migration-4dma-e3 observer v4，Linux00084。原disk5卡身份确认，写卡备份results/sd/update-pfb48x8t，状态written_not_readback_verified，第二分区未写；已安全推出。下载哈希/回读按用户要求跳过。fetch_e3_service.py、sd_card.py和fpga.py支持新版本；步骤E3_SD_UPDATE.md。等待用户将SD插回断电FPGA并接好上电，然后加载新BIT；尚无E3上板结果。先3项双轮验收，再10对A8+2对A1，需新schema解析及每轮19条日志，无触发不补采，真实错误停止。旧v3完成结果保留。

## 最新：v3分段路径实板完成

migration-4dma-path BIT加载成功，STAT0x501079fc，Linux00083/path_observe=Y。固定V0/A1，3短验收+30正式全部通过，19行/试次完整且守恒通过，数据错误/quarantine/taint0。硬件中位MiB/s283.32/149.31/103.28，原50/50慢化保持；正式V miss全0、A8 A miss/PTW全6160。stage1及写stage3等待增加，但不证明纯PTW/HOL。详见PATH_STAGE_RESULTS.md，实验20260916-164633-run-vvguros8。旧数据/产物保留，无重编或扩矩阵。

## 最新：v3 分段路径版准备

新交付 e4-path-20260916 已下载，profile migration-4dma-path，Linux00083。SD写卡记录 results/sd/update-o8n3zxd9/status.json；跳过产物哈希及写后回读。详细步骤 PATH_SD_UPDATE.md，短验收脚本 scripts/linux_path_short.sh。此版必须同时换 BIT/Linux；尚未上板。按新交付先3次短验收，再固定V0/A1的Solo/A1/A8各10次，每次完整19条观测；旧50/50复测仅作为历史对照。

## 最新：50/50 BIT 回切复测完成

已有 migration-4dma-batch（DS50/50）BIT回切成功，STAT0x501079fc，Linux00082正常。固定V=DMA0/A=DMA1，Solo/A1/A8各10次，30/30通过；硬件中位283.33/149.51/103.31MiB/s，V miss/S1 PTW Solo/A1全0，A8中位0（少数样本可2），A miss/PTW中位6160。与DS0/0的A8 135.38MiB/s相比，回切后恢复约103MiB/s；结果DS50_RETEST_RESULTS.md，实验20260916-135544-run-eawziww2，已回传dell out/mac-batch-ds50-20260916-135544-run-eawziww2。无重新编译/刷卡，退出root/关闭串口，当前保持50/50 BIT。

## 最新：DS0/0实板完成

baseline-4dma-batch加载成功、STAT0x501079fc、Linux00082正常。按用户偏好无短校准/无角色交换，固定V0/A1，六条件Solo/A1/A8/A16/A32/A64各10次，60/60通过；data_errors/quarantine/taint0，观测检查通过。硬件中位MiB/s 283.17/149.59/135.38/131.61/129.98/128.64。A8～A64全部V miss/PTW2，Solo/A1为0，不能把提升直接折算单次PTW代价。报告DS0_BATCH_RESULTS.md，实验20260916-132332-run-ebgxhirr，已同步dell out/mac-batch-ds0-20260916-132332-run-ebgxhirr。已恢复printk/退出root，保持DS0/0配置。旧DS50数据为历史对照，哈希/回读和原签核边界保留。

## 最新：DS0/0 batch准备

新BIT为baseline-4dma-batch，顶层a7d66cb，期望SHA c6555d14…，Linux/payload沿用00082。上次助手误复用旧交付，已重新定位。新脚本fetch_ds0_batch.py，fpga.py已加入baseline-4dma-batch profile。本次SD results/sd/update-v432cy0p在04:17 UTC写入完成、未回读、第二分区未写；当前Mac无外置SD。详见DS0_BATCH_UPDATE.md。尚未加载新BIT，等待确认SD已回到板卡；用户偏好不短校准、不交换角色，固定DMA0受害/DMA1干扰。

## 最新：A16/A32/A64固定角色扩展完成

按用户要求不短校准、不交换角色，DMA0受害/DMA1干扰，4096项/warmup256，各10次，30/30通过。硬件吞吐中位98.27/95.95/94.84MiB/s，A miss/PTW中位7246/7743/7976；第28项A16的V miss/PTW2，其余29项V0，原样保留。data_errors/quarantine/taint0，观测恒等式通过。报告BATCH_EXT_RESULTS.md，结果20260916-114036-run-8pjv_djc，已回传dell out/mac-batch-ext-20260916-114036-run-8pjv_djc。没有重编/刷卡，旧对照未重跑；原HPM/时序边界保留。已恢复printk、退出root。

## 最新：batch正式矩阵完成

4096项/warmup256、Solo/A1/A8、两角色每条件10次，60/60通过（含短校准累计74/74），taint/data_errors/quarantine=0。硬件中位吞吐约283/149/103 MiB/s；双路重叠99.69%～99.79%。第42项DMA1+A8有V miss/PTW2，其余59项V0，保留全部样本，不补采。不能归因为纯翻译或HOL。详见BATCH_MAIN_RESULTS.md。实验20260916-112954-run-9sa69rhc，已回传dell out/mac-batch-main-20260916-112954-run-9sa69rhc；Linux00082、BIT不变。printk恢复、退出root。正式阶段已完成，不自动扩展E2/E3。

## 最新：batch首次上板短校准通过

2026-09-16：migration-4dma-batch加载成功，STAT0x501079fc，Linux00082启动。14/14短校准通过，数据错误/quarantine/taint均0，重复计数、observer窗口与AXI守恒通过；双路在途重叠99.696%～99.802%。64项双路硬件约1.68～2.44ms，但done_to_read约18.15～27.79ms，应分开报告。结果BATCH_SHORT_RESULTS.md，实验results/experiments/20260916-111626-run-n8mg7wsz/。正式4096项/warmup256对照尚未运行，不混用旧E4。下载哈希/SD回读未做。新脚本linux_batch_short.sh、assess_batch_short.py、return_batch_short.py。

## 最新准备：硬件批量版

用户已将SD插回Mac。原卡disk5身份确认，e4-batch-20260916配套BIT/payload已下载至artifacts/software/e4-batch-20260916/delivery/，Linux00082，CVA6 7ed43840cab2。跳过下载哈希/写后回读，旧产物保留。新增fetch_e4_batch.py、update_sd_batch_terminal.applescript，sd_card.py支持此release、fpga.py支持migration-4dma-batch。详细写卡状态见BATCH_SD_UPDATE.md；尚未上板，先批量短校准，不套旧E4判据。

## 最新：E4实板阶段完成

新observe BIT已1MHz配置、STAT0x501079fc，Linux00081启动正常。105/105通过：9短测、36开关对照（最大配对中位变化0.506%）、60主样本。86观测组任务/窗口/AXI守恒检查通过，数据错误0、taint0。H各组约56ms，共跑增加主要在P段，全部主样本both=0；P包含控制MMIO等待，不是纯CPU结论，不证明HOL。

结果E4_STAGE_RESULTS.md，实验results/experiments/20260916-094001-run-kzi_s0zc/，UART results/serial/20260916-094001-console-x8xsc_f0/console.raw.log。已同步dell out/mac-e4-stage-20260916-094001-run-kzi_s0zc/。首轮直接printk缺字，由完整dmesg恢复；后续loglevel4+每次dmesg读回，最终恢复7 4 1 7、observe=N、taint0，已退出root/关闭串口。实际脚本快照在实验目录。当前fpga.py支持--profile migration-4dma-observe，跳过下载哈希记expected_sha256，旧默认不可误用。未修改供给/RTL/Linux或启动E2/E3。

## 最新：E4交付下载/写卡完成，待接板

2026-09-16，新交付dell out/e4-observer-20260916/delivery/已下载至artifacts/software/e4-observer-20260916/delivery/。内核00081-g853e60b234c3，新BIT期望SHA41fa23d0…，必须配套observe BIT，不用旧scfix。用户明确跳过下载哈希和写后回读；仅核对元信息/长度及卡身份，保留旧版。

原7.8GB卡disk5第一32MiB原始启动分区已更新，第二分区未写，备份results/sd/update-61nvnvtt/，状态written_not_readback_verified，安全推出成功。尚未加载新BIT/启动新版Linux。详见E4_SD_UPDATE.md；下载fetch_e4_observer.py，写卡sd_card.py --release e4-observer-20260916，终端入口update_sd_e4_terminal.applescript。

待用户接板后先串口记录、新BIT加载、默认observe=N短Solo，再开启观测校准。当前fpga.py program未接入observe profile，不能误用默认或旧profile。新交付计划为短测后36开关开销+60主样本，失败即停；不要重跑/扩充旧E1。

更新：2026-09-15。请先读本文件，再按需核查本地环境；不用重新全面扫描服务器工程。

## 1. 用户目标与工作方式

- 用户用 **MacBook M1 Pro** 工作，希望在身边只开这一台电脑，完成 FPGA 下载、串口操作和实验。
- 用户已有 Parallels Desktop，另有 Windows 电脑，但不希望同时使用两台本地电脑。
- 源码、Linux 和 FPGA 编译在实验室 **Linux x86_64 服务器**。服务器任务的工作目录是 `/home/xvhaocheng/work`，平台在 `/home/xvhaocheng/work/iommu-platform`。
- 本文件是交给 **Mac 本地任务**的上下文；首先确认实际执行环境是 macOS/Apple Silicon。不要把服务器的 `/home/xvhaocheng/...` 当成本地路径，也不要在服务器上安装 Mac 工具。
- 简洁中文，先说结论。只读取与当前步骤有关的文件；不要重复完整构建或扫描整个仓库。

## 2. 最新建议及证据边界

**优先试：服务器编译 + Mac 原生 openFPGALoader 下载 `.bit` + Mac 串口操作。** 本地无需先安装完整 Vivado 或虚拟机。

- openFPGALoader 的项目支持表明确列出 `genesys2` / `xc7k325tffg900`，内存加载和 Flash 写入均标 OK。
- Homebrew 提供 Apple Silicon 原生包。
- 这是工具支持依据，**还没有在用户的 Mac/板卡上测试**。不能提前声称 USB、下载、串口或 Linux 启动已通过。
- 如需 Vivado Hardware Manager / ILA 界面，再考虑 Parallels + Windows 11 ARM + Vivado Lab Edition。NUS 有 M1 Max/Parallels/Windows 11 运行 Vivado 并通过新版 Digilent Adept Runtime 连接开发板的记录；并非当前 Genesys 2 + Vivado 2023.1 的实测。
- Linux/Rosetta 也有社区方案，但 USB/XVC 有额外适配要求。不要把“AMD 未官方支持 ARM”解释为“ARM 绝不可能运行”。

## 3. 当前平台与启动方式

- 板卡：**Digilent Genesys 2，Kintex-7 XC7K325T**。
- 平台：RV64 CVA6 + 4 路 iDMA + IOMMU + Linux；CPU 50 MHz，timer 25 MHz，DID 10～13，IOTLB 16。
- `migration-4dma` BIT/MCS 已构建，软件构建和 HPM 定向仿真有通过记录；**尚未上板验收**，时钟/CDC/IO 约束审查待办保留。
- 现有 BIT 是 `migration-4dma`，DS 输入/输出延迟 50/50 cycles。`baseline-4dma` 是 0/0，未构建。不可混为同一性能基线。

区分两个步骤：

1. **配置 FPGA 电路**：JTAG 加载 `.bit`。首次只做易失配置加载，断电后需重新加载；不把 Flash 固化当作首次步骤。
2. **让 CVA6 启动 Linux**：目前沿用 SD 启动。Boot ROM 源码读取 SD 的 GPT 第一分区原始数据到 DDR 后启动，不是普通 FAT 文件拖拽流程。

当前软件交付目录不是 SD 整盘镜像。用户已接入原 8 GB 启动卡，其 32 MiB GPT 第一分区与项目 Boot ROM 说明一致，已确认应更新第一分区的 `fw_payload.bin`，见第 9 节及 `SD_CARD.md`。当前流程保留既有布局。

仓库存在 RISC-V Debug Module/OpenOCD/GDB 入口，但完整 Linux 无 SD 启动尚未验证。Vivado 下载 `.bit` 不等于把 Linux 镜像加载并启动。

## 4. 本地任务下一步

1. 本地准备已完成，详情见第 7 节及 `README.md`。macOS 15.0 / arm64，已有 Homebrew 和 openFPGALoader 1.1.1；本地 BIT 已下载校验。可运行 `python3 scripts/fpga.py check` 复核。
2. 确认板卡供电、数据线与板载 USB-JTAG 接口连接。串口 USB 是单独的连接用途，按板卡标识核对，不能把 JTAG 口默认当 Linux 控制台。USB 留给实际使用的宿主机，避免虚拟机抢占。
3. 若尚未安装且用户要求开始配置，可在 Mac 使用 `brew install openfpgaloader`。
4. 按已安装版本的帮助检查 `genesys2` 支持，并先验证 JTAG 设备识别。遇到失败先查供电、接口、线缆、USB 占用及驱动，不立即重建 FPGA。
5. 从服务器取得下节 `.bit`，校验 SHA256 后下载到 FPGA。命令形式：

   ```sh
   openFPGALoader -b genesys2 /Mac上的实际路径/ariane_xilinx.bit
   ```

   不加 `-f`；此步骤不是写 Flash。
6. 识别实际串口设备并以 **115200、8N1、无流控**连接，保存完整启动日志。没有准备好 SD 时，能配置 FPGA 与能启动 Linux 应分别记录。
7. Linux 软件交付已于 2026-09-15 更新，包含最新两项修复；SD 现有布局及启动文件已核对，使用第 9 节脚本备份/更新。启动后再做 smoke、E0 校准。当前唯一实验前端是 `idma_experiment`，不要使用已退役的旧 benchmark。

仍未知：Parallels 版本、连接板卡后的 USB/JTAG/串口枚举。SD 卡和 USB 读卡器已到位，卡身份见第 9 节。已确认 macOS/Homebrew/工具版本及可用 SSH 别名 `dell`（经 `ras` 跳转）。板卡尚未连接。

## 5. 服务器产物与传输核对

现有硬件 BIT（服务器绝对路径，2026-09-15 已确认存在并计算哈希）：

```text
/home/xvhaocheng/work/iommu-platform/out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit
大小：11443722 bytes
SHA256：cd49d4aba0195d5176618fb7d03071154bcd5b347425b43f854945149cceed70
```

Mac 已下载至 `artifacts/migration-4dma/ariane_xilinx.bit`，Python SHA256 和 `LC_ALL=C LANG=C shasum -a 256` 均校验通过。使用已有 SSH 别名 `dell`，不保存私钥或凭据。

服务器的软件交付目录：`/home/xvhaocheng/work/iommu-platform/out/experiment-v1/`。

**用户已于 2026-09-15 更新该软件交付，本地任务已重新核对。** 新交付创建时间 `2026-09-15T14:03:27.711421+00:00`，顶层 `0bb9851bd12fc054bc40d217756d6f0b3662badb`、Linux `031e91dbadc115dafac957067135002b74035a10`，已包含小 chunk 校验修复和多页部分覆盖 smoke；服务器全部 SHA256 校验通过。SD 布局仍未确定，交付仍不是整盘镜像。

如需服务器侧证据，仅定向查看：

- `/home/xvhaocheng/work/HANDOFF.md`：总交接。
- `/home/xvhaocheng/work/iommu-platform/docs/board-host-setup.md`：方案比较与依据。
- `/home/xvhaocheng/work/iommu-platform/docs/validation.md`：构建、仿真与待验收边界。
- `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/bootrom/src/main.c`、同目录 `gpt.c`：SD 启动代码。
- `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/ariane.cfg`：RISC-V 调试入口，非已验证的无 SD Linux 运行器。

## 6. 已查资料

- [openFPGALoader 板卡支持表](https://trabucayre.github.io/openFPGALoader/compatibility/board.html)
- [Homebrew Apple Silicon 包](https://formulae.brew.sh/formula/openfpgaloader)
- [openFPGALoader 下载命令](https://trabucayre.github.io/openFPGALoader/guide/first-steps.html)
- [NUS：Parallels 上运行 Vivado 的实测说明](https://nus-cg3207.github.io/labs/getting_started/vivado_install_guide/)
- [Digilent 论坛：其他板卡的 Apple Silicon 实测](https://forum.digilent.com/topic/32526-report-on-vivado-working-on-arm-macos-to-program-basys-3/)
- [Linux/Rosetta 社区方案](https://github.com/ichi4096/vivado-on-silicon-mac)

完成本地实质工作后，在本地更新交接，记录工具版本、BIT 哈希、USB/JTAG/串口结果和日志位置；明确区分“下载成功”“Linux 启动”“smoke 通过”“HPM 校准通过”。

## 7. 2026-09-15 Mac 首次离线上板准备结果（软件更新前的历史记录）

- 用户当前未连接 FPGA；本轮授权准备脚本、获取 dell 文件和建立结果目录。
- 本机：Darwin arm64、macOS 15.0 (24A335)，Homebrew 位于 `/opt/homebrew/bin/brew`；已有 `openFPGALoader v1.1.1`，本机支持表列出 `genesys2 / digilent_b / xc7k325tffg900`。未安装新依赖。
- 已实际 SSH 连接 dell，并在服务器重新核验 BIT：11,443,722 bytes、SHA256 与第 5 节相同。已下载到本地工作目录。
- 服务器相关说明、当前源码 smoke、Boot ROM 源码和旧软件交付元数据快照：`references/dell/incoming-v14j6phw/`。`download.json` 记录远端绝对路径、大小和每个下载文件的 SHA256，状态为 `verified`。
- 再次确认服务器 `out/experiment-v1/delivery.json` 对应 2026-09-08 构建、顶层 `ffe55a4…`，未纳入最新软件修复。本轮未取回旧软件二进制、未重建服务器软件或 FPGA。仍需服务器更新软件交付及核对 SD 启动布局。
- 操作入口：`scripts/fpga.py`，提供 `check/fetch/ports/detect/program/console/new-run`。`program` 在固定 BIT 校验后才启动工具，仅使用易失下载；`--dry-run` 仅显示命令。串口使用 Python 标准库，115200 8N1 无流控，Ctrl+] 退出，接收字节实时落盘。
- 结果目录：`results/preflight/`、`results/jtag/`、`results/serial/`、`results/experiments/`。每次运行独立建目录。首个实验占位记录 `results/experiments/20260915-225059-run-i8uj8bql/` 已生成，状态为 `not_started`，验收字段均为空。
- 本地检查通过：`results/preflight/20260915-225058-check-h_ihbdx6/check.json`。离线测试 7/7 通过，覆盖坏 BIT 拦截、损坏下载不安装、预览不触发硬件、JTAG 错误码/日志保留、模拟串口字节完整性和无效采集时长；摘要见 `results/preflight/OFFLINE_VERIFICATION.md`。
- 当前 `/dev/cu.*` 只有 `Bluetooth-Incoming-Port`、`debug-console`。未实际调用 JTAG detect/program，未打开物理串口；所有板上验收均待连接设备后完成。操作顺序见本地 `README.md`。

## 8. 2026-09-15 新软件交付同步（当前状态）

- 用户在服务器完成软件更新后，本地已重新核对并下载完整 13 个文件至 `artifacts/software/20260915T140327Z-7bsjoi_r/experiment-v1/`。上层 `download.json` 状态为 `verified`。
- 服务器和 Mac 的 SHA256SUMS 全部 12 项通过。新版 `delivery.json` 固定 SHA256：`1204c6cc66b63495b3bd3436aeca5cd621b6760dffb6c0c8807d875c10cab282`；payload SHA256：`c596f516b33e54f25bd62dec76ae00eccfb02939a4cf3029b044db689b0a3776`。
- 新增 `scripts/fetch_software.py`：独立版本目录下载，固定清单校验、全部文件校验和大小核对；3 项离线验证通过。原 `fpga.py fetch` 仍只下载 BIT/参考资料，已移除过时的软件未更新提示。
- 源码/构建版本及 dirty 状态详见 `artifacts/software/README.md` 和下载的原始清单。没有在本轮重复服务器构建或更改服务器文件。
- 第 7 节及旧 references 快照反映软件更新前状态；当前“交付待更新”已完成。下一步是 SD 布局核对、连接板卡及实际启动/smoke/E0 验证。

## 9. 现有 8 GB SD 启动卡更新（已完成）

- 用户确认是旧启动卡，要求继续并用脚本完成、写好 README。本次识别 `/dev/disk5`，外置 USB，容量 7,817,134,080 bytes；第一分区 33,554,432 bytes、偏移 1,048,576，UUID `46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0`；第二分区 Linux，UUID `0AF525DD-76E6-44E8-A5CD-4A66DC5697B8`。
- 已定向核对项目 Boot ROM 说明、main/gpt 源码、OpenSBI 地址及平台打包过程，采用 `fw_payload.bin` 写入第一原始分区。payload 29,363,656 bytes，小于 32 MiB；保留第二分区和既有分区表。服务器源码与 FPGA 导出目录的 bootrom_64.sv/main.c/gpt.c 哈希一致，详见 `SD_CARD.md`。
- 新脚本 `scripts/sd_card.py` 支持 inspect/backup/update/restore 和 dry-run；固定卡 UUID/容量/偏移及软件版本，检查主备 GPT CRC，先备份旧启动区，再写入、同步、重开设备回读校验。6 项离线测试和实卡元数据预览通过。
- 管理员进程直接访问桌面脚本曾被 macOS 隐私保护拒绝。备选入口 `scripts/run_sd_admin.py` 使用临时输入并归档，但其管理员辅助进程后来仍被拒绝访问原始设备。本机最终实测成功的是用户已授权的系统终端执行 sudo，或用 `scripts/update_sd_terminal.applescript` 打开该终端命令。说明在 `SD_CARD.md`。
- 写卡实际状态以 `results/sd/*/status.json` 为准，成功状态为 `readback_verified`。FPGA Linux 启动与 smoke/HPM 校准依然未验证。
- 本次实际尝试：系统管理员认证后读取 `/dev/rdisk5` 返回 `Operation not permitted`，在读取 GPT、生成启动区备份和任何写入之前停止。失败记录已归档 `results/sd/update-g081d9rb/status.json`，授权/归档记录 `results/sd-admin/launch-e97fg9oc/`。卡内容未修改。需用户在已获磁盘访问权限的系统终端运行 `sudo /opt/homebrew/bin/python3 scripts/sd_card.py update --disk /dev/disk5`；详见 `SD_CARD.md`。此项是 macOS 设备访问限制，不是软件/卡布局校验失败。
- 用户随后表示已授予访问权限，已重试同一脚本；卡身份和软件预览再次通过，但管理员读取 `/dev/rdisk5` 仍被拒绝（EPERM）。新记录 `results/sd/update-95327_69/status.json`，运行记录 `results/sd-admin/launch-we0yal9h/`。普通用户单扇区只读检查返回 EACCES（UID 501，无 operator 组）。均未读到卡内容，也未写卡；需确认授予权限的具体应用及权限是否已对进程生效。

### 最新实际结果：2026-09-15 23:28 JST

- 用户已给 Codex 和系统终端授权。后台管理员方式再次 EPERM（`results/sd/update-v3n1drkg/`，未写入）；随后通过系统终端执行相同 `sd_card.py update --disk /dev/disk5` 成功。
- 完整备份与成功记录：`results/sd/update-6curphb9/`。状态 `readback_verified`；起止 UTC `14:28:50`～`14:28:54`。
- 旧第一分区 32 MiB 已备份，SHA256 `7cefdfde8eb6707aa20f5d0900eef8c8729a34ef760fb4c3c434af4989b5e97b`；归档后再次核验通过。
- 新 payload 回读 SHA256 `c596f516b33e54f25bd62dec76ae00eccfb02939a4cf3029b044db689b0a3776`；补零后的完整 32 MiB 回读 SHA256 `90dcd0bd70cd55a6679d01d5154cdd6c758a34822ad0ca5930a21f1eb0b408ba`。
- 主备 GPT CRC 通过；写前后的磁盘头/尾各 1 MiB 完全一致。仅第一分区设备被打开写入，第二分区保留（未对第二分区全量哈希）。
- 更新后重新核对卡身份，`diskutil eject /dev/disk5` 已成功；可以取卡。下一步插入断电的 Genesys 2，连接 JTAG/串口，先开始串口采集再加载 BIT。Linux 启动、smoke、E0/HPM 仍待实测。

## 10. 首次 JTAG/串口上板（配置通过，Linux 登录失败，详见 FIRST_BOARD.md）

- 用户已插入 SD 并连接板卡。USB-JTAG 序列号 `200300BD5148`，JTAG IDCODE `0x03651093` / `xc7k325t`；独立 UART 为 `/dev/cu.usbserial-AV0KRCHT`。`200300BD51480/1` 是 JTAG 芯片的串口候选，不用作 Linux 控制台。
- 默认 6 MHz 初次易失下载返回 0，但 DONE/EOS/GWE=0，未配置就绪。原始日志 `results/jtag/20260915-233348-program-oww1gsel/`；旧脚本该次状态只表示工具命令成功。
- 只改变 JTAG 为 1 MHz 重试后，IR DONE=1，独立 STAT `0x501079fc`，DONE/EOS/GWE=1、无 CRC/ID 错误。成功下载日志 `results/jtag/20260915-233744-program-lthwbt0o/`；状态核验 `results/jtag/20260915-233931-status-kvcoq9y_/`。BIT 未变、未写 Flash、未重建 FPGA，6 MHz 失败机制尚未确定。
- `scripts/fpga.py` 已默认 1 MHz，新增 `status` 和未就绪返回 2 的逻辑；相关离线测试 8/8 通过。新示例见 README。
- 原始串口日志 `results/serial/20260915-233330-console-gv7d1io6/console.raw.log`。SD 整个 32 MiB 第一分区加载完成，OpenSBI 成功进入 Linux `6.5.0-rc2-00078-g031e91dbadc1`，4 个 iDMA 设备注册。日志 27,543 bytes，SHA256 `6958a69754dc52f0e00b2a4c0f928a0ce9378cfcdf9cea19114bfaf25a1c1226`；会话已正常关闭。
- Linux 先出现 proc/dentry 卸载警告，随后 cat、dhcpcd、login 共 3 次 NULL pointer dereference，均在 `generic_permission+0x1c/0x1b2`，`badaddr=0x60d`。到达 Buildroot 登录提示后输入一次 root，login 崩溃，未获得可用终端。详细顺序见 FIRST_BOARD.md；不能记为 Linux 可用性通过。
- 已有限只读核对 rootfs init/inittab、相关内核源码及 dell Linux 工作树（干净），根因未定；未更改服务器或重新构建。下一步定位 Linux 异常，而非继续 DMA 测试。smoke/HPM 均未运行；首个 run.json 已更新为 `linux_login_failed`。
- 用户报告 LD15 亮。已解释 LD15=电源正常、LD14/DONE=配置完成，不能混为 Linux 启动通过。

## 11. 2026-09-16 远程诊断（缺陷已复现，修复未交付）

- 用户授权将问题同步 dell 并远程修复；原始 FIRST_BOARD、串口日志/session、run.json 已上传 `iommu-platform/out/board-bringup-20260915-mac/`，串口 SHA256 两端一致。
- 同 Image 的 QEMU virt 启动没有复现 proc/Oops；普通登录到密码提示，未完成认证；独立 init-shell 中 100 轮 proc 读取/进程退出通过，taint=0。这不是 FPGA 功能验证。
- 安装 OpenOCD 0.12.0，FT2232 channel 0 的 CPU 调试可用。实板小测试：高半部 SC.W 在没有新 LR 时，写入被拒绝却返回成功，1000/1000 复现；低半部 SC.W 和 SC.D 正常。BIT 使用 WT_DCACHE，其 wt_axi_adapter.sv 把 amo_off_d 默认清零，导致等待 B 响应时偏移丢失；本地候选改为保持 amo_off_q。
- 详细证据、脚本及边界见 `LINUX_BOOT_FIX.md`。首轮调试脚本的 get_reg 缓存读取问题已记录并改为 -force，不能声称旧 Linux 上下文成功恢复。CPU 当前暂停；SD/Flash 未修改，后续需重新启动/配置。
- 新 RTL 定向测试首轮缺少 ariane_dm_pkg 依赖，尚未编译通过。后续远程请求的自动权限审核连续两次超时，服务器源码尚未修改、未提交、未重建 BIT；需要恢复远程权限再完成测试/修复/新 BIT 上板。保留服务器既有未提交文档，不得覆盖。未运行 DMA smoke/HPM。

## 12. 用户确认继续后的完成结果：RTL 修复与新 BIT

- dell CVA6 提交 `7f086b73f19d8e4d48ed74a671e8315f827d2ce0`，只改 wt_axi_adapter 的 amo_off_d 默认保持。旧真实生产模块测试 54/6 失败，修复版及提交后的重跑均 54/0。21 个组件/gitlink/lock 检查通过。
- 顶层构建提交 `c539ef10654929e5b972392d833e59966429449c`；独立 `migration-4dma-scfix` 保持原 50 MHz/DS50/50/四 DMA 参数。2026-09-15 UTC 15:28:24～15:56:47 构建退出 0，BIT/MCS 均生成。旧目录与旧 BIT 哈希未变。
- Mac 新 BIT：`artifacts/migration-4dma-scfix/ariane_xilinx.bit`，11,443,722 bytes，SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`。下载脚本验证固定源码版本/lock/profile、服务器清单和本地内容，独立 shasum 再通过。
- 正式 routed WNS 0.177 ns、WHS 0.010 ns、WPWS 0.062 ns；所有已设置的时序约束满足，但 6 项 Critical Warning 与 IO/CDC 审查保留。结果/原始报告：`results/build-scfix-20260916/`；详见 LINUX_BOOT_FIX.md。
- Mac 使用 `python3 scripts/fpga.py program --profile migration-4dma-scfix`，新实验记录也须指定同一 profile。默认旧 program 仍是历史 BIT，不要误用。23 项本地离线测试通过；本轮仅 dry-run，尚未实际下载新 BIT 到 FPGA。
- 服务器三份既有文件在校验 SHA256、备份后更新，原未提交研究文档保留。最终结果已在 dell 本地从既有构建状态生成并追加到 HANDOFF/SC_WORD_FIX，备份保留；没有上传被拒绝的整份最终研究交接文档。附加原始原子测试文件未补传，仍在 Mac。
- 当前 FPGA CPU 仍处于之前的调试暂停状态；SD 不变。下一步以新 BIT 重启、串口采集、验证 Linux 登录/原子指令，再做 smoke/HPM。新 BIT 尚未上板，不能宣称登录故障已完成硬件验收。

## 13. 最新实板复测：新 BIT 的 Linux 登录/proc 检查通过

- 已加载 `migration-4dma-scfix`，STAT `0x501079fc`，同一 SD 启动 Linux `6.5.0-rc2-00078-g031e91dbadc1`。
- root 正常登录并确认 uid=0；`scripts/linux_proc_check.sh` 的 100 轮 proc/子进程退出检查通过、退出码 0，taint 前后均为 0，未见原 proc/Oops 异常。
- 本次记录：`results/experiments/20260916-010522-run-p_yayuc_/run.json`；串口 `results/serial/20260916-010547-console-tmk58t9_/console.raw.log`，27,315 bytes，SHA256 `250890f2d73fa62d8a55879fd4f51c621103ead315616a46ffe586cab42673f4`。
- 详细操作和边界见 SCFIX_BOOT.md；原历史失败日志未覆盖。Linux 保持运行，已发送退出登录并正常关闭串口，未 halt、未写 SD/Flash。
- 独立原子探针、DMA smoke/HPM、网络及完整 CDC/IO 签核未完成。本次是有限启动/登录/proc 验收，不代表全功能或长时间稳定性通过。

## 14. 最新 DMA smoke：预热用例数据校验失败

原交付脚本因缺 BusyBox timeout 退出 127（尚未提交 DMA）。临时兼容脚本保留超时保护和原参数后，DMA0/DID10 的 warmup=0 用例通过；warmup=32 用例 8/8 完成但 data_errors=1、status=-84，脚本停止，剩余 12 组未运行。详细证据与可复用脚本见 DMA_SMOKE.md。

Linux taint=0，无新内核异常；退出登录、关闭串口，板卡保持运行。未重载 BIT/改 SD、未改 dell 源码。本轮串口和结果仅保存在 Mac。当前任务重点转为定位首次数据不匹配，而不是宣称 HPM/并行实验通过。

## 15. 诊断版 SD 已更新，等待重新上板

用户已提供 dell 的独立诊断交付 `out/dma-smoke-debug-20260916/delivery/`，Linux `6.5.0-rc2-00079-g53d3113dee74`。Mac 15 文件下载/14 项哈希校验通过；新增固定版本下载脚本和 SD `--release dma-smoke-debug-20260916`，离线测试 27/27 通过。

SD 更新通过：`results/sd/update-r7q8_l66/`，status=readback_verified；payload SHA256 `58ba4d4ae9601ba8fad6064e217dabef51f897ac5461ca07fcc77eb9232836bb`。旧启动区已备份、主备 GPT/头尾不变、第二分区未写，已安全推出。等待用户插回断电 Genesys 2 并供电连接；断电后须重新加载同一 SC.W 修复 BIT。

具体流程/哈希见 DMA_DIAGNOSTIC.md；板上只运行 `scripts/linux_dma_diagnostic_once.sh` 的一次 DMA0 Solo，保存 JSON 与 `experiment data mismatch:` 整行。诊断尚未执行；缓存假设仍非最终根因，即使刷新后值恢复也保持失败判定，不运行完整 smoke/HPM/双路矩阵。

## 16. 诊断内核已上板，追加 smoke 再次复现数据错误

用户接好后重新加载同一 SC.W BIT，STAT 0x501079fc，新内核 `6.5.0-rc2-00079-g53d3113dee74` 正常登录。一次指定 warmup=32 Solo 成功（8/8、data_errors=0），无首错行；独立记录 `results/experiments/20260916-015253-run-jlcizoxg/`。

随后用户明确选择完整 14 组 smoke。正式新脚本在第一组 DMA0 warmup=0 即失败：8/8 完成、data_errors=1、status=-84、脚本退出 1，其余 13 组自动停止；记录 `results/experiments/20260916-020422-run-_hjma4td/`。完整首错：offset=912，src=14、after=14、expected=14；dst=eb、after_fence_i=14、expected=14。错误后刷新未掩盖 EILSEQ，支持旧缓存视图假设但未最终定位。

完整字段及日志哈希见 DMA_DIAGNOSTIC.md。每轮 JSON/首错行/摘要分别回传到 dell `out/dma-smoke-debug-20260916/mac-20260916-015253-run-jlcizoxg/` 与 `mac-20260916-020422-run-_hjma4td/`，远端回读哈希通过；没有覆盖旧结果。第二轮控制台实时首错打印损坏，dmesg 与后续只读 grep 获得完整一致整行，没有重跑 DMA 来补日志。

两轮 taint=0、无旧 proc/Oops 崩溃；已退出 root 并正常关闭串口，Linux 保持登录提示，无残留实验进程。未继续其他 DMA、HPM 或性能实验，未实施进一步软硬件修复。

## 17. 最新：缓存同步修正版两次启动功能 smoke 全通过

SD 已更新至缓存同步版 `6.5.0-rc2-00080-g3b47ab1645e4`，Linux `3b47ab1645e412095b601e3be68b6347b9fad213`。用户要求跳过下载哈希与写后回读，状态 `written_not_readback_verified`，备份 `results/sd/update-hjkqrpsa/`；期望 payload SHA `6f52ad85c4d2fc87e97301d98f1394216b843cd8be601b2965eca429caf74053` 不是 Mac 实测值。沿用同一 SC.W 修复 BIT，两次 STAT 均为 0x501079fc。

启动 A 的 warmup32 短试次 + 完整14组全部通过（15/15）；重新加载同一 BIT、全新启动 B 后直接完整14组，也全部通过（14/14）。共29组，各项数据校验、完成数、返回状态、内核日志通过，taint均0，无首错或旧 proc/Oops。未运行性能/HPM校准，不将有限功能验收写成长时间稳定性或全Linux DMA一致性签核。

详细数据/路径/复现脚本见 DMA_CACHE_SYNC_RETEST.md。两次 Mac 实验分别 `results/experiments/20260916-023501-run-94s4dxf3/`、`results/experiments/20260916-024506-run-_8u4jct1/`；所有 JSON/判定/摘要已回传 dell `out/dma-cache-sync-20260916/mac-20260916-023501-run-94s4dxf3/` 与 `mac-20260916-024506-run-_8u4jct1/`。完整串口留在 Mac。

板卡保持新内核运行，已退出 root、关闭串口，无遗留测试进程。下一阶段是另行规划 per-DID HPM 校准；旧版性能样本不得与本版混用。

## 18. HPM第一步：单路DID归属检查通过

继续同一缓存同步版内核，通过sysfs空闲第7槽独立监测，避开驱动前6槽。事件4(IOTLB miss)、7(S1 PTW)各4×4监测DID/活跃DID矩阵，共32个DMA短用例；另8个1秒空闲对照。所有对角线128，与会话读数一致；非对角线及空闲对照0。全部数据校验、完成数、状态通过，taint=0。

恢复读回确认 inhibit=0xffffffc1、event7=0、counter7=0；退出root，关闭串口，无残留实验进程。未刷SD/重配BIT。完整记录/边界见HPM_SINGLE_DID.md，Mac实验 `results/experiments/20260916-025923-run-9y2shoth/`，串口 `results/serial/20260916-025929-console-2i5ot7ew/console.raw.log`。

32条JSON/矩阵判定/摘要已回传 dell `out/mac-hpm-single-did-20260916-025923-run-9y2shoth/`。单路DID过滤通过不是完整HPM校准：双路窗口、绝对事件语义、位宽/溢出、S2正向、冷/热及开销仍待做；未运行性能矩阵。

## 19. 双路 DID 短测通过

事件4/7、DMA0/1角色交换、监测DID10/11和未参与DID12，12/12及6个空闲对照通过。victim完整调用/窗口=128/128，aggressor=128/126，未参与DID及空闲=0；数据错误0、taint0、状态已恢复。不是精确边界或硬件重叠证明，完整HPM校准和性能矩阵仍未完成。

详见HPM_DUAL_DID.md；脚本linux_hpm_dual_did.sh、assess_hpm_dual_did.py、return_hpm_dual_did.py。Mac实验results/experiments/20260916-030906-run-fcxw42m1/，完整UART results/serial/20260916-030912-console-pne_83ah/console.raw.log。结果已回传dell out/mac-hpm-dual-did-20260916-030906-run-fcxw42m1/。没有刷卡或重启，板卡回到登录提示，串口关闭。

## 20. 用户授权跳过剩余校准，探索性性能完成

18/18样本通过：DMA0/1互换，Solo/A8页/A32页各3次，V1页、4096字节、4096次、预热256。Solo约27.3 MiB/s，双路V约15.2–15.3 MiB/s，软件观测耗时约1.79倍；不是纯硬件慢化或HOL证据。data_errors/quarantine=0、taint=0、退出0。现有BIT/Linux，未刷卡或重启。

详情PERFORMANCE_EXPLORE.md；脚本linux_perf_explore.sh、assess_perf_explore.py、return_perf_explore.py。实验results/experiments/20260916-031757-run-4he_xf4c/，UART results/serial/20260916-031757-console-sf21dhww/console.raw.log。结果已回传dell out/mac-perf-explore-20260916-031757-run-4he_xf4c/。板卡留在登录提示、串口关闭。完整HPM校准仍未完成，后续样本继续明确标记探索性。

## 21. 当前双路E1完成（用户确认不含E2/E3）

144/144通过：16项事件/有限冷热/进位回绕检查，48敏感性，80主样本。当前软件计时口径下A1页约1.73倍，A8/32页约1.79倍；data_errors/quarantine=0、taint0。预热/窗口加倍归一化变化最大0.427%，通过预定5%门槛。未重建/刷卡/重启，串口已退出。

见E1_STAGE_PLAN.md、HPM_REMAINING.md、E1_STAGE_RESULTS.md。实验results/experiments/20260916-032650-run-rbo67pvd/；UART results/serial/20260916-032650-console-jz4u_u_b/console.raw.log。144条记录/判定/协议已回传dell out/mac-e1-stage-20260916-032650-run-rbo67pvd/。

不能称完整E0：S2正向、溢出IRQ、空窗口/禁HPM开销、硬件请求重叠未验证，已知相邻不同事件漏计/时钟约束待办保留。当前PTW计数来自miss状态，不是独立访存量。E2/E3未做，不自动扩展RTL/Linux。
