## 本轮新增交接段（供 root 前置保存；旧正文由 root 原样保留）

### 归属
- **设计/关键核查**：Codex root 及两名关键设计席（walk_preemption_design、permission_lifecycle_design、endpoint mechanism 设计）亲自完成机制、证明、手算、成本与条件裁决，并直接核查关键原文与源码。
- **整理/起草**：DeepSeek 思考模式负责整理源码证据并起草本普通文档。
- **审核/保存**：Codex 审核并实际保存；Claude 调用 0 次。

### 本轮边界（务必保留）
- 本轮 0 模型、0 RTL/Linux 改动、0 构建、0 上板；旧 29 次 COPY 仅引用，不算本轮验证。
- 未执行任何仿真/综合；**未测 LUT/Fmax/实际周期/Linux CPU 成本**；所有手算数均为归一化服务时间或声明假设，不是平台 cycle 或成功实验。
- 未取全文：Marching Page Walks、NB-Walker 论文全文，ASPLOS 2016 True IOMMU Protection（fetch timeout）；SDM 未重新读完。

### 下一批（判别顺序，非承诺）
1. A：先核同合同信任/权限与成本账（对普通静态端点分段 cursor / rIOMMU / 逐页源完成 + 预装 gate / 双缓冲），找到可检验差异或归档主张。
2. A：过门后接 M1/M2（复用现有事件模型，先成功路径范围 + 真实返回）。
3. C：先出手推差异点，否则不启动 M8/M9。
4. B：不再建模；只作两 context 普通 walker 强对照。

### 交接入口
- 权威：`out/higher-model-mechanism-20261009/round01/root-result-acceptance.json`、`root-range-complement-design.json`、`root-cost-and-gates.json`。
- 细节：`root-primary-review.json`、`root-source-review.json`、`walk-critical.json`、`endpoint-design.json`；未采用手推 `lifecycle-redesign.json`。
- 关键源码（实现者先读指定行，不扫描仓库）：`cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:170-360`、`idma_backend.sv:519-532`、`frontends/register_64bit/idma_reg64_frontend.sv:323-340,478-554`、`fpga/src/ariane_peripherals_xilinx.sv:1255-1277`、`riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:330-390`、`riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:90-148,192/218,384-405`。
