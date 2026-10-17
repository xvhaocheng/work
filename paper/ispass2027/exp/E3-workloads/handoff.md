# A2 交接：E3 準備與 P0 修改邊界

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent，run準備／主機驗收。
- 日期：2026-10-17 JST；`HOST_CHECKS_PASSED / E3_BOARD_UNMEASURED`。
- 獨立工作樹：`/home/xvhaocheng/work-a2-workloads`，分支 `ispass/a2-workloads`，
  起點 `01f981a`（main）。只修改本E3目錄；沒有內部repo或實際構建源變更。
- Codex執行來源核查、方案、主機編譯及驗收；此研究保持Codex-only，未使用DeepSeek。
- FPGA/JTAG/UART/SD均未存取；Root是唯一板使用者。

## Root先看

1. [P0限定修改選項](p0-change-option.md)：可直接供使用者決定是否擴大到driver/RTL。
   推薦緊密N頁環；真實len、地址步進、V/A獨立UAPI、bytes/touched、版本相容與驗收均已列。
2. [能力證據](capability.md) 與 [關鍵原文](source-evidence.txt)：實際pathv3的完整call chain，
   含全域鎖否決「多進程／不同fd獨立長度並行」及batch三層4096B限制。
3. [執行包](run-package.md)：目前沒有合法45點P0命令；現有4KiB命令、主機重現命令、
   完整342 cell矩陣／1710次、150次論文候選子集及基於既有15.7秒/點的有界板時設計。
4. [結果狀態](subset-results.md)：新45次全部未測。舊DS0的4KiB/Solo/A1/A8只是歷史參考，
   n=10，不改標為本輪n=5。16MiB footprint的軟體驗證時間未測，不能保證外推。

## 已實際驗收

- [host-validation.json](host-validation.json)：GCC11.4，原CLI/UAPI哈希匹配既有manifest，
  21項parser／合成caps與幾何檢查通過；原CLI靜態RISC-V交叉編譯通過。
- [analysis-validation-v2.json](analysis-validation-v2.json)：15項校驗通過，含完整PATH、
  AXI/stock/flow、排空、bytes、錯誤、缺Solo、錯DS與偽小包／偽n5拒絕。
- [historical-ds0-analysis-v2.json](historical-ds0-analysis-v2.json)：重算既有50次，
  5組各10次；新增全部sample與raw入口，每個舊aggregate數值逐項與v1相同。
- Python編譯檢查與Git whitespace檢查通過。沒有宣稱測過driver或RTL的新行為。

## 二進位與更新

原CLI重編譯產物：
`/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/host-check01/idma_experiment.pathv3`，
517336 bytes，SHA256 `1e6a776bd6b7ed9613016aaae858b0d504e13643f00cde0b343161aa38e8de92`。
原輸入CLI SHA256 `f91112f6d844af1fc683266a87a287edcbc42ddd9b3b419f40e37aac16a732f8`，
UAPI SHA256 `f67cc1066c6183dd9794f6e34d6a55bf5b0840465cae268cc16acbd4cc7365aa`。
實際編譯命令在host-validation。二進位本機保留，artifacts被Git忽略。

這個產物仍是原版介面，現有payload中的CLI已足夠，不需為本包更新Linux或傳輸。
P0若獲准擴大範圍，至少需新driver/UAPI與新BIT及新CLI/collector；本席尚未實作。
完整三形態仍是設計，P0方案也不包含16/64KiB或真正2D。

## 保存與下一步

從主目錄只讀複製的原E3草稿、19項host-check工具、11項分析驗證和v1回算原樣保留；
本席用verify_*、v2回執及獨立artifacts增量完成，不覆蓋歷史回執。
分支供Root審核合入 `ispass/root-a`，不合main；共享狀態／README／資料長表由Root統一收尾。
原始新板測log尚未收到，沒有待分析的新E3結果。取得已授權、已驗收新版本和Root原始log後，
才建立新schema adapter及實際P0結果；不能用ABI1分析器接受偽造小包。
