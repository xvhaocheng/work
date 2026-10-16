# R107 stage109 工件身份关（2026-10-12）

结论：现存 serial v5 BIT 的历史身份可核，但当前工件证据还不足以启动一次 R107 普通准备 pilot。当前执行路线、live 身份、选择的 adapter/软件兼容与完整 IO 时序接受仍缺新事实。本结果不作科学阴性，也不因旧工件 mtime 较早否定其可用性。

- 本地 serial v5 BIT：11,443,722 B，SHA256 `baca9197c69d5faf23c381953bf464460952dd97b3b26f65902e7f7b0a702f66`，精确匹配历史 Mac `flash.json:3`；历史刷写在 2026-10-04，不能证明当前 Dell 正在运行此 BIT。
- 原失败 manifest 是初次综合失败；`build-inputs.json:100–111` 保留作用域/源表恢复，恢复 manifest `:6` exit0。冻结 lock/profile 哈希匹配；18 项 overlay 中 17 项直接匹配，top 仅 builder 将 LegacyObserver1→0、Mode0→1，逆向归一后精确匹配输入哈希。这里只核已保存 provenance/overlay，未做全源树重建证明。
- 已记录构建 mode1；冻结 staged source 的 DMA0/1=512 beats、DMA2/3=3，入口 `ariane_xilinx.sv:172`、`ariane_peripherals_xilinx.sv:1207`。这不是当前 live mode 的证据，也不能据此把 mode0 或 stage106 私有credit修复并入生产。
- 原始 routed report `:159`：WNS0.177ns、WHS0.035ns、WPWS0.062ns，setup/hold/pulse failing endpoint均0；`:174` CPU clock 为50MHz/20ns。原报告哈希 `99abab12f38f9fbd109c480f50e974a3fecc2c0a8ed16bfaf5e14baead3d0395`。
- 完整 IO/clock 验收未成立：report `:45–48` 有6条关键警告（TIMING-4/6/7/27）；`:74–79` 有21个 input/23个 output缺delay、3个partial input；旧acceptance `:19` 明示 IO review=false。报告方法学快照自身提示可能非最新，正内部WNS不能替代这些缺口的接受。
- 当前 Image：24,131,072 B，SHA256 `8c19da27ac91c9f2a9d6b1fc7267a2810b2432393e8cf3921455926e6e5cb6bd`；banner是 `Linux version 6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty (xvhaocheng@xvhaocheng-dell) (riscv64-linux-gnu-gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #28 SMP Tue Oct  6 20:36:51 JST 2026`。当前DTB：3510B，SHA256 `aa9cdf4cb28700c83374205caca5ac0085a527f03aaeae49f7e19e8d2d3d90af`，声明 CPU50MHz/timebase25MHz。二者匹配 `fpga-migration-4dma-composition.json:29–30` 的后期 mode3 软件blob快照，与 serial v5 历史软件hash不同。此manifest指向 `out/composition-isolated-20261007/...`，并非旧generic profile目录；本席没有选择或审核新的composition执行路线。
- 当前 IOMMU Git HEAD为 `57eb5621802ef9ed8d77c684b9a03205c6630391`；旧锁 `sources.lock.json:96` 为 `238a39e66a07b27a67fb5749df40691682555027`。当前工作树有dirty源码，旧BIT不能标成当前源码构建。当前Image源码/config冻结也未从匹配FPGAmanifest得到证明。

最小后继事实：root选择明确的普通adapter/硬件/软件组合并接受兼容；取得当前可执行路线与新live身份；对选择工件的IO/clock约束缺口作有证据的接受。若选择当前cpn3软件，还需其构建source/config冻结证据。无需为了这些缺口重复R107源码/CPU字节回放、旧矩阵或综合。

导航：`gate-result.json`给11个前提的verified/unverified/failed；`identity-evidence.json`给数字、hash/mtime、overlay检查及当前Git状态；`manifest-extracts.json`/`raw-extracts.txt`保留原文、原文件路径/行号与完整hash；`current-platform.decompiled.dts`是从本地DTB只读反编译；`execution-ledger.json`记录边界。
