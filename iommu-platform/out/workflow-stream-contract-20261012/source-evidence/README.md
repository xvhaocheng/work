# B 源码合同导航

- [source-findings.json](../source-findings.json)：原源摘录、行号、固定版本与合同。
- [source-manifest.json](source-manifest.json)：22个固定原文件URL与SHA256。
- [mm-large.c](hero/openmp-examples/mm-large/mm-large.c)：49–83六槽；93–236输入/计算；238–273排空；284–328参数；335–346host reference；367–404active copy/inactiveSVM。
- [hero-target.h](hero/support/libhero-target/inc/hero-target.h)：83–117启动与完成。
- [pulp/hero-target.c](hero/support/libhero-target/pulp/hero-target.c)：27–85、124–138完成ID。
- [host/hero-target.c](hero/support/libhero-target/host/hero-target.c)：24–56同步memcpy；88–99wait空函数和NULL allocator。
- [functional-run-proposal.json](../functional-run-proposal.json)：功能执行提案，实际编译/执行由root验收。

官方HERO固定提交c8481a930a15a385d4f58f519bed83d2e75e2a73，Apache-2.0。新来源适配，不是原SP复现。256²、stripe16行/16KiB、六槽96KiB、288搬运、256tiles/16,777,216uint32乘加。目标计算与DMA时延仍未知。
