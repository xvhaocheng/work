# E3 負載形態：條件矩陣與待執行包

> 此頁保留最初完整矩陣設計與原pathv3限制。後續僅P0修改、構建和45點已獲批准，
> 新實作／採集入口與當前實際狀態見 [p0-implementation.md](p0-implementation.md)。其餘矩陣仍僅規劃。

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run（使用者指定準備與實驗；依 change-request 停止執行）
- Origin Date: 2026-10-17 JST
- Verification Status: UNVERIFIED（E3 尚無板測）
- Version Label: E3-workloads-preparation-v1

**狀態：需改動 driver/UAPI 與 batch RTL，依使用者規則停止；以下皆為規劃。**
實際能力見 [capability.md](capability.md)，具體請求見 [change-request.md](change-request.md)。
立即供Root審查的P0最小選項為 [p0-change-option.md](p0-change-option.md)；未實作。
未載入 BIT、未更新 Linux、未開啟 UART、未執行小子集，也未啟動長模擬。
下述完整矩陣與論文候選子集不是額外執行授權。

## Root 現在可執行什麼

**沒有符合45點P0的現有命令，也沒有只換使用者程式即可解除的缺口。**
現有pathv3 Linux/BIT不需要因本包而更新；目前唯一合法的對應batch點仍是雙方4KiB。
以下是既有介面命令入口，僅供Root已有實驗需要時使用，不是請求重測既有4KiB：

```sh
platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor 1 \
  --victim-pages 1 --aggressor-pages 8 --chunk 4096 \
  --transfers 4096 --warmup 256 --timeout-ms 60000
```

Solo改`--aggressor none --aggressor-pages 1`；A1改`--aggressor-pages 1`。
`--chunk 64`拿掉batch雖可通過既有軟體補發介面，A也會變64B；不同進程又被全域鎖串行，
因此不能替代P0。若日後需要同長軟體補發pilot，須獨立命名並明示CPU補發混淆。

本機有原CLI靜態重編譯：
`/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/host-check01/idma_experiment.pathv3`。
它沒有新能力，不建議為此單獨UART傳輸。未生成假裝支援新ABI的執行包。
若Root只需驗證重編譯，可沿通用skill上傳該檔後以`chmod +x`執行`--help`；本席未碰UART。

可直接執行的主機重現命令（新輸出目錄，不訪問板子）：

```sh
cd /home/xvhaocheng/work-a2-workloads
python3 paper/ispass2027/exp/E3-workloads/verify_capability.py \
  --output /tmp/e3-capability-NEW --cross-compile
python3 paper/ispass2027/exp/E3-workloads/verify_analysis.py \
  --records /home/xvhaocheng/work/paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle/records.json \
  --manifest /home/xvhaocheng/work/paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle/manifest.json \
  --output /tmp/e3-analysis-checks-NEW.json
```

## 基座與邊界

沿用 [baseline100 配套包](../../board/packages/baseline100-20261017/README.md)
及 [交付清單](../../board/delivery.json)：DS0 direct-wire、16-entry PLRU、50 MHz，
BIT SHA256 `e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6`；
pathv3 Linux release `6.5.0-rc2-ispass2027-pathv3`，共用 payload SHA256
`2de1e1551bc86c9651a39a46a0c4ceac7f630a593766109cadc6027102ceddd5`。
來源 commit、編譯命令、觀測與必要修復復用既有 manifest，不以活動研究 checkout 取代。
這些是能力核查輸入，並非本輪載入身份；若 change-request 獲准且實作，
新 driver/RTL 會產生新版本，須明確保留與此基座的差異，不能沿用上述 hash 冒充新能力。

## 三類幾何合同

S = {64, 256, 1024, 4096, 16384, 65536} bytes。
每一點皆單獨報告 V/A 的 descriptor payload、descriptor 數、有效 bytes、
source/destination footprint、步距及重複次數。copy 的有效 bytes 只計一次 payload；
讀寫總線流量不能直接當成兩倍有效吞吐。V0/A1 表示設備角色；條件 A1/A8 表示
干擾者每個 source/destination buffer 的頁數，兩者勿混淆。

| 形態 | 典型工作點與地址序列 | 可比較的大小軸與限制 |
| --- | --- | --- |
| NIC ring | 256B descriptor，N=8 個4KiB頁；小描述符依序填頁，再回環；source/destination 分開各 N 頁 | 64/256/1024B；另列1500B與1536B邊界。完整共同大小軸中的4/16/64KiB是幾何控制，超出典型封包範圍；放大 ring 至至少一個完整 descriptor，不稱巨型 descriptor 為 NIC 封包 |
| Storage sequential | 64KiB descriptor，source/destination 各4096頁（各16MiB）；依 payload 位移掃過整個 buffer 才回環 | 16/64KiB 典型；64B..4KiB 是相同順序幾何的大小控制。64KiB 必須是真正一個1D請求跨16頁，不能重標16個4KiB copy |
| Accelerator tile | 16KiB tile = 16列×1024B；row pitch=64KiB，source/destination 各16MiB。依 row-major tile 掃描矩陣；跨列以多個1D descriptor 合成 | 共同大小軸表示「每 tile 有效 bytes」：64B=1×64、256B=1×256、1KiB=1×1024、4KiB=4×1024、16KiB=16×1024、64KiB=64×1024。小 tile 的單列情況是退化控制；絕不把 tile bytes 當單一 descriptor bytes |

對非頁整除長度（1500/1536B），ring 在頁尾剩餘空間不足時跳到下一頁，保留 padding；
bytes 僅計 payload。2D 用相同 source/destination pitch，矩陣每列64個1KiB位置，
共256列；先掃列內 tile 起點，再掃下一組 tile rows，全部地址須落在各16MiB映射內。
當 tile 有多列時會改變1D descriptor數與供給次數，須同時報告每 tile 與每 descriptor 成本。
這些幾何目前均未接入現有 batch ABI，不把主機規劃描述成已實作流量。

## 完整矩陣（只設計）

先只定 DS0；DS50 如另行需要，作獨立附加延遲診斷，不能與 DS0 混樣。

1. V形態×A形態為 NIC/NIC、NIC/Storage、NIC/Tile、Storage/NIC、
   Storage/Storage、Storage/Tile、Tile/NIC、Tile/Storage、Tile/Tile 共9組。
2. 每組獨立掃 V size∈S 與 A size∈S：9×6×6=324個共跑條件。
   每個形態使用上表 canonical footprint；NIC footprint 至少容納一個 descriptor。
   足跡變化與超出典型應用的大小皆在標籤中明示。
3. V形態×V size 的 Solo 共3×6=18條件；不為每個 A size 重測同一 Solo。
4. 共342條件，各5次=1710次。完整DS0+DS50為3420次，未列入本次執行。

每個 V 條件的共跑時間除以同形態、size、footprint、有效工作、供給合同的 Solo。
canonical形態矩陣沒有 A1/A8 語義：64KiB 請求本來就跨至少16頁，不能叫 A1。
因此「相對 A1」僅對下一節的固定4KiB干擾者切片定義；其他組合填 N/A，
不能拿另一形態/大小/足跡的 A1 當作等量工作基準。

## 論文必要候選子集：DS0 150次，上限5.5小時

| 切片 | 條件 | 次數 | 用途 |
| --- | --- | --- | --- |
| P0：使用者先看量級的小子集 | V=64/1024/4096B，V source/destination各1頁；A=none/4KiB單頁環/4KiB八頁環 | 9×5=45 | 直接比較短 descriptor 的 A8/Solo 與 A8/A1；是此次唯一已授權上板切片，但被能力限制擋住 |
| P1：完整大小切片（包含P0） | V size∈S；V重複一個連續區塊，footprint=max(1,ceil(size/4096))頁；A仍為上述4KiB none/A1/A8 | 18×5=90，其中45次為P0 | 分開讀 size 與新增頁數；16/64KiB是重複單塊控制，不冒充大量頁 streaming |
| P2：典型形態交叉 | 上表3種 canonical V×3種 canonical A，加每種V的Solo | (9+3)×5=60 | 在8頁NIC環、16MiB順序及16MiB跨步幾何下檢查外推；其足跡/幾何與P1不同，無重複點 |

P1含P0，故合計90+60=150次，不能加成195次。其餘完整矩陣點保留待補。
P0/P1固定4096次 V descriptor、256次 warmup；V有效工作隨size變化，
只在同一size內算慢化，不以不同size的原始時間直接判斷干擾。
P2固定每路4096個邏輯操作；NIC/Storage每操作一個descriptor，Tile每操作16個descriptor，
256個邏輯操作預熱。須報告真實供給與完整完成，觀測器若按descriptor計數則為Tile 65536個，
不能強套既有 collector 的4096次/每copy兩個AXI交易假設。
A由硬體持續供給至V結束，再停止與排空；A不得提前用完工作。

所有重複在同一DS0配置內按5區組輪換條件；P0每區組包含9點，大小與A條件順序
按區組輪轉，記錄實際順序與boot。不要求逐點重啟，不新增往返切BIT。
相同區組中 V/A mapping、warmup、驗證及供給規則一致，正確性/排空失敗立即保留並停。

### 板時估算及上限

實測依據：[既有DS0中段manifest](../../board/runs/baseline100-20261017-01/ds0-middle/manifest.json)
從11:21:58.344到11:35:03.418 UTC完成50次，共785.074秒，平均15.7015秒/點。
此值包含UART/dmesg等開銷，遠大於單點硬體56–125ms；不可只以硬體ms估板時。
規劃採20秒/點，另預留30分鐘載入/傳輸/啟動；不含尚未授權的開發、建置與驗證時間。

| 範圍 | 常態預算（20秒/點＋30分鐘） | 硬性執行預算 |
| --- | --- | --- |
| P0 45次 | 約45分鐘 | 45×120秒＋30分鐘=2小時 |
| DS0論文子集150次 | 約80分鐘 | 150×120秒＋30分鐘=5.5小時，低於6小時 |
| 完整DS0 1710次 | 約10小時 | 超出6小時，不作此次排程 |
| DS0/DS50完整3420次 | 約20小時（各留30分鐘） | 不執行 |

120秒是每點完整操作（含採集）的上限；未來工具須配置DMA timeout60秒、
板端外層90秒、host120秒，整輪另有5.5小時上限。任何失敗停止、原樣保留；
host逾時不代表板端已停，先按skill收尾，無自動重試或補測。
若開發後的單點輸出/時間不能滿足此預算，就縮減尚未開始的P2，而非越過6小時。
目前沒有新版本，故此估算不構成已驗證的執行時長承諾。
既有50次只覆蓋最多64頁干擾者；P2的16MiB footprint之分配與全byte驗證成本尚未測，
不能把15.7秒/點外推成P2實測。執行前須保留每點120秒與整輪5.5小時上限，
若新版本的首批實際耗時超預算，只縮減尚未開始的P2，不減少正確性/PATH檢查。

## 板子互斥與實際本輪動作

本次協作由Root唯一使用板；A2沒有存取任何FPGA/JTAG/UART裝置、沒有載入硬體或更新SD。
主目錄其他工作的DS25/DS100與DS sweep材料只讀保留；本包修改都在獨立工作樹。
未來真正在新版本執行時，由Root復用
[通用上板skill](../../board/skill/genesys2-iommu-board/SKILL.md) 的獨占鎖、載入及採集。
不要求額外hash、SD回讀或重複確認；本輪停止原因是使用者明示的修改範圍界線。

## 分析與接受條件

- 硬體時間ms = matched observer cycles / 50000；V吞吐MiB/s =
  validated completed payload bytes / 2^20 / (cycles/50,000,000)。
- 每cell列5次原始值、median/min/max、boot與sequence；V/A miss原樣列出。
  Solo的A miss是N/A，不是測得0。
- slow-down相對Solo = median(Tcase)/median(TSolo)；相對A1 =
  median(Tcase)/median(TA1)，同size、同V工作與幾何。百分比增量=(ratio−1)×100%。
  吞吐百分比損失另算1−throughput_case/throughput_Solo，不與時間增幅混用。
- V/A HPM計數仍是軟體前後非原子快照；不是硬體窗口內精確miss，
  A完成數可能包括啟動和最終排空。A1共跑含頻寬/仲裁成本，A8−A1不能直接叫純翻譯成本。
- 小傳輸初判同時比較 A8/Solo 與 A8/A1；報效應與樣本離散，n=5且同一boot，
  不憑中位數差或跨平台Bermuda趨勢宣稱統計顯著/純因果。

[analyze.py](analyze.py) 只離線分析現有已驗收的 pathv3 `records.json`，
直接從原始bytes與cycles計算，不信任既有metrics快取。它拒絕偽造ABI1小chunk、
重複sequence、失敗/未排空、缺失PATH、AXI/PATH守恆失敗及缺少匹配Solo/A1；
每cell保存原始sample數值、sequence、boot及raw log入口。
15項可重現校驗見 [analysis-validation-v2.json](analysis-validation-v2.json)。
`--require-subset` 是完整45次的拒絕檢查；
目前ABI沒有辦法通過這項檢查。新ABI/幾何獲准後須新增對應adapter及collector，
目前腳本不能冒稱已支援三類負載。

只回算既有歷史50次的例子（選新輸出，不重新測量）：

```sh
python3 paper/ispass2027/exp/E3-workloads/analyze.py \
  --records paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle/records.json \
  --manifest paper/ispass2027/board/runs/baseline100-20261017-01/ds0-middle/manifest.json \
  --expected-repeats 10 --output /tmp/e3-historical-ds0-analysis-NEW.json
```
