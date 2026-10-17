# 实验合并合同

合并目标只写 `analysis/merged-board-results.csv`，保留canonical `data/board_results.csv`。历史列完整保留，新增`base_id,base_scope,input_table,input_line`。`base_id`表示可合法比较的源码/软件基座，不替代BIT、boot或真实加载身份；历史loaded-byte未知保持未知。

新增板测结果以 `exp/<experiment>/results.csv` 或 `board_results.csv`交付，字段包括原长表全部17字段，另有`base_id`。必须同时发布相邻`analysis-import.json`：

```json
{
  "status": "passed",
  "complete": true,
  "results_sha256": "完整CSV的真实SHA256",
  "base_id": "实际固定基座ID",
  "source_manifest": "相对或绝对的实际源码清单位置",
  "run_manifest": "相对或绝对的完整运行清单位置"
}
```

先写结果和验收，最后原子发布receipt；未完成/无receipt时不导入。读取时检查文件大小、mtime和inode未变，并核CSV真实hash和receipt相符。任何全局重复`row_id`、行/receipt基座不符、必需字段缺失、把仿真混入板测都会中止。新实验各自的硬件正确性检查仍由执行线负责，合并不会授予运行资格。

绘图原生接口均使用逐试次`statistic=sample,n=1`：

| 图 | experiment | 必需condition | metric |
| --- | --- | --- | --- |
| DS四点 | `board_ds_sweep` | `ds=25/25`或`100/100`; `mode=Solo/A1/A8/A64` | `hardware_ms` |
| 设备数 | `board_E2_scaling` | `devices=2/3/4`; `mode` | `hardware_MiB_s` |
| 负载/大小 | `board_E3_workloads` | `shape`; `chunk_bytes=64/1024/4096`; `mode` | `hardware_MiB_s` |
| 页大小 | `board_E4_pages` | `page_bytes=4096/2097152`; `mode` | `hardware_MiB_s` |
| 请求CDF | `board_E5_latency` | `page_bytes`; `mode`; 请求/采集范围保留运行清单 | `request_latency_cycles`，且`statistic=request_sample,n=1` |

DS补点必须与原100次同一base：`baseline100-fixed-885ed0a7-45bf76b1-pathv3`，否则不能连成一组。后续若实际换基座，需要单独panel而不是改ID冒充同源。设备数、负载、页大小各figure原生结果也要求一个base，不借历史不同BIT/软件数值填空。缺少某条件的点不插值；summary median/IQR行不能被当作请求样本或独立重复。

E7是例外的**独立仿真宽表**：`exp/E7-capacity-sim/results.csv`，字段为`base_id,capacity,condition,v_cycles,a_cycles,v_walks,a_walks,v_jobs,a_jobs,bytes_per_copy,checks_passed`，可保留其他审计字段。仍需同样的完整hash receipt，且严格检查16/32/64×Solo/A1/A8/A64共12格、无重复、所有checks_passed。它永不并入板测表，只交给图9。正在写入的校准两格不会被当作完整结果。

Root A的历史DS0派生JSON只有同一原50点的离线重算，不是新板测，也不再复制为新增样本。外部exp路径只有在命令行显式传入时扫描；默认只读本工作树的exp。所有canonical来源、接收/拒绝决定见`merge-manifest.json`。
