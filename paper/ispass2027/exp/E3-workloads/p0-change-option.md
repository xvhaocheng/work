# E3 P0 限定修改選項：保留批量供給的獨立 V/A 長度

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent（run 準備階段）
- 日期：2026-10-17 JST；狀態：`PROPOSED_NOT_IMPLEMENTED`。
- 目標僅為 DS0：V=64/1024/4096B × Solo/A1/A8，A 固定4096B，各5次，共45次。
- 本文件供使用者決定是否擴大修改範圍；沒有改 driver、UAPI 或 RTL，沒有構建新內核/BIT，沒有板測。
- 接受「緊密順序 N 頁環」這一明確形態後，P0 才有下列最小實作路徑；不把它宣稱為全部 NIC/Storage/Tile 能力。

## 現在為何不可執行

以下 SW=`/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-software01`，
HW=`/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6`。
完整證據與實際 build manifest 見 [capability.md](capability.md)。

1. `SW/inputs/bench/idma_experiment.c:395` 發送單一 `RUN_BATCH` 或 `RUN_EXPERIMENT`；
   `SW/src/linux/include/uapi/linux/idma.h:47–61` 僅有一份 `chunk_bytes`。
2. `SW/src/linux/drivers/misc/idma.c:901–906` 對 V/A 均傳 `e.chunk_bytes`，
   `337–345` 將 `s->chunk` 寫進真正 `NUM_BYTES` 寄存器。
3. 同 driver `85–86,868,1030` 以全域 `idma_experiment_lock` 從準備一直鎖到完整清理。
   不同 fd、不同 `/dev/idmaN`、兩個進程均不能繞過它形成獨立長度的同步共跑。
   `1072–1077` 也沒有 mmap/read/write 或使用者 descriptor 入口。
4. driver `863–865` 強制 batch `chunk_bytes==4096`；
   `HW/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:96–100`
   強制 `num_bytes.q==4096`、S/D 4KiB 對齊，`344–346` 固定每任務 `offset+=4096`。
5. driver `561–563` 以 `completed*4096` 核帳與定義連續已觸及區；
   現有 collector 另以每4096B任務兩個 AXI 交易核對 PATH。只放寬一層檢查會失真。

## 建議選項：緊密環，固定頁邊界的整除長度

P0 採 source/destination 各自 N×4096B 的連續環，任務 i 的位移為
`(i*L) mod (N*4096)`，真實描述符長度為 L。
V 的 L∈{64,1024,4096}、N=1；A 的 L=4096、N∈{1,8}。
所有 L 整除4096，因此每個描述符都不跨4KiB邊界，S/D 使用相同位移。
V 每輪4096個任務、預熱256個任務，其有效 payload 分別為256KiB、4MiB、16MiB。
只在同一 V size 內相對 Solo/A1 算慢化；不以跨 size 原始時間直接作干擾判斷。

這一選擇保持已有連續前綴資料驗證可用：安全排空後，已觸及集合是
`[0, max(預熱觸及前綴, min(completed*L, buffer_bytes)))`；其他 byte 仍必須保持反模式。
不用 bitmap、不新增逐包地址表、不變更 mapping、頁大小、排空或 CPU cache workaround。
4KiB 時地址序列、實際 payload、task 數及驗證集合退化為原始行為。

**另一個幾何選項是每頁一個槽**：`offset=4096*(i mod N)`、len=L。
此時 V64B 一頁環只重複同一64B槽，而緊密環會遍歷該頁的64個槽；不是相同流量。
多頁小包槽會產生洞，舊連續前綴驗證必須改為按頁槽區間驗證，並核對所有 padding。
不建議把這個選項與緊密環默默混用；若使用者選它，需明確記錄 stride=4096，
增加針對洞與 padding 的驗收。P0 推薦前者，因其貼近既有非 batch 順序環且修改範圍較小。

## 最小修改面（方案，未實作）

| 檔案／介面 | 必要修改 | 保留條件 |
| --- | --- | --- |
| `linux/include/uapi/linux/idma.h` | 保留 ABI1 結構與 `0x06/0x07/0x08`；新增明示版本的 batch 操作和結構，V/A 各一份 `chunk_bytes`，並明示 geometry=`packed-ring`。能力查詢須明示該新操作、合法長度集合、頁環及上限；不借用 ABI1 reserved/flags。 | ABI1 結構、ioctl number/size 與既有4096B語義保持；新使用者程式遇舊內核須明確拒絕，不能降級為軟體補發。具體 ioctl number 和欄位配置在實作審查時定稿，尚未佔用。 |
| `linux/drivers/misc/idma.c` | 新操作共用既有全域鎖、prepare/測量/stop/drain/驗證路徑；分別 validate/prepare V/A 長度。只接受 P0 的整除長度、DEFAULT copy、對齊基址與原有頁數上限。`idma_submit()` 確實寫各 lane 的 L；batch bytes 改為 `completed*L`，touched 前綴改 `min(completed*L,size)`，與預熱 `touched_end` 取 max。 | 原 ABI1 batch 仍只允許4096B。V/A 各自完整 submitted/completed/bytes、資料與 quarantine 檢查；錯誤停止、保留緩衝、釋放順序及 cache workaround 不刪。乘法／加法溢位與 bounds 明確檢查。 |
| `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv` | 批量配置檢查由固定4096B改為新能力允許的 L；COUNT 中只在 batch 啟用時使用 `offset_next=offset+num_bytes.q`，達 `pages<<12` 回零。單個 lane 的 `num_bytes` 本來就是獨立寄存器。非 batch 的舊位移／控制行為保留。 | 不動 backend、仲裁、IOMMU、observer 或 DS。保持 DEFAULT copy、基址4KiB對齊、active 配置不可寫、STOP 只停止後續任務且排空當前任務。4KiB分支與原來相同。 |
| batch 能力標記（同 RTL＋driver） | 使用可識別的新 batch magic／版本，driver 按新操作明確核對，防止只更新內核而仍在舊 BIT 上量測。新 driver 的舊4096B操作可支援舊／新magic；新操作只接受新magic。 | 不將舊 BIT 反標為支援小包；不擴充 observer 身份體系。沿用現有 manifest 保存實際新 BIT/payload。 |
| `bench/idma_experiment.c` 或 E3 專用使用者程式副本 | 新 CLI 參數 `--victim-chunk`、`--aggressor-chunk`，只走新操作；JSON 新 schema 分別記錄 V/A length、bytes、geometry。舊 CLI／schema 原樣可重現。 | `--chunk` 的舊共享語義不偷換；CLI 核對 `Vbytes=N*Vlen`、`Abytes=Acompleted*Alen` 與所有既有正確性狀態。 |
| E3 專用 collector／analysis | 配置45次輪換；保留 boot/sequence、TIME/AXI、16條同sequence PATH、正確性、完整 drain、stock/flow 守恆。交易數使用實際 AXI 計數與獨立推導的小包預期；不得沿用 `2*N`。 | 64B/1KiB 在本次對齊 packed-ring 下通常每描述符1個AXI burst，4KiB原基座為2個；須以實際backend burst設定及RTL驗證確認，不能把「通常」寫成已驗證事實。PATH級間、開閉庫存與V全窗內完整性檢查全部保留。 |

RTL 不需要新 descriptor buffer 或逐包軟體補發；每 lane 仍一次配置、持續硬體 batch，
A target 仍長跑至 V 結束後 STOP，再排空。這是測量輸入／供給幾何的擴充，
並非干擾緩解機制。必須披露新基線相對原4KiB版本的差異。

## 有界驗收

1. 主機/UAPI：舊 ABI1 結構與 ioctl 值不變；新CLI在舊kernel/BIT明確拒絕；
   非法長度、非整除、越界、非零 reserved、同V/A裝置與計數溢位均拒絕。
2. 針對 frontend 的有限檢查：L=64/1024/4096、N=1/8，各自記錄真正送入backend的
   `length`、S/D offset、issued/completed；測滿一圈、兩圈、單任務、STOP中途。
   V64+A4096並行確認每lane真實不同長度；4096舊序列與原行為相同。不得只測參數接受。
3. driver：實際 bytes 與 descriptors 守恆；預熱與測量各自正確回環；安全排空後逐byte
   檢查全部 source、應寫目的區與未寫區。注入錯誤須保留資料錯誤／quarantine，而非吞掉。
4. 現有 PATH 全部檢查仍在，新 collector 根據真實 transaction 數守恆，不為小包移除驗收。
   原4KiB資料與新4KiB資料可分版本比較，但不套用歷史G1的±2%作新版本硬門檻。
5. 新 payload/BIT 身份按既有 manifest 記錄並配套保存。驗證成功且使用者授權範圍包含新版本上板後，
   由 Root 獨占執行一次有限 P0，錯誤即停，原始log保留，不自動重試。

## 未涵蓋

此 P0 方案沒有 1500/1536B 的 padding ring、16/64KiB 單描述符或非平凡二維 tile。
完整三形態的必要介面仍見 [change-request.md](change-request.md)。
本席交付到具體方案、原始證據與主機驗證；未執行上述任何 driver/RTL 修改。
