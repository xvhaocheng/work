# 完整E3：有界接口與觀測容量提案

Material Passport：2026-10-17，ARS experiment-agent/plan；Codex-only；`DESIGN_ACCEPTED_NOT_IMPLEMENTED`。
使用者已授權論文負載的必要RTL/Linux獨立分支改動與構建，Root要求接口先審、再分批凍結。
本頁沒有實作ABI5／BTC3／observer5，沒有改已完成P0共同包，也沒有新板測。
P0仍是原V64/1024/4096、A4096、V1頁、A1/8頁、4096+256，45點不改。

## 固定來源與新觀測證據

候選基座是已交付P0 CVA6 `8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8`、
Linux P0+HPM+multi `acece3a3ab7d72ba1f936c1984b82a4a55f28d46`，IOMMU仍`85b157e`。
後續Root整合A3的E4與A1的E5已審提交後再凍結共同軟體，不改目前實際build archive。
Root已接受本有界方案；觀測整合基座指定已審E5 `6c5c666`，BTC3 frontend先交diff與有限證據再審，未審不建新BIT。
ABI5預留ioctl0x0f/0x10；不占用ABI4的0x0d/0x0e或E5既有observer控制口。
全文實際檔案、固定來源SHA、精確行範圍與原文摘錄見[full-source-evidence.json](full-source-evidence.json)。
RTL的CVA6 tracked檔來自上述獨立工作樹；IOMMU/vendor使用同commit的已完成P0編譯archive；
Linux來自最終acece3a工作樹與實際共同kernel配置。

[probe_long_backend.py](probe_long_backend.py)復用A1 run03完整凍結backend與memory/guard模型，
加入原生`dma_path_stage` Depth4／16兩組被動實例；DUT/backend／生產observer原碼都沒有修改。
R首回覆延至400cycle，B至12000cycle，地址ready合法、資料及目的外guard全驗證。
這是backend與實際stage3 scoreboard的有限實驗，不是全IOMMU或板測。

| 單COPY | 實際AR/AW | 峰值AR未返/AW未返 | 原Depth4 | 同事件流Depth16 |
| --- | --- | --- | --- | --- |
| 4096B對照 | 2/2 | 2/2 | 無error | full=2/2、無error |
| 16384B | 8/8 | 8/8 | 讀寫sticky error | full=8/8、無error |
| 65536B | 32/32 | 8/11 | 讀寫sticky error | full=32/32、無error |
| 65536B、S/D offset64 | 33/33 | 8/11 | 讀寫sticky error | full=33/33、無error |

原B=800cycle探針AW峰值是9；延長合法背壓後成11，證明不能將NumAxInFlight=8或一次實測峰值當上界。
最新四情境原始[manifest](test-results/long-backend04.json)、[summary](test-results/long-backend04-summary.json)、
[compact握手事件](test-results/long-backend04-events.jsonl)、[獨立逐beat回算](test-results/long-backend04-audit.json)，
實際[wrapper](source/long_backend04_backend_top.sv)與[C++](source/long_backend04_backend_test.cpp)均保存。
原完整逐beat事件保留在manifest所指的本機artifact並保存SHA。
這些Depth16資料不標成E5 v4或原4KiB基座資料；Depth16只是對照，以下首選Depth64。

## 容量的descriptor級硬界：選Depth64

令每個DMA／方向的`P=private accepted AR/AW − private returned RLAST/B`。
證明前提限定本API：DEFAULT COPY、單一private ID=0、相同S/D相對offset、page-aligned S/D基址、
每個COPY長度≤65536且64B對齊；不允許reduce_len、更小burst上限、原生2D midend或多COPY並行。

1. **同lane最多一個COPY。** `idma_reg64_frontend.sv:336–346`的COUNT只發一個job，握手後立即WAIT；
   `357–368`只有`trans_complete_i`才回COUNT。`dma_core_wrap.sv:123–181`選IsTwoD=0，
   job FIFO雖Depth8，producer在WAIT不填第二項；backend rsp_ready固定1，completion接`idma_rsp_valid`。
   Tile也保持這個狀態機，每一row就是一個串行COPY，不改成一個tile同時掛64個row。
2. **每個COPY每方向最多33 bursts。** `idma_pkg.sv:107–110` AXI最多256beats；
   `idma_legalizer.sv:75–85` DataWidth64使page=min(4096,256×8)=2048。
   `139–145/169–175`在src/dst_reduce_len=0時使用該2048邊界。
   frontend `194–215`先將options清0，但`238–244`會用CONF.deburst覆蓋reduce_len。
   實際driver `idma.c:350–357`固定寫CONF=0；新BTC3的batch_cfg_ok必須額外要求
   `conf.deburst==0 && conf.decouple==0`，且active配置鎖保留，不能只憑初始options=0證明。
   因S/D基址均4KiB對齊且offset相同，兩側的2048邊界一致，不會因不同對齊交錯再拆小塊。
   `B=ceil(((address mod 2048)+length)/2048)≤33`；完全對齊的64KiB為32。
   33包括尚未ready的最後offer，不是33以外還另加一整個COPY。
3. **COPY completion不留下上一COPY的返回。** DEFAULT的backend完成由最後write-burst B返回產生：
   `idma_backend.sv:519–531`的`w_dp_rsp_valid & w_last_burst`，
   `idma_axi_transport_layer.sv:447–455`由private B valid/ready直接驅動；
   最後B前必須已完成該COPY所有W，DEFAULT W只消耗該COPY的R資料（同檔`254–278`與write datapath）。
   相同ID的AXI B/R有序，故此前的B及生成W所需R已被private端接收。
   下一COPY只在這個完成後開始，不會在P中疊加上一COPY。
4. **SoC返回buffer已被P涵蓋，不另加burst來源。** 請按以下真實方向讀源碼：

   ```text
   private AR/AW → DMA mux spill → IOMMU TR FIFO/translation → COMP request tap → SoC xbar/memory
   private R/B  ← DMA mux return ← IOMMU response demux      ← COMP return tap  ← SoC R/B buffers
   ```

   `ariane_peripherals_xilinx.sv:914–917`是private接受／返回；`938–946`是PATH的TR、COMP及返回tap。
   `1131–1132`將COMP接`iommu_comp`，top `ariane_xilinx.sv:287–307`的CUT_ALL_PORTS／PipelineStages1
   在SoC crossbar中包含返回寄存；其buffer位於COMP tap的記憶體側，會延遲返回但不能產生新burst。
   IOMMU `rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:113–156`先接受TR入FIFO，
   `319–322`將正常返回送回TR（經其response demux）；故COMP返回tap不晚於對應private返回。
   DMA mux `axi_mux.sv:46–54`為SpillAr/Aw=1、SpillR/B=0，且peripheral `990–1014`未覆寫。
   即使未來返回路加buffer，只要保留該方向與COPY completion條件，P仍包含所有尚未private返還的burst。
5. **每stage界。** stage1/2/3 pending都是已private接受且尚未private返回之burst的子集，≤P≤33。
   stage0 enter為首次offer，最多再有一個尚未private接受的held request；保守界≤P+1≤34。
   `dma_path_observer.sv:132–145`的held-valid只計一次，`stage`按V/A與read/write分開，不把多DMA加總在一個FIFO。
   開窗carry及SoC返回buffer不新增COPY或burst身份；ARM仍先drain/idle再開始，關窗不清掉錯誤。

因此統一64是高於34的power-of-two容量界，無需把8/11的實測峰值當完整共享路徑證明。
若後續新增COPY重疊、獨立S/D pitch、reduce_len或更大COPY，此證明立即失效，不能沿用Depth64資格。
實作前仍需定向有限測試覆蓋最大對齊／非2048對齊、FIFO wrap、carry/open/close、STOP與所有stage，
並核對加入被動容量前後DMA/AXI trace不變；這不是以有限樣本取代上述硬界。

容量改動只給完整E3專用參數`PathQueueDepth=64`，預設4保留A1已審E5原基座。
16個FIFO的timestamp+born儲存從2112增到33792bit，增加31680bit；實際FF/LUT／時序必須看獨立實作。
Root已定稿獨立observer MAGIC OBS5=`0x49444d414f425305`，HIST_INFO格式2、INFO[55:48]=實際64，
其餘欄位與A1 v4直方圖兼容；不修改已審E5 OBS4/default4／格式1。
新ABI5要求OBS5+depth64，不能以OBS3/4或深度0靜默通過。原PATH錯誤、queue overflow與直方圖失效全保留。

## 形態與每個操作的實際COPY

最大每buffer **1MiB/256頁**，不改CMA或MAX_ORDER。原16MiB canonical只保留為歷史紙面設計：
凍結config `# CONFIG_CMA is not set`，`include/linux/mmzone.h:28–33`預設MAX_ORDER10，
`idma_stream_prepare():303–310`強制連續分配；`dma-iommu.c:1493–1504`不走分散remap，
`1458–1461`經dma_alloc_contiguous失敗後alloc_pages(get_order)，order12超過4MiB上限。
1MiB是order8候選，不保證碎片下成功；分配失敗正常報錯停止，不重試或更改kernel配置。
每lane source/destination分開各自分配同大小，所有頁仍4KiB映射。

| 形態 | 實際地址序列／工作集 | 典型點與控制 |
| --- | --- | --- |
| NIC頁內環 | 8頁；page內連續slot，剩餘不足下一COPY則跳下一頁，環尾回0。每op一個COPY。64/256/1024/4096與額外1536B可用 | 1536B每頁2slot，padding不寫；16/64KiB只作長COPY幾何控制，改用linear ring，頁數max(8,ceil(L/4096))，不稱NIC封包 |
| Storage順序環 | 各1MiB，offset=i×L mod span，每op一個真L-byte COPY；16/64KiB真跨4/16頁 | 六種size都可作大小控制；小size固定N時未必走完整環 |
| Accelerator tiled rows | 各1MiB，64 matrix rows，pitch16KiB；row_bytes=min(op_bytes,1024)，rows/op=op_bytes/row_bytes；column起點先掃，再掃row group | 16KiB=16×1024B。64KiB=64×1024B。row串行硬體供給；64/256/1024B是一row退化控制，地址順序恰與順序環相同，明示不當獨立形態證據 |

Tile的第k個row COPY：`op=k/rows_per_op, row=k%rows_per_op`；
`col=op%columns, group=op/columns`（均先對完整ring周期取模）；
`offset=(group×rows_per_op+row)×pitch+col×row_bytes`，S/D相同offset。
沒有CPU在測量中逐row補發，也不啟用現有未驗證的IsTwoD=1。

## N、實際頁集合與預熱：兩個候選protocol不能混算

[model_full_coverage.py](model_full_coverage.py)與[49份規劃結果](full-planned-coverage.json)
用獨立數學地址公式列每shape×size的測量／warmup頁集合、每頁descriptor觸及次數、有效bytes、
row COPY數、完整環次數與尾部；全部標planned_address_model，不是RTL或板測。
包括固定4096/256、等16MiB/一完整V環預熱兩案，及P1最小環、1536B邊界。

- 固定案只保留為對照設計：V4096 logical ops、256 logical warmup，必須明列活躍足跡。
  例如Storage64只傳256KiB、只訪問64/256頁、沒有走完整1MiB環；warmup只4頁。
  NIC64測量訪問8頁但warmup只4頁。不能將mapped_pages當實測活躍頁或稱fully warmed IOTLB。
- Root選定新研究預設等byte案：V operations=16MiB/op_bytes，V warmup operations=一個完整幾何環的op數。
  各storage/tile點測量都完成16個1MiB環；copy數依row_bytes改變，最大262144。
  V一環預熱是地址覆蓋聲明，不是「大於IOTLB容量的工作集全部常駐」聲明；P1最小環也使用此新協議。
  A仍按既有joint warmup持續供給，不能假設跟V等量／走完一環。
- 額外NIC1536B只作獨立邊界能力／correctness點；1536不整除16MiB，不能假稱其4096-op=6MiB對照是同等byte協議。
  六size主軸全為2的冪次可精確16MiB；1536實板若後續需要，另列真實ops與bytes，不自動加入150點。
- A實際warmup／測量row COPY數、byte數由driver返回；測量含startup+STOP後最後row排空。
  其真正頁集合由從0起的實際完成prefix計算，不使用V的4096或映射頁數代填。
  STOP可落在tile中間，報full_ops=floor(rows_done/rows_per_op)與partial_rows，不補完虛構tile。
- 兩protocol與P0不混池；跨size的時間不同工作量不能當干擾比較，慢化只在同shape/size/protocol/V幾何內對Solo。
  異BIT的舊P0不默認是新版本重複樣本；需要比較或重用時保留各自實際身份，不自動重測舊45點。

## ABI5草案：caps64B、run544B

完整[草案header](source/proposed_idma_workload.h)只在exp，沒有寫入正式Linux。
[host執行＋RV64靜態布局回執](test-results/workload-draft-layout01.json)已核實大小與offset，沒有實作driver能力。
CAPS=0x8040690f，RUN=0xc2206910；ABI1/2/3/4 opcode與layout原樣。

| run區塊 | byte offset／大小 | 合同 |
| --- | --- | --- |
| ABI/size/flags/Aid、timeout/supply/reserved | 0／32 | V0，A none或DMA1；supply=1；flags/reserved全0；timeout1..60000ms |
| V logical ops、V warmup ops、reserved | 32／32 | ops>0，展開descriptor target≤262144；warmup展開≤16384；checked乘法 |
| V/A幾何 | 64、128／各64 | geometry、row_bytes、rows/op、pitch、pages、matrix_rows、columns；reserved全0；Solo A幾何全0 |
| status/HPM/完整窗／DID／seq／軟體時長 | 192／64 | 同現有成功／失敗與非原子HPM口徑 |
| 舊stats V/A | 256、352／各96 | completed/submitted是row COPY；bytes=actual_completed×row_bytes；原data/quarantine語義 |
| V/A真warmup descriptor/bytes | 448／32 | 包含最後排空row；不是假設的A warmup工作量 |
| V/A coverage | 480、512／各32 | measured distinct bytes/pages、warmup distinct pages、verified union bytes；reserved0 |

caps明列幾何mask、row length mask、pages上限256、rows/op上限64、operation bytes上限65536、
batch/observer magic與實際PATH depth、timeout上限；這不是任意參數組合授權。
Palette為64/256/1024/1536/4096/16384/65536；1536限page-packed NIC；tile只用上述六軸固定分解。
Linear要求span整除COPY；page-packed只≤4096；tile要求固定pitch16384、matrix_rows64、
columns=pitch/row_bytes、pages256、rows/op整除64且op_bytes屬六軸。
Ring型pages只允許1..256的power-of-two並須滿足上述長度約束；論文canonical使用表中8/16/256頁，
P1最小環使用1/4/16頁。caps的max_pages不是任意非整除span或任意tile布局的許可。
所有offset／offset+row_bytes／pages×4096／ops×rows／count×row_bytes使用checked u64計算，
實際配置不可超出映射，不能從user傳任意IOVA或KVA。

## BTC3與最小RTL接口

候選magic BTC3=`0x49444d4142544303`；舊kernel明確拒絕。
新driver舊ABI1 batch仍只4096；P0原三len和multi/role-swap原合同保留，BTC1/2不做新寄存器存取。
BTC3的CTRL=0除原狀態外，將新幾何回到legacy linear/rows1；防止前一個ABI5遺留tile污染舊操作。

原0x80/88/90/98/a0/a8與NUM_BYTES含義保持；0x90/issued/completed仍是row COPY descriptor數。
新增候選bank：b0 geometry、b8 rows/op、c0 pitch、c8 matrix_rows、d0 columns，
其餘d8..f8保持reserved/read0。NUM_BYTES始終是真實每row長度，單COPY長度≤64KiB。
Tile新增row/column/group cursor，row之間加pitch，下一column起點加row_bytes，row group到環尾回0。
只在原COUNT握手後推進cursor；WAIT/STOP時穩定，STOP排空當前offered row，不取消AXI、不偷偷補完tile。
所有配置只在IDLE且全8 byte strobe時寫；active寫配置置error，只有STOP例外；invalid geometry/溢出不發job。
BTC3額外硬驗CONF.deburst=decouple=0；DEFAULT/相同S/D geometry/page-aligned base的原條件仍保留。

## 驗證及分批實作順序

1. 先審本有界接口與Depth64論證；frontend小批只改BTC3長度／幾何／cursor，
   保留COUNT/WAIT單COPY，舊4KiB/P0/nonbatch逐週期gold對照；實際DUT驗offset/len/回環/STOP中途tile／active鎖。
2. 被動觀測小批單獨引StageDepth參數（default4）與E3 explicit64；按Root協調整合已審E5，
   不改其原Depth4實驗基座。所有stage/role/channel、FIFO wrap、carry、timer/overflow與ready/valid trace對照。
3. Driver新ABI adapter共用原mapping/cache/全域鎖/HPM/STOP/drain/quarantine；不複製一套生命週期。
   新幾何在warmup及batch使用同一地址合同。每buffer的精確64B粒度bitmap可表達所有64B對齊COPY與NIC padding，
   最大每張2KiB；warmup/measured各建集合並union。全源完整驗證、目的觸及／未觸及字節與hole驗證保留。
   若A已繞環很多次，只算完整周期＋prefix，不按數十億完成逐條迴圈。舊prefix驗證不改口徑。
4. CLI/collector記錄logical ops、row descriptors、真bytes、mapped與actual頁集合、partial A tile；
   V expected AXI按真地址與長度計，還要backend有限實測每種邊界，不能用2×descriptor。
   shared AR/AW仍對PATH0 leave，private守恆與16 PATH原檢查不刪；fixture發布拒絕沿用已接受邊界。
5. 各實際組件在獨立branch/worktree提交並做bundle，Root審後凍結獨立50MHz BIT／共同payload。
   WNS<0或任一端點／routing失敗即停，不改約束。當前已交P0包完全不變，上板仍Root串行排隊。

## 論文子集與板時

完整六size×九V/A形態＋Solo仍342 cells×5=1710，不排成≤6h的承諾。
優先候選為原P1六size×Solo/A1/A8=90與canonical3×3＋Solo=60，共150；
canonical採NIC256B、storage64KiB、tile16KiB，上述1MiB候選取代歷史16MiB。
150個新點使用等16MiB／一完整V環預熱；既有P045獨立保留，合計195次不能混稱150次。
不以舊P0加新三size拼成同一等byte protocol的六size切片；若須縮減板時，先減尚未開始的新canonical條件。
Root選定的等byte案是新protocol，不與原P045去重或跨BIT合池；150新點仍只是候選，按實際耗時鎖定≤6h。

既有DS0 50次原測包含UART約15.70秒/點；150點按20秒/點＋30分鐘約80分鐘，
這只是已有4KiB的實測外推，不是新1MiB allocation／verify／64KiB的實測承諾。
保留單點kernel60/外層90/host120秒、首次失敗停、不自動補測。
新配置首批成功後按實際wall秒數更新估算；整輪deadline6h，啟動下一點前預留120秒，
不足時縮減尚未開始的canonical組合，不能降低資料／排空／PATH檢查。

## 固定4096/256案的規劃覆蓋摘要（不是實測）

| 形態 | op bytes | row bytes×rows | V COPY數 | V payload KiB | mapped/測量/warmup頁 | 完整環次數 |
| --- | --- | --- | --- | --- | --- | --- |
| nic | 64 | 64×1 | 4096 | 256 | 8/8/4 | 8 |
| nic | 256 | 256×1 | 4096 | 1024 | 8/8/8 | 32 |
| nic | 1024 | 1024×1 | 4096 | 4096 | 8/8/8 | 128 |
| nic | 4096 | 4096×1 | 4096 | 16384 | 8/8/8 | 512 |
| nic | 16384 | 16384×1 | 4096 | 65536 | 8/8/8 | 2048 |
| nic | 65536 | 65536×1 | 4096 | 262144 | 16/16/16 | 4096 |
| nic | 1536 | 1536×1 | 4096 | 6144 | 8/8/8 | 256 |
| storage | 64 | 64×1 | 4096 | 256 | 256/64/4 | 0 |
| storage | 256 | 256×1 | 4096 | 1024 | 256/256/16 | 1 |
| storage | 1024 | 1024×1 | 4096 | 4096 | 256/256/64 | 4 |
| storage | 4096 | 4096×1 | 4096 | 16384 | 256/256/256 | 16 |
| storage | 16384 | 16384×1 | 4096 | 65536 | 256/256/256 | 64 |
| storage | 65536 | 65536×1 | 4096 | 262144 | 256/256/256 | 256 |
| tile | 64 | 64×1 | 4096 | 256 | 256/64/4 | 0 |
| tile | 256 | 256×1 | 4096 | 1024 | 256/256/16 | 1 |
| tile | 1024 | 1024×1 | 4096 | 4096 | 256/256/64 | 4 |
| tile | 4096 | 1024×4 | 16384 | 16384 | 256/256/256 | 16 |
| tile | 16384 | 1024×16 | 65536 | 65536 | 256/256/256 | 64 |
| tile | 65536 | 1024×64 | 262144 | 262144 | 256/256/256 | 256 |

頁集合與每頁COPY觸及次數見完整JSON；NIC1536雖觸及8頁，實寫distinct bytes只有24576，padding不算有效足跡。
