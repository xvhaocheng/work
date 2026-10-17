# 本机已准备产物

2026-10-17 JST。用户追加允许为准备产物继续编译；本轮仅做源码包编译及
保存数据的离线重建，没有使用 FPGA、重跑 RTL 模型或发布投稿产物。
所有归档和新二进制均保存在忽略的 `packages/`；Git 只同步生成器、输入
审计和验收报告。原论文、原始试次及历史身份清单未改写。

| 产物 | 本机入口（相对此目录） | 已执行验收 |
| --- | --- | --- |
| 论文源码及独立 PDF | `packages/paper-source-20261017-02/paper-source.tar.gz`、`compiled.pdf` | 新目录解包编译为8页；渲染及TeX输入闭包检查通过 |
| 离线统计与十图 | `packages/analysis-20261017-215432/analysis-replay.tar.gz` | 100试次、600统计、17宏及十图状态/数值一致；实际最小字号8pt |
| E7 可移植编译源码 | `packages/e7-portable-20261017-01/local/e7-portable-20261017-01.tar.gz` | 冻结114文件完整；新解包目录16/32/64项均编译通过 |

完整哈希、工具版本和证据边界分别见 [论文包报告](paper-package.md)、
[分析包报告](analysis-package.md)、[E7编译报告](e7-package.md)。
[输入审计](replay-input-audit.md)区分最低数值闭包与完整历史证据。

从独立工作树根目录重做时，必须选不存在的新输出目录；所需工具及原输入
仍需按各报告准备。不要用下列入口覆盖已验收包：

```sh
python3 paper/ispass2027/artifact/build_paper_bundle.py \
  --output paper/ispass2027/artifact/packages/paper-next01
python3 paper/ispass2027/artifact/build_analysis_bundle.py \
  --output paper/ispass2027/artifact/packages/analysis-next01
python3 paper/ispass2027/artifact/build_e7_bundle.py \
  --output paper/ispass2027/artifact/packages/e7-next01
```

分析包明确标记为 `DERIVED_REDACTED`，私有反向映射不入包。E7 包保留上游
许可证与作者声明。路径/账户扫描通过不等同会议匿名审查通过；当前工作稿
的实验缺口和待定声明均保留。成功编译也不新增板测或仿真性能证据。
