# Codex：下一项可被推翻的机制假设

当前 §70–74没有证实独立性能创新。因为普通broadcast/read combining/动态请求窗口/独立调度可以解释已测大部分收益，不能把threshold、额外等待或AR提前12–14cycles本身作为新算法。当前有价值的约束是：独立context各自真实授权、每个logical parent独立LAST、已经firstVALID的AR/R/AW/W不可撤、B债仍在、小buffer、单个global R/W通道。这些约束的组合是候选研究问题，不是仅靠新名称就成立的贡献。

## 假设：允许 A 在旧 V physical burst 退休前加入

现规则总补读A[0,E)，E是旧V已接受physical READ端点；之后共享[E,N)。如果A实际能在旧V仍busy时追到J<E，且双方raw W数据已消费至J、无held logical R/skid、两方自身授权仍有效且PA/normalattrs相等，则只给未来尚未交付的旧V physical R增加A recipient，不改变旧physicalAR/header/RLAST，也不改变任何已首VALIDlogicalR。已冻结skid成员不修改。此后旧V[J,E)同时供两方，E处再发共同[E,N)。

在没有额外重读时总sourceR=N+J，而现规则=N+E，理论节约E−J拍。这只是工作量推导，不是DUT结果。必须真实观察“oldVbusy=1且rV=rA=rawWV=rawWA=J<E”及其后真实fanout，不能用提前metadata或AR计数代替。

## 两个必须否决的捷径

1. 为等A追上而直接把旧V RREADY关死：AXI允许内存先保持V下一拍RVALID，这会挡A prefix/PTW R，原global R流不允许撤这个VALID；小FIFO不能无限接收长尾。不能假定slave总会绕过heldV响应。设计必须保持不依赖A的安全继续/退出，超时不撤既有R/W/B。
2. 只按地址相同合并：A必须自身READ允许且目的WRITE许可/容量够，跨DID的generation数不能互相当授权；完成V的记录释放不能堵住独立A。fault/remap需新的协议，不由今晚成功fixture证明。

## 最小下一实验

先建立可产生机会的对照夹具，再记录上述J<E状态。**不能直接在当前E端点补读上加计数后用0否定候选：当前A已接受的prefix固定到E，在rA=J<E时其physical child仍busy，本来就不满足安全加入条件。** 下一夹具先固定发A[0,J)（J>0且J<E），保持header不可撤；若追不上，正常独立补读[J,E)并按原E规则继续。V不停等A，不修改已有R/W/B。需另构建/验证这个交易切分；它本身是强普通对照，不算已实现的加入算法。当前uniform B模型下V通常先行，不能凭空声称有catch-up窗口。用真实可解释的双设备速度差（各自目标W/B服务或不同原生页界），报告为因果实验；不假装应用trace。只有出现窗口才实现未来physicalR成员绑定；否则重定设计。

强对照必须包含：相同独立成功授权、相同privatebuffer与总metadata成本、相同信息的普通pending-read合并（包括late receiver）、短READ共同切分、普通full independent与固定broadcast。如果只是普通late merge能逐项重现，就不能称独立创新；如果新机制以小buffer维持不可撤合同、减少E−J真实重读并保护设备进度，而最强前作/对照缺少这个能力，才可以把“安全在途加入的机制与边界”写作独立贡献。下一门槛不是许诺二区，而是实际见证、最强前作比对和可复查的增量。

最接近前作：Intel US20200402198A1 pending scoreboard/dynamic window/multicast return；DMA-Latte v3 source-read broadcast/parallel策略；Torrent grant/data/finish；AXI multicast crossbar的AW/W/B drain屏障。检索未发现完整上述长burst/独立授权/旧数据债组合，不等于证明世界上没有；读过的具体body才是目前边界。
