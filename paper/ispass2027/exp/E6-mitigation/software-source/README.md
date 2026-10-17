# G唯一正式软件构建候选

Root已接受Linux `69d8b9d4549c473477bcdc1df7c09ee1bb55528c` RUN小批。
本批driver不改，只新增platform G CLI与对应gitlink/lock，平台tip
`6491dfd76fc92dc020d17166b327da93605f58b1`，parent `d5cc44834a89d8e1633a028ad82d6e1f26b38831`。
[实际patch](platform-g-software.patch)、[增量bundle](platform-g-software.bundle)及[verify](bundle-verify.log)保留；
bundle前提为d5cc，同名内部branch `ispass/e6-mitigation`，未推第三方upstream。
Linux恢复继续用上一批`../linux-validation/linux-g-run.bundle`，前提889f，tip69d；不重复或改写原件。

[精确身份](component.json)与[唯一builder](../build_g_software.py)固定平台6491/Linux69d、
OpenSBI e8d21b、CVA6 cfb9a0a8。CVA6只是软件source reference；真实BIT由Root运行load manifest另绑。
**正在运行的唯一G BIT仍使用原d5cc/65b参考身份，不被本次软件更新改写。**

软件builder复用[原full-E3 builder](../source/build_full_software.as-reused.py)的
olddefconfig→Image→dtc→lloader→OpenSBI→32MiB门→匹配SPI模块流程。
[完整可审差异](software-builder-vs-full-e3.diff)保存。精确复用的archive/newc helper在
`../source/platform.as-reused.py`与`../source/rootfs.as-reused.py`，均与6491平台Git字节一致；
运行只读共享repo Git objects，导出和构建只写本worktree E6/artifacts。

rootfs/config/DTS取已passed完整E3包
`/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-software01`。
原manifest SHA7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4，
原rootfs SHA784a147961a9a51a49dd451bba2aaae72f937dd396eeb2a6fbfb940b4788800d。
所有原rootfs entry body/mode保留，含原idma_experiment/P0/scaling/pages/workload/timeout/updater；
仅加入新静态idma_g。保留旧CLI不表示把其数据与新G协议混池。
正式release `6.5.0-rc2-ispass2027-g-v1`。

`prepare`只交叉编译静态G CLI／生成rootfs，不构建临时kernel。
`finalize`只接受prepared包和同一builder/helper/input/rootfs/CLI字节；先标building，再独立archive69d/OpenSBI。
任何异常保存failed，原包不重试／不覆盖；不改变配置或payload门寻求通过。
SPI模块使用实际新release、vmlinux.symvers及modules_prepare生成的module.lds；不绕过modpost。
模块仅为后续软件更新，正式测量不加载。

CLI507项和软件入口9项有限检查已通过，见[CLI导航](../cli-validation/README.md)。
当前**没有运行正式prepare/finalize，也没有G Image／payload／新G板测**。
Root审实际CLI/builder并实跑有限检查后才执行本次唯一Image构建：

```sh
python3 paper/ispass2027/exp/E6-mitigation/build_g_software.py prepare \
  --out paper/ispass2027/exp/E6-mitigation/artifacts/g-software01
python3 paper/ispass2027/exp/E6-mitigation/build_g_software.py finalize \
  --out paper/ispass2027/exp/E6-mitigation/artifacts/g-software01 --jobs 8
```

最终交付须以该目录`build-manifest.json` passed与实际Image/fw_payload.bin/SPI模块为准，
planned/source_checked不等于ready。Root独占硬件；本builder没有SD/UART/JTAG操作。
collector另小批：10区组×Solo/A8gap0/1000/2000共40点，不扩大协议。
