# stage110：cpn3-clean 内核来源与最小 ordinary 构建路线

结论：**本地匹配 Image 的精确配置已恢复；25 项历史输入及全部交付文件均逐项吻合；完整历史 dirty Linux 编译源码仍未闭合。** 现存 `c0f5e13ea81f` 加已对上 hash 的 overlay 可组成恢复候选，但不能据版本横幅、当前 HEAD 或 `.cmd` 推定每个旧对象的实际源码。建议需要新 ordinary adapter 时，在独立源码与全新输出目录构建一次，保留生产 Linux、贡献分支及旧产物。本报告只读核查，没有构建或设备操作。

- 机器可读裁决：[source-correspondence.json](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/source-correspondence.json)；构建接口与依赖：[ordinary-build-route.json](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/ordinary-build-route.json)。
- hash 清单：[audited-file-hashes.json](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/audited-file-hashes.json)；实际命令、UTC 与耗时：[command-ledger.json](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/command-ledger.json)。

## 已证明与边界

| 项目 | 实际证据 | 可接受的结论 |
|---|---|---|
| live/build 标记 | stage109 `current-live-evidence.json:6-18`：boot `db0664f0-2d67-4b0d-91fb-c67d7557abe6`，notes SHA `9dc6815e908b46c1dbd7819ba2df6f010b1f9f8f671100d7c1d4d1bfdababa56`，GNU ID `1e019b5536c7825dfbb5fdfd26a8ce93a9f9fb3c` | 继承已有 live notes→本地 ELF/Image 的构建身份关联；子席未重新访问设备 |
| 当前本地 Image | SHA `8c19da27ac91c9f2a9d6b1fc7267a2810b2432393e8cf3921455926e6e5cb6bd`，24,131,072B；与旧 software state:185-188 和交付 Image 相同 | 对应同一已保存二进制 |
| 精确 config | Image 偏移 16804376 的 `IKCFG_ST` 至 16834208 的 `IKCFG_ED`，解压134,589B；SHA `b9af38b79a9f0d7c5348bb70ea4641ac0f7e21a4e44119c63a1e653639c2933d` | 与 `out/linux/.config`、`software/delivery/linux.config` 逐字一致；不是用 defconfig 猜配置 |
| 配置 gzip | Image内29,824B，SHA `8ab984c8f0395ecebec09d4201ed8cb24fb220f73aa49e730bf88991f6ba681d` | 与 `out/linux/kernel/config_data.gz` 逐字一致 |
| 历史列出的输入 | state:44-165 的25项输入全部与现在相同，含 idma/research/UAPI、lowrisc、配置/DTS、OpenSBI相关文件 | 这些具体字节仍可恢复；不覆盖未列出的全部 Linux 编译依赖 |
| rootfs | 8,019,456B，SHA `a3075bc621c712bb78ad8234576700988149f057caff3aeffbc98fb8c8f8a705` | `usr/initramfs_inc_data` 解压逐字等于 rootfs；压缩数据完整出现在 Image偏移12,684,224 |
| DTB | 3,510B，SHA `aa9cdf4cb28700c83374205caca5ac0085a527f03aaeae49f7e19e8d2d3d90af` | 当前、旧交付及stage109 live hash一致；50/25MHz只是DT声明 |
| 当前 source | HEAD `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`；5个tracked dirty文件与lowrisc untracked均在记录输入范围 | 可形成现在的完整冻结候选；不能反推旧构建时完整source |

配置提取的源定义见 [configs.c:20](/home/xvhaocheng/work/iommu-platform/linux/kernel/configs.c:20)（`IKCFG_ST`、gzip、`IKCFG_ED`）及 [kernel/Makefile:142](/home/xvhaocheng/work/iommu-platform/linux/kernel/Makefile:142)（由 `KCONFIG_CONFIG` 生成）。实际结果见 [config-correspondence.json](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/config-correspondence.json)，精确全文见 [Image-embedded.config](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/Image-embedded.config)。若root需要直接 live 配置，应在root独占同boot采证中只读 `/proc/config.gz`；子席没有做该动作。

## 为什么完整源码还缺证明

1. [历史state:10](/home/xvhaocheng/work/iommu-platform/out/composition-clean-20261006/software/software-update-state.json:10)只记录platform HEAD/dirty；[builder:264](/home/xvhaocheng/work/iommu-platform/scripts/build_update_software.py:264)的实现没有保存Linux完整HEAD/tree/dirty清单或完整source归档。横幅里的 `gc0f5e13ea81f-dirty` 不补齐这些内容。
2. [builder:67](/home/xvhaocheng/work/iommu-platform/scripts/build_update_software.py:67)只hash固定输入组。25项全相同是有用原证据，但不证明所有内核头文件、Kconfig、Makefile或其他曾经dirty的路径。
3. [旧build log:20](/home/xvhaocheng/work/iommu-platform/out/composition-clean-20261006/software-build.log:20)至68显示增量链接：34行仅 `AR drivers/misc/built-in.a`，没有重新 `CC idma.o` 或IOMMU对象。保存的 [.idma.o.cmd:1](/home/xvhaocheng/work/iommu-platform/out/linux/drivers/misc/.idma.o.cmd:1)及IOMMU `.cmd` 是命令/路径依赖，不是旧source字节hash。mtime也不作为来源证明。

追加核验：现有 `vmlinux`、`idma.o`、`iommu.o`、`io_pgtable.o`、`lowrisc_ethernet.o` 的ELF均无 `.debug` / `.zdebug` section，DWARF rawline输出均0B；精确config的4807–4810行为DEBUG_INFO_NONE=y、DWARF5未启用。因此这些现有编译文件没有可用的DWARF源checksum，不能用这一条补齐关键dirty源码。只查这五个ELF，没有扩大检索或构建。见 [source-checksum-availability.json](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/source-checksum-availability.json)。

完整比较见 [historical-input-comparison.json](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/historical-input-comparison.json) 和 [historical-artifact-comparison.json](/home/xvhaocheng/work/iommu-platform/out/r107-platform-identity-recovery-20261012/kernel/historical-artifact-comparison.json)。检索限于已指定构建目录、历史软件交付和必要接口；“未发现完整冻结”只限本次检查范围，不断言整个历史备份中必不存在。

## 最小隔离路线（方案，未执行）

- **源码**：从当前Linux完整commit只读export到新目录，叠加本次已保存binary diff及精确lowrisc untracked输入；录下base/archive/overlay/完整staged源码hash。只在这个副本加root接受的必要 ordinary adapter，并另存patch。构建输出使用全新空 `O=`，不拷贝旧 `.o`。
- **保护IOVA/HPM**：`3b47ab1645e4`、`affbfbf6adda` 已是生产 `c0f5e13ea81f` 的祖先，两项实际 `merge-base --is-ancestor` 均exit0；[io_pgtable.c:206](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/io_pgtable.c:206)已保留leaf内offset，[iommu-bits.h:200](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/iommu-bits.h:200)已括住HPM宏参数。独立提交 `8e3a7406fadb` / `77805904b3c0` 及分支保留，不在生产树覆盖、reset、stash或重复cherry-pick。
- **配置**：使用已恢复精确 `.config`，仅将INITRAMFS_SOURCE改为冻结rootfs副本路径，选明确的新ordinary release，报告LOCALVERSION_AUTO行为及必要adapter符号的完整diff。固定并记录KBUILD_BUILD_TIMESTAMP/USER/HOST/VERSION；olddefconfig后核完整diff，Image完成后重新提取IKCONFIG确认相同。
- **最小rootfs/DTB**：直接冻结复用已有8,019,456B rootfs和3,510B DTB；无需重编七个旧benchmark或重跑旧实验。ordinary CLI独立静态编译、单独交付，其头文件来自同一冻结UAPI；保持旧ioctl/struct布局兼容。若必须内嵌CLI才额外repack，并冻结rootfs.py:8-29列出的所有输入。
- **编译**：kernel/OpenSBI用 `riscv64-linux-gnu-` GCC11.4.0/ld2.38；loader用 `riscv64-unknown-elf-` GCC10.2.0。host Make4.3、Python3.10.12、flex2.6.4、bison3.8.2、gzip1.10、DTC1.6.1已可用。kernel命令接口 `make -C frozen/linux ARCH=riscv O=new/linux CROSS_COMPILE=riscv64-linux-gnu- olddefconfig`，随后同接口 `-j12 Image`；新目录必须独立且首次无旧对象。
- **启动链**：只复制冻结 `tools/lloader/Makefile/rv64.S/loader_rv64.ld` 的source，以新Image+冻结DTB构建ARCH=rv64/TARGET=linux-rv64-cva6。OpenSBI base `e8d21bfdc2dd753314627968e496c3a1188f6dfb` 加现存 `platform.c` overlay冻结，使用新 `O=` 与 `PLATFORM=fpga/ariane FW_PAYLOAD=y FW_PAYLOAD_PATH=new/lloader/linux-rv64-cva6.bin`。旧payload29,363,656B，相对32MiB窗口余4,190,776B；新payload须实际核大小。
- **ABI**：[misc/Makefile:71](/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/Makefile:71)以CONFIG_RISCV_IOMMU连接idma.o，不能找不存在的CONFIG_IDMA。现有RESEARCH2 ioctl0x10、COMPOSITION ABI1/128words/ioctl0x11是旧接口；新ordinary API需要明确version/struct_size、kernel所有权和root接受的mapping/leaf/fence/cache/NEXT-DONE/guard协议。本子席没有实现、编译或运行它。

**不能直接把旧 `--output-dir` 当隔离构建。** [platform.py:176](/home/xvhaocheng/work/iommu-platform/scripts/platform.py:176)仍覆盖production `out/linux/.config`、repack生产rootfs；[platform.py:188](/home/xvhaocheng/work/iommu-platform/scripts/platform.py:188)仍写production lloader/OpenSBI；[build_update_software.py:272](/home/xvhaocheng/work/iommu-platform/scripts/build_update_software.py:272)只把交付/state移到另一个目录，仍调用同样的production构建函数。应使用上面显式隔离路径或root审过的有限wrapper，避免旧source/config及旧输出被覆盖。

## 成本与接受条件

旧软件state起止共10.092114秒，只是增量软件构建，不能当ordinary干净构建的预计耗时。现在Linux目录7.1GiB（含git）、旧out/linux390MiB、OpenSBI5.6MiB/旧输出98MiB；本机可用磁盘约1.3TiB、内存121GiB。新source快照、干净Image、CLI、loader和firmware的真实时间/空间分别记录，现未测。prepare/map/leaf/fence/cache/issue/done/verify/unmap/free等R107运行成本也未测，不能用构建成本替代。

接受新内核来源需：完整冻结输入+新增adapter patch、完整config diff、固定构建元数据与命令、Image提取config一致、ELF/Image/DTB/payload哈希与build ID、payload尺寸符合窗口。随后设备更新、同boot身份和配置/DTB核验由root负责；有效FPGA链、bit、S0、clock/timing/route门槛仍独立适用，齐备前不执行R107恢复。本轮没有重跑stage106/108，也没有扩场景。

本子任务全部由Codex Sol/ultra完成，零DeepSeek/Claude/Astra；source/config/branch/device/credential/shared-doc操作均0，输出仅写本轮kernel目录。
