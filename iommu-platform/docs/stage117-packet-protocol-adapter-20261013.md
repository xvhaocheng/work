# stage117逐包协议模型验收（2026-10-13）

原公开接口缺少peer仍活跃时的完整包交接、前缀发布、消费及归还合同。本轮按用户选择先实现协议模型，受控原始工作负载的功能验收通过。缓存后端为`MODEL_UNCACHED_ASSUMPTION`，没有实际CVA6 CPU/cache模型，因此不能据此声称缓存安全、应用加速或独立创新；论文未完成。

进入条件是已有具名软件适配范围、原来源和冻结任务，且原硬件支持精确span和逐engine公开NEXT/DONE。结束条件是实际实现、定向故障测试、两个新案例、独立原始证据核验及状态/计划/交接/验证完整交付。本轮不改生产driver、ABI或RTL，不上板，不恢复关闭分支，不重跑旧实验。

原三帧60/65/1514B、1639B载荷、1675B记录及四span（60/65/503/1011B）保持不变。完成收据绑定engine、span、packet、cookie、ID和世代；包3必须两段完整退休。公开DONE匹配后先启动已准备的合法后继，再处理可发布前缀。交接凭据绑定不可变16B目的容量闭包，每次记录和局部guard读取都检查范围。先校验完整记录，再发布原前缀；源A退休后覆盖复用，实际consumer通过原B cookie读完整链，消费后才归还B。全池与表快照仅在最终已验证quiet读取。

root直接检查协议、控制器、测试和独立检查器代码，发现最初凭据未强制地址闭包，要求在首次RTL构建前补齐逐次读取范围检查及peer/全池/溢出拒绝。修订已纳入最终编译输入。

17项协议测试通过，覆盖部分包、前缀、陈旧/重复收据与凭据、提前回收、未知接受、超时和范围。一次RTL构建及single/parallel两个新案例各一次通过，同一二进制SHA256为`1343401f25c0d66cf25fa0f20b2874700067319b0a6e35fd52835c3c38b31b8f`。独立检查不信执行器PASS标志，从原JSON、公开MMIO、R/W/LAST/B、实际host访问及后端transport重建；14项篡改拒绝和1项执行器标志独立性正控制通过。检查器有离线修订/重复读取，完整次数见账本，未额外运行RTL。

| 双DMA功能见证 | consumer事件位置 | peer实际后端活跃区间 |
|---|---:|---:|
| 包1完整消费 | 853 | 包3 span2：[92,1414) |
| 包2完整消费 | 1458 | 包3 span3：[1422,1619) |

两次消费前本包R/W/B均已退休。这些位置只证明因果顺序；host校验/消费不推进RTL时钟，不能当作真实应用周期。旧single1161/parallel1601有限G0仍关闭，本轮不产生新性能比较或440周期归因。

每案AR/AW/B各6，读总线1672B，有效W字节1639B且恰一次；翻译请求/响应12，lookup12/hit11，PTW_AR2，DDTW_AR分别1/2。每案39项host操作：预先写header36B；发布前读记录1675B和guard133B；consumer另读1675B和guard133B；覆盖源1639B；最终全池读16384B、表读12288B。初始化28672B计入记录。双DMA活动资源2对单DMA1；每DMA 4KiB缓存，FIFO8、AXI在途8，DEFAULT每engine一个公开job，驻留共享2MiB Sv39映射、16KiB逻辑池及12KiB表。全部具体参数、MMIO及源持有记录见成本文件。

RTL构建宿主耗时6.096376秒，两个案例进程分别0.034319/0.036391秒，只是执行成本。单元首次编译的两项缩进警告修正后通过；包装依赖时间字段解析失败也保留。无RTL重试。软件、缓存、应用RTL时延和host scratch峰值仍为null；metadata仅host heap，真实PA/IOVA alias未绑定，逻辑隔离不证明进程退出后的生产物理隔离。

主会话实际Sol/ultra，实现席Astra/ultra，独立检查席Sol/ultra；Codex-only，原生上下文，无额外token预算。模型与执行证据完整保留，没有以配置覆盖冒充实际窗口。

下一关口：在原任务和布局下做最小真实CVA6缓存交接见证，核物理alias、metadata归属、完成收据、完整包读取与cache清空/重填成本。实际接口功能通过后，再冻结来源支持的交付目标，纳入CPU、单DMA、普通多DMA及合法驻留/大页/批量，对照一次G0。不能用更多本模型轮数替代真实缓存验证。

[稳定接受](../out/stage117-packet-protocol-adapter-20261013/root-result-acceptance.json)／[证据导航](../out/stage117-packet-protocol-adapter-20261013/README.md)／[成本与参数](../out/stage117-packet-protocol-adapter-20261013/implementation/cost-and-parameters.json)／[完整执行账本](../out/stage117-packet-protocol-adapter-20261013/implementation/complete-execution-ledger.json)／[最终交付核验](../out/stage117-packet-protocol-adapter-20261013/root/delivery-verification.json)。
