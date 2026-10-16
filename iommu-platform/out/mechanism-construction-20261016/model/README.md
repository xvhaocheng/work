# 冻结条件离散事件模型：执行与边界

2026-10-16；Codex-only。请求 gpt-6.1-sol/medium；实际独立 model/effort metadata **unknown**。本目录只实现并执行新的独立 Python 模型，不是 MIG、Verilator、RTL、FPGA 或原生应用实验。原生可达窗口、物理面积、Fmax、CPU/集成费用、真实 split 费用均未知。

合同 SHA256：`8b59b91b9ab73e0480400ed25a196c55b300e18aa893e30ccc339591a6bd7541`。完整冻结合同、五组输入和14个固定程序在 [frozen-inputs.json](frozen-inputs.json)。代码先保存并通过语法检查，输入在模拟前落盘；首次完整主批 **1 次，70条运行（5×14），82项检查通过，0失败、0修复、0参数搜索**。实际命令与产物hash见 [execution-receipt.json](execution-receipt.json)。不为复核重跑主批；独立重消费现有输出可用 `python3 model/runner.py --verify-only`，该操作不模拟。

## 实际运行结果

时间为 suffix 整数边界；共同前缀显式有768单位串行服务，full-start时间须加768。完成按各job两个B都退休计，quiet按所有R/W/B和ticket义务排空计。

| 冻结case | O_CURRENT quiet | M_TICKET quiet | 结果 |
|---|---:|---:|---|
| positive_cold | 1298 | 1292 | 双方完成与quiet均提前6；A最后源R晚3 |
| paired_hot | 1282 | 1282 | 固定guard2的quiet=1284，晚2 |
| adjacent_short_gap | 1291 | 1292 | 候选慢1，保留失败例 |
| holdout_longer_gap | 1302 | 1293 | 候选快9 |
| holdout_late_intent | 1298 | 1298 | 标记missed_window，原先L不撤销；服务/offer事件同current |

`O_SAME_TICKET` 与 `M_TICKET` 在全部五组输入上 **逐事件完全一致**；不是独立调度算法的证据。所有固定guard 0/1/2/3/4/8/16均在所有case用同一程序执行，没有逐case挑选guard拼合。

强普通对照保留原4096B parent，只将最后L拆为2×128 read；write仍四个256beat burst。cold quiet依次为 **1290/1291/1292/1294**（a_split=0/1/2/4），hot为 **1282/1283/1284/1286**。因此零额外暴露开销时，split cold比候选快2，hot追平，额外1个AR；a_split=2只在cold追平候选。`a_split`是事前假设的新增串行暴露开销，**不是已测硬件费用，也不是额外AR天然要付的常数**。来源支持legalizer适配可能，现成生产接口缺read-only128选项不构成候选胜利。

## 可审实现与独立检查

[runner.py](runner.py)把 AR admission 和 data service 分开；单boundary最多一个AR，accepted reads严格FIFO，R/W/setup共用一个串行非抢占资源，所有service是`[start,end)`。边界先退休，后选择；不让正在服务的read被抢占。P2/P3依前PTE返回加processing gap出生。write顺序固定V0/A0/V1/A1，write translation/source/owner条件成立才发布AW；每次W有独立B延迟并可与后续数据服务重叠。

完整工作不是免费suffix：两个4096B COPY各有512 R/512 W；共同前缀的三次256beat读在时间-768到0显式记录。cold总AR7、AW4、B4、payload R1024、PTE R3、W1024；hot AR4、PTE R0；split只增加AR1。所有原始parent字节区间、permission、属性与OKAY错误状态保留；原buffer共8192B，无新增payload内存、无物理保留改善主张。

[verification.json](verification.json)由另一路径重消费已序列化的 [results.json](results.json) 和 [traces.json](traces.json)，不调用scheduler做自证。检查守恒、parent连续覆盖、burst上限、PTE/translation/owner依赖、服务不重叠、整数边界、AR与offer字段、accepted FIFO、response关联、全部B延迟与最终drain；对照合同事前expected字段及split预期。另有6项有意义的故意破坏：删除最终B、制造资源重叠、PTE过早、破坏owner顺序、丢有效字节、用错误PTE释放ticket，全部被checker检出。聚合CSV见 [traces.csv](traces.csv)，不为每个变体生成大量文件。

checker不重新证明native source reachability或全系统不可区分历史。固定模型按输入承诺共同前缀和t=0无public WVALID；private cold/intent只供其声明的接口使用。当前有限case没有下游背压，因此first offer同boundary被接受，没有held stall；lateintent测试的是已经接受的L不可撤销。timer和missed分支在代码内，**缺目标/超时、真正held背压、reset/epoch、多walk公平逃逸与错误注入均未作为已执行协议测试宣称通过**。本例最多3个DS，F4第四次逃逸不触发；不建立一般liveness证明。

## 未执行普通构造与结论边界

`O_PREP_EARLY` 和 `O_SEGMENT` 的2053 full-start时间只保留为合同给定的**手工条件乐观服务构造**，没有在本模型运行；没有stock deburst sweep或任意分段/prepare执行成功声明。其接口、CPU、descriptor、area、lookup及B/owner成本须在实际程序中另核。

这个有限结果说明：支付early-intent/首PTE标识可避免本例current-VALID信息窗口的完成损失，固定等待在paired hot需付代价；同接口普通if/else完全等价。零暴露开销的普通split反例已经改善相同cold/hot服务向量，不能认定early接口为必要、独立新算法或普遍净收益。只有当split/reassembly/额外AR实际费用或资源目标有据可查时，才可能形成有意义的接口/组织成本差异；本模型没有证明该原生成本域。
