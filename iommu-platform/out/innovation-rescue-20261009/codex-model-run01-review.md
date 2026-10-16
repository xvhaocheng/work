# 首次模型执行与根级修订接口

run01实执行exit1，293 scenarios/3569 checks，598 failures；原始source/proposal/result/log/ledger均保留。不是候选机制或生产RTL已有bug的证据，是模型实现失败。诊断在run01-diagnostics.json。

## 根Codex直接诊断

1. do_resume只设置Sim.resumed而没设Flow A.resumed；ctrl_open/ordinary检查后者，故补读从未发。late resume也应从actual returned_at_exit补读。early resume若已经脱离，不能把A.exited清掉后重join仍在途的较后shared offset，否则丢中间拍；区分shared-membership退出与允许solo datapath接收。
2. ctrl_open/ordinary还不应在A尚未实际退出时凭resumed就补读。若resume快到旧短W尚未drain，且根本没跳过A数据，可继续原cohort；只有saved returned_at_exit存在才有补读。
3. ordinary在A solo完成余下也不该被A.exited过滤掉。try_offer与flow_can_accept要分清member-exited和flow runnable；A.paused时旧债仍可返R，resume后独立AW/R可前进。
4. 补读AR不得在原global physical R/skid未退休时当作已发，首版只有一个物理child。控制器可以等待而不把后来的index送A；真实新增physical_AR_count在可启动child时记录。
5. 首版未实现exit后V/A longW expansion而合同要求，worker也已主动披露。把nextWrite当实际字节/拍位置，而不是bands_completed*q当通用地址；用last_offered_end独立记录已退AW末端。A退出/恢复solo后新AW可长至剩余N（不改变既有short AW），closed或共同阶段仍q。这不是随意减少checks。
6. open can_produce已经包含未paused A的offeredW入skid gate，是正确方向。closed_short_long_read必须获相同gate：不在future AW之前锁A进skid，避免以弱人为phantom obligation宣称胜出；naive_long Lockstep原长AW已覆盖可以正常broadcast直到buffer满。
7. 现probe_held_aw/R是手写独立toy变量，不测试Sim控制器。把有限AWREADY delay及A RREADY stalls接到实际Sim/同一个state组件，检查firstVALID header/data稳定；模拟AW在firstoffer入Wowner队列，但W要等它真正接受。每flow<=1 unretiredAW。
8. 新增ingress phantom-member检查、每R handshake assert logical index==actual parent_returned、AW/Wlast/B完成时严格相等。partial/stall时不相等是合理未退休，不冒充complete。
9. expected=True目前会让permanent-pause candidate V未完成也不致suite fail。负例是预期outcome（structural stall）而不是可跳过的坏check；所有检查的ok=false都要导致exit1，包括naive未复现、candidate永久pause未让V完成。反例运行本身不是程序错误，正确expected structural outcome应使check=true。
10. no-pause各策略当前没有suite case；补少量primary no-pause与early resume，N256/q2/B3一个direct尺寸模型也有用。无需新增大量扫描；已有小网格足够。pause boundary按真正请求时序固定对所有控制一致（P0可声明initial short band已经offer的fixture）。

## 要交付的受限修订

只改open_copy_model.py，保留5 strategies/common datapath及现有检查，允许删除不接Sim的self-fulfilling toy probes并替换真实注入。不能删一个失败case、隐藏失败、放宽授权/容量/顺序来制造通过。main全部ok checks都计fatal；schema清楚区别预期stall/partial-live、模型事件时间与平台cycles。

新模型的实际state位估计可只列fields/宽度/最大entries与复用既有字段，不能声称RTL面积。如果考虑新增unresolved，应直说具体功能未验证，不能减少critical合同后说完整PASS。worker无exec，root随后直接核实际修改并run。
