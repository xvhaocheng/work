# stage111：有限 CAPS 计时区间分析

结论：若每次 CSR 读取确实发生在独立 Dell begin/end 区间内，可以给出相对 Dell `CLOCK_MONOTONIC` 秒的保守平均计数率上下界。串口、进程、ioctl 和关闭串口的开销只会放宽区间，无需假设往返平均分摊。至少三次样本、两个保证最短间隔不小于 5s 的相邻段可以检验共同恒定计数率是否与证据相容。样本不能单独证明之后的 DMA 阶段没有变频、停钟或漂移；Dell 时基本身未有准确度界时，也不能声称严格 SI 物理 Hz 的误差界。

当前实际样本为空，new kernel 尚未由本席启动。`actual_frequency_bounds=null`、`actual_calibration_accepted=false`。本席没有设备、串口、网络、凭据、编译、模拟 ioctl 或 goal 操作。已有 C／RV64／收据 validator／UAPI／kernel／冻结数据保持；实际 SHA 前后核验见 `timer-bounds-analysis.json`。

## 原始证据和接口

- `host/idma_finite_cli.c:151` 的 CAPS 只复制 raw `cpu_cycles/time_ticks/nominal_ns` 与 live identity，CLI 自己的 `userspace_nominal_ns` 来自板端内核时基，不用作 Dell 参考。
- `source/linux/include/uapi/linux/idma.h` 的 `idma_finite_caps.sample` 为同一冻结 ABI1。`source/linux/drivers/misc/idma_finite.h:34` 先读 CSR_CYCLE，再读 CSR_TIME，再读 nominal kernel ns；它们不是原子同时采样。`:561` 在 CAPS ioctl 内取得该 stamp。
- `live/board_query.py:130` 在串口权限、owner、控制台初始化之前记录 Dell begin；`:167` 在控制台 close 后记录 Dell end。必须由 root 核实该命令确实产生所解析的本次 CAPS。软件开销全部保留在事件区间中。
- `design/root-source-review.json` 只接受一个隔离构建；没有把 source review 或本单测当新 boot／板端验收。
- 当前 RV64 SHA256 为 `7fb60c67e3d7736d729a20939f69d725e5bd2be41b0a0a5afb66e4c667737cb9`，本次未重建。

新增 `host/calibrate_finite_timer.py` 无设备访问。输入格式为：

```json
{
  "schema": "r107-finite-timer-samples-v1",
  "samples": [
    {
      "dell_begin_ns": 0,
      "dell_end_ns": 1,
      "caps_JSON": "此处须填完整已解析的 CAPS JSON object；此示意不是有效样本"
    }
  ]
}
```

要求至少三条完整样本；同一 quiet DMA0／DID10／S0／单 hart 的 CAPS identity、零 quarantine、实际 ioctl 成功。CPU 与 TIME 各自要求所有 delta 正且无倒退，不做 wrap 推断。相邻事件区间重叠或接触时无法给有限上界，拒绝；至少两个相邻段的 `next.begin - previous.end >= 5e9`，否则拒绝。每个 pair 都加入约束，不按某个有利 pair 选择结果。共同区间不相交则拒绝恒定 rate 接受。

调用方式：

```text
python3 -B host/calibrate_finite_timer.py --input SAMPLES.json --output BOUNDS.json
```

输出保存完整解析输入、原始文本、原始字节 base64 和 SHA-256，包括被拒输入；数值主结果使用精确有理数，十进制展示向外舍入。不会根据 DT、板端 nominal_ns 或 CPU:TIME=2:1 推出物理频率。

## 数学边界

第 i 次真实 CSR 读取点为 `t_i ∈ [b_i,e_i]`，第 j 次为 `t_j ∈ [b_j,e_j]`。在 `b_j > e_i` 时：

```text
d_min = b_j - e_i > 0
d_max = e_j - b_i
d_min <= t_j - t_i <= d_max
```

设 raw counter delta 为正整数 `N = C_j-C_i`。观察到的平均 counter advancement rate，相对 Dell 秒的精确保守区间为：

```text
[ N*1e9/d_max, N*1e9/d_min ] ticks / Dell second
```

这个平均率区间不要求区间内瞬时频率恒定。若另采用恒定 tick frequency 模型 `C(t)=floor(f*t+a)`，两端的 tick 相位造成不到一个 tick 的 delta 不确定性；采用闭区间保守放宽：

```text
[ max(0,N-1)*1e9/d_max, (N+1)*1e9/d_min ] Hz relative to Dell second
```

CPU 与 TIME 分别计算全部 pair 的 lower 最大值／upper 最小值。共同区间只是“恒定 rate 尚未被输入否决”，不是恒定性证明。区间相对半宽为 `(upper-lower)/(upper+lower)`；这是几何不确定性范围，不是统计置信区间，也不用于捏造一个精确点值。

如果外部已有独立证据保证 Dell 时钟速度相对 SI 秒在 `1±ε` 内，可以提供整数 `dell_reference_rate_error_ppm`，`ε=ppm/1e6`。则 SI 频率的保守界为：

```text
[ relative_lower*(1-ε), relative_upper*(1+ε) ]
```

工具只传播这个显式误差输入，不验证或默认假设 Dell 的振荡器准确度。缺该值时 `conditional_SI_frequency_bounds=null`；相对 Dell 操作参考的上下界仍可报告。CPU 和 TIME 的真实 CSR 读取时刻可能不同，分别给界，不把两者强行视为同一个采样点。

## 后续 raw 阶段时间如何报告

在 root 已确认同 boot／同 hart、计数器连续且校准到阶段之间时钟没有变化的前提下，某个 raw stage delta `D` 与频率区间 `[f_low,f_high]` 对应：

```text
[ max(0,D-1)/f_high, (D+1)/f_low ] seconds
```

`stage_elapsed_bounds` 提供此条件转换。CPU 与 TIME 两类 raw stamp 分别报告区间；这些 CSR 的读取点不同，不能仅因为转换区间不同就混同为同一精确时刻。nominal kernel ns 和板端 userspace nominal 成本继续单列。Dell 包围的 UART 命令墙钟又包含进程、命令传输、ioctl、数据交付等开销，不能扣成精确 DMA 时间。

如果没有稳定时钟证据，保留 raw cycle/time ticks、两个校准段的平均率范围和独立 Dell 命令包围墙钟，不能把平均校准率直接升级为所有阶段的可靠瞬时 physical duration。真实新 boot／BIT identity、counter continuity 和稳定性仍归 root 接受。

## 实际离线检查

只运行了一次 `python3 -B host/test_calibrate_finite_timer.py`。两个正例使用明确 synthetic 的任意 37MHz／23MHz，并安排不对称 sample windows 与事件位置；检查保守界确实包含构造真值、±50ppm 的向外传播和 raw stage 条件区间。14 个反例覆盖缺字段、CPU 倒退、TIME 零 delta、重叠区间、两个 5s 下界不足、不相交率区间、不同 CAPS identity、quarantine、ioctl 失败、样本不足、伪独立板端 clock、布尔时间字段和无界参考误差。另核有理数十进制向外舍入。

完整 synthetic 输入、预期真值、结果和拒绝原因保存在 `timer-bounds-synthetic-tests.json`；实际命令、返回码、独立本地检查耗时、文件哈希与“实际样本缺失”状态在 `timer-bounds-analysis.json`。这些测试只接受区间算法，不表示板端计时校准通过。
