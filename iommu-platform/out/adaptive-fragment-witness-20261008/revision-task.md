这是上次几何 smoke 草案的聚焦修订。你不能运行或改文件，只交付 valid unified diff（真实行号/context，不许伪 @@，不许占位）或可执行 Python 精确字符串替换脚本（每处assert原块唯一），且只改我指定的 out 实验副本；不得修改生产或tests当前文件。旧草案 source-answer.md 不是可应用diff，且混入DS/PTW读计数、CONF0观测缺失。请修正。

选择已有生成模型：out/write-order-boundary-20261007/obj-samebank-private （build JSON 已确认 PolicyEnable1/DmaBufferDepth512/SharedPoolMode0/PoolSlots1/SameBankDmasEnable1/ReverseReadOrderEnable1/TEST_COMPLETION_GATE）。仅一个DMA V手动拷贝，其他3DMA和CPU静默，因此同bank和跨DMA逆序不会影响单DMA几何。ROOT独立已验证 CONF bit0 -> decouple_rw（idma_reg64_frontend:367；不是decouple_aw）。RAWCouplingAvail0，backend会独立把AW放fall_through，不要求先有R数据。不能假定W2一定迟于R，必须实际测量。
归档原始C++：out/e2-interface-poc-20261007/before/nonblock_actual_dma_test.cpp。所有top->字段都在选定生成模型头文件存在。请基于这个副本，最终新文件 out/adaptive-fragment-witness-20261008/geometry_test.cpp；代码作者DeepSeek，Codex会复制/应用/编译运行。不要写新SV/完整build。

只支持新flag +geo-smoke=1 和 +geo-conf=0|1，固定2048 B，src V_SRC_IOVA，dst V_DST_IOVA+3072。无batch/CPU/gap/预热，cold单次copy。初始化现有页表/页内容。必要调用reset、init等复用run_scenario正确顺序；measurement_phase=false，无政策掩码需要policy=false使Mode0普通路径。正常手动cfgCONF geo_conf、src、dst、NB2048、trigger/wait_done，再check_quiet。验证实际整个2048B数据，最好验证相邻未写字节也未改变（原页面0xcd），所有response/队列/CFG错误不能吞掉。读side只统计DDR prefix2(同原decode)且addr在 V_SRC_PA 段的数据AR、R，不能DS prefix3/CPU prefix0；写sideDDR AW/W prefix2目的地址且无其他DMA。记录每个源读AR(周期、addr、len)、目的AW(周期、addr、len)、每个源R的首/尾/全局第129beat周期、源Rbeats总数，输出单行 GEO-SMOKE-JSON。每次R握手可直接用现有mem_q.front()作为当前addr/id/beat/len BEFORE pop，不必额外队列乱配。AR/AW握手应复用tick现有采样时刻（eval前后区别要核）。检查CONF0读两段各1024、CONF1读一段2048；两者目的写两段各1024。R256beat W256beat；不要求W2在什么时刻（观测结果可能候选被淘汰）。
使事件trace在geo过程可记录，不假定win_open。最小局部改动；无需增加通用任意几何支持。
说明该smoke没有实现候选I，没有性能创新结论，仅验证“读长已锁定但W2尚未prepared”的真实时序空间，512beats普通缓冲非资源节省实验。root独立I设计见codex-rule-i.md，已有透明splitter先例不能声称拆分创新。

输出：1)短摘要与局限；2)一个完整可用Python替换脚本（优先）或真实diff fenced；3)确切编译/运行建议，不执行；4)原源码依据路径行号。不要长篇重做领域地图。
