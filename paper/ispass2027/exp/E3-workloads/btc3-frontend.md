# BTC3 frontend：第一批实际源码与有限验证

2026-10-17，ARS experiment-agent/run，Codex-only。`FRONTEND_ACCEPTED`，Root已直接核原始源码与最终证据。
没有新 BIT，没有 ABI5 driver，也没有板测。当前继承 E5 的 OBS4／INFO1／Depth4，
仍不能用来采集长 COPY 的有效 PATH 数据；下一批[OBS5／INFO2／Depth64](path64.md)已单独实现、验证并交审。
已交付的 P0 BIT、共同软件和 45 点合同不变。

实际 CVA6 独立工作树 `/home/xvhaocheng/work-a2-p0-cva6`，分支 `ispass/a2-workloads`。
本批 commit `2db78c0927d6e70b819b46651df86e82ab139710`，parent 为已审 E5
`6c5c666d45979d3a03fb98cff89ba7ad095f6125`。只改
`corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv`。
源 SHA256 为 `19e34c67606ccb15f630b1731d7f58a9ab84bd8aa6ca3bb517c4670dfe7d7257`，
与实际编译 snapshot 相同。详细身份见 [component](source/btc3-frontend-component.json)。

## 实际接口

新 magic 为 BTC3 `0x49444d4142544303`（实际源第336行）。旧 kernel 明确拒绝新 magic。
原 NUM_BYTES 和 0x90 target、0xa0 completed、0xa8 issued 的单位仍是每个实际 COPY／row。
新增寄存器全为 64-bit MMIO，写入先验证完整数值，再截取内部位宽。

| offset | 寄存器 | 可写枚举 |
| --- | --- | --- |
| 0xb0 | geometry | 0 linear、1 page-packed、2 tiled rows |
| 0xb8 | rows/op | 1、4、16、64 |
| 0xc0 | pitch | 0、16384 |
| 0xc8 | matrix rows | 0、64 |
| 0xd0 | columns | 0、16、64、256 |
| 0xd8..0xf8 | reserved | read0；任何写置 sticky error |

源第107–135行验证组合，不能只看单字段枚举：

- Linear：row bytes 为64/256/1024/4096/16384/65536，rows1、其余几何0；span≥length且整除。
  硬件保留旧1..4096页与3/6页合同；未来ABI5只接纳已设计的power-of-two≤256页，不宣称任意组合。
- Page-packed：row bytes 为64/256/1024/1536/4096，rows1、其余几何0；不足下一槽则跳到下一页。
  1536B每页只发offset0/1536，最后1024B是padding，本测试只证明地址跳过，数据空洞校验留给driver批。
- Tile：256页，pitch16384、matrix rows64；row64/256对应rows1与columns256/64；
  row1024对应columns16、rows1/4/16/64。每row是真1024B COPY，64KiB logical tile不是一个64KiB COPY。
- 所有batch要求DEFAULT、CONF.deburst=decouple=0，S/D基址4KiB对齐，
  `base≤U64_MAX−span`，因此排除exclusive end等于2^64的最高页。非batch原寄存器行为不受这两个新guard约束。

第160–173行保留全byte strobe／active锁／STOP例外；CTRL=0额外重置新几何为linear/rows1。
第409–438行仅在COUNT ready握手后移动cursor；第443–455行的WAIT仍等该COPY完成，才到下一个COUNT。
STOP不会取消held row或补齐未发出的tile rows。源／目的使用相同offset，保持≤33 burst的有界论证前提。

## 实际有限验证

[报告](test-results/full-frontend03-report.json)、[原输出](test-results/full-frontend03-test.log)、
[编译输出](test-results/full-frontend03-build.log)、[全部事件复算](test-results/full-frontend03-event-audit.json)。
真实frontend由Verilator编译；backend仅在握手后7cycle发synthetic completion。
本批是前端MMIO／地址／长度／握手证据，不是AXI legalizer、数据内存、IOMMU、队列容量、时序或板测证据。

206情境通过，102,190个accepted COPY的实际src/dst/length逐条独立复算。
69情境逐周期对照冻结BTC2 `8aeb9ac`；其中27情境还直接对照BTC1 `8aac3fc`。
对比实际完整burst请求、valid、type、write data及MMIO响应，仅版本magic的返回值允许不同。
包含旧64/1024/4096与1/3/6/8页、旧STOP／active锁、六个代表性旧非batch长度、六size线性最小环和1MiB环、
全部tile分解、NIC1536 padding／页／环边界、九种独立V/A形态组合，及新字段的非法组合、全宽溢出、
partial strobe、reserved写、配置锁、地址溢出和COUNT/WAIT两个位置的STOP。
256B在BTC3是有意新增能力，不再当作BTC2的非法长度；zero和其余非法palette仍不能发出COPY。

[61个关键实际输出](test-results/full-frontend03-boundaries.json)带完整raw事件行号，
覆盖tile rows1/4/16/64的row、column、group和两次ring回绕，以及1536最后slot→下一页／环。
完整raw位于 `artifacts/full-frontend03/events.jsonl`，SHA256
`e9e41d91090790513b6c502ed39bdab23c60facb90c490f2302b6b73512ad00b`，本机保存不进Git。
编译版本和harness的不可变副本是 [frontend](source/btc3-frontend03.idma_reg64_frontend.sv)、
[wrapper](source/btc3-frontend03.full_frontend_top.sv)、[C++](source/btc3-frontend03.full_frontend_test.cpp)、
[as-executed runner](source/btc3-frontend03.verify_full_frontend.py)。

首轮01的RTL情境本身全部通过，独立后处理发现重复情境名而将整体报告记failed；原报告与log保留。
02修正情境名后161情境通过；03补齐旧STOP／锁双gold及九V/A组合后206情境通过，生产RTL三轮相同。
03原report没单列textually included `idma/typedef.svh`，
[额外源绑定回执](test-results/full-frontend03-header-binding.json)明确是执行后补录，
其hash匹配已冻结P0 build archive的既有manifest；不改03原report。后续runner已在编译前记录该header。
编译warning来自原生成reg_top／axi_pkg及BTC1旧位宽写法，本批新frontend没有新增warning。

复现（只运行host有限RTL测试，不访问硬件）：

```sh
python3 paper/ispass2027/exp/E3-workloads/verify_full_frontend.py \
  --cva6 /home/xvhaocheng/work-a2-p0-cva6 \
  --output paper/ispass2027/exp/E3-workloads/artifacts/full-frontend-NEW
```

## 恢复与下一批

[incremental bundle](source/cva6-btc3-frontend.bundle)已实际`git bundle verify`通过，前提为6c5c666。
[带commit patch](source/cva6-btc3-frontend.patch)与[原始review diff](source/btc3-frontend-review.patch)同批保存。
CVA6没有用户现有可写remote；只提交本地同名组件分支，不向zero-day upstream推送，也不创建remote。
顶层任务分支推送这些恢复材料；不将其描述成组件远端已推送。
平台pins待完整frontend＋容量批稳定后再更新，不修改当前build输入。

Root完成最终证据验收后，继续独立被动容量批和Linux／collector的OBS5严格判别。
新BIT必须在这些实际源码与有限测试再次通过Root审查后才构建，50MHz约束和失败停止条件沿用。
