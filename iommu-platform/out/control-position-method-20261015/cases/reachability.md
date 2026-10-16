# A：有来源的有限任务与合法选择（2026-10-15）

## 结论与证据等级

存在一个**源码允许、需要软件适配的源端选择**：HERO `mm-large` 中，A0 的异步调用已返回以后，thread 0 的 A1 未来预取与 thread 1 的 B00 当前输入之间没有程序依赖。把 B00 的完整配置/启动事务先发布，是合法跨线程交错。首块计算需要 A0+B00，而不需要 A1。因此“在发布前选择当前用途输入”有可检查的任务意义；是否缩短完整任务必须连同后续计算、重复 B、C 写回、等待、释放和消费一起执行验证，不能由这个局部事实推出。

本交付只给来源合同，不运行模型、RTL、构建或板操作。这里没有证明当前 CVA6 RTL 在 DDR 选择点出现两头竞争，也没有恢复 ACV-2x2、MIG 暂停 goal 或旧关闭实验。后者 28 个 offer 零竞争的结论保持。

- **源码事实**：固定 HERO 提交 `c8481a930a15a385d4f58f519bed83d2e75e2a73` 的现有本地证据；明确的调用、barrier、缓冲与输入公式。
- **来源支持的抽象合同**：给两个尚未发布的调用加一个有界统一软件队列，以原子配置/启动事务执行不同合法交错。该队列、任务标签、仲裁与核间同步需要收费；不是原程序已有接口。
- **可条件执行**：有限 source-contract 分析器可使用 root 事前冻结、明确未校准的服务与计算代价，输出合同层预测/反例。
- **未证 native 可达**：HERO SPM 与当前 CVA6 双端 IOVA 搬运不同；已有源代码不提供实际平台双头共现、翻译延迟、MIG 服务、W 进度或端到端软件成本。

HERO 的 active `main` 是 367–379 行的 **copy-based OpenMP target**。382–404 行的 SVM 分支在 `if(0)` 内，且原注释明确说尚不能正确工作。因而本源码不证明真实 HERO 已在共享 IOMMU 上执行该任务；把它放到共享 IOMMU 是本轮显式适配与待验接口。

## 原文入口

源码根是 `iommu-platform/out/workflow-stream-contract-20261012/source-evidence/hero/`。

| 原文 | 位置 | 可直接支持的事实 |
|---|---:|---|
| `openmp-examples/mm-large/mm-large.c` | 44–70 | n_stripes、stripe_size、六个 A/B/C 双缓冲与三个双 job 数组 |
| 同上 | 93–138 | thread0 A0→A1→wait A0；A1 是下一 stripe，地址已知 |
| 同上 | 103–107、164–205 | thread1 B00→B01→wait B00；后续 B10/B11 重复相同两条 B |
| 同上 | 143–161 | s=1 由 thread2 发布 C0；大于两个 stripe 时才有循环内旧 C 等待 |
| 同上 | 208–228 | 显式进入 barrier，计算 A 行×B 行，omp for 结束隐含 barrier |
| 同上 | 238–275 | 最后先 wait C0，再同步 C1 写，parallel join，释放六槽，返回 |
| 同上 | 284–305 | height=32 是正常接受的最小 main 输入，导出 width=32、stripe_height=16、n=2 |
| 同上 | 320–345、369–379、406–410 | 初始化、host reference、copy-based map、host compare 与 host free |
| `support/libhero-target/inc/hero-target.h` | 83–104 | async 可因队满阻塞；返回时保证已启动；每个已创建 job 必须恰一次 wait |
| `support/libhero-target/pulp/hero-target.c` | 58–85 | 5 次地址/长度配置写，tf_id 读触发启动并取得真实 job ID |
| 同上 | 129–137 | 同步 dev2host 是 async+wait；wait 按 stream/id 等完成 |
| 当前平台 `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv` | 490–559 | NEXT_ID→COUNT 首次 VALID→WAIT 完成；COUNT 中 VALID/地址不得撤回 |
| 当前平台 `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv` | 622–727 | AW 优先以打开 W 排空；翻译响应后仍要完成口接受才释放控制器 |
| 当前平台 `cva6/vendor/pulp-platform/axi/src/axi_mux.sv` | 333–363 | AR 仲裁 LockIn 与 spill；更迟策略不能撤销已输出的 offer |
| `docs/stage117-real-cache-handoff-20261013.md` | 7–9、25–29 | 已验证 WT 缓存交接需要真实失效确认、B 退休、consumer 与释放；不能把 DMA 完成等同于 CPU 已消费 |

最后一项只证明另一已有适配的实际边界，不把其逐包 consumer 插入本 HERO 程序。HERO 原程序的 host compare 在整个 target 返回及 map(from:c) 后。

## 完整 height32 任务

固定 width=height=32、stripe_height=16、uint32 模 2^32 乘加。A[i,j]=32i+j，B[i,j]=2·[i=j]，预期 C[i,j]=2A[i,j]。每条 stripe 2048B；六 L1 槽各 2048B，总 12288B；host A/B/C/D 各 4096B。四块 K00/K01/K10/K11 每块 16×16×32=8192 MAC，总 32768 MAC。

| DMA | 类型 | 源→目的 | 发布程序前缀 | 消费/等待 |
|---|---|---|---|---|
| A0 | host2dev | A[0:2048]→a0 | 分配成功 | A1 发布调用返回后 wait A0；K00/K01 使用 |
| A1 | host2dev | A[2048:4096]→a1 | A0 发布调用返回 | s=1 wait A1；K10/K11 使用 |
| B00 | host2dev | B[0:2048]→b0 | 分配成功 | B01 发布调用返回后 wait B00；K00 使用 |
| B01 | host2dev | B[2048:4096]→b1 | B00 发布调用返回 | B10 发布调用返回后 wait B01；K01 使用 |
| B10 | host2dev | B[0:2048]→b0 | K00 结束 barrier 后，b0 不再被 K00 读 | B11 发布调用返回后 wait B10；K10 使用 |
| B11 | host2dev | B[2048:4096]→b1 | K01 结束 barrier 后，b1 不再被 K01 读 | K10 结束后 wait B11；K11 使用 |
| C0 | dev2host | c0→C[0:2048] | K01 完成，s=1 thread2；该调用返回是 K10 进入 barrier 的必要前缀 | K11 后 wait C0 |
| C1 | dev2host | c1→C[2048:4096] | K11 完成且 wait C0 返回 | 同步 helper wait C1；再 join/free |

原图 8 笔×2048B=16384B 逻辑 DMA 搬运，含读写方向；不能把这个数字当 CVA6 总线字节或 PTE 字节。每笔外部/内部两端流量与总线 padding 另计。原图四个 compute 的顺序固定 K00→K01→K10→K11。**不能只保留数据依赖**：K00 之前 A1/B01 发布调用都要返回；K10 之前 C0/B11 发布调用都要返回。完整事件 DAG 位于同名 JSON，包含 wait、join、free、map/visibility 与 host consume。

## 适配接口与强普通对照

最小新适配是容量为 2 的**待发布描述符**队列，不是 64B launch permit。每项至少保存 src/dst/length、owner/thread、role、任务依赖标记、状态及返回 job ID。源/目的数据仍使用原六槽，不增加 payload 缓冲。提交函数把完整配置原子交给单一 publisher；publisher 执行该后端真实配置+启动操作，取得真实 job ID 后才允许 async 调用返回。不能在只入软件队列时伪称原 async 已启动。等待在队列锁外执行，不能跨 barrier 持锁；所有合法后继必须最终发布。

HERO C helper 的 5 配置写+1 启动读只是该后端事实，不等于 CVA6 的寄存器次数/时延。硬件配置是否按 thread/stream 分银行未在本次核实，不能假定原来的多次 volatile 写天然原子；新 publisher 必须明确串行化。若换到 CVA6，逐 lane 配置、frontend busy、普通完成与 job 所有权都要适配，不能仅替换地址常量。

公平对照都获得相同两个描述符槽、相同完整任务信息、相同六 payload 槽、相同启动/等待 API 和计费；允许采用合法“先当前用途 B00”的普通规则。root 新方法若推导到同一选择，只能证明控制位置/任务预测成立，不能据此声称该调度规则新颖或独有收益。

**B 保留是必须可执行的普通变换**：n=2 时 b0/b1 已经容纳全部只读 B。host 在 target 内没有 B 写入，也没有第三 B stripe；原 B10/B11 分别覆盖相同值。保留两个 B buffer 到 K10/K11 结束可以删除 B10/B11 两次搬运，不增加 payload 容量。需删除对应未创建 job 的 wait，并把第二 outer stripe 的读取改为“第一次 B 完成且槽仍由本任务持有”检查；第一次 B00/B01 的真实 wait 各恰一次保留。不是预填、不是免费 cache hit，也不是把不存在的 job 标为完成。代价至少包括该源码变换/调度路径、两个槽的延长持有、两个有效/所有权状态与检查；原 b_dma 数组可继续保存 job ID，不能强制另加不必要资源。实际 CPU 指令与调度时间待测。变换后 6 笔×2048B=12288B，少两笔发布和真实等待，四个 compute/两个输出/最终消费不变。

## 四案的来源合同（事前固定，不执行）

四案共享上述完整输入、同一共同 t0、同一服务/计算成本合同；都从 t0 走完初始发布前缀、C1 等待、join/free、map/visibility 与 1024 元素 host compare。前缀是完整执行产生的状态，不从中间免费开始，也不通过调整相位/压力/延迟寻找胜例。

1. **P：未发布选择。** A0 发布调用已返回；A1 与 B00 均满足各自程序前缀、源/目的所有权和队列资格，尚未首次 offer。存在 A0→A1→B00 与 A0→B00→A1 两种合法跨线程发布交错。选择证书是 A1/B00 无依赖关系、目的 a1/b0 非别名、首个 compute 只依赖 A0/B00。事前预测：B00 先使首个 compute 的数据路径更早就绪；是否改善完整任务由完整同服务执行判别。失败：任一待选描述符实际未 ready/槽忙/已承诺，或完整尾部抵消早期改善时不能继续宣称 task 收益。
2. **N：相邻首次 offer 边界。** 同一任务的合法 thread0 先 A0/A1 发布前缀已经发生；A1 在待考察合并输出首次 VALID 已出现。B00 信息/请求此时来到较迟控制点。保持 A1 完整 packet 至接受；不能把 B00 插到这个已承诺 offer 前。事前预测：若只给该晚位置权限，原 A1 前缀不可撤销；若允许回到发布前选择，必须另计那是 P 的接口，不能把事后重排算晚控制能力。保留真实前缀成本，不加“让 B00 刚好晚到”的延迟。
3. **H1：已有 B 所有权保留。** 同样完整任务，允许上段明确的 B-retain 普通程序；初始 B00/B01 正常发布并真实等待，两个 B 槽不交给别的 owner。后续 B10/B11 不再是可选择工作，算法应从剩余任务中删除其翻译/搬运/等待义务。事前预测：若新方法把它们的等待当成可避免翻译损失或优先级机会，即错误；收益比较必须对这个同信息、同六槽的普通方案闭合。此案不免费改变缓存状态。
4. **H2：consumer/释放闭合。** 同一原图的最后自然位置 K11 已完成，但 C0 可能已接受而未完成。thread2 的原 wait C0 尚未返回，故 C1 未 ready。任何已接受 AW/W/B 义务继续排空；K11 完成不能标任务完成；不能提前发布 C1、free c0/c1、返回或让 host compare。事前预测：完整结束必须沿 C0_done→wait_C0→publish_C1→C1_done→wait_C1→join→free→map/visibility→host compare。没有新鲜可换选择时应报告义务/供给边界，不捏造仲裁机会。是否真的存在该未退休状态是具体服务合同执行结果；若 C0 已完，这个子条件记未覆盖，不改延迟制造它。

H1/H2 为结构留出：不使用其结果修改 P 的规则。H2 的“未完成 C0”是待覆盖条件，不能被预设为实际 RTL 事实；闭合次序无论是否覆盖都成立。

## 当前真正缺少什么

source-contract 层已有完整数据、DAG、合法交错与所有权证书，可以在 root 冻结成本及范围后一次有限验证。**本文件没有冻结数值性能结论**。任务长度 2048B 与真实六槽来自源码；服务 1 beat/tick、compute 1 MAC/tick 等如被采用，是未校准合同假设。

若要推广到当前 CVA6 native，需要另证：两个 eligible pending 调用在哪个真实接口同现；每个 lane 的 ordinary 发布/忙状态与同预算映射；源/目的都经 IOMMU 时的 AW/AR/W/R/B 完整义务与有限容量；真实 PTE/TC/PTW 路径、实际结果保存/所有权；CVA6 CPU 或 Linux 的供给、缓存交接、应用消费及所有部署成本。现有 4KiB fullbuffer 证书不能自动授权 2048B 的所有者绕行，原 64B controlled launch 也不能泛化为普通原生 permit。没有这些证据，不把有限 source-contract 模型写为 native RTL/MIG 或论文性能结果。

所有变换/队列/普通强对照本身是否构成新意，由 root 按准确前作和实质新增另行判断。本任务只提交可达与完成边界，不扩张 novelty 主张。
