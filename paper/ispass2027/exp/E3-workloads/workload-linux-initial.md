# ABI5 Linux第一小批：几何助手与OBS5

2026-10-17，Root已直接审查并接受本批源码设计。实际Linux独立分支`ispass/a2-workloads`的提交
`682a2caf0a7fdf69749ebdac146df60df55dcbe6`，直接parent为共同E4/E5冻结
`da6b69fc007ad5e671dc288dd8038d02e3ebe43d`。没有ABI5 RUN/CAPS handler，没有新Image或payload。
当前板测P0／acece软件与所有已完成archive不变。

[组件身份与三个源码SHA](source/workload-linux-initial.json)、[commit patch](source/linux-workload-initial.patch)、
[增量bundle](source/linux-workload-initial.bundle)已保存；bundle实际verify通过，前提da6。
Linux只有既有上游remote，本批不推上游、不创建remote，恢复材料随顶层任务分支推送。

## 实际代码边界

`include/uapi/linux/idma_workload.h`正式定义已审ABI5的caps64B、run544B、0x0f/0x10。
只补充已接受的有限常量与length mask，不改变草案结构／offset，也不把有header说成有运行能力。
私有`drivers/misc/idma-workload.h`只有几何验证、descriptor offset、prefix bitmap、页集合及union字节计算。
没有MMIO、分配、cache、锁、DMA或cleanup动作；尚未接入运行生命周期。

helper严格限定linear、page-packed、固定1MiB tiled rows的已审组合，pages为power-of-two≤256。
tile按row/column/group闭式公式；1536B只发每页0/1536两个slot，padding不入触及集合。
每bitmap为64B粒度、最大16384bit／2KiB；`prefix_bitmap`先将count限制为一完整环再枚举，
因此即使A实际完成prefix很大也只处理有限环，不枚举数十亿descriptor、不溢出offset乘法。
暖机与测量集合分别表示，union单独计字节，映射页数不冒充活跃页数。

`idma.c`本批只加入header和OBS5严格识别，不改变旧执行／cache／STOP／排空代码。
has_path接纳OBS3/4/5，hist仅4/5。v4 INFO必须低格式1、高16位0；v5必须低格式2、
高8位0、bits55:48=64，任何overflow仍失效。v4原日志逐字节保持，v5打印depth64，
16个stage行仍schema1，因为桶布局算法未改。A1独占共享`hist_records.py`扩展，本批不改它。

## 实际验证

- [344项实际helper回执](test-results/workload-helpers01-manifest.json)：ASan/UBSan通过，
  241合法／103非法；含49份原规划、A完成prefix边界（测试输入，不是实际A测量）、
  partial tile、0／一环／环边／2^32／U64_MAX前缀。每个合法case逐bit对独立Python模型验证两张bitmap及union，
  并验证页集合、row/column/ring边与最大descriptor索引。
- caps/run布局和全部关键offset在host实际运行、RV64实际交叉编译断言通过；仍64B／544B。
- [119项实际OBS函数](test-results/workload-observer01-summary.json)由生产`idma.c`直接抽取，
  模拟MMIO并用ASan/UBSan执行。v5的错误版本／深度／保留位／overflow／计数／run错误均拒绝，完整512桶仍读取保留。
- [v4字节对照](test-results/workload-observer01-log-contract.json)与A1已审run02的有效v4原输出完全相同；
  [v5实际mock输出](test-results/workload-observer01-valid-v5.log)已交A1只读兼容核查。
- [真正RV64 idma.o](test-results/workload-linux-obj01-manifest.json)编译通过，100144B，
  SHA256 `07d97dafaf258ee4493a4353914bff20480b97e1813d0321d8f47f002cb92068`。
  使用独立O目录，实际source三文件SHA与native测试相同；没有Image/rootfs/payload构建。

所有native结果为`fixture_only`／有限函数证据，kernel bitmap原语由host bitops替代；
geometry/offset/prefix生产函数逐字节不改。它们不是cache／DMA／IOMMU／AXI或板测证据。
完整raw bitmap输出留在`artifacts/workload-helpers01/native-run.log`并记录SHA，不发布成论文实测数据。

## Root可复跑入口

从A2顶层工作树运行，使用Git保存的最小实际source集合：

```sh
python3 paper/ispass2027/exp/E3-workloads/verify_workload_helpers.py \
  --linux paper/ispass2027/exp/E3-workloads/source/workload-initial/linux \
  --output paper/ispass2027/exp/E3-workloads/artifacts/workload-helpers-ROOT01
python3 paper/ispass2027/exp/E3-workloads/observer-validation/run_driver.py \
  --linux paper/ispass2027/exp/E3-workloads/source/workload-initial/linux \
  --out paper/ispass2027/exp/E3-workloads/artifacts/workload-observer-ROOT01
```

每个output必须新建，不覆盖原执行。最小snapshot用于native复跑，不是完整kernel树；
实际RV64对象需在恢复至682a2ca的完整独立Linux树使用`build_workload_driver_object.py`，
输入本次正式配置、独立O目录，不能在对象编译时更改源。

下一小批才接ABI5 adapter与`idma_run_experiment_mapped`现有生命周期：
新context保存几何、实际warmup／测量prefix，旧调用传NULL；新bitmap验证全源、目的触及／空洞，旧prefix分支不改。
保留原cache交接、全局锁、HPM、观察开关窗、STOP／drain、retain/quarantine与cleanup；
真实A完成row及partial tile单列，不补齐虚构tile。实际生产diff与旧路径回归完成后再交Root审查，不提前编新软件。
