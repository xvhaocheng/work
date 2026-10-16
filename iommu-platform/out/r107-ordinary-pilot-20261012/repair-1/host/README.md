# R107 普通有限执行的 CLI 与主机收据校验

本目录由 Codex gpt-6.1-sol／ultra 实现、主机构建和离线验证。没有 DeepSeek、Claude、Astra、真实设备打开、真实 ioctl、旧 source/CPU 回放或旧 stage106 八案矩阵。静态 RV64 构建成功不代表板端执行通过。

## 交付物

- `idma_finite_cli.c` 直接包含同一冻结 `../source/linux/include/uapi/linux/idma.h`。新增 ABI1、CAPS 0x12、RUN 0x13；静态尺寸断言为 caps104／run4520／batch136／leaf88。
- `idma_finite_cli.rv64` 为静态 RV64 可执行文件；`idma_finite_cli.host` 为主机数据检查版本，拒绝执行 RUN，CAPS 仅输出 ENOTSUP 收据而不打开设备。
- `validate_finite_pilot.py` 动态导入旧校验器的 `Rejection/require/sha/digest` 基础函数，关闭 bytecode 写入；不调用旧固定 shape／64 笔／八案函数。
- `../data/` 只复制 stage108 六个冻结完整池资产；`raw-manifest.json` 保存原资产路径与固定 SHA-256，`SHA256SUMS` 供独立校验。
- `../validation/host-check-ledger.json` 为实际构建、数据检查、离线测试及主机模拟失败路径的命令、返回码、耗时、输出路径和哈希账本。

## 使用

```text
idma_finite_cli.rv64 --describe
idma_finite_cli.rv64 --check-data --data-dir DATA
idma_finite_cli.rv64 --caps --device /dev/idma0
idma_finite_cli.rv64 --run --device /dev/idma0 --data-dir DATA --output-dir NEW_DIRECTORY
python3 -B validate_finite_pilot.py NEW_DIRECTORY/receipt.json --data-dir DATA --result VALIDATION.json
```

RUN 的输出目录必须此前不存在。只发一次有限 RUN，不自动重试、不自动发额外 CAPS。两份初始 GPU seed 使用各 2,359,296B 用户参考缓冲；四份完整输出各 2,359,296B，记录容量为 192 batches／4096 leaves，flags/reserved=0，fault timeout=300000ms。参考资产依次流式校验，不另分配全池 expected 缓冲；额外读取块为 64KiB。程序内置标准 SHA-256，没有新的 RV64 OpenSSL 依赖。

每次 RUN 输出四个完整原始池、全容量 `uapi-run.bin`／`uapi-batches.bin`／`uapi-leaves.bin` 与 JSON `receipt.json`。JSON 保留所有阶段、release/status/quarantine、batch/leaf/layer、HPM、observer、composition 原始字段及实际 syscall errno；负返回也保存原始输出。写 raw buffer 失败同样保留各 errno 并继续尝试写 JSON。硬件失败收据不转写为 PASS。运行返回 0 要求 syscall、kernel status、受检释放、quarantine 和四个全字节比较均成功；普通失败返回 1，参数错误／主机拒绝硬件返回 2。

## 校验边界

成功收据要求真实先 GPU5/6/7→CPU5/6/7 的 48 logical runs，然后 CPU5/6→GPU6/7 和 CPU7→GPU5 的 96 logical runs；双方各 1,769,472B／432 个 4KiB 硬件任务。基础为 144 个 whole batches；允许真实 allocation extent seam 拆分至容量内更多 batches，并独立核每个逻辑 run 的连续、完整、无重复 byte coverage。每行保存一个实际 whole-batch NEXT ID，DONE 必须精确匹配该 ID，不能把它解释为每个 segment 的独立 acceptance。

校验器检查四池的实际 leaf/PTE/PA、权限、Sv39 页表深度、16MiB 全 backing、extent 归属及 PA/IOVA 不重叠；stage2 准备 checked fence、stage9 撤叶 checked fence 和 release 都必须成功。24 worker-layer 的 CPU 检查 ready 位于 stage7，全部 restore DMA 完成之后；12 joint-layer 只是 bookkeeping，不是 attention 或 token 完成。完整 output pool 同冻结 expected assets 双重 SHA-256 与逐字节比较，包含 padding／未分配槽 guards。全部 JSON 原始接口字段另与 `uapi-*.bin` 解码一致性检查。

kernel CSR_CYCLE／CSR_TIME 是原始数；nominal_ns 来自配置的 kernel 时钟，未做物理频率校准。CLI 在板端运行，`CLOCK_MONOTONIC` 依赖同一未校准 kernel 时钟；`userspace_cost_nominal_ns` 记录分配/数据载入、设备 open/ioctl/close、完整 hash/memcmp、raw 写出的 userspace nominal 成本，CAPS／数据检查为 `userspace_nominal_ns`。这些值不是独立 Dell 主机参考时间，也不是校准物理时间。root 另用 Dell `time.monotonic` 包围 UART 操作记录独立墙钟，与板端 nominal 数分列。HPM 为顺序原始快照，不能称原子精确窗口。

目前 ABI 没有单独的 per-beat trace、每个物理 task 的独立 NEXT、逐批实际源/目的寄存器地址、撤叶后全 PTE walk 原始数组，亦没有每个 joint-layer 的独立时间。收据只如实保存现有字段；对应低层检查由 kernel stage2/stage9/release 状态表示，接受与源码验证由 root 负责。覆盖型 guard_bytes_verified 与 source/destination 全池覆盖有重叠，不能相加充作独有有效字节。

`test_finite_receipt.py` 的正例是明示 synthetic 的离线收据，当前收据反例均拒绝。`check_failure_delivery.py` 链接主机 open/ioctl 包装器，实际设备与 ioctl 完全不可达，验证 EIO／部分阶段／quarantine／原 buffer 都被保存，且校验器拒绝该失败收据。它不是板端测试。
