# P0 統計與長表

狀態：分析流程已驗證，實板仍0/45；此文件與有限測試不填入任何論文實測值。
[analyze_p0.py](analyze_p0.py)預設只接受status=passed的完整45點，重新解析每個已接受raw命令，
檢查同boot／獨立sequence、固定輪換plan、真實bytes/cycles、PATH與AXI守恆，並核對四份build/load/backend來源hash。
不使用records中快取的metrics；不修改採集檔案。
正式主CLI拒絕collection、load、FPGA、software或任何保留raw命令的`fixture_only`；
backend fixture也始終拒絕。拒絕發生在任何snapshot／analysis／CSV輸出建立之前。
沒有公開CLI參數繞過此限制。內部有限測試必須顯式使用`_fixture_mode=True`，
結果傳播`fixture_only`並標`source_board_cases=0`；CSV的experiment、evidence_level與sample_set均明確synthetic。

長表的18列欄位與E1b完全相同：
`row_id,experiment,condition,metric,statistic,value,unit,n,bitstream_id,kernel_id,raw_path,source_locator,evidence_level,value_status,identity_status,boot_id,sample_set`。
本目錄[table_schema.py](table_schema.py)是E1b該檔的逐byte副本，原提交
`3a124edc7e026bfabcb01323f712bd215cc5e103`，SHA256
`423a745aa9a689b25ec0e5a4233dde11789b7abc1f76612551966b5e66f80462`。
保留副本使E3任務包可獨立使用；沒有改E1b或共享data。
每個CSV scalar的`raw_path/source_locator`指向本次不可覆寫的analysis JSON；
JSON中每個sample帶原records index、raw命令路徑、case/block/boot/sequence，並保存原檔hash。
identity狀態只聲明collector/load receipt/hash/boot綁定，不增添BIT配置回讀聲明。

每個V大小／條件列出真實n、min、median、max、Q1、Q3、IQR。
Q1/Q3與E1b相同，使用Type 7：對排序後n個值取`h=(n−1)p`，在floor(h)與ceil(h)間線性插值。
沒有樣本時n=0、數值null；Solo沒有A的HPM／PATH延遲標為not_applicable，不補零。
時間、有效完成bytes計算的吞吐、HPM、重疊與PATH延遲各用自己的觀測窗，
不能把非原子HPM軟體括號或整個A startup/drain工作當成精確V窗吞吐。

JSON的`paired_comparisons`只配同boot、同block、同V大小的A8與A1：
每對計算`t(A8)/t(A1)`，再統計這些比值的n/median/Q1/Q3/IQR。
未配對block明列並保留，不拿另一block補配。
各cell的`ratio_of_medians`另算`median(t(condition))/median(t(reference))`；
原`slowdown_vs_Solo_x/slowdown_vs_A1_x`保留為這個量的相容欄位。
CSV的ratio_of_medians列分別附numerator_n/denominator_n，n欄留空，避免冒稱配對樣本數。
兩個估計量在有限反例中分別為2與101/3，驗證沒有混用。

完整採集後，在E3內新建輸出，例如：

```sh
python3 paper/ispass2027/exp/E3-workloads/analyze_p0.py \
  --records ACTUAL_RUN/records.json --manifest ACTUAL_RUN/manifest.json \
  --backend-evidence ACTUAL_BACKEND_SUMMARY_JSON \
  --output paper/ispass2027/exp/E3-workloads/ACTUAL_ANALYSIS.json \
  --long-table paper/ispass2027/exp/E3-workloads/ACTUAL_LONG_TABLE.csv
```

部分採集快照在同一命令加`--allow-partial --snapshot-dir NEW_E3_SNAPSHOT_DIR`。
只讀取records.json已成功的連續prefix，不glob候選或執行中UART檔，不恢復被拒絕點。
每個來源只讀一次，複製實際讀到的bytes與hash到新snapshot；活動檔案完全不寫。
若records與manifest恰在兩次寫入之間不一致或JSON尚未寫完，立即拒絕，不自動重試或發布半份快照。
snapshot保留原collection_status；running或failed的成功前綴始終標partial，不能作為完整45點。
原failed命令不加入統計，也不重試；完整錯誤記錄仍由採集器保留。

[verify_p0_analysis.py](verify_p0_analysis.py)的24項有限檢查包含主CLI拒絕fixture發布、
五個來源邊界各自獨立拒絕與synthetic證據傳播、內部顯式test模式的部分快照、
來源逐byte不變、候選檔不讀、來源hash與raw重新解析拒錯、空prefix、未配對block、
分位數與兩種比值反例；[原始回執](test-results/p0-analysis04.json)明示fixture_only與新板測0。
合成UART與CSV只在ignored artifacts測試目錄，不作論文資料。
較早analysis01/02回執和本機fixture保留為歷史；其中的測試輸出不具備目前的fixture發布邊界，
不能作板測長表。新24項回執驗證了修正後的明確拒絕與synthetic標記。
