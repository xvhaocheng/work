# 失效规范与原版 RTL 来源核查

术语：CQ 是内存中的命令队列，每项 16B；PSCID 是一阶段地址空间编号，GSCID 是二阶段虚拟机编号；AV/PSCV/GV 分别表示地址/PSCID/GSCID 参数有效；global 指页表的 G 位。

**原版 IOTLB 实现了按 PSCID 和按地址失效，软件宽失效并非硬件只能整体清除。真正的 CQ 取命令路径存在，但原样 c80 的 DS 写路由与 PTW 叶处理有已知功能缺陷。后续使用明确带修复的 F5 完整 legacy top，CQ 和 IOTLB 保持原 blob；不把直接 flush 线当作 CQ。** 本文件仅核原文和源码，没有构建或运行。

## 固定来源

- 正式规范：[RISC-V IOMMU Architecture Specification，v1.0.1，2026-02-22，Ratified](https://docs.riscv.org/reference/iommu/v20260222/_attachments/riscv-iommu.pdf)。已下载 [PDF](spec/riscv-iommu-v20260222.pdf)，SHA256 `be2134b407a1a3ce5a9da1779cbdc4b4c641ee50ab32676e0a858e5a04ff2b8e`。
- 发布 tag `20260222` 固定源码 commit `054005f4ffe30838a4ffe144a01ea3933ffea6f2`；API 原输出见 [spec-tag-20260222.json](spec-tag-20260222.json)，获取时间和全部文件 hash 见 [spec-sources.json](spec-sources.json)。规范为 RISC-V International 的 CC-BY-4.0 一手规范；[许可证](spec/LICENSE) 与原文保留。
- 规范 §4.1.1、**Table 11（印刷页49、PDF第53页）**是 VMA 范围表；§4.1.2（页50–51）定义 IOFENCE；§6.6–6.8、6.15 定义 CQB/CQH/CQT/CQCSR。HTML 页内编号可能显示 Table 2，本文采用完整 PDF 的 Table 11。原文入口：[队列源码](spec/iommu_in_memory_queues.adoc)，L183–204、L214–250、L344–428；[寄存器源码](spec/iommu_registers.adoc)，L446–548、L758–884。Table 11 的 [页面截图](spec/invalidation-table-page49.png) 已目视核对。
- 版本以发布 PDF 封面为准。该 tag 的 `src/riscv-iommu.adoc` 仍有默认 `revnumber=1.0`/`revdate=06/2023`，不能拿这个构建默认值覆盖已取得的发布身份。
- 原样实现：`zero-day-labs/riscv-iommu` 的 `mc/dti-ats` 固定 `c80bba7ae74b54b50eb1028e6efd4292661ca75b`，快照在 [rtl/c80bba7/](rtl/c80bba7/)。CVA6 原上游 `1f992d8` 的 child 为 `f8f918aa251209a8b7fafde89ba46a01cf0f7c68`，定向比较快照在 [rtl/f8f918a/](rtl/f8f918a/)。两版 IOTLB 文件逐字节相同，CQ 主要差别是后者尚无新增 ATS 通路；本次所述 CQH/PR/PW/marker 宽度问题两版都有。
- 修复基座 F5：`85b157ebe71a623e326e51fff7b7126fe77e8eb9`。该版本的 **CQ handler、IOTLB blob 与 c80 完全相同**，DS owner/W 路由和 PTW 叶修复另列，见 [rtl/fixed-baseline/](rtl/fixed-baseline/)。完整原 blob、SHA256、路径和源码 URL 都在 [rtl-sources.json](rtl-sources.json)。生产仓库 HEAD/refs/status 前后未变。

## IOTINVAL.VMA 的八种范围

下表是 Table 11 的中文转述，完整英文原表已保存在 `source-excerpts.json` 的 `spec-vma-table`；原版 IOTLB 对应 [rv_iommu_iotlb.sv](rtl/c80bba7/rtl/translation_logic/rv_iommu_iotlb.sv) L204–264。

| GV | AV | PSCV | 必须失效的范围 | global | c80 实际代码行 |
| --- | --- | --- | --- | --- | --- |
| 0 | 0 | 0 | 所有 host 地址空间 | 包含 | 206–211 |
| 0 | 0 | 1 | 指定 PSCID 的 host 地址空间 | 排除 | 212–217 |
| 0 | 1 | 0 | 所有 host 地址空间中，覆盖 ADDR 所指 IOVA 的一阶段叶项 | 包含 | 218–224 |
| 0 | 1 | 1 | 指定 host PSCID 中，覆盖该 IOVA 的一阶段叶项 | 排除 | 225–232 |
| 1 | 0 | 0 | 指定 GSCID 下的所有 VM 地址空间 | 包含 | 233–238 |
| 1 | 0 | 1 | 指定 GSCID、PSCID 的 VM 地址空间 | 排除 | 239–246 |
| 1 | 1 | 0 | 指定 GSCID 下所有 VM 地址空间中，覆盖该 IOVA 的一阶段叶项 | 包含 | 247–254 |
| 1 | 1 | 1 | 指定 GSCID、PSCID 中，覆盖该 IOVA 的一阶段叶项 | 排除 | 255–263 |

这是逐分支源码核对，未声称已运行八组合符合性测试。RTL 将 host 定义为 `!en_2S && en_1S`，VM 一阶段项定义为 `en_2S && en_1S`；PSCV=1 的四个分支都实际检查 `!content_1S.g`。地址匹配兼顾 4KiB/2MiB/1GiB 叶（L163–170）。

直接影响实验解释的三点：

- `GV=0` 表示 S2=Bare 的 host 通路，GSCID 被忽略。不能给两个普通 host 设备写不同 GSCID 就假设失效隔离。
- 按地址不自动等于按设备。`AV=1,PSCV=0` 会清掉所有 host PSCID 中的相同 IOVA；不同 domain 可以使用相同 IOVA。若 M3 表示“域内按地址”，应明确 `AV=1,PSCV=1`。
- PSCV=1 不清 global 项。测试映射应明确 G 位，不能把 global 保留误计成按 PSCID 失效失败。IOTLB 自身也允许 global 项跨 PSCID 命中（L94–98）。

IOTINVAL.GVMA 不同于 VMA：PSCV=1 非法；GV=0 时 AV 无效并清所有二阶段空间；GV=1 才可按 GSCID/地址收窄，见同节 Table 12。新规范的 NL/S 扩展位在 c80 的命令结构中仍属 reserved，本实验应保持零，不能假设支持范围/非叶扩展。

## 命令确实从 CQ 到 IOTLB

源码入口均位于 `rtl/c80bba7/`：

| 环节 | 实际路径和原文位置 |
| --- | --- |
| 软件寄存器 | `packages/rv_iommu_reg_pkg.sv` L564–576：CQB=0x18/0x1c，CQH=0x20，CQT=0x24，CQCSR=0x48 |
| 接到 CQ handler | `rtl/software_interface/rv_iommu_sw_if_wrap.sv` L171–220：CQB PPN/size、CQT/CQH、CQCSR、iotinval 逐项接线 |
| 真正内存取命令 | `rv_iommu_cq_handler.sv` L240–293：head≠tail 发 AR，地址 `CQ_base + head*16`，两拍 64-bit R 组成 128-bit 命令 |
| 解码失效 | 同文件 L310–338：传 AV/GV/PSCV/VPN/GSCID/PSCID；VMA 产生 `inval_vma`，非法保留位/func3/GVMA+PSCV 设置 cmd_ill |
| 内部连接 | `rtl/rv_iommu_top.sv` L216、431/502、567 → `rv_iommu_tl_wrap.sv` L547–552 → IOTLB 的 `inval_i` |
| 失效与填充优先级 | `rv_iommu_iotlb.sv` L204–319：失效优先于同拍 update，下一拍标签有效位更新；没有在这里增加 P/PN 或 mask |

命令布局来自 `packages/rv_iommu_pkg.sv` L328–381，与 §4.1 的基础编码一致：opcode[6:0]、func3[9:7]、AV[10]、PSCID[31:12]、PSCV[32]、GV[33]、GSCID[59:44]；地址 ADDR[63:12] 位于命令 bits125:74。因此页对齐 IOVA 的第二个 64-bit 字为 `iova >> 2`；不是直接把字节地址写入高字。

CQB 的 PPN 位于寄存器 bits53:10，故 4KiB 对齐的物理基址编码为 `(base >> 2) | (log2(entries)-1)`。环容量是 `2^(LOG2SZ-1+1)`，保留一个空槽区分满空。现有上游 VIP 已提供真 CQ 初始化和 `mem_write` 两个 64-bit 字、再写 CQT 的任务（`sim/.../rv_iommu_vip_pkg.sv` L552–600）；该任务本身**没有等待 fence 完成**，不能拿它当已完成的新回归。

## IOFENCE：已实现部分与实际偏差

规范 §4.1.2 要求 IOFENCE 之前的 CQ 命令完成；PR/PW 请求已处理设备读/写到达全局排序点；AV=1 时完成通知是 **4B store**。原版源码实际为：

1. CQH 在命令两拍读完的 REGISTER 阶段就加一，L279–283，早于 DECODE；所以“CQH 越过带 AV 的 fence”不能证明该 fence 自身的 marker/B 已结束。CQCSR 错误处理也应检查，不能只看 head。
2. CQ handler L362 明确 PR/PW 为 TODO，代码没有读取 `cmd_iofence.pr/pw`。本任务限定 `PR=PW=0`，COPY 已真实 DONE 后再 unmap；不把它称为原版支持设备读写 drain fence。
3. AV=1 真正进入 WRITE，L366–369；WRITE 按 AW→W→B，且只在 B 返回 OK 后回到 IDLE，L466–491。因而 **VMA → AV marker fence → 普通 fence 哨兵**，等 marker 值匹配、哨兵已被取到，并检查无 CQ 错误，可在这个串行 FSM/ATS-off 实现中确认前一个 marker 的 B 已结束。这是源码推导，后续仍需实际跑通。
4. Marker AW 固定 size=3（8B）、WSTRB 全1、WDATA={0,DATA}（L178–189），不符合规范精确 4B store 的宽度。使用专用、8B 对齐且独占的槽，避免破坏邻接数据；报告保留这个实现偏差。
5. WSI 在 DECODE 就设置 fence_w_ip（L372–375），也不能替代 marker/B 结束。ATS 关闭时 `atsinval_inflight=0` 由 top L663–678 绑定，不引入 ATS 等待。

限定也很重要：由于该 CQ 串行执行，下一条普通 fence 被取到时，其之前的 IOTINVAL 脉冲已执行。这可以证明局部失效脉冲顺序，不能扩大成 PR/PW、ATS、AV 写回和一般 IOFENCE 的完整符合性声明。Marker、哨兵的额外 CQ fetch/DS store 成本必须另列，不称 Linux 原 AV=0 native fence 的总费用。

后续主测已选择 `IOTINVAL + IOFENCE(AV=0) + IOFENCE(AV=0)`：第二个 fence 的 CQH 前进证明第一个 fence 已完成 DECODE，并且失效已执行。第二 fence 自身回到 IDLE 的周期可另记。AV marker+哨兵仅用于先验证真实 CQ AW/W/B 的烟测，不放进每次主测失效的费用，以免诊断写回改变 B 的竞争。

## 为什么使用明确修复的 legacy top

- 原 c80 DS W mux 的 owner0 注释为 CQ（`rv_iommu_ds_if.sv` L154–160），但输入拼接最低项为 PQ（L186–188）。在 PQ 没有 W 的情况下，CQ marker AW 被接受后不能得到自己的 W。这是实际 CQ marker 路径的功能障碍，不是失效粒度机制。
- F5 的既有 F3 修复将最低项改为 CQ，并保持 accepted AW owner 到 WLAST，固定快照 L158–203 可核；F5 同时有 F4 叶修复。原 c80 PTW L424–430 的第二个 if/else 会误拒 LVL2/LVL3，不能拿原样 c80 称为已成功完成普通 4KiB COPY。
- F5 还继承独立的多 DMA sideband、AW-before-AR 进度、HPM 边沿、AXI 边界修复。这些须在所有模式共同固定；特别是 AW 优先改变服务先后。它们不等于新增分区、命中旁路、banked 控制器或 DS-first/F4 研究服务规则。
- 已有 PLRU runner 的局部切面没有 CQ/SW/top，不能只在旧切面直接拉失效线。后续使用 F5 完整 legacy top、真实内存 CQ/MMIO 接入，是将现有硬件通路接进独立测试副本；本次没有修改或验证该副本。

## 原文和核查边界

[source-excerpts.json](source-excerpts.json) 保存上述关键段落的原文、精确行号和源文件 hash；原始规范、完整相关源码快照与 [rtl-c80-f8-cq.diff](rtl-c80-f8-cq.diff) 均保留。规范是规范性一手来源，RTL 是实现一手来源；论文统计/期刊证据分级不适用。

本轮没有用代码注释替代实际逻辑，没有把源码推导写成仿真通过，也没有直接驱动任何 flush 信号。未做生产修改、RTL 编译、实验、Linux/FPGA/板测。CQ 并非完全缺失，F5 的真实路径有源码依据；是否实际可运行和每种范围的命中/完成数字，交由后续测试取得。
