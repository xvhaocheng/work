# V1局部接口与状态合同：源页所有权、目标映射和一次使用分开

这是源码绑定的设计推导，不是C实现、形式化验证、内核测试或性能模型。适用官方v6.12普通page-backed mlx5e native XDP单页路径；source/target关闭、DMA backend实际保证仍按下列条件处理。

## 授权、粒度和引用

首版使用CPU基础页，不要求物理PMD。实际IOMMU granule必须能被源owner合法覆盖；bounce、权限、DMA mask/ops、设备配置均须相容。持续B映射只有在T允许B按规定权限访问该source generation期间全部映射字节和后续用途时才合法；不从首次route推导这种授权。S逐对象撤权分支不跨越对象边界保留图。

三个寿命分开：

- 源page_pool和正常frame／consumer链持有物理页。不因B空闲mapping cache额外get_page或增加pp_ref。
- binding持有元数据、合法DMA mapping和target配置寿命；它不是物理page的独立长期owner。
- 每次准备／在途使用持binding token，并同时保留现有frame的物理存储责任。token只防binding被cache淘汰／关闭，不替代真实DMA完成。

key至少区分source owner cookie、source generation、page/granule、target DMA配置代际、方向/权限/attrs。PFN或pool指针单独不够。源内cache recycle和新packet不换代；源所有权真正结束后重建才换代。

## 拟议接口及顺序

1. **注册owner和查关联。** 源加入pool时建立owner identity，并使其寿命覆盖到page_pool_return_page的release计账/clear_pp_info/put_page之前。B map前原frame仍有效，normal frame引用保护source。只在owner OWNED且target OPEN时取得LIVE binding token；缺项首建图可用预分配record和现有DMA API。并发CREATING不睡等，允许退回普通短map。绑定容量不足也是付费fallback，不能丢有效工作。
2. **首建图。** 元数据record先预留再调用DMA API；map成功后复核owner/target状态，才能发布LIVE。失败或关闭竞争要清理已做工作，不把费用抹掉。这里未实现分配/锁或设备适配，不承诺任何原子上下文下的backend时延。
3. **提交前同步。** CPU改包头和A的DMA结束后，按真实coherency/方向完成每次B sync及发布。持久map不免除sync。SQ接受仍在后面，不能预知。
4. **实际接受。** 新持久mode保存原xdpf和binding handle；原frame存储责任继续保留至B CQ。单页stock FIFO当前为mode、xdpf、dma_addr共3个逻辑entry，候选用handle替换地址entry，仍3个逻辑entry。最终union大小/ABI/缓存行成本未编译验证；binding表和预留空间另外计费。
5. **失败与完成。** map/SQ失败先释放本次token和私有资源，原frame交devmap未接受后缀归还。正常CQ完成先释放binding token，再xdp_return_frame_bulk；不能反过来。短map mode保持原unmap责任。目标close的残余descriptor回收必须建立在真实driver/DMA drain合同上，不以软件refcount代替。
6. **cache淘汰。** 只把binding从LIVE改CLOSING，禁新token；已有使用完成后由唯一unmapper卸B映射。不退源页，不改source generation。CLOSING尚未撤图的记录必须仍被owner退出流程追踪，不能先从所有索引失联。
7. **源真正退出。** 在page_pool_return_page增加release计数、清pp_info和put_page之前，将owner置EXITING、拒新binding。取得待关闭binding的元数据引用，禁新使用；等待/核已有使用，执行实际B unmap与必要同步。全部完成后才允许原源退出步骤。正常frame引用与“token先放、frame后还”的合同应使这里没有本帧的活token，但检查不代替这个合同。
8. **target关闭。** target gate禁新binding/token；真实停止新TX、完成或合法停机回收已有DMA。与source EXITING竞争时同一个binding只有一个实际unmapper，target/domain配置在其间仍有效。frame cleanup、失败和source退出必须同样用这条责任规则。

拟议锁序只作接口要求：target注册控制锁→source owner锁→binding状态锁；不得拿owner锁再等待target锁。source退出先在owner锁下关门并收集持引用的binding，再释放owner锁处理target/卸图；DMA API或等待均不在这些spinlock下执行。completion只修改短时binding责任后再归frame。尚未给出可编译锁实现；不可将此文字当并发证明。

## 退役上下文不允许被隐藏

page_pool的最后return可以在NAPI/softirq发生，不能无条件使用dma-buf的睡眠map/unmap/reservation入口，也不能等同一个NAPI还未处理的CQ。

- 若选定DMA API/backend允许在该上下文完成少量关闭且成本满足合同，可同步B撤销后继续原source release。
- 若须worker/读者宽限期，则把该真实退出页和待关闭关联转入退役隔离Q。应保留原source持有与pool inflight责任，未完成时不得先做release_cnt/clear_pp_info/put_page。worker完成后才执行一次原退出步骤；不能反复递归同一hook。Q期间物理页、控制记录、target引用和延迟全部计费。这里保留的是**真正退出的页**，不是让空闲mapping cache一直pin所有可recycle页。
- driver target drain或同步没有有界服务事实时，不给有限退休时间保证。容量不足时退回普通O0/O1，不能用无限Q或预留资源隐藏问题。

## 条件正向序列与最小反例

| 序列 | 条件内判断 |
| --- | --- |
| p为正常源页；B首次map、CQ、token_put、frame_return；p仍满足page_ref=1回pool；后续实际分到p且T仍有效，再次B使用；源最终EXITING→B撤图完成→原release | 可以复用一个source generation的mapping，不增加空闲cache的普通page ref。只是合同内序列，未观测实际分配/流量或节省时间 |
| cache额外get_page；consumer归还时page_ref=2→退出A pool；随后A无其他可用页而分配q | 源码允许的反馈反例：p留在B外部，q是替代源供给，可能比源内复用多一页。不能强令两policy下次仍是p，也不能断言有其他cache时一定新增q |
| 无ref缓存没有source release联动；源clear/put后p改作其他用途，B alias仍有效 | 失去owner授权，弱引用查表加generation也不能撤掉已存在硬件权限。反例要求补真实退出联动，不是增设逐包S需求 |
| B CQ后先frame_return，最后pool return触发source-close，再等待本帧仍未put的token | 自等待风险；必须token先放再归frame，或给出不在原上下文等自己的等价协议 |
| owner先clear/put再让worker卸B | physfree/新owner与旧alias重叠，失败。安全defer必须记Q和真实所有权 |
| 同一页只去B一次，随后退出；或后续用途不授权B；或generic adapter已用相同hook/表示完成相同工作 | 分别失去CPU摊销、失去授权适用性、或失去独立整合差异。均只限制这一版本/范围 |

上述序列均为条件推导，未执行状态模型或驱动。已有dma-buf、DMA_PMD的通知/反向关联模式不在此认领为新；普通O3获得全部同样付费接口及FIFO优化。当前可核的是执行位置、必要责任和反例，独立实现成本差异仍待证。
