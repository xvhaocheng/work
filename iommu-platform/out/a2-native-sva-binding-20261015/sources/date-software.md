# DATE 2025 软件来源绑定：公开的 enabled IOVA 链已定位

2026-10-15，Codex-only，只读来源核查。**已找到此前漏掉的作者软件仓 `pulp-platform/hero-tools`，其 `carfield/main` 分支包含实际启用 IOMMU 映射的 FP32 AXPY、用户态库、Linux 驱动及固定 LLVM runtime。软件构建文件明确指向论文的 `Carfield/date_iommu_evaluation`。因此不能继续说“论文的 SVA 软件链没有公开”。**

不过，这次取得的是可固定、与论文高度相关的公开实现；**尚未证明它就是论文实验的完整准确快照**。而且源码中确有尾 DMA 完成、独立 consumer、unmap/unpin 的生命周期缺口，不能直接验收完整任务时间或当前平台可运行性。以下区分“已取得源码”“该源码没有闭合的义务”和“论文准确版本未确认”。

所有原件、固定 ref、URL、SHA256、精确行号见 [date-software.json](date-software.json) 的 `key_sources`；全部 HTTP 请求及返回结果见 [retrieval-ledger.json](date-raw/retrieval-ledger.json)，本地文件哈希见 [file-manifest.json](date-raw/file-manifest.json)。本文件中的源码路径均位于 `date-raw/hero-tools-carfield-archive-files/`，另标明者除外。

## 1. 版本与引用关系

| 层 | 本次固定取得 | 证明到哪里 |
|---|---|---|
| 论文 | 复用 `riscvsva2025.txt`，arXiv:2502.17398v1，§III-B / TXT 137–164 | Linux 6.9.0、custom driver、OpenMP、FP32 tiled double-buffer 工作负载的论文描述；不把说明性 `create_iommu_mapping` 当真实符号。 |
| 官方 slides/poster | [作者 DATE slides](https://pulp-platform.org/docs/date2025/Cyril_Koenig_IOMMU_DATE_video_slides.pdf) 最后一页仍指同一 Carfield 分支；原件已保存 | 没有另外给出实验软件 lockfile。poster 只声明 platform RTL 可用。 |
| 软件 | [hero-tools/carfield/main](https://github.com/pulp-platform/hero-tools/tree/47339e040c43ab4d44d9a870bc307251084a1ba4)，固定 `47339e040c43ab4d44d9a870bc307251084a1ba4`；作者 Cyril Koenig，2025-02-03 | [platforms/platforms.mk:113–115](https://github.com/pulp-platform/hero-tools/blob/47339e040c43ab4d44d9a870bc307251084a1ba4/platforms/platforms.mk#L113) **明确 clone `date_iommu_evaluation`**；129/159 项的相关树均未截断。此分支159项，archive 182505B / 160项（含根目录）。 |
| SDK gitlink | `pulp-platform/cva6-sdk@fb56489c97b41471b6b9d707f0a38b3bc789a76e` | HeroSDK 该 commit 的真实 gitlink；区别于已核旧 Carfield gitlink `98611e6…`。 |
| Linux 候选 | [CyrilKoe/linux@e4b0725a5089f6cc0fc5afec06f3091d98dffa68](https://github.com/CyrilKoe/linux/tree/e4b0725a5089f6cc0fc5afec06f3091d98dffa68)，branch `ck/riscv_iommu_v6`，head 2024-06-17 | SDK 的 `configs/buildroot64_defconfig_iommu:25–27` 明确指作者该仓/分支；该固定内核的 `Makefile:2–5` 确为 **6.9.0**。该引用是可变 branch，未证明论文用了本次解析的准确 commit。 |
| LLVM runtime gitlink | `pulp-platform/llvm-project@d8cffb8d7294483b45732283719e250116fbe6c0` | HeroSDK 的固定 submodule；已取得 `openmp/libomptarget/plugins/hero/{rtl.cpp,plugin-hero-dev.h,…}`。 |
| 硬件及 target runtime | Carfield `dbb4ad3380e6e81fa7eac3955e2340752f29dab0`，Snitch `269830af872fa943d8b079470120d42d62e1cb69` | 复用已核硬件版本；补取异步 DMA helper 与 barrier 关键边界。没有用 Snitch 裸机 double axpy 替代论文 FP32 程序。 |

作者身份由 Carfield DATE commit 的 GitHub author `CyrilKoe` 确认。作者同一账号的 `carfield/date_iommu_evaluation` 后续 head 为 `644418d…`（2025-05-19，仅 flashing/programming 变化）；递归树仍固定旧 SDK `98611e6…`，不能用该 gitlink冒充论文软件栈。软件仓 main 的 head 在2024-11，`carfield/main` 在2025-02；本次选择的是后者含实际 Snitch 和基准的分支。

同仓 history 还提供了可核关联：2024-09-12 `app: Update merge_sort for paper`；2025-01-07 `Add LLC bypass in driver and clean axpy`；2025-01-23 `sdk: Bump cva6-sdk with IOMMU support`；2025-02-03合并 `Add IOMMU to Carfield main`。这些支持“同一项目实现谱系”，不能替代准确实验快照证明。

**构建缺口具体是什么：** 固定 SDK 的普通 `configs/buildroot64_defconfig:23–31` 默认仍选 Linux 6.1.22；`hero.mk:35` 也写6.1.22。`_iommu` 配置才指6.9候选；本轮没有取得论文实验选用它的固定命令/配置日志。驱动使用 `BR_LINUX_DIR`，由 `scripts/setenv.sh:14–17` 查询实际 Buildroot 结果；不是已验证的6.9构建产物。本轮0构建。

## 2. 取得的一条真实链：FP32 AXPY

**机制是“显式 pin 用户页＋独立 IOVA 的零拷贝 DMA”，不是相同 UVA、shared-mm 页表或自动 unified shared memory。** 驱动把用户页重新映射到自己的 device domain；应用把返回的 IOVA 传给 target。不要把论文的广义 SVA 术语误写成共享 CPU 页表。

| 步骤 | 精确源码与行 | 已取得的事实 / 剩余义务 |
|---|---|---|
| 应用分配/开关 | `apps/omp/basic/axpy/axpy_main.c:48–74`；`apps/omp/common/iommu.c:22–49` | `argv[3]` 是 `do_map`；用户页 `aligned_alloc(0x1000,…)`。启用分支通过 `/dev/mem` 把 DDTP mode设为4；不是不可达 `if(0)`。 |
| 构建选择 | `apps/omp/basic/axpy/Makefile:7–17`；`sw/libhero/Makefile:26–33` | 默认 `carfield_snitch_cluster`；host/target 源分开；Snitch libhero 定义 `DEVICE_IOMMU`。 |
| 用户映射接口 | `axpy_main.c:96–109`；`sw/libhero/src/common/driver.h:90–109` | `hero_iommu_map_virt(NULL,size,x_host/y_host)` 调 `IOCTL_IOMMU_MAP`，返回 IOVA。库通过 `src/carfield/snitch_cluster.c:67–80` 打开 `/dev/cardev--1`。 |
| domain attach | `sw/hero-driver/carfield/carfield_driver.c:228–237,255–308` | 取得 legacy 名称 `spatz-cluster` 的设备，分配 domain 并 `iommu_attach_device`；再 identity-map设备内存/寄存器。该名字来自同一硬件的历史命名，不能据名认定不是Snitch。实际加载/attach成功未运行验证。 |
| pin/map | `carfield_fops.c:251–289`；`common/hero_iommu.c:13–59` | IOVA公式 `0x80000000 + (userVA % 0x40000000)`；`pin_user_pages_fast(..., FOLL_WRITE,…)` 后每页 `iommu_map`，并映射 `+0x200000000` 的LLC bypass别名。权限为读写；不是CPU虚拟指针直接递交。 |
| 供给 target | `axpy_main.c:112–136`；`axpy_snitch.c:17–28` | IOVA加 bypass offset；fence/LLC flush后先冷、再热 offload；`#pragma omp target device(1)` 传 IOVA/alpha/n标量。 |
| host runtime | LLVM `rtl.cpp:457–509`；`plugin-hero-dev.h:116–152` | init调用libhero；发送 START/function/args mailbox，等待 DEVICE_DONE。runtime把device1叫MEMCPY，但这里只复制标量参数，bulk矩阵走显式映射IOVA；**不能按device1名称误判SVA关闭**。 |
| target dispatch | Carfield `sw/tests/bare-metal/snitchd/omptarget.c:24–74` | core8读mailbox，全部核调用offload函数，barrier后发DONE。已复用原件，不重复展开旧裸机基准。 |
| FP32/双缓冲/DMA | `axpy_snitch.c:43–146` | 8个compute核＋core8为DMA publisher；两个x buffer、两个y buffer；每tile 512 floats=2048B。开始发x0/y0；每轮先wait_all，barrier后发下轮x/y并计算本轮，再barrier和发布本轮y输出。 |
| 最后完成 | `axpy_snitch.c:131–146`；Carfield `runtime.h:238–286,445–458` | helper明确是异步DMA，`dma_wait_all`轮询busy；**末轮y输出之后没有显式wait_all**，只有barrier/打印。不能把源码循环退出直接当最后DMA已写回。 |
| consumer | `axpy_main.c:96–101,125–154` | 存在比较循环，但`do_map`时`y_dev_virt=y_host`；冷跑后所谓恢复是自拷，host验证又原地改同一结果，最后比较相同地址。它不能作为独立正确性consumer。 |
| unmap/free | `axpy_main.c:165–172`；`carfield_fops.c:82–85`；`hero_iommu.c:51–59` | 用户free存在；映射记录list插入全注释；无per-buffer unmap/unpin接口，`card_release`为空操作。模块卸载detach/domain_free（driver.c:365–370）不提供每个用户buffer的pin所有权闭包。 |

末DMA的后继也已定向核：Carfield `runtime.h:51–80` 的barrier包含FPU fence和cluster barrier；固定Snitch `hw/snitch_cluster/src/snitch_cluster.sv:930–942` 的DMA busy输出未接，`1054–1061` 的barrier只接各core。`snitch.sv:2303–2306,2522–2526` 的CSR barrier等待barrier输入。**这没有给出以DMA完成为前提的DONE协议保证。** 打印或mailbox开销可能在某次运行中足够覆盖DMA尾部，但本轮没有运行证据，也不能把偶然覆盖写成协议。

## 3. 可以复用什么，不能借来的是什么

可以将这条固定公开源码作为A2的真实来源起点：source-level publisher确为core8，实际动作为DMA descriptors；`n=32768,alpha=1,do_map=1,noise=0` 是论文输入规模相符的**只读候选配置**（本轮未执行）。据源码可推导64tiles、128笔输入＋64笔输出，共192个descriptor，L1双缓冲合计8192B。不能借此声称已知道DMA实际accepted容量、服务曲线、DDR双头共现或完整时间。

完整闭环仍需四个具体输入/修订：

1. 固定准确构建：选择并固定6.9内核、SDK、LLVM、Carfield及配置，区分“公开近论文实现”与“论文原实验快照”。
2. 独立consumer：保留不可变原输入和独立预期输出，明确热跑恢复及结果可见性，替换当前alias比较作为验证依据。
3. 完成/释放协议：末输出DMA完成必须先于DONE/host consumer；随后逐buffer完成unmap、IOTLB完成、unpin和free，并保留可追踪owner。不能给缺失步骤任意成本。
4. 当前平台绑定：这里是单Snitch DMA、作者domain与legacy节点；能否映射到当前四iDMA/CVA6的publisher、owner和accepted资源，需要单独证明。不能自动继承HERO旧四案的发布图或当前平台物理服务。

不把别的同名kernel拼进这条链：该分支 `gemm/main.c:175–180` 虽创建 `A_iommu/B_iommu/C_iommu`，`188,198` 的target调用仍传 `A_phys/B_phys/C_phys`；仅有map调用不证明实际DMA使用IOVA。`benchmark.sh:27–49` 的尺寸与论文多项一致，但标签字符串不能证明某个准确构建启用了映射。这个局部事实不否定论文使用的其他版本或其结果。

## 4. 本次检索边界

复用既有两份 enabled-sva 报告及原件，未重复核旧5.10SDK/archive应用目录或旧裸机double axpy。新增只沿作者论文、官方slides/poster、Carfield/SDK相关refs、commit作者账号及PULP项目关联仓进入HeroSDK；找到实际链后停止广查。

GitHub网页被restricted时使用公开API/raw/codeload。若干SDK固定raw路径404，但小archive含真实文件，已按archive补齐；未据404判不存在。额外OpenBLAS分支的 `.gitmodules` 明确链接内部GitLab buildroot，公开访问跳转登录页；这个单点不能推出实验软件永久不可取得，也未用它覆盖现已取得的公开链。没有登录、发邮件、发issue、构建或实验。

旧HERO disabled-SVM结论只保留原范围；**新找到的HeroSDK enabled-IOVA源码是实质进展**。这轮接受“公开来源已定位”，保留“准确实验provenance与完整生命周期待闭合”，没有否定SVA方向、没有新性能数字，也不改变原MIG暂停及旧结果。
