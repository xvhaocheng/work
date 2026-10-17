# E6-PN 来源与实际调用链

## Material Passport

- ARS-Codex `experiment-agent / plan`；2026-10-18 JST；只读来源核查，未执行旧仿真/板测。
- 计划见 [port-plan.md](port-plan.md)。行号区间针对本页明确指定的原字节；活动HEAD不替代固定commit。绝对路径可供本机Root直接读；恢复时用下列repository-relative path与完整commit。
- 本批Codex直接核关键原文、RTL及七文件逐字节比较；没有DeepSeek/Claude/Astra调用。实际运行模型/原生窗口元数据不可见，保持所请求Sol/ultra而不伪造验收。
- [只读核查原始回执](source-check01.json)保存实际四组件HEAD/clean状态、lock/gitlink输出、七份donor原字节比较与报告hash；`passed`只限该只读来源范围，不是功能/构建/板测通过。

## 1. 新独立工作树与固定基座

| 仓库 | 新独立工作树 | 完整起点（branch均`ispass/e6-pn`） |
| --- | --- | --- |
| 顶层work | `/home/xvhaocheng/work-e6-pn` | `7123bbfe79cca298c36a880206ab85a2d68e6f67`（创建时main） |
| platform | `/home/xvhaocheng/work-e6-pn-platform` | `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273` |
| CVA6 | `/home/xvhaocheng/work-e6-pn-cva6` | `8aac3fcd345af7686a55af1867783fd6b91a0069` |
| IOMMU | `/home/xvhaocheng/work-e6-pn-iommu` | `85b157ebe71a623e326e51fff7b7126fe77e8eb9` |
| Linux | `/home/xvhaocheng/work-e6-pn-linux` | `45bf76b15e36cf608dff95013d91f8bedb7b1b8e` |

已核platform `sources.lock.json`、`git ls-tree <platform> cva6 linux`与CVA6 `git ls-tree <cva6> corev_apu/riscv-iommu`一致。新组件分支没有修改或新提交；它们是独立平行worktree，未把空child目录误作完整deps，不初始化/升级子仓库。本批顶层只新增本目录两份设计及一份只读回执，不动Root或旧A1树。

统一原版不是当前研究整合HEAD。版本导航 [versions.md](/home/xvhaocheng/work/iommu-platform/docs/versions.md:29) 和 [来源分层](/home/xvhaocheng/work/iommu-platform/provenance/source-organization-20261017/README.md:7)区分upstream、适配、corrected与research capture；旧mode0不构成原版证明。85b必要修复链包含AW-first进度、AW owner直到WLAST、HPM边沿、PTW leaf、AXI边界，移植不得倒退。

## 2. 历史来源身份与本批实际比较

实际旧BIT输入根为：

`H=/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6`

`I=$H/corev_apu/riscv-iommu`，`C=/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007`。

旧执行是当时workspace overlay，**不能把后整理commit说成当时运行HEAD**。`C/overlay.json`列24份输入，其中有旧observer、P4M frontend、FIFO/DS/PTW等；`C/build-command.json`实际为 `scripts/platform.py fpga --profile migration-4dma-composition --workspace-overlay ... --fpga-stage ...`。这些是只读来源，不能整包用作本次构建输入。

| 来源 | 完整固定身份或SHA256 |
| --- | --- |
| 实际P2增量 `C/production.patch`，仅IOTLB/TL两文件 | `a3603eee35bef84be97a670d5ad86121b97374af9b09dfd590e58a8c75ff6b8d` |
| `C/overlay.json` | `3e4dbe12834f9978347a3ec0caa110e4915050af69de609b4db33652dfaa9bbb` |
| `C/build-command.json` | `4456ce3420a4713c5a11e9bc68070e02177351a29db424901c2ae02b55cc726e` |
| `C/board-acceptance.json` | `748e4e3d2eabfea0233686cc2019eed11dab76974ebbe6549d4953325a87fd66` |
| 后整理B1：banked lifecycle/owner准入 | IOMMU `6a47d91fd19161430e1ef2a59041e2b23870f17c` |
| 后整理N1：真实cache hit/touch | IOMMU `011e7b5f82e4d4fe03aec765067d399ffded4c2d` |
| 后整理P1：allocation mask/quiet clear | IOMMU `b35d23078036e4ce6e22c728b303b68a3fe9d56f` |
| 后整理P2：局部替换历史（本次donor终点） | IOMMU `0b7939445298a23ea3e131ffd4e003cebb15ac59` |
| CVA6旧完整copy证书 | `c7527202b8c93d751d73bf34892df3883c0454af` |
| CVA6旧大FIFO FPGA存储选择 | `f4432405308bb27a5708b7bb4b9d2116b78373a2` |
| CVA6旧composition control（只读，不整搬） | `46cfd935b2106e8e92d7d7ade2d1d222913a3c82` |
| CVA6研究接线capture（只读，不当基座） | `991074e7fca9eddd33c61d72767c3729f573dc3e` |

本批用 `git show 0b793944...:<path>`与`I/<path>`原字节比较，七项均相同（不是仅feature-family关联）：

| IOMMU相对路径 | 实际旧输入SHA256 |
| --- | --- |
| `rtl/transaction_controller/rv_iommu_banked_ctl.sv` | `613f044b4cd2519128dab4b5ac44d1edfb9d53cd84a0c3c4f84d1ea2062bd852` |
| `rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv` | `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f` |
| `rtl/translation_logic/rv_iommu_nonblock_hit.sv` | `657f277a023c29c0ae4821fdf0b49f9535cfd2d36d66c4a8c880e0074f902010` |
| `rtl/translation_logic/rv_iommu_tl_wrap.sv` | `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae` |
| `rtl/translation_logic/rv_iommu_iotlb.sv` | `2868d6de26c8d98d2a750a0d78f6ce96cb896b9253b82b070a3e3772f37e9d26` |
| `rtl/translation_logic/rv_iommu_ddtc.sv` | `0e0a79973b2be796f4bc532cd0619f6f75787be962b2b433eb812f1dca353528` |
| `rtl/rv_iommu_top.sv` | `d0ceda29c99904c8d8f3567133b17bf9be784bdc8eb67a3b5cc5542ccba56669` |

比较只覆盖这七份明确原件，不推广为完整build闭包一致。85b是P2祖先；但85b→P2完整差异12文件、1483插入/62删除，含E3 queue/delay及research包，不能整栈cherry-pick。`production.patch:3–14`已经假定TouchEnable/PolicyEnable存在，说明它不是85b可独立套用的完整P补丁。

## 3. P：fill与touch实际调用链

| 步骤 | 固定源/行号 | 必须保留的语义 |
| --- | --- | --- |
| 原版lookup/touch | [85b IOTLB](/home/xvhaocheng/work-e6-pn-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:328) 328–363 | 原PLRU按命中路更新，full mask victim位对位对照；没有invalid-first |
| P2增量 | [production.patch](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/production.patch:15) 15–74 | 固定2/14、history_nodes、resident-way局部touch，TL开参数 |
| 稳定fill owner/mask | [donor TL](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:657) 657–683 | 主PTW/MSI `req_data.did`与protected DID比较，mask0003/fffc；fast DID不改fill owner |
| mask victim | [donor IOTLB](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:353) 353–373、405–424 | eligible树只限allocation；S/N full mask reproduces原树；policy_clear清PLRU |
| resident-way touch | [同源IOTLB](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:384) 384–400 | `i<2`选protected历史节点，另一分区反之；不拿当前policy_mask判断touch归属 |
| tags clear | 同源IOTLB 215起 | policy clear使tag失效；不是只换mask后留旧驻留 |

`research_o` entries/pre-next/PLRU快照在donor IOTLB427起，本次不移植。P只需必要mask/clear/touch端口，不需要40counter+16×128bit双snapshot诊断。

## 4. N：真实候选→接受→响应

以下行号属于上表P2原字节，七份与实际BIT输入相同。

1. [banked_ctl](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:235) 235–244用原始AXI ID bit选bank，不用DID；162–168将8/32均分4/16；280–298 input demux `MaxTrans=16`，不加spill；314–356各bank独立controller。
2. banked_ctl367–390只在完整bank-local请求上加bank位，经RR仲裁；400–408按response.bank路由ready/valid；413起输出ID prepend/drop/reconstruct须逐字段保留。one lifecycle/bank不是每DID任意多笔。
3. [TL](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:282) 282–306：normal_gate、quiet全条件、另一bank/PT/S1/Bare/PTWactive窗口；315–329把真实candidate/cache结果接hit模块。
4. [nonblock_hit](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:47) 47–58选start/lookup；62–99核DDTC/DC、Sv39/Bare、权限、canonical、4KiB与输入/leaf MSI排除。
5. **真正N接受**是同源100–103 `state_q==LEAF && enable_i && candidate_valid_i && candidate_i==req_q && !invalidate_i && ddtp_i==ddtp_q && dc_ok && ddtc_dc_i==dc_q && leaf_ok`。106–107只有accept才cache touch；TL287 `nonblock_fire_o=n_accept`是最小新增计数应接的实际事件。127附近probe/diag hit不是accept。
6. nonblock_hit157–171只有accept形成response并保存bank/physical/权限；OUT直到ready。TL346–353 RR merge fast/slow响应。拒绝候选走REJECT后等待正常handshake/候选撤去，不能丢弃或自行发正常response。
7. TL503/516 DDTC TouchEnable/touch、657/676 IOTLB TouchEnable/touch是探测不扰历史的必要支持。不得把每个probe lookup当普通hit计入HPM或touch。

N所需PTWactive已经是85b `rv_iommu_ptw.sv`的`active_o`原输出；旧TL把它命名`e3_ptw_active`不代表必须搬E3主动延迟或level计数。85b→P2的PTW文件改动只是新增`e3_level_o`读出及文件尾换行，本次不需要该改动，保持85b PTW原字节。DS接口同理保留85b owner修复，不复制旧E3 delay通道。

原85b top111–139的req/resp没有bank；322起单controller、380/451起TL。原TL145–183保留normal arbiter+spill，不能被旧NonblockEnable全局gate无声替换；原controller599–600 AR选择没有read-admit新增条件。

## 5. N依赖：小buffer、W-owner与MSI

| 边界 | 实际源 | 结果 |
| --- | --- | --- |
| 原版数据缓冲 | [8aac DMA wrap](/home/xvhaocheng/work-e6-pn-cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:27) 默认BufferDepth3；273–286接真实backend | 64bit×3=24B；原peripherals1018–1034未覆盖buffer参数 |
| 旧N大buffer | [H/peripherals](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1269) 1269 `IOMMUNonblockMode !=0 && i<2 ?512:3` | DMA0/1各4096B，较原各+4072B，合+8144B数据容量；存储/控制开销另列 |
| 原因与owner准入 | [I/banked_ctl](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:246) 246–275 | 非owner读可堵满小R→W FIFO，而其AW被前owner W锁住；共享R又挡owner数据/PTW，形成环；不能删防护 |
| 真证书豁免 | [I/trans_ctl_wrap](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:645) 645–647 | 只在read_admit或对应DID真实fullbuffer_job许可时选新AR，不撤已offer请求 |
| 证书来源 | [H/idma_fullbuffer_certificate](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:14) 14–41；wrap173–183 | capacity>=4096、normal、长度/对齐、真实jobfire/retire、无异常重叠；不能软件伪造 |
| 原MSI/容量 | [8aac soc cfg](/home/xvhaocheng/work-e6-pn-cva6/corev_apu/tb/ariane_soc_pkg.sv:110) 110–130 | outstanding8、WFifo32、IOTLB16、DDTC8、ATS0、MSI_BT_MRIF |
| donor不支持MRIF | I/banked_ctl143–147 | MRIF fatal；ATS须0 |
| donor全局变更 | H/peripherals1392–1408 | `nonblock_cfg()`改BT_ONLY，BankedEnable/NonblockEnable始终1；runtime S也受影响 |

更具体地，donor banked314–356实例各bank时没有连接其MRIF request/fault输出，348/355只接输入；360–363总MRIF request/fault绑0。base `trans_ctl_wrap.sv:653–663`在TL返回`is_mrif&&!error`后进入MRIF并等待`mrif_req_ready_q`。因此保留全局MRIF能力不能靠删banked fatal或忽略handler口实现；未接通的MRIF有真实挂住路径。当前受限N实验域与任务发出前显式切S/P回退提案见port-plan §1，仍待Root裁决，不声称N任意MSI/MRIF透明兼容。

软件原件已直接补核：[45bf enable_ir](/home/xvhaocheng/work-e6-pn-linux/drivers/iommu/riscv/iommu.c:953)953–978在CAP_MSI_FLAT时初始化256项MSI page并设置DC.msiptp=FLAT、mask255、pattern IMSIC；[probe](/home/xvhaocheng/work-e6-pn-linux/drivers/iommu/riscv/iommu.c:1313)1313–1316调用enable_ir；85b `rtl/software_interface/regmap/rv_iommu_regmap.sv:465–467`广告MSI_FLAT。因此N受限域不能写成“DC禁MSI”；应允许普通flat context、核实际payload IOVA与叶子GPA不匹配MSI范围。历史hit helper62–99正支持这项排除。实际阻止MRIF/非ordinary输入的执行位置仍待Root锁，不能虚构已有prepare约束或devmem不可绕过。

Root已选同BITS/P原controller+backend3、N/PN另历史banked+backend512/真证书支路，共享TL。原backend实际插点是8aac wrap160–176的job FIFO输出与273–319的request/rsp/protocol/busy接口；无需复制寄存器前端/idgen/Job FIFO，也不运行时改BufferDepth或异步reset。N额外两backend各4096B，resident新增8192B；历史8144B只是活动容量差，不能替代同BIT实际驻留成本。`idma_backend.sv:112`还有MetaFifoDepth=BufferDepth+NumAxInFlight+MemSysDepth，:633–647每份depth520的2bit last FIFO；不能只核8192B payload。

512-byte-lane FPGA RAM选择在donor `idma_stream_fifo.sv:105–110`及固定f443提交的vendor `common_cells/src/stream_fifo.sv/fifo_v3.sv`参数支持；默认小FIFO不变。最小源码清单应显式列这三文件、证书与wrapper，不把存储选择藏在构建器或动态全局vendor覆盖中。面积/BRAM/LUT/FF只有后续综合可验收。

## 6. 现有BTC1/OBS3软件接口边界（与A3分工）

| 固定接口 | 源码入口/行号 | 可观测或缺失 |
| --- | --- | --- |
| batch配置 | [8aac frontend](/home/xvhaocheng/work-e6-pn-cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:124) 124–143、279–287 | 0x80BTC1magic、88control、90target、98pages、a0done、a8issued；原无gap，未定义高bank写error；Root已授权必要最小cooldown设计，本批未改 |
| 后续供给 | 同源350–362 | 完成后WAIT→COUNT，无gap状态；不能借旧P4M/G代码称原版 |
| 原硬件窗口 | [8aac dma_observer](/home/xvhaocheng/work-e6-pn-cva6/corev_apu/fpga/src/dma_observer.sv:148) 148–156、171–204 | V首issue→目标done，35counter；Batch A必须一直active，否则error5 |
| PATH/索引 | 同源29–63、84–99、238–270 | OBS3，8bit index；0..34与64..191有效，其他拒绝；要加PN192..199须显式新增decode/cap |
| AXI窗口 | 同源178–203 | V/A入AR/AW/RLAST/B、AWwait、开闭stock；没有A whole硬件elapsed、无PTW配对计时 |
| Linux目标/STOP | [45bf idma.c](/home/xvhaocheng/work-e6-pn-linux/drivers/misc/idma.c:941) 941–943、963–980（A3直接核） | V4096，A2^32；V完成后STOP A并drain。A final completed_bytes含窗口外，不能除V窗造own吞吐 |

实际统一baseline CLI来自platform **`a2e847539bb638662f98dc920de0e8fd63d046e3`**，binary SHA256 **`f91112f6d844af1fc683266a87a287edcbc42ddd9b3b419f40e37aac16a732f8`**，不能用platform885的旧P4M bench替代。A3完整工具合同当前入口为 [measurement-contract.md](/home/xvhaocheng/work-e6-pn-tools/paper/ispass2027/exp/E6-pn/measurement-contract.md)、同目录`software-plan.md / tool-source-map.json`；由独占A3分支交Root整合。本批MODE bank尚未硬编码软件。

用户已批准约256counterFF+flags的最小被动A own/union时长：197=A own、198=union、199=valid/overflow flags。原ARM后首A/任一路issue起算，最近对应done更新elapsed；V原窗冻结后继续跟踪，原STOP/drain与真实双方quiet后冻结。原实际A预算仍2^32背景，不改成4096自然结束；分子绑定同次ARM后真实A completed bytes。没有事件FIFO/旧G observer/新身份，PTW AR→R仍跳过。此前reserved0是方案历史，当前正式布局按此替换。

## 7. 四份指定报告：原事实与本次不能推断的结论

| 指定原文 | 已直接核的内容 | 本次使用边界 |
| --- | --- | --- |
| [composition §§3/8/9](/home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md:23) 23–40、84–105、109–131 | P2后A miss同2048，PN仍慢12.67–12.79%（n3）；实际DMA/行为DDR模型FIFO下+1.744%，DS优先模型−0.455%；同BIT18点V64 gap敏感 | 旧DS50/50、旧buffer/banked/供给/内核；不等新统一原版。模型干预不代表真实MIG/整板因果，不搬DS优先 |
| [workflow-g0-m0](/home/xvhaocheng/work/iommu-platform/docs/workflow-g0-m0-results-20261010.md:7) 7–27、31–39、54–58 | 双方自然完成64、共同t0的行为模型；DS优先消P/PN差；各配置确定性单次 | 非板测、CPU关闭、非本次背景A STOP合同；不声称15%硬件收益，不作为恢复新gate依据 |
| [problem-discovery](/home/xvhaocheng/work/iommu-platform/docs/problem-discovery-results-20261017.md:15) 15–44、46–81 | 8条件真实DMA/行为DDR；基座含承诺前DS-first/F4、非stock；maskedPLRU14 quota仍R8反复walk；合法2MiB消重复walk/差额 | 不拷F4/2MiB/新替换；保留原负结果、普通组织边界，不能预设P/PN有效 |
| [DDR边界](/home/xvhaocheng/work/iommu-platform/docs/ddr-service-boundary-20261014.md:3) 3–19 | 实际MIG4.2 AXI64/fullID6，upsizer64→256；已接受读/heldR不可撤或read-vs-read提升 | 原C++返回优先不是硬件外部能力；本次不加PTE计时或拥塞诊断，不把AWwait当MIG内部阻塞证明 |

本次读取报告SHA256：composition `e12ad5e3da0bdf4bf22bda80051d05a9e565d2b0ec087a7eb9ae750959248f1f`；workflow `481f69b0e25d4a6662a186eef0a93deaf28840aa8d10275f638c0a8f2e259bea`；discovery `62ae0462483a860589cac61f9f7594d4624fae801ff22f3ced9c99f9f846b04b`；DDR `4cfeae39049cd8b1e47dd798ea9e76aece71d7307abb7cb174acaa73ad9980f7`。这些是本批读取身份，不改历史raw/报告。

## 8. 历史一次post-route流程

[C/route-opt/optimize.tcl](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/route-opt/optimize.tcl:136) 136–153：打开原routed DCP；143仅一次 `phys_opt_design -directive Explore`；145–149仅当不完全路由时至多一次 `route_design -directive Explore`。原脚本包含旧绝对输出地址，**后续只复制到任务内修改输入/输出路径，不能直接执行旧脚本写历史目录**。

旧 `board-acceptance.json`/`execution-state.json`记录 native WNS−0.219ns、最终+0.022ns/WHS+0.051ns、三失败端点0/routing0，LUT156186、FF127714、Slice50556/50950；只证明旧构建曾通过，不保证新设计面积/时序。Root授权的新构建硬门是WNS>=0、三失败端点0/routing0，允许同一历史单次流程，仍失败停，不改约束不继续试。

## 9. 本批核验与待审

- 五个工作树建立成功，四组件完整HEAD匹配指定基座；platform与CVA6父子指针/lock核对一致。生产树无diff，顶层只本目录文档。
- 直接读production.patch全部74行、P/N实际关键调用链和四指定报告；七份donor与固定P2逐字节相同。历史证据仅只读，未运行validator、旧仿真、构建或板命令。
- 新RTL/软件/接口都未实现，未声称通过功能/时序/性能。Root已锁双支路首选、flat-context普通payload受限域、必要cooldown/Naccept计数与获批A/union时长，矩阵严格205独立试次；实际MSI/error拒绝接线仍须另审。下一小批只P IOTLB+TL必要mask/flush/localhistory和原gold/真实DUT有限检查，不启动完整BIT。
