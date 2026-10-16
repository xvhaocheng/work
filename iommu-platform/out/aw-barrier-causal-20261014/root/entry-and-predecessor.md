# 选择点与前驱：预留数不能替代完整来源证书

## 1. 精确进入条件

B/A是实际不同bank当前sole job，不是按bank标签假设依赖。guard包括：normal aligned4KiB/64bit non-deburst；当前job原选项/ordinal；B output reservation累计2、A累计0；L属于B当前job必要R且未下游首次VALID；P属于A必要read walk；真实raw W owner与source routing；原1S/N hit/FullBuffer上下文许可；无error/reset/invalidation/未知关联。实际历史及服务class无效则普通回退。

reservation count按原job出生/正常退休维护而非FIFO当前usage；已pop掉的B slot也在累计2中。原输出FIFO为8个bank index，不含LEN/job tag；新4bit记录仅非deburst构造，未实施。

## 2. 源约束兼容的前驱，尚非原工作流witness

若原有供给使B两AW先在原有序upstream AW链进入input demux并完成各自TC准备，随后A.AW0被原upstream spill接住而在input demux因B raw W未清空等待，则：A source AW已握手、A raw input AW尚未接受、A output count仍0，三个事实可以同时成立。

coupled模式的A private AW0已离开，private空槽可接受第二对AW1/AR1；原AR链独立，A的两个read headers可以进入其bank AR FIFO，FullBuffer保持真实R数据而非免费W授权。A currentread P可占慢NormalTL；B的剩余read若由已有正常前缀建立src/context cachehit，可经different-bank native N进入B physical AR spill形成L。不得增加warmup、增加槽或假设没有真实lookup/invalidation。

这是按原接口允许的条件partial order；本轮未取得原供给、缓存资格和逐拍就绪记录，因此**没有宣称自然snapshot/完整cycle witness已经成立**。当前P未来leaf、下一PTE和A write cachehit亦未知。

## 3. upstream顺序对入口的限制

peripherals:1228–1252未覆盖SpillAw；CVA6 axi_mux:47–54默认SpillAw=1、SpillAr=1、SpillW=0。AW arbiter的LockIn和原两槽spill保留承诺顺序（188–205/259–271）。source arbiter当前hold、已source-accepted spill header和input handshake是三个不同端点，不能由其中一个代替有序队列。

若**同一当前A job的AW0已在原承诺的upstream AW序列中排在B.AW1之前**，B.AW1不能越过A.AW0进入input demux。A.AW0先要等B当时raw W open清空；它进入后新的raw W owner是A，B.AW1须等A第一raw burst的WLAST。

在原声明配置中bank W FIFO=32/2=16 beat，target W spill两数据寄存器，output bank mux SpillW=0；正常非MRIF/error W路径在output mux前至多16+2=18已接受而未输出的beat。A第一burst是256beat；若其raw WLAST已过，则至少256-18=238 A beat已过output W端点。输出W FIFO只有已预留A slot才会选A W，故A当前job output reservation累计至少1。于是上述**同job upstream A0-before-B1前驱**不可能产生所需B累计2/A累计0。

边界：只核原32/16配置、正常256beat burst、相同current A ordinal、无额外W旁路/MRIF/fault；不是所有deburst/换job前缀的全局不可达定理。相同job假设不能跨退休后reset计数；stream中的原后继要另核。

所以需要来自原供给的B.AW1-before-current-A.AW0实际有序证据。不能让A迟启动、暂停A或改RR来制造它。本轮既未确认共同就绪源能产生这个顺序，也未据接口可兼容就假称已可达。

## 4. 本轮准确进入缺项

未取得实际accepted decouple_rw/deburst、current ordinal/full header/P绑定、upstream AW承诺顺序/spill占用、native hit/context/FullBuffer资格和原fixed supply。仅source候选卡不足以允许新条件/bench。本页保留必要条件和条件前驱，缺项详见missing-evidence.json；不把入口判别转成新压力实验。
