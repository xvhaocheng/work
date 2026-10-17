# TL/MSIPTW：只读完整生命周期 quiet 与稳定 MSI 来源小批提案

**仅接口/原文/有限计划，未改 TL/MSIPTW、未运行新仿真/构建/硬件。** TC/vendor批已经封存，IOMMU候选基座为 `6082d3bc2e0aeafcf4e86c42ddf1485c778fe941`（TC批待Root最后验收；其TL/MSIPTW字节仍等于已接受N/history源）。同一独立 `/home/xvhaocheng/work-e6-pn-iommu`、`ispass/e6-pn`；本批仅拟改 `rtl/translation_logic/rv_iommu_tl_wrap.sv`、`rv_iommu_msiptw.sv` 两文件。9份固定Git blob/SHA/编号原文在 [`source-map.json`](source-map.json)。不改 DDTW/PDTW/PTW/cache/packages、SW、TC/vendor、top/CVA/constraints，不加源文件/计数bank。

## 接口与原路径兼容

两模块均在参数末尾追加 `parameter bit PnEnable=0`、端口末尾追加 `output logic pn_idle_o`，默认输出0。新quiet只旁听，0新增state，不反馈ready/valid/cache/FSM/原N准入。

**原TL `policy_quiet_o`方程 `:288–299`保留原字节**，不把新增complete-quiet条件塞回该既有接口，保持既有finite使用口径；新增输出才供后续完整域聚合。policy_clear_i不进新quiet，不制造MODE→clear→拒MODE环。Root外围拟议top端口仍为pn_quiet_o（尚未实现），其未来还必须合DS transport、TC、SW等。

## TL只读谓词

在既有主FSM/arbiter→spill/Nhelper/fast-slow response/旧update+inval/DS当前AR/R保护 `policy_quiet_o` 之外，补齐：

```text
pn_idle_o = PnEnable && policy_quiet_o
          && !ddtw_active && !ddtw_init && !ddtw_error && !ddtw_imp_init
          && pn_pdt_idle && !ptw_init && !ptw_error
          && pn_msi_idle
          && !ptw_iotlb_update.update && !msi_iotlb_update.update
          && no AW/AR/W request or B/R response offer on all four walker DS ports
```

- DDTW `active_o`已有（ddtw`:107`），TL`:542`目前未连；仅显式接回现输出。PTW active已有接线TL`:707`且原policyquiet已有检查，不改它的功能。
- PC生成域 TL`:567–654`内 `pn_pdt_idle = !pdtw_active && !pdtw_init && !pdtw_error && !pdtw_imp_init && !pdtc_update.update`，PDTW既有active (`pdtw:100`，TL`:619`目前未连)只接回。**无PC实例分支内部factor=1**，不引用不存在的hierarchy/未驱动默认0。
- MSI生成域 TL`:759–861`内 `pn_msi_idle = actual_msiptw.pn_idle_o && !msi_ptw_init && !msi_ptw_error && !msi_ptw_ignore && !mrifc_update.update`，MSIPTW显式传PnEnable。**MSI_DISABLED无实例分支factor=1**（其req/update原0保持）。MRIF cache是否实例不改变MSIPTW本身存在的事实；BT_ONLY仍需BT idle。
- 不把DDTC/PDTC/IOTLB/MRIFC中的resident tags、PLRU或旧payload当busy；补当前各update写边沿。DDTW/PDTW隐式PTW请求由现active/init闭合，不把可能陈旧的ddt_imp_trans_q/pdt_imp_trans_q单独追加为永久busy。

## MSIPTW只读谓词与尾拍边界

```text
pn_idle_o = PnEnable && bt_state_q==IDLE && pn_mrif_factor
          && !init_msi_ptw_i && !init_msi_mrif
          && !iotlb_update_o.update && !mrifc_update_o.update
          && !error_o && !ignore_o
          && no mem_req AW/AR/W or mem_resp B/R offer
```

MSI_BT_MRIF下 `pn_mrif_factor=(mrif_state_q==MRIF_PTE)`；BT_ONLY未实例子FSM内部factor=1。不要求旧bt/mrif_wait_rlast寄存器或edge_trigger_q全0，不改变其状态/更新/错误/忽略/能力。

**这仍是walker本地生命周期quiet，单独不覆盖外部响应债务**：真实BT首拍可回IDLE（`:291–293`），MRIF不对齐ignore也可回MRIF_PTE（`:425–429`），第二拍可能尚未回来。完整IOMMU pn_quiet必须AND已接受DS `pn_transport_quiet_o`（9713/6082内原件），不能为“完整quiet”修改MSIPTW等待尾拍或重新加计数。已接受DS24拍空窗原件保留，本批新quiet不会替代该保护。

## 唯一功能修正：restricted_msi只限制稳定normal来源

当前TL`:287`只看compileN/runtimeN/restricted_bank；`:1402–1407`因而可把debug MSI误拒绝。最小修订：

```text
restricted_msi = NonblockEnable && nonblock_enable_i && restricted_bank_i
              && !req_data.is_debug && !is_pcie_ats_req && !is_pcie_page_req
```

全部字段来自TL arbiter后的**原spill保存的req_data**（`:198–211`）；现ATS/PR判断`:405–409`精确等于 `PCIE_ATS_TRANS_REQ(8)` / `PCIE_MSG_REQ(9)`。不读当前candidate/fast helper字段，不增加origin状态。真实debug源 `dbg_if_ctl:58–70`固定is_debug1、ttype依读写/执行，`:93–103`GO请求保持至实际响应。

这个修正由既有NonblockEnable/runtimeN门控，**不另用新PnEnable门控功能语义**；PnEnable仅管quiet旁听。S/P/N0原restricted为0，所有旧路径保持。N/PN normal遇MSI仍原error260，DTF0经真实fault接口，DTF1只error，msi_ptw_init门仍关闭，无新walk/fill/MRIF事务；不改原ERROR/COMPLETE优先级。debug继续原BT翻译，MRIF-debug命中按原TL`:1444–1449`报错语义；不是普通MRIF成功。全局MSI_BT_MRIF/PC/debug和目标ATS0不降级，不新增完整MRIF仲裁/回退。ATS1不是本轮支持声明，ATS0 top原tie-off不改。

## 有限实际DUT接受建议（未运行）

1. 真实TL/cache/DDTW/PDTW/PTW/MSIPTW、实际dbg_if_ctl GO→request（reg结构输入test-interface，不声称本批跑完整MMIO/SW），以及独立原gold闭包。PnEnable0/1对6082原TL/MSIPTW比较所有原外部接口/HPM/fault/cache更新；N0/fullmask同时保持已接受85b gold口径。原policyquiet表达式字节断言，新增quiet不进入旧sequential AST/旧数据依赖，XML证明0新增state/正确MSI/PC生成分支。
2. 用真实DDTW/PDTWactive与主TL/heldresponses，覆盖地址接受后memory当前VALID0、隐式PTW/相应cache update；currentinit/update/fault/DSheld阻quiet，完全排空有正例；InclPC0/MSI_DISABLED/BT_ONLY无实例内部factor1均可idle，defaultoff输出0。静态参数按实际cfg，不能只模块名判断。
3. 真实MSIPTW BT首拍→尾拍≥16拍及MRIF首拍→NOTICE_PTE/ignore/error/两拍：本地idle若已真仍AND实际DS guard为0，尾拍外部退休而真实Rspill未交时仍非quiet。不得把旧已经接受的DS证明删掉或换公式，不改walker功能来制造合格。
4. normal受限MSI DTF0/1，真实TL fault/error260、无MSIPTW init/无walk/无IOTLB或MRIFCfill，保留既有N原负例。debug BT与MRIF-debug原语义，基准是**6082原TL runtimeN1但restricted_bank_i=0的原shared调试路径**（相同N仲裁/response控制，避免把85b无N准入的不同预取时序硬混成等价）；N0再对85b原完整gold。新candidate runtimeN1/restricted1只应在稳定debug/ATS来源解除原新增限制，不改变normal域。
5. 稳定来源交错：slow normal MSI已在req_data时扰动当前candidate.is_debug/ttype，必须仍error；slow真实debug req_data时当前candidate置normal，也不能误拒绝。ATS8/MSG9仅作稳定字段/来源predicate有限接口检查，若在ATS0 harness驱动明确标test-interface，不声称生产ATS1支持。probe/trueNaccept原代码不改，不重复做性能/长矩阵。

每个compile180s/jobs2、run/XML各30s、新目录保存执行源码/commit dirty/hash/命令/原始失败；后续组件小commit+6082 prerequisite bundle，顶层只提交任务目录材料。代码批需Root接受本短提案后才开始；无fullbuild/board授权。
