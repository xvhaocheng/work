# 所有权、接口与完成合同

这是CO1的逻辑设计，不是生产ABI/RTL实现或板端安全证明。原生容量和源0/1并发保持，Q4登记槽可顺序复用两个源。

| 事件 | 翻译用途状态 | task / 数据 / buffer状态 | 必要接口与费用 |
| --- | --- | --- | --- |
| 可信登记且完整校验 | 安装尚需翻译用途；没有许可 | 尚未发布；元数据owner保留range、DAG、source/job关联 | CPU登记与验证、地址空间/epoch获取、完整范围检查、原子发布；不足则整体普通回退 |
| pre-NEXT_ID软件选择或实际合法ADMIT/HOLD | 可更新性能保护建议 | 尚可延后发起；寄存器/functional ID持有规则照原生 | 软件调度/配置/轮询或新增硬件控制路由；COUNT不撤回 |
| TL真实接受 | 用途仍活 | TC/正常TL拥有完整稳定请求；一bank一生命周期 | 关联必须含source/full-ID、RW、range/序号、任务代；bank位不能免费区分多个job |
| TL response真实握手 | 结果被TC持有，但本卡仍保留用途 | TC持有物理/错误结果，待完整地址移交 | 反馈读端口/仲裁/事件记录；不提前回收task或缓存结果owner |
| 最后登记用途的完整物理AR/AW真实交接 | 清对应尚需翻译用途；仍有其他task用途则仍保护 | 原生spill/目标拥有地址与后续R/W/B债务；数据buffer不释放 | 单次exactly-once阶段反馈，任务/最后用途关联、顺序与epoch检查，原子更新；不把内部/外部端点混为一处 |
| 原生最后B且后端done | 不凭done倒推漏记的最后用途；无精确阶段反馈的保守版可到此清用途 | 已完成copy，但consumer/cache/目的buffer可能仍活 | 实际B错误、R/W/B完整性、done匹配；不能读super_last当job-final证明 |
| CPU/设备consumer真实完成、cache交接、最后owner解除 | 可清剩余保守用途 | 按合同释放逻辑buffer；物理回收另要映射/互连完成合同 | 消费/metadata操作、cache flush/refill、轮询/确认；当前不是免费硬件done |
| 未发布任务取消 | 撤销其尚需用途 | 没有本任务新R/W/B债务；实际已持有buffer仍按owner处理 | 可信取消、registration generation和任务状态一致更新 |
| 已发布任务取消/fault | 关闭新优化；不能假装最后用途成功 | 阻止依赖任务当成功消费；保留旧R/W/B/error drain与buffer隔离 | 原生错误终结、取消/超时排空、隔离/恢复费用；本卡不声称所有生产fault活性已验 |
| 映射失效/remap或epoch过旧 | 清保护提示、拒绝旧登记产生新优化；正式invalidate优先 | 已接受PA不因epoch自消；物理页保留到正式排空 | IOTINVAL/IOFENCE与互连排空/软件更新，generation不能替代这些步骤 |

## 两种释放必须分别证明

缓存保护解除只影响未来victim选择，不清权限、不改PTE、不释放物理页。用途清零表示不再需要旧缓存内容；已经取得的完整地址/许可由真实下游owner持有。若未来可能重放、还有未登记burst、跨页集合不完整或更新代不一致，则这项用途不能提前清零，退到task真实完成的保守普通处理。

只有正式授权检查可以让访问成功；pin、任务ID、release_path和snapshot都不作permission-bearing结果。当前research snapshot含tag/global/页大小但不是完整授权内容或epoch同步协议；即便用于性能查询，也必须支付读取/同步费用并处理更新并发。

## 当前接口的硬边界

- source0/1才有现HOLD；现HOLD只支持严格64B，不能作为一般4KiB controlled launch。
- 当前banked模式与旧非零research role互斥，旧ResearchPort ADMIT不自动是banked下的四源免费准入。
- 生产软件在NEXT_ID触发之前可以延后供给；通用硬件任务门和逐用途反馈、登记/epoch协议仍属于新增接口。
- TC保存TL结果后仍在COMPLETE等待真实地址接受；因此用途退休选这个保守事件。不能把TL response、内部spill移交、外部AR/AW接受三者全部写成同一握手。
- backend NO_ERROR_HANDLING的super_last为元数据，不等于task末次完成；copy done来自w_dp_rsp_valid与w_last_burst，transport的w_dp_valid是B有效，wrapper固定response ready。错误、consumer及cache完成仍需另外契约。

用途退休的保守端点明确为TC的`axi_comp_req/axi_comp_resp`完整地址交接（COMPLETE使用它），不冒充DDR外部接受：成功的`spaddr`已写入完整header，之后原生demux/cut/spill继续持有；本轮要求`trans_resp_data_q.error==0`且最后用途可信。该端点后不再需要IOTLB内容，但映射撤销/物理回收仍需正式排空。source侧相同数值的AR/AW handshake不作为此回调。生产`dma_core_wrap.sv:319`明确选择NO_ERROR_HANDLING、backend response.error固定0，故原done不能作完整成功/错误收据；新增error callback与软件隔离是双方共同未实施费用。

## 公平与循环等待

保护是可牺牲的性能建议。无另一合法ready任务时，或者服务释放链/公平任务时，不等未ready后继解除保护，回到相同缓存的普通victim。解除保护不会帮助违反原生read-admit的任务越过W owner；此类请求继续遵守原控制。给定正常有限任务、可信完整DAG、原生数据路径能推进与公平目标返回，CO1不新增“满pin等consumer、consumer等新页”的等待边；不由此宣称整个production系统无死锁。
