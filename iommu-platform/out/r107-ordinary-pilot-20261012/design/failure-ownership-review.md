# stage111：有限驱动失败所有权对抗核查

结论：初读发现一个必改问题，root已经修正并经本席直接核源码；当前接受的ordinary GFP_KERNEL、稳定binding、无其他map/用户unbind范围内，没有发现另一个未证明quiet时提前读/free/reuse资源的具体反例。这是源码核查，不是编译、故障注入或板端成功证明。

审查仅涉及partialalloc、invalid NEXT、timeout STOP/drain、releaseerror、copyout和HPM/device retain；本席未修改driver。完整原始诊断receipt、反例、source hash及定位在同目录failure-ownership-review.json。root仍负责最终接受。

## 唯一初始必改项：准备checked fence失败没有unsafe

初读版本idma_finite.h:638-643在checked preparation barrier失败时只goto cleanup，没有设置unsafe。反例是第一次checked flush因CQ错误代际返回EIO，错误处理后quiet仍成立：cleanup先copy pool，再撤PTE并第二次尝试checked flush，后者若成功便free。它违反root-implementation-contract.json:28的failed checkedfence保留、无retry/revoke/free约定。

root当前修正在source/linux/drivers/misc/idma_finite.h:667-672：仅在audit成功后尝试barrier；barrier任意非零立即设置unsafe。cleanup的池复制守卫:714和release guard:98均拒unsafe，所以池不会再读，实际release helper不会再进入。这一具体项已闭合；本席没有执行硬件来证明其运行效果。

## STOP诊断明确撤回

本席原诊断把STOP写命令2混为control读状态的bit1，曾误报它会令quiet永久失败。这个诊断错误，正式撤回。完整旧表述保存在JSON的withdrawn_diagnostic_receipt，不能记为成功证明。

实际冻结frontend:189-190表示写命令2设置batch_stop_q；:434-436的read concat是enable bit0、active bit1、stopped bit2、completed bit3。quiet使用&2检查active，sticky stopped是4，因此原反例不成立。这只纠正编码判断，并未运行timeout/drain。

## 剩余失败路径的有界源码结论

- partialalloc：:123-127只把成功对象计入nr，:138-141若checked清理失败先置unsafe；成功清理:102-104才清所有记录及nr。partial retained pool没有落入四池copyout路径。
- invalid NEXT：:347先置triggered再read，:350-353的zero ID置unsafe；没有拿上一笔DONE当安全排空。timeout:378-398只在own DONE及全平台quiet成立后清triggered，否则unsafe保留。
- releaseerror：:729-746在helper非零后设置unsafe，保留owner/run/device reference；没有调用旧void free或重试PTE撤销。只在release成功后nr才变0。
- copyout：:714-717要求!unsafe、!triggered、experiment lock及quiet，才读DMA池。技术失败仍可复制kernel-owned metadata；final copyout错误:763-767不会重新issue或再次free。
- HPM/device：:740-757在quarantine发布后才解experiment mutex，保留owner和device ref；HPM release只恢复HPM寄存器并释放session（iommu-hpm.c:55-67,220-227），不释放DMA对象。旧session入口idma.c:2393/:2127全局拒quarantined requester。当前idma.c:3019和iommu-platform.c:166均抑制bind/unbind属性；按root当前built-in/no user unbind/no other map前提接受，不扩大为通用hot-unplug保证。

原DMA API返回NULL之前的内部失败回滚，仍是此前明确登记的边界；本 review没有把它升级成已证实retain。没有新增风格检查、driver整套复审、编译、设备或模型操作。
