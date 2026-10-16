# 本轮原文阅读记录（Codex，2026-10-08）
- AMD IOMMU 48882 rev3.09-PUB Oct2023，§2.1，官方PDF搜索返回的完整节选已核：device/page/interrupt-remapping表访问采用等时VC，command/event等用normal VC；由此证明虚拟通道区别已有，不证明物理DDR带宽独立。后续直接PDF解析失败未用作机制证据。官方来源见研究报告。
- MASK ASPLOS2018，作者原PDF§5.4，印刷p8：Golden翻译队列高于Silver/Normal；Silver配额使用并发walk和阻塞warp。已在网页原PDF核，不称其完整DMA双端机制。
- VDC Emad Malekzadeh Arasteh，Lund硕士论文2011，§4.2/印刷p33、本地TXT1270–1280：页译址与FIFO满时暂停；最大16拍因行/页结束及FIFO空间不足缩短。双端copy与跨DMA W义务没有因此自动被覆盖。
- Arm DMA350 TRM 102482_0000_04_en Issue04 Jan2023，§5.8.1/印刷p87、本地TXT4535–4551：完整读返回空间、完整写burst数据就绪才能请求，读写循环依赖避免。读取官方原PDF与本地TXT，两者相符。
- Ober等，High-Throughput Multi-Threaded Sum-Product Network Inference in the Reconfigurable Cloud，H2RC2019作者PDF§IV-B/印刷p4：多引擎一方占W另一方读反压引起循环，完整R缓冲与W数据后提交解除。网页原PDF关键全文已核；本地获取代理503，未计PDF取得。
- TAPA2023本地TXT437–450：运行时连续地址合burst，下个地址迟于阈值则结束，async_mmap允许分别发AR/收R；与worker摘录相符。
- CTC2009本地TXT476–489：最后credit标tail结束当前packet释放路；Connection仍到全消息交付才结束。其合同不等于取消AXI既定ARLEN。
- TI US20250291754A1原专利公开说明：FIRST/LAST/BYTECNT及FIFO/优先级事件控制特定合同下进行中burst结束/延伸；未作法律状态/FTO判断，未照搬到现有AXI。
- 尚未核2026 adaptive-ECC DMA正文、CN114780466完整机制。检索返回/DOI访问失败不能算已排除创新。

