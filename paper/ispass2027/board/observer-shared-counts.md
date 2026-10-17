# 私有入口与共享入口的计数边界

2026-10-17，Root A 对新 P0/E2 采集器的源码审查记录。只修正软件计数解释，不改 RTL、不增加板测或验收运行。旧 E1b 采集器保留既有检查；若实际原始记录触发解析问题，再依据完整证据处理，不重测替换。

## 原始源码

本记录的 `HW` 为实际 baseline 冻结目录：
`/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6`。

- `HW/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:910` 起将 observer 的私有 AR/AW 接入 `axi_dma_tr_req/resp`；共享 AR/AW 则接 `axi_iommu_tr_req/resp`。
- 同文件 `990–1014` 的 `axi_mux` 实例不覆盖 `SpillAr` 或 `SpillAw`。`HW/vendor/pulp-platform/axi/src/axi_mux.sv:49`、`:53` 两参数默认均为1。因此私有握手和共享握手之间存在寄存器库存。
- `HW/corev_apu/fpga/src/dma_path_observer.sv:132–136`：stage0 的 enter 是选中端口首次 offer，leave 是带对应 mux ID 的共享 AR/AW 握手；每个 role/channel 独立统计。
- 同文件 `37–44,73–87` 的 `full` 与 `carry` 只在 active 窗内 leave 时累加；前者在窗内进入，后者在窗前进入。同一窗内的全部 stage0 leave 数为 `full+carry`。

实际完整路径和输入版本仍由既有 baseline100 manifest/source.json 定位，不以当前研究 checkout 替代。

## 新 P0/E2 采集器的守恒

设 `leave(k)=PATH[k].full+PATH[k].carry`，则：

```text
V_selected_shared_AR = leave(vr0)
A_selected_shared_AR = leave(ar0)
V_selected_shared_AW = leave(vw0)
A_selected_shared_AW = leave(aw0)
```

P0 只有 V 和最多一名 A；shared 总量必须分别等于上述两个角色的 leave 之和。Solo 的 A PATH 全零。这比直接把不同测点的私有握手相加更准确，并保留共享侧完整守恒。

E2 有更多 A 时，shared 总量减去这两个 selected role 的 stage0 leave，得到**未选中端口在共享入口的接受总数**，须非负；它不能拆成 DMA2/3 各自的请求时间或 PATH。只有两设备时该残差须为0。

`shared_AR - (V_private_AR + A_selected_private_AR)` 另有 mux 开闭库存差，若保留应使用有符号值并注明测点边界，不能把它直接标成未选 A 流量或一律要求非负。AW 同理。

原有每个私有 lane 的 opening+accepted=returned+closing、16条同sequence PATH 的库存/流量和级间守恒、V全窗内完整性均保留。P0 的 V 事务数另由真实 backend 对64/1024/4096B的有限验证确定，不再无条件沿用每描述符2笔。

有限采集协议验证应覆盖 closing 时 A 已在私有入口接受、共享 spill 仍有库存的合法记录；共享 PATH0 守恒成立时不能仅因私有/共享计数差而拒绝它。该合成协议验证不能冒称新板测或硬件时序验证。
