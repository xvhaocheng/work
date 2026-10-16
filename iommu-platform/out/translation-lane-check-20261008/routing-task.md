只做定向证据收集+普通修改草案，不替代Codex架构判断。用户问：既然IOMMU翻译/PTE读取和DMA数据搬运互挤，能否单独开辟翻译线路？同时继续原研究候选。请核附源码而非复扫仓库。
1. 生产路径：多个PTW/DDTW等在rv_iommu_ds_if怎么合并，输出到FPGA top哪个AXI master/crossbar端口？完成DMA数据接口走哪个端口？最后是否同一个DDR/MIG接口？确认隔离现状（不同master、内部队列、ID、到最终内存仲裁/返回通路），表格给file:line+原代码摘录，未知的下一文件。
2. 测试现状：四master CPU/DMAcfg/DMAdata/DS，哪些最终会进入同一DDR request队列，响应和backpressure是否同一R头？只有独立AXI端口是否真的消除head blocking？给真实字段/调用点/行号。
3. 复用现有测试加一个默认关闭的最小干预“页表返回与payload返回服务隔离”的可行修改范围。只限tests/nonblock_actual_dma_test.cpp，不动SV/生产/linux，不重新编译BIT。作为上界诊断而非真实DDR：保持每孤立request 31周期准备、全部数据byte/RLAST/ID/合法VALID持续规则；分开DS read队列，允许原已有不同master独立返回。说明因split同时返回可能增加总每cycle带宽，分成A:同总每cycle一beat且轮转/DS优先（资源公平基线） vs B:分离每master响应可各一beat（新增带宽上界），不可混称同资源。提出最简单可执行方案/完整patch，仅根据接口，不扩大矩阵，不重复既有serial/overlap测试。若仅改C++不可能合法隔离实际返回路径，明确指出不能硬做、需要哪处SV。
4. 指出“仅PTW私有lane”仍不解除已承诺W owner等待的情况，不能宣称它必修PN或已证明DRAM带宽饱和。
输出短导航+关键证据+最小修改草案/patch或不实施的明确理由+验收项。不运行/不写文件，不降低思考或输出预算。不得把纯行为模型提速写成新机制/新颖性。
