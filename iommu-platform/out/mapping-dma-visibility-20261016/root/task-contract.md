# MAP-DMA-VISIBILITY-CONTRACT-1

2026-10-16 JST。新具体部署/费用条件触发：B持久streaming映射跨同一source页不同包的复用，是否依法维持CPU/A/B所有权和数据可见性，哪些backend同步会隐含bounce复制。固定v6.12 commit adc218676eef25575469234709c2d87185ca223a，mlx5e cyclic/linear order0 nativeXDP单buffer/非XSK，B/config固定；T持续授权仅条件部署，S独立。沿用source/frame/target和V-C表示接受条件，不重复NAPI/metadata协议。

API席只核DMA前端和官方streaming/range/direction契约及真实mlx5/page_pool sync放置；backend席只核dma-iommu/SWIOTLB路径的同步/复制/参数触发及费用，不重做frame全链。root设计合法事件序列并直接核关键原文；普通O0/持久/复制/O3同享付费优化，不把同步正确性或常见backend分类认作独占创新。原生配置/成本未知，模型和投影不得写实测。

原始字节复用mapping-admission-20261016/v612-source/raw。仅为具体新问题补必要官方同commit DMA文档和swiotlb实现；不新搜论文/全仓/旧数据入口。不driver/RTL/ABI/collector编辑，不状态/性能模型/native/旧账/板测，不hostprobe。Codex-only：API核查Sol/high，backend核查Sol/high，root机制按当前席，文档Sol/medium；actual独立metadata未知，不声称切换。

root接受冻结后一次合并canonical并重读并发；AGENTS用户脏编辑只读保留。B1/I/D/A5/A2、独立工程、MIG暂停及原10月15—28窗口不变。完成检查后只同步本批到top work/main，正常增量不强推。
