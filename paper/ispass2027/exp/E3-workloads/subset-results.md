# E3 DS0 小子集：采集中，已同步首区组9/45

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: PARTIAL（首区组各cell n=1）
- Version Label: E3-subset-P0-progress01

**一句话初判：首区组64B/1KiB的A8相对A1增幅约14%，4KiB约10.4%；每格仅n=1，等待全部45点再作最终判断。**

实际P0 DS0 BIT与共同内核已启动验证通过：release `6.5.0-rc2-ispass2027-p0-multi-v1`，
boot `088d7493-26d3-4254-b49e-2e1e8f66e910`，taint=0。Root为唯一板操作者，
已冻结的[首区组数据](checkpoints/p0-progress01/README.md)含9个真实接受试次、17列CSV与原字节来源。
实际活动campaign仍在继续，其余点不补齐；累计前缀与最终包不得重复计样。

| V descriptor | Solo（ms，n=1） | A1（ms，n=1） | A8（ms，n=1） | 配对A8/A1额外慢化 |
| --- | --- | --- | --- | --- |
| 64B | 6.42780 | 59.12696 | 67.55024 | 14.2461% |
| 1KiB | 16.59726 | 68.93826 | 78.50768 | 13.8811% |
| 4KiB | 56.49434 | 107.03206 | 118.16248 | 10.3991% |

每点V固定4096个descriptor，不同大小的有效工作不同；只在同大小内计算慢化。
A保持既有4KiB背景供给至V结束，A1/A8仅改变其映射足迹，A完整完成量另记。
上述为DS0实测共跑差异，不作纯翻译归因；完整三形态采用另一等16MiB协议，不与本45点混池。

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

## 余下工作

继续完成同一boot的45点固定矩阵，每cell目标5次；技术失败保留原始记录并停止，不重试替换。
完成后从原始命令重算中位数、Q1/Q3/IQR及同区组A8/A1配对比较，更新一句话判断。
完整三形态的BTC3/OBS5/Depth64硬件正在独立编译，ABI5软件分批审查；其结果另建实验包。
