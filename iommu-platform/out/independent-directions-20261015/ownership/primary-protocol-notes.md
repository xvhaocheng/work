# 补核：ATS给B留下什么，不能证明什么

2026-10-15，本席通过浏览工具直接读官方RISC-V规范相关方法；没有取得并精读PCI-SIG ATS1.1完整正文。下列网页行号是本轮浏览器提取定位，不冒充本地原文件行号。

1. [RISC-V IOMMU，v20260222，Data Structures](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html)，Device-context fields → Translation control，浏览提取98–115行：EN_ATS允许translation request与后续translated R/W；T2GPA配置明确先返回GPA及权限/粒度，设备ATC缓存该翻译，后续访问再使用。由此，先取得/保存翻译、以后才访问payload的分离已有正式协议。这里没有说协议掌握应用SPM槽owner。
2. 同一原件，PCIe ATS translation request handling，浏览提取612–685行：响应包含权限与属性/粒度约束；协议成功状态本身不保证授予所需R/W权限。B不能把任意返回值当作payload可提交许可证。
3. [RISC-V IOMMU，v20260222，In-memory queue interface](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)，IOMMU PCIe ATS commands，浏览提取131–139行：ATS.INVAL清指定设备ATC范围，并等设备响应或超时；IOFENCE.C可同步此前命令。由此，结果驻留和失效握手也并非B新引入的通用概念。
4. [PCI-SIG官方ATS1.1条目](https://pcisig.com/PCIExpress/Specs/Base/AddressTranslationServices_1.1)：官方说明其目的包含设备ATC缓存DMA地址翻译；完整规范需要访问权限，本席只核公开metadata，没有据此宣称逐条读过PCIe所有ordering规则。

这一补核只排除“翻译完成与payload访问分离”“结果保存+正式失效”这些宽泛接口新意。它不能证明某个片上非ATS实现与PCIe成本相同，也不能证明ICCD2018实现了跨应用SPM槽的显式owner票据。

关键推理是对象分离：HERO未来host源页的映射权限与当前SPM槽的payload占用是两个对象。对同一已授权host地址先准备翻译、然后等计算释放SPM再搬运，可以由已有翻译协议加应用依赖来组织；它不自动增加跨保护域授予权限的新能力。若以后要保护不可信任务的SPM写入权限，那将是新任务/威胁模型，不能悄悄用来为当前B立题。

留存与限制：本轮尝试本地保存官方HTML，第一次因可选解析库不存在在联网前失败；改用标准库后官方Data Structures URL返回HTTP403，未继续网络重试。浏览工具已成功取得并阅读上述官方内容；未写入伪造的HTML快照或哈希。已存在的规范原件可从`out/local-read-reclaim-20261015/controls/sources/riscv-queues.html`导航，本席未改动它。新关键源码原文及hash另见`critical-source-excerpts.md`和`source-manifest.json`。
