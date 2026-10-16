# FIRST-PTE-INTERFACE-COST-1：舊正式結果與最近前作的有界核對

2026-10-16，Codex-only。ARS `deep-research/fact-check` 的既有來源復用階段；本席輸出為證據核對，不啟動完整 pipeline。本席請求 Sol/high，獨立實際模型／effort 元資料 unknown。僅寫本目錄兩份檔案；未重跑 simulate、verify-only、測試、綜合或硬體，未改狀態與 Git，未聯網。來源版本和完整70列結果的只讀摘錄見 [evidence.json](evidence.json)。

## 1. 證據層次與共同起點

本輪核對的是舊 `mechanism-construction-20261016` 產物：`model/execution-receipt.json` 記載一次主批次、70 simulations、82 checks、passed；本席核對其 contract、runner、results、trace、verification 等已記錄 SHA-256 與當前位元組全部相符，但沒有重新執行驗證器。舊結果是有限條件模型，非原生 RTL／MIG 時間。舊程式與凍結合同是模型假設的直接證據，不能作為假設在平台中成立的證據。

共同工作為 V、A 各4096B COPY，payload R=1024 beats、W=1024 beats；cold另有三筆1-beat PTE。共同前綴為 V兩筆256-beat source R加A一筆256-beat source R，時域[-768,0)，逐事件計帳，並非免付費初始狀態。所有策略保留既有兩個4096B fullbuffer，模型新增payload storage=0；物理保留量只是共同條件輸入，非測得記憶體成本。suffix的四個W owner固定V0→A0→V1→A1，各256 beats，各一個B，B在WLAST後2單位返回。完整完成要兩job成功且R/W/B與票據全部排空。

直接證據：`design/model-contract.json:30-104`；`model/runner.py:51-62,165-195,321-349`。程式前綴768與L的256／128長度實際寫死，不能只改合同中的`L_beats`便聲稱驗證一般長度公式（runner:53,61-62）。

來源事實、條件輸入分開：合同 `origin.source_facts`（14-20行）指向舊source核讀；`conditional_not_observed`（22-26行）明列及時intent、cold/hot相同selector歷史、特定PTW gaps與job readiness共現、serial service law均非已觀察。模型只給一個serial R/W data resource、read burst不可搶佔、每beat一單位、AR FIFO；實際MIG不因這些數字而具有同樣時序。hot/cold前綴僅在聲明的selector觀測投影相同，不保證CPU、完整軟體或全系統歷史相同。也未證普通提前prepare不能在更早起點合法做完。

## 2. 正式已執行數值與guard

以下均為**已存正式模型結果**，full-start quiet=suffix quiet+768；V/A完成也是最後各自B完成。列中的數值不代表硬體cycles。

| case | O_CURRENT quiet | M_TICKET quiet | O_SPLIT128 a=0 quiet | G=2 quiet | O_CURRENT−M |
|---|---:|---:|---:|---:|---:|
| positive_cold | 2066 | 2060 | 2058 | 2060 | +6 |
| paired_hot | 2050 | 2050 | 2050 | 2052 | 0 |
| adjacent_short_gap（tau1=1） | 2059 | 2060 | 2058 | 2060 | −1 |
| holdout_longer_gap（d=3,tau1=12） | 2070 | 2061 | 2058 | 2072 | +9 |
| holdout_late_intent（arrival=1） | 2066 | 2066 | 2058 | 2060 | 0，missed |

精確結果入口：舊 `model/results.json` cold O/M/split為7／43／381行；hot為533／565／867行；short為1003／1039／1377行；long為1529／1565／1903行；late為2055／2091／2429行。evidence.json保留每列suffix/full-start、PTE、L、V/A、AR/AW/B和bytes。

完整guard向量如下，每一行是一個跨所有case相同G的普通程序；不能逐case選最好值再冒充同一policy。

| G | cold | hot | short | long | late |
|---|---:|---:|---:|---:|---:|
| 0 | 2066 | 2050 | 2059 | 2070 | 2066 |
| 1 | 2067 | 2051 | 2060 | 2071 | 2067 |
| 2 | 2060 | 2052 | 2060 | 2072 | 2060 |
| 3 | 2060 | 2053 | 2060 | 2061 | 2060 |
| 4 | 2061 | 2054 | 2060 | 2061 | 2061 |
| 8 | 2065 | 2054 | 2058 | 2065 | 2065 |
| 16 | 2066 | 2054 | 2061 | 2072 | 2066 |

G=2在主cold追平M且hot慢2；G=3也主cold追平，但hot慢3。G=8在short比M快2。大G的hot不線性：hot_write_ready=4時，空read FIFO允許V0先服務，故G=8/16的L R完成均為516，而quiet同為2054（runner:161-175；results相應guard列）。這是已執行的相位改變例子，禁止把每個門延遲都直加最終時間。

主cold服務向量（L/P為suffix，V/A為full-start）：O_CURRENT=(256;[257,266,268];1810,2066)，M=(259;[3,260,262];1804,2060)，split0=(257;[129,258,260];1802,2058)。所以split0在該例的L/P1沒有全部優於M，但V/A完整完成各快2且hot完整完成追平。不可寫split0逐項支配包括P1 latency在內的所有指標；也不可只看P1快便稱M全任務勝。

## 3. 同付費接口等價與模型實際包含什麼

`O_SAME_TICKET`是**同新增付費接口能力控制**，不是具名已發表舊方法。合同166-168行要求普通if/else使用同intent、marker、timeout、fairness，逐事件等於M；程式77行把M與O_SAME_TICKET送入同一`ticket_policy`分支，101-110及142-145行共用close/release規則。381-383行對五case整份trace作精確相等檢查。因此舊結果證明的是這一實作的標籤不產生額外調度差異，並支持「同接口普通邏輯足以表達」；不是兩份獨立程序／RTL實作的交叉驗證，更不構成獨立新調度演算法證據。

模型M使用明確字串marker=P1，首次cold intent在t=0即close，P1 t=2接受便release；下一邊界t=3發布L；P2/P3僅按普通現有VALID／FIFO處理。它不是新M-ID版本的源碼驗證：沒有真實ID寬度、舊同ID義務清空、七路DS marker傳播、spill／held狀態的實現。模型也沒有執行真正held背壓、缺目標／timeout、reset/epoch、多walk公平逃逸、協定錯誤／取消注入（舊verification.json的`protocol_not_executed`及runner:410-414）。`error=OKAY`與attributes是抽象標籤，checker驗相等及bytes coverage，沒有硬體錯誤聚合演算法驗證。有限四B drain不可升格為一般活性／AXI正確性證明。

普通O_CURRENT只用當前DS head與不可撤回既有offers；不准予hidden hot/cold bit或未可見PTW start。這是所選控制邊界的合同，不能反過來限制所有已發表設計或普通有合法prepare接口的實現。split則是普通paid read-only128構造，非當前reg64已存在且免費的runtime switch；具體本地legalizer適配與可復用資源由另一源碼席核對，本席不重做。

## 4. paced-split發布時刻與成本公式的確切含義

主cold舊trace（`model/traces.json`鍵`positive_cold/O_SPLIT_128_PACED_a0`及`a4`）與runner:80-160吻合：

- t=0 L0 AR並服務[0,128)，P1在t=2已AR入FIFO。
- t=128 L0 RLAST；**L1在同一t=128即AR發布／接受**。P1早已接受，所以FIFO先服務P1，並非L1等P1 RLAST才發布。
- a=0時P1服務[128,129)，L1服務[129,257)，P2 t=137 AR、[257,258)服務，P3 [259,260)；V0 ready=264，最終suffix quiet=1290。
- a=4時L1 AR仍128，setup獨佔共享serial data資源[128,132)，連早已接受P1也延後到[132,133)服務。L1 [133,261)，P2 t=141 AR並[261,262)服務，P3 [263,264)，V0 ready=268；suffix quiet=1294。setup不是僅L1 header處理延遲，也不是一個額外AR天然要付的固定4cycle。

合同 `cost`（183行）已將a_split公開定義為該位置的額外**暴露serial delay**；runner在first_done==t時、讀FIFO服務之前插入setup（155-160行）。在已執行的a∈{0,1,2,4}上，cold full quiet=2058+a，hot=2050+a；M為2060／2050。這是舊實行數值＋程式定位，不是本地legalizer的實測費用。

**條件算術，非新模型執行：**若h_e是M仍及時保住首PTE窗口後的額外暴露critical-path費用，且上述事件次序、共同前綴、成功完成與時鐘尺度不變，M主cold勝split須`2060+h_e < 2058+a_split`，即`a_split>2+h_e`。hot若以同樣全程附加h_e算，則M勝須`a_split>h_e`；若h_e只在cold ticket活躍時出現，hot費用須另列，不能默認也加。主cold界線僅比較這兩個已指定程序及費用注入位置，不是某一面積／bit-count門檻。

何時不能套常數：

1. **intent到達相位改變**：舊正例在t=0 gate之前尚無L，晚1單位就已有L offer，latecase直接回到2066，不能寫成2060+1。真實到達slack／CDC與hold影響未知。
2. **split費用只拖L1 publication或只占legalizer本地資源**：P1可仍於[128,129)服務；一部分額外AR生成延遲能被該PTE或其他工作隱藏。若L1發布拖到P2可見的t=137附近／之後，P2可能先AR入FIFO，整個依賴次序變了，不能沿用舊setup把P1也擋住的斜率1。
3. **長度／gaps／readiness跨界**：P2不再被L1遮住、H先timeout、現有ready write先起跑、或新read已不可撤回時，應保留max／不可搶佔分段關係。上節大G hot正是已存反例。
4. **新cost是Fmax下降或共同前綴增長**：它可能改變所有beats及前綴768的時間，不能當suffix末尾一個常數；area、routing、CPU和metadata維持各自量綱，不轉成猜定cycles。

同理，若a_split按最終暴露時間的實測差額定義，它可用於事後盈虧核帳；但尚未測得時不能從「多一個AR」或「有split state」推出a_split>2。隱藏開銷允許a_split=0。

2053是另一層：舊results末尾明標`O_PREP_EARLY`與`O_SEGMENT`均`manual_conditional_optimistic_construction`，不是70次中另有已跑案例。算術2051=1024R+3PTE+1024W，再加末尾B latency2；合法prepare需更早知道目的地址、權限／生命期及現有接口，fine segmentation也需具體控制／額外AR/AW/B成本。故2053只保留為手算的普通服務機會／條件樂觀構造，不能說已測普通吞吐或任意系統下界達成。

## 5. 最近前作：原文實際能力、成本與未核到的差項

三份本地PDF存在且hash與catalog一致，無須網路。以下是本席直接核過的已存原文文本，非把summary當全文證據；沒有新增使用者人讀標記。頁數為PDF實際頁；iDMA另外給印刷頁。不能因沒有同名票據接口的記述便宣稱首創。

| 具名已發表／公開工作 | 原文能力與實際資訊、控制點 | 原文明列代價與本輪邊界 |
|---|---|---|
| MASK，ASPLOS2018，§5.4 p8，§7.4 p12 | DRAM controller內Golden小FIFO專收translation並高於Silver，再高於Normal；Silver給一個app資料請求配額，依每app concurrent walks×stalled warps計算，Silver/Normal內FR-FCFS。依賴已進系統translation request分類／page-walk depth與app metrics，不等於本例不可改accepted FIFO的selector接口。 | 6-bit per-app concurrent-walk counter與每TLB MSHR 6-bit stalled-warp counter；3-bit depth metadata。每channel Golden16、Silver64、Normal192 entries；作者報DRAM request queue storage +6%。這些不是M-ID成本。定向章節未提供本例「PTW accepted-start至尚未發出P1之間」一次首PTE gate或同ID排空合同；也未核到針對AXI RLAST/B的分段。 |
| AXI-REALM，arXiv2501.10161v2（2025-07-07），§III-A pp3–5，§IV-B p7 | 入口irealm以AXI地址／長度／flags、region與period/budget控制；burst splitter保留meta、更新child地址；fragmented B透明合併，read data直通，僅按原transaction長度gate R.last。對不支持的atomic／某些短non-modifiable transaction明確拒絕轉error subordinate。獨立write buffer收完整fragment後才放AW/W；週期補充逐beat預算。 | write buffer容量兩AW＋一個write fragment；大fragment需要較大buffer，splitter可關以省面積。作者§IV-B說irealm bypass零額外cycle，active經write buffer多1cycle；erealm零額外latency。這不支持「額外AR一定形成正泡」。本文成本模型／ASIC評估不等於本平台增量a_split或M cost。定向章節未核到早PTW-start資訊、首PTE身份／一次票據；透明split與返回組織已有前史。 |
| iDMA，IEEE TC 73(1),2024，§II-C PDF pp4–5／印刷pp266–267；§IV-C/D PDF p10／p272 | backend legalizer保存1D transfer資訊，依協定、頁邊界、最大burst及user burst limits產生合法child；source/destination要求共同考慮。read/write與legalizer以FIFO解耦，dataflow採generic byte streams；小FIFO可替換成存整transfer的SRAM；read manager收地址、長度、配置，回read-aligned stream。optional error handler可continue/abort/replay，錯誤回報offending legalized burst base。 | paper backend首request latency2cycles，無legalizer為1cycle，這是transfer起動而非每child固定泡；fine-grain吞吐可用outstanding隱藏延遲，代價是更多decoupling buffers，作者某base配置每新增stage約400GE，不能移植為本地比值。paper足以支持復用既有分段／metadata／manager與fullbuffer機會，尚未證本地當前frontend支持獨立read128或paced RLAST策略；實際改動由源碼席判。 |

原文精確入口（路徑均相對workspace根）：

- MASK：`iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt:867-879,881-932,1230-1235`；最後一組給queue容量與6%，不是全MASK面積；§7.5使用CACTI，不能混成FPGA綜合。
- AXI-REALM：`iommu-platform/out/rule-novelty-check-20261007/papers/axi-realm2025.txt:20`定v2日期；182-188控制位置；287-304 metadata、B、RLAST與合法性；328-351寫buffer；413-439對稱R/W budget；604-616 latency。
- iDMA：`iommu-platform/out/paper-driven-innovation-20261008/papers/idma2024.txt:250-295,325-347,661-674,697-718`。原件題頭為TC73(1) January2024（txt:1），不以summary的unknown venue覆蓋原件。

論文說明足以證明相關原語／組織已存在，但不代替本地RTL的差分成本核對；尤其**DMA backend legalizer並非必然需要新增一套post-TL parent R reassembly**。若既有manager／generic stream／fullbuffer已承擔相應職責，普通適配要付其真實新增差額，不能把另一架構的全部return join狀態重複收費。反之，paper generic能力也不保證本地任何desired pacing已免費實作。

## 6. 可以接受與不能接受的原創性表述

| 分類 | 證據支持的裁決 |
|---|---|
| 具名前作覆蓋的構件 | PTE/data priority、app配額、透明合法分段、地址／長度meta持有、B合併／原RLAST、read/write decoupling、可選fullbuffer均有具體原文前史；不能認領這些原語首創。 |
| 獨立普通構造 | O_GUARD、paced read2×128、optimistic prepare／segment為本研究構造／對照；即使可從既有能力自然組成，也不冒充曾已發表同程序。split0的2058/2050正式結果已足以否決「只有早intent才能改善完整COPY」；2053保持手算標籤。 |
| 同付費接口能力控制 | O_SAME_TICKET可表達並逐事件相同，限制獨立新調度主張；不能將候選同接口克隆算成已發表舊方法，亦不能單靠等價否定接口的工程／組織價值。 |
| 仍可條件研究的差項 | 保持原burst與return path，用實際早PTW啟動資訊保護一次首PTE順序，若對具體M-ID與復用legalizer O的**完整新增成本**有可證優勢，仍有待判內容。當前只有條件盈虧與實作接口問題，沒有已證獨特性、native收益或成本優勢。 |
| 缺證據、不作結論 | 三篇是否全面包含等價早票據、所有文獻是否已覆蓋、實際a_split/h_e及area/Fmax/route/CPU、prefix自然可達與發生率，均未由本席解決。不能據本次定向核讀作全面新穎性通過或整方向否決。 |

核讀品質：三篇均以本地primary text直接核能力，既有PDF hash有版本綁定；本席未另做venue／COI／撤稿／窮盡性審計，也無獨立論文重現，不據此給「已全面查新」或「無風險」標章。交付是root六段主報告的證據輸入，不新增後繼輪次或恢復暫停實驗。
