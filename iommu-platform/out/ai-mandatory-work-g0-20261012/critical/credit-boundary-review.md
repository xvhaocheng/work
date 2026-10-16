# stage106：W credit空/满别名的定向审查

**裁决：接受根提出的两处私有修正，作为当前banked、MRIF禁用配置下的空/满判定纠错。** 将W FIFO输出READY与completion W VALID中的`(w_cnt_q != '0)`同时改成`(w_cnt_q != '0 || w_cnt_ovf)`；原AW的`!w_cnt_ovf`选择门控、计数宽度、计数更新、端口、FIFO及模型参数均保持不变。它恢复已有AW授权对应的W排空，不是增加写入额度或新调度机制。本席没有实现、构建或运行DUT；根执行的首案4k-solo0已直接命中并排空满计数边界，见下方实际证据；完整八案仍由根接受。

## 直接原文与实际错误

实际停滞日志第14行：`ACTUAL_W_COUNTER bank0=8 bank1=0`。第5–12行表明外部已有7条AW、第一条仅3个W beat、零B响应；第1–4行显示payload R被阻塞，后面的PTW读也排队。[原日志](../diagnostic-counter/4k-solo0/run.log:1)

诊断driver打印的是实际`i_w_counter.i_counter.counter_q`，不是由DDR日志反推的计数。[实际打印](../private/diagnostic_counter_driver.cpp:262) 外部DDR AW=7与内部计数8不矛盾：credit的增量计于内层`axi_comp_req.aw_valid && axi_comp_resp.aw_ready`，位于后续demux/缓冲之前；它不等于最终DDR口AW计数。log本身没有逐拍列出内部第八条AW去向，故本席不把某一个具体spill位置当作已重放的事实。[增减定义](../../../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:174)

上层`NumOutstandingTrans=8`按bank拆成4；banked模块直接拒绝MRIF配置。[实际拆分](../../../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:143) 每bank `w_cnt_t`与counter的WIDTH为`$clog2(4)+1=3`；`counter`将delta固定为1并调用`delta_counter`。[宽度与更新](../../../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:242)／[counter封装](../../../cva6/corev_apu/riscv-iommu/vendor/common_cells/src/counter.sv:28)

`delta_counter`实际内部寄存器是`logic [WIDTH:0] counter_q`，即4位。`STICKY_OVERFLOW=0`时`overflow_o=counter_q[WIDTH]`，`q_o=counter_q[WIDTH-1:0]`；加减在完整4位内部寄存器上进行。因此当前`{overflow,q}`才是完整credit状态。overflow不是单拍carry脉冲，也不是sticky历史标记。[完整状态](../../../cva6/corev_apu/riscv-iommu/vendor/common_cells/src/delta_counter.sv:28)／[输出与算术](../../../cva6/corev_apu/riscv-iommu/vendor/common_cells/src/delta_counter.sv:54)

| 完整C | overflow | 低三位q | 原W许可 | 修正后W许可 | 原AW选择许可 |
|---:|---:|---:|---:|---:|---:|
| 0 | 0 | 0 | 0 | 0 | 1 |
| 1…7 | 0 | 1…7 | 1 | 1 | 1 |
| 8 | 1 | 0 | **0（错误）** | **1** | 0 |
| 9…15 | 1 | 1…7 | 1 | 1 | 0 |

原WREADY与WVALID仅看低三位；恰在C=8时把满计数当空，禁止W排空，无法产生最后一拍来降到7。[原WREADY](../../../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:220)／[原WVALID](../../../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:359) 修正后C=8可继续W，WLAST握手使完整寄存器8→7，overflow自然清零，原AW门控随后恢复。

## overflow是否也可能表示underflow，或非零低位的“满”

**在脱离协议的任意数值操作中可能。** 完整4位零减一会变15，其overflow=1/q=7；overflow单独不证明“正好满8”。然而当前连接可以证明合法轨迹仅在0…8：

1. reset使完整C=0；clear/load均固定关闭。
2. 增量只来自内部completion AW真实握手。一个bank每次通过IDLE→REQ_TRANS→AWAIT_RESP→COMPLETE串行处理一条地址，只有IDLE的`aw_valid && !w_cnt_ovf`才选择新AW。选中AW时C≤7，翻译/完成期间不可能有另一条AW给该bank增加credit；期间W只能使C减小。因此该AW真正握手后C≤8。[地址选择](../../../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:625)／[一次完成退休](../../../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:714)
3. 减量是`w_valid && w_ready && last`。修正后的WREADY在完整C=0时仍为0，因此C=0时不能减，underflow不可从合法起点产生。WREADY和WVALID采用同一credit谓词，FIFO弹出与内层completion W握手保持等价。
4. AW和WLAST同拍时`en=incr XOR decr=0`，保持完整C不变，符合一次授权和一次消费抵消；其余情况只加一或减一。

所以C=8是唯一合法overflow=1状态；9…15意味着守恒、复位或其他协议条件已经失效，不能用这两处修正宣称恢复非法状态。在9…15中修正前后W许可本来相同，修正没有新增这一问题，也不能替代越界断言。

AW原`!w_cnt_ovf`不变意味着最大已接收未完成W credit仍为8。这个阈值来自现有counter宽度和门控，**并非NumOutstandingTrans=4就是总credit硬上限4**；AW输入FIFO深度4与下游已经接受的AW不是同一个资源。本修正不提高任何现有门控阈值。

## 根验收的最小边界

- 私有源diff恰好两处W空判断；counter实例、AW选择（含E3选择字段）、所有参数/端口/FIFO和行为DDR不变。新build必须实际替换旧wrap源、冻结hash，不能同时编译两个同名模块或只改未被使用的副本。
- 直接证明实际轨迹触及`C=8/q=0/overflow=1`，之后W仍可握手并通过WLAST降回7；不只拿长job最终PASS代替对错误边界的命中证据。
- 每bank从reset全程计`internal accepted AW − internal accepted WLAST = {overflow,q}`，且0≤C≤8；终点C=0。DDR口AW/W/B守恒另验，不能将两个边界的AW数混为一数。
- C=0时W不得弹出；C=8时AW新选择仍受限而W可排空；C=1的最后一个WLAST使C→0；同拍AW+WLAST保持C。可用定向现有观察/小边界检查取得证据，不扩性能矩阵。
- 验W VALID/payload在READY=0时稳定，尤其7→8时不再因低位归零撤回VALID；W FIFO消费与内层completion接口握手须一致。修正两处缺一会造成一侧消费/另一侧不消费或仍死锁。
- 保留原失败日志和counter诊断证据。修正后的同一整张量八案均须完整数据、NEXT/DONE、翻译/读写/响应守恒与quiet通过；CPU-off和原模型容量/时序保持。只裁这份私有纠错后的模型，不把修正前后的时间差当论文性能收益。
- 当前证据范围是banked、MSI_BT_ONLY、正常直接DMA copy。MRIF路径把`w_cnt_q`低位另存为`mrif_w_cnt`，完整高位语义没有在这里审计；banked已禁止MRIF，因此当前纠错可接受，但不能据此宣布任意MRIF/非banked配置的全面正确性。[MRIF低位字段](../../../cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:466)

## 首案实际边界证据：已读取根执行的产物

本席直接核私有wrap与原源diff，**恰好上述两处OR**；新manifest实际RTL source list已将旧wrap替换为私有文件，源及driver当前/冻结hash相同，执行记录binary hash与build manifest一致。原源与旧失败冻结副本保留。[私有wrap](../private/rv_iommu_trans_ctl_wrap_credit_fixed.sv:220)／[新构建](../build-credit-fixed-manifest.json:5)／[实际执行](../credit-fixed-runs/4k-solo0/execution.json:1)

4k-solo0实际returncode=0；完整六张量共9,447,936B、hardware issue/done各6、payload AR/R/W与AW有效字节相等、AW/B各4,614、TL接受/响应各9,228、错误0。最终hardware done=2,582,008，natural quiet=2,582,009，最后REG_DONE观测=2,582,013。summary的实际内部credit最大值[8,0]，C=8采样[506,0]，实际8→7转移[2,0]，终点[0,0]；不再停在满计数别名。全部quiet队列为空、CPU-off、数据/孔洞/保护区通过，VALID稳定采样2,582,078拍。[首案原始结果](../credit-fixed-runs/4k-solo0/summary.json:1)／[PASS日志](../credit-fixed-runs/4k-solo0/run.log:1)

计数观测直接读取实际内部完整counter寄存器，每拍检查≤8，full/drain计数取相邻采样状态；这些观测没有成为dispatcher输入。[边界观测源码](../private/credit_fixed_driver.cpp:146) 此证据闭合bank0已知C=8故障；不扩大为bank1、全部八案或MRIF/其他参数的完整验收。本席核了实际summary、run log、执行身份和关键观测源；没有独立逐拍重放内部AW−WLAST、没有另跑DUT或宣称实现全部协议的形式证明。

静态身份：本席对比诊断冻结目录中的wrap、counter、delta_counter与当前实际源，三者逐字节相同；诊断manifest记录build成功、原输入/冻结输入未变。本席只做关键源码审查，不重跑该诊断，不改变既有G0八案或引入新机制。
