## 声明状态成本审计（bounded，非综合后）

### shared_pretl_read.sv
| 行 | 信号 | 位宽/规模 | 分类 | 说明 |
|---|---|---|---|---|
| 33 | parent_q[2][4] | 2×4 entries | 6 entries assigned-but-unused; 2 functional | 仅 slot0 被写入/读取（119, 141, 155, 75-76），其余恒为 0 |
| 34 | pa_q[2], done_q[2] | 32-bit each | functional | 值仅 0/1，位宽过度但功能使用 |
| 35 | src_q[2], dst_q[2] | 64-bit each | functional | 地址保存与比较 |
| 35 | length_q[2] | 64-bit each | assigned-but-unused | 写于 104，从未读取 |
| 36 | active_q[2] | 2-bit | functional | job 活跃状态 |
| 37 | returned_q[2] | 9-bit each | functional | 返回 beat 计数 |
| 38 | offered_q, busy_q | 1-bit each | functional | 物理 child 握手 |
| 38 | child_owner_q | 1-bit | observation | wrapper 的 DIRECT39 日志读取；不控制返回 owner，返回 owner 来自 skid mask |
| 39-41 | child_q 等 | — | functional | child 描述与计数 |
| 42 | detached_q, detach_pending_q | 1-bit each | functional | detach 状态机 |
| 43-45 | skid_q, skid_data_q, skid_pending_q | 1/72-bit | functional | 一拍 skid |
| 46-48 | required, beat_index, eligible_r, return_owner | comb | functional | 组合逻辑 |
| 49-51 | held_ar_q, held_r_q, saved_ar_q, saved_r_q | 148/72-bit | observational assertion | 仅用于 94-97 的 held 检查，非生产功能 |

### q_write_splitter.sv
| 行 | 信号 | 位宽 | 分类 | 说明 |
|---|---|---|---|---|
| 15-18 | parent_q, child_q, live, counts, error_q | — | functional | 写拆分核心状态 |
| 19-21 | held_aw_q, held_w_q, saved_aw_q, saved_w_q | 152/req_t | observational assertion | 仅用于 49-52 的 held 检查 |
| 13 | groups_o[32] | 32-bit | functional barrier | 用于 line 65 的 hold 条件，功能必需 |
| 13 | offers_o[32] | 32-bit | stats | 仅输出计数（line 71） |
| 13 | pending_o | 32-bit | observation port / comb | 由 child_live_q 组合生成，没有额外32-bit寄存器 |

### hit_bypass_parent_translation.sv
| 行 | 信号 | 位宽/规模 | 分类 | 说明 |
|---|---|---|---|---|
| 40 | valid_q[2][4], prepared_q[2][4] | 8-bit each | functional | cache 有效/准备位 |
| 41 | value_q[2][4] | 83-bit ×8 | functional | 8 row 声明，4 live（文档说明） |
| 42 | seq_q[2][4] | 32-bit ×8 | functional | generation 比较 |
| 43-48 | slow 状态, hit_q, hit_resp_q | 118/185-bit | functional | slow/hit 控制与响应保存 |
| 53 | forwarded_q, filled_q, reused_q, prepared_count_q | 32-bit each | stats/observation | 仅统计输出 |
| 53 | cycle_q | 32-bit | observation | 仅用于 $display |
| 54-55 | held_bg_q, held_bg_req_q | 1/118-bit | observation assertion | 仅用于 60-61 检查 |
| 56 | overlap_hits_q | 32-bit | observation | 统计输出 |
| 239 | 类型宽度 | req=118, resp=185, normal=83 | confirmed | 编译期断言与日志一致 |

**边界说明**：以上为声明状态审计，不包含综合后 LUT/FF/timing；`parent_write_i` 是输入端口，未存为寄存器；READ source 与 WRITE target 各走真实 TL 调用。

Codex 核类型：生成头 Vnonblock_actual_dma_system_top.h 中 q-W parent/child 是152bits，source AR148bits，R skid72bits。按声明手算，两个q-writer功能寄存器共758bits（每路2×152+5+4×9+2+32）；shared source 的被功能使用状态1084bits，另有恒零未用header888bits、未用length128bits；helper完整声明1281bits，共3123bits被使用的局部功能声明，另1016bits赋值但无功能使用。这个小计包含72bit skid，不能再把其8B payload重复加进去；未含原iDMA FIFO、TC FIFO、AXI管线、所有观察状态或组合逻辑，也不是综合面积。父tag/固定seq等可能被优化，普通对照也可裁剪，须后续全账与综合。

