# E7 可移植源码编译包

2026-10-17 JST。**已在全新解包目录编译通过 16/32/64 项三种配置，均退出 0；本轮没有执行仿真模型或使用 FPGA。** 这证明所打包输入能重新编译，不代表重跑或重新验收历史 12 格结果。

生成器为 [build_e7_bundle.py](build_e7_bundle.py)，本机交付位于 `packages/e7-portable-20261017-01/`。`packages/` 由仓库既有忽略规则排除；归档、对象、可执行文件和日志均不进入 Git。生成器和本报告供主会话审查、提交。

## 包含内容与大小

| 内容 | 实际范围 |
| --- | --- |
| 冻结源码闭包 | 原 `frozen/source-manifest.json` 列明的 114 个文件：94 个源文件、10 个 include、10 份 license；逐文件 SHA256 相符，字节和为 **1,266,145 B（1.208 MiB）** |
| E7 实际输入 | `inputs/ariane_pkg_stub.sv`、`invalidation_dma_top.sv`、`capacity_dma.cpp`，另保存 `test-overlay.patch`；直接复制，不调用 `prepare.py` 重新生成 |
| 论文数据 | 完整 12 格 `results.csv`、原样 `analysis-import.original.json`、每格原样 `summary.json` 和 `jobs.csv`；导出时按接受回执核对哈希 |
| 历史身份导出 | 3 份构建和 12 份运行的中性派生清单；保留原清单哈希、源码 refs/哈希、参数、工具版本、历史可执行文件哈希及已带数据的哈希 |
| 新入口 | 包内 `build.py`、简短 README、忽略规则和 `bundle-manifest.json`；使用已安装的 Python、Verilator、C++ 编译器、make |

共 163 个 payload 文件，另有 1 个包清单；payload 共 **10,296,054 B**。源码压缩归档为 **1,634,855 B（1.559 MiB）**，位于 `packages/e7-portable-20261017-01/local/e7-portable-20261017-01.tar.gz`。

没有复制事件 trace、运行/构建日志、生成 C++、旧 build 对象或二进制、出版社 PDF 或凭据。唯一 `.cpp` 是实际手写输入 `capacity_dma.cpp`。许可证及上游版权声明保留；源码中的上游作者名不被删除，本包不构成双盲匿名保证。

## 来源与身份边界

冻结闭包来源仍为 `iommu-platform/out/invalidation-interference-20261017/frozen/source-manifest.json`；E7 历史来源仍为 `paper/ispass2027/exp/E7-capacity-sim/`。生产源码、原冻结 RTL、原始输入、接受回执和历史 build/run manifests 均未改写。

| 组件 | 固定 commit |
| --- | --- |
| CVA6 / iDMA | `ba8cdd10659a777ccff0080bf020be64b5c1e064` |
| IOMMU F5 | `85b157ebe71a623e326e51fff7b7126fe77e8eb9` |
| register_interface | `73de8e51b79f416350229b1d2420b2c527e002b8` |
| axi_slice | `aae8ca49dcfbfa8e44e1938a2e4a768db83006cb` |

| 入口或输入 | SHA256 |
| --- | --- |
| 原 source-manifest | `8774c0d383719b4805ff23bbae5418efb1b49e0c428f05a3af1b1594ac6e9ca7` |
| 原接受回执 | `f96f5a98abeba2f62772c8becb458069d3b552ef2e82c800fe7df8f6c104dfe4` |
| `ariane_pkg_stub.sv` | `81cdd45cee143d70f84e190ebabf2fed07073dba8564a0324bf98b62e0507af1` |
| `invalidation_dma_top.sv` | `26ac89c95a5ce582b3e563c098d5cbecb537eed36824e30b432b51c0a0c58980` |
| `capacity_dma.cpp` | `cb760532c17897f5f6bf1f5df05b11965547a3e6e50dfe26a02c2d0ca86ea1aa` |
| `test-overlay.patch` | `0c15e1bed06e48455f0b4f43fb104a577b45616dd36e2d1d9b71ae7ad0a50852` |
| 新 source identity | `f9970e05d4696fca7d718c590e9ba93854dea59e4d9bd8206a44bae8cc101af1` |
| 新 bundle manifest | `845784eada9bcd25ba3ab74789378c635d212cd96506246c74ce803eee162d32` |
| 源码归档 | `3490cd71c4fbb75b3b986980ad89afd550d367550e5bb95315611843faadea07` |

`source_identity_sha256` 绑定上述 refs 与包内全部冻结源码/输入哈希。三容量共用这一源身份，各自从对应历史构建清单保留容量参数、源顺序和编译选项。实际 SV `-GIOTLB_ENTRIES` 与 C++ `-DIOTLB_ENTRIES` 均保持为各自 16/32/64；包括 assertion、输出拆分、top module 和 include 顺序在内的其他选项不变。改变的是可移植文件位置、新对象目录和构建并行度。

包内 `historical/` 文件明确标注 `identity=derivative_not_original_manifest`，路径为包内相对路径，附带原 manifest 的 SHA256。它们不是匿名修改后的“原件”。`analysis-import.original.json` 保留原始字节，其 run-manifest 哈希指向未携带的历史原件；对应的中性导出单独列于 `historical/runs/`。因此不能把这些导出冒充原 manifest，或直接以其替换原 `validate.py` 的历史输入。

## 导出与独立编译

在持有原冻结闭包及已接受 E7 数据的工作树根目录，选用不存在的新输出目录：

```sh
python3 paper/ispass2027/artifact/build_e7_bundle.py \
  --output paper/ispass2027/artifact/packages/e7-portable-new01
```

导出器拒绝覆盖既有输出，并在结束时核对所读历史输入未变化。归档自动写入新目录的 `local/`。本地开发路径不进入 payload；导出器不调用 Git，也不调用原 E7 准备、构建或运行脚本。

将归档解到另一处后，进入包根目录：

```sh
python3 build.py --verify-only
python3 build.py --attempt compile01 --capacities 16 32 64 --jobs 6
```

这一路径只依赖包内源码和已安装工具，不需要 Git、生产仓库、开发机绝对路径或网络。尝试目录必须新建；`build.py` 不运行生成的可执行文件。其 build 输出均在包内忽略的 `build/<attempt>/`。

实际工具为 Verilator **4.038**（`rev v4.036-114-g0cd4a57ad`，与历史相同）、g++ **11.4.0**、GNU Make **4.3**、Python **3.10.12**。其他工具版本尚未验收。

## 本轮实际编译结果

从上述归档新解包到 `local/relocation-check-01/e7-portable-20261017-01/`，从另一个工作目录启动 `build.py --attempt relocated-compile01 --capacities 16 32 64 --jobs 6`，没有借用原 build 对象或可执行文件。下表的三项均绑定上表完整 `source_identity_sha256`，各自编译清单再次记录源身份、包清单哈希、工具、对应历史构建清单哈希、参数模板、新时间及退出码。

| 容量 | 源身份前缀 | Verilator | 状态 / exit | 编译耗时 s | 新可执行文件 SHA256 |
| --- | --- | --- | --- | ---: | --- |
| 16 | `f9970e05d469` | 4.038 | passed / 0 | 5.88 | `9bbcf185861cbb8f7f733f92a5cf8e6e261003872b5d78e6564643850f8a4c69` |
| 32 | `f9970e05d469` | 4.038 | passed / 0 | 7.14 | `6b8362c9a61a66c792b8accfe256296295c66f24194a4f1e93d6d13407453f55` |
| 64 | `f9970e05d469` | 4.038 | passed / 0 | 6.09 | `8e57df3bce93cfddc290a3fb9840d4dc947115308f366f3e19b46360d56826f7` |

这些耗时只记本次编译过程，不是性能测量；不据此声称冷缓存构建成本。新文件路径可能影响可执行文件字节，未要求它们与历史二进制哈希相同。

完整本机证据为包目录的 `local/package-checks.json`，及解包目录下 `build/relocated-compile01/summary.json`、各容量 `compile-manifest.json` / `build.log`。历史命令已转换成无私有路径的参数模板；新清单通过该模板、attempt 和源身份复原本次编译输入，不声称在执行前额外保存了未记录的原始 argv/cwd。

实际验证还包括：

- 独立重新导出到不同目录后，源码归档逐字节一致；其 SHA256 与上表相同。
- 新解包副本的 `--verify-only` 通过；在另一个独立副本改动 `capacity_dma.cpp` 后，校验明确拒绝输入。
- 归档恰有 164 个文件，无 trace/log、build 对象、旧二进制、PDF 或私有开发路径；三个容量的参数模板均仅指向包内源码。
- 三次编译后再次校验全部 payload 通过；`model_executed=false`、`fpga_used=false`。

## 可选手动复现单格

以下仅提供未来的手动操作命令，本轮没有执行。完成相应编译后，可在包根目录用新输出路径运行一个 Solo 格：

```sh
build/compile01/16/obj/Vinvalidation_dma_top \
  --out local/manual-c16-solo01 --case-id c16-Solo --a-pages 0 --v-jobs 4096
```

这会执行 RTL 仿真并生成新的输出；后续验收应另存本次命令和对应编译清单。它不使用 FPGA。模型仍为队首首拍 16 周期、每拍 8 B 的行为 DDR、普通 MMIO 供给及每设备一个在途 COPY。历史 12 格的仿真证据级别和供给口径随包保留，不转换为板测周期或吞吐。
