# 2026年7—8月：多DMA扩展与日期证据缺口

本单元由主席直接只读查看现存Linux、CVA6和IOMMU仓库的作者提交列表、选定原始补丁；另阅读全文早期Git证据文件、归档恢复说明及工具准备说明。未以10月回顾替代七月原始提交。Git读取只用log/show，没有checkout、fetch、构建或实验。源码历史不是完整日记，不能据无提交宣称没有工作。

## 原问题与扩展

原研究关注Accelerator-side IOMMU的Remote Page Walk自身代价：用FPGA可控页表延迟区分页表读取与载荷搬运，Pinned与Bounce Buffer/CPU copy提供端到端参照。此背景不等于真实多Chiplet、P2P安全或完整一致性系统验收。后来多DMA扩展增加了设备间翻译缓存替换和共享服务等待问题；它不是突然从10月CO1开始的研究。

## 七月可直接核对的原始事实

| 日期（提交原始作者/提交时间，JST） | 实现 | 原始证据与范围 |
|---|---|---|
| 7月9日20:49 | Linux增加idma1、DID11；新增按长度单次搬运IDMA_IOC_RUN_ONCE及bytes/cycles/miss/PTW/active/errors返回 | Linux `0cb98744633bcb038ed691f9fd0cd69d23e289a2`。本轮直接读设备树及UAPI补丁；驱动119行变化只看stat，未全文代码审计 |
| 7月11日01:28 | CVA6更新IOMMU子模块指针6cc8ed30→7fbb2b99，引用大传输AW优先死锁修复 | CVA6 `3e409badcca73d786b510319ef09444fbdf3a731`。本轮直接读gitlink补丁；修复自身提交日期为6月29日，不能说7月11日重新设计了修复 |
| 7月26日18:03 | Linux加入idma2/3、DID12/13以及Pair/Multi ioctl | Linux `48185ad5dc62d32bd4429bf63b955ba1daceb47d`。题名“3个idma”与实际四路DTS不同，本轮直接读完整DTS/UAPI补丁；软件描述不证明当时四路RTL/板测已完成 |

仓库路径分别为 `/home/xvhaocheng/work/iommu-platform/linux`、`/home/xvhaocheng/work/iommu-platform/cva6`。补充直接取证文本保存在本整理目录的 `evidence/july-august-git.txt`、`evidence/july-selected-patches.txt`。这是原Git内容的审阅副本，不修改或补写用户历史。

原基础rootfs由7月11日镜像继承，归档 `platform-archives/restore-check/assets/README.md` 明确它并非全部用户空间源码重建。后续可复现范围是由该固定镜像打包新内核/benchmark，不是整个镜像来源已完备。

## 八月的具体缺口

本轮对已知Linux、CVA6、IOMMU仓库运行只读 `log --all --author=xvhaocheng`，限定7月1日至9月1日；取得上述3个七月作者提交，未取得八月作者提交。该筛选不覆盖其他作者、未提交工作或缺失ref，因此不能推出八月没有研究。

目前工作区中没有找到可供本轮阅读全文、明确属于八月的逐日日志/周报。九月8日迁移把四DMA RTL、HPM edge、Independent/drain/quarantine与计时修改保存为版本；这些修改的最初开发日期仍不确定，不能把保存日期填为八月某日。早期Barrier/Lockstep表格仍缺命令、版本和完整原始运行身份，不能用后来汇总替代它们。

## 平台之外相关记录

本轮完整阅读 `platform-archives/restore-check/README.md`、`docs/{patches,validation,build}.md`、`assets/README.md` 五份归档说明，明确迁移/离线恢复、HPM定向测试、FPGA构建和仍未上板的历史边界。它们是9月8日快照，不作为最新板端状态。

另阅读全文 `tools/claude-advisor/README.md` 与 `preparation/{draft,revised}/README.md`，核对工具草稿→授权限定修订→离线测试准备的支线；没有读取任何密钥或实际配置、没有调用Claude。工具准备不是科研性能结果；历史默认分工与后期Codex-only要求分别保留。

## 对旧材料的纠正

- 不能写七月已验证四DMA共享性能：七月原始证据主要是软件入口与继承修复，早期性能表原运行身份仍缺。
- 不能把“实现了3个idma”提交题名当硬件路数，DTS实际有idma0—3。
- 不能凭9月迁移的保存提交日期倒填八月开发时间。
- 当前缺的是八月原始日记/周报和早期运行证据，不是用新实验填补历史。若以后发现原始记录，应按正文日期、commit、命令及运行身份补入时间线。

本单元没有读取原学位论文全文、全部旧工程源码、七月驱动完整实现或全部早期原始实验日志。上述限制同时记入coverage与首次daily记录。
