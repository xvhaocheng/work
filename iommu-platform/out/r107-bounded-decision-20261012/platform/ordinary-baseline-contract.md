# R107 普通准备与接口合同（stage108）

结论：这份有限回放的普通准备路线在源码接口上可达；当前实验 helper 还没有 KV 适配接口。这个实现缺口不能当作翻译相关损失。全部属于同一可信 research-owner，因此允许 DID10 接管两分片四池的完整任务，并计其映射、别名与提交成本。本席未实现 adapter、未分配物理页、未构建/运行 DUT/上板。

## 固定实例与预算

实例合同见 [finite-contract.json](/home/xvhaocheng/work/iommu-platform/out/r107-bounded-decision-20261012/instance/finite-contract.json) / [finite-contract.md](/home/xvhaocheng/work/iommu-platform/out/r107-bounded-decision-20261012/instance/finite-contract.md)。参数来自本轮源席和 root：OPT125m 固定 revision `27dcfa74d334bc871f3234de431e71c6eeba5dd6`，L12、每分片 H6、D64、B16、fp16、N3，每端每分片 8 槽。两个平台回放分片用 DID10/11；这不是把 CUDA stream 认作硬件 DID。每池逻辑 2,359,296B，四池 9,437,184B。一次恢复共 1,769,472B、144 个源码等价的 K/V 连续 copy 段；12 层事件要求两分片该层全部有效数据完成。CPU 代理在显式选定的 cached-valid positions 上读取/核验的标记字节为 663,552B；它不是 native attention 的必读量，Sequence 长度也不自行证明 cache-valid。相对于这份代理标记集合的尾槽复制 padding 为 1,105,920B。无编造的计算、到达、期限或容量压力。

每池准备两个完整 2MiB extent、连续且 2MiB 对齐的 4MiB IOVA：共 8 个 order9 分配、16MiB backing、8 个目标大页叶，7,340,032B padding 归同 owner。各池可按 layer-major 的 K/V 视图普通打包；打包本身的 CPU/元数据成本必须记录。全池映射比逻辑 KV 占用大，二者分列。

更简单的备选是四个完整 4MiB（order10）连续分配。当前 MAX_ORDER=10，源码只拒绝 order>10，因此本实例仍在 generic 上限内；旧 10MiB/order12 不可达的结论不能套过来。实际成功率、碎片和时间未知。如果只申请/映射 2.25MiB，尾 256KiB 不能冒充第二个完整 2MiB 叶。

## 普通强对照必须保留

- 2MiB 分散 backing、四个完整 4MiB backing、合法 4KiB 回退均保留；4KiB 全逻辑池是 2304 页，若保留完整 16MiB backing 则是 4096 页，不能混报。
- 整池提前映射、合法预留、稳定驻留和批量映射允许。逻辑 block free 不等于撤 IOMMU 叶。实际 live3、容量8，不把源码保守预留当物理强迫。
- 同 owner 可授权 DID10 访问两分片四池，保留完全相同的恢复集合与层依赖。物理 backing 可复用；新增/替换 domain 映射与每个 alias 单独记账。当前 pair 拒同 DID 是 helper 限制，不是禁止合并的科学依据。
- 连续 copy 可跨 IOVA 叶，硬件 legalizer 已分段；不人为要求每个 extent 增加一条 descriptor。普通 SG/core 批量路径可减少同步调用数，但须检查实际 IOVA/PA 对齐及叶结构。

## 当前接口的精确缺口

现 ABI 只有 pages/chunk/transfers 等实验参数；每次 stream_prepare 分配/填模式，src/dst 以同 offset 循环，chunk<=4KiB，每次 cleanup 撤映射/释放。没有任意 KV 地址列表、tensor import、稳定 pool handle、逐层事件或 consumer proxy。底层 SRC/DST/LEN 寄存器前端可接受一般连续复制，DID 固定为设备参数；因此缺的是有限 manifest、池生命周期、授权/界限检查、全有限集合完成和层事件适配。

当前 NEXT_ID 返回非零才记提交，DONE==ID 可核单路完成，drain/quarantine 与源/未触及区校验逻辑可复用。旧 A 流在 V 窗结束后停止/排空，不能替代两有用有限任务的全部完成。未来适配若必要，必须先冻结 manifest；本轮不实现。

## 一份具体符号 IOVA 见证

以下地址只证明几何与授权安排，没有执行 map、没有实际 PA。四池 IOVA 分别取 `0x10000000`、`0x10400000`、`0x10800000`、`0x10c00000`，每池完整 4MiB；占 `[0x10000000,0x11000000)`。每池第二叶为 base+2MiB。实际使用前须核 aperture、DMA mask、IOVA 预留/空闲以及物理分配。

| 池 | 双 DID 授权 | 合并授权 | 逻辑/映射字节 | 符号物理 backing |
|---|---|---|---|---|
| 分片0 CPU source | DID10 READ | DID10 READ | 2,359,296 / 4,194,304 | P0 或 E0_0/E0_1 |
| 分片0 restore target | DID10 RW | DID10 RW | 2,359,296 / 4,194,304 | P1 或 E1_0/E1_1 |
| 分片1 CPU source | DID11 READ | DID10 READ | 2,359,296 / 4,194,304 | P2 或 E2_0/E2_1 |
| 分片1 restore target | DID11 RW | DID10 RW | 2,359,296 / 4,194,304 | P3 或 E3_0/E3_1 |

P_i 是未分配的 4MiB 对齐连续块；E_i_0/E_i_1 是两块未分配、各 2MiB 对齐的独立块。池内容来自原 full8-slot/layer K/V 预分配合同，普通 packed view 的偏移为 `((2*layer+kind)*8+slot)*12288+byte`，kind=K0/V1，最大逻辑字节仍为2,359,296。GPU target 的应用语义在 FPGA 上由 DDR restore pool 回放，不冒称实际 GPU tensor。全部映射页及 padding 由同 owner 授权。DID12/13 不访问此任务。

双 DID 方案和单 DID 方案都各有8个任务使用的目标大页翻译键；实际 map数为0。若保留 DID11 的4叶又将其增加到 DID10，必须报12条映射记录和相应撤销成本，物理 backing 仍16MiB。硬件已经有真实 preissue SRC/DST/NUM_BYTES/NEXT_ID 与 DONE 控制；所缺普通 adapter glue 与 live Image/叶/缓存/fence 验收分开，不能称为机制控制时点不可达。源等价事件是每 worker12个、合计24个，回放的12个 joint层gate要求两分片都完成。消费者只核显式缓存有效位置的 CPU 代理标记；固定源码会在 last-token position 再 forward 并在 layer wait 后 reshape/覆盖 cache。native attention 必读量、计算及时间未验。

## 域、叶与缓存边界

实际源码 IOTLB=16，查 [ariane_soc_pkg.sv](../../../cva6/corev_apu/tb/ariane_soc_pkg.sv):110–129；lookup 按 VPN、PSCID/GSCID、stage，DID 选择 device context，本身不在 IOTLB tag 中。两个独立分片两池各4叶，共8个潜在翻译键；同 DID 合并四池仍8叶，不因换 DID 自动减少不同分片的数据足迹。8<16只说明静态容量关系，不能证明零 miss、冷填次数、替换或时序。

`iommu_iova_to_phys` 仅返回 PA，不公开叶层级；必须核实际 PTE/层级。现 `find_iova` 仅扫描 PTE 空洞，未预留 DMA cookie；稳定适配需要合法 IOVA 预留或排他管理。`dma_alloc_attrs` 会自动建立映射，现 helper 又建手工 alias，且源/目标均 RW；不得把这些成本隐藏。READ source 可用，低层 destination WRITE 实际仍 RW，整页 padding 要归同 owner。

当前 S1 sync_map 发全 host S1 IOTINVAL、两次 IOFENCE 调用；PSCID/ADDR 限定被注释，且 void flush 路径忽略 post/fence 的 bool。map 返回0不单独证明 fence 完成。普通批量/提前 map 可保留；稳定有用窗口内不制造 remap。

默认 coherent config 可让 DMA API sync 直接返回，本 CVA6 helper 仍显式 FENCE.I。源码 controller 会 flush D-cache 并等 ack，WT cache 先 drain write buffer/MSHR；这是平台特定约定，非本轮实板证明。安全顺序为初始化→cache handoff→issue/done→所有 accepted ID drain→cache reacquire→verify/consumer→真正生命周期结束后 unmap/free。STOP 不取消 AXI；无有效 ID 或 drain 超时则 retain/quarantine，不先撤映射。旧 PAGE_SIZE cleanup loop 不能未经调整拿来撤完整大页叶。

## 成本与证据

`cost-inventory.json` 给 allocate、初始化/打包、domain/IOVA、map/leaf/IOTINVAL/IOFENCE、cache、issue/poll、layer event/proxy、全任务、drain/verify、unmap/free、失败回退的控制点；全部新运行耗时为 unknown。稳定寿命如摊销，必须报告实际使用次数、首次准备/稳态/回收。不能用旧 victim_elapsed 当 R107 端到端时间。

`source-evidence.json` 保存当前源码 SHA、必要完整片段与行号。当前 IOMMU HEAD 是独立维护后的57eb562，而旧 sources.lock 仍238a39e；工作树也含既有研究修改，不能冒称当前锁/已运行镜像相同。mode0 的3beats与 stage106 私有512beats分列；mode1源码首两路512且运行 NB关的配置不是 live 身份/时序证明。root 负责翻译残余损失与推进/保留/归档裁决。
