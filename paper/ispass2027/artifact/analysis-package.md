# 离线数值与十图派生包

状态：**DERIVED_REDACTED；实际归档解包重建已通过**。这是已记录论文数据的离线数值重建包，不是原始字节证据归档、会议匿名审查认证或端到端硬件重现。

最终归档位于本机：

`packages/analysis-20261017-215432/analysis-replay.tar.gz`

- 字节数：`1973823`。
- SHA256：`39cb56133cd1f33dba2128c1a4015dfa731a6c8e53f97eb906bf37224551e32b`。
- 包外验收回执：`packages/analysis-20261017-215432/receipt.json`。
- 包外私有映射及适配补丁：该目录的`private-map/`，不在归档内；目录权限0700、映射文件0600。此前试做目录不是最终交付入口。

## 实际包含和排除

保留`paper/ispass2027`相对层次。输入来自明确白名单：B1 `inputs_sha256`列出的12份输入，加失效CSV、历史composition/E3两个最小数值快照、E7完整results/receipt，共**17份数值输入**；另有8个分析脚本、build.sh和requirements，共10份控制/依赖文件。归档也包含实际重建的统计、宏、十图PDF/PNG、来源及验收记录，方便直接查看。

未递归复制board或out目录；不含UART、events、stdout/build日志、BIT/Image/payload、仿真二进制、venv、付费论文或凭据。`records.json`是完整逐试次数值与既有语义记录，含原命令位置的派生定位符；没有内嵌UART日志。E7仅带图9读取的完整CSV及hash绑定receipt，独立RTL编译层由E7便携包负责。

## 受限派生和路径适配

原件保持不变。所选副本中的当前账户、本机开发目录、作者仓库URL，以及明确的host/hostname/JTAG serial字段被替换为中性字符串；所有JSON数字/布尔字段逐项核对，CSV科学值和稳定ID保持原样。boot分组、source commits、BIT SHA256、统计公式和seed均保留。

包内代码仅有两类受限适配：将追溯位置序列化为包根相对路径，并拒绝解析包外输入；说明中的个人路径示例也只在副本中替换。**未更改统计公式、样本选择或seed。** 原/派生字节哈希、具体删改以及适配diff仅保存在包外private-map；包内有`DERIVED_REDACTED.json`和派生文件清单，不宣称派生件等于原始字节。

保留的科学commit、hash及数据指纹仍可能与公开来源关联。扫描范围是当前用户名、原开发目录、作者仓库URL及显式设备/账户字段；通过该扫描不等于通过会议匿名审查。发布前仍由Root审核这个具体派生结果。

## 实际验收

导出器将输入归档**实际解包到新的临时目录**，用已安装的Python工具环境运行标准`analysis/build.sh`。Python audit hook阻止Python网络连接及包/工具/系统依赖之外的文件打开；这不是操作系统安全沙箱。原工作树仅由导出器读取白名单和接受结果，重建进程的实验输入全部来自解包目录。没有下载依赖或执行硬件/RTL程序。

严格对照通过：

| 验收对象 | 结果 |
| --- | --- |
| 原始试次 | 100，boot分组保持 |
| canonical标量行 | 17404，行数不是试次数 |
| B1统计 | 600项，科学对象精确相同 |
| B1逐试次派生量、表及图中统计 | 精确对象相同；仅排除明确的路径/哈希/原文行号等追溯元数据 |
| 论文数字宏 | 17个，全部值相同 |
| 十图状态与base | 相同：1–2实测，3部分，4–7缺图骨架，8历史独立版本，9–10仿真 |
| PDF实际字体 | 十图最小均8pt，pdftohtml直接检查 |
| 当前用户名/开发路径/作者repoURL | 包内容扫描通过 |
| 日志、私有映射、环境及二进制进入归档 | 无 |

导出席的第一次干净复制验收与输入暂存归档解包验收产生相同的交付归档
SHA；完成后该席停止写入和重建。

Root最终审查又针对**含生成物的最终 `analysis-replay.tar.gz`**实际解包并
执行一次标准构建，避免将导出器内部的输入暂存归档与交付归档混称。
最终包内文件哈希、B1科学对象、图中统计、17宏、十图状态/base、图9的
12格配置及实际8pt字体检查全部通过，退出0。补充回执保存在包外
`local/final-archive-check01/verification.json`；对应日志仅本机保留。

## 使用和重建

工具依赖需要另行准备：Python 3.10及以上、requirements中的NumPy/Matplotlib，以及Poppler的`pdftohtml`用于实际字体检查。本次复用了已有隔离环境的工具路径；**没有声称完整依赖已vendored**。不带LaTeX全文构建或RTL编译入口，它们分别属于论文包和E7包。

将最终归档解包到一个空目录并保持目录层次，然后运行：

```sh
ANALYSIS_PYTHON=/path/to/prepared/python bash paper/ispass2027/analysis/build.sh
/path/to/prepared/python paper/ispass2027/analysis/scripts/baseline_extra.py --check
```

包中README说明可选的Python audit hook环境设置。完整导出/验收入口是本仓库的`artifact/build_analysis_bundle.py --python /path/to/prepared/python`，它拒绝覆盖已有输出目录；Root不需要为已验收归档再次运行它。

有些历史`raw_path`只保留中性追溯位置，完整历史table producer原件不在最低包内；其实际使用操作数已由canonical表和两个快照提供。无日志情况下不能声明从硬件到论文的端到端重现，也不能补认证历史实际加载BIT字节。

逐文件原闭包、许可证和非便携开发脚本路径证据见[输入审计](replay-input-audit.md)及机器清单；冻结RTL的94 source、10 include、10 license属于独立编译层，不是本数值包的隐式依赖。
