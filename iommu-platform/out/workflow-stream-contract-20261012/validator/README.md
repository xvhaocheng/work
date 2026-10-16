# 固定 HERO 双缓冲功能核验

`validate_functional.py` 读取 root 已执行的 `trace.csv`、`output.bin`、`result.json`，不编译或重跑驱动。只接受已固定 commit、实际适配源和二进制/产物哈希；删除两个已声明观察块后，实际函数须与原 44–276 行逐字一致。

```sh
python3 /home/xvhaocheng/work/iommu-platform/out/workflow-stream-contract-20261012/validator/validate_functional.py /home/xvhaocheng/work/iommu-platform/out/workflow-stream-contract-20261012/functional-execution/run-default --negative-tests --output /home/xvhaocheng/work/iommu-platform/out/workflow-stream-contract-20261012/validator/acceptance.json --ledger /home/xvhaocheng/work/iommu-platform/out/workflow-stream-contract-20261012/validator/execution.json
```

原输入 A 从 0 开始；独立 Python 参考为每个输出执行完整 256 项内积，使用 `B[j,k]`，按 uint32 模数核验全部 65,536 值，并另交叉 `2*linear_index`。CSV 固定 20 列；全部字段、角色/线程、288 个搬运的 issue/DONE/wait、256 个 tile 起止、偏移、字节及六槽代次必须吻合。A 重用须等该 stripe 全 16 tile 完成；B 每 tile 重新加载，重用须等旧 tile 完成；C 输出须等全 16 tile，复用须等旧输出复制结束及原 wait。应用结束前全部作业完成并 wait、六槽释放。

负例只篡改内存副本：改一个输出值、删除一个 COPY DONE 并重排 event_id、把 B 槽重用提前到旧 tile DONE 之前并保持 event_id/host_ns 单调。原产物不改。实际命令、起止 UTC、实测耗时、exit 和拒绝理由记录在 `execution.json`。

核验仅支持真实主机功能与程序依赖合同。CPU memcpy 为同步，额外 512 个 barrier 改变时序；host_ns 仅用于观察顺序，不是目标周期，不能证明异步 DMA、AXI B 回收、IOMMU 干扰或新 G0 性能结果。默认 B=2I 的数值无法区分转置形式，因此另固定原函数索引。16,777,216 MAC 是保留源码的工作量，不是硬件退休计数；slot 非 alias 依赖独立 live malloc 与固定源码，20 列不含指针。八线程依赖固定驱动的实际 team guard 和标记，而不是每线程每 MAC 的日志。
