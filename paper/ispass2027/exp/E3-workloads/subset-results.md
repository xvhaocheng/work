# E3 DS0 小子集：P0產物就緒，未產生新測量

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: ARTIFACTS_VERIFIED / BOARD_UNMEASURED
- Version Label: E3-subset-P0-ready-v2

**一句話初判：目前仍無法判斷DS0小傳輸的干擾是否大於4KiB，45點尚未執行。**
原pathv3確受4096B與共享chunk限制；後續已批准的P0改動及有限主機／RTL驗證已完成小批，
獨立DS0 BIT時序／routing與共同P0+HPM+multi Linux產物已實際完成，
見[p0-artifact-package.md](p0-artifact-package.md)；等待Root完成既有板排程後採集。

| 指定V descriptor | Solo | A1（4KiB） | A8（4KiB） |
| --- | --- | --- | --- |
| 64B | 未執行，0/5 | 未執行，0/5 | 未執行，0/5 |
| 1KiB | 未執行，0/5 | 未執行，0/5 | 未執行，0/5 |
| 4KiB | 未執行，0/5 | 未執行，0/5 | 未執行，0/5 |

本輪新板測0次；沒有為了湊子集而單獨重跑可用4KiB點。未觸碰UART、JTAG、SD，
最初階段未改動driver或RTL，後續P0最小修改已有明確授權；始終未把軟體供給模式當batch模式。
最初停止依據是使用者原文：「只改測試程序，不改內核驅動和RTL，確需修改時寫change-request.md並停下」。
見 [能力證據](capability.md) 與 [具體變更請求](change-request.md)。
Root可審查的P0最小修改選項已補在 [p0-change-option.md](p0-change-option.md)；
該P0限定方案現已實作並構建；仍沒有解除本輪未測狀態。

## 已有4KiB參考（不是本輪E3結果）

以下僅重新計算已完成的統一100次基線之DS0中段，各條件n=10、同一boot；
絕不重標為本輪每點5次。原始 [records](../../board/runs/baseline100-20261017-01/ds0-middle/records.json)
與 [manifest](../../board/runs/baseline100-20261017-01/ds0-middle/manifest.json)
均保留，離線回算見 [historical-ds0-analysis.json](historical-ds0-analysis.json)。
新版含全部sample與PATH/AXI守恆重檢的回算為
[historical-ds0-analysis-v2.json](historical-ds0-analysis-v2.json)，數值與舊版一致。

| 舊V=4KiB | n | V MiB/s中位 | 硬體ms中位 | V miss中位 | A miss中位 | 時間/Solo | 時間/A1 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Solo | 10 | 282.992 | 56.53868 | 0 | N/A | 1.0000× | 0.5280× |
| A1 | 10 | 149.407 | 107.09009 | 0 | 0 | 1.8941× | 1.0000× |
| A8 | 10 | 135.336 | 118.22441 | 2 | 5141 | 2.0910× | 1.1040× |

吞吐按每次16MiB有效payload除以硬體時間後取中位；表中時間比是時間中位數比。
HPM miss來自軟體前後非原子快照，不能當硬體窗口精確miss。
舊4KiB A1本身已慢約89.4%，A8比A1再慢約10.4%；這是既有共跑觀察，
無法據此補出64B/1KiB，也不能把全部共跑下降稱為純翻譯成本。

## 已完成與尚缺

已完成能力核查、主機參數拒絕驗證、完整矩陣/板時設計與舊數據離線分析。
本席另完成21項可復現parser/caps檢查、15項分析完整性與拒絕檢查，及原CLI靜態交叉編譯；
見 [host-validation.json](host-validation.json) 與 [analysis-validation-v2.json](analysis-validation-v2.json)。
P0限定packed-ring的driver／RTL／CLI／collector及獨立產物已完成，
45次新採集與其V吞吐/硬體時間/V與A miss/慢化表仍未完成。
完整三形態、六種size矩陣和150次候選子集仍為後續設計，沒有拿P0合成測試補值。
