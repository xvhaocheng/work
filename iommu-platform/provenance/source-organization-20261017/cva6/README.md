# CVA6／iDMA 源碼分層與精確恢復（2026-10-17）

本輪只整理來源、提交與相依性。新建 27 個功能提交及一個原始檔案 capture 提交；沒有編譯、測試、RTL 模擬、綜合、板測或新實驗，也不代表這些新組合已經執行過。Codex 執行此獨占任務；本項共享 IOMMU 研究沒有使用 DeepSeek。

## 從這裡審查

- [manifest.json](manifest.json)：來源、三層錨點、每一功能的完整 SHA、原提交映射、語義相依與 patch 順序。
- [steps.json](steps.json)：27 項提交、parent/tree、原提交、child gitlink、補丁路徑與雜湊。
- [critical-source-evidence.json](critical-source-evidence.json)：29 段帶有來源 SHA、檔案 SHA-256 和精確行號的關鍵原文。
- [verification.json](verification.json)：實際完成的靜態驗證；[file-identity-check.json](file-identity-check.json) 逐檔記錄 1460 個普通源檔案的凍結／新提交／原 checkout 雜湊。
- [frozen-manifest.json](frozen-manifest.json) 與 [frozen-source](frozen-source/)：凍結身份及相對固定上游有變化的 27 個普通源檔案。不是複製整個 vendor 或 build 樹。
- [patches](patches/)：27 份真正的 `git format-patch --full-index --binary`；[capture.patch](capture.patch) 是另一路原始檔案 capture 的單筆 patch。
- [cva6-chain.bundle](cva6-chain.bundle)、[cva6-capture.bundle](cva6-capture.bundle)：帶有明確 prerequisite 的增量 bundle，保留精確 commit 身份。

## 固定上游、必要集成、功能修復

| 層 | CVA6 SHA | IOMMU gitlink | 邊界 |
| --- | --- | --- | --- |
| 上游 U0 | `1f992d808dc0abe599329dedf73842789f8013b8` | `f8f918aa251209a8b7fafde89ba46a01cf0f7c68` | 原 CVA6 `feat/iommu-refact` 與其原始 child，不能無適配換成 c80 後仍稱同一完整上游平台 |
| 必要集成 A | `54c62575ecc5154e009421d59f9778836db50bd3` | `7bea3f202903279c18df99a2284d6f56ccdacdc6` | 先補 DTI／ATS 關閉時的 ABI、MMU metadata tie-off、來源清單/include，再以原有 mux/demux 接四 DMA、四頁寄存器與來源 ID |
| 功能修復 F | `ba8cdd10659a777ccff0080bf020be64b5c1e064` | `85b157ebe71a623e326e51fff7b7126fe77e8eb9` | SC offset 修復為獨立 `f9938ca8e09fc57190dce25a56c3b0f6eeb9a2fa`；另筆只綁定 IOMMU 的分立修復鏈 |
| 完整研究源碼 | `991074e7fca9eddd33c61d72767c3729f573dc3e` | `0b7939445298a23ea3e131ffd4e003cebb15ac59` | 包含現有全部研究源檔案；原子綁定對應 child 新介面，普通檔案與凍結狀態逐位元組相同 |

上游來自現存本地祖先；[只讀 ls-remote 原始結果](upstream-ls-remote.txt) 確認 `zero-day-labs/cva6` 的 `refs/heads/feat/iommu-refact` 正好指向 U0。沒有對原元件 fetch／升級依賴、修改 remote 或重寫原有 refs；恢復驗證只在新 bare repo 中本地導入固定 prerequisite 與 bundle。U0 原始 IOMMU `f8f918a` 的 commit 物件也確實存在於原 child 物件庫。

A/F 的頂層、peripherals、frontend 經靜態檢查不含 DS delayer、active-cycle counter、E3、batch、observer、banked/nonblock 或 research policy；DS 使用原本直接連接。F 的 IOMMU 鏈包含明列的服務順序及正確性修復，因此不能把它描述成未變的上游服務時間。所有新 profile 的編譯與執行身份仍未驗證。

## 研究功能分層

真實 patch 順序見 `stack_prerequisites` 與每項 `parent`；功能／資料／介面需求另見 `semantic_requires`。例如被動觀測與 batch 不因為排在 DS-delay patch 之後，就理論上需要人為 DS delay。這裡沒有宣稱任意功能子集可直接 cherry-pick、編譯或執行。

| 功能 ID | 本層行為 |
| --- | --- |
| `CVA6-DS-DELAY` | **主動** DS 服務模型，插入 `axi_delayer`，現有預設 50/50；與必要適配分開 |
| `CVA6-ACTIVE-CYCLES` | 被動 backend 活躍週期計數，offset `0x50` |
| `CVA6-BUILD-CLEANUP` | 保留原 Vivado clean 覆蓋；本輪未執行該命令 |
| `CVA6-LIFECYCLE`、`CVA6-PATH` | 被動生命週期／AXI overlap、逐 ID 路徑時間 |
| `CVA6-BATCH` | **主動** 有界自主 copy 供給與完成統計，合併原計數重用修正 |
| `CVA6-E3-TRACE` | 純被動 E3 tracing，原子綁定 child O1 `5304477888b627546fa5f46ef36735f78f0ee504`；該 child 物理上沒有 E3 delay 模組或控制輸入 |
| `CVA6-E3-ACTIVE` | **主動**連接 `E3Enable`、enable/arm/active/delay128；綁定 child E1 `027583cd564af8ccb0cc05228756afa8a322d017` |
| `CVA6-E3-OPPORTUNITIES` | 記錄觸發機會與有界間隔身份 |
| `CVA6-HELD-LAUNCH`、`CVA6-P4M-GAP` | **主動** held-job launch/cancel 與 batch gap；其進度／窗口計數另屬觀測 |
| `CVA6-E2-COST`、`CVA6-LOG-RAM`、`CVA6-COST-AGGREGATE` | E2/COST 被動紀錄、存儲實現、六組聚合；arm/window 寫入不等於 DMA admission |
| `CVA6-LEGACY-E3-SELECT` | 依 profile 選擇是否 elaboration 舊觀測器，影響硬體成本 |
| `CVA6-RESEARCH-POLICY` | **主動**研究 admission/probe 接口，合併原 accepted-command／feedback 修正 |
| `CVA6-QUIET-TAPS` | 被動完整 idle／batch-gap 狀態；本身不切換策略 |
| `CVA6-FULLBUFFER-CERT` | accepted/retired job 衍生的單一完整 aligned 4 KiB copy 許可；不改 job handshake |
| `CVA6-FULLBUFFER-RAM` | tracked iDMA／通用 FIFO 的 FPGA 存儲選擇，供後續 512-beat 研究緩衝；未驗證成本與時序 |
| `CVA6-NONBLOCK-OBS` | 被動 accepted-request 與診斷計數模組 |
| `CVA6-COMPOSITION-CONTROL` | **主動** quiet-gated mode／clear／generation 控制，加上被動窗口與替換計數；不能整個叫純被動觀測器 |
| `CVA6-OBSERVER-V9` | 精簡 composition 観測 ABI 的明確版本 |
| `CVA6-BANKED-PN-WIRING` | 原子連接完整 banked/P/N child 介面；mode1=S、mode2=N、mode3 可選 S/N/P/PN，DMA0/1 完整 512-beat 緩衝、許可、quiet、MSI_BT_ONLY 與計數 |

最終 legacy 預設 mode=0 並不使整棵研究源碼成為原版。新 P/N 介面、存儲、記錄及設定成本必須按具體 profile 另行核帳。

## 原始 gitlink、實際 child、capture 的三種身份

原 CVA6 `platform` HEAD 是 `a1b08934391833aed21e6ad63b545634040808ac`，其 HEAD／索引 IOMMU gitlink 為 `238a39e66a07b27a67fb5749df40691682555027`；原主 checkout 裡 child 的實際 HEAD 則是 `57eb5621802ef9ed8d77c684b9a03205c6630391`，且另有 dirty 源碼。三者不能互換。

獨立 capture `20c93721d719b03dbc17cd4f58d04dd21a97f2d5` 保存「原 HEAD/索引 gitlink + 所有凍結普通源檔案」，不是完整當前 child 源碼的隱式封裝。它與新研究末端只在 `corev_apu/riscv-iommu` 這個 gitlink 不同；完整 child 源碼由 [IOMMU manifest](../iommu/manifest.json) 和其增量 bundle 明確綁定。

iDMA 自上游 `e74b0593` 起已是 CVA6 跟蹤的普通源碼，不是這次另行建立的 iDMA Git repo。普通 vendor 差量只有 `common_cells/src/stream_delay.sv`、`fifo_v3.sv`、`stream_fifo.sv`，依 delay 與研究緩衝分層。其他 13 個原有 child gitlink 保持原值，全部 14 個邊界記錄在 manifest。

## 實際靜態驗收與恢復

27 份 patch 由 U0 在獨立索引逐步 `--check` 並實際應用，每步 `write-tree` 與相應新 commit tree SHA 完全相同。1460 個普通源檔案（包括三個原未跟蹤必需 RTL）與凍結副本、新末端及仍在原處的主 checkout SHA-256 全相同。

`cva6-chain.bundle` prerequisite 為 U0 `1f992d8`；`cva6-capture.bundle` prerequisite 為原 HEAD `a1b0893`。兩者通過實際 `bundle verify`，並在新建 bare repo 中先導入這兩個固定 prerequisite，再導入 bundle，精確恢復末端／capture commit SHA。增量 bundle 不包含全部 prerequisite 歷史；需配合保留的原元資料或相同來源物件。

已確認原主 checkout 的 HEAD、分支 `platform`、索引、status、dirty binary diff、普通檔案內容及全部舊 refs 均未改變。新 refs 全使用 `codex/organize-20261017-cva6-*`；`...-adapted`、`...-baseline-fixed`、`...-final` 為正式錨點。第一次僅普通源碼重放的 `...-chain` 作為審計草稿保留，沒有逐層 child 研究綁定，不能選作構建 profile。正式研究鏈是 `...-research`／`...-final`。

`organize.py`／`finalize.py` 記錄本次實際構造過程，`package_verify.py` 記錄驗證與封裝；它們具有新 refs 前置條件，不是對生產 checkout 的重放指令。無需重跑既有驗收或恢復暫停的任何研究活動。
