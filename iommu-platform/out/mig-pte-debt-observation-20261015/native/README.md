# 私有MIG债务观测器 v2

按已核生成MIG4.2/NORM全局FIFO读返回记账，检查真实RID/LEN/RLAST/RESP及held稳定。首次PTE不依赖K；同沿Rfire先扣；knownempty epoch起连续跟踪，ARM保留carry。

1536条Rfire日志，各含握手UI tick及此前连续held周期数，可无损恢复选中episode的RVALID等待；任何容量或held19溢出invalid，无AXI反压。只读回两UI注册，native3584word＋source1024word＝4608word／36864B。原执行队列、DMA buffer、walker和工作量未增加。

新功能120项、异步读回29项和解码器5项测试通过；原通用101项与FIFO初版97项保留但不作当前版本验收。修正后UI5ns OOC WNS+1.020ns；完整FPGA时序、BIT绑定及真实记录尚待完成。

最终第6次完整时序未通过：WNS−0.391ns／598 setup违例。未加载或采集，按用户收紧边界停止并暂停；OOC通过不代表可运行。
