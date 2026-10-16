# E3 D0/D0 诊断软件交付

2026-09-18：软件完整构建退出 0，manifest passed，产物与清单哈希一致。主机验证通过，尚未上板。成功日志仅保存。

本次只更新 SD 启动软件，继续使用已经验收的 9 月 16 日 E3 BIT（observer v4、DS50/50），无需编译或替换 BIT。BIT SHA256：`a99ce9565b82449ca5eb0a8afc18c8b79cdf9e23a5a4e9197aafae02b5df98d4`。保持 V0/A1 和 CPU 整批轮询。

- `fw_payload.bin`：按原来的 SD 原始分区方式更新；新内核 `6.5.0-rc2-00085-g1b268b89200c`。
- `idma_e3_pair`：已经内置 rootfs，新增 `--diagnostic 1`；旧内核不支持，不能只替换程序。
- `E3_DIAGNOSTIC.md`：本轮实验逻辑、三个短验收和六对诊断、字段解释、结果判断。
- `idma_experiment`、`platform-timeout`：同时提供的用户态程序。
- `delivery.json`、`software-manifest.json`、`sources.lock.json`、`profile.json`、`idma.h`、`validation.json`、`SHA256SUMS`：版本、接口、验证及校验记录。

payload SHA256：`1e4a49b08bfcbcbeb1e86eb17fb0836d373d5950d61b274b885d443f7c458f62`。
构建起点顶层 `c5c4c43cbf4324a977fbcefea2abd18f4fd8c28a`；Linux `1b268b89200cf8d603ba006a0b614c2d40e064dc`。CVA6/IOMMU 未变。profile 描述沿用旧配置，其中“待上板”不代表旧 BIT 尚未验收；本轮新软件需要验收。软件 manifest 中的 BIT 条目是复用旧产物的哈希，不是新 FPGA 构建。

新输出 schema 为 `idma.e3_diag.v1`，两轮 D 均为 0。采集器须保留新增 `diagnostic` 字段，按两个 sequence 收齐各 19 条 batch/path 观测和原 256 个 E3 字段，保存完整 UART、退出码和 taint。原 D0/D128 命令及 schema 保持。真实错误停止，无触发不补采。

先三个短验收，再按 A8、A8、A1、A8、A8、A1 做六对 4096 项/warmup256 的诊断。软件 success 只表示执行检查通过；本轮用来调查双轮预热/启动前史，不证明 HOL 或 D128 效果。现 BIT 的 AR 队头触发覆盖限制仍在，待诊断后处理；原时钟/CDC/IO 边界保留。

主机验证：64 个驱动控制场景（新增 11 个诊断场景）、3 个缓存场景、96 个小 chunk 组合；E3 CLI 39、原 CLI 69、rootfs 5 项通过。仅主机模型与打包验证。

日志在本目录上一级 `software.log`，旧交付与板上原始结果保留。没有本机刷卡、JTAG 或新上板操作。
