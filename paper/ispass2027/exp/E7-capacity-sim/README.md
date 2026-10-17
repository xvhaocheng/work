# E7：IOTLB 容量敏感性仿真

2026-10-17 JST。**12格矩阵已完成并通过验收；不使用 FPGA。**
本目录只修改独立测试副本，生产 RTL、内部仓库和主工作目录均不变。

| 项目 | 身份与范围 |
|---|---|
| IOMMU | 必要修复基座 F5 `85b157ebe71a623e326e51fff7b7126fe77e8eb9` |
| iDMA/CVA6 | `ba8cdd10659a777ccff0080bf020be64b5c1e064` 的既有冻结夹具 |
| 源闭包 | `iommu-platform/out/invalidation-interference-20261017/frozen/source-manifest.json`；每次构建核对既有哈希 |
| 本轮改动 | `inputs/`、`test-overlay.patch`：仅测试顶层容量、被动tag导出宽度和持久映射并发工作负载 |
| DDR模型 | 队首首拍等16周期，随后每拍8B，FIFO读服务；无额外DS延迟 |
| 供给 | 真实iDMA的普通MMIO、每设备一个在途COPY；不同于板测的自主batch前端 |
| 任务 | V固定4096次4KiB COPY；A1/A8/A64分别1/8/64对源/目标页，即2/16/128个翻译条目 |
| 正确性 | 每次COPY逐字节验证、两读/两写突发及B响应守恒、最终静默、无失效；Solo/A1预热后双侧零walk |

16项校准已完成，Solo < A1 < A8趋势成立。仿真A1/Solo=1.76915，
板测DS0约1.89410；仿真A8/A1=1.10741，板测约1.10397。
**仅接受趋势校准，不宣称数值重现板测。** 16/32/64项各
Solo/A1/A8/A64全部完成；`validate.py`直接核对12格的源输入、构建和
运行身份、逐任务/beat/B响应守恒与计时，生成hash绑定的
`analysis-import.json`，供图管线读取完整矩阵。

A8从16项的5122次A walk降至32/64项的0，V时间随之与A1相同。
A64占用128个A翻译，加V的2个条目仍超出64项：A walk从
7810降至6274但未消失。A1的零walk共跑残余仍存在；本矩阵不能单独
区分内存竞争与共享仲裁/队头阻塞。

实际预热为A先执行256次（Solo无A）、V执行64次并再做2次零walk
检查；这不是板测256次批量预热的等同执行。
`v_cycles`为首个V硬件ISSUE到最后V DONE；`a_cycles`沿用原摘要
的最后A DONE相对测量阶段`t0`，含最初MMIO供给，不能当独立A
硬件窗口。原始`jobs.csv`保留各自issue/done/poll边界。

已通过的逐点CSV、逐任务`jobs.csv`、结果摘要和必要构建/运行清单
可在实验途中分批同步。完整事件trace与运行/构建日志保留本机，
不属于用户要求途中同步的论文可用数据，不上传这些日志。
构建对象、可执行文件和生成C++仅本机保留在忽略的`build/`。
每次构建输入、完整命令、工具版本、输出哈希以及运行命令和退出状态
均保留；历史失败或已完成目录不覆盖。

复现与分析：

```sh
python3 prepare.py
python3 run.py --attempt new-calibration --capacities 16 --conditions Solo A1 A8
python3 run.py --attempt new-matrix --capacities 32 64 --conditions Solo A1 A8 A64
python3 analyze.py
python3 validate.py
```

应使用不存在的新attempt名称。上述命令写本目录，不能对既有结果目录
执行覆盖重跑。最终原始证据和比例见`results.md`及`calibration.json`。
容量后残余慢化不能仅靠这个矩阵进一步拆成队头阻塞或内存竞争。
