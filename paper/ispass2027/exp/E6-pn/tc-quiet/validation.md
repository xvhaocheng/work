# TC / banked / MRIF / vendor 只读 quiet：实际有限交付

Root 已接受 [`plan.md`](plan.md) 后实施。本批固定为：

- AXI vendor **`95f121700539d9e44430ec79786780d3a1f9e87d`**，直接 parent **`e72230ef6c8cd24387b788b69d02c10174decd45`**；独立 `/home/xvhaocheng/work-e6-pn-axi`、`ispass/e6-pn`。
- IOMMU **`6082d3bc2e0aeafcf4e86c42ddf1485c778fe941`**，直接 parent **`9713cd0cee4ba23fa37041d9bf48e2d58ece74af`**；独立 `/home/xvhaocheng/work-e6-pn-iommu`、`ispass/e6-pn`，其 `vendor/axi` gitlink精确指向95f1217。
- vendor仅 `src/axi_demux.sv` / `src/axi_demux_simple.sv` 两文件；IOMMU仅原三TC/MRIF文件+该gitlink。SW、TL、MSIPTW、DS、top、CVA、backend、约束不改。无完整构建、无硬件，board_cases=0。

所有新增逻辑只是现状态/当前offer的只读组合归约，**0新增状态**，无shadow counter/新flush/reset/READY或VALID反馈。原FIFO/ID库存/owner/WLAST/read admission/FullBufferDID/FSM/credits/原in_flight保留。Default `PnEnable=0` 输出0；只读逻辑生成裁掉。MRIF源末尾原无newline，编辑工具补上末尾newline；除此新增均为计划内接口/只读逻辑，所有旧sequential AST原样。

## 实际接口与适用边界

统一新参数 `PnEnable=0` 末尾追加，新输出 `pn_idle_o` 末尾追加。vendor文件内 id_counters输出 `pn_empty_o`。没有新论文统计bank或功能控制口。

- vendor `pn_empty_o = PnEnable && !(|occupied)`，取全ID现库存。simple合两库存empty、w_open0、AW/AR锁0和当前全部边界offer。outer合simple与AW/AR data、select **分别**VALID、W cut、R/B cut/output、边界offer；外部响应已进spill但ID库存退休后，仍不可误idle。
- `NoMstPorts=1`及`UniqueIds=1`没有真实ID债务库存，新idle保守常0。Spill不能替代响应未返空窗。数据继续原逐拍gold；不加shadow补齐。实际主配置MRIF TC3口/非MRIF TC2口均默认UniqueIds0；bank input2口显式0，实际XML已核这些specialization。
- singleTC合主FSM、AW/AR/W FIFO valid、w_cnt0/无当前overflow、completion demux真idle、MRIF FIFO/handler与所有当前TR/TL/comp/MRIF/fault offer。handler只有主FSM IDLE且无req/fault/memory offer/return才idle。`wr_state=B_RESP`的陈旧值不代表工作。无handler编译分支内部factor明确1。
- banked合原AW/AR/W三计数、partial、**signed w_unfilled==0**、两TC quiet、input demux/currentTL与compoffers。fullbuffer许可不是库存、不作为busy。实际BankIdBit=2/global8/32→bank4/16；ReadAdmissionEnable/owner原合同不变。
- 原output mux仍原2口、spill全0，未修改。有限真实test-only taps验证每个quiet1记录时W FIFOempty且AWlock0；无生产hierarchical引用。未发现反例。
- 这些是**TC/vendor局部生命周期/transport保护**，不是完整IOMMU quiet。MRIF错误后可能已回本地IDLE但DS尾拍未返，必须另合已接受DS debt guard；SW/TL/外层privateDMA库存仍由后续批闭合。

## 实际有限结果

最终三份原manifest/源/trace/XML/log在 `evidence/`，绝对原执行路径保留，归档为独立字节副本。实际测试从parent+dirty源码运行；[`summary.json`](summary.json)把执行SHA逐项绑定随后提交的实际commit，并未篡改执行时HEAD。

| 最终执行 | 真实DUT及gold闭包 | 情境/采样/检查 |
|---|---|---|
| `tc-vendor03` | candidate on/off 与e722原demux→原simple→原id_counters，只有namespace重命名，原CC依赖 | 18情境，539拍/1713eval，4341检查 |
| `tc-single03` | 真singleTC→真FIFO/error/ignore/MRIF handler→vendor；独立9713/e722原gold对应闭包 | 13情境，889拍/2800eval，6030检查 |
| `tc-banked01` | 真banked→两TC+原readadmission/owner/error→原mux，原9713/e722独立gold | 11情境，2137拍；2060个正常service的pre-edge原始采样，8400检查；reset循环未计trace采样 |

所有旧外部完整struct都比较，包括invalid payload与原in_flight；vendor/single每eval比较，banked每service pre-edge比较。gold不与candidate共享被修改的TC/MRIF/vendor模块；共同CC/未改mux/error/ignore的来源固定。TL响应、DS memory响应及fullbuffer许可在这些bench中由finite test-interface驱动，**不称真实TL/PTW、DMA full-copy证书或性能复现**。Single的MRIF main/child FSM/数据通路都是真生产RTL；不是行为式MRIF替代。

vendor包含2/3口的spill/bypass和NoMst1/UniqueIds1负域、AW/AR锁、W-before-AW、WLAST/B债务空窗、R非last/尾拍、R/B spill内部库存已退休但上游held、两ID库存、真实ATOP读库存注入、**同拍ARpush+ATOPinject+Rpop**（两种spill配置各1）。主参数从XML实取，不靠模块名或表格推断。

single包含8个AW/AR header FIFO、32个W先于地址的数据FIFO、REQ_TRANS/TL背压、normal主FSM提前IDLE但B/R仍欠账、SLVERR/ignore heldB；真实MRIF WAIT_INT_ID无busVALID仍忙、后继MRIF FIFO被handler阻塞、IP/IE两拍读与20拍间隙、IP写/notice地址/内容/WSTRB、B延迟、R/noticeB错误及fault_ready背压、成功notice后主IDLE且wr_state=B_RESP仍quiet。FIFO饱和/排队观察场景以reset终止，未宣称这些被reset的队列自然完成。

banked保留原11情境，新增完整on/off gold、quiet/库存约束和真实outputmux不变量。**73条 signed w_unfilled负值采样均nonquiet，1475条quiet采样均mux FIFOempty且AWlock0**。两bank各TC quiet与input demux真empty由实际实例接线，不以fullbuffer许可代替。

## 独立可重放证据核验

[`check_evidence.py`](check_evidence.py)实际通过 **13,718项**：

- 所有7个attempt的源与原始log/trace/XML hash、gold namespace完整变换、5份生产文件执行字节→commit、IOMMU真实vendorpin；各最终执行的生产字节完全相同。
- 从真实XML对**48个candidate参数specialization**与对应独立gold的全部原clocked AST逐项比较，原赋值/敏感边沿/断言条件相同，没有新PN信号进入原sequential过程。只规范化XML本地dtype索引、源位置、Verilator诊断字符串中namespace/行号（详见checker原失败说明）。不是单纯数寄存器名，也不是物理面积/时序结果。
- 原始quiet/库存/边界offer、unsupported常0、真实三事件同拍、banked负库存及mux闭合、MRIF旧状态/held FIFO/fault270/陈旧B_RESP的正负样本。35条主IDLE+wr=B_RESP且quiet，77条排队MRIF记录均阻止quiet。

只读重放，不跑仿真：

```sh
python3 /home/xvhaocheng/work-e6-pn/paper/ispass2027/exp/E6-pn/tc-quiet/check_evidence.py \
  --iommu-root /home/xvhaocheng/work-e6-pn-iommu \
  --axi-root /home/xvhaocheng/work-e6-pn-axi
```

恢复入口 [`../component-source/tc-quiet01/manifest.json`](../component-source/tc-quiet01/manifest.json)，两个独立 incremental bundle 均verify：vendor prerequisite e722，IOMMU prerequisite9713。先恢复vendor95f，再恢复IOMMU6082和其子指针；完整patch与实际源文件同包。未建立或推送第三方upstream remote。

需要新有限运行时，`run_quiet.py --kind vendor|single|banked`显式给 `--iommu-root/--axi-root/--deps-root/--verilator/--out`；输出必须不存在。每compile≤180s/jobs2，XML/run各≤30s。精确完整实际命令在对应manifest，避免重跑已过结果。

## 保留的失败/补充与局限

原 `tc-vendor01` C++文件变量名logf与math声明冲突，保留buildfail；仅改test变量。原 `tc-single01`测试oracle误写cause264，真实pkg/handler明确MSI_PT_DATA_CORRUPTION=270，保留failed/raw；只改oracle并在后继raw记录cause。生产5文件自初次实现后字节未改。`tc-vendor02`、`tc-single02`先通过，后续03只新增所需同拍事件/MRIF排队及raw taps，并非改变RTL后隐藏原失败。初版checker AST comparison误把Verilator错误诊断中的源码行号/namespace当逻辑差异；[`checker-attempt01.txt`](checker-attempt01.txt)保留原因与限定规范化，不删除任何断言谓词。

**实际FPGA source-list仍有明确后续工作**：Root发现CVA8aac `根 `Makefile:228–229`（axi_mux在227）`读取CVA普通flattened `vendor/pulp-platform/axi/src/axi_demux{,_simple}.sv`，不是IOMMUvendor副本。本人只读Git再次核两处原始字节都等于e722，记录于 [`flattened-source-pending.json`](flattened-source-pending.json)。本批不跨改CVA。之后经Root接受的独立CVA集成批须将vendor95f这两文件**原字节同步**到实际flattened路径，绑定vendor commit/SHA，仍保留IOMMUvendorpin。仅当前子repo已pin不代表未来BIT已读取新vendor。

新组合归约会增加只读负载/配置资格路径，0新增状态不等于零时序影响；尚无综合/routed结论。top/TL/SW/实际backend/外围MODE未整合，不能声称完整domain quiet、S15通过、物理合格或可上板。Codex-only执行，原生模型/窗口元数据未暴露，未修改模型缓存或窗口。
