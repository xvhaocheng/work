# §32 Codex 关键源审定

## 现成接口与被纠正的推断

1. 原tests/nonblock_actual_dma_top.sv:556–577在Mode0直通，SourceReadGateEnable/Frontier只被latepool消费。旧latepool限制normal4KiB aligned/Depth8总量/len255，不能直接复用32KiB错位。本轮修改out复制的Mode0，仅SourceReadGateEnable1才例化新普通门，实际build-owner argv含新门和新top，无TEST_COMPLETION_GATE。
2. worker原答只看owner的说法已被修订撤回。原tests/nonblock_completion_pool_late.sv:94–106 can_drain含源/目的偏移及len/size条件。
3. worker修订对sticky位置仍读错。原该文件:301–302开始 if(!rst_ni)，:327–328的release更新在其内，:348才else运行分支；rg全部赋值除reset中的更新外，运行仅job_fire清零。因此不能称旧模块有正常运行期sticky释放/保持。即使更新在else，至本次meta_in清零也是每次首次offer后保持，不能推导为后续所有事务绕过首次判据。没有在本轮运行/修改旧模块，不把这条源码问题泛化为所有历史验收失败。
4. axi_mux.sv:166定义switch_id_t，:183–184 w_fifo存switch index，:330把aw仲裁选择入队，:353据其选slave W。四输入真实mux活跃port0/1，bit0是这两流的owner；prepend original ID bit2也是流标签，BankIdBit3是当前同bank分类，三者含义有区别。

## 实际新门

ordinary_copy_owner_gate.sv逐字段透传请求和响应，仅ARVALID/READY条件门控；无R/W payload storage。真实descriptor+virtualAW ring+physical准备/完成序号的对应，在当前两constant-ID normal静态copy范围内成立。virtualAW记录用mux首次offer，不重复记录锁住等待的AW；真实prepared physicalAW计数用completion握手，不用首次offer替代。

held_q在运行期每拍更新，首次offer后AR不撤回并核payload稳定；各流输入read ID0核；AW ring可被回收后重复使用，32bitseq支持本32KiB32段；job结束核两边写义务已清。宏ASSERTS_OFF不关闭本门手写fatal检查。仿真没有触发这些fatal，不是形式证明。

各owner运行grant64、两份CSV独立128次virtualAR精确匹配当前virtualAW的owner、offset、len与got0。当前W将源数据耗尽后才改变，所以不提前入不合适owner的AR到共享TC。物理侧含既有cut/队列，DDR-visible paw可以晚于virtualAR一拍；audit未强行用DDR时刻反证completion准备。

Gate增加两流descriptor、AW环/序号及held，既有physical_stream_order此时也成为功能信息。48B是iDMA数据FIFO，绝不是全部存储或LUT面积。计数grants/waits和held_ar历史用于观测/断言，其成本与可裁剪边界仍需综合。

## 执行与结果界限

DeepSeek prepare script仅准备artifact，Codex执行一次。manifest.json保留准备时哈希/not_run/not_built；其后Codex增加一条不推进clock的NATURAL-GATE printf，派生patch和最终source hash已存。没有第二次执行生成器。models/owner/relink.conf0.json仅被用作首次SV+C++构建一次，不执行canonical构建后再重复build。optional CONF1 argv均未运行。

另一个公平对照只在旧obj-full-buffer archive上重链接相同CONF0 CPP，不重建其RTL。owner和full分别V先/A先32KiB四项exit0/PASS，64AR/64AW、各流4096 R/W拍，bytes/guards/quiet通过。sourcegate会经正常iDMA反馈改变AW出现顺序，未强制保持两种模型完全相同输入Ax时序。

last DDR W27894 vs25146仅reset-relative终点差2748；无正式copy-time/steady-throughput窗口、无法分解纯译址成本。首VAR1032 vs68说明入口准入不同，first DDR sourceAR2880 vs1934还混有TC排队、AW顺序和页表/缓存时序，不能称全部为翻译延迟。wait55046是两流累计阻止计数；两流每拍各1，可能接近2×周期，不是占用或平均等待。

创新裁定：准确普通源读门已解除这一CONF0等待环，不能用解除停滞当新机制贡献。保留“提前逻辑译址/后端服务粒度与总成本”候选；强固定后译址分片必须先实现，动态同效同成本就停。先核actual AR flags允许拆分范围。post-TL固定/动态parent调度未实现。
