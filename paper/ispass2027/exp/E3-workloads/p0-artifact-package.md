# P0 實際交付包

2026-10-17；Root唯一上板執行者。新P0測量仍0/45，以下是已完成構建的實際檔案。
全部大型二進位只保留本機，真實組件變更／恢復包與回執已提交任務分支。

| 產物 | 本機絕對路徑 | 身份／結果 |
| --- | --- | --- |
| 新BIT | `/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-ds0-build01/src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit` | 11,443,722 B；SHA256 `1056d7795ffc5673b1aaab29b09ad3e1654a299b481457bf0b8067d18f23afa7` |
| 完整FPGA manifest | `/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-ds0-build01/build-manifest.json` | status=passed；CVA6 `8aeb9ac`，IOMMU `85b157e`；DS0直接wire、50MHz、observer3 |
| 正式共同軟體manifest | `/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-software01/delivery-manifest.json` | status=passed；Linux `acece3a`，平台 `52c5fdd`；release `6.5.0-rc2-ispass2027-p0-multi-v1` |
| 原payload | `/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-software01/fw_payload.bin` | 29,363,656 B；32MiB餘4,190,776 B；實際SHA見manifest，不沿用舊pathv3身份 |
| 壓縮payload | `/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-software01/fw_payload.bin.gz` | 10,520,154 B；SHA256 `69ccd60ee57c0b9a54ac444548a8f007207048d4539dbe01d3e481b2b5714a8a`；解壓host驗證相等 |
| 靜態CLI | 上述軟體目錄`bin/idma_p0`與`bin/idma_scaling` | 以正式最終UAPI編譯；原`idma_experiment`／timeout／updater原bytes保留 |
| 新kernel恢復模組 | 上述軟體目錄`spi-recovery/spi-bitbang.ko`與`spi-xilinx.ko` | 正常modpost／link、正式release vermagic；只供日後更新，不在正式測量boot載入 |

FPGA原始[完整回執](test-results/p0-fpga-ds0-build01.json)、[時序摘要原文節錄](test-results/p0-timing-routed01-summary.rpt)、
[路由報告](test-results/p0-route-status01.rpt)、[routed boundary](test-results/p0-routed-boundary01.json)。
完整7.7MB原生時序報告保留在artifact內；[節錄索引](test-results/p0-timing-routed01-index.json)保存
完整原檔路徑／SHA與節錄行號／SHA，沒有用節錄冒充完整原檔。
WNS +0.177ns、TNS0；hold slack +0.035ns、pulse slack +0.062ns；setup／hold／pulse failing均0；
257848/257848需路由網全部完成，routing errors0。
routed檢查CPU period20ns、delay cell0、研究機制cell0，passed；不稱為完整IO/CDC signoff。
約束與實作策略原樣，沒有為通過時序放寬條件或重構RTL。

`build/fpga-ds0-build.json`只是baseline流程的短pointer receipt，不能直接傳collector；
它沒有完整parameters／direct_ds_wire。collector使用上表完整FPGA manifest。
軟體原`artifacts/p0-software01/build-manifest.json`保留原先SPI模組失敗狀態，
collector使用上表正式delivery manifest；模組-only repair與一次Image/payload構建的證據見
[p0-implementation](p0-implementation.md)。

Root按現有board skill先完成舊DS25/DS100採集，再由原pathv3 boot用舊同版恢復模組寫入新payload。
UART上傳壓縮檔後用`gzip -dc /tmp/p0-fw_payload.bin.gz > /tmp/p0-fw_payload.bin`，
再沿既有updater程序更新；A2沒有執行板操作。
新BIT／共同Linux啟動後，以實際load manifest執行：

```sh
python3 paper/ispass2027/exp/E3-workloads/collect_p0.py \
  --output NEW_P0_RUN --load-manifest ACTUAL_NEW_P0_LOAD_JSON \
  --fpga-manifest /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-ds0-build01/build-manifest.json \
  --software-manifest /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/p0-software01/delivery-manifest.json \
  --backend-evidence paper/ispass2027/exp/E3-workloads/backend-validation/runs/run03/summary.json \
  --expected-kernel 6.5.0-rc2-ispass2027-p0-multi-v1 \
  --board-tools paper/ispass2027/board
```

其中NEW_P0_RUN與實際新load manifest由Root指定；backend路徑已在Root合入的A1 `966d523`包中。
真實backend13合法情境／87 descriptors給出64/1024/4096的AR/AW 1/1/2；
其summary明列零長既有缺陷與NO_ERROR_HANDLING不傳播AXI錯誤的限制。
P0 CLI／driver／batch frontend三層枚舉拒絕零長，有限合法域證據不擴成任意descriptor通過。
45點全部程序內檢查、資料驗證、STOP/drain/quarantine、PATH與AXI守恆保留。
分析命令、長表與partial快照見[analysis-contract](analysis-contract.md)。
