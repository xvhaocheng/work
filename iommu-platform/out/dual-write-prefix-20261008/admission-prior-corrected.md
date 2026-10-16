## §43 证据勘误：W先于AW、isolate边界与扣费点

范围：仅静态读源，未build/run/test。冻结axi_rt=`50153a346b753dc2bc7723c446656a43db35d02d`，axi依赖=`fccffb5953ec8564218ba05e20adbedec845e014`。仅在out副本读源；E1-E5/E8/E9除下列外维持，不重抄700行。

源URL：
- axi_write_buffer.sv https://raw.githubusercontent.com/pulp-platform/axi_rt/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_write_buffer.sv
- axi_rt_unit.sv https://raw.githubusercontent.com/pulp-platform/axi_rt/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_rt_unit.sv
- axi_rt_unit_counter.sv https://raw.githubusercontent.com/pulp-platform/axi_rt/50153a346b753dc2bc7723c446656a43db35d02d/src/axi_rt_unit_counter.sv
- axi_isolate.sv https://raw.githubusercontent.com/pulp-platform/axi/fccffb5953ec8564218ba05e20adbedec845e014/src/axi_isolate.sv
- 依赖清单Bender.lock（本地sources.json记录来源） https://raw.githubusercontent.com/pulp-platform/axi_rt/50153a346b753dc2bc7723c446656a43db35d02d/Bender.lock

### E7/§2.6 勘误：外部AW前W burst已供给
`axi_write_buffer.sv:95-102`将`{mgmt_valid_aw, egress_aw_valid}`做`stream_join`，只有两者同时有效才置`mst_req_o.aw_valid`。`axi_write_buffer.sv:108-123`的mgmt FIFO只由`ingress_w_last`压入；`axi_write_buffer.sv:149-150`定义`ingress_w_last = slv_req_i.w.last & slv_req_i.w_valid & slv_resp_o.w_ready`。`axi_write_buffer.sv:167`定义`mgmt_valid_w = num_lasts_q > 0`，`:170-177`的WVALID也由`num_lasts_q>0`与`egress_w_valid` join。故在本buffer对外合法提供AW前，对应W burst必已到达WLAST并进入W FIFO；在有序AXI输入且FIFO/token不溢出下，任一外部被接受的destination AW，其整段W已供给，不需要未来source R来生成该committed write的数据。

边界：若childburst超过`WBufferDepth`（`axi_write_buffer.sv:130-146`），可能永远到不了WLAST，从而无法admit该AW；这是上游容量/进度问题，不是下游AW仍待未来R的流式反例。未主张全局isolated单元progress；R存储、B、tail、held、其他shared资源未证无影响。不得写成“全W缓存使R→W继续无关”的普适定理。

### E6/§2.5 勘误：Normal同拍仍连通
`axi_isolate.sv:253-255`每拍默认`mst_req_o=slv_req_i`、`slv_resp_o=mst_resp_i`。`axi_isolate.sv:260-284`的AW Normal分支：未达容量且无转换时，即使`isolate_i`已raise，该拍AW仍按默认连通；只有容量命中才立即cut。AR同理`:328-349`。状态寄存器在时钟边沿更新（`axi_isolate.sv:203-204`）；`:292,:299-301,:310-312,:356,:363-365,:375-377`是组合下一状态选择。`Hold`仅保留已offer的`aw_valid/ar_valid=1`等ready（`:286-292,:351-357`），不丢VALID。`Drain`抑制未来AR/AW，zero并拉低ready（`:294-302,:359-366`）。已有W按`:318-323`继续；RREADY默认随master（`:255`）。顶层`isolated_o`为head/tail OR（`axi_rt_unit.sv:415`）。

### E1/E2 扣费点：on handshake，非after handshake
`axi_rt_unit.sv:278-282`：`aw_happening=aw_valid&aw_ready`，`ar_happening=ar_valid&ar_ready`。`axi_rt_unit.sv:352-386`将这些送入counter；`axi_rt_unit_counter.sv:67-80,84-90`的`delta_counter`在时钟边沿寄存，`budget_en=enable_i & !budget_spent_o & ax_happening_i`，`budget_spent_o=(budget_left_o==0)|overflow`。应写“on handshake（counter边沿寄存）”，不是“after handshake”。

### E3/E8 勘误：budget_left只驱动isolation
`axi_rt_unit.sv:219-220`：`global_isolate=((|r_isolate)|(|w_isolate))&imtu_enable_i`，`isolate_o=global_isolate`；该信号驱动head `:214`与tail `:410`。`budget_left_o`不接`NumPending`、`MaxReadTxns`、`MaxWriteTxns`、`NumOutstanding`。冻结版未证明等于论文评估revision；2023可选throttle语言不能证明冻结代码匹配评估revision，也不能从代码缺席推出novelty verdict。

Codex root已直接核上述原代码；原response及长证据稿保留历史，E7/§2.6、E6/§2.5的结论以本勘误为准。
