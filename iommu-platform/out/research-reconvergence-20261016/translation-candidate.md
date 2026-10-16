# 翻譯組織候選：借用 TC 生命週期的細粒度 walk 續行

日期：2026-10-16。獨占範圍：IOTLB／PTW 狀態組織與併發，不主做 DDR 調度或失效回收。工作區基準 `c981e014d42368409e8bb64b4259570803ca492d`；所讀 IOMMU 組件 HEAD `57eb5621802ef9ed8d77c684b9a03205c6630391`。請求模型 Astra／xhigh；獨立執行 metadata **unknown**，不以請求值冒充實際配置。Codex-only；未調用 DeepSeek。

**裁決：本次最強候選未通過「獨立新機制或已具體成立的成本差項」的研究准入。** 保存續行狀態並以單一執行器推進多個 walk 的核心能力，已由 Power、Hill、Wood 的 HPCA 2014 原文直接提供；不能僅與 DWS 的完整 walk 分配相比便認作新架構。TC 請求頭借用有可核對的局部儲存機會，但沒有證據證明強普通 threaded walker 必須重複儲存它，且候選也沒有進一步改變一個普通實現難以完成的決策。本報告保留該具體 IOMMU 適配，**不把整個共享翻譯、狀態壓縮或廉價併發家族判死**。

這不是以「普通程式可重寫同一規則」否決接口價值，也不是因未做綜合而把任意成本不等式暫掛為候選。已發表能力、普通獨立構造、候選新增接口上的消融，分開記於下文。本次沒有新性能模型、實驗、綜合、RTL／Linux 生產改動或硬體操作；First-PTE 固定 COPY 的 M mask0 結論保持，不重跑、不改相位。

## 1. Problem：需要改變的具體結果

原始問題是：多個 DMA 的翻譯 miss 在同一 IOMMU 相遇時，**有限的 walk 狀態儲存與一個長期被佔用的執行器綁在一起**。即使後來請求的上下文已知，也只能等前一個完整 walk 結束，才可取得執行器。本候選嘗試讓「一個有未完成翻譯的請求」持有小型續行記錄，而不持續獨佔整套 PTW／TL 主控制。

目標任務 Y 是已解析合法設備上下文的兩個 S1 miss，至少有一個請求需要較早得到服務；Z 是後來 miss 的首個／後續 PTE 能在前一個 walk 完成前得到合法服務。此處先提出條件架構問題，**尚沒有自然來源頻率或完整應用收益證據**。不把 E3 的 V-hit 當作兩個 miss 的直接實驗，也不把 E2 的事後替換當作併發排隊。

### 本輪直接查看的原始證據

| 原始輸入 | 本輪讀到的事實 | 對候選的約束 |
|---|---|---|
| [E2 W7 forward N112 r1 record](/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w7-forward-n112-r1/record.json) | warm、deadline、freeze 的 PSCID1 兩個有效條目一直在 slot6／14；probe `phase_stats[2]` 完成 4096B、0 miss、0 walk、0 data error | 已有保留翻譯的陰性條件，不需要新併發機制解釋 |
| [E2 W8 forward N112 r1 record](/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w8-forward-n112-r1/record.json) | warm 的 PSCID1 條目在 slot2／10，deadline 無 PSCID1 有效條目，freeze 在 slot5／13 重新出現；probe 完成 4096B、2 miss、2 S1 walk、0 data error | 續行機制不能讓被替換的條目變回 hit；容量保護與 miss 服務仍是不同責任 |
| [E3 原始配對](/home/xvhaocheng/work/iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json)，第9對、sequence17／18 | D0：e4=6089、e41=6361、e70=6370；D128：6113、6514、6523。Aremaining 272→401，Vremaining 281→410，均 +129；e4−e39 均135、e4−e69 均131；兩輪 V 均0 miss，兩方各完成64B且0 data error | 支持局部 miss→hit 等待傳播；現有 N 已有合格 hit 旁路，不能拿這個現象當新 miss 續行收益 |

E2 快照按原分析器的公開欄位定義，只讀解碼 `raw[0x100..]`、`raw[0x120..]`、`raw[0x140..]`，沒有重跑全64項驗收：[解碼](/home/xvhaocheng/work/iommu-platform/scripts/research_results.py:318)、[快照布局](/home/xvhaocheng/work/iommu-platform/scripts/research_results.py:966)。E2 表中的 phase active_cycles 不是報告 D87，沒有互換計時分母。原64項與原22輪總結仍分別以 [E2 報告](/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/RESULTS.md)及 [E3 報告](/home/xvhaocheng/work/iommu-platform/docs/e3-controlled-results-review.md)為入口。

## 2. RootCause：源碼中的綁定點

以下行號均為本輪實讀的當前源碼，不冒充歷史板測當時行號。

| 編號／源碼入口 | 決定性事實 |
|---|---|
| S1 [PTW 狀態](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:90)；[狀態寄存器](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:102) | 一份 `state/main_lvl/s1_lvl/ptw_stage`；IOVA、兩份 GPA、S1 leaf PTE、實體 pointer、global、implicit 與 fault 狀態只有一組（102–151行） |
| S2 [PTE 端口](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:274)；[記憶體狀態](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:384) | AR 固定 ID0、len0、size3，即每次8B；`MEM_ACCESS` 等 AR 接受後到 `PROC_PTE` 等 R，非葉再回 `MEM_ACCESS`。不是現成的多 outstanding／cache-line PTW |
| S3 [S1 續行](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:507)；[完成](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:478) | 非葉回應提供下一層 pointer／level；S1-only 可在這個已消費 R、尚未發下一 AR 的邊界換入。葉完成產生 IOTLB update，並仍有 A/D、頁大小與權限相關合法性判斷 |
| S4 [TL 請求持有](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:168)；[解析上下文](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:219)；[PTW 接線](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:779) | 正常請求 spill 等 response 才 ready；`dc/pc/pscid/gscid/root` 是主請求的一份寄存器。PTW 的 store、root、IOVA、context ID 等來自這些活輸入，不能只切 pointer 卻仍用另一請求的身份 |
| S5 [TL 完成路徑](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:1280) | 正常成功路徑仍為填 IOTLB 後 lookup hit，才到 COMPLETE（1293–1331行）。多個完成結果、回覆反壓和填回端口都必須核賬 |
| S6 [N 窗口](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:303)；[N 資格](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:84) | N 是跨 bank 的受限 S1 4KiB hit 路徑；accept 需 DDTC hit、合法 context、IOTLB hit 等，不是第二個 miss slot |
| S7 [IOTLB tag／lookup](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:58)；[fill](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:317)；[P 替換](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:353) | resident 條目是有 PSCID／GSCID／stage 的完成翻譯；P 改 allocation mask／PLRU 歷史，沒有把在途續行放入 leaf cache。兩域相同 IOVA 不代表同一翻譯 |
| S8 [bank 生命週期](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:42)；[持有輸出](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:603)；[等待](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:674) | 每 bank 至多一個在途翻譯。TC 在 AWAIT_RESP 保留 `trans_req_data_q`，即使 VALID 降低也持續輸出其值；回覆後才可進 COMPLETE |
| S9 [bank 仲裁與回覆](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:365) | wrapper 內有兩份 bank request，對外只送 RR 仲裁結果；回覆按 bank 路由。候選要讀已接受 bank 的穩定頭，須新增按 bank 的讀出口，不能假設現行仲裁輸出永久指向該頭 |

## 3. NovelMechanism：先發展最強合理版本，再判新意

候選暫稱 **TC 借用續行**，名稱不表示新穎性已成立。先限於普通、合法、S1-only 請求；兩個 bank 可屬不同 PSCID，不能偷用同一權限或把相同 IOVA 合併。S2、ATS、MSI、PID、fault 與重配置須走完整共用合同，不以本提案宣稱已解決。

### 操作與信息時間

1. **請求接受前**：bank ID／IOVA／DID／PID／access type 已由 TC 穩定提供。必須先有空續行槽或可用的合法重試規則，才拉 ready。DDTC 命中或完成 DDT walk 後，才有可信的 PSCID、頁表 root、模式與權限控制；原始 DID 不足以代替已解析 context。
2. **walk 啟動後**：共同執行器選一個 ready slot，按其穩定 request 與解析 context 發出 PTE 讀取。槽保存「現在停在哪裡」，其原始 request 可借用 TC。初版每次只容許一筆 outstanding PTE，完全不要求改 DDR 已承諾順序。
3. **已消費 PTE 回應、尚未 offer 下一 AR**：合法非葉結果產生下一 pointer／level；把它寫回該槽。執行器可換到另一 slot。已拉高但未握手的 AR 不可改 owner；已接受而未返回的 R 歸屬也不可轉移。
4. **完成**：結果必須帶 bank 與身份，經共同權限／fault 路徑、IOTLB fill 與 response 路由退休。若採直接結果返回以免等 fill，這是另付費的完整路徑，普通對照同樣取得；本提案不把它算成免費附贈。
5. **調度**：首要對照用每 PTE 邊界的 work-conserving RR。若任務確有可信優先級／服務需求，再用同信息的固定優先級＋aging。候選沒有額外敏感度預測器，也不靠 P+N 組合立新。

### 具體可借用資源與必要新增成本

[top 的 request struct](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv:135) 在此配置是 **118 bits**：64 IOVA＋24 DID＋1 PID-valid＋20 PID＋6 type＋1 priv＋1 debug＋1 bank。位寬來自 [package](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv:79)、[type](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv:528)、[平台配置](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv:110)；PAddrWidth=56、AxiAddrWidth=64、IOTLB=16。

**可確定的局部節省**：若一個獨立 walk buffer 原本為每條 walk 再複製這份完整頭，按 bank 借用可避免該118-bit副本；這是源碼支撐的儲存機會。**它不是相對最強普通實現的118-bit淨節省**，也不代表可以直接刪掉現行 spill register 的全部內部容量。

| 項目 | 候選必須支付／保留 | 強普通方案的同等機會 |
|---|---|---|
| 原始請求 | TC 頭的只讀出口、選擇 mux、owner 有效期；不可在舊回應還可能返回時重用 bank | threaded walker 可只保存 handle，讀同一 TC 頭 |
| S1 續行核心 | 最直接欄位表示為 pointer56＋level2＋global1；59 bits 是欄位級小計，**不是完整成本或不可壓縮下界**。8B 對齊低位可省，普通對照也可省 | 同樣壓縮、同樣 pointer／level |
| 已解析身份 | PSCID20、必要模式／錯誤控制與有效性；若改為重新 DDTC lookup，須支付端口、重查延遲及 miss／失效處理，不能寫成零成本 | 同等保存或重查 |
| 同時多筆 PTE | 另需 return-owner／ID／等待狀態。現行 ID0 單筆路徑不能免費得到亂序返回能力 | 同等 outstanding 數、ID、DS 端口和返回緩衝 |
| 多完成／反壓 | 共用 fill 口、回覆仲裁、被 backpressure 的結果儲存；不能重用尚未退休的槽 | 同等結果 holding／bypass |
| IOTLB 容量 | 續行槽若獨立，須計新增 bits；若借 leaf entry，則扣除有效 leaf 容量與 lookup／更新端口，不假定16項全保留 | 同容量、同端口、同可用 entry 類型 |

TC 只保留原始 request，不保留整個 TL 已解析 context（S4）；因此「因為已有 TC，第二條 miss 零狀態」是錯誤結論。

### 在同一候選內再發展三個可能的增量

為避免只停在「GPUMMU 已有」，本輪把候選加強到以下三處，然後逐一判斷。

**A. 長遠端延遲時繼續其他 walk。** 可在第一條 PTE outstanding 時讓共同執行器發第二條；這需要至少兩份返回歸屬／續行狀態和能接受這種併發的端口。HPCA2014 的 page-walk buffers＋單狀態機本來就以記憶體回應驅動續行，直接具備這個能力。現在只有一筆 outstanding 的合法接口時，任何方案都不能靠換掉控制器寄存器讓未返回的 PTE 消失；增加 ID／端口時，普通方案同享。這個增量被直接前作覆蓋，不保留為新點。

**B. 共用上層路徑，壓縮兩條續行。** 同一合法頁表根與 prefix 下，可共享已確定的下一節點，僅讓各請求保留其 index／權限責任；這確能少做重複工作。但 MICRO2018 Neighborhood-aware 已把 PA／PL／CL 加到 IOMMU pending buffer，從上層返回推進尚未開始的 partial walks。更簡單的同 PTE MSHR 合併／PWC 也須作普通對照。本平台每次只讀8B，不自帶該文64B行內8個PTE的條件；擴成64B必須計帶寬、緩衝與無用讀。多domain不同root時也不能憑VPN相同壓縮；共同S2結果可重用只是一個條件來源，尚無本次自然軌跡或獨立低成本表示。這裡未取得殘餘新差項。

**C. 僅一個完整續行槽時讓後來的急件取得槽。** 在安全 PTE 邊界把背景 continuation 丟棄，保留 TC 頭，先服務急件，之後從 root／PWC 重啟背景。它避免第二份完整 continuation，但用重讀 PTE 與背景延遲換空間；普通 abort/retry＋同一 PWC 可完成同樣動作。若不允許重讀且兩條路徑無共享，至少必須保存被暫停者的已得進度或把它移到另一個付費位置；不是更換「續行票據」名稱就可消除的狀態。這是明確空間—工作量取捨，不是本輪發現的新算法。未用缺綜合的任意係數來保留收益主張。

## 4. ExistingSolutions：具名前作與三類強對照

### 決定 novelty 的原文

| 原作／本輪實讀範圍 | 已發表能力與原條件 | 與本候選的關係 |
|---|---|---|
| Power, Hill, Wood，**Supporting x86-64 Address Translation for 100s of GPU Lanes**，HPCA2014；[作者 PDF](https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf)，[本地原件](/home/xvhaocheng/work/references/papers/gpummu2014.pdf)，PDF p7 §4.4／Fig.9，已視覺核頁 | 一個 page-walk state machine，page-walk buffers 保存各在途 walk 的 outstanding address／state；記憶體返回後匹配 buffer，送回狀態機發下一級或完成。另有 PWC。原文沒有聲稱每個 walk 都必須複製完整上游 request | **最近且直接覆蓋核心架構**：共享執行器、狀態與執行分離、長等待期間多 walk。未核得該文已實現本平台 TC bank 借用或多domain QoS；不把這兩點也宣稱已發表 |
| Pratheek, Jawalkar, Basu，**Improving GPU Multi-tenancy with Page Walk Stealing**，HPCA2021；[作者 PDF](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)，[本地](/home/xvhaocheng/work/references/papers/walk_stealing2021.pdf)，PDF p5–6 §V–VI，印刷頁630–631；p5已視覺核頁 | walker 先按tenant分配，完整walk結束後可steal；DWS等待其他tenant至多一個walk，DWS++放寬借用。它並非在每次PTE返回切換整個walk的論文 | 可以說候選動作邊界比其具體設計細；不能據此說比最接近的threaded walker更新、更省 |
| Shin 等，**Scheduling Page Table Walks for Irregular GPU Applications**，ISCA2018；[作者 PDF](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)，[本地](/home/xvhaocheng/work/references/papers/walk_scheduling2018.pdf)，PDF p6–7 §IV，印刷頁185–186；p7已視覺核頁 | 空閒walker選pending request；instruction ID分組、估計總PTE工作量；PWC的2-bit計數保護已有pending reuse，全部受保護時退回PLRU | 完成組／剩餘工作與PWC保護已有前史。不能把「續行有引用所以不換出」獨立立新；其instruction ID不能免費視為DMA job ID |
| Shin, LeBeane, Solihin, Basu，**Neighborhood-aware address translation for irregular GPU applications**，MICRO2018；[作者 PDF](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)，[本地](/home/xvhaocheng/work/references/papers/neighborhood2018.pdf)，PDF p6 §IV-A/B／Fig.7，印刷頁357，已視覺核頁 | pending buffer保存PA／PL／CL，walker service table提供當前VA／level；一次64B PTE行返回可更新其他pending walk的partial進度，並抑制可合併walk啟動 | 直接限制候選的prefix共享加強版。沒有把其64B／相同合法地址空間條件當作本平台已存在 |
| Li, Wang, Tang，**Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs**，DAC2023；[作者 PDF](https://libingyao.github.io/assets/files/DAC2023.pdf)，[本地](/home/xvhaocheng/work/references/papers/orchestrated2023.pdf)，PDF p4 §IV | 用TB壓力調度、TBid索引TLB分區、鄰接set sharing；TB駐留與可重用ID是其具體條件 | 容量／並發聯合考慮不是新點；DMA bank生命週期與TB不可任意等同 |
| Costa, Martins, Pinto，**Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems**，RTCSA2025；[作者全文](https://arxiv.org/html/2508.19670v1)，[本地](/home/xvhaocheng/work/references/papers/bermuda2025.pdf)，PDF p9 §V-C | 討論stream QoS、static mapping／coloring；完整緩解實驗明確在該文範圍外 | 提供IOMMU干擾的直接前史，不宣稱它已實作本續行架構，也不把其未做緩解誤讀成全領域空白 |
| Alam 等，**ShareMMU**，MICRO2026 camera-ready；[作者入口](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)，[本地](/home/xvhaocheng/work/references/papers/sharemmu2026.pdf)，PDF p7–8 §V-C1/2、Algorithm1及相鄰討論 | Permission Tracker分離每加速器權限；AA-LRU使用access-mask與LRU，處理共享與不同工作集 | 不得以「跨設備共享＋逐設備權限／共享度替換」重新包裝續行；本輪未全面審閱其安全／失效證明 |

以上是有界原文核查，不是全面系統查新。DWS／ISCA18的完整walk邊界與GPUMMU的共享FSM不構成相互矛盾：前兩者討論隊列／分配策略，後者給出更早的內部實現組織。該關係為本輪reader inference，非作者間正式否定；scholar confirmation pending。

### 對照必須如此配置

1. **實際源碼基線 C0**：現有單PTW＋16項IOTLB，保留N對合格hit的既有能力與P合法配置；兩者不是候選的創新成分。以源碼S1–S9為準。
2. **發表近鄰 C1**：GPUMMU共享FSM＋buffer，移植必要RISC-V／多domain語義；固定同context數、同PTE outstanding數、同IOTLB容量、同結果端口，不以32個完整walker去比較候選1個FSM。
3. **獨立普通構造 C2**：兩個continuation槽＋一個共享S1 step函數；每PTE邊界RR，另有與候選相同可信優先級的版本。只存pointer／level／必要context，請求以bank handle從穩定TC取得。不是讀完候選後任意發明一個重型「普通」對手；它直接由現存TC保持語義與既有threaded-walker結構導出。
4. **可執行簡單替代 C3**：若不新增第二槽，則固定優先級的整walk排程；若准許重讀，加入安全邊界abort/retry，原頭仍留TC，重新開始時走普通PWC。兩者都計背景完整完成／重讀流量。以下介面級規則可直接落成有限模型，但本輪沒有執行：`on R: advance(owner); select ready slot by RR/priority; issue exactly one legal next PTE; on no slot: defer acceptance or legally restart a retained request`。

候選新增的TC讀口若讓C2得到逐事件同等行為，**只限制選策獨占性**。是否有整合貢獻，另看真實既有資源、額外端口和部署能力；本輪對這一層的否決理由見第7節。

## 5. ExpectedBenefit：可證偽差異及相鄰失敗

**條件預測，不是測量：** 限於一筆outstanding PTE、已完成對方context解析、後來V已可選且有槽，A在當前返回之後尚需r次PTE訪存。若給V固定優先級，完整walk不搶占基線在V首個PTE前仍需完成A的剩餘r次訪存；細粒度續行只需等已承諾的當前PTE返回及切換。差值為「被越過的A剩餘服務」減去context暴露／切換開銷。對兩個S1 4KiB cold walk，r最多是剩餘兩層，但其實際時間不設常數；DDR沒有已證上界時，不能把這寫成絕對時限保證。

這個差異足以區分C0／整walk排程與候選，但**C2同樣得到**。若RR交錯兩個等長walk，末條完成未必改善，前條還可能變晚。完整makespan通常不會因單端口只換順序自動縮短；對任何deadline／公平性價值，必須同報A的延後、兩方完成字節、最後response、所有PTE與quiet。

相鄰失敗條件：

- V是既有N可接受的hit：續行不增加它所需的walk能力，可能只增加控制延遲。
- A下一返回就是最後葉，或V在這之後才成為合法可選請求：無可讓出的剩餘PTE服務。
- 後來V的DDTC本身miss、尚未解析context：不得假裝已可直接開始S1 PTW。
- 相同單筆接口下A的PTE長時間未返回：僅在返回邊界換入的版本不能隱藏這段時間。
- 所有續行槽被互不共享且不能重試的在途請求佔滿：壓縮原始頭不會生成新的返回歸屬槽；候選無特殊准入能力。
- 背景恢復前IOTLB容量不足、fill／response被反壓或再用消失：改善局部walk等待可能沒有完整任務收益。

## 6. Falsification：最小判別與本輪已完成程度

原本最小可證偽命題應是：**在同兩個上下文、同儲存／端口機會、同合法信息下，TC借用續行相對C2，是否少一項必要儲存或端口，或出現不同的合法服務決策？** 不是只問「比現行single blocking PTW快嗎」。

本輪已完成源碼與原文層的判別：

- GPUMMU Fig.9直接給出共享FSM＋address/state buffers，否決「單執行器、多續行」本身的新架構主張。
- TC在等待期間持有118-bit頭，確有資源；但相同bank只讀出口讓C2同樣以handle取頭。候選沒有另一個可核對、C2仍需支付而候選可省去的必需狀態／端口。
- 候選的prefix加強版與Neighborhood-aware的PA／PL部分續行有直接結構前史；現行8B端口還使64B鄰域收益不能免費移植。
- 有限buffer的最強合法退路是保留原request後重試，工作量代價明確；沒有找到新的壓縮不變量讓兩個無關地址空間的不可重試進度共用一份狀態。

因此本輪**不安排局部模型或綜合去驗證一個已無具體殘餘差項的泛化口號**。這是有界設計裁決，不是已執行RTL等價證明。若後續另有確定部署需求要求在原兩bank限制下支持雙miss，該適配可作工程候選，先明確比較對象和服務目標；它不自動恢復研究／硬體實驗。

## 7. NoveltyRisk：接受／否決理由與未解問題

**接受的事實與可復用產物**：單PTW的安全切換點、活輸入依賴、TC頭的118-bit重用機會、S1續行欄位、同總資源的普通基線，以及三個加強版的必要費用，均已落到源碼或原文。這些足以避免下一次把完整walker複製作為唯一成本對照。

**否決本候選當前研究版本的三個共同理由**：

1. 核心組織有**直接發表覆蓋**，不是單純「模型可改寫成普通O」。GPUMMU已把狀態與共同執行器分離，Neighborhood-aware已讓pending buffer持有partial進度。
2. 未發表覆蓋的本平台TC借用，是具體的整合選項；然而目前只能證明它避免一份**可選的重複副本**。已核原文不要求該副本，源碼也讓普通handle式實現直接取得相同資源。未找到其與公知組織相比的另一項必要代價下降／部署能力增量。不能把弱基線多複製118 bits當候選淨節省，也不能僅因「尚未綜合」而把差項留作待驗證勝例。
3. 實驗動機尚未跨到新對象：E2證明容量替換，E3本輪代表對是V-hit等待；兩者都不提供自然的雙miss急件合同。沒有因果證據說這個具體系統現在值得為該適配新增context／端口，而即使未來值得，也不自動使架構新穎。

**未解但不自動排期**：自然多domain雙miss的完整工作／服務目標；若有特殊既有bank讀口或可用儲存宏，是否出現與標準threaded設計不同的實體成本曲線；通用S2／fault／重配置需保存的最小上下文；在實際DS可用outstanding／ID數下的代價。這些不是本輪已找到的殘餘創新，不把unknown寫成正面候選分數。

停止範圍僅為本報告的「TC借用＋PTE邊界續行／普通多上下文」版本，不否定未來來源證實的特殊狀態壓縮、協同服務或廉價實現研究。完整主線選擇交回root，本席沒有改動共享狀態、計畫或文獻目錄。

## 8. 可導航原始入口、讀取完整性與執行界線

原始輸入SHA-256：

| 輸入 | SHA-256 |
|---|---|
| E2 W7 forward N112 r1 record | `38a2441e1d5a2eb706075ce5b4c91ae7adf222dedd582aba1a582383593b37cb` |
| E2 W8 forward N112 r1 record | `d7edd3c8c494cdae74e969bdc99eb1b84305cefc6c94f82ece8bcf17e0800301` |
| E3 all-original-pairs | `3c6f8096f46aec59fc778a6741be0eb148bdeb90e48b5afc580a1074ac1b7efe` |

PDF頁碼均為本地PDF的一起算物理頁。對全部七篇調用一次ARS `pdf_read_preflight.py`，共同結果為 **UNAVAILABLE**：`pypdf-not-installed: preflight cannot parse the document`；不是PASS。頁面內容由Poppler提取，GPUMMU p7、DWS p5、ISCA18 p7、Neighborhood p6另以渲染圖直接核查；沒有把結構預檢不可用掩飾成完整文件認證。以下按ref_slug保存其必要sidecar信息；`declared_page_count`、`enumerated_page_count`、`reader_page_count`均為null，schema=`pdf_read_preflight/1`，tool=`pdf_read_preflight/1.0.0`。

| ref_slug | SHA-256 | preflight generated_at（UTC） |
|---|---|---|
| gpummu2014 | `d229fa527894a2dd0309bed84a12dcf4fb67b01f5cb1202ad6b4761d819c94ac` | `2026-10-16T11:37:29.470679+00:00` |
| walk_stealing2021 | `5672fecbdbbd99e8f5a3ccccd5fde3f5877c8f36124f5010186dfd21f633d22a` | `2026-10-16T11:36:01.191562+00:00` |
| walk_scheduling2018 | `a559d550ee41d30e53f057d32154234dd4ba8bbfb22d89699c812f5fce4b3ef1` | `2026-10-16T11:36:01.312523+00:00` |
| neighborhood2018 | `112f9cefc6c1bbcb3f171a13207c72b5e1c5062472fb879dccd48c8b1967f425` | `2026-10-16T11:40:52.144212+00:00` |
| orchestrated2023 | `65f29701646a6e8e1b7b9507855d0e10047e3286439e199b955391475664e46b` | `2026-10-16T11:36:01.397241+00:00` |
| bermuda2025 | `d85ffeaa949a3ad5f17e950871a2f85c49b61bf237e70fca813a2535bb8a51f8` | `2026-10-16T11:36:01.464852+00:00` |
| sharemmu2026 | `a9759951dc4a17ec99e95bd82b15c7234a18aab4cce0e696c24aa798af6dcfd2` | `2026-10-16T11:36:01.527939+00:00` |

本輪定向web查缺包括 `page table walker multithreaded`、`page table walk preemption checkpoint walker`、`Supporting x86-64 GPU Power Hill Wood pdf`。使用搜尋定位作者原文後，核心結論回到本地同hash PDF正文；不以搜尋摘要、舊worker summary或論文題名作覆蓋證據。未對所有專利、商用IOMMU或完整領域做查新。

ARS-Codex按有界問題構造→原文驗證→比較分析使用；沒有啟動完整pipeline、外部模型、學者已讀標記或pipeline passport。本文由Codex產生，原研究中的DeepSeek歷史歸因不改寫。本席只交付本文件；臨時PDF提取／渲染與預檢在 `/tmp/iommu-translation-candidate/`，不作持久研究結論入口。
