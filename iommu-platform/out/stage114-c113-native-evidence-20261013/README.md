# stage114 同一C113原生证据关导航

2026年10月13日JST。**C113条件保留并补强自然大模型NVMe需求，当前FPGA翻译实验不准入。** 一个工作流、一个已发表0.5T运行见证；固定v0.16.4功能配置与2021原论文运行分列，不跨版本／跨表拼证据。本关口不是论文完成。

1. [完整报告](/home/xvhaocheng/work/iommu-platform/docs/stage114-c113-native-evidence-20261013.md:1)、[稳定裁决](root-result-acceptance.json)、[同卡准入更新](c113-admission-update.json)：实际path/sharedunit/naturalpeer、native结果/byte/quiet、完整普通成本残余仍缺。
2. [一条已发表配置与容量界](published-run-acceptance.json)：Table1/Figure5c single-node0.5T、16参与GPU、MP1、h18432、124层、seq1024、batch/GPU8、参数及optimizer NVMe。nominal p/m/v约6e12B超过CPU1.5TB；不是exactP/H/filebyte或v0.16.4run。
3. [固定代码功能配置／字节](source/config-derived-byte-contract.json)：twoLinear2048/source worldsize1、tmpdir、B4/p1/q8/pipelinefalse、两个16MiB大weight；mature有界256MiB预期AIO/step，非实际counters。test源码6次step调用含checkpoint分支重做，未运行，不宣称6次成功更新。
4. [root26关键源码／原文组](critical/root-source-review.json)、[root亲看Table1](critical/arxiv-v1-page10.png)：默认、整个parameter分组、test完整生命周期、原论文精度/预算与不合并边界。
5. [唯一新席独占报告](workers/published-boundary/report.md:7)、[18原文／视觉组](workers/published-boundary/critical-quotes.json)、[12原始来源hash](workers/published-boundary/source-manifest.json)、[有界访问及失败](workers/published-boundary/bounded-search-ledger.json)。该席只论文/官方补充/hardware边界，root独占code/config/log与最终接受。MSR raw403未声称完整保存。
6. [固定官方源码原始下载](source/download-manifest.json)、[实际issue查询](source/native-record-search-ledger.json)、[原始record筛选](source/native-record-adjudication.json)、[精确sha CI查询](source/frozen-commit-actions-runs.json)。CPU-only issue3024原body/comments及文本完整留存；检索无可用记录不是全球absence/科学否决。
7. [原进入合同](goal-contract-input.json)、[真实goal登记快照](goal-registration.json)、[实际完成回执](goal-completion.json)、[实际账本](execution-ledger.json)、[交付检查](validation/delivery-check.json)、[旧关键输入hash](critical/preserved-input-hashes.json)、`before/`旧状态原样留存。
8. [下一合同](next-goal-contract.json)：inactive，须有同一既有运行的关联nativebundle才继续；没有新原始资料不重复相同公开检索，不重新生成未变卡，不启动系统部署或第二工作流。

保持Sol/ultra、Codex-only、原生上下文默认；无DeepSeek/Claude/Astra、旧席恢复、生产/config/DB/history改动、新实现/build/RTL、GPU/NVMe/板操作或旧实验重跑。原R107/C112及先前关闭范围不变。服务器仅核来源，主实验平台仍Genesys2/CVA6＋四iDMA＋RISC-V IOMMU/Linux。
