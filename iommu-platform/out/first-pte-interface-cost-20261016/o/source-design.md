# O：復用 legalizer 的完成驅動 read128 分段

2026-10-16；FIRST-PTE-INTERFACE-COST-1/O。ARS-Codex 有界原始碼核查與來源綁定設計；Codex-only。建議／請求 Sol/xhigh，actual 獨立 model/effort metadata unknown。本席只讀源碼並寫本 O 目錄，沒有改生產 RTL、執行模型、重跑 70 條、模擬、綜合、建置、板測或 Git 變更操作。

**結論：普通完成驅動分段可以大量復用現有 legalizer、metadata FIFO、AR holding 和獨立 W 計數；並不需要重寫 splitter 或新增 payload buffer。現行軟體配置尚不能直接啟用它，現行 frontend 單 job 在途也不天然提供 read pacing。最低狀態的 O-local 將准入門放在 legalizer descriptor 接受點，用已有 read metadata FIFO 的 nonempty 保證 L1 在 L0 真正 RLAST 後才產生；它另有一次 source TL 請求與 release→shared-firstVALID 的跨層延遲，尚不能直接對應舊模型的零費用 2058。** 補強後的強 O-prepared 可復用 A bank TC 的現有 FIFO head、翻譯結果及 COMPLETE 狀態持有 L1，使用固定地址／ID辨識及一個 sticky 完成 bit／回線；無需新增 header slot／通用 tag FIFO。額外 lookup 的準備時機、TC 佔用與回線時序仍要計費。

所有 C 編號對應 [evidence.json](evidence.json) 的原文片段、工作樹 SHA256、component HEAD。本文簡稱路徑相對 `iommu-platform/cva6/`，`iDMA/` 表示 `corev_apu/iDMA/`。實際 component HEAD=`a1b08934391833aed21e6ad63b545634040808ac`，與 sources.lock 一致；iDMA 是 CVA6 跟蹤目錄，沒有獨立 HEAD。frontend、wrapper、FIFO 和 FPGA integration 有既存 diff／新檔；本報告採目前 bytes，不用 clean HEAD 冒充當前源碼。

## 1. 真正呼叫鏈與完成端點

1. `iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:325–373` 生成原 4096 parent，固定 AXI ID、來源／目的屬性及 backend options。`src_max_llen=dst_max_llen=0`，兩側 reduce 共用 deburst；`CONF.decouple` 只給 `decouple_rw`。generated CONF `:28–47` 無 source128/pacing 欄位。這是配置缺口，不是 backend 不會分段。[C1,C18]
2. `iDMA/src/systems/cva6_reg/dma_core_wrap.sv:173–203` 在 frontend job 接受時取 fullbuffer 證書，job FIFO 保存整個原請求；`:306–346` 呼叫 backend。原 4096、原 src/dst 與單 parent 不變。[C5]
3. `iDMA/src/idma_backend.sv:326–354` 呼叫 `idma_legalizer`。legalizer `:139–163` 算 source 虛擬頁／上限；`:201–214` 決定 R/W 是否共用最短邊界；`:235–249` 用已有 `r_tf_q.{length,addr,valid}` 推進 source；`:285–316` 在 job 開始保存 options。64-bit 路徑 `src_reduce_len=1, src_max_llen=7` 給每 1024B／128beat 分界。[C2,C17]
4. backend `:471–473` 令 read ready 同時需要 metadata FIFO 及 AR fall-through 可收。`:554–570` 將一份 r_dp metadata 放入原 Depth8 FIFO；`:610–625` 同次把 AR 放入原 fall-through。**這是 descriptor 接受 D，不等於外部 AR offer 或 AR handshake A。** FIFO entry 可以涵蓋尚在 AR holding／TC／translation／回程中的工作。[C3,C12]
5. `iDMA/src/idma_axi_transport_layer.sv:199–202` 將 holding 的 AR 接到 DMA 外部；`:257–270` 只有 buffer 可收且 response consumer ready 時接受 R，真正最後一拍才 pop metadata。令 `Rlast_fire = protocol_rsp_i.r_valid && protocol_req_o.r_ready && protocol_rsp_i.r.last`。在合法有 metadata 的路徑，`r_dp_req_out_valid && r_dp_req_out_ready` 與本 burst 的 `Rlast_fire` 等價。[C4]
6. `r_dp_rsp_valid` **單獨不夠**：transport `:281–282` 在中途 R error 也會將它拉高。`r_dp_rsp_valid && r_dp_rsp_ready && r_dp_rsp.last` 才是已接受最後 R；未握手的 RLAST 或非 last error 都不釋放 pacing。[C4]
7. COPY 的 parent completion 仍是最後 B：backend `:524–526`，last FIFO `:631–649`；transport `:447–455` 逐 B 回報。wrapper `:206–207` 把 parent rsp.valid 接 frontend，frontend `:519–570` 先發一 job、等該回應才再發。**等這個 trans_complete 不能推動 L1**：它等寫端／W owner／B，而 L1 是同 parent 所需資料，會有循環或失去原 S 狀態的讀進展。[C1,C5,C6,C15]

端點偏序，等號只限同一同步邊界：

```
D_L0 → AR_firstVALID_L0 → AR_accept_L0 → accepted R beats → Rlast_fire_L0
Rlast_fire_L0 ≤ D_L1 → AR_firstVALID_L1 → AR_accept_L1
each W beat needs its corresponding source bytes (AW may be earlier)
all source data of each parent → its final WLAST → its final B → that parent retirement
```

此處第二行的 AR 是 DMA 本地 AR；通過 IOMMU 後還有不同的 physical/shared firstVALID／accept，不能混同。L0 以前已發出的同 ID prefix 仍要完整返回。

## 2. 可復用什麼、不能把什麼當 credit

| 能力／資源 | 分類 | 證據與界線 |
|---|---|---|
| source/destination 各自 max_llen/reduce、地址／剩餘長度 | 現有直接具備 | legalizer :139–214、235–249、301–308；不另買地址／分段長度計數器 [C2] |
| read128 / write256 | 有限接線／配置 | 獨立 source cap 已在 options；配置 `decouple_rw=1` 且目的 cap 不縮短。不是原 CONF 的現成功能 [C1,C2] |
| 一個 descriptor 的 metadata 與 AR 持有 | 現有直接具備 | Depth8 read metadata FIFO、原 AR fall-through；payload 仍流入原兩個512beat buffer [C3,C7,C12] |
| read descriptor 在途數 | 現有資源可復用、需有限回接 | FIFO 的 out_valid 是 nonempty；entry 從 D 持有到真正 RLAST，不是外部 AR 計數 [C3,C4,C9] |
| 所有分段一律等前一 RLAST | 新控制、可不新增獨立狀態 | 以下用既有 FIFO nonempty 控制 D，不減 FIFO 深度 [C3,C4] |
| tail 選擇及僅 tail cap | 新控制 | 重用 remaining，新增比較與 source cap mux；需要靜態／job 政策入口 |
| frontend 單在途 parent | 現有直接具備但不可作 R credit | completion 依賴最後 B；不是 segment completion [C1,C5,C6] |
| `NumAxInFlight=8` | 現有容量參數，非現成限速開關 | 不是「有 8 個 RLAST credits」；backend :875 要求 >1，idma_stream_fifo :41–44 亦禁 Depth1 [C7,C11] |
| RAW coupler credit | 本路徑不使用；不能無條件借用 | 生產 RAW_COUPLING_AVAIL=0；coupler :105、133–158 計 first R 對 AW，不是 RLAST。若啟用且 3/4 R 對 2 AW，須重新核它，不能憑空免費沿用 [C6,C7,C13] |
| error aggregation、原生時序／面積 | 尚未具備／未證明 | 見第6、8節；不填0 |

**FIFO 邊界陷阱：** `fifo_v3.sv:59` 的 usage 只輸出 status counter 低3位，Depth8 滿時也是0；用 `!r_dp_req_out_valid` 判 empty，不能用 `usage==0`。Depth2 分支 usage 甚至為 x。`usage==1 && out_valid` 可供組合末 entry 判斷，但不需新增寬 counter。[C9–C11]

## 3. 配置及無重複 descriptor 的共同接線

最低成本普通版可採 **靜態 instance 參數**，quiet 時確定策略：O-tail 只在 A 的 DMA instance 選 TAIL128；O-job 兩個 instance 選 JOB128。配置不是每 child CPU/MMIO 動作；整合／重新生成的開銷仍 unknown。若要同一生產實例 runtime 切換，需新 mode 欄位／保存及 busy-write 防護，不能冒稱現有 CONF 已有。

具體可復用原 request options 傳政策，而不拓寬所有 entries：在 wrapper 的 job FIFO 輸入前，對靜態 policy 啟用且已符合原 fullbuffer eligibility 的 normal aligned4096 job，保留 length/src/dst，設 `src_reduce_len=1, src_max_llen=7, dst_reduce_len=0, decouple_rw=1`。eligibility 可有限接線共用 `idma_fullbuffer_certificate.sv:16–17` 既有組合判斷；原 certificate 仍觀察原 parent。這些 options 已經經 job FIFO、`opt_tf_q` 保存。TAIL128 的 module 參數表示 source cap 僅在 tail 開啟，JOB128 直接用既有整 job source cap。其他 job 保留原配置；原 deburst 的 max_llen=0 不與受限 source7 編碼混淆。[C1,C2,C5,C8,C17]

原有 read descriptor 為 ready-coupled 輸出：legalizer `:457` 的 `r_valid_o = r_tf_q.valid & r_ready_i & !flush_i`。因此共同接線是：

```
base_ready = r_dp_req_in_ready && ar_ready;
leg_r_ready = base_ready && admit;
legalizer.r_ready_i = leg_r_ready;
r_valid = legalizer.r_valid_o;
r_meta_FIFO.valid_i = r_valid;
AR_fallthrough.valid_i = r_valid;
D = r_valid && leg_r_ready;
```

不是只 mask 後端一側 READY、卻讓原 VALID 反覆被另一 FIFO 收走。這一現有 ready-coupled 行為使兩個目的同步接受；accepted descriptor 之後，其外部 AR firstVALID／holding 全由原 fall-through 管，**不能再用新的 admit 去撤它**。如果未來改 legalizer 為獨立 held VALID，須另加成對隔離／held slot；上述零新增 held bit 的理由只適用已核源碼。[C2,C3,C12]

## 4. O-tail-local：只拆 A 的最後256

範圍：A 原4096 source 為256 +（128+128）；V 仍256+256，write 都256+256。只在未產生所選末段前選定策略，不對已 firstVALID 的256改長度。

**動態 cap 的實際改點**是 legalizer `:139–148`，不是單在 frontend 填 src cap：受限 TAIL128 job 若 `r_tf_q.length > 2048`，有效 source log length 仍8；若 `0 < remaining <= 2048`，才用已保存的7。其餘 source address offset、r_num_bytes_to_pb、min(remaining,boundary)、metadata 與地址加法均照既有路徑。固定4KiB aligned4096因此依次為 remaining4096→2048→1024，最後一次只清 valid，**既有 RTL 不把最後 length 清0**，故所有 tail 判斷要含 `r_tf_q.valid`。[C2]

可執行 FSM 由既有 remaining/valid 及 FIFO nonempty 編碼，無必要另買 phase counter：

| 狀態／條件 | descriptor 准入 | 下一事件 |
|---|---|---|
| PREFIX：remaining>2048 | 原 base_ready；source cap256 | D_prefix 後 remaining2048 |
| FIRST：remaining=2048 | 原 base_ready；source cap128 | D_L0 同步入 rFIFO 及 AR holding，remaining1024 |
| WAIT_FIRST：valid && remaining=1024 && rFIFO_nonempty | 禁止下一 D；不門控原 AR/R/W/B | 接受所有原 prefix 及 L0；等 rFIFO 真空 |
| SECOND：valid && remaining=1024 && rFIFO_empty | 准入一個128 descriptor | D_L1 後 r_tf.valid 清0，原 read metadata 持有到其RLAST |
| READ_ISSUED／WRITE_DRAIN | 不要求新的 R descriptor；write照舊 | 等所有 R/W/B，原 final B 才退 parent |

因此 `admit = !(qualified_tail && r_tf_q.valid && remaining==1024) || empty_or_final_pop`。WAIT 時 L1 還未進 FIFO，所以 empty 必然表示「更早 prefix 及 L0 全完成」，沒有把其他 entry 的 RLAST 誤當 L0。A 單一固定 ID、frontend 單 parent 給既有有序歸屬；不新增 per-segment return ID／重組 buffer。新控制用 remaining==1024 識別 pending L1，而不是拿總 usage 猜哪個 child。[C1–C4]

第一個128可以與較早 prefix 同時在途，不強迫整個 prefix 一次只一筆；這避免把普通版削弱成全 job pacing。當 L0 descriptor 已在原 AR holding 中但尚未 external accept，rFIFO 已 nonempty；門只擋 L1，不擋住 L0 自己。

## 5. O-job-local：兩個 job 全部 source128

O-job 不是 O-tail 的另一叫法。兩 job 各4個128 read、各2個256 write；固定3個256前綴會變6個128，AR 機會、翻譯請求及時序都改變。

FSM 亦可由原 metadata FIFO 編碼：EMPTY（可 D）→PENDING（禁止新 D，原 AR照常發、原 R照常收）→接受該段 RLAST→EMPTY。在 qualified job 的 `r_tf_q.valid` 期間令 `admit=empty_or_final_pop`；read invalid 時不額外擋空閒／write進展。原 source7 cap 路徑產生128，不加 tail 比較。FIFO 保留原 Depth8，但策略最多一個 source descriptor 未 RLAST；不能把 parameter 改1冒充不用控制。[C2–C4,C7,C11]

此策略沒有等 B、AW owner 或 write drain 才放下一 read。完整512beat buffer足以保存全部 source，而總 source useful bytes仍4096，W長度仍原256；不能把一般 `decouple_rw` 可能死鎖的註解直接外推成此有限 fullbuffer domain 不可實現。[C7,C8,C15,C17]

兩個 local FSM 可各選：

- **註冊邊界版**：`empty_or_final_pop = !r_dp_req_out_valid`。RLAST edge pop 後下一拍才准下一 D；permission 一旦使 D 接受，AR holder保持後續 VALID。沒有新 pacing flip-flop，代價是既有 FIFO empty 註冊邊界和跨層路徑。
- **組合同邊界版**：`empty_or_final_pop = !out_valid || (out_valid && usage==1 && r_dp_req_out_ready)`。在最後 entry 真正 pop 的同步邊界可同時接受下一 D，FIFO count保持1；不是看到 RLASTVALID 即釋放。此版把 buffer-ready/RLAST/response-ready/usage→legalizer→AR 前送放上組合路徑，需要實際時序與無組合環驗收。若合同要求「新 AR 第一次 VALID 的線路發布必須嚴格晚於已完成 RLAST edge」，選註冊版；組合版只給 edge-based 同邊界接受，不能把 edge 前 wire-valid 偷稱嚴格 after。

## 6. 保留 parent、寫端、fullbuffer 和錯誤責任

- 原4096 parent、映射、地址及權限不變；certificate `:16–18` 沒有 burst長度條件，仍覆蓋單 parent直到其完成。兩1024 subCOPY不是本版本，不能破壞4096資格。[C5,C8]
- `decouple_rw=1`使 source128不把目的也縮短；原 W mover自己計num_beats，不能用 child RLAST合成 WLAST。FIFO資料順序把兩128串成原256 W，四個AW、四個B與final parent retirement保持原義務。[C2,C6,C15]
- AR字段仍由legalizer `:326–345`產生，每child保留AXI ID、SID/SSID、cache/prot等，地址/長度由已核合法化路徑產生；額外AR必走正常TC／翻譯，不是新權限。[C2,C3]
- **現行錯誤缺項是共同基線問題，不能藏掉：** wrapper選 NO_ERROR_HANDLING、backend把parent.error強制0。直接改 ERROR_HANDLING也不行：generic handler會在中途error發rsp，而現行wrapper/frontend把任意rsp.valid當parent retire。其num_outst計parent，不是read段；不是可免費借的read完成計數器。[C5,C6,C14]
- 若本成本對照要求正確宣告失敗，最小正常AXI drain方案為：一個parent sticky-error對每個實際Rfire和Bfire累積非OKAY；中途error仍不釋放段，真正RLAST才釋放；維持原4個W/B直到final B，最後rsp帶 `sticky || same-cycle-error`，再由frontend新狀態欄位／錯誤出口發布。這是**設計新增**，至少sticky bit及有限錯誤發布接線，exact status storage/CPU成本unknown；M及原普通COPY都應同享／同付，不能全部算O獨有的「重組成本」。若只保留原生forced0，須明示只驗證OKAY路徑，不能聲稱一般錯誤正確。
- 若reset、missing/malformed RLAST、永久背壓／失敗slave打破原AXI完成假設，以上不是超時／復原證明。本輪無錯誤注入，未知不填0。

## 7. 請求數、IOMMU延遲及強 O-prepared

固定64bit、兩 aligned4096 job（總8192 useful read bytes及8192 write bytes）：

| 版本 | payload AR | cold含原3 PTE AR | hot AR | prefix AR | AW / B | 相對原source TL請求 |
|---|---:|---:|---:|---:|---:|---:|
| 原256 | 4 | 7 | 4 | 3 | 4 / 4 | 0 |
| O-tail：只A末256→2×128 | 5 | 8 | 5 | 3 | 4 / 4 | +1 |
| O-job：兩job所有read128 | 8 | 11 | 8 | 6 | 4 / 4 | +4 |

cold三PTE是凍結舊模型的同一翻譯史條件；新時間線／競爭若改變cache命中／miss，額外PTW不保證仍3。lookup請求增加不等於每次cold walk，更不等於每AR固定多一cycle。[C2,C16]

**O-local 的決定性映射差項：** L1直到DMA端L0 RLAST後才入descriptor／本地AR，之後仍須走DMA→TC→TL lookup→physical AR holding→shared-firstVALID。故即便本地組合准入同邊界，物理L1也未必在舊模型t128已global AR accepted。V walk active時此extra hit在P/PN能否並行服務、是否等待walk、release到shared的管線／backpressure延遲，交root定向TC/TL核查；本席不將它填0或重審整個IOMMU。

### 7.1 有界補強：O-tail-prepared 復用 A bank TC 持有

**此具體普通版本可行，無需新增 header slot 或通用 tag FIFO。** 限 normal aligned4096 單 parent、A 固定 DMA／原 AXI ID、兩 job 的源頁 mapping 在 quiet pair 中穩定；reset／重新配置必須先 quiet。產生 child 時只沿用第4節的 tail cap，**取消 O-local 的 descriptor pacing 門**，讓 L0、L1都可提前排入既有 DMA rmetadata／AR 路徑，並各經正常翻譯。門改在 A bank TC `COMPLETE`、任何 physical AR firstVALID 之前。

新增直接來源 `corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`（下稱 TC），component HEAD=`57eb5621802ef9ed8d77c684b9a03205c6630391`、存在既存 ReadAdmission diff。TC `:141–164` 的 AR FIFO head 在 `ar_ready` 前不 pop，`:108` 和`:698–701`保存當前 translation response，`:610–617`預設保持，`:716–725`的 COMPLETE 等 downstream accept才離開。physical header在`:346–357`由 `ar_data` 與 `trans_resp_data_q.spaddr` 組合。因此，把L1留在 COMPLETE 就是佔用**原** header／PA／state 保存，不是另加一個physical header register。[C19]

**身份：** 在固定 A bank 內，定義

```
is_L1 = prepared_policy && fullbuffer_job_i && is_read && ar_valid
        && ar_data.id == A_original_axi_id
        && ar_data.addr[11:0] == 12'hC00 && ar_data.len == 8'd127
        && ar_data.size == 3 && ar_data.burst == INCR;
```

`ar_data`是原 IOVA header，成功正常頁翻譯保留頁內offset；同一 parent的prefix為offset0/len255，L0為0x800/len127，L1為0xC00/len127。這個辨識在 physical firstVALID 前完成，故不必把新tag經TL傳到physical header。固定 A bank＋原ID＋單parent epoch 排除別的DMA／舊job同型請求；不能把bank號或SID直接當AXI ID。實際 bank wrapper以原AXI ID的BankIdBit選bank，bank內保留原ID寬度並在out mux前後處理bank位（banked_ctl `:15–25,315–347,410–449`）。mapping／parent條件失效時，不能把此地址簽名泛化成任意stream識別。[C19,C20]

**L0完成marker及提前釋放記錄：** legalizer `:403–407` 的 `r_dp_req.decouple_aw`目前只向metadata傳遞；transport不讀此欄，backend唯一實質consumer為`:827`的RAW coupler，而當前production設RAW_COUPLING_AVAIL=0。可在這個受限profile將它重用為 `is_L0 = qualified_tail && r_tf_q.valid && remaining==2048`，prefix、L1都寫0；原write/AW的decouple_aw保持原義。既有8條metadata entry存該bit，不拓寬FIFO。此為新語義／選擇接線，必須以static assertion或配置約束拒絕RAW=1。[C2,C4,C6,C7,C21]

在DMA backend新增**1 bit** `l0_done_q`：backend接受本parent（`req_valid && req_ready`）時清0，reset清0；`r_dp_req_out_valid && r_dp_req_out_ready && r_dp_req_out.decouple_aw`時置1。後一事件是marker head的真DMA RLAST接受，prefix RLAST／mid-error都不觸發。保持直到下一parent開始；frontend既有序列化及quiet epoch保證無舊L0 response跨入新parent。將此sticky level以**1條新1bit跨模組邏輯線**由backend→DMA wrapper→platform→IOMMU→A TC傳入；各層現有port尚無此線，須實際加線，不假稱現成。現行均接同一clk，若為實體時序再加pipeline，額外register和晚到費用另計。[C1,C3,C4,C5,C7,C21]

記錄從parent開始就生效，**不能等L1進入COMPLETE才arm**。所以L0很早完成、L1翻譯很晚，或二者同一edge，都不漏釋放事件。TC讀sticky level，不再新買armed／held／release bit；先存成sticky後才使用的版本在L0 RLAST edge之後可開門，符合嚴格after，不靠未接受的RLASTVALID。

| O-tail-prepared FSM（既有 TC state＋1bit l0_done） | 動作／保存 | 轉移 |
|---|---|---|
| parent開始，done=0 | 原rmetadata填prefix0、L0=1、L1=0；兩child可按原credit提交 | L0 marker真RLAST接受→done=1，不管TC目前state |
| TC IDLE/REQ_TRANS/AWAIT_RESP | 原程式處理L0/L1的正常lookup | 翻譯結果存既有trans_resp_data_q，進COMPLETE |
| TC COMPLETE，is_L1 && !done | `complete_read=0`，`ar_ready=0`，保持COMPLETE／AR FIFO head／PA；尚未physical firstVALID | done level到達後開門 |
| TC COMPLETE，非held read或done=1 | 原read/write發送規則；一旦firstVALID發布，done不會再清直到本parent已完成，故原spill/mux保持穩定 | **實際VALID&&READY**才pop head及回IDLE |
| parent剩餘drain | 原R/W/B及parent error／retirement合同 | 原final B；下一backend parent接受才清done |

**必須修改同一端點的發布與退隊條件：** TC `:718`改為 `complete_read = is_read && !hold_L1`；`:721–725`不能仍用裸 `ar_ready && is_read`，應以 `axi_comp_req.ar_valid && axi_comp_resp.ar_ready`（即complete_read&&ready）判read accept，再令內部ar_ready並回IDLE。否則READY可能預先為1，沒有VALID也pop/drop L1。write accept保持 `complete_write && aw_ready`。門在內部completion demux之前；不准在已被demux/spill接收或已firstVALID之後撤回L1。[C19]

**真實新增與佔用：** 1個l0_done register bit、1條跨模組level net、rmetadata既有bit的L0選擇／抽取接線、固定ID/addr/len/size/burst比較、TC complete_read及read-accept成對門控。新增header／payload／generic tag FIFO storage為0（此有界設計）；已有metadata中的bit、原AR FIFO head、原PA結果和TC state只改用途／延長佔用，不全算新增。runtime policy/status/error出口與可選回線pipeline另計；實際cell/area/Fmax仍unknown。

**TC stall邊界：** hold期間A bank留在COMPLETE，故它不能再選本bank的新AW或AR翻譯；這是實際吞吐／佔用費用，不能說完全不影響寫準備。已接收的W/B／R資料路徑與另一bank並不被此門直接關閉（TC `:359–368`，banked_ctl `:315–358`）；A完整512beat buffer讓L0完成不需要後續A新AW來釋放容量，不能誤等W owner或B才release。[C5,C7,C8,C19,C20]

**剩下的有界限制：** L1多一次lookup仍可能被V的active walk堵住；要在舊模型L0邊界附近已有physical L1可發，需把L1正常translation準備在Vwalk開始以前完成，或實際P/PN能合法並行服務該hit。此版本證明普通方有低狀態的可實作hold方案，未證該準備時機可達，也未把DMA RLAST回到TC／shared的routing與pipeline延遲算0。root另核TC/TL排程；本補強不執行新模型／sim。兩邊若使用相同新增line／靜態資格或error合同，應同享同付，不把可復用TC state全部只記普通方。

後TL splitter仍不是免費別名：它另需child header及第一RLAST抑制／final joining；以上prepared版本已避免要求該重寫。

舊 `runner.py:51–62` 保留3×256 prefix、只拆L；`:156–160` 的a_split是占用**共享server**的serial setup，甚至可以延後已accepted P1 data。它不是本地註冊空拍、extra AR、TL延遲的自動換算單位。O-tail-local、O-prepared與模型split的時序對應需另核；O-job尤其不能引用舊2058。[C16]

## 8. 增量成本與驗收界線

- O-tail-local新增職責：靜態／runtime策略入口；source cap動態比較／mux；remaining1024時的descriptor准入；回用rFIFO nonempty；可選同邊界last-pop旁路。主要已有state為source長度/地址、opt、metadata FIFO occupancy、AR holder、原W計數和parent certificate；**不把它們全算新增**。最低靜態受限版不需要獨立pacing busy、phase、寬R counter或payload buffer；實際合成register數尚未量得。
- O-job-local沒有tail comparator，但對前綴也節流、每job多2 AR及lookup，是否較快未知。兩版都可能改變讀寫重疊，完整parent結果與CPU／面積／帶寬分報，不能只取更短child服務時間。
- 註冊empty版的新增組合深度主要是nonempty判斷與原r_ready/r_valid；組合last-pop版多usage==1及R completion回授，critical path、Fmax、routing與clock boundary成本全unknown。從本地gate釋放到TC／shared的寄存邊界必須另外計。
- 可供M資格或模型前綴到達核對的有限source_done接口：backend本job已`req_valid && req_ready`，且`!busy_o.r_leg_busy && !r_dp_req_out_valid`，表示本job source descriptor已全發且全真RLAST。**不能僅用frontend fullbuffer pending**：job尚在job FIFO、未進legalizer時這兩者也空，會誤報。可加本job開始跟蹤／sticky輸出或復用確有等價的parent跟蹤；目前沒有跨層現成source_done port。這避開新增10bit beatcounter，但仍須計線／有限狀態。只需要條件性能分析時不強迫M生產接口加此port。[C2,C3,C5]

本輪交付的是可按具名源碼實作的兩個FSM及強版本邊界，沒有RTL驗證。接受下一步時，最低必要核對是成對D握手不重複、真RLAST前L1沒有firstVALID、AR stalled保持、read128/write256不改4個B與parent、mid-error不早退、原fullbuffer資格保持，以及local release到target gate的實際層次；不用先重跑舊模型或原生a_split。
