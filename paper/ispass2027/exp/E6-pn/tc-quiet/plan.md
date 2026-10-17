# TC / banked / MRIF / AXI vendor quiet：独立只读批提案

2026-10-18，Root 已接受 DS-only `9713cd0cee4ba23fa37041d9bf48e2d58ece74af` 后的下一子批。**本文件仅方案/来源定位，未改生产 RTL、未编译仿真、未接 top、未上板。** Root 接受本方案后再实施实际 diff/有限验证。SW 九文件由 A2 独占；TL stable debug/ATS 限定另批；CVA/backend/frontend/peripheral/observer 不在本批。

## 1. 固定来源、将改文件与依赖提交

IOMMU 独立树 `/home/xvhaocheng/work-e6-pn-iommu`、branch `ispass/e6-pn`，固定 parent `9713cd0cee4ba23fa37041d9bf48e2d58ece74af`；三份 TC 源与原已审 N/history 父链相同，DS 新批保持原字节。真实内部 gitlink 为 AXI **`e72230ef6c8cd24387b788b69d02c10174decd45`**、common_cells **`f36828f3fd76fa6c79f80cb495829cf967bcf20c`**；不是把 Bender.yml 的 upstream dependency rev 冒充实际 vendor 身份。

[`source-map.json`](source-map.json) 冻结17份 Git blob/SHA/逐行原文，所有 read_path 实际字节等于固定 Git。vendor 阅读副本来自固定 DS0 build，已与真实 vendor Git 对比，不修改 build 副本。`freeze_sources.py --out NEW_FILE` 不覆盖历史。

生产拟改范围：

| 仓库 | 拟改文件 | 内容 |
|---|---|---|
| IOMMU | `rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv` | 只读 PnEnable/quiet、原 demux/MRIF 参数/输出接线 |
| IOMMU | `rtl/transaction_controller/rv_iommu_banked_ctl.sv` | 只读 quiet、input demux/两 bank quiet 接线；BankIdBit 实际有限实例锁2 |
| IOMMU | `rtl/transaction_controller/rv_iommu_mrif_handler.sv` | 现 FSM/输入输出旁听 quiet |
| AXI vendor | `src/axi_demux.sv` | 现五通道/两 select spill、当前边界 offer 加 simple quiet 只读输出 |
| AXI vendor | `src/axi_demux_simple.sv` | simple quiet，以及文件内 `axi_demux_id_counters` 现 occupied→empty 输出 |
| IOMMU | `vendor/axi` gitlink | 指向独立 vendor 提交，保留原依赖历史 |

vendor 实施时新建 **`/home/xvhaocheng/work-e6-pn-axi`、同名 `ispass/e6-pn` branch，从 e722 精确出发**；不切/改主目录 vendor 分支。先提交 vendor 两文件、制作 e722 prerequisite incremental bundle/完整 patch/source；再 IOMMU 三文件+vendor gitlink 提交，bundle prerequisite9713。common_cells/axi_mux/err/ignore slave不改、不新增源文件、不需要修改 Bender source 清单或随手升级其 dependency rev。parent CVA/platform gitlink/lock 不在本批。

## 2. 参数/端口和无状态约束

统一追加 `parameter bit PnEnable = 1'b0`；TC、banked、MRIF handler、vendor demux/simple 输出 `logic pn_idle_o`；文件内 id_counters 输出 `logic pn_empty_o`，也追加 PnEnable 默认0。新参数/输出不重排旧 positional 参数/ports。已有 named callers 可不连新输出；本批触及的层级显式传 PnEnable、显式连接新输出，不使用生产 hierarchical reference。

默认关闭时新输出保守0，新增只读组合归约裁掉；**无新增 FF、FIFO、shadow transaction counter、error latch 或复位/flush**。旧 READY/VALID、选择、计数/FSM nextstate/原 `in_flight_o` 字节与行为保持。新增 quiet 绝不用于本批任何旧数据/准入控制。它是外围将来 MODE 资格的只读输入，不是本批 MODE 实现，也不把它倒接旧 regmap in_flight。

下文 `req_offer(x)=x.aw_valid || x.ar_valid || x.w_valid`，`resp_offer(x)=x.b_valid || x.r_valid`。所有 quiet 同时检查当前 offer 和现库存，READY、旧 payload、选择指针、旧 MRIF 数据不代表 busy。语义限定原合法 AXI/既有容量/原协议检查；不宣称被动导出现状态能够新增非法协议容错能力。

## 3. vendor 三层精确谓词

固定源 `axi_demux_simple.sv:210–228/355–373`已有两组 ID counter；`:544,614`中 occupied 是各真实 in_flight 的 OR，`:225/227`写库存按原选择 reservation push→B retire；`:370/372`读库存按原 AR push/ATOP inject→RLAST retire。复用它们，**不按当前 ID lookup 的 occupied 代替全向量 empty**。

```text
id_counters.pn_empty = PnEnable && !(|occupied)

simple.pn_idle = PnEnable && supported_real_inventory
               && aw_all_ids_empty && ar_all_ids_empty
               && w_open==0 && !lock_aw_valid_q && !lock_ar_valid_q
               && no current slave AW/AR/W or slave R/B
               && no current master AW/AR/W or master R/B
```

`w_open`原 counter 与 AW/AR locks：`:88–102,137–198,232–253,295–343`。写 ID 库存从首个被锁定 offer 即 reservation，不可把它换为新 AW handshake 计数；保留 ATOP 注入原路径。读/写响应 mux 的当前/held VALID 由 `:264–280,380–457`读出；R/B arbitration 的 RR 指针本身不当 busy。

外层 `axi_demux.sv:89–160`分别有 AW data/select、AR data/select 两对 spill；不能只查二者 AND 后 cut.valid，必须分别检查 `slv_aw_valid_chan/sel`、`slv_ar_valid_chan/sel`，防止未闭合的一侧被藏掉。`:119–130,162–186`的 W cut、B/R output VALID 覆盖五通道 spill（Bypass时同样按当前 VALID 判）。

```text
demux.pn_idle = PnEnable && simple.pn_idle
             && !slv_aw_valid_chan && !slv_aw_valid_sel
             && !slv_ar_valid_chan && !slv_ar_valid_sel
             && !slv_req_cut.w_valid
             && !slv_resp_cut.b_valid && !slv_resp_cut.r_valid
             && !slv_resp_o.b_valid && !slv_resp_o.r_valid
             && no current slave AW/AR/W
             && no current master AW/AR/W or master R/B
```

**无库存 generate 分支按 Root 锁定 fail-closed**：simple `NoMstPorts==1` (`:73–75`) 直接接线，没有 ID 库存；`UniqueIds==1` (`:200–208,345–353`) 也完全省略库存。即使外层有 spill，也无法从它证明 AR 接受后→未来 RLAST 之间的空窗。因此在 PnEnable1 时这两类 `pn_idle_o` 固定0，数据通道仍原字节；不伪称局部 empty 是 transport idle，不新增 shadow counter 补齐。NoMstPorts0仍原非法参数，不改变原校验。

实际主配置**均不会落入该保守0分支**：

| 实例 | 原固定行/配置 | NoMstPorts | UniqueIds | spills |
|---|---|---:|---:|---|
| 原 singleTC `gen_mrif_support.i_iommu_axi_demux_mrif` | TC `:393–427` | 3 | 未覆盖=vendor默认0 | AW/W/B/AR/R全1 |
| 原 singleTC non-MRIF / N两 bank私有TC `i_iommu_axi_demux_no_mrif` | TC `:476–527`；banked `:159–174`仅私有BT_ONLY | 2 | 未覆盖=默认0 | 五通道全1 |
| banked `i_in_axi_demux` | banked `:279–307` | 2 | 显式0 | 五通道全0 |

参数传入时保持原这些数值；实际有限 XML 逐实例验证，不能仅凭表中推测。MRIF disabled 的 **内部 factor=1** 与 unsupported vendor 的 **输出=0** 是不同语义，不混用。

## 4. singleTC 与真实 MRIF

TC `:101`原 in_flight只等于 FSM非IDLE；`:679–683`地址送出后就回 IDLE，不能证明未来 B/R 已返。AW/AR/W stream_fifo (`:117–201`) 的 valid正是非empty（common_cells stream_fifo `:42–45`），复用，不另导 usage 或重数。原 w_cnt_q (`:172–219`) 仍必须0，并保守排除当前 w_cnt_ovf。

```text
TC.pn_idle = PnEnable && state_q==IDLE
          && !aw_valid && !ar_valid && !w_valid && w_cnt_q==0 && !w_cnt_ovf
          && selected_existing_completion_demux.pn_idle
          && mrif_factor
          && !trans_req_valid_o && !trans_resp_valid_i
          && no TR request offers or TR response offers
          && no completion request offers or completion response offers
          && no MRIF memory request/response offers or MRIF fault offer
```

全 MRIF compile 分支 `mrif_factor = !mrif_req_valid_d && !mrif_req_valid_q && handler.pn_idle`，包括原 MRIF FIFO (`:431–452`) 当前入队/出队与 handler。无 handler 分支 `:480–537`明确 `mrif_factor=1`：不引用不存在的层级，不接一个默认关闭/未驱动的新 idle0误阻该分支。error/ignore slaves不改；它们的接受后库存由现 completion demux的真 ID debt直到B/RLAST捕获，已返回未交TR则由outer response spill/current VALID闭合。error slave实例 `:544–557`、ignore slave `:376–391`仍原样。

```text
MRIF_handler.pn_idle = PnEnable && state_q==IDLE
                    && !req_valid_i && !fault_valid_o
                    && no mem_req AW/AR/W offers
                    && no mem_resp B/R offers
```

handler原 `WAIT_INT_ID` (`:185–205`) 即使当前 DS VALID0仍忙；MEM_ACCESS/FETCH/WRITE/ERROR (`:208–343`)亦然。外部 W 输入是对原TC W流的**旁听**，不是handler独有输入库存；TC自查W FIFO/当前TR offers，不把所有别的normal W流重复当handler所拥有状态。WRITE_NOTICE B成功后主state IDLE但wr_state保留B_RESP (`:319–324`)；**不要求wr_state==AW_REQ**。原错误路径在fault获收后可能先回IDLE，尚未返的DS尾拍依靠已接受的DS transport guard闭合，本局部输出不宣称单独覆盖DS外部债务。

## 5. banked 聚合与 output mux 边界

```text
banked.pn_idle = PnEnable && !in_flight_o
              && inflight_aw_q==0 && inflight_ar_q==0 && inflight_w_q==0
              && !w_partial_q && w_unfilled_q==0
              && bank0.pn_idle && bank1.pn_idle && input_demux.pn_idle
              && !bank_trans_req_valid[*] && !bank_trans_resp_valid[*]
              && !trans_req_valid_o && !trans_resp_valid_i
              && no TR / completion / per-bank completion current offers
```

现 32bit计数/partial原定义 `:522–584`完全不变；signed w_unfilled的负值亦非quiet (`:255–276`)，不能仅 `<=0`视空。read_owner/w_owner旧位无需清为0；fullbuffer_jobs是许可，不是owned transaction，**不拿许可常1当busy，也不用许可代替backend/job quiet**。MRIF输出 `:358–361`原0不改。

BankIdBit实际新 harness与后续top固定**2**，四口shared ID4中DMA0/1原ID0000/0100；不按DID猜bank。global8/32、bank4/16、MaxTrans16跟踪参数原样；不改credit、read admission、FullBufferDID、WLAST owner或真实许可语义。

原 output `axi_mux` 固定2口、所有spill0、FallThrough0 (`banked:416–438`)。本批**不修改 axi_mux、不新增其production idle端口、不用hierarchical生产取值**。其 existing W select FIFO与AW lock (`axi_mux:172–188,283–332`) 的关闭路径由两 bank 的completion demux库存/当前AW/W闭合：mux预留AW选择时，对应bank仍有真实AW offer/lock或W-open；W FIFO在该bank的WLAST被mux接受时pop (`:360–365`)，不会比bank的真实W-open退休更晚。AW尚未握手即使W先完成，AW locked/currentVALID仍使bank非quiet；地址接受后，bankdemux的B/R debt仍覆盖至上游退休。所有mux spills0，返回没有隐藏spill。未来有限必须用**test-only** taps证明`banked.pn_idle→mux W_FIFOempty && !AWlock`及所有当前无offers；若出现反例，停报Root，不擅自扩axi_mux或造counter。

## 6. 有限真实 DUT/gold 接受计划（未执行）

每次新目录，build≤180s/jobs2、每run≤30s，原失败/源码/manifest不覆盖；可按两个小harness拆compile，不启动长仿真。

1. **vendor真实 demux三级**：candidate PnEnable0/1分别与e722原demux+原simple+原id_counters组成独立gold（仅模块/内部绑定namespace改名），共同原common_cells；完整slave response/全部master request逐拍比较，invalid payload也比。2/3ports、Spill全0/全1、每对AW/AR data/select、5通道held、不同ID/同ID、multi-beat W/R、AW/AR锁、W-before-AW、delay B/R、同拍push/pop/ATOPinject原行为，库存未退休空窗quiet0、全排空quiet1。直接测试NoMst1和UniqueIds1全数据gold，PnEnable1 quiet恒0；defaultoff亦0。actual XML绑定主4实例类别参数、无新增sequential state；记录counter empty与locks来源。
2. **真实 singleTC + vendor + error/ignore + MRIF handler**：固定parent9713原全部相应模块/vendor e722单独gold闭包，candidate PnEnable0/1都逐拍比较旧TR/comp/TL/MRIFmemory/fault/in_flight。原cfg8/32+MSI_BT_MRIF，及私有cfg4/16+BT_ONLY；ReadAdmissionEnable原值不改。TL response与memory由finite harness驱动，明确test-interface，不称真实TL/PTW/完整系统。实际原FIFO满/empty、REQ_TRANS背压、AWAIT_RESP、queuedAW/AR/W、heldTLresponse、COMPLETE→IDLE但B/R欠账、error SLVERR排空/heldB、normal读/写R/Bspill，均quiet0；无事务/完全排空quiet1，默认关闭quiet0。
3. **真实 MRIF路径**：TC真MRIF response→真MRIF FIFO→真handler→原ignore slave，实际W interrupt-ID驱动；WAIT_INT_ID bus空窗、MRIF读两拍间隙、IP写/notice写B延迟、fault_ready背压、success后陈旧wr_stateB_RESP、无MRIF compile factor1。所有旧行为仍gold；本批只验证局部quiet，延迟DS尾拍要与已有DS guard合取才是完整域，不能拿handlerIDLE冒称全系统idle。
4. **真实 banked + 两TC + vendor**：同9713原bankedgold闭包，不复跑已接受N cache/TL性能测试。BankIdBit2/真实ID0000/0100；原global8/32/bank4/16；测试两bank queued/held translation、原fullbuffer许可与owner准入、W-before-AW/负w_unfilled、partial/nonlast/WLAST/同拍AW/B/AR/RLAST、SLVERR不走completion、跨bankcompletion mux heldAW/W/B/R、完整排空。许可由harness驱动明确test-interface，非真实DMA full-copy证明。每个quiet1边沿test-only核原muxFIFO/lock为空；defaultoff旧路径逐拍gold、enabled亦相同，禁新增任何data/READY反馈。
5. **原件与恢复**：before-run实际完整Git/dirty/diff/新文件SHA、vendor/cc源码身份、gold每项变换、命令与budget；commit后执行字节逐项绑定commit；源码/manifest/raw trace/log/failedattempt本地和任务Git保留，bundle验证两级prerequisite。模型元数据不暴露则如实未知；Codex-only，不改缓存/窗口。有限合格后交Root实际diff+source+raw复审，**未授权完整FPGA build/硬件**。

Root已在本轮确认unsupported vendor分支保守0及MRIF-disabled内部单位元1。剩余端口/谓词/依赖按本提案待Root快速接受后进入实际代码批；不重新向用户索取普通许可。
