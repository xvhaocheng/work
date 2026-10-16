# stage110：当前内核一次干净重现与有界metadata恢复

**配置、四个关键驱动及2078/2079个实际链接输入完整复现；全Image/ELF仍未匹配，按授权边界停止。** 唯一差异是生成的临时版本对象 `init/version.o`。这次已通过真实干净构建补到了此前缺少的对象来源证据，不能把尚未匹配的完整Image写成成功。

- [最终机器结果](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/reproduction-result.json)、[执行合同](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/reproduction-contract.json)、[完整冻结源码清单](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/frozen-source-manifest.json)。
- base `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`，归档SHA `e3646d7ef2f08cf01aeaf5e5f4a5c98cb764dd55323efd487e7b5b6a0709dea5`；完整81,122项清单SHA `f9a4a0e06ddd3ffd995770361d59b8c145c67f7510062f610a60fe856aea0c3f`。5个tracked overlay与3个lowrisc untracked均保存具体hash；源码、模式和symlink目标在两轮完成后全未变。
- 新建空O，干净 `make -j12 Image` 一次，exit0，56.8862945秒；实际执行10:18:27.913900–10:19:24.800672 UTC。明确指定旧user/host/final时间/#28及release只是重现参数，本次执行时间独立保存；没有更改旧历史。

## 核心字节证据

[链接输入比较](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/static-linked-input-comparison.json)：旧/新 `vmlinux.a` 各2079项，归一输出根后顺序完全相同，2078项完整SHA相同。idma.o、iommu.o、io_pgtable.o、lowrisc_ethernet.o，以及configs.o、initramfs_data.o、version-timestamp.o完整字节相同；不是只比函数名、横幅或部分代码。精确.config SHA `b9af38b79a9f0d7c5348bb70ea4641ac0f7e21a4e44119c63a1e653639c2933d`，compile.h/utsrelease.h/最终utsversion.h均相同。

旧Image SHA `8c19da27ac91c9f2a9d6b1fc7267a2810b2432393e8cf3921455926e6e5cb6bd`；新Image SHA `70ac648089ee329ebb95d2d5bb35f871feeb0cfc323f038f6f9e4b1f33d8f19d`，两者均24,131,072B。旧ELF SHA `24f1c188a4e652f24f5e2eb8232fecff0e868447fa4bbee2eb815158e2bca5cb`；新ELF SHA `f6892b5f6fdb2004f700bf124003cfa7845854468b63723204d795dc24aa2e1b`。第一次Image共有47,137B不同；[section差异](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/static-difference-section-analysis.json)与[对象差异](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/static-difference-object-analysis.json)定位到一个32B临时banner增长及其link布局变化，没有发现关键驱动代码对象变化。

## 唯一metadata因果与一次恢复

[init/Makefile:31](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/linux-source/init/Makefile:31)的 `or(KBUILD_BUILD_*, auto)` 让显式final时间/#28同时进入temporary header。旧 `init/utsversion-tmp.h` 是 `#define UTS_VERSION "# SMP "`；首次重现变成完整 `#28 SMP Tue Oct  6 20:36:51 JST 2026`。旧/new init/version.o的.rodata为348/380B；[version.c:43](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/linux-source/init/version.c:43)明确这是先weak、后final的版本数据。

root据2078/2079证据授权一次定向恢复，提前保存[恢复receipt](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/metadata-recovery-receipt.json)及首轮Image/ELF/对象/headers副本。第一命令在同source/O以空KBUILD_BUILD_VERSION/TIMESTAMP只生成temporary header并CC init/version.o，exit0、0.374296秒，header和对象当场与旧完整字节相同。第二命令固定final元数据并以三项 `--old-file` 企图保留临时header、finalheader和finaltimestamp对象，exit0、6.717798秒，但外层选项未阻止内层Kbuild重生temporary header并再次CC version.o，最终又回到首轮Image。见 [命令2原日志:5](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/metadata-recovery-command2.stdout:5)及[实际结果](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/metadata-recovery-result.json)。这是本次构建入口选择错误，未把其写成技术成功，也没有追加第二恢复。

最终[2079项复核](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/final-linked-input-comparison.json)仍为2078相同，唯一init/version.o。已经精确相同的final version-timestamp.o在恢复前后hash及mtime均未变；生产46项输入hash均未变，生产source/config/git/旧out/linux只读。设备、凭据、ordinary adapter、firmware、部署、科学回放和共享文档操作均0。

完整Image来源接受仍有最后一个生成metadata入口缺口；本席依root“仍不等就停止”规则结束，不再search或编译。新源码冻结与2078个真实代码对象已提供可亲核的材料，交由root决定整个身份关的接受范围。

补充留存边界：第一步与旧hash相同的本次新 `init/version.o` **没有独立副本**，只有当场比较及实际编译日志；不能用旧.o补造。最终留存对象已被第二步覆盖。真实顶层argv及 [link入口原文](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/reproduction/root-link-entry.raw.txt)已写机器结果。
