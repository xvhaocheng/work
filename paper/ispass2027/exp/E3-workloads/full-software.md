# 完整E3／共用E4/E5软件构建入口

当前冻结Linux=`65b427946152e3aca27babab0a437821729805d4`，platform=`ec99648e3c9a1e48c1e9721e3662bb632eb439bd`，
CVA6=`1d587125eee690e623e6ffae13e70f9bbe989adc`。
CVA6仅为software source-reference，软件构建不编RTL，也不表示加载了该BIT。
现有E5 OBS4 BIT来自6c5，后续full-E3 OBS5 BIT来自1d587，运行时分别由实际load manifest绑定。
正在运行的full-FPGA构建仍保留其原dfeb/da6 reference输入，不能事后改为46df。

[platform组件记录](source/platform-full-e3-cooperative-software.json)、[patch](source/platform-full-e3-cooperative-software.patch)、
[增量bundle](source/platform-full-e3-cooperative-software.bundle)已固定；前提4788，实际verify通过。
实际platform独立branch已commit，既有Git DB无remote，本批未创建remote或冒称推送组件远端。

`build_full_software.py`复用已审E4/E5 builder，仅更换固定pins、release、额外ABI5 CLI／critical源记录。
[原复用builder](source/build_pages_software.as-reused.py)保留。out严格限本worktree的E3/artifacts；
所有CLI从固定platform commit提取，不读取可变的外部源。保留原baseline三个binary：
`idma_experiment`、`platform-timeout`、`platform-sd-update`；另外编P0、scaling、pages、workload四CLI。
新ABI1/P0/multi/pages/workload UAPI与leaf/private源均记录哈希。

完整软件使用独立src archive、O目录、rootfs、lloader、OpenSBI和payload，不在编译过程中改源。
32MiB payload门保持。SPI原驱动source独立构建，先modules_prepare并要求module.lds存在，
使用实际vmlinux.symvers，不绕过modpost；正式内核release为`6.5.0-rc2-ispass2027-full-e3-v1`。
模块只用于该release后续软件更新，正式测量不加载模块。旧acece启动需要自己的P0-multi匹配模块。

Root已验收CLI与65b427的multi合作调度候选，批准本次唯一构建。旧4788/46df候选未执行Image构建，
原builder另存`source/build_full_software.46df-candidate.py`，原恢复包与Git历史保留。

执行一次：

```sh
python3 paper/ispass2027/exp/E3-workloads/build_full_software.py prepare \
  --out paper/ispass2027/exp/E3-workloads/artifacts/full-e3-software01 \
  --platform-commit ec99648e3c9a1e48c1e9721e3662bb632eb439bd
python3 paper/ispass2027/exp/E3-workloads/build_full_software.py finalize \
  --out paper/ispass2027/exp/E3-workloads/artifacts/full-e3-software01 \
  --platform-commit ec99648e3c9a1e48c1e9721e3662bb632eb439bd \
  --linux-commit 65b427946152e3aca27babab0a437821729805d4 --jobs 8
```

正式manifest将是`artifacts/full-e3-software01/build-manifest.json`，相邻`fw_payload.bin`、
`linux-build/arch/riscv/boot/Image`和`spi-recovery/{spi-bitbang,spi-xilinx}.ko`。
只有manifest状态passed才交付，不把planned或running当ready。

构建通过后，`prepare_full_transport.py`沿用E4的`delivery_manifest`／`gzip`／`spi_archive`字段，
生成确定性gzip(level6/mtime0)、仅含两匹配模块的tar.gz，并检查host解压逐字节一致。
模块vermagic读取实际ELF且必须匹配正式release。

```sh
python3 paper/ispass2027/exp/E3-workloads/prepare_full_transport.py \
  --build-manifest paper/ispass2027/exp/E3-workloads/artifacts/full-e3-software01/build-manifest.json \
  --output paper/ispass2027/exp/E3-workloads/full-software-transport.json
```

这些程序不含SD、UART、JTAG或上板操作。新软件可在严格已注册known-profile下共用于E4/E5 OBS4，
再由Root只切full-E3 BIT；旧da6 ready包与P0共同包均保留原字节。
