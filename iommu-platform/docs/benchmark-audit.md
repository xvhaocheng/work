# 继承的 benchmark 核查（2026-09-08）

结论：当前是一套自写、由 Linux 驱动提交和轮询的 DMA 内存拷贝微基准。可用于基本拷贝检查、给定软件调度下的总耗时和 backend 活跃时间比较；尚不足以直接测量 Victim 独立完成时间或判定 IOMMU 干扰根因。还发现旧入口失配和 suite 错误处理遗漏。

## 核查范围与版本

定向读取 `bench/idma*.c`、`linux/include/uapi/linux/idma.h`、`linux/drivers/misc/idma.c`、IOMMU 驱动 HPM 初始化、iDMA active counter、构建与 rootfs 脚本、board smoke。未扫描整个仓库，未重跑构建或上板测试。

平台 HEAD `1db0cfd97b4d2f5bd29a707ca023d9ae4e93ee4b`；Linux `2ef3f4b82afd3d2b039d6c67f214e50f27f10198`；CVA6 `7f64251542f9eaf2e80dced55d87c9d6e6a2ab79`；IOMMU `da8a8011f2c52fb1e0c5fd8ae9b9757a5f0ffbb5`。三个相关组件 HEAD 与 lock 一致、工作树干净。顶层原有 `docs/validation.md` 修改和未跟踪 `AGENTS.md` 保留。

## 实际有哪些程序

| 程序 | 行为与范围 |
|---|---|
| `idma_bench` | 旧单路前端，仍请求 `0x01`；当前驱动不支持，因此正常向当前 idma 设备发起该请求会返回 `ENOTTY`。不应作为可用入口。 |
| `idma_pair_bench` | Solo + 双路 Victim/Aggressor；默认 independent，可选 lockstep；`--single --aggressor-id none` 是当前可用的 Solo 入口。 |
| `idma_multi_bench` | 默认扫 1～4 路，每路相同 size/iters/模式；每个 chunk 依次提交所有参与设备，等全部完成才进下一 chunk。 |
| `idma_once` | DMA API 分配源/目标，一次提交整个 size 的描述符，完成后检查数据；用于大描述符进度/拷贝检查。 |

构建脚本编译 `bench/*.c`，rootfs 脚本打包这些产物；因此编译通过本身不会检出旧 `0x01` 入口失配。board smoke 的 Solo 已使用 Pair 的 solo 模式。

Pair 默认：每路 size=2 MiB、20 次遍历、3 次重复，chunk=256 B/1 KiB/4 KiB；交换 DMA0/1 的 Victim/Aggressor 角色。CSV 的 direction 标签表示角色互换，并非 DMA0 向 DMA1 拷贝。

Pair suite 含三种 Solo（2m-linear、4k-linear、4k-random）与四种 Pair（2m/2m linear、2m/4k linear、2m/4k random、4k random/4k random）。Multi 默认扫前三种同质负载，另可选 dma-linear。

## 实际负载和计时

- 每个设备有独立的源、目标缓冲区，使用 `dma_alloc_attrs` 分配。普通 benchmark 源填 `0xa5`、目标清零；每次遍历拷贝整个 size，重复使用相同缓冲区。
- `4k`/`2m` 指手动 IOMMU 映射请求的页粒度；每个 DMA 描述符仍按 chunk 拷贝。`2m` 不表示一次提交 2 MiB，完整大描述符由 `idma_once` 测。
- `dma` 使用 DMA API 返回的地址，不能据此称为 BARE，也不能只凭软件 mode 标签断言最终页表叶项大小。
- random 是对所有 chunk 槽位做一次无放回洗牌，源/目标使用相同偏移；每次 iteration 重用该顺序。没有可设置、记录的随机 seed。
- 映射、分配、初始化和最终数据校验都在计时窗口外。窗口内有 MMIO 提交、轮询、CPU 循环及实际 DMA，不能称为纯硬件传输时间，也不是包含分配/映射的完整应用 E2E。
- Solo 每次提交一个描述符并等完成；Independent 每路最多保持一个软件未完成描述符，由同一个 CPU 循环交替检查、完成后补交；这不等于限制 AXI 内部只能有一笔 outstanding。
- Independent 的两路都是有限工作量，任一路完成规定遍历后就停止；不保证 Aggressor 覆盖 Victim 全程，也没有记录请求重叠比例。
- Pair/Multi 的 cycles/HPM 都是开始到所有参与流结束的公共差值，复制到各设备结果；不能相加这些重复的 HPM，也不能把公共 cycles 当成每路完成时间。
- `dma_active_cycles` 在 RTL 中按 `!backend_idle_i` 累加，排除 backend 空闲时间；包含数据路径等多种影响，不等于 IOMMU 等待时间。与 HPM 的采样边界也非原子同步。
- Pair suite 的 `victim_active_slowdown = (Victim active/bytes)_pair / (Victim active/bytes)_solo`。字段名已表明 active，不能改述成 Victim 完成时间 slowdown。

## 已确认问题与证据

1. **旧单路入口失配。** `bench/idma_bench.c:34` 定义 `0x01`，`:65` 调用；`linux/drivers/misc/idma.c:1615` 的 ioctl 分派只有 0x02～0x05，默认 `-ENOTTY`。定向静态核对确认其余三个前端使用的命令有对应分派。
2. **Pair 默认 suite 不因数据错误判失败。** `bench/idma_pair_bench.c:455` 只在 ioctl 失败时累计 failures，`:462` 随后即用结果建立 baseline/计算 slowdown；未检查返回的 errors。驱动校验失败仅增加 errors，运行函数仍可返回 0。因而 suite 可输出 errors>0、计算性能比且最终退出 0。`--single` 在 `:533` 检查 errors，Multi 也检查，不能混为一谈。
3. **数据校验覆盖有限。** `linux/drivers/misc/idma.c:631` 附近将整片源填为同一字节，`:732` 的校验只在全部迭代结束后执行。源偏移选错但读到相同内容、首轮正确后后续漏拷贝，都可能漏检。`idma_once` 使用随偏移变化的模式，但也不是完整地址唯一性检查。
4. **缺乏设备专属翻译计数。** `linux/drivers/iommu/riscv/iommu.c:2136` 附近配置全局事件，没有 DID 过滤；Pair 的 `idma.c:1173` 和 `:1182`、Multi 的 `:1307` 将相同公共计数写入多路结果。Pair CSV 已标 shared，Multi CSV 仍只标 cycles/iotlb_miss，更容易误读。
5. **稳态与对照不足。** prepare 后直接计时，没有显式 warmup/独立 cold-warm 测量阶段；多次 iteration 聚合初始与后续行为。Solo、Pair 调度循环不同，默认固定 case 顺序、随机顺序不记录，无法仅凭性能比排除软件供给差异。2m/4k/random 标签也不等于已证明 hit/miss 状态。

主要执行依据：`idma.c:556` prepare、`:747` Solo、`:841` lockstep、`:1086` independent、`:1201` Multi、`:1360` once；计数器依据 `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:175`。

## 结果该怎么用、下一步怎么收敛

已有结果可保留为给定版本/命令/调度下的拷贝微基准观测；成功记录的总 bytes / 公共 cycles 可描述整批工作吞吐，active/byte 可描述每字节 backend 活跃代价。它们不能单独证明 Victim hit 被 foreign PTW 阻塞，或区分容量、仲裁、内存竞争。

后续若要用作论文测量，应先修复入口与 errors 传播、加强数据模式；再定义 Victim 独立窗口及持续 Aggressor、记录重叠和 seed、分离 warmup/测量、校准 per-DID HPM，并补同负载 BARE/命中对照及直接阻塞观测。本次只核查和记录，未修改 benchmark、驱动或 RTL；这些改造尚未实施。
