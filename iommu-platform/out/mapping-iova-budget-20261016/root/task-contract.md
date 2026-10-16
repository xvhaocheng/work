# MAP-IOVA-FALLBACK-BUDGET-1

ARS-Codex既定有界贡献发展与原始源码核查。新输入问题：K只界关联库存，是否足够保护同B domain普通短映射/普通TX所需IOVA？与上轮数据可见性正交，不重做同步/metadata/NAPI协议。固定v6.12 adc218676eef25575469234709c2d87185ca223a、mlx5e cyclic/linear order0 nativeXDP非XSK、B/config固定；T/S保持。先考虑地址预算、预映射copy池与普通allocator回收三种合理组织，重点核第一项的可部署条件与残余。

allocator席独占dma-iommu与iova分配/回收大小及FQ/cache事实；ordinary席独占同设备普通发送来源和强普通方案比较。按需只补同commit的drivers/iommu/iova.c、include/linux/iova.h及mlx5 en_tx.c，已有raw复用，勿重复下载或全文调研。两个证据席请求Sol/high；root机制与接受，文档Sol/medium；独立实际metadata未知。Codex-only。

不把源码可达当运行部署，不填IOVA窗口/压力/费用实测数字。普通O3同享新付费信息/接口；普通动作能复现只限策略增量，不否定架构思想。K槽、IOVA占用、物理RAM与源/目标/后端副本分账，分配反馈各策略分别导出。无源码/ABI/driver/RTL/collector实施、模型、native/旧实验、板测或hostprobe。B1/I/D/A5/A2、独立工程和MIG暂停及Oct15–28保持。root接受冻结后一次文档合并；仅同步本批，不碰用户AGENTS脏编辑或外国未完成事项。
