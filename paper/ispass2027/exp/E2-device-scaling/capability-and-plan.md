# E2：设备数能力与最小软件扩展

## Material Passport

- 2026-10-17；Codex-only；原始源码审读与运行准备，无E2板测。
- 固定实际基座：CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`、Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`，不使用活动研究树替代。
- 当前接口仅V+A一对。**既有DS0 BIT具备四路硬件，E2可采用有界软件扩展**；多A驱动部分待Root在A2 P0稳定后整合。HPM独立小批已由Root授权，状态和结果另见本目录交付。
- 使用既有 [baseline100源码清单](../../board/packages/baseline100-20261017/source.json)和实际DS0 load receipt；不新增硬件变更或身份体系。

源码前缀：`HW=/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/`；`SW=/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-software01/`。关键原文与hash在 [source-evidence.json](source-evidence.json)。

## 能力边界

| 项目 | 证据与判断 |
| --- | --- |
| 四路物理DMA与身份 | `HW/src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:868`固定4路；`1017–1034`四路各有batch port、DID10+i，private AxID=0；`990–1014`mux把端口编码到共享侧ID。`SW/inputs/configs/platform.dts:141–174`四个独立MMIO窗口与DID10..13。 |
| PATH范围 | `HW/.../dma_path_observer.sv:123–142`仅V与selected A各读写4段。其他端口的高ID不导致错误（`173–176`只拒private低位非0），但没有它们的PATH记录。 |
| 硬件HPM容量 | `HW/src/cva6/corev_apu/tb/ariane_soc_pkg.sv:110–126`的**实际配置**为31 counters；`ariane_peripherals_xilinx.sv:1137–1138`传入该配置。不是仅凭wrapper默认值。4DID×3event=12，够用，无RTL扩容。 |
| 当前Linux限制 | `SW/src/linux/drivers/misc/idma.c:847–850`仅两个streams，UAPI `include/uapi/linux/idma.h:48–74`仅一个aggressor_id及两组stats；全局mutex在`idma.c:85–86,868,1030`覆盖整个实验。多进程/多fd无法绕过，不能宣称3/4路并行。 |
| 当前HPM软件限制 | `SW/src/linux/drivers/iommu/riscv/iommu-hpm.c:11–19,68–95,133–159,188–194`独占6槽、两DID、每DID的miss/S1/S2三event；是软件会话限制。 |
| shared等式 | **内核**`idma_observation_finish:758–802`未要求shared=V+selectedA，保留的检查是V/selectedA在途及PATH守恒。该两设备等式在主机`board/collect.py` parser里，multi采集器必须另写。 |

## 矩阵（仅DS0，每点10次，尚未执行）

V固定DMA0，源/目的各1页；4096B COPY×4096，预热256；A为DMA1、DMA1+2、DMA1+2+3，selected A固定DMA1。主矩阵每个A源/目的各1页或8页，6个条件。相同总足迹对照选择**每方向总6页**：单A6、两A各3、三A各2，另3条件。合计9条件×10=90点；不因准备包自动加Solo，已有DS0 Solo保持独立来源。这里页数分别指源和目的；总6页/方向表示所有A共有12个4KiB映射叶项，不是合计6个。

设备数和映射足迹两条轴分开；同总映射足迹仍不保证相同到达率、活跃并发或实际总压力，需同时报告各A进度。十个区组轮转九条件，同DS0集中测完。暂未用这份计划运行硬件。

## 新ABI的具体候选（Root裁定后实施）

保持旧`struct idma_experiment`及旧ioctl不变。Root已与A2对齐命令布局：P0的CAPS/RUN为0x09/0x0a；E2预留`CAPS_MULTI=0x0b`、`RUN_MULTI_BATCH=0x0c`，独立multi ABI version3（本包暂不写UAPI）。CLI独立`idma_scaling`，不能将多A塞进旧JSON。

- 输入：`abi_version/struct_size/flags=0/nr_devices(2..4)`；`device_id[4]`、`pages[4]`、`path_aggressor_index`；`chunk_bytes=4096/run_timeout_ms/victim_transfers/warmup_transfers`；所有reserved及未使用数组槽为0。`device_id[0]`须等于打开的victim设备，本轮固定0；A为其后元素，设备和DID都唯一；path索引本轮固定1。
- 输出：status、nr_devices、did[4]、measurement_started/complete、非原子snapshot前后范围、软件V时间/排空时间；`idma_experiment_stats stats[4]`逐路保留submitted/completed/bytes/active/miss/S1/S2/errors/quarantined；每A启动/STOP发出/排空完成的**软件**时间及停止前batch控制状态。硬件observer仍原有3条batch+16条PATH并记录同sequence，增加JSON角色映射及shared残差标签。
- 任何缺失设备、重复DID、跨IOMMU、参数/保留槽错误、已有quarantine、MMIO能力不符或HPM忙均在启动前拒绝；copy失败/中途失败保留原始错误，所有已经启动设备按既有STOP/排空/quarantine规则收尾。

## HPM第一批接口与12槽布局

旧`sample[2]`、`acquire(v,a,out)`、`read`及`release`完整保留；旧pair包括Solo继续保存/验证6槽，Solo仅启用前3槽，后3槽仍保存并恢复。新增：

```c
#define RISCV_IOMMU_HPM_MAX_DEVICES 4
struct riscv_iommu_hpm_multi_sample {
    u64 cycles;
    u64 iotlb_miss[4], s1_ptw[4], s2_ptw[4];
};
int riscv_iommu_hpm_acquire_multi(struct device *const devices[],
    unsigned int nr_devices, struct riscv_iommu_hpm_session **out);
void riscv_iommu_hpm_read_multi(struct riscv_iommu_hpm_session *session,
    struct riscv_iommu_hpm_multi_sample *sample);
```

multi允许1..4设备，slot `3*d+{0,1,2}`分别为该设备的TLB miss/S1 PTW/S2 PTW；最多0..11，inhibit bits1..12。共享内部helper/session动态保存数量，验证唯一DID、同IOMMU、同task持锁。先inhibit并保存所有owned selector/counter，再逐个selector精确DID readback/清零检查/启用；失败恢复**全部已保存槽**和原inhibit。release恢复所有owned槽，不动其他槽；cycles不清零，保留其原inhibit状态恢复。仍非原子快照，不把软件差分说成与V硬件边界完全重合。32bit桥读取沿用high-low-high。

## 多A安全调用链（下一批，不在HPM批实施）

1. 在同一`idma_experiment_lock`下取得全部device引用，检查所有quarantine、每路batch magic/空闲，逐路`idma_stream_prepare`；共用当前映射、缓存交接与校验机制。
2. acquire_multi后一次缓存交接。推广当前`idma_run_window:392–449`为有限软件warmup：先启动各A，V每次完成后补交，轮询/补交每名A；完整保留超时/信号和每路quarantine。预热后停止软件补交并逐路drain，再配置所有batch；V的next_offset复位0，A沿用现有warmup后的明示状态。
3. V batch目标4096；每A沿用既有长目标`IDMA_EXPERIMENT_MAX_TRANSFERS=2^32`，在V之前依次启动。ARM仍只编码V与selected A；仅在所有A已成功启动后启动V。V发出前/结束后记录每A控制状态；启动顺序/软件时间照实输出，不声称同时起跑。
4. 热循环只轮询V，保留`idma_batch_window:601–615`的截止时间、信号和V首次观测完成时刻。硬件window依旧由V首issue至第4096个done自动冻结，源码`dma_observer.sv:148–154,199–203`。A全部自主batch；每A长目标不会正常提前用尽。任一路启动失败时不启动V，停止已启动A并排空。
5. V完成后先对所有A发STOP（不取消AXI），再取HPM/active后快照，逐路drain→batch_collect→byte/count检查；无错误时一次缓存重新交接并逐路verify。STOP前须验证所有A仍处active且目标未耗尽；否则保留试次并标为不满足持续A合同，不能补采替换。原错误优先，无法排空的lane保留缓冲与quarantine，不能cleanup释放。
6. 冻结observer保留V/selected A的所有旧守恒；主机记录shared_ar/aw总量及`shared - (V+selectedA)`非负残差。残差只标**未选中流量的总量口径/边界差额**，不拆为每A的PATH或精确窗口COPY进度。neither/v_only/a_only/both只相对V与selected A，不能称全体设备的重叠。所有A的最终完成数/bytes与HPM分别输出。

被动硬件无法给DMA2/3各自的PATH或精确V窗口COPY完成数；若图表要求这些新增字段，须另提RTL请求，不能靠软件采样伪造。当前软件方案接受范围是V精确硬件窗口、全部A持续及最终正确完成、逐DID HPM、selected A PATH、shared总量。

## 验证与整合边界

HPM先做真实C函数MMIO桩测试1..4路、重复DID/跨IOMMU/busy、selector失败完整恢复、旧pair/Solo回归及实际kernel对象编译；结果/patch单独交付。multi driver只能在Root直接审查HPM及A2 P0接口稳定后开下一批，不并发大改idma.c，不构建新BIT。新软件最终与A2 P0合成同一payload，兼容原DS0 BIT与P0 BIT，具体由Root整合和上板。
