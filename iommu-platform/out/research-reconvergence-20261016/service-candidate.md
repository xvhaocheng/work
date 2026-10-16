# 共享服務資源：一個最強構造及有界否決

2026-10-16 JST；基準頂層 `c981e014d42368409e8bb64b4259570803ca492d`。獨占範圍：TC／DS／xbar／DDR 之間的服務承諾、非搶占讀、合法分段及返回回壓。其他席的 IOTLB／PTW 組織與失效生命期不在本席範圍。

**裁決：本席不交付准入候選。** 最強具體構造是「在翻譯完成後透明分割 payload 的物理讀、限制已承諾 fragment，並為其返回預約可排空空間」。它有可執行動作與條件收益鏈，卻被 AXI-REALM 的透明分段、既有端到端 credit 與具名普通適配覆蓋；本輪未找到額外動作或更低完整成本的表示。將它留作強普通對照，不再替它建立性能入口。

本輪依 ARS-Codex 對應的有界研究／來源核查方法，直接讀下列關鍵正文與 RTL，以來源事實、條件推導和未知分層。不是全面查新、正式系統綜述或性能實驗。Codex-only；請求／建議為 Sol/xhigh，實際獨立 model/effort metadata unknown；沒有 DeepSeek、Claude 或新子席。

## Problem

多 DMA 的已翻譯 payload 與 IOMMU 的隱式訪存最終分享 DDR 服務。即使 IOTLB 分區與 hit-under-miss 各自有效，一筆長 payload 讀也可能在 PTE 成為可見請求前，取得不可撤回的物理服務義務；若其接收者又回壓，PTE 的順序返回還可能被延長。研究問題應是：**能否減少真正落到有用任務關鍵路徑上的、可合法避免的服務承諾，同時保留完整背景工作與可部署成本？** 不能用 PTE 延遲下降代替完整 COPY、consumer 或背景進展改善。

來源支持這個問題有合理機制基礎，並不支持當前正常 COPY 已遭遇它：

- MASK §5.4 已明確把 data→translation 與 application→application 的 DRAM 干擾共同處理，不能宣稱這個耦合首次被提出。[MASK 原文](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)，PDF p8，§5.4。
- 本平台 DS 與完成口在 xbar 分開輸入，之後才於 DDR 匯合；DS 本身還含 DDTW／PDTW／PTW／CQ 等，不是純 PTE 隊列。見 S1、S3、S4。
- 最新固定正常 COPY 的 M 與 base 完整 quiet 相同（cold/hot 4481/1749），M mask0；A 最後源 RLAST3507，V 合格寫 walk3514。本席繼承這個否定結果，不改相位、冷歷史或正常來源重做勝例。[正式結果](/home/xvhaocheng/work/iommu-platform/docs/first-pte-paired-local-20261016.md:44)、[原始比較](/home/xvhaocheng/work/iommu-platform/out/first-pte-paired-local-20261016/root/paired-analysis.json)。上述數字是行為 DDR 的 RTL clock steps，不是 MIG 或板測時間。

## RootCause

### 1. 必須分開三個承諾位置

| 位置 | 已取得的來源事實 | 仍能做的合法控制 |
| --- | --- | --- |
| 虛擬 AR／翻譯請求 | TC 先請求翻譯，在 `AWAIT_RESP` 保存結果，`COMPLETE` 才發布物理 AR。ARLEN 從原 header 保留。S2:346–357、646–725。 | 可以在持有 header 的 adapter 重新組織將來物理 child；不能因已有 PA 就忽略原 parent 的完成責任。 |
| xbar 首次物理 offer／spill 接受 | `axi_mux` 的 AR arbiter `LockIn=1`，其後還有 AR spill；已 offer 的 stalled header 不能被後來 PTE 替換。S4:333–364。 | 首次 offer 前選 current-VALID DS；未發布 child 可繼續暫緩。跨 atomics／CDC 前的接受不是 MIG 接受。 |
| MIG 已接受讀及順序 R | 已生成 shim 只有 active read，展開完整 burst 後才允許接下個 active read；RID／last 元信息和 data 按序流出。QoS 僅參與讀寫切換，沒有 read-vs-read 提升。S8–S12。 | 接收／排空當前 R；不能取消舊 AR、改其 LEN、把後到 PTE 插入 active burst 或替換 held R。 |

PTW 的 `init_ptw_i` 在 IDLE 使下一狀態轉為 MEM_ACCESS，而 MEM_ACCESS 才產生本地 ARVALID；本地 AR 還要經 DS 的七輸入仲裁及非旁路 spill。這解釋早意圖可能領先共享頭的**結構窗口**，沒有證明正常 workload 恰有可利用的 payload 首次 offer。S1:77–109、S5:325–327/384–397；不恢復已結束的 First-PTE 規則。

### 2. 尚未服務、順序未輪到、接收者回壓是不同原因

MIG 物理 bank 可重排，不代表這個 AXI shim 能讓後到 PTE 先返回。已生成 read channel 的元信息 FIFO 只有依次輸出的頭，upsizer 的 held R 寄存只有 ready 才更新，不能用 ARQOS 改選。S8–S12 的直接源碼與[既有邊界](/home/xvhaocheng/work/iommu-platform/out/ddr-service-boundary-20261014/ip-semantics/primary-sources.md:25)一致。

若外側觀察到 payload `RVALID && !RREADY`，只證明當前拍被接收者擋住；不能據此推定 PTE 已在內部 ready，更不能把這段等待都算成可被優先級消除的 DDR 服務。只有新請求尚未承諾的順序、或因接收端缺空間造成的等待，才分別對應入口控制與返回 credit。

## ExistingSolutions

| 具名對象與本輪直接核讀 | 能力、信息與費用 | 對本構造的限制 |
| --- | --- | --- |
| **MASK，ASPLOS2018 §5.4，PDF p8**；[本地原文](/home/xvhaocheng/work/references/papers/mask2018.pdf)、[作者 PDF](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf) | 控制器內 Golden／Silver／Normal 三隊列，翻譯請求最高優先；Silver 應用選擇考慮 concurrent walks 和 stalled warps，並有配額。 | 「翻譯與 data 聯合管理、優先 PTE、控制背景」均已有正文前史；未證明可免費搬進既有 MIG，也未完整覆蓋本 adapter 的接線成本。 |
| **AXI-REALM v2，§III-A1/A2，PDF pp3–4**；[本地原文](/home/xvhaocheng/work/references/papers/axi_realm2025.pdf)、[正式預印本](https://arxiv.org/html/2501.10161v2) | 入口接收原 burst，按 runtime 1..256 beats 透明分段，保存地址／size／flags，重構原 RLAST；write responses coalesce。寫緩衝收齊資料才發 AW；預算與隔離亦需成本。對不合法類型並非任意分段。 | 透明物理分段與 parent-preserving 返回不是新增原語。本文沒有聲稱其「純讀分段接 IOMMU_COMP」已原樣部署；那是普通適配，需收整合費。 |
| **Aromolo 等，RTAS2026 §V-A/B/C、VI，PDF pp6–8（印刷 pp280–282）**；[作者全文](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf)，DOI 10.1109/RTAS68450.2026.00033 | 每 DPU 內至多一 pending／執行請求，其餘留外層優先隊列；合法 DNN 分塊提供有限搶占點；ABU 預算與包含 CPU 同步的 chunk 時間共同進入任務分析。 | 隊外控制、縮短非搶占單位、預算與完整任務分析這組寬主張已不足。其 chunk 時間聚合內部 memory 行為，不證明已實現逐 PTE 適配或本平台 WCET。 |
| **本地普通構件**：iDMA legalizer S6、vendor `axi_burst_splitter` S7 | legalizer 已有獨立 src/dst 長度及 R/W decouple；frontend 目前只把共同 deburst 暴露成單拍。vendor splitter 是未接入的單拍版本，保存原 header、按 ID 維護計數與原 RLAST。 | 不可強迫普通只用現有單拍軟體開關，亦不可把未實例化模組當零成本任意粒度能力。原 FIFO／TC holder／union 字段合法可復用。 |
| **端到端接收 credit 與 IOMMU VC 前史**：CTC2009 §3–4，PDF pp4–6；[全文](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)。US7480784B2，Fig10；[公開原文](https://patents.google.com/patent/US7480784B2/en) | CTC 的 CB 基礎明確以 receiver 可 eject 的 credit 限制注入；端點 credit 與完整網路複製各有費用。專利 Fig10 明確為 IOMMU 隱式請求及 response 提供獨立 VC，並說明獨立 buffering。 | 「替 PTE 留返回路／有依賴的流量用 VC／先留足返回空間」不是新概念。專利是公開技術披露，非同行評審性能證據；HTML 沒有核定 PDF 頁碼。兩者都不使 MIG 自動可重排。 |

前作範圍只到上述實際正文。本輪未把摘要、歷史 worker 摘錄或題名提升成全文覆蓋，也未宣稱窮盡 2026 年文獻。

## NovelMechanism

### 最強合理版本：譯後分段、原 parent 完成、完整返回預約

這是**被否決的新方法主張／保留的普通方案**，不另取候選縮寫。相對當前未分段資料路徑，其可執行差異很具體：在 `IOMMU_COMP` 到共享 xbar 的讀支路接 adapter，接收一筆已翻譯原 AR，將它變成較短物理 children；全部返回仍呈現成原 AR 的單一 RLAST。只做合法普通 memory INCR，unsupported／atomic 等保留原合法通路或按共同合同處理。

1. 原 TC 對原 256-beat 虛擬 burst 翻譯一次，保存 PA 及身份。adapter 接原 header 後擁有其完成義務，不因拆 child 再向 IOMMU 請求 child 翻譯。
2. 用**預先固定** `q=64 beats` 作可執行示例參數，並非搜尋出的最佳值或已測設定。每次 child 發布前，必須有覆蓋該 child 全部剩餘 R 的可信接收空間；保持原 ID／地址／屬性，child 不跨原已驗邊界。
3. 採最保守的全 COMP 支路至多一個未完成 child；真的收完其 R 才允許發布下一個。不把 `MaxReadTxns=1` 自動等同這個物理 child 限制：S7 的 `MaxReadTxns` 計原 bursts，原版仍可連續發布很多單拍 children，額外 child credit 門必須明確付費。
4. 共同 DDR selector 在首次 offer 前採 current-VALID DS-first/F4；已 held 頭優先、持續雙方 ready 時四個 DS 後給背景逃逸。這是固定具名普通規則，不看未來 PTE、實際 DDR hidden-ready 或事後 winner。
5. RRESP／RID／原 transaction occurrence／beat 索引及最後 RLAST 一起保持；原 W／AW／B 及 consumer 繼續完成，不停止既有寫 owner 所需的 drain。不把原 parent 當成四個應用完成。

因果鏈的**條件推導**是：若一筆原 256-beat payload 讀本會先於 PTE 完整承諾，而 PTE 在第一個 child 的順序服務期間自然成為合法可見 DS 頭，剩餘 children 尚未首次 offer，則 PTE 可在該 child 排空後、剩餘 192 beats 首次承諾前被選。這改變服務義務本身；沒有在 MIG 內取消／搶占既有讀，也不需要 First-PTE 早意圖。

但同一條規則可以逐句寫成普通「AXI-REALM 式 AR fragmenter＋一 child credit＋DS-first」adapter。授予相同原 header、返回空間、端點身份和 selector 接口後，沒有殘留候選專有動作。**因此結束這份構造的獨立機制主張。** 這並非證明任何實現成本完全相同；沒有已驗證的成本優勢時，亦不把未知差項當成繼續投入的理由。

### 為什麼譯後位置是必須保留的強對照

若只在 iDMA legalizer 把原 read 拆短，child 先於 IOMMU 生成，通常會增加翻譯請求／TC 佔用，即使命中不增加 PTE。若在 TC 之後拆，原虛擬請求、翻譯次數、4096B parent 和 frontend fullbuffer 資格均可保持；增加的是物理 AR 及其下游費用。S2 的 ARLEN 透明、S7 的 header 保存及 RLAST 重建直接支持這個位置區分。

這是對照能力校準，不是本輪已實作或相對既有 O-tail-prepared 的新收益。現有 splitter 同時處理 R/W、只有單拍，純讀任意 q 需普通適配；不能稱原 stock 已有完整接線。反過來，不能強迫普通方案重做 child 翻譯、分段全部 AW 或新增完整 payload 副本，來製造候選優勢。

## ExpectedBenefit

本輪無數值收益主張。上述條件若成立，可能縮短一個關鍵 PTE 所遇的**後續 payload 承諾**，並減少接收端回壓造成的附加等待。它只有在 PTE→翻譯完成→合法 V payload／W→B／consumer 是實際關鍵路徑、節省未被其他瓶頸吞掉時，才改善有用完成。

完整比較至少同時報 `(T_V, T_A, T_all, T_quiet)` 與各自有用工作、R/W bytes、實際 AR/AW/B、錯誤與 consumer 退休。若保護 V 而背景更晚，必須按相同服務需求判斷取捨；不能只在固定前台窗口計背景較少，或少做背景 COPY 取得勝利。對本輪固定 COPY，既有 base 已復現 M，沒有上述增量機會。

| 費用項 | 正確處理 |
| --- | --- |
| 原 header／PA／身份與持有 | 普通可復用 TC／AR holder，但 adapter 接受後的 owner 和原 completion 不能消失；若另存 header，明計。 |
| 翻譯及物理請求 | 原虛擬 AR 次數可不變；一筆 256 beats 在 q64 下變四筆物理 AR。額外 metadata、CDC／MIG 每 request 費用、row locality 及吞吐變化未知；不沿用舊「每 AR40」模型常數。 |
| payload 空間 | 原 DMA0/1 的512beat資料 FIFO 可在有證明時復用。若另加 q64 的不可回壓 COMP sink，僅資料就是64×8=512B，另有 RESP／ID／beat與有效狀態；不稱零 RAM。以共同可用物理並集記帳，不重算同一 FIFO。 |
| 返回 credit | 槽位必須在 child 發布前預約、直到實際消費才歸還；不能用平均空間或尚未收回的 bytes 發多張 credit。buffer usage 現未導出，新增精確觀測／傳線／門控均付費。 |
| 效能／時序 | 一 child 限制可能犧牲 MLP；選擇及 READY 路徑、原 spill、寄存延遲、Fmax／area／動態功耗均 unknown，不能只數計數器 bits。 |
| 系統工作 | 原 AW/W owner、B、錯誤、reset、完整consumer、CPU 控制及公平 escape 全部保留。沒有測得 CPU 或物理記憶體總收益。 |

## Falsification

以下是可驗偽預測與相鄰失敗，**不是排程好的新實驗，也沒有執行**：

| 判別 | 預測／停止條件 |
| --- | --- |
| 可行正條件 | 自然的 PTE 已成 current-VALID 頭；更短 child 的下一段還未首次 offer；PTE 後繼處在完整任務關鍵路徑。譯後分段改變後續承諾，可能改變完整完成向量。 |
| 鄰近失敗一 | PTE 來時原長讀或全部 children 已被 MIG／上游 held 邊界承諾，不能追回；改計數器或 QoS 不會改變偏序。 |
| 鄰近失敗二 | 同一正常 COPY 中 A 源已全回完才開始合格 V 寫 walk；這正是最新 mask0 結果，分段先行可能只增加費用。 |
| 鄰近失敗三 | 原接收者已有充足空間且持续 ready；返回 FIFO／credit 不改善原順序服務，只有面積／可能新延遲。 |
| 完整目標失敗 | PTE 提前卻被後續 B／consumer／其他資源掩蓋，或背景服務退化超出共同合同；不能算成功。 |
| 新意否定，已成立 | 同新增付費接口上的具名普通 fragmenter＋credit＋DS-first 執行相同動作；未提出不同合法結果或被驗證的較低完整費用。此版本不投入性能／生產。 |

舊 LR1 是 source 全讀後撤權／源頁歸還；本構造不釋放原 source 或提前撤權。[LR1](/home/xvhaocheng/work/iommu-platform/docs/local-read-reclaim-20261015.md:7) 的 phased／targeted 普通覆蓋不恢復。舊 SDH1 是同 paid direct-OUT 的 hit 選擇；不重包其 current-W/source 最老優先。[SDH1](/home/xvhaocheng/work/iommu-platform/out/source-drain-hit-20261014/README.md:3)。CO1 的用途退休／釋放鏈排序亦繼承普通 OC1-min 覆蓋，本構造不借任務登記改名再提。[CO1](/home/xvhaocheng/work/iommu-platform/docs/completion-obligation-design-20261014.md:5)。

## NoveltyRisk

**最高風險已不是少一個實驗，而是核心動作沒有新差異。** 翻譯感知優先已有 MASK；透明分段已有 REALM；隊外與合法有限搶占已有 RTAS2026；返回 credit／IOMMU VC 亦有準確前史。普通適配不等於已發表同一完整 SoC，但不能僅因本平台沒有它而認領方法新意。

仍未被本輪證明為不存在的空間只有：在一個具體正常來源和相同服務需求下，證明超出該普通 adapter 的新合法行為，或相同吞吐／保護下有可實現且實付的狀態、buffer、端口、頻率或 CPU 成本差。這是有界未知，沒有現成見證，也不是本輪新候選／自動下一 gate。不能靠多加 deadline／PTW 標籤／分類器，或限制普通復用原 FIFO，填補這個缺口。

### 補充有界判斷：held payload 的 R 回壓

按 root 的一次有界補問，直接核了這個變體，結果仍不准入另一候選：

1. **結構上可能阻塞。** TC:361 的 RREADY 直透，`axi_mux`:369–385 按 RID 路由 ready 並共用 R spill。iDMA transport:254–257 又使 RREADY 同時依賴 byte FIFO 可收與 `r_dp_ready_i`；所以一拍 payload 真 held 可以延長其後順序 PTE 的返回。
2. **不能憑空造當前反例。** PN 中 DMA0/1 已各用512beats資料 FIFO，fullbuffer 證書僅允許正常、對齊、單4096B parent；原設計正為避免 source 資料填滿後反擋 shared R。S13–S15。這不保證所有狀態恒 RREADY=1：`r_dp_ready_i` 仍接 completion 控制，既有佔用／metadata／錯誤須另核；本輪沒有新增 native held 樣本。
3. **純入口 QoS 不解 held 拍；普通 sink reservation 可解容量回壓。** 若先為每個已承諾 child 預約足量獨立 sink 空間，可以先把 held payload 存入原 buffer 或付費 FIFO，之後才使其後 PTE 依 MIG 原順序出現。這去除接收端等待，沒有讓 PTE 越過尚未服務 payload。只在 PTE 端加一個小 FIFO 而不排空前序 payload，不能提供這個效果。
4. **端點 VC 不等於改造 MIG。** 下游可重排的假設沒有被加入；若要求 PTE 在尚未傳完的舊 payload 之前出現，必須改服務者／返回路並支付新存儲及排序，不是現有 QoS。若只要求對端不回壓，CTC 型接收 credit 已提供普通構造，原 fullbuffer 有條件復用；雙方獲同空間和同觀測後沒有新專有決策。
5. **成本判斷範圍。** 同一 q sink／同一 credit 表可被普通方案逐項使用，故無候選獨占的必要成本優勢；未作電路實現，不能宣稱兩種任意 VC 實現面積／Fmax 必然相同。這個變體仍關閉在普通能力層，不另開模型或原型。

## 直接來源與閱讀邊界

表內行號是本輪直接讀取的實際文件，不以其他 vendor 同名模組替代。源碼層級不代表目前 FPGA 加載或時序身份。MIG 是歷史已生成4.2參考，板端綁定沿用既有研究限制。

| ID | 實際源碼與決定範圍 |
| --- | --- |
| S1 | [DS interface](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv:77)：77–109，七路 AR 和 spill；118–185，AW/W owner 保持。 |
| S2 | [TC wrapper](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:332)：332–368 原 header／ARLEN、RREADY；620–725 AW 先行、讀准入、翻譯和 COMPLETE。 |
| S3 | [SoC mapping](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv:20)：20–32 COMP=2、DS=3及 ID；[FPGA instance](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_xilinx.sv:291)：291–319 xbar/port cuts，850–851 接線，934–945 atomics，1006 起 CDC。 |
| S4 | [實際 CVA6 vendor axi_mux](/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/axi/src/axi_mux.sv:333)：333–364 AR LockIn/spill；369–385 R 按 ID 路由；[xbar instantiate](/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/axi/src/axi_xbar.sv:131)：131–163 每下游口的 mux 與 spill 參數。 |
| S5 | [PTW](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:325)：100 active、325–327 start、384–397 首次 AR／R 消費。 |
| S6 | [legalizer](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:139)：139–175 分離 src/dst page length；199–214 decouple；[frontend](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:365)：365–376 共同 deburst 與停用 serialize。 |
| S7 | [vendor splitter](/home/xvhaocheng/work/iommu-platform/cva6/vendor/pulp-platform/axi/src/axi_burst_splitter.sv:19)：19–32 單拍與 MaxReadTxns；99–119 unsupported filter；231–305 AR／RLAST；397–462 保存原 header、可連續發 children；488–535 原 burst counter／ID queue。不是已接入或任意 q 的驗證。 |
| S8 | [MIG active AR](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_ar_channel.v:138)：138–178，單 active header 與 QoS。 |
| S9 | [MIG command FSM](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_cmd_fsm.v:107)：107–117，最後 command 後才重新 axready。 |
| S10 | [MIG arbiter](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_cmd_arbiter.v:125)：125–131 僅讀／寫及 hi_pri0；159–215 RD_PRI_REG；202–203 QoS 只釋放讀寫 hold。 |
| S11 | [MIG R channel](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_r_channel.v:225)：225–275，data/metadata FIFO 及 RID 頭。 |
| S12 | [MIG R upsizer](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_ddr_r_upsizer.v:885)：885–930 held output 只有 ready 才更新。 |
| S13 | [iDMA transport](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:254)：254–282 FIFO／completion 回壓；468–483 byte buffer；[buffer](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_buffer.sv:33)：33–50 byte FIFOs、usage 未導出。 |
| S14 | [實例容量](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1263)：1263–1272 兩路512beats與另兩路3beats；[fullbuffer certificate](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:14)：14–18、26–41 正常對齊4096B／單parent資格。 |
| S15 | [iDMA backend](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519)：519–532 NO_ERROR_HANDLING 和 response-ready；664–705 transport 接線，不能由容量獨自推 RREADY 恒1。 |

重點本地原件 SHA-256（直接計算；不新增一份源碼副本）：

| 原件 | SHA-256 |
| --- | --- |
| MASK PDF | `f72e642e761582784d7508dabc743b5e5176f1bb2cb11282260bef18545c41fb` |
| AXI-REALM PDF，封面為2501.10161v2／2025-07-07 | `e097628f29be9c0b99bf3f1077305517de1ebc2b2591af6891db36572dccc926` |
| S1 DS | `a5fb800cc23613e57704bb7622ccb679510b01871aa81457dbfc673580a86cc8` |
| S2 TC | `4d7919da6890980b3ec7dd11f71dbeee7671045e432d0f7ffa74492d1477106f` |
| S4 axi_mux | `71b6b0187138fe1a659154f939678a92b1b36c3db7a5b74f7b64329beb107e97` |
| S7 splitter | `16023ba5ffdec045313e3d794976d569e26b43484c1fba9d685988a9db1998f1` |
| S13 transport | `4447bec83a3335ee106a627177a1113730c27c9587d30780892eaaa972f7d80a` |

RTAS2026 與 CTC 本輪經作者 PDF 網頁直接讀取，按 PDF 絕對頁數定位，未另下載入本輪資料夾；不製造本地 hash。定向網路檢索另見到 2026 Preemptive VCs 正文 §III，[來源](https://arxiv.org/html/2607.01430v1)，但本裁決不依賴其數值、成本或完整覆蓋，也不把預印本當成已驗原生能力。

**執行帳：**只寫本報告；源碼／原文／舊接受紀錄只讀，沒有新模型、仿真、綜合、構建、trace 重放、板操作、goal／暫停狀態變更、共享文件或生產修改。本席不單獨提交／推送，由 root 統一審閱與同步。完整成本、正常可利用窗口、native 預測及硬體時序均未藉此補為已知。
