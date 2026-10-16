# v1 复审证据

2026-09-09。生产源码未修改。版本与源文件/日志 SHA256 见 `evidence.json`。完整结论见 [复审记录](../../docs/experiment-v1-review.md)。

- `hpm-adjacent-events/`：生产 HPM 单元的相邻类型事件漏计复现，源事件序列经静态 FSM 核查。不是完整翻译集成仿真。`result.log` 记录 DDTW 无低 valid 间隔得 0、有间隔得 1。
- `dma-wrong-source/`：冻结的生产函数抽取及主机模型；模拟 DMA 交换源缓冲，4 字节错误拷贝未检出，8 字节检出。不是 FPGA 运行。

复现程序均按“缺陷仍存在、对照可检出”的预期退出 0，不能将退出 0 标成生产正确性通过。DMA harness 是锁定版本的证据快照；正式修复回归应继续动态抽取新生产函数。

从平台根重建 HPM 复现（输出使用单独 review 目录）：

```sh
verilator --cc --exe --build -Wno-fatal --top-module hpm_test_top --Mdir "$PWD/out/review-v1-20260909/hpm-adjacent-events/build" \
  "$PWD/cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv" \
  "$PWD/cva6/corev_apu/riscv-iommu/packages/rv_iommu_cfg_pkg.sv" \
  "$PWD/cva6/corev_apu/riscv-iommu/packages/rv_iommu_reg_pkg.sv" \
  "$PWD/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_hpm.sv" \
  "$PWD/out/review-v1-20260909/hpm-adjacent-events/hpm_test_top.sv" \
  "$PWD/out/review-v1-20260909/hpm-adjacent-events/hpm_sequence.cpp"
out/review-v1-20260909/hpm-adjacent-events/build/Vhpm_test_top
```

重建 DMA 主机复现：

```sh
cc -std=gnu11 -O0 -g -Wall -Wextra -Werror -I out/review-v1-20260909/dma-wrong-source out/review-v1-20260909/dma-wrong-source/harness.c -o out/review-v1-20260909/dma-wrong-source/harness
out/review-v1-20260909/dma-wrong-source/harness wrong_device_chunk4
cc -std=gnu11 -O0 -g -Wall -Wextra -Werror -I out/review-v1-20260909/dma-wrong-source out/review-v1-20260909/dma-wrong-source/harness_chunk8.c -o out/review-v1-20260909/dma-wrong-source/harness_chunk8
out/review-v1-20260909/dma-wrong-source/harness_chunk8 wrong_device_chunk4
```

第二个程序保留相同用例名称，但内部 chunk 已改为 8，预期错误返回 `-EILSEQ`。
