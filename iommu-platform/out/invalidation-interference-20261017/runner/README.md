# 原版失效路径驱动器

`invalidation_dma.cpp` 使用固定 F5 完整 legacy IOMMU 和真实双 iDMA，所有寄存器操作都经过真实 AXI MMIO。C++ 只提供软件操作调度、行为 DDR 和只读观察；没有直接 flush、寄存器 force、研究控制器、命中旁路或 P/PN 分区。实际构建来源见各 `build/*/build-manifest.json`；每次运行的完整命令、二进制／源码哈希、退出码和时间见 `runs/*/campaign.json`。

普通文件镜像可独立复建：在本任务目录运行以下命令，`--out`必须是`build/`下尚不存在的新目录。需要Python 3.10+、Verilator、g++及make；原构建工具版本记录在manifest内。

```sh
cd /path/to/invalidation-interference-20261017
python3 runner/build.py --from-frozen --cpp runner/invalidation_dma.cpp --out build/rebuild-frozen01 --jobs 8
```

`--from-frozen`逐文件核验`frozen/source-manifest.json`中source/include/license的SHA256，按source角色的原顺序及原include roots编译。它不读取生产Git或导入生产tests；缺文件、哈希不符或顺序不符直接失败。`runner/`中的top、stub和CPP会在新build目录留存独立输入快照。build04验证了此模式，只构建，未运行仿真；既有28组结果仍绑定build03。build03原builder字节保存在`build03-builder.py.snapshot`，SHA256为`d03fb3df9be57ad70a8dd4adccd466646c2b85911d14ce89b32b09fad49a46f8`。

仅重新分析既有28组原始数据时，安装固定分析依赖并选择新输出目录；这些命令不会构建或启动仿真：

```sh
python3 -m venv runner/.venv-analysis
runner/.venv-analysis/bin/python -m pip install -r runner/requirements-analysis.txt
runner/.venv-analysis/bin/python runner/analyze.py --summary runs/matrix01/*/summary.json runs/sensitivity01/*/summary.json --output analysis-reproduced01
```

DDT 使用合法两层、32 字节 base DC，DID 为 B=10、A=11；B PSCID=0x80000，异域 A=0x80001。同域共用页表根，异域根不同，S2 Bare，叶 PTE 的 G=0。固定 RTL 的单层 DDT 地址计算无条件使用 64 字节步长，首次 `smoke01` 因而读到空 DC 并停止，原始失败保留。后续只改夹具为两层 DDT，不改 RTL。所用 DID 均小于 64，使该实现的首级切片与规范首级索引均为零；不将本次运行外推到全部 DID。增加的共同 DDT 根读取成本对所有模式相同。

每组 flush 按普通 C 软件路径分两次提交：先 IOTINVAL.VMA＋AV0 IOFENCE，再经过真实 CQH MMIO 轮询，提交第二条 AV0 IOFENCE 并再次轮询。CQH 在读取命令时提前前进；读到第二条 fence 已被取走，结合串行 CQ FSM，才证明前一条 fence 与失效已执行。debug 只做观察和断言。AV1 marker＋后继哨兵仅用于独立烟测，使用私有八字节槽验证 AW/W/B，主矩阵不支付这个诊断标记成本。PR/PW/WSI 均为零、ATS 关闭，unmap 发生在本条真实 COPYDONE 经 DONE 寄存器被软件确认之后。

主矩阵每配置 A=64 COPY、B=256 COPY，每 COPY 4096 字节。A 每条 COPY 有两个 region 的 map 和两个 unmap，IOVA 从 0x20000000 附近向下使用独立新页，PA 不回收。B 固定源页加 3／7 个目的页，共 4／8 页。B 先独立 warm 64 COPY，直接检查 16 个 tag 中全部 B 页驻留，再做两轮目的页覆盖并确认零新增 walk，之后才从共同 t0 开始主窗。

| 模式 | map 和 unmap 的失效操作 | A64 主窗 IOTINVAL／IOFENCE |
| --- | --- | --- |
| M0 | A 全部页在 t0 前持久映射，主窗不 map/unmap | 0／0 |
| M1 | 每个 region 宽失效，AV=0、PSCV=0 | 256／512 |
| M2 | 每个 region，AV=0、PSCV=1，限定 A PSCID | 256／512 |
| M3 | 每个 region，AV=1、PSCV=1，限定 A 页 | 256／512 |
| M4-16 | map 每 region 宽失效；每 16 个 unmap region 一次宽失效，末尾排空 | 136／272 |
| M4-64 | map 每 region 宽失效；每 64 个 unmap region 一次宽失效，末尾排空 | 130／260 |

M0 是无失效基线，仍有 A 流式容量／替换干扰。M4 是延迟 unmap 模型，不声称来源 C 驱动支持 FQ。可选 `unmap-only` 仅作为四组 M1 敏感性控制，仍有完整 map/unmap 操作，但取消 map 后 flush；它不能替代主表完整 API 路径。主机直接写模型内存更新 PTE，不计 CPU 软件执行周期；计入的是真实 MMIO、CQ、DMA 与 DDR 周期。

DDR 只有一个 FIFO 读返回队列，不按 ID 重排；队首等待 16 个完整拍，再每拍一条 64 位 beat。**空队列、RREADY 有效时，AR 接受至首个 R 的间隔为 17 周期。** `ddr_first_cycles=16` 是已有字段名，表示队首等待拍数，不能解释为严格 AR→R 为 16。后排请求到达队首才开始等待；全部模式使用同一规则，无随机数。详见 `model-timing.json`。

lookup 按 RTL PT lookup episode 首拍计数；walk 按 PTW 在 IDLE 且未被 edge-trigger 屏蔽时接受 init 计数，两者不强制相等。每次真实 CQ 失效先保存全部 16 tag，按匹配范围确定实际驻留键，并在上升沿后断言对应 way 的 valid 已清零。只有这些键失效后第一次 lookup 为 miss 才归为 `invalidation_attributed_misses`；此前未访问过的键归 cold，其余无失效标记的 miss 归 replacement。每个 miss 恰属一类。数据验收逐条核满 4096 字节及 512 R／512 W beats／2 B，不能只看最终 DONE。

`events.jsonl` 与 `events.csv` 保存相同七个事件字段和 `era`；只用 `era=measure` 汇总主窗，`measure_begin/end` 明确边界。`jobs.csv` 保存每条 COPY 的身份、时间、地址和传输数。summary 的 T_A/T_B 为共同 t0 至各自最后硬件 COPYDONE；`A_final_flush_end` 含 A 最终 flush 软件可见完成，`T_end` 为它与 B 完成的最大值。二者相加与 makespan 不混用。CQ peak／entry-cycles 使用实际硬件 CQT−CQH，CQH 提前前进意味着该值不等于完整执行驻留时间；round waiting 包括两次 doorbell、两次 CQH 等待及共享 MMIO 仲裁。

`execute.py --stage smoke|matrix|unmap-only --binary ... --build-manifest ... --output ...` 只接受本任务目录下新输出，核验成功构建 manifest、二进制与当前 CPP 哈希，逐配置串行执行、每组单次，失败立即停止并保留原始输出。异常 summary／watchdog 包含真实 fault cause、CQ／TL／PTW／lookup 末态与最近事件；不得通过静默修改 RTL 或暂停 B 掩盖进度失败。
