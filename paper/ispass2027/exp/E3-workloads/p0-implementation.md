# P0 實作與準備狀態

## Material Passport

- 日期：2026-10-17 JST；ARS experiment-agent／run。
- 使用者已批准「僅P0最小改動、構建和45點板測」；Root唯一執行板操作。
- 狀態：`FRONTEND_DRIVER_CLI_HOST_CHECKS_PASSED / FPGA_BUILD_PASSED / COMMON_SOFTWARE_DELIVERED / BOARD_0_OF_45`。
- 新P0實板結果仍為0/45，沒有把有限RTL、主機模型或協議fixture當作板測。
- 目前完整身份與恢復入口見 [source.json](source.json)、[patches](patches/)。

## 已完成的小批

| 小批 | 實際來源 | 檢查與邊界 |
| --- | --- | --- |
| Frontend BTC2 | CVA6 `8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8`，父`8aac3fcd345af7686a55af1867783fd6b91a0069`，IOMMU子指標仍`85b157ebe71a623e326e51fff7b7126fe77e8eb9` | 只變batch長度合法集、magic和batch地址步進。79個有限情境通過，含實際length/offset、STOP/backpressure、配置鎖、舊4KiB／非batch逐週期gold對照；完成由合成backend回覆，不是AXI/data驗證。Root已直接審查接受。 |
| ABI2與driver | Linux P0 `b2455905e44e`；與A3原HPM提交`968bec65dbf9fb0d85585574134be9597a6d24c6`真實合併後為`f81c82c24a8c6833c14a1d5c6fa315a8b26e1345` | 新0x09/0x0a、64B caps／328B run、reserved全零；舊ABI1/ioctl不變。共用mapping/cache/全域鎖/stop/drain/verify/PATH，只有V/A真實chunk分別傳入。38項真實函式host/MMIO模型與RV64 idma.o通過，Root已直接審查接受。 |
| 新CLI | 平台 `e76cadf824a820ebe52df8ad2bfffe21d4ff1168` 的`bench/idma_p0.c` | 獨立程式保留原CLI。32項實際main+fake syscall檢查通過，靜態RV64 ELF已生成；舊kernel/BIT拒絕、bytes/drain/data/quarantine/input echo拒絕、EINTR不重試。 |
| 採集與解析 | [collect_p0.py](collect_p0.py)、[p0_records.py](p0_records.py) | 只產生45點計畫；50MHz／DS0／新payload／BTC2／同boot＋新sequence、全資料與排空、16 PATH、AXI守恆都保留。20項合成協議檢查通過；沒有真UART或新板測。 |

原始回執：[frontend79](test-results/p0-frontend01.json)、
[driver38](test-results/p0-driver02.json)、[RV64物件](test-results/p0-linux-obj01.json)、
[CLI32](test-results/p0-cli02.json)、[協議20](test-results/p0-protocol01.json)。
函式模型替換了kernel/MMIO/HPM/observer/cache；它不證明實機cache或IOMMU行為。
安全排空後的資料錯誤仍沿原實作返回EILSEQ且可釋放；未知ID／排空失敗會quarantine並保留緩衝。

## 身份與相容性

所有真實內部變更都在獨立`ispass/a2-workloads`工作樹，不改活動研究checkout或既有build輸入。
平台先更新CVA6 gitlink與source lock；最終Linux+multi凍結後再更新Linux指標／鎖。
最終共同Linux已凍結為`acece3a3ab7d72ba1f936c1984b82a4a55f28d46`，
是P0+HPM合併點`f81c82c`的直接後代；A3原HPM與multi提交均保留。
實際平台`52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0`的Linux gitlink與source lock已同步，
CVA6仍為`8aeb9ac`。新恢復包為`p0-linux-multi.incremental.bundle`（前提M）與
`p0-platform-final.incremental.bundle`（前提`885ed0a`）；均已實際git bundle verify通過。
CVA6/Linux只有上游remote，平台無remote；沒有推上游或新建remote。
最小增量bundle／patch隨work任務分支推送，不能稱為已推組件遠端。

新BIT的0x80 magic為BTC2；舊kernel會明確拒絕它的batch操作。
新driver的舊ABI1 batch接受BTC1/BTC2但仍只允許4096B；新P0操作只接受BTC2。
非batch維持原功能。新P0限定V0、可選A1、V1頁、A1/8頁、V64/1024/4096、A4096、
4096次＋預熱256、packed-ring；caps的max_pages僅為allocation limit，不擴大這個合同。

原始FPGA構建source manifest保留了平台參數`71e3af5`，其完整解析在source.json補記，
不改寫已啟動構建的原manifest/hash。實際編譯CVA6與所有vendor commit在make前已是完整SHA且凍結。
軟體prepare首次用了task branch ref；其exact CLI bytes與事前提交`e76cadf`已核對並補存完整SHA／原ref。
finalize入口在執行前將ref解析為完整commit。這些補記不冒充原始時間點已具備的欄位。

## 共用軟體與構建

[build_p0_fpga.py](build_p0_fpga.py) 使用保存的原baseline builder流程，
不更改main的baseline pins或約束，僅DS0、50MHz；WNS<0、任一setup/hold/pulse失敗或routing error都停止。
本機輸出：`artifacts/p0-ds0-build01/`；原流程已passed：50MHz、WNS +0.177ns、
setup/hold/pulse failing均0、routing errors0；routed CPU period20ns、delay/research cell0。
實際BIT／完整manifest／原始報告與Root執行入口見[p0-artifact-package](p0-artifact-package.md)。

[build_p0_software.py](build_p0_software.py) 已實際完成prepare：
新rootfs 6,349,312 bytes，包含原rootfs的原版`idma_experiment`、`platform-timeout`、
`platform-sd-update`及新`idma_p0`。回執見[prepared](test-results/p0-software-prepared01.json)。
Linux/HPM/multi最終凍結後只build一次共同payload，可用`--extra-program NAME=SOURCE`加入E2 CLI。
release為`6.5.0-rc2-ispass2027-p0-multi-v1`（最終以實際kernel.release為準），
沿原config/DTS/rootfs/lloader/OpenSBI流程，最終fw_payload不得超過33,554,432 bytes。
舊payload實際29,363,656 bytes，headroom 4,190,776 bytes；目前只是預算參考，最終大小尚未測。

最終步驟用真正kernel的vmlinux.symvers與release編譯未修改的spi-bitbang／spi-xilinx恢復模組。
從舊pathv3寫入新image時仍使用既有baseline100包的pathv3恢復模組；
新模組只供以後軟體更新，正式測量boot不載入外部模組。
沒有因prepare或build更新SD，也沒有額外hash/SD回讀或獨立板測關口。

共同軟體已從上述固定提交實際構建一次，包含新`idma_scaling`，原三個工具逐byte保留。
正式release為`6.5.0-rc2-ispass2027-p0-multi-v1`；rootfs 6,857,728 bytes，
payload 29,363,656 bytes，32MiB餘4,190,776 bytes。
正式交付manifest為本機`artifacts/p0-software01/delivery-manifest.json`，
完整回執副本見[software-final01](test-results/p0-software-final01.json)。
已生成`artifacts/p0-software01/fw_payload.bin.gz`，10,520,154 bytes；
[運輸回執](test-results/p0-software-transport01.json)記錄原payload／gzip的SHA與host解壓相等檢查。
Image／lloader／OpenSBI先成功；最後恢復模組因缺`linux-build/scripts/module.lds`失敗。
原[failed manifest](test-results/p0-software-build01.failed.json)與[原log](test-results/p0-software-build01.failed.log)不改寫。
[repair_p0_software.py](repair_p0_software.py)依既有board recovery流程補`modules_prepare`，
正常modpost／link生成兩個匹配正式release的SPI模組；沒有降低符號／版本檢查。
[repair原始回執](test-results/p0-software-repair01.json)記錄Image/payload/rootfs/config/關鍵源碼與
原失敗manifest/log前後hash完全一致；沒有重編Image或payload。
prepare與finalize實際執行版builder分別保存於`source/build_p0_software.prepare01.py`與
`source/build_p0_software.finalize01.py`；目前builder只補未來新構建所需的`modules_prepare`。

## 守恒與採集入口

新parser按Root的[測點邊界核查](../../board/observer-shared-counts.md)處理共享AR/AW：
`shared = V_PATH0.full+carry + A_PATH0.full+carry`。
PATH0 enter是首次offer，私有AR/AW是握手接受，故不把兩者庫存硬等同。
`shared−private`保留帶符號診斷值；它不是DMA錯誤或未選裝置流量。
20項協議檢查包含closing時A已私有接受、共享spill留1筆的合法fixture；
新公式通過而舊私有加總差為−1。原E1b collector未改。

實際小包AXI burst數尚待A1的真實backend有限測試；不能用1/1/2的源碼推導冒充結果。
`collect_p0.py`強制要求該實際passed報告，拒絕`fixture_only`；全部實際產物與Root新load到位後：

```sh
python3 paper/ispass2027/exp/E3-workloads/collect_p0.py \
  --output NEW_P0_RUN --load-manifest ACTUAL_P0_LOAD_JSON \
  --fpga-manifest ACTUAL_P0_FPGA_BUILD_JSON --software-manifest ACTUAL_FINAL_SOFTWARE_JSON \
  --backend-evidence A1_ACTUAL_BACKEND_SUMMARY_JSON --expected-kernel ACTUAL_KERNEL_RELEASE \
  --board-tools /home/xvhaocheng/work/paper/ispass2027/board
```

這些待填位置均為尚未完成的實際產物，不是現在可執行的假命令。
五區組輪換，單point kernel60秒／外層90秒／host120秒；失敗立即停止並保留raw，無自動重試。
若只是parser修正，可`--resume-run`恢復完整成功raw前綴，只執行餘點，不能繞過實際失敗。
完成後 [analyze_p0.py](analyze_p0.py) 從raw-derived bytes/cycles重算全部45次，
輸出E1b同列長表與n/median/Q1/Q3/IQR；同block同size的A8/A1時間配對比，
與原cell時間中位數之比分開列出。支援明確partial成功前綴的只讀不可覆寫快照；
完整合同、Type7與18項finite回執見[analysis-contract](analysis-contract.md)。
完整三形態與16/64KiB／2D仍僅設計，不在此次授權或實作中。
