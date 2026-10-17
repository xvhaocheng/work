# E5 第一批：有限真实 RTL 验证

结论限于被动 observer RTL，当前主证据为 `runs/run03/summary.json`，**passed**。
尚未执行新 Linux、完整 Vivado 或板测，没有 E5 性能数据。

实际候选是组件 commit `6c5c666d45979d3a03fb98cff89ba7ad095f6125`，基座
`8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8`。生产只改三个 observer 相关文件；
P0 frontend SHA256 仍为 `7c4837431c52a16f01e3df0bc9a8526be50237046c01a38fcfdd507a3be7ad30`。
完整 patch、提交与执行输入一致性见 `../metadata/`。

本地复现使用新输出目录，不覆盖已有证据：

```sh
python3 paper/ispass2027/exp/E5-latency/rtl-validation/run_rtl.py \
  --cva6 /home/xvhaocheng/work-a1-e5-cva6 \
  --out paper/ispass2027/exp/E5-latency/rtl-validation/runs/NEW_RUN
```

每次编译上限300秒、仿真30秒；run03 三次编译和三次有限执行总计4.94秒。
`manifest.json` 保存全部命令、源码哈希、基座、dirty patch、预算与返回值；
`src/base` 保留原字节，`src/reference` 只作机械 module 重命名以便同拍接入，`src/candidate` 是实际生产候选原字节。
生成的 wrapper 与 C++ 也完整保留，目标模块不是自写延迟公式。

| 检查 | run03 实际证据 |
| --- | --- |
| HistEnable=0 与已接受原 DUT 同拍、同输入 | `equivalence-run.log`：17 个原集成场景、923 个时钟，每个高低沿比较全部35旧计数、128旧PATH字段及MMIO输出 |
| HistEnable=1 的真实 MMIO/窗口/端口/读写集成 | `histogram-run.log`：18场景、192段桶总数与full守恒；已知2/3/2/4周期分段逐桶比对；最高index767可读、768和空洞193拒绝 |
| 32位真实 age 输入的桶边界 | `stage-run.log`：98个0/1/2/3、每个二次幂两侧、最大32位年龄样本，独立阈值模型比对 |
| FIFO/采样总体 | 实际stage对照事务队列模型：满队列pop+push、同拍空队列enter/leave、carry-in/out、open/close与run停止；共233模型步骤 |
| 溢出和冻结 | 实际stage将测试专用桶宽降至3，饱和7而full继续8/9；实际path传播mask0x000f至PATH error；8位timer wrap；abort后冻结、reset/clear恢复 |

旧17场景还含 held-valid、不同设备ID、队列满、非法return、非零private-ID、held output ID改变以及有未清流量时ARM。
保留原 Verilator `-Wno-fatal` 用法；日志中的 WIDTH 提示包含原3位队列count与整型Depth比较、旧MMIO数组索引宽度，
新索引由8扩至10且仍以`index<35`保护旧数组访问；没有静默吞掉编译错误或新增latch警告。

保留失败和改进过程：run01是测试比较wrapper未声明NumPorts/AxiIdWidth而在编译前失败，修正测试wrapper后run02通过；
run03进一步增加每拍全部字段比较与abort/reset检查，生产RTL与run02相同。原始目录都保留，run01不算通过样本。

这不证明50MHz实现可收敛、RTL对实际FPGA时序无扰动、Linux读取有效或板测样本分布。
Depth4和所有原错误检查保留；更长descriptor的容量合同须另行验证，不能用桶统计消除原容量错误。
