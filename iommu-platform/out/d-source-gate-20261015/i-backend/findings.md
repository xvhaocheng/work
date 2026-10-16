# I固定profile后端门：strict接口存在，ixgbe实例接不上现配置

2026-10-15。**单项裁决：否决“已有ixgbe map/DD/unmap已是当前固定平台的原生可执行链”。I继续`not_ready`，排序不变。** 因为已有构建配置未启用IXGBE，当前DTS没有PCI host/ixgbe接入路径；DTS中的lowrisc Ethernet也没有`iommus`关联。否决限于该实例的当前profile，不否决I方向。

同时，**“当前RISC-V驱动不支持标准DMA-IOMMU”这一可能原因不成立**：源码接受DMA domain，接有generic iommu-dma ops。本轮仅接受条件源码可达性，不接受已运行、正确性已验证或真实设备存在的推论。精确路径、SHA256、短原文见[sources.json](sources.json)，机器裁决见[assessment.json](assessment.json)。本轮未联网、未用DeepSeek/其他外部模型、未开子agent；0编译/测试/模拟/设备探测/板操作，未改驱动、配置、ABI或trace。

## 1. 源码层：条件strict后端连通

以下路径均相对`/home/xvhaocheng/work/iommu-platform/`，行号对应实际工作树字节。

| 接口 | 亲核事实与原文位置 | 可接受边界 |
|---|---|---|
| 分配类型 | `linux/drivers/iommu/riscv/iommu.c:1449–1469`显式接受DMA、DMA_FQ、UNMANAGED、IDENTITY、BLOCKED、SVA | 类型未挡住标准DMA；不是实际分配/attach成功记录 |
| DMA cookie | `linux/drivers/iommu/iommu.c:1911–1939`调用驱动alloc，写type，再为DMA域取cookie | cookie由core配置；不能因驱动alloc未直接取cookie误判缺后端 |
| 默认域 | 驱动`riscv/iommu.c:2082–2098`无def_domain_type override；core `iommu.c:1611–1664`按通用偏好分配，`:186–213`按passthrough/strict选择，`:2822–2899`尝试attach | 源码默认可选translated strict；运行boot参数、sysfs与其他owner状态未知 |
| probe到ops | core `iommu.c:408–446`先建/挂域再调probe_finalize；驱动`riscv/iommu.c:1352–1355`先清dma_ops再调iommu_setup_dma_ops | 不能只见清NULL就裁永远direct；后续setup才是关键 |
| setup成功 | `dma-iommu.c:1616–1636`检查domain为DMA域，init成功后`:1630`安装iommu_dma_ops；失败则告警 | 成功源码分支存在；失败、identity域和实际ops不能靠源码排除 |
| attach与页表 | `riscv/iommu.c:1512–1552`分配PSCID/root/pgtbl；`:1575–1658`要求endpoint/DC有效，finalize成功后写TA.PSCID/FSC并绑定域；`:1861–1891`转交map/unmap | 未验实际CAP、分配、attach、硬件效果；`:1601`研究域DID10/11限制受research_domain条件保护，不排除普通DMA域 |
| API到失效 | `kernel/dma/mapping.c:147–181`选direct或ops；`dma-iommu.c:1588–1601`注册map/unmap，`:707–727`在strict unmap后sync；`riscv/iommu.c:1841–1845`转驱动flush | 只有实际ops/domain正确且设备用普通DMA API才有该路径；不证明命令已产生 |

短原文：

```text
riscv/iommu.c:1453: if (type != IOMMU_DOMAIN_DMA &&
riscv/iommu.c:1454:     type != IOMMU_DOMAIN_DMA_FQ &&
riscv/iommu.c:1355: iommu_setup_dma_ops(dev, 0, U64_MAX);
iommu.c:1935: if (iommu_is_dma_domain(domain) && iommu_get_dma_cookie(domain)) {
dma-iommu.c:1630: dev->dma_ops = &iommu_dma_ops;
```

**FQ边界：接受DMA_FQ类型不等于支持lazy队列。** 驱动`riscv/iommu.c:1198–1209`的capable只对CACHE_COHERENCY、PRE_BOOT_PROTECTION返回true，其他包括DEFERRED_FLUSH均false。通用后端`dma-iommu.c:618–620`明确在该能力不存在时降为DMA：

```text
618: if (domain->type == IOMMU_DOMAIN_DMA_FQ &&
619:     (!device_iommu_capable(dev, IOMMU_CAP_DEFERRED_FLUSH) || iommu_dma_init_fq(domain)))
620:     domain->type = IOMMU_DOMAIN_DMA;
```

所以当前源码下尝试lazy默认域也会沿此setup回退strict，不能把通用FQ实现算成当前RISC-V有效合批。普通合法合批仍应按真实能力、撤销合同和完整回收成本评估；本门不授权添加能力声明。

## 2. 配置层：有IOMMU_DMA，未启用ixgbe

- `configs/linux_defconfig:187`设RISCV_IOMMU=y；`linux/drivers/iommu/riscv/Kconfig:7–10`选择IOMMU_API、IOMMU_DMA、SVA、IOVA；通用`drivers/iommu/Kconfig:148–155`再选择DMA_OPS。`riscv/Makefile:1`编入核心、PCI/platform glue和页表。这是构建规则，不是本轮编译结果。
- **已存在**的`out/linux/.config:3924–3931`为IOMMU_DEFAULT_DMA_STRICT=y、IOMMU_DMA=y、RISCV_IOMMU=y，`:4731`DMA_OPS=y；`:1799`明确`# CONFIG_IXGBE is not set`。IXGBE的Kconfig `linux/drivers/net/ethernet/intel/Kconfig:142–145`要求PCI。tracked defconfig `:118–129`包含LOWRISC_ETH，没有IXGBE设置。
- `scripts/platform.py:179–189`构建入口先复制tracked defconfig到out/linux/.config，olddefconfig后构建Image并从当前platform.dts生成DTB。本轮只读规则和已有.config。已有.config不证明现在Image按当前工作树构建，更不证明已启动。
- `sources.lock.json:125–128`锁定Linux base为`c0f5e13ea81fa30d78f3bb1d6389e2397321e040`，实际Linux HEAD相同；但决定性`drivers/iommu/riscv/iommu.c`本就dirty。平台HEAD为`0a570deda89636631462bb188d39c058409df364`。因此用文件SHA标识所读字节，不以锁定HEAD冒充运行身份。

## 3. DTS层：四个iDMA声明共享实例，没有ixgbe部署

完整亲读当前`configs/platform.dts:1–177`：

| 声明 | 位置 | 本次判断 |
|---|---|---|
| iommu@50010000，compatible riscv,iommu，#iommu-cells=1 | 132–139 | 源码声明一个目标实例 |
| 四个iDMA master@50000000/50001000/50002000/50003000 | 141–175；关联147/156/165/174 | iommus指向同一实例，分别DID10/11/12/13 |
| lowrisc-eth@30000000 | 108–115 | 没有iommus属性；不能替代ixgbe的IOMMU生命周期来源 |
| PCI host、PCI iommu-map、ixgbe endpoint | 完整DTS 1–177 | 没有对应声明；CONFIG_PCI=y不能创造接入拓扑 |

驱动`riscv/iommu.c:1193–1195`从OF参数登记ID，`:1306–1313`要求合法对应fwspec与已probe IOMMU；非PCI `:1329–1332`取fwspec->ids[0]，`:1424–1428`用generic_device_group。四DID同实例不证明同一Linux group/domain，更不证明真实加载了该DTB。不能据声明推出本机、当前FPGA或外接设备存在。

## 4. 对已有I预筛的精确补充

ixgbe `ixgbe_main.c:8317–8329`普通TX map/记录descriptor、`:1142–1144`先查DD、`:1161–1165`再unmap的源码事实保留。**“当前源码树有这种正常驱动”成立，“当前固定profile可原生执行这种负载”被配置与拓扑门否决。** strict后端的条件接口已核，缺的是实例和运行身份；FQ则有明确能力回退。

实际设备、成功probe/attach、运行DMA ops/default domain/boot参数、正常TX与live任务共置、deadline/translation余量、命令/完成/回收联合trace仍未知。不给每包fence/失效率估计，不拿任意人造iDMA remap补成正常应用来源。

原strong baseline和反解释保留：

1. 先给正确PSCID/range、global/huge/nonleaf/alias覆盖及完成验证机会。驱动`:1804`仍注释PSCID setter，`:1807–1816`仍注释逐地址路径；旧host扩大失效不能支持新机制独有收益。
2. 普通合法合批、持久映射/可复用pool、分domain/context均保留，计IOVA、pin、保护窗口、回收和group/attach成本。当前FQ回退不等于所有普通合批均失败。
3. 作者full-walk容忍反解释保留：本地`smmu-range-response.txt:28–40`说明等时设备可能已有偶发full walk预算。它不是目标测量，却足以阻止“任何额外miss必违deadline”的推断。

## 5. 最小后继与接受范围

**接受条件strict后端源码可达性；本门整体否决将ixgbe链当作当前固定平台原生可执行实例。** I保持not_ready，不提高排序，不改D安排，不认定I创新/性能已成立。

最小后继是从已有证据中确定一个真实正常map/unmap任务及其实际IOMMU关联，取得构建/启动身份、实际domain/backend/strict状态与live余量/回收合同；没有就保留缺项。**此门不授权开启ixgbe、添加PCI/IOMMU拓扑、修改驱动/ABI或搭trace。** 另选具名端点须另立来源门，不能继承ixgbe的正常工作流证明。root须亲核决定性源码后接受。

建议/父任务请求：gpt-6.1-sol / high；工具未返回可核验实际模型与推理运行元数据，实际字段记unknown，不用请求参数冒充实跑证明。
