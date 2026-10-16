# D-DRAM-BOUNDARY-1：来源关已过

2026年10月15日。普通读前史在固定Ramulator2来源的严格排空后仍能影响下一读，显式warm可能保留、改写或洗去；这不是额外信息、P/PN收益或原生性能验收。下一仅核完整任务准备及write完成域，再决定有限动作比较。

- [root裁决与结果表](root/decision-analysis.md) · [稳定接受](root/root-result-acceptance.json) · [下一最小合同](root/next-minimal-contract.json)（尚未执行）。
- [事前合同](root/dram-contract.json) · [预测](root/pre-run-predictions.json) · [输入绑定](root/run-inputs.json) · [完整输入清单](root/input-manifest.json)。
- [9个实际run](runs/) · [实际执行账](root/execution-ledger.json) · [原始摘录](root/raw-excerpts.json)。8私有＋1原版control，25次读全部回调；不重跑旧实验。
- [来源接口边界](interface/boundary-card.md) · [源码阅读范围](interface/source-read-ranges.json) · [root源码审查](root/source-review.json) · [私有被动记录补丁](implementation/passive-audit.patch)。
- [构建与失败日志入口](implementation/build-report.md) · [核账说明](verification/README.md) · [root实际执行的独立核账](verification/root-verified-report.json)。105全状态快照、50官方命令，重放/passivity全等及4负变异拒绝。
- [本轮文档保护收据](documentation/receipt.json) · [写前五文件快照及hash](documentation/before-write/manifest.json) · [局部新增链接核对](documentation/link-check.json)。
- [上一MQSim轮](../d-mqsim-identity-20261015/README.md)：四完整排空边界GC=0，本版局部停止保持。

本轮2库构建＋2driver最终成功；2driver编译/链接失败和缺cmake前置失败保留。少量读warm不代表M0 warm256通过；WR issue不代表COPY完成。没有生产/硬件/旧实验改动。D主线、I轻量not_ready、A5/A2、MIG1792023314 paused（WNS −0.391ns、598 setup失败、0新加载）和10月15—28日窗口保持。共享研究Codex-only；文档请求Sol/medium，实际模型/强度元数据unknown。
