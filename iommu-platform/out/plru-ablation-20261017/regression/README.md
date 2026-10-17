# 模块回归与独立策略检查

这批是主机上的Verilator RTL仿真，不是FPGA、综合或板测。记录中的命令、输入SHA256、退出码、原始stdout/stderr与编译日志都保存在各子目录。没有重跑与IOTLB替换无关的AXI边界大规模回归，也不把以下范围称为整个IOMMU所有功能已穷尽验证。

| 检查 | V0 | V1 | V2 |
| --- | --- | --- | --- |
| 原`research_iotlb_test.cpp`完整刺激 | 703比较，0失败 | 703比较，0失败 | 703比较，0失败；策略参考适配 |
| 原`composition_iotlb_test.cpp`默认完整刺激 | 25,834比较，0失败 | 25,834比较，0失败 | 25,834比较，0失败；策略参考适配 |
| 原`nonblock_tl_test.cpp`完整刺激 | PASS | PASS | PASS |
| 原`composition_tl_test.cpp`完整刺激 | PASS | PASS | PASS |
| 新增独立masked策略检查 | 339,374检查，0失败 | 332,468检查，0失败 | 567,536检查，0失败 |

已有四套回归结果见[results.json](results.json)和[integration-results.json](integration-results.json)，masked检查见[masked-results.json](masked-results.json)。V0/V1两套IOTLB回归的C++、top及旧参考均未适配；它们的full-mask/默认路径与原tree-PLRU相同。两套TL集成回归三个变体全部使用原C++和原top；只重定向IOTLB源文件和输出目录。`nonblock_tl`覆盖快命中与阻塞、DDTC miss、写权限拒绝、DDT/leaf期间失效、冷页回退、写命中与响应反压；`composition_tl`覆盖四种分区/提前命中使能组合、并发probe时fill归属、quiet、clear及重新预热。

V2测试适配按以下层次处理，没有删除查找、页大小、全阶段失效、tag/content、逐边沿next→q、保护分区保留、one-hot、mask边界或功能输出检查：

1. 原`137f5e5...`参考保留其查找、tag/content和失效源码，只将替换改为独立64位时间戳LRU；被测RTL使用的是不同的rank压缩全序。原测试继续逐evaluation比较功能输出。
2. 原“victim等于PLRU位路径”两个断言改为“victim等于独立时间戳LRU参考”；composition full-mask fuzz另外逐evaluation比较实际victim与时间戳参考。
3. 原“连续两fill无hit会复用同victim并逐出首tag”是PLRU特有预期；V2改为“仍有空项时两fill保留两tag”。其余stimulus完整保留。旧PLRU寄存器继续被动维护，原测试对其无update/miss/失效副作用检查仍执行，但不把这个被动树当作LRU状态。

全部适配diff在[../diffs/](../diffs/)，包括reference/top/testbench；V0/V1空diff明确表示原文件原样。历史H1探针与partition-local反例中按原位号和原树编码写死的预期没有当作V1/V2测试，改由下面的实际策略检查覆盖其相关语义。

[masked_policy_test.cpp](masked_policy_test.cpp)使用另一种组织的独立参考：V0为二叉树路径模型，V1为显式递归节点对象，V2为从旧到新的C++条目列表；不是重抄RTL的先序bit索引或rank算法。它从复位开始，通过正常接口执行冷/满填充和12,000次确定性混合操作，每次检查边沿前victim与查找、边沿后全部16个valid/VPN/PSCID。覆盖两个固定mask、跨owner触碰、touch关闭、miss、clear压过fill、失效与同边沿hit+fill；V0/V2还检查零mask不分配。V1的其他mask沿原树回退，由默认回归覆盖full-mask；它没有宣称对任意mask重建近均衡树。

可复现入口是[run_regression.py](run_regression.py)、[run_masked.py](run_masked.py)、[run_integration.py](run_integration.py)。运行失败并未隐藏：integration包装器最初少转发`subprocess.STDOUT`，在RTL构建之前退出1，见[integration-wrapper-attempt01.json](integration-wrapper-attempt01.json)；masked测试驱动最初在main之前静态构造Verilated top，三进程均信号11退出且无RTL运行输出，见各`masked/attempt01/`。移至main内构造后通过，RTL变体没有因此修改。成功后的receipt是当前`masked/receipt.json`；未声称这些测试失败曾经通过。
