# SPDK 成功事件与真实记录可得性核查

**结论：已找到同版本官方实现的最小 PCIe 事件 schema 和可读工具；本次没有取得一份实际运行 trace，更没有 host IOVA—NVMe—另一任务—IOMMU/PTW/DDR 的联合记录。** 这保留了完成驱动供给的源码事实，却不能冻结 D 的真实正反状态或完整任务时间。所有“已有字段”均指官方事件实现已有，不表示本次已经获得该字段的实测值。

固定 SPDK v24.09，ref `19524ad45373e35d5fdc06f4e081444d87ceed80`。原件 URL、SHA-256、实际读取范围与失败取件均在 [sources.json](sources.json)；机器可读分类在 [assessment.json](assessment.json)。核查采用 ARS-Codex source-verification / fact-check，属源码接口证据，不是实验或独立文献新意判断。

## 1. 最小严格子集：真实实现的 schema，非测量 trace

主证据是 [官方 nvme_pcie_common.c](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/lib/nvme/nvme_pcie_common.c#L624) 的 624–712、1831–1862 行。保存原件 [nvme_pcie_common.c](nvme_pcie_common.c)。

| 事件 | 原件字段与真实含义 |
| --- | --- |
| `NVME_PCIE_SUBMIT` | owner 为 `qpair->id`，object 为 `nvme_request *`；参数为 `ctx=req->cb_arg`、CID、opcode、CDW10/11/12、软件 qpair queue_depth |
| `NVME_PCIE_COMPLETE` | 同样的 owner/object/ctx，CID、`cpl->status_raw`、软件 qpair queue_depth |
| 公共时间/核字段 | reader 的 JSON 输出保留原始 `tsc`、`lcore`，文件顶层输出 `tsc_rate` 与 tpoint 参数定义；不是硬件 DMA timestamp |

这是实际库代码的两种事件定义与发出点，**不是日志格式设想、合成 expected、测试 fixture 或公开实验段**。本次没有虚构示例事件值，也没有把任何脚本输出当实测。

[官方 reader](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/app/trace/trace.cpp#L197) / 保存原件 [trace.cpp](trace.cpp)：197–305 行输出事件与定义；326–384 行遍历输入；387–407、425–520 行支持文件 `-f`、共享内存 `-s` 及 JSON `-j`。这证明有解析入口；没有输入文件就没有事件数据。reader 未执行，底层 parser 的丢失、环覆盖及关联策略未额外审查，不能声称一段将来的文本/JSON已完整。

## 2. 成功条件与关联陷阱

**COMPLETE 不是成功专用事件。** 在 `nvme_pcie_common.c:681–703`，记录发生在 error/retry 判断之前；重试会再次调用 submit_tracker。同文件 722–734、738–751 行的人工错误完成也调用此路径。不能把每条 COMPLETE 记作成功 IO，也不能把每次同 CID 的 SUBMIT 当一笔新的用户任务。

成功条件直接核 [官方 nvme_spec.h](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/include/spdk/nvme_spec.h#L4429) / 保存原件 [nvme_spec.h](nvme_spec.h)：1434–1465 行给 status 布局；1512/1524 行定义 GENERIC/SUCCESS 均为 0；4429–4433 行明确 `sc==SUCCESS && sct==GENERIC`。phase 是独立一位，因此 **`status_raw==0` 不是正确成功判据**，phase 为 1 的正常完成也可能成功。

**SUBMIT 是软件提交点，先于写 SQ，更不等于设备已接受。** 634–637 行先记录，650–654 行才复制命令；665–666 行还受 delay_cmd_submit 控制。COMPLETE 则是软件 poll 从 CQ 处理到 tracker 的时刻：887–931 行读取/检查 CQ 后进入 complete_tracker。两时刻之间包含 poll/供给影响，不能解释成纯 SSD、payload 或翻译服务时间。

object 是 request 指针、ctx 是 callback 参数，均不是 host payload 地址。perf 源码 892–904 行确实把 task 作为 API callback 参数，但本席没有完整审查任意分裂/child request 的 callback 重写；perf 自己在 1285–1300 行明确按最大传输/stripe 大小为 children 与 parent 预留请求资源，分裂不是可无视的假设。只有核实未分裂路径后，才可用 ctx 条件关联 perf task。指针还受分配器复用影响。CID/QID 也需分控制器和生命周期，schema 没有 BDF/NSID/完整命令。

## 3. 所选 stock perf 实际提供什么

复用已登记 [官方 perf.c](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/app/spdk_nvme_perf/perf.c)，本地原件路径见 sources.json。本次只定向补读输出、选项、启动/收尾，没有重做 host 映射审查。

- `-L/-LL` 是软件延迟汇总/直方图（1966–1967、2704–2706）；1539–1552 行按回调 tally，2020–2115 行输出吞吐和汇总。**没有逐请求提交—成功状态—buffer 的事件序列。**
- `-G` 设置 debug log（2687–2696），不是 trace 文件初始化接口。全文件 `trace/spdk_trace/trace_setup/spdk_app` 定向搜索未发现 trace 初始化或 mask 配置；main 3232–3399 行直接走 `spdk_env_init`，没有 `spdk_app_start`。因此所选 perf 源码没有提供可确认的 stock trace 启动路径，不能因库含 tracepoints 就宣称该二进制默认有 trace。
- 上述是源码接口核查，未运行二进制，未全面审计所有链接库构造器；不把它夸大成任何部署都无法启用 tracing 的证明。现在既没有该运行身份，也没有实际 trace 文件。
- `task_complete` 1541–1544 行无条件增加 io_completed，`io_complete` 1581–1601 行错误回调最后同样进入该函数。汇总吞吐和直方图不能直接充当成功字节或成功时序。
- 1843–1859 行确有尾部排空，但 `g_elapsed_time_in_usec` 在 1835–1840 行先取主循环退出时间。因此打印出的统计分母不能直接当作包含尾排空的完整有限任务 elapsed。

## 4. 按字段裁定

| 所需字段/关系 | 本次分类 | 明确边界 |
| --- | --- | --- |
| 软件 TSC、lcore、QID、request pointer、ctx、CID、opcode、CDW10/11/12、qd、status_raw | 官方现成 schema 字段；本次无实测值 | `common.c:634–637,681–682,1831–1862`; `trace.cpp:197–305` |
| 成功 NVMe completion | 若给真实 trace，可由已核原件计算 | 用 sc/sct 解码；包含失败/重试的总 COMPLETE 数不成立 |
| 软件请求提交至完成间隔 | 有条件可计算 | 完整生命周期、重试/指针复用/跨控制器身份须先解决；非硬件服务时间 |
| NVM read/write LBA | 有条件可计算 | CDW10/11 已有，但命令集、namespace/命令布局须先确定；本席未新增完整命令解码审查 |
| host buffer VA / IOVA / PRP/SGL / mapping generation | 需要新探针或已有独立 join 记录 | 这两事件不含；离线无法解引用 req/ctx 恢复 payload，也不能用 LBA 替代 |
| NSID、控制器 BDF、Requester/domain、实际共同 IOMMU | 需要独立身份/部署证据 | QID 或 ctx 不足以证明共享翻译实例 |
| 成功 callback → 同 task 再提交 | 源码因果规则已知；一般轨迹 join 仍需边界记录 | `perf.c:1560–1601`; 要区分重试、child requests、分配器复用；没有显式逻辑 IO 序号/再提交边 |
| drain 开始、最后应用完成、task/buffer free、下一任务/quiet 决策 | 需要新探针或已有应用边界记录 | COMPLETE 的 qd 在减计数及 callback 前采样（681–712），qd=1 不能证明应用尾部已结束 |
| 完整成功任务字节 | 需要完整 success-filtered trace 加 namespace/任务元数据 | 不能从 io_completed 或直方图反推；尾排空时间另缺 |
| payload 接受/每笔服务、IOTLB miss、PTW/DDR owner、NIC 同时序 | 根本不在这份可得 NVMe schema | 必须另有对应实际测量源，不能由 CPL 猜测 |

“需要新探针”只说明缺口，不是本次实施或运行授权。未来即便把现成库 trace 打开，也只能补最前几项，不能免费补出 buffer/IOVA/DDR 或完整双任务成本。

## 5. 可得性边界与最小交接

本次第一批严格限于三个候选原件/页：pcie wrapper、NVMe 文档、trace.md（404）。补证前已向 root 说明 wrapper 不含事件实现；同版本官方 tree 指出 pcie_common 和 reader 后仅追这些直接路径，最后经 root 明确允许再取一个 status header。tracepoint.md 也为 404，未无限猜路径、广搜数据集或下载整个仓库。NVMe 文档的 perf 命令只是示例，不是运行记录；test/iscsi_tgt/trace_record 只在文件清单中看到，未读取、运行，也未用它冒充 perf 实测。

**取得实际 SPDK trace：0；取得联合 trace：0。** 这是本轮可得性结果，不能推出“互联网上不存在任何公开记录”。可复用的最小资产是固定 ref 的真实 schema + reader + 成功谓词 + 精确缺项。若后续收到现存真实 trace，先核来源身份、启用方式、完整性和上述 join 能力，再决定是否需要新采集；没有这些输入时不填写时间、成功字节或隐藏服务态。

本席仅写 events 目录；0 新实验、SPDK 运行、编译、设备/板操作、collector/parser、goal 操作。请求配置为 `gpt-6.1-sol/high`；本席工具未暴露实际模型/推理运行字段，故实际配置标为未知，不以请求参数冒充验收。全部工作由 Codex 完成，未用 DeepSeek 或其他外部模型；最终接受归 root。
