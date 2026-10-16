# M：以有界同步通路關閉 late-P1 的 first-PTE intent

## Material Passport

- Origin Skill / Mode：ARS-Codex `experiment-agent / plan`，本件是有界機制設計；非全 pipeline。
- Origin Date：2026-10-16 JST。
- Version Label：`FIRST-PTE-INTERFACE-COST-1 / M-ID-ordered-v2`；補入 P1 必達所選 DRAM decode 目標的 root-table 資格。
- Verification Status：`SOURCE_BOUND_DESIGN`；只有原始碼閱讀、結構與整數算術檢查。未實作 RTL、未模擬、未綜合、未建新效能模型、未重跑舊 70 筆、未板測。
- 模型建議／請求：Sol / xhigh；實際獨立 model / effort metadata：unknown。不以請求配置冒充執行證明。共享 IOMMU 本輪 Codex-only。

**接受候選是 M-ID-ordered：只有已確認 normal-S1 root table 整頁位於所選 DRAM decode 窗口、root/route 在本 pair 穩定的來源才發 qualified intent；實際 start 經固定、無排隊、同時鐘控制通路送到 DRAM 輸出 mux 的 AR 選擇器，用既有 PTW 身份識別 P1。主版 f=1，即門本地同一 start 邊緣捕獲；f=2 是可選的一級來源寄存。控制訊息不是可任意延遲的獨立 mailbox。** 源碼給出不借人工 DS50 延遲的 P1 最短路徑：start 後 C3 才能首次出現在該選擇器。f≤3 的意圖嚴格早到，f=4 最早與 P1 同拍；連同 P1 目標資格，主版不需要在每筆 DS AR 中傳遞 marker，也不需要 P1 issue/seen/ack 來修補 late-P1。

這個關聯結論與能否搶在 L 前是兩件事。f=2 仍可能晚於 C0 已可選的 L，不能沿用舊模型 `intent=0/Lready=0` 的 2060。主版 f=1 能覆蓋該最早相位，但實際布線 setup／hold、額外延遲與原生共現窗口仍未知。不存在「光靠 single walker + ID + timeout 即可」的無條件版本。

## 1. 當前來源身份與路徑

本件下列路徑均以 `/home/xvhaocheng/work/iommu-platform/cva6/` 為基準，`I/` 縮寫為其 `corev_apu/riscv-iommu/`。實際 bytes/hash、差異摘要與檢查見同目錄 `evidence.json`。重要判斷均直接核過下列原始碼，不由舊摘要互引代替。

| 項目 | 本輪觀察 |
| --- | --- |
| CVA6 component HEAD | `a1b08934391833aed21e6ad63b545634040808ac`；兩個 ariane 檔及 Makefile 有既有工作樹差異 |
| IOMMU component HEAD | `57eb5621802ef9ed8d77c684b9a03205c6630391`；PTW、TL、DS、top 均有既有差異 |
| sources.lock | CVA6 指向上述 HEAD；IOMMU 仍指向 `238a39e66a07b27a67fb5749df40691682555027`，與當前組件 HEAD 不同 |
| 本件作用對象 | 當前工作樹的具體整合草圖，非已載板映像身份。`scripts/platform.py:250–253` 依 lock 匯出後再套工作區 overlay；不能由讀到本檔宣稱既有 frozen fpga-src/bitstream 自動一致 |
| 實際 AXI/common_cells 定義 | `Makefile:209–240,793–799` 明列 `vendor/pulp-platform/{axi,common_cells}` 並生成 FPGA add_sources；不是 `I/vendor/axi` 的另一份同名模組。`corev_apu/fpga/scripts/run.tcl:41–43` 使用相應 include 與生成清單 |

本輪所讀 PTW diff 是既有 4KiB leaf 修正，不改 start／MEM_ACCESS／PROC_PTE 串行性；DS diff 是 AW/W owner FIFO 修正，不改 AR arbiter/spill。TL/top 的既有 banked/nonblocking 改動確實改變請求來源，因此 bank 與方向用當前檔案，不拿 upstream HEAD 的接口代替。沒有修改上述檔案。

當前 `ariane_xilinx.sv:165–172` 的源碼預設是 DS input/output delay 50/50、nonblock mode 0。`ariane_peripherals_xilinx.sv:1404–1409` 只在 nonblock mode 非零時開 banked controller，`:1262–1270` 只在該模式 DMA0/1 配置 whole-copy buffer。本設計的兩 bank、兩完整 COPY 切面屬這條已有可配置路徑，不能說源碼預設 mode0 本身已實例化它；本輪亦未產生新 profile。

實際請求鏈：

```text
I/rtl/translation_logic/rv_iommu_ptw.sv
  → rv_iommu_tl_wrap.sv 的 E3 adapter（AR 直通）
  → I/rtl/rv_iommu_top.sv 的 ptw_axi_req
  → I/rtl/ext_interfaces/rv_iommu_ds_if.sv 的 AR arbiter + DS AR spill
  → ariane_peripherals_xilinx.sv 的 axi_delayer
  → ariane_xilinx.sv 的 slave[IOMMU_MEM=3]
  → axi_xbar_intf / axi_xbar / axi_xbar_unmuxed：ingress AR spill、decode、1×multicut
  → gen_mst_port_mux[DRAM].i_axi_mux：AR arbiter ← 新門所在位置
  → 該 mux 的既有 output AR spill
  → master[DRAM] → axi_riscv_atomics_wrap → dram
  → xlnx_axi_clock_converter（clk → ddr_clock_out）→ MIG AR
```

對應原文：`I/rtl/rv_iommu_top.sv:316–338,542–547,624–629`；`ariane_peripherals_xilinx.sv:1353–1387,1444–1456`；`ariane_xilinx.sv:291–319,838–852,934–945,1006–1083`；`vendor/pulp-platform/axi/src/axi_xbar.sv:102–162`。

PTW、TL、DS、delayer、xbar、輸出 mux 都使用同一 SoC `clk`、`ndmreset_n`。`ariane_xilinx.sv:312–314,839–841` 把相同 clock/reset 接往 xbar/peripherals；IOMMU 往內續接 `clk_i/rst_ni`。真正 CDC 在 `ariane_xilinx.sv:1007–1008,1049–1050` 的 converter 後段。本 intent 到所選門不需要 synchronizer 或 async FIFO。門若改設到 MIG/UI clock，就不再是本最小成本版本。

## 2. start、bank 與資格的精確來源

在 PTW 內新增**組合事件輸出**，使用原來真正接受 start 的相同條件，而非另造 busy 上升沿：

```text
normal_s1_start = (state_q == IDLE)
               && init_ptw_i && !edge_trigger_q
               && en_1S_i && !en_2S_i && !imp_init_i;
```

依據是 `I/rtl/translation_logic/rv_iommu_ptw.sv:315–365`；當拍 ARVALID 的預設仍 0（`:275–285`），下一狀態才 `MEM_ACCESS`。`init_ptw_i` 的 level 不等於 occurrence，`edge_trigger_q` 的置／清見 `:150–157`。不需要新增 PTW occurrence counter。

在 **TL wrapper** 與同拍鎖定 `req_data` 配對：要求 `req_valid && !req_data.is_debug && req_data.ttype == UNTRANSLATED_W`，並排除 implicit/DDT/PDT/MSI。原正常讀寫分類可核 `rv_iommu_tl_wrap.sv:697–711`，但本件只允許目的 write，不能把原 E3 的 normal R/W 範圍直接照搬。TL 的 `ptw_init` 是 `:373–375`；完整 request 已在非旁路 spill 保持（`:200–214`）；方向是 `:414–419`；stage/IOVA/context 輸入是 `:769–803`。PTW 目前沒有 bank 輸入，故 bank 應由 TL 把事件和 `req_data.bank` 配成一束。

bank 的原始鏈不是 guessed DID：`I/rtl/transaction_controller/rv_iommu_banked_ctl.sv:235–244` 以不可變原 AXI ID 的 `BankIdBit` 分 bank，`:366–376` 只覆寫請求 `.bank`。TC 把 AW 設成 W 類別，AR 設成 R/RX（`rv_iommu_trans_ctl_wrap.sv:625–662`）；完成 AR 保留 `ar_data.id`（`:346–357`）。整合中四個 DMA mux 的原 ID 寬度為 `DmaAxiIdWidth = 4−clog2(4)=2`，bank bit 因而為 2（`ariane_peripherals_xilinx.sv:870–872,1228–1252,1409`）。

**資格輸入採 root 的靜態窄合同：** quiet 時確認只有 DMA0/1 的兩個正常 4096B COPY；owner 確認實際物理範圍沒有 write→read 或 write→write 依賴；輸入資料和 mappings 在 pair 生命週期保持；對應 pair-qualified/epoch 的 arm、失效與 retire 需要付費。事件必須是該 pair 的 V 目的 normal-S1 write，門只能暫緩該 pair 的 **exact A payload read**。V 自身 R/W/DS、其他 stream 與已接受工作均不得因這張票據被 gate。bank 不同只供路由比對，不能證明應用獨立。

**另有不可省的 P1 目標資格：** quiet arm 的 owner 必須把 V 實際選用的 normal-S1 context/root 綁定到本 profile：root table 的整個 4KiB 都由共享 xbar 解碼到 DRAM，根及路由保持穩定。PTW `:360–365` 的實際 P1 地址是 `{iosatp_ppn_i[PPNW−1:0], iova_i[VLEN39−1:30], 3'b0}`，`PPNW=PLEN−12`（`:87–88`），且 `I/packages/rv_iommu_pkg.sv:73` 給 `VLEN39=39`。因此 P1 位於當前 root 頁內，頂層 index 是9bit、8B一項，偏移0…4088；P1 所佔最後 byte 至多 root+4095。此資格只用既有 root 與頂層 IOVA index，不讀未來 PTE 內容。

實際 map 是 `ariane_xilinx.sv:276–288` 的 `{idx:DRAM,start_addr:DRAMBase,end_addr:DRAMBase+DRAMLength}`；`ariane_soc_pkg.sv:64,81` 給 `[0x80000000,0xc0000000)`，半開區間由實際 `vendor/pulp-platform/common_cells/src/addr_decode.sv:47–48,90–96` 確認。設 `root_base=zero_extend(iosatp_ppn_i[PPNW−1:0])<<12`，要求：

```text
DRAMBase <= root_base;
root_base + 4096 <= DRAMBase + DRAMLength; // 用擴位算術，不能溢位後比較
root/context/route are stable for the armed pair.
```

本 profile 因 root 自然4KiB對齊，可等價寫成 `0x80000 <= root_ppn <= 0xbffff`。owner 可在 quiet 配置時核對並把結果合入 **付費的 pair-qualified**，但必須確保日後實際 context 就是該 root，不能僅憑任意先前 software page-table base 放行；若此綁定不能由靜態合同保證，start 端須增加實際 root 的 range／綁定比較並核帳。root 不在此 DRAM 頁範圍、context/route 改變或無法確認者，跳過該 start，原 PTW 與 current-VALID 路徑照行。只保證 P1 的目標即可，P2 及後層地址可不同，因 P1 在允許 P2 出生前已消費票據。

V source 已完整、V 持 W owner 是舊數值收益適用的性能條件。本設計不把等到 `source_done` 當生成 intent 的硬性前提，否則會直接消掉有用早窗。此處不發明通用依賴分析器、不預設加 R beat counter。已有 source-complete/owner 證據可由 root 另核，但不能把它們當現在免費到門的信號。

## 3. endpoint 偏序與最早可見時序

定義 `e0` 是 PTW 真正從 IDLE 啟動的上升邊緣，`Ck=[ek,e(k+1))`。以下是**空管線、無背壓、DS FixedDelayInput=0** 的源碼最短偏序。去掉延遲只是為推導保守最短路徑，沒有改配置或執行模型。DS 內部仲裁／下游背壓只能延後這條路徑。

| 事件 | 最早時刻 | 具體端點與依據 |
| --- | --- | --- |
| S：真實 start 被接受 | e0 | PTW `:325–365`；PTE pointer/state 在該邊緣鎖入 |
| PTW-local first AR VALID | C0 | MEM_ACCESS `:384–389`；不在 start 當前 IDLE 的前半週期提前給 AR |
| A_local = DS spill input accept | e1 | PTW `.ar_valid && .ar_ready`，DS arbiter 選中 PTW 並由 `ar_valid && ar_ready` 收進 DS AR spill；DS `:81–109`。E3 adapter AR 直通，`rv_iommu_e3_delay.sv:20–29` |
| DS spill output first VALID | C1 | DS 非旁路，`ds_req_o.ar_valid`；不能把 local accept 當這個 output 或共享接受。spill 實現 `spill_register_flushable.sv:36–95` |
| DS output / xbar ingress spill input accept | e2 | 取 delayer bypass；xbar demux AR 與 select 成對 spill，`axi_demux.sv:132–160` |
| ingress spill output / demux route available | C2 | `CUT_ALL_PORTS` 的 DemuxAr 位有效，`axi_pkg.sv:450–475`；demux 同 ID 跨目標限制只會增加等待 |
| 1×xbar multicut input accept | e3 | `PipelineStages=1`，top `:296–298`；`axi_xbar_unmuxed.sv:231–250`，`axi_multicut.sv:43–79`；每個 cut 的 AR 是非旁路 spill，`axi_cut.sv:86–97` |
| F_P1：共享 selector 第一次 VALID | C3 | `gen_mst_port_mux[DRAM].i_axi_mux.gen_mux.i_ar_arbiter` 的 `ar_valid/mst_ar_chan`；`axi_mux.sv:333–350`。此後若 !ready，已形成 held 義務 |
| A_sel：共享 output spill **輸入** accept | e4 | 同 mux `ar_valid && ar_ready`；`:352–364`。**在這個握手消費票據** |
| output spill **輸出** first VALID | C4 | `mst_req_o.ar_valid`，之後才到 `master[DRAM]`；不是 A_sel |
| DRAM/atomics/converter slave accept | 未給固定拍數 | `ariane_xilinx.sv:934–945,1006–1047`；前方可能已有 payload／atomic 義務 |
| MIG accept | 不同 clock 的獨立事件 | `ariane_xilinx.sv:1049–1083,1135–1168`；不拿它回頭撤銷 shared selector 次序 |

source 中固定 DS input/output=50 的 artificial delayer 仍在（`ariane_xilinx.sv:165–166`、peripherals `:1361–1380`）。`stream_delay.sv:31–35,64–93,113–132` 在 delay=0 直通，正延遲會延後 forward；本關聯證明完全不依賴 DS50。沒有把正向50、回程50、抽象模型 d=2、MIG 實際服務時間互換。此輪不為 delay50 的 counter 做額外時間實驗。

### f 的精確意義與兩個及時條件

f 是從 pre-e0 的 `normal_s1_start` 組合事件到門生效，**包含門本地捕獲**的同步寄存級數，沒有 FIFO 排隊或 ready stall。f=1 在 C0 生效；f=2 在 C1；一般為 C(f−1)。

- **身份安全：** f≤3 嚴格早於 P1 最早 F_P1=C3；f=4 可與 P1 同 C3，必須先用當拍已捕獲 pending 參與選擇，仍早於 A_sel=e4，但只有 current-VALID 式共現，不保證早意圖能力。建議可接受主合同只許 f≤3，主版 f=1、可選 f=2。f≥5 無此證明，禁止再盲認「下一筆 PTW ID」。
- **對 L 有效：** `t_gate_effective <= t_L_first_new_VALID`，且該 L 在此前不是 held、也未進 output spill。f=1 可影響 C0 才首次可選的 L；f=2 對 C0 的 L 已經遲到。L 早於本次門生效時，只能保留原承諾；沒有撤回、回溯或 deferred re-arm。

即使 f=1、P1 關聯一定正確，也不能從這張表推出某個 native 任務真的提供 start→L 窗口。f=1 把較長來源至門路徑放在一個週期；f=2 改善可能的物理時序路徑卻縮窄性能窗口。源碼可界定邏輯拍界，不能證明任何一版 timing closure 或 h_e=0。改減 DS/ingress/multicut 任何一級都要重算合同，不能繼承 f≤3。

## 4. fullID、pending 與為何不會把 P2 認成 P1

PTW 的 local ID 固定 0，len=0、size=8B（`rv_iommu_ptw.sv:275–280`）；DS R demux 的 0 唯一回 PTW，其他 walker 使用 1/2/3/4/5（DS `:235–245`）。AXI mux prepend 真正來源 port（`axi_mux.sv:135–181`、`axi_id_prepend.sv:80–95`）。目前 top `IOMMU_MEM=3`、輸入 ID4、輸出 ID6（`ariane_soc_pkg.sv:20–32`），因此在 DRAM mux output：

```text
PTW identity = {2'b11, 4'b0000} = 6'd48.
payload source = fullID[5:4] == IOMMU_COMP(2).
exact A additionally matches the qualified DMA0/1 original source prefix,
not just arbitrary source 2 or a different bank.
```

比較符號 `{IOMMU_MEM,PTW_ID0}` 最穩妥，48 只對本位寬有效。完整 ID 是既有欄位；不新增 6-bit ticket ID 寄存、不修改任何 ARID/RID。

`exact A` 在這個比對裡識別的是 pair 中 A 的 DMA 來源，**不是**其 4096B parent 裡「最後那個 AR」的獨立身份。M 只改未承諾的新 A AR eligibility，已 first-offer/held 或已進 spill 的 L 永遠無效於本次 gate，不能撤回。舊模型只剩最後 L 的切面可直接使用這個來源比對；若還有後續 A AR，仍要另核其性能影響。若要求「L 已 offer 後整張 intent 立刻失效，且絕不能影響後續 A AR」，就需要來源綁定的 target-L/window 欄位或 offered receipt/sticky 及其生命周期費用，不能由 fullID 免費推出；此項由 root 的來源窗口合同明示，不把末段判別藏進本表的 bank 位。

安全鏈有五項，不是單一 single-walker 口號：

1. 新 normal walk 必須先進 IDLE；上一正常 walk 每筆 AR 被本地接受後，PTW 停在 PROC_PTE，只有收到該筆 R 才能下一 MEM_ACCESS 或完成（PTW `:384–414,478–510,618–657`）。所以 fresh start 時沒有上一 walk 尚未返回的同 ID PTE。
2. quiet arm 確認當前 normal-S1 root table 整頁位於該 DRAM decode 目標且 context/root/route 穩定；所以 P1 的地址必到本門，而不是在另一個合法目標完成後讓 P2 成為本門第一筆 PTW ID0。這是具體來源資格及其費用，不由 single walker 或 active 推出。
3. 控制 f≤3 使此 start 的 pending 一定在當次 P1 shared firstVALID 前存在。中途沒有能讓 control 任意落後 DS 的排隊通道。
4. 第一筆相符 fullID 的 **A_sel** 因而確實是 P1 並消費 pending。P2 只有在 P1 穿過下游並回到 PTW R 消費後才可能出生，因而 P2 不可能比該消費先發生。
5. H 到期只停 gate，仍保留 pending 身份，直到該 P1 A_sel 或真正 walk retire。不能在 H 到期清身份，再接受一個延遲的舊 start 或把後續 PTE 續租成 P1。

若提出「允許 start 任意遲到，仍只用 next ID0」，則相鄰反例是 `P1 A_sel → P1 R → P2 ready → late start`，下一筆 ID0 正是 P2。此版否決；timeout 不修復該誤認。

另一個獨立反例是 `P1→非DRAM合法nonleaf → P1 R被PTW處理 → P2→DRAM`：控制再早、PTW再串行，若不限制 P1 目標，本門看到的第一筆 ID0 仍會是 P2。active 此時一直為1，不會救回身份。本 v2 以 root 整頁的 DRAM 目標資格排除它，非以 timeout/active 掩蓋。

## 5. 具體狀態與退出協議

### 最小門狀態

門保存 `pending_q`、`remaining_q[W_H−1:0]`、`V_bank_q`、`pair_epoch_q[E−1:0]`、`escape_q`。`W_H=ceil(log2(H+1))`、H 是編譯期有界常數。`pending && remaining>0` 就是 gate_active，**不另加 lease-active bit**。三種語義狀態為：

| 狀態 | 編碼條件 | 動作 |
| --- | --- | --- |
| IDLE | !pending | 普通共享選擇；只有 fresh 合格 start 可配置一次 |
| LEASE | pending && remaining>0 | 尚未 held 的 exact A 新 AR 可抑制；每拍遞減，不續租 |
| PASSIVE | pending && remaining==0 | 不再抑制任何新 AR；保留這次 P1 身份直到結清 |

`escape_q` 不是 ticket 身份：每次 ticket 結束或 timeout 後，要求至少一筆普通 non-DS AR 在相同 output-spill input 被接受，才能讓**以後新的 start**再 arm。沒有 eligible non-DS 時普通 DS 可繼續通行；不為等 escape 而阻 DS，不把 miss 保留起來在 escape 清掉後再補 arm。若無任何 non-DS，票據優化保持停用而原邏輯照行。

### 導出 PTW active 以結清未到該目標的 P1

沿與 start 配對的控制通路導出既有 `active_o=(state_q!=IDLE)`（PTW `:99–100`），不新增 PTW busy 狀態。**f=1 時 active 是從既有 state 組合產生、直接送進門 next-state 判斷的線路，門 record 不含 active flop；f=2 只在額外來源級寄存 active，該1bit已包含在 `3+E` 的來源 bundle 費用。** start 當拍源 `active=0`，故處理順序明確：先完成舊紀錄結清，再由 fresh start 配置新紀錄；**不能讓同拍 idle 清掉剛分配的 pending**。start 後下一個 bundle 來自 MEM_ACCESS、active=1，不會誤關。

active 只負責真正完成／fault／missing-target 最後導致 retire 的退出；讀解碼錯／page fault／AXI error 的處理保留原路徑（PTW `:395–414,618–657`）。若原 PTW 接到真正 R/error、經 ERROR 回到 IDLE，`active=0` 才能清未消費 pending，H 只先行放 gate。**不能由此聲稱一般非 DRAM P1 都安全：成功 nonleaf 會讓 PTW 繼續，active 不下降，P2 仍可能到 DRAM；它必須由前述 root 目標資格在 start 時拒絕。** 符合本合同的正常 P1 必到 DRAM，若有真實 fault/retire 的防禦性關閉才使用 active。永久沒有回應則 pending 可長留 PASSIVE，但新 gate 已停；不能從 AXI 永不回應假設推出整個系統活性。

### 每個時鐘邊緣的規則

1. `p1_commit = pending && ar_valid && ar_ready && selected_fullID == PTW_identity`；這是 A_sel。此事件把 pending/remaining 清零，設定 escape。即使 H 同拍到期，也只消費一次。
2. `walk_retired = pending_q && !forwarded_ptw_active`，以邊緣前的舊 pending 判斷，清舊 pending/remaining 並設定 escape。若舊 pending=1、retire 與另一 fresh start 同拍，結清舊紀錄並跳過新 walk，不把新 start 保留重放；若舊 pending=0，start 當拍 active=0 不會清剛分配的新紀錄。若先前 P1 已消費，此項自然無效。
3. LEASE 每拍把 remaining 減1，包含共享門正被舊 held 工作佔用的拍數。變0只進 PASSIVE 並設定 escape；絕不重新載入 H，不由 `ptw_init` 的高電平續租。
4. 合格 start 僅在邊緣前 `!pending_q && !escape_q`、當前 pair-qualified 與相同 epoch 有效時配置 `{pending=1,remaining=H,V_bank,pair_epoch}`。一拍 occurrence 只配置一次；忙時、escape 時或資格不符直接跳過本 walk，不能排隊之後重放。
5. pair 失效／epoch 不符／軟 disable 時 remaining 立即清0、停止新 gate；pending 仍按 1/2 結清。它們不清 arbiter lock 或 AXI FIFO。
6. `nonDS_commit = ar_valid && ar_ready && selected_source != IOMMU_MEM` 可清 escape；同邊緣非 DS 接受可滿足該次 exit 的逃逸。為避免依賴邊緣次序，new start 不利用「本拍才清 escape」補 arm，等待下一個真正新 start。

arm／更換 pair epoch 還要求前向控制 stage 無 start valid、pending=0、原 pair 工作已按其 quiet/retire 合同結清；不能先改 epoch，再把舊 pipeline 中的 start 解讀成新 pair。stage 的 `{start,active,bank,epoch}` 同步前進，每拍捕獲、無 backpressure，reset 清 start valid；不能把其不同欄位作獨立 CDC 或不同拍延遲。

以上是完整設計規則，未執行狀態模型／形式證明。H 最大抑制的是新 arbitration eligibility 的 H 拍，不保證 H 拍內 P1/R/整個 COPY 完成。未到門的 P1 不得延長 lease。已經 held 的 P1 在 H 後仍必須保持，這是原 AXI 義務而非新 lease。

## 6. held／READY：不能直接在既有 mux 外做 VALID AND

既有 `axi_mux.sv:333–350` 的 `rr_arb_tree` 設 `AxiVldRdy=1,LockIn=1`。其 lock 記住輸出首次 VALID 而未 ready 的請求組，`rr_arb_tree.sv:151–197`；raw VALID 一旦提供就要保留，不能在下一拍 intent 到來時撤回。output spill 中已接受的所有 AR 更不能改。

低狀態接入是只為這個 DRAM AR arbiter 增加 **unlocked eligibility**：

```text
when !lock_q:
    req_d = raw_req_i & allow_i;
when lock_q:
    req_d = req_q;            // 原 held winner / request set 一律保留

allow_i removes only exact_A while gate_active and pair still valid;
raw data_i is unchanged;
upstream_ready[i] = tree_grant[i] && req_d[i];
```

這可以在專用 AR arbiter wrapper 內重用既有 `lock_q/req_q`，或在通用 tree 增加 default-all-ones mask 接口、只於 unlocked 分支採用。需要一個明確實作位置，不能靠外部未宣告階層參照讀內部 lock。非選中、masked 的 raw source 必須收到 READY=0。**只屏蔽 VALID 卻把 tree READY 原樣回送是錯的：** AxiVldRdy 模式的 grant 未必依賴該葉 req（tree `:274–282`），被 mask 的 raw VALID 可能被 upstream 視為已接受而丟失。上述 `&& req_d[i]` 同時關閉該假握手。這不新增 payload buffer，也不需要複製既有 held state。

原 tree 保持的 raw source 一直由上游 xbar cut 持有；H、pair disable 或新 start 不得改 held winner。若實作選擇不改 tree、另做 held_q 跟蹤，須額外計1bit及對應等價證明，不能同時說是零狀態再用一個隱藏 held register。

當前 production 這裡是 round-robin，不是舊模型宣告的 DS-first/F4。M 的 first-PTE gate 可疊於一個兩側共享的 current-VALID DS-first/F4 policy，但不能把該 policy 當現在已部署、免費的硬體。O_SAME_TICKET 同樣得到本接口；本件不主張新增獨占排程算法。

## 7. 線路、記錄與寄存增量

| 項目 | M-ID f=1 主版 | f=2 選項／限制 |
| --- | --- | --- |
| PTW 事件 | 新 normal_s1_start 組合輸出1bit；現 active 續導1bit | 相同 |
| TL 配對 | 新 qualified start + bank + pair epoch；normal write/stage 是入口 predicate，無需單獨長期存 store/stage | 將 `{start,active,bank,epoch}` 以同一來源級捕獲，不可分開 pipeline |
| 實際穿越的 wrapper | PTW → TL → top（base/ext generate 皆接）→ ariane_peripherals → ariane_xilinx → axi_xbar_intf → axi_xbar → DRAM axi_mux/專用 AR gate | 相同；只有指定的1級 source capture 多一拍 |
| 既有 DS 請求/回應通路 | 不擴 AR/R struct、不穿 marker、不改 ARID/RID/RLAST，不為 bank 再開 DS FIFO | 相同 |
| 門 record | pending 1 + remaining W_H + bank 1 + pair epoch E + escape 1 = **W_H+3+E 個有角色的邏輯儲存 bit**；沒有 active flop，active直接參與 next-state | receiver 相同；另加來源級 `{start,active,bank,epoch}` **3+E bit**，總 **W_H+6+2E**；active的1bit拷貝已計 |
| held | 復用原 lock_q/req_q，新增 mask邏輯與 READY qualification；沒有新增 held record | 相同 |
| PTW/P1 issue／seen／ack | 不需要：由 f≤3 的實際偏序保證 identity；gate 消費本地可見 A_sel | 不需要；同一界仍成立 |
| pair-qualified／arm／epoch | 非免費。root 的 shared pair 配置與生命週期記錄另外核帳；表中 E 只計此接口管線與門內保存的拷貝 | f=2 的 epoch/active/start 原子配對需多拷貝 |
| P1 必到 DRAM 的資格 | quiet owner確認實際root整4KiB在profile DRAM窗口、context/root/route穩定，合入既有付費pair-qualified；包含配置核對及持續綁定責任，不能視為免費 | 若不能靜態保證實際root綁定，新增start端root range／expected-root比較；根值在TL已存在，但新比較/配置/必要拷貝需另計，不能隱去 |
| 本地固定 pair 測試 | 可用固定 DMA0/1／bank與外部已確認條件，省去動態可選pair record；此為受限測試配置 | 不能把這個省略當一般部署成本 |
| 運行部署 | quiet arm、fresh pair epoch、選中A/V、物理獨立性及輸入/mapping保持生命週期都是共同前提且有產生/傳遞成本 | 非本件新增通用證書系統；root 分項列實付成本 |
| 資料容量與 AXI 數量 | 沒有新 payload RAM；兩COPY原4個payload AR/4AW/4B；cold PTE數不減 | 相同；不意味總cell更少 |
| 物理代價 | start/active/bank/epoch 長線、fanout、比較、mask、timer 的電路/布線及可能 critical path | 多一來源級但性能窗縮短；未綜合不報實際 cell、FF、面積、功耗或 Fmax |

以上 bit 是 RTL 設計角色核帳，不是綜合 cell 數、淨 FF 差或全系統成本。完整 ID、TL request bank/ttype、原 AR 所有欄位、原 spill／lock／payload FIFO 已存在，只計新拷貝及邏輯，沒有把既有寬欄位一概算新增面積。H、E 的例值未被選成新性能參數。

## 8. 哪些改動會要求 ack／marker／epoch

**本主版需要來源 qualified pair epoch，毋須另外造每 walk occurrence epoch。** 同一 PTW 只發一個原子 start event，固定 f≤3 到門，上一 walk 沒有未回 PTE，P1 必由穩定root整頁DRAM資格到所選門，且全傳輸 common reset；這幾項一起取代 occurrence tag。pair epoch 的用途是部署 arm／失效來源，不可冒充它能消除任意長延遲訊息或錯誤P1目標。

若物理時序要求把 control 變成 f≥5 或帶 queue，而仍要保留 ID-only，最小可審替代是 **start-receipt fence**：gate 收到一次 start 後回 receipt，PTW 的首 AR 在首次 ASSERT VALID 以前必須等 receipt；若 gate 不 arm，也回 skip receipt，不能留一個可在 P1/P2 後才生效的舊事件。receipt 是「start 已被門分類」，不是 P1 commit。來源需至少 first-launch-pending/permit 的記錄、正反向控制線及寄存；若 round trip 超過現有一拍 start→MEM_ACCESS 間距，首 PTE 真實增加等待，必須算入 h_e/窗口。H 到期仍只放 gate，不得偷偷發 P1後保留晚到start；ack 通道需有界送達，不能以 AXI timeout 無條件逃過身份義務。這不是本輪主版的零額外 round-trip 成本。

若又拒絕 fence，就改成真正 M-tag，至少把 first-PTE occurrence/marker 和 AR **原子保持**，穿 DS arbiter/spill、delayer、ingress/cut 到共享門，並在 late intent 到來前記住該 occurrence 的 seen/consumed；不能只加一根瞬時 first 信號，也不能只用既有 ID0。其 tag/epoch回收需和未完成回應及控制訊息退休相連，單bit任意回捲並不安全。這版路徑更寬、狀態更多，本輪不為不需要的泛化範圍推薦它；也不捏造其實際面積。

正常 disable、observer arm、H 到期、policy clear 都不能 flush 原 FIFO 或 reset identity。**只有從 PTW 到 xbar/atomics/converter/MIG 同一 transport reset/已 quiescent drain 的合同，才可同時清 intent stage/pending/escape/原 held 義務。** `ndmreset_n` 確實接到這些 RTL 端點，不代表本輪已驗證 MIG reset後所有舊 response 的硬體行為。若允許單獨 reset PTW 而其他 AR/R 還活著，本 ID-only 版本不成立，必須禁止這種操作至 drain完成，或升成真正可隔離舊義務的 reset/epoch/tag 協議。加一bit epoch 無法令早已送出的 untagged AXI R 自動安全。

## 9. 已可界定、未知與相鄰失敗

| 分類 | 本輪可給的結論 |
| --- | --- |
| 來源已確定 | 真實 start條件、同時鐘/共同reset接線、AR ID與bank路由、P1最短3級前向存儲、held/READY義務、A_sel消費點、PTW單筆AR→R串行 |
| 設計條件推導 | f≤3 加穩定root整頁DRAM目標資格的無late-P1 M-ID；f1/f2性能窗不同；H只關gate，身份至P1 commit或真正walk retire；exactA獨立COPY範圍與escape可避免新gate造成依賴環 |
| 原生未證 | 真實 t_gate≤L firstVALID 的共現頻率、DS50/其他排隊下適當H、V sourcefull/owner相位、MIG服務收益、布線時序、h_e、cell/Fmax、完整pair資格部署成本 |
| 性能相鄰失敗 | f2或更晚錯過C0 L；L已held/已進spill；H先到P1後到；後層處理gap短；普通低費分段或legal prepare達成相同/更好完成 |
| 正確性相鄰失敗 | 只比較bank而未證獨立；誤擋V sourceR；把source2任意DMA當A；非DRAM root的P1成功nonleaf後P2才到DRAM（必須skip）；root/context/route在arm後改變；H清pending後接收舊start；f≥5 blindnextID；maskVALID卻不maskREADY；對已held輸出撤選；局部reset丟失舊ID義務 |

本輪可接受的進展是 **一個可實作、來源綁定的窄 M-ID 接口設計及其非零線路/狀態成本**，不再只有「未來要做關聯」的清單。普通 O 可得到相同資格與新增接口；與現有 legalizer reuse 的完整成本與服務比較由 root 合併。舊 2060 或 `a_split>2+h_e` 只能留在原條件模型域，不能把 f=2、新原生d或未完成資格接線代進去仍聲稱已取得該數值。
