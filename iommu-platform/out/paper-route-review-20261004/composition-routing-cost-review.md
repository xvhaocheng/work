{
  "answer_draft": "这次布线久，最直接的可得证据是资源已经接近满：新报告显示 Fully Placed 为 183014 LUT / 145452 FF，LUT 利用率 89.80%，Slice 50949/50950 = 100.00%。实验RTL确实加了非零开销：dma_composition_control 综合为 5754 LUT / 4171 FF，另有 nonblock/research/cost/e2/observer 等观测路径；所以“实验新增了控制与观测逻辑”可证实。但不能仅凭面积数字唯一证明“route瓶颈就是实验RTL造成”：新旧资源不是严格同阶段，新 183014/145452 是 Fully Placed，旧 v5 final 176476/141170 是 Routed；只能做方向性比较，差值为 +6538 LUT（+3.70%）、+4282 FF（+3.03%），不能用综合模块 5754 LUT 去除以 placed/routed 全芯片增量当精确比例。四 mode 复用同一 BIT，P 固定 16 条，DMA0/1 既有 512 拍 buffer 本次未增，也不是“四模式=四倍硬件”。结合实际 route 日志，global iteration 2 冲突 122736→59514→17402→1247→666，当前 1h16，说明初期拥塞很高但迭代中快速下降。更稳妥的结论是：高占用率与局部拥塞使布线耗时偏高，实验控制/观测增加非零开销，但不能断言它是唯一 root cause，也不能断言原方法优化失败。",
  "evidence": [
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 10,
      "fact": "新 composition 报告 Design State: Synthesized。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 25,
      "fact": "新 synth top：185607 LUT / 148905 FF。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1268,
      "fact": "dma_composition_control 综合为 5754 LUT / 4171 FF。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1269,
      "fact": "dma_nonblock_observer 综合为 2245 LUT / 1305 FF。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1270,
      "fact": "dma_research_observer 综合为 11326 LUT / 10448 FF。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1272,
      "fact": "dma_research_cost_groups 综合为 3196 LUT / 3530 FF。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1273,
      "fact": "dma_research_e2_timing 综合为 343 LUT / 395 FF。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1267,
      "fact": "dma_controlled_launcher 综合为 717 LUT / 1018 FF。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/ariane.runs/impl_1/ariane_xilinx_utilization_placed.rpt",
      "line": 10,
      "fact": "新 placed 报告 Design State: Fully Placed。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/ariane.runs/impl_1/ariane_xilinx_utilization_placed.rpt",
      "line": 35,
      "fact": "新 placed：Slice LUTs 183014，利用率 89.80%。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/ariane.runs/impl_1/ariane_xilinx_utilization_placed.rpt",
      "line": 40,
      "fact": "新 placed：Slice Registers 145452，利用率 35.68%。"
    },
    {
      "file": "out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/ariane.runs/impl_1/ariane_xilinx_utilization_placed.rpt",
      "line": 75,
      "fact": "新 placed：Slice 50949 / 50950 = 100.00%。"
    },
    {
      "file": "out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 10,
      "fact": "旧 v5 N final 报告 Design State: Routed。"
    },
    {
      "file": "out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 25,
      "fact": "旧 v5 N final routed top：176476 LUT / 141170 FF。"
    },
    {
      "file": "out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1267,
      "fact": "旧 N routed：dma_controlled_launcher 728 LUT / 1018 FF。"
    },
    {
      "file": "out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1268,
      "fact": "旧 N routed：dma_nonblock_observer 2157 LUT / 1305 FF。"
    },
    {
      "file": "out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1269,
      "fact": "旧 N routed：dma_research_observer 11481 LUT / 10286 FF。"
    },
    {
      "file": "out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1271,
      "fact": "旧 N routed：dma_research_cost_groups 3139 LUT / 3530 FF。"
    },
    {
      "file": "out/migration-4dma-n-hit/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt",
      "line": 1272,
      "fact": "旧 N routed：dma_research_e2_timing 344 LUT / 395 FF。"
    },
    {
      "file": "out/n-prototype-20261004/hit-v5-build-acceptance.json",
      "line": 7,
      "fact": "旧 N 验收：WNS 0.177 ns，WHS 0.038 ns，WPWS 0.062 ns。"
    },
    {
      "file": "out/n-prototype-20261004/hit-v5-build-acceptance.json",
      "line": 16,
      "fact": "旧 N 验收：slice_luts 176476。"
    },
    {
      "file": "out/n-prototype-20261004/hit-v5-build-acceptance.json",
      "line": 17,
      "fact": "旧 N 验收：slice_registers 141170。"
    },
    {
      "file": "cva6/corev_apu/fpga/src/dma_composition_control.sv",
      "line": "89-119",
      "fact": "新观测计数器索引定义：V/A/O lookup/hit/miss、fast、gap、fill/repl/part/inval/first_bad/fast_N；cnt_q[40]/cnt_d[40] 为 40 个 64 位计数器。"
    },
    {
      "file": "cva6/corev_apu/fpga/src/dma_composition_control.sv",
      "line": "124-136",
      "fact": "cache 双快照：plru_open/close、pscid_v/a、seen_lookup_v/a、entries_open_q/entries_close_q 各 16×128 bit。"
    },
    {
      "file": "cva6/corev_apu/fpga/src/dma_composition_control.sv",
      "line": "261-274",
      "fact": "OPEN 采 entries_i/plru_i，CLOSE 采 entries_next_i/plru_i，作为窗口前后双快照。"
    },
    {
      "file": "cva6/corev_apu/fpga/src/dma_composition_control.sv",
      "line": "280-419",
      "fact": "窗口 active 时对 lookup/hit/miss、fast accept、gap、fill、replace、partition、invalidate、first_bad、fast_N 计数。"
    },
    {
      "file": "cva6/corev_apu/fpga/src/dma_composition_control.sv",
      "line": "465-518",
      "fact": "rdata_o 为 128 index case 读 mux，其中 64-95/96-127 读取 OPEN/CLOSE 快照。"
    },
    {
      "file": "cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv",
      "line": "35-37",
      "fact": "IOTLB 新增 policy_clear_i/policy_mask_i，注释明确为 experimental replacement allocation only；不改 lookup/permission。"
    },
    {
      "file": "cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv",
      "line": "336-385",
      "fact": "P 分区为 mask 约束 tree-PLRU；allocation_mask = PolicyEnable ? policy_mask_i : all ones；全 mask 等价旧 PLRU。"
    },
    {
      "file": "docs/composition-v1-spec-20261004.md",
      "line": 2,
      "fact": "主 profile migration-4dma-composition，16 IOTLB 条，4DMA；DMA0/1 同 512 拍缓冲；mode 0/1/2/3 = S/N/P/PN；P 固定 low2 保护 DID，其余 14。"
    },
    {
      "file": "docs/composition-v1-spec-20261004.md",
      "line": 3,
      "fact": "P 选择为 mask 约束 tree-PLRU；全 mask 逐拍等价旧 PLRU；P 关闭总 16 条；PolicyEnable 默认 0，旧 profile 功能不动。"
    },
    {
      "file": "docs/composition-v1-spec-20261004.md",
      "line": 46,
      "fact": "16short = 4mode × Solo/A1/A8/A16，共 172；主表同物理 BIT 同 boot 四 mode。"
    },
    {
      "file": "（用户提供的运行日志，无文件行号）",
      "line": null,
      "fact": "Route 35-447 congestion priority routability；global iteration 2 冲突 122736→59514→17402→1247→666；当前 1h16。"
    }
  ],
  "table": [
    {
      "category": "full-chip",
      "item": "新 top",
      "stage": "Synthesized",
      "value": "185607 LUT / 148905 FF",
      "source": "new synth line 25"
    },
    {
      "category": "full-chip",
      "item": "新 top",
      "stage": "Fully Placed",
      "value": "183014 LUT / 145452 FF；LUT 89.80%；Slice 50949/50950 = 100.00%",
      "source": "placed line 35, 40, 75"
    },
    {
      "category": "full-chip",
      "item": "旧 v5 N final top",
      "stage": "Routed",
      "value": "176476 LUT / 141170 FF",
      "source": "old routed line 25"
    },
    {
      "category": "full-chip delta",
      "item": "新 placed - 旧 routed",
      "stage": "Placed vs Routed，非严格同阶段",
      "value": "+6538 LUT（+3.70%），+4282 FF（+3.03%）",
      "source": "computed"
    },
    {
      "category": "new observation - control",
      "item": "dma_composition_control",
      "stage": "Synthesized",
      "value": "5754 LUT / 4171 FF",
      "source": "new synth line 1268"
    },
    {
      "category": "new observation - counters",
      "item": "40×64 bit 窗口计数器",
      "stage": "RTL source",
      "value": "V/A/O lookup/hit/miss、fast、gap、fill、repl、part、inval、first_bad、fast_N；cnt_q[40]",
      "source": "dma_composition_control.sv lines 89-119, 280-419"
    },
    {
      "category": "new observation - cache dual snapshots",
      "item": "OPEN/CLOSE 双快照",
      "stage": "RTL source",
      "value": "entries_open_q/entries_close_q 各 16×128 bit；plru_open/close；pscid_v/a；seen flags",
      "source": "dma_composition_control.sv lines 124-136, 261-274"
    },
    {
      "category": "new observation - read mux",
      "item": "rdata_o 读 mux",
      "stage": "RTL source",
      "value": "128 index case，含 64-127 快照读出",
      "source": "dma_composition_control.sv lines 465-518"
    },
    {
      "category": "P partition",
      "item": "rv_iommu_iotlb PolicyEnable/mask PLRU",
      "stage": "RTL source",
      "value": "PolicyEnable 默认 0；mask 约束 tree-PLRU；全 mask 等价旧 PLRU；P 固定 16 条",
      "source": "rv_iommu_iotlb.sv lines 35-37, 336-385；spec line 3"
    },
    {
      "category": "inherited N",
      "item": "dma_nonblock_observer",
      "stage": "old Routed / new Synth",
      "value": "2157 LUT / 1305 FF -> 2245 LUT / 1305 FF",
      "source": "old line 1268；new line 1269"
    },
    {
      "category": "inherited N",
      "item": "dma_research_observer",
      "stage": "old Routed / new Synth",
      "value": "11481 LUT / 10286 FF -> 11326 LUT / 10448 FF",
      "source": "old line 1269；new line 1270"
    },
    {
      "category": "inherited N",
      "item": "dma_research_cost_groups",
      "stage": "old Routed / new Synth",
      "value": "3139 LUT / 3530 FF -> 3196 LUT / 3530 FF",
      "source": "old line 1271；new line 1272"
    },
    {
      "category": "inherited N",
      "item": "dma_research_e2_timing",
      "stage": "old Routed / new Synth",
      "value": "344 LUT / 395 FF -> 343 LUT / 395 FF",
      "source": "old line 1272；new line 1273"
    },
    {
      "category": "inherited N",
      "item": "dma_controlled_launcher",
      "stage": "old Routed / new Synth",
      "value": "728 LUT / 1018 FF -> 717 LUT / 1018 FF",
      "source": "old line 1267；new line 1267"
    },
    {
      "category": "four-mode reuse",
      "item": "四 mode 复用同一 BIT，不是四倍硬件",
      "stage": "Spec",
      "value": "16short = 4mode × Solo/A1/A8/A16；P 固定 16 条；DMA0/1 既有 512 拍 buffer 本次未增",
      "source": "spec lines 2-3, 46"
    }
  ],
  "limits": {
    "proven": [
      "新 composition 报告是 Synthesized；新 full 报告是 Fully Placed；旧 v5 N final 报告是 Routed；新旧 full-chip 数字不是严格同阶段。",
      "新 placed top 相对旧 routed top 可得差值为 +6538 LUT（+3.70%）和 +4282 FF（+3.03%），但这是 Placed vs Routed。",
      "实验 RTL 新增了 composition control 与观测/研究路径：dma_composition_control 综合为 5754 LUT / 4171 FF，非零开销。",
      "P 分区固定 16 条；PolicyEnable 默认 0；全 mask 逐拍等价旧 PLRU；四 mode 复用同一 BIT；DMA0/1 没有本次新增 512 拍 buffer。",
      "当前 placed 资源已很高：LUT 89.80%，Slice 100.00%；路由日志显示高初始冲突后快速下降。"
    ],
    "inference": [
      "高资源占用与拥塞会显著拉长 route；实验观测/控制逻辑增加了非零资源，可能加重拥塞。",
      "但仅凭面积无法唯一证明 routing 瓶颈由实验 RTL 造成；冲突快速下降也可能说明 router 正在收敛。",
      "更合理表述是“高占用+局部拥塞使 route 耗时偏高，实验控制/观测增加非零开销”，而不是“实验把 RTL 改复杂所以布线必然慢”。"
    ],
    "unknown": [
      "没有新 design 的 routed 资源报告，无法做同阶段最终面积比较。",
      "没有直接拥塞热图、利用率分布、关键路径或布线迭代耗时分解，无法定量归因。",
      "未提供新 route 最终 WNS/完成状态；不能断言最终时序或布线一定失败。",
      "不能用 dma_composition_control 的综合 5754 LUT 去除以新 placed 与旧 routed 的全芯片 LUT 增量当精确比例。"
    ]
  },
  "recommendation": [
    "现在不改正在构建/运行的 RTL，也不建议中断正在跑的 route；等 route 完成后用 routed utilization、congestion、时序和迭代日志再做归因。",
    "若后续要瘦身，优先检查 dma_composition_control 的快照与计数器：entries_open_q/entries_close_q 各 16×128 bit、cnt_q[40]×64 bit、plru_open/close、pscid_v/a、seen flags；可评估只留必要字段、压缩位宽、单快照或流式输出。",
    "其次检查 rdata_o 的 128-index 读 mux 与 64-127 条目快照读出路径，评估是否合并/分段读取。",
    "再次检查继承 N 的大观测器：dma_research_observer、dma_observer、dma_path_observer、dma_nonblock_observer、cost_groups；这些在旧 N routed 中已是大头，新 profile 在其上叠加 composition 控制。",
    "不要声称原方法优化已失败；P 全 mask 与旧 PLRU 逐拍等价，P 固定 16 条，四 mode 复用同一 BIT。",
    "不要把“新观测必为唯一 root cause”作为结论；当前证据只能支持“实验新增非零开销+高占用/拥塞共同影响 route 耗时”。"
  ]
}