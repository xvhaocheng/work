# G observer/wrapper有限小批

实际CVA6 tip `0f0403f56aff3f1ee10e18dccea7a9fd6f2f26a4`，从已接受前端
`2c7c68f59190b9ce8739efd7b67d92262b0021f0`增加observer与wrapper连接，独立分支/worktree保持不变。
完整diff=`cva6-g-observer.patch`，源/结果入口=`component.json`与`summary.json`。
本批未修改Linux/UAPI/collector、未构建BIT、未使用硬件；不合M或BTC3/OBS5。

## 实际连接

`dma_core_wrap`的BatchGapEnable默认0转发给已接受前端，三个实际只读tap
经`dma_core_wrap_intf`到`ariane_peripherals_xilinx`各lane数组，再进入observer。
peripherals新增`IOMMUGapEnable=0`同时门控前端和observer；上层ariane_xilinx没有启用它。
两维DMA分支的新tap均接0，旧普通一维/原RTL路径不变。
没有改变backend、IOMMU服务、AXI握手或原Depth4/PATH/hist文件。

`dma_observer.sv`内并列的`dma_gap_observer`只读真实issue/done/ID、GAP占用、target/gap配置及原open/close事件，
新bank768..799严格沿已锁布局，769/799为0，不新增身份或可写bank。
ARM使用实际tap，不使用软件期望；不合法/whole busy控制不能清旧数据或重ARM。
live ID配对独立于测量控制和测量1..4096 ID序列，warmup任意匹配ID合法；
abort边沿仍处理真实live/AXI事件。G bank abort不计abort边沿事件，冻结前一拍累计并置bit7，
后续drain只更新live/原AXI库存；原OBS4 counters保留原abort边沿口径，整样本因此invalid，不能混作合格测量。
正常V poll/FROZEN不触发G abort；A自然完成后独立冻结，V关窗不截断A。
只有合法完整A4096、无live pending和任何G错误时才豁免旧A-active检查。
G错误与whole未完成时的premature finish在旧status bit38参与valid，不能只在新bank暗记。

## 最终实际证据

| 入口 | 结果及边界 |
| --- | --- |
| `runs/g-observer05/G-run.log` | 19场景、269检查、143352周期，真实observer，合成task/AXI事件 |
| `runs/g-observer05/Goff-compare-run.log` | G关闭对精确6c5 OBS4的17场景逐前/后沿比较：35计数、128 PATH字段、全部512桶和overflow/MMIO |
| `runs/g-observer05/Goff-hist-run.log` | 原18场景stockflow/hist集成，原验证脚本复用 |
| `runs/g-wrapper05/simulation.log` | 69检查/126周期，两个实际dma_core_wrap经实际AXI-to-reg、真实frontend/taps、observer ARM与MMIO读回；真实backend被编译但没有启动DMA job |

G-on覆盖Solo、A在V前/窗内/窗后自然完成（双方各4096），各自elapsed/时间戳及V窗口一致、
真实GAP active占用、旧V poll不abort、A仍live期间clear/reARM不抹数据、
显式abort同拍A done+issue与后续R返回、开窗同拍done+issue、strict carry、关窗同拍issue carry-out、
非法真实target与运行中gap变化、测量ID从1、非法done、未完成batch消失和非法非ARM控制。
G-on Solo还产生读/写真实PATH事件，各4stage有非零full；每个合格案例检查全部512桶/full守恒。

另四项明确的**验证侧寄存器状态注入**只用于饱和/错误守卫：64bit timer、counter到最大值，以及window周期/库存损坏。
它们调用的是真实RTL状态和守卫，但不是合法流量、板测或运行2^64周期的证据；生产RTL没有test hook。
运行日志按case区分这些项目，source_board_cases=0。

wrapper配置实验实际写两lane target4096、A gap1000，确认ARM bank锁存这些真实tap；
随后把A实际gap改2000，原ARM快照仍1000且bit9/旧status valid拒绝生效。
该实验验证控制/只读tap连接，不证明DMA数据、突发、IOMMU或整个SoC行为。
peripherals/外层interface的完整连接保留实际源码和diff；未做整个SoC elaboration或Vivado，不能用本批宣称其面积/时序通过。

## 原件与恢复

每次运行manifest保存实际命令、输入hash、时间和编译180s/仿真20s预算；原test/candidate/gold与完整log保留。
最终observer与wrapper的实际候选字节均与tip组件源核对相同，前端仍为已接受SHA
`13c33cf32a1961f9f71b7433a9b85629c533eddacb1978dea31e65bf25902ba0`，没有重跑原39项。
`runs/g-wrapper05/dependencies/`保存62份依赖/头文件字节；inventory注明固定旧基线来源和事后捕获角色，
原绝对执行路径未改写。ariane常量stub只提供原依赖的FPGA_EN，不是CPU模型。
编译warning保留，含旧生成/打包类型WIDTH和旧AXI转换器/合法化器UNOPTFLAT；未修改这些依赖，不声称完整工具签核。

失败/中间证据保留：observer01的G13通过，但旧v3对照test仍期望OBS3而失败；只修test magic后02/03通过。
04只增加真实GAP占用刺激，未重复未变G-off组。
最终源码复核发现非法非ARM control0应是bit8、不是bit0，形成0f0403f5小修，05在最终真实源上重编所有三组与wrapper，全部通过。
wrapper01缺完整AXI include、02缺配置转换依赖、03缺stream_arbiter_flushable，均为有限编译清单失败，原件未删；04/05通过。

最小`cva6-g-observer.bundle`包含d4e9b081初实现与0f0403f5修订，唯一prerequisite为已接受2c7c68f5，
已在实际组件仓库verify（`cva6-bundle-verify.txt`）。不推第三方upstream，不改冻结树。
Root审此实际diff后才能排Linux/后续集成或完整BIT构建；本批未固定新的platform build pin。

有限命令：

```sh
python3 paper/ispass2027/exp/E6-mitigation/observer-validation/run_observer.py --cva6 /home/xvhaocheng/work-a1-e6-cva6 --output paper/ispass2027/exp/E6-mitigation/artifacts/NEW_OBSERVER_RUN
python3 paper/ispass2027/exp/E6-mitigation/observer-validation/run_wrapper.py --cva6 /home/xvhaocheng/work-a1-e6-cva6 --output paper/ispass2027/exp/E6-mitigation/artifacts/NEW_WRAPPER_RUN
```
