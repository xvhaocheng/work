# 独立测试 top 监测接口

实现文件：`../runner/invalidation_dma_top.sv`。原接口保留；下面只列新增。所有输出只读，不反馈RTL。每次在时钟低相位、`eval()` 后、上升沿之前采样；valid/ready同时为1表示即将在该沿接受。next_state/complete也描述该沿提交。

| 端口 | 宽度 | 含义 |
| --- | ---: | --- |
| `sim_fence_read_done_i`, `sim_fence_write_done_i` | 1各 | 固定pre-fence设备请求cohort已到本模型全局点；root C++分别以DDR RLAST/B回执驱动，首次DECODE后新请求不扩展目标 |
| `dbg_iofence_wait` | 1 | PR/PW测试hook正在WAIT_PRPW状态（编码6）；不关DS或comp推进 |
| `oracle_remove_i`, `oracle_pscid_i`, `oracle_vpn_i` | 1/20/27 | 仿真ORACLE输入，exact host/S1 key在下个上升沿清valid；三层overlay及test top均`ifndef SYNTHESIS` |
| `dbg_cq_next_state`, `dbg_cq_wr_state` | 3/2 | CQ下一主状态；当前写子状态AW_REQ0/W_DATA1/B_RESP2 |
| `dbg_cq_cmd_complete` | 1 | 当前DECODE、WRITE或WAIT_PRPW将回IDLE；只表示此RTL状态机的命令提交 |
| `dbg_iofence_decode`, `dbg_iofence_complete` | 1/1 | 上述操作且opcode=2；AV0/ATS关闭时decode与complete可同拍 |
| `dbg_iofence_av`, `dbg_iofence_pr`, `dbg_iofence_pw` | 1各 | 当前命令结构字段；只有IOFENCE时有意义，PR/PW被原frozen忽略；本轮测试overlay通过cohort输入实现有限AV0/ATS-off等待 |
| `dbg_tl_active`, `dbg_tl_active_did`, `dbg_tl_active_iova`, `dbg_tl_active_ttype` | 1/24/64/4 | TL spill中req_valid及其当前请求；active为0时不能解释owner/data |
| `dbg_tl_next_state`, `dbg_tl_resp_valid`, `dbg_tl_resp_ready` | 3/1/1 | TL下一状态；normal translation响应握手 |
| `dbg_{cq,ptw,ddtw,ds,comp,dma0,dma1}_bus` | 11各 | 下表的AXI握手掩码；dma0=B、dma1=A |
| `dbg_{cq,ptw,ddtw,ds,comp}_ar_addr` | 64各 | 对应接口的AR地址，只在ARVALID有效时解释 |
| `dbg_comp_aw_addr` | 64 | payload完成接口AW地址 |
| `dbg_ds_{ar,r}_id`, `dbg_comp_{ar,aw,r,b}_id` | 4各 | 相应有效通道ID；comp的bit2区分A=1/B=0 |
| `dbg_iotlb_update` | 1 | update_i.update，可能被同拍失效压制 |
| `dbg_iotlb_fill` | 1 | 原RTL update分支生效，等于update&&!inval_vma&&!inval_gvma；在ORACLE最后清除之前 |
| `dbg_iotlb_update_vpn`, `dbg_iotlb_update_pscid` | 27/20 | 原update key |
| `dbg_iotlb_replace_en` | 16 | 当前PLRU选中way；只有fill时才是实际写入目标 |
| `dbg_iotlb_lu_hit_vec`, `dbg_iotlb_plru` | 16/15 | 当前命中way掩码；当前PLRU树 |

| AXI mask bit | 信号 |
| ---: | --- |
| 0 | ARVALID |
| 1 | ARREADY |
| 2 | RVALID |
| 3 | RREADY |
| 4 | RLAST |
| 5 | AWVALID |
| 6 | AWREADY |
| 7 | WVALID |
| 8 | WREADY |
| 9 | BVALID |
| 10 | BREADY |

CQ主状态沿用原编码IDLE0/FETCH1/REGISTER2/DECODE3/WRITE4/ERROR5，新增WAIT_PRPW6仅存在测试overlay；TL状态IDLE0/DDT1/PDT2/PT3/MSI4/ERROR5/COMPLETE6；PTW状态IDLE0/MEM_ACCESS1/PROC_PTE2/ERROR3。既有tag输出覆盖valid/S1/S2/global/VPN/PSCID。

ORACLE与fill同拍同key时，ORACLE比对**tags_n**并最后清valid。若观察者检查fill后的tag，须排除该交集或显式断言本轮交集为0。ORACLE没有修改PLRU树，没有新增空项优先，也不清其他PSCID；它是不可部署的即时移除上界，不是新的CQ或Linux失效语义；后续FQ计时与域失效/fence政策仍与ML-FQ相同。

五份overlay只替换frozen manifest中相同logical_path的`rv_iommu_top.sv`、`rv_iommu_tl_wrap.sv`、`rv_iommu_iotlb.sv`、`rv_iommu_sw_if_wrap.sv`、`rv_iommu_cq_handler.sv`。每份旁边保留`.sv.diff`。`make_monitor.py`从上一轮固定top与frozen源重新生成这些文件；不改生产仓库或旧frozen。

PR/PW测试hook：REGISTER最后beat用已锁存的低64b识别opcode2且bit12/13非零，推迟CQH到真正模型完成。合法AV0/ATS-off的首次DECODE只执行一次；若各请求位要求的done尚未达到则进入WAIT_PRPW6，达到后CQH++回IDLE。AV1+PR/PW或ATS inflight显式报cmd_ill，不宣称支持；legacy PR=PW=0路径保持原CQH推进/DECODE/marker行为。去除全部新增`ifndef SYNTHESIS`块后，五份源与原frozen代码（忽略空白）完全相同，静态检查已实际执行。版本1的原diff归档在`overlay-v1/`，build01快照保持不变。
