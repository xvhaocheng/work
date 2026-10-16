# Research v1 首次上板：第4次停止

2026-10-02，按交付运行一次 `platform-research all /tmp/research-v1-01`。Linux 6.5.0-rc2-00090-ga8d28cd44f35，使用 next-experiment-20261002/delivery 配套 BIT 与 payload。先前已写 SD，本次未重新编译或修改被测代码。

- 前3项 short-e2-w0/w1/w16 的 CLI exit0，runner计为3项完成；未进行独立JSON语义验收。
- 第4项 short-cost-solo CLI exit1，stderr原文：`idma_research: run: Invalid argument`。
- 实际参数：`--cost --a-pages 0 --a-gap-cycles 0 --transfers 64 --warmup 8 --timeout-ms 10000`。
- runner final_exit=1，cases_done=3，total=84，collection_failed=0；主机捕获RESEARCH_HOST_EXIT=1。4项尝试，80项未运行。不能称完整实验通过；错误根因未诊断。
- 未重跑、未跳过失败、未扩大矩阵。Linux保持运行，随后只取回整个原始结果目录。
- JTAG首次默认6MHz未完成配置（DONE0）；按此前成功设置1MHz加载同一BIT成功，IR done1。串口包含第二次加载后的完整SD/Linux引导和实验输出；此前已有少量零字节，不清理原日志。
- 交付来源：Dell `/home/xvhaocheng/work/iommu-platform/out/next-experiment-20261002/delivery/`；板端归档和传输报告见collection/。
- 本轮按用户“不要额外工作”要求直接执行已有工具，未调用DeepSeek、未新增验收或根因分析。
