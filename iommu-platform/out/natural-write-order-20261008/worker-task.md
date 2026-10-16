写一项严格限定的普通实验驱动草案，不能执行命令或写文件。最终只创建 out/natural-write-order-20261008/ 的文件，不能修改tests当前文件或任何生产RTL。Codex已亲自提出研究候选I，负责设计/根因/验收，你只负责观察端口与普通C++驱动的机械实现。

目的：关闭所有诊断强制排序，检验真iDMA两流长拷贝在同一bank时，自然虚拟AW顺序/物理AW顺序/源读提交与R反压是否出现依赖。不是实现I，不比较PN性能，不宣称创新。
模型选择：复用既有build-samebank-private-command.json源列表，但将output Mdir改本轮/obj-natural，system_top源码改本轮副本，C++改本轮副本。-GDmaBufferDepth=3 -GSharedPoolMode=0 -GSameBankDmasEnable=1 -GReverseReadOrderEnable=0 -GInterleaveAwEnable=0 -GSourceReadGateEnable=0 -GFrontierEnable=0 -GHeaderParkMode=0。PolicyEnable1但运行+mode=s，policy_partition=false。仅两个真DMA，共享bank；**测试BankIdBit=3当前把DMA0/1合bank，并不等于四真DMA生产完整复制；Codex随后核ID映射**。原项目生产源码无变更。

提交一个完整Python精确替换脚本（每个anchor assert count1），生成：
1. system_top.sv：复制tests/nonblock_actual_dma_system_top.sv，追加只读端口，直连内层 i_dma.tr_aw_valid/ready/id/addr/len 和tr_ar_* 同样5组，tr_w_valid&&tr_w_ready及tr_w_last （2bool）。不引入寄存器、不反馈、现有模块名不改，不修改inner test_top。C++可以top->nat_vaw_valid/ready/id/addr/len，nat_var_*，nat_vw_fire/last采样。观察物理AW/AR/W/R复用既有DDR端口。
2. natural_test.cpp：复制tests/nonblock_actual_dma_test.cpp或§30 geometry_test.cpp（二选一，推荐当前test），增加+natural=1、+natural-bytes=2048|32768、+natural-first=v|a、+natural-trace=路径；只手动两DMA一对任务；CONF=1(decouple_rw)，源offset0，目的offset3072。不得CPU、batch、warm、window fixture、read_admit门。直接使用已有setup_memory/reset/cfg_wr/trigger/wait_done/check_quiet。a_pages=8，复制长度至多32768；源V0x10000000→PA0x80000000共8页，目的V改IOVA0x10010000→PA0x81000000共9页，避免原Vdst与扩展source overlap；A源A_IOVA_BASE→A_SRC_PA_BASE共8页，目的A_IOVA_BASE+8*4096+3072→A_DST_PA_BASE+3072共9页。改V L0 0x50000+i*8映射8src页、0x50000+(16+i)*8映射9dst页；原A源/8dst表已setup_memory(8)，补第9dstPTE index16 @0x70080。所有src填page_word(域,page内beat)，所有dest9页填可区分page_old；保存源及目的初值，正确完整字节/未写头尾/源未改检查。编程两DMA所有CONF/SRC/DST/NB之后，按natural-first顺序先trigger另一trigger，再两wait_done，checkquiet；正确使用NEXT reserved ID，不用staleDONE。

观测：
- 在tick现有边沿前采样位置 capture nat_vaw/nat_var handshakes，记录virtual AW/AR cycle/id/addr/len；nat_vw_fire计每拍并last记录，保持虚拟AW序列queue，用WLAST归属head原id；不得虚构AW/AR。
- DDR真实物理数据AR/AW只comp prefix=(id>>4)&3==2，并按PA区间归属V/A；物理W按现有caw_q.front() BEFORE mutate/popup归属，记录每个WLAST周期/id/addr/len；物理R按mem_q.front() BEFORE pop归属，记录每burst首尾cycle/id/addr/len；**RVALID&&!RREADY**记录每次停顿begin/end及归属，不能改变DDR服务规则或RVALID保持。
- CSV逐事件输出至natural-trace，不用已有win_open门控event_trace；不能打开win_open引入CPU或夹具。每个virtual Wbeat可record或者至少总count和WLAST；务必可重建global W顺序、sourceR与对应dest范围。每个physical Wbeat亦可record保证进行状态（总8k beats仅几千行不大）。
- 每次捕获写后flush仅关键AR/AW/RBLOCK/WLAST事件，watchdog失败完整现场仍保留trace；不伪造PASS/JSON。如果失败有exception本来返回1要保留；可以结束前统一summary在maincatch但明确completed/verified=false。
- 输出NATURAL-JSON，仅成功后确认data geom/两DMA数据R/W each natural_bytes/8、虚拟AW/Wlast队列已排空、RSTALL合计等。不要断言任何特定AW排序或停顿有无，这就是测量目标。
- 新scenario不要污染旧run_scenario，默认+natural0仍原逻辑。限定flag合法，精确匹配无伪diff或省略块。

输出短摘要、一个完整可运行Python精确替换脚本 fenced python、root compile执行命令建议、关键原源码依据行号/不能判定项。没有shell/write/browse，不能声称已经构建或实测。不要重做前沿地图。
