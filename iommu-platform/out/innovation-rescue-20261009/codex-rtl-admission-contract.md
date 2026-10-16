# §49 下一 RTL 准入合同：先修实际属性边界，不改变旧结果

2026-10-09，Codex 根直接核模块/原始 benchmark/生成寄存器及 Arm 原文。

## 发现的必要条件

`out/dual-write-prefix-20261008/multi_test.cpp:1040–1046` 当前实际完整 copy 用 CONF=1，即 decouple_rw，cache 字段为0。`idma_reg64_frontend.sv:344,356` 将该 cache 原样用于源和目的；实际生成 `idma_reg64_frontend_reg_top.sv:324–346,534` 的 cache 字段是 CONF[9:6]，不是 hjson 中只写0–2位的旧定义。后续试验必须以实际 build 的生成 source 为准。

Arm 官方 [Channel signals](https://support.arm.com/documentation/102202/0200/Channel-signals) 和 [AXI/ACE IHI0022H A4.3.1](https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/IHI0022H_amba_axi_protocol_spec.pdf) 明确 AxCACHE[1] 为 Modifiable；它允许事务拆分/合并以及单次 fetch 服务多个 read。普通RAM的地址/immutable标签，不能直接绕过 non-modifiable 的总线约束。

因此首个候选原型与全部公平对照都配置正常、不缓存、不提前写响应的 cache=0010，实际 CONF=1 | (2<<6)=129；保持IOMMU各context独立read/write授权和同属性。实际下游是否保留该AxCACHE须在原型观测确认。若 read 不允许合并或 write 不允许所需拆分，就走原独立数据路径，不能偷偷改 device/MMIO 属性。后续对历史 CONF1 的结果只作历史，不混称同条件速度。

## 可复用部件与新控制范围

现成 `axi_burst_splitter.sv:19,99–112` 只拆成 **单 beat**，不是自动 q=2/可调长度 splitter。可复用其 parent/B-error 汇合及计数结构，但首版 q>1 必须明确实现 q-sized children 或采用已有等效能力，不能把名字当功能。

写单独拆分、读保留长 burst；held AW 的所有字段固定，perflow raw logical parent单独保存。每流只允许一个 physical q-group从first-VALID至B退休；从下游 mux first-VALID 推owner提前锁，不能只看AWREADY握手。B不提前伪造，所有原W传完且children B收到后才回应raw parent；单group迟到B期间不offer下一physical group，计其周期成本。

共享只在各自成功检查的同 PA/range/attrs/immutable epoch 中执行；每个 logical R ID/returned/RLAST 独立。共享 skid required mask 在载入时冻结，按sink delivered mask去重；new beat需要每个选择sink的existing W coverage。pause阻止futureA物理first-VALID，保留旧logicalparent与已承诺physical义务。若Vlong W已经first-VALID，earlyresume先deferred drain/EXIT，不重返commonq循环。

## 一次可审阅实验的入口与接受

复用 §43 `multi_actual_dma_top.sv` / `multi_test.cpp` / `build-argv.json`，仅 out 新副本；清楚禁用旧实验模式，而不是默认修改生产 RTL。旧src/context映射分别指不同PA，需在新测试中显式制造 shared PA，仍独立完成两路授权；不同dst不重叠。首个2KiB输入将src/dst置于已准备单页内，避免旧dst3072的跨页 WR准备混淆。之后32KiB只能在真实page/256-beat限制内分段，不允许32KiB单个AR。

root架构决定接口后，DeepSeek只承担有界helper/test与普通实现提案；root直接审concurrency/AXI控制、执行构建、核原始数据。先一个build与有限几个complete copy场景：no-pause、中途pause恢复、heldVALID+earlyresume、交换两流。强tile接受同splitter/q/buffer/recipe/完整属性；closed和independent作辅助条件对照。输出payload、完整parents/B、R/AW/W/TL/PTW、V/A完成、makespan、pause义务、metadata/payload成本。

当前本合同只是新原型的实现规格，未编写/构建该 RTL，未上板。事件模型没有执行真实 AxCACHE 或 IOMMU permission，也没有真实 delayed-B；run03/run04 不能代替这些接受项目。
