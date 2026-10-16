# 当前W顺序能证明什么

## 1. 两个原生边界：首VALID预留与WLAST释放

原配置：banked_ctl.sv:155–172、236–310、409–452；global NumOutstandingTrans=8、WFifoDepth=32，每bank4/16，输出两bank mux仍MaxWTrans=global8。原DMA0/1 FullBufferCapacity=512，没有新增容量。

输出axi_mux的IOMMU副本:282–332、350–367及CVA6副本:208–256、276–290：`w_fifo_push`发生于一次新AW的首次转发VALID，即使AWREADY=0；锁防重复登记。FIFO仅存bank/switch index，两个bank时每entry1bit、原8entry，不存burst长度或job ordinal。真实WLAST握手pop；B另行返回。

输入axi_demux_simple两个副本:140–198、233–253、410–437：只有`w_open==0`或新的AW仍选当前W bank，才首次转发AW。`w_cnt_up`也在首次VALID预留；真实输入WLAST使`w_cnt_down`。因此当前bank尚有raw W义务时，另一bank AW不能转发。AW/W可解耦；先完成W再接受held AW不违背此事实，不能误写成“必须AW握手后才允许W”。

两个demux_simple副本本次指纹相同；两个mux副本不相同，但本结论的预留与pop逻辑分别已读。没有推导其他功能相同或重做生产bit绑定。完整原文及SHA见[source-snapshot.json](source-snapshot.json)、[source-excerpts.md](source-excerpts.md)。

## 2. 有限当前job的部分顺序证书：正向接口产物

限定已accepted、sole pending、无错、aligned DEFAULT4KiB/64bit、deburst=0。frontend:110–179/279–304和legalizer:140–214/224–316说明OffsetWidth=3、最大256beat、虚拟页2048B，因此每当前job两份AW，各LEN=255、SIZE=3、INCR、无ATOP；保留原options，不为造收益切换它们。

在原job birth初始化每bank2bit reservation_count：0无、1部分、2全部、3无效。在该bank原physical AW第一次`w_fifo_push`递增；核packet/job关联与LEN/SIZE/BURST/ATOP；重复/超量/不匹配/error/reset epoch使无效；原job真正退休后清空。这样合计4bit新增直接编码，没有复制payload/header，没有额外credit或W队列。这是源码支持的构造，未实施；不是信息论最小总面积。

若B_count=2、A_count=0且native关联有效，则B当前job所有已预留W槽在未来A槽前；无需把未接受A AW读为现成事实。相邻B_count=1时，尚未预留的第二B槽可能受其他承诺/许可约束，不能从该计数单独推出完整B→A job顺序。此正反是逻辑条件，不是本轮取得的自然prefix/实验。

若保留一般合法deburst/变burst，改用每bank10bit累计LEN+1，0..512及513无效sentinel，合计20bit；不是同时再加4bit。这仍只证明reserved W字节次序，不能代替actualW、B或未来AW-ready。

## 3. 当前完整AW证书的不可达性定理

前提明确限定：两bank各一份当前normal aligned DEFAULT copy，全部job raw AW均已真正由输入demux接受，且physical AW全部已准备；P是两copy之一仍必要的普通AR/AW完整walk，L是另一copy尚未下游首次VALID的实际payload AR。没有预取/ATS/debug/隐式额外请求、fault、reset、额外job或其他流。

取两copy全部raw AW中最后接受的那一个，所属bank记为X，另一bank记Y。为切换到X，输入demux此前必须将Y所有已forwarded W reservation清空。所有Y的raw AW在这个最后接受事件之前都已经forwarded；此后没有Y当前job新AW。因此Y整份raw W/WLAST已完成。若最后接受时是先前held AW，锁仍阻止换bank，结论相同。

DEFAULT数据来自同copy真实R数据：transport:238–360/390–454、backend:518–533/632–650。完整Y W消耗了该copy整份4096B，即全部512个64bit R数据已经可用且消费；不能靠后来未接受的payload AR提供过去W。

- Y若是L所属job，L不可能仍是这份job必需且未首次VALID的AR。
- Y若是P所属job，其整份R已完成且全部physical AW已准备，该copy不再需要这里声明的普通AR/AW walk P。

所以以上**current-full-AW直接映射入口**不可达，必须回退。这个证明不是“所有未来AW预测都不可实现”，也不是“W FIFO为空才能调度”。父轮原条件仅要求AW在相关数据需要时合法可用；本定理不把它改成普遍当前-ready要求。

## 4. 仍合法的部分前缀与准确缺项

当前B raw W owner有未完成W时，A raw AW被demux挡住；A raw AR在原read-admission/FullBuffer证书满足时可以进入，native N又可按真实缓存上下文/权限命中原B后续请求。FullBuffer证书只证明容量与sole job资格，不授予未来AW。故“部分W证书+P属于A读翻译+L给B供数据”没有由上一定理排除。本轮没有观测或宣称它实际发生。

但A写资格至少受以下独立端点约束：原B raw WLAST；A原AW header供给/原队列；A write NormalTL所需完整翻译与physical AW准备；既有credit/held/错误路径。它们随ADMIT/WAIT的实际R供给和walk进展变化。若A AW需同一PTW，还必须等当前read walk完成；不能给每walk套固定时延，N-hit时同样要真实hit/上下文证书。

可记预测下界`T_AW_A[s] >= max(T_rawWLAST_B[s], T_header_A)`，再加/合并A自己的合法翻译和准备约束；是否还受当前walk约束取决于真实请求/命中路径，不无条件把所有请求串行化。该式是约束说明，不是新运行模型。

父两score若要在这一合法分支使用，需要显式重推`H_A[s]`等策略相关AW/W许可；两job的真实B尾也应分别`Z_A[s]`、`Z_B[s]`进入各自完成max/sum。W顺序不使跨job末B相同，也不能保证CA总是max。当前输入W owner不是DDR W服务owner，raw WLAST也不等于输出WLAST/B；必须分别记录。

因此本轮保留部分顺序证书，停止不含这些项的literal-ready在线版本，future-AW-aware版本open、需独立修订；没有“全部W包络都不行”的裁决。
