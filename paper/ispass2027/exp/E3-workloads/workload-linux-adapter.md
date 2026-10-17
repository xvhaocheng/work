# ABI5 Linux第二小批：共用pair生命周期适配

实际Linux组件提交`46df97c9820d0d63878215cd553cfee1c1b6b760`，直接parent为已审`682a2caf0a7fdf69749ebdac146df60df55dcbe6`。
本批只改`drivers/misc/idma.c`；正式UAPI、私有纯几何助手与OBS5日志合同不变。
驱动SHA256为`667583750f58863c04921719819e2563325729dbc3d76f000a79cc6a7dcc0834`。
[组件记录](source/linux-workload-adapter.json)、[完整commit patch](source/linux-workload-adapter.patch)、
[原始diff](source/workload-adapter-review.patch)、[增量bundle](source/linux-workload-adapter.bundle)齐备。
Bundle前提682a2ca，实际verify通过；Linux只有上游remote，未推组件上游或创建remote。

## 生产代码与边界

新CAPS/RUN仍为ABI5、0x0f/0x10，caps64B/run544B。RUN严格拒绝未知supply、flags/reserved、
非法geometry、非V0/DID10或A1/DID11、超限/溢出的operations×rows；V≤262144 row COPY，
warmup≤16384 row COPY，timeout≤60000ms。Solo的A geometry必须全零。

新handler先把用户请求复制到8800B heap context，输出区清零，再转换到内部ABI1-shaped请求。
`idma_run_experiment_mapped`仅增加可选workload指针；旧ABI1/P0及pages调用传NULL，multi流保持原实现。
分配、4KiB映射、cache交接、全局实验锁、HPM、开关窗、STOP/drain、retain/quarantine和cleanup继续调用原函数。
不更改CMA、MAX_ORDER、分配模型或现有错误检查。bitmap在heap中，未进入计时窗口。

新路径要求实际BTC3（V及A各自核查）和OBS5/Depth64；观察reset在原全局锁内进行，
随后严格读取INFO=`0x0040000020202002`。CAPS仅只读magic，不改INDEX/CONTROL；RUN才做锁内INFO检查。
BTC3配置写b0/b8/c0/c8/d0五个几何寄存器，每lane `NUM_BYTES`为自己的row_bytes。
软件warmup同样按实际几何offset提交，完成后排空；硬件测量从各环0重新开始。

预热计数只在CPU COPY确实完成时增加。STOP/drain后按实际硬件completed生成测量prefix bitmap，
暖机与测量集合分别保留。A的startup和final drain均属于其实际完成计数，不按V数量或整tile补齐。
A还需在STOP前保持active、无error且issued未耗尽。CPU验证检查完整source和完整destination：
触及的64B granule应与source一致，未触及的padding/holes仍应保持初始化反码。
输出分别给出实际warmup rows/bytes、测量distinct bytes/pages、warmup pages和已验证union bytes。
只有该lane完整数据验证通过才报告verified_union_bytes；错误、unknown quiet仍走原retain/quarantine规则。

BTC3同时接受旧入口。旧CAPS_P0的batch_magic继续表示ABI2兼容合同BTC2，保持旧CLI协议；
它不是BTC3硬件身份读回。新ABI5 CAPS明确报告BTC3/OBS5，正式实验身份仍由BIT manifest绑定。
本批不让旧P0 collector跨BIT合池，也不修改Root正在执行的45点协议或软件archive。

## 实际验证

[资格汇总](test-results/workload-adapter-qualification04.json)将两类证据分开列明：

- [最终actual-functions有限回归](test-results/workload-driver04-validation.json)使用原生产函数逐字抽取，
  ASan/UBSan、-Wall/-Wextra/-Werror通过。旧38 pair/P0、57 multi、97 pages在682 gold与candidate
  的有序寄存器摘要均为`9128914394721286305:498206`。
- 新416项包括18个Solo、324个六size×三V/A形态交叉、1536槽位、warmup大于测量的非连续union、
  A停止于32/64行partial tile、独立64B至64KiB各lane真实len、padding/tile gap/未触及source损坏拒绝、
  alloc/map/HPM/ARM、STOP后control仍active、drain/零ID retain、A提前停止或耗尽、错误版本/INFO/reserved/overflow，
  以及BTC3下旧P0、ABI1 batch/nonbatch、multi和pages入口可用。
- bitmap期望由测试侧独立逐字节访问集合计算，不调用生产offset或prefix helper。
  [完整原始输出](test-results/workload-driver04-workload-run.log)和[实际编译单元](source/workload-adapter/host04/workload.c)保留。
- [真正RV64 idma.o](test-results/workload-adapter-obj01-manifest.json)通过，113520B，
  SHA256 `9d42fc024c57d9fd7fbc91bf46ead56cb65dc7a6d1c9e88db432f60b3f6f38c0`。
  实际编译源SHA与有限测试相同，独立O目录，配置未改变；没有编Image/rootfs/payload。
  [反汇编prologue](test-results/workload-adapter-obj01-frames.log)显示mapped core帧992B、新adapter帧400B，
  这只说明这两个函数的局部帧，不冒称总调用栈上界。

原01失败为harness累计trace超过2M断言；02在401项时passed；03失败为新增STOP fault注入早于STOP，
修正test-only触发时点后04通过。原failed回执/源均保留。所有尝试的生产驱动字节相同，没有为通过测试修改生产源码。
这些是host actual-function＋mock MMIO/mapping/cache/HPM证据，不是Linux原生运行、cache一致性、AXI或FPGA实测。
实际BIT布局路由仍在独立full-E3硬件构建中，当前没有可上板的ABI5共同payload。

## 复跑入口

Root从顶层任务工作树可直接使用冻结source和gold，无需改当前内核worktree：

```sh
python3 paper/ispass2027/exp/E3-workloads/verify_workload_driver.py \
  --linux paper/ispass2027/exp/E3-workloads/source/workload-adapter/linux \
  --baseline-driver paper/ispass2027/exp/E3-workloads/source/workload-adapter/baseline-idma.c \
  --output paper/ispass2027/exp/E3-workloads/artifacts/workload-driver-ROOT01
```

output必须新建。[实际可移植复跑05](test-results/workload-driver-replay05-validation.json)已用上面的冻结source/gold入口通过416＋38/57/97，源SHA不变。
当前runner补充只读gold文件入口、fixture_only标识和结构化case数；
04实际执行版另存`source/workload-adapter/host04/verify_workload_driver.as-executed.py`，原回执未改。
对象复跑需完整Linux恢复至46df97c，使用`build_workload_adapter_object.py`，输入正式kernel配置与新O目录。
实际对象路径为`artifacts/workload-adapter-obj01/kernel/drivers/misc/idma.o`，二进制仅本机保存。

下一批为静态CLI和collector：默认16MiB V有效字节＋V一完整几何环预热，按实际A prefix和实际burst数解析，
复用已审OBS5 histogram helper。完整软件构建等Root验收本批及CLI冻结后独立进行；P045不变。
