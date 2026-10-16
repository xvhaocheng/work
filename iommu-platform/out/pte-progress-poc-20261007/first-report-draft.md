## 16. B ingress+PTE 进度预算概念验证收口

本轮完成 21 个 system 真实 2DMA 短仿真，固定 64 V/A8/gap0/warm256/PTWlat31，PolicyEnable=1，runtime S/N 分区关，无生产/FPGA/Linux/SD 动作。

架构审定：test-only 单入口 completion AR 代理位于 TC accepted translation/physical AR 之后、shared xbar/DDR offer 门控之前。buffer 策略同容量 3-state；none passthrough 默认精确旧 P91878/PN93480/优先 P79847/PN79484。当前事务标签来自真实 bank_trans_resp 握手+is_read+fastrespbank，不靠 PA 猜测。physicalAW 已承诺或 visible AW 同 bank 宽松豁免，但本测试 exemptoffers=0，复杂排空例外未验证。预算 256 beat；仅 PTW active 且 FAST read other bank 且非 exempt offer 收费；首次向下游 VALID 前 reserve，R 仅减 debt，已有 VALID/payload 不撤回。确认 PTE cross DDR（DS port3 ID0）。progress/newwalk 真实 return 重新计量，period 固定 clock，walk 整个 walk 一次 Q，outstanding 可因 R 再放。无 memory future peek。共享 proxy 形成 HOL，不能称生产 TC PREPARED 或 MIG 实板保证；仅固定一条件/一轮确定性 simulation，含 synthetic CPU、DDR FIFO/priority 代理。无 Formal general liveness/invalidation 验收。

| 组 | case | W | A bytes/cycle |
|---|---|---|---|
| none | S/N/P/PN | 105588/152042/91878/93480 | 2.48/1.72/2.85/2.80 |
| buffer | P/N/PN | 91874/151947/93487 | 2.85/1.72/2.80 |
| PN policy | ingress/progress/walk/outstanding | 92917/92362/91091/92362 | 2.82/2.84/2.87/2.84 |
| period | 64/256/1024 | 92917/92927/92227 | 2.82/2.82/2.84 |
| priority none | P/N/PN | 79847/91860/79484 | 3.28/2.85/3.30 |
| priority buffer | P/PN | 79842/79491 | 3.28/3.30 |
| priority progress/walk | PN | 79486/79495 | 3.30/3.30 |

全部 window v_completed=64, a_completed=64。

结论：普通 priority buffer PN 基础 79491，progress PN 79486 只快 5 cycle≈0.0063%，无单独收益可立题；FIFO progress 比 buffer PN 减约 1.2%，仍慢于 P；period1024 及 walk 更好；outstanding 与 buffer PN 等 W。B 必须停止主方法推荐，不展开生产/BIT/新 matrix。不能说所有场景都不创新或普通方案全球覆盖。创新不能由五周期、一条件差异确认；成本未综合。当前 A8 模型 N 最慢，勿外推所有工作集。

接下来入口只保留“具体完成依赖/普通优先的代价与失效条件”，先拿真实应用地址路径和前作无法覆盖的反例，再决定改 RTL。这只是待核问题，不是新机制立项。性能主线保持，不自动启动下一大题。

验收脚本：21 case 唯一、returncode 0、日志末 PASS、无 SCENARIO-FAIL；JSON fields、W==close-open、V64/A64、iotlb_miss>0、rate=completed*4096/W bytes/cycle。proxy!=none terminal captured==down_ar_fires/empty1/debt0；none captures0/debt0。gate=ingress+budget，oversizeescape0，zero-exempt 不 fail 仅备注。trace 按 case-events.csv 读取 (open,close]，event count 对 window 计数，newcaptured tagbit5==fastread_captures；none arfire nonzero/newcaptured0 正常。progress 阶段 budget 检查 fastgated 非 exempt Σ(len+1)<=256，仅完整 phase；walk 不强制不可观测项。acceptance 不以 B 优于为功能 pass；function/protocol check passed 与 innovation_supported=false 分开。comparison grouped DDR priority bool 由 +ds-priority=1，baseline matching buffer，不拿 buffer B 比 none P 归因。数据在原日志与 measurements.json，source hash 在 source-sha256.json。

链接：../out/pte-progress-poc-20261007/measurements.json，../out/pte-progress-poc-20261007/summarize.py。

