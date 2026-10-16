# §54：两个实际 WRITE parent 的推进与 row1 换代

Codex 独立关键源审核，2026-10-09。只读实际 iDMA backend/legalizer/transport/channel-coupler/dma_core_wrap 及 stage54 当前复制的 q writer/helper/top；没有实现、构建、运行或修改其他文件。本文判定根席拟接入的正常2KiB copy合同，不宣称复制代码已经接受 len127 / offset3072。

## 决定性结果

**没有“首个1024B WRITE的B必须等整个2048B source RLAST”的硬件依赖。** CONF129确实使read/write长度与准入独立；actual W counter在128拍生成第一个raw WLAST，第一笔raw B可被iDMA接受，而source parent仍只返回128/256拍且RLAST=0。因此“第一WRITE parent B退休→私有捕获第二rawAW→真实WR预准备”可以推进，candidate与ordinary都不需要更改iDMA退休语义。

必要接口是：q writer按每个**实际rawAW parent**返回一个合并B；首个B不能被外部controller延到source done/job done；WR门只冻结新**global qAW**，不能冻结下一rawAW的私有捕获。否则会人为制造“没有第二rawAW就没有WR key，没有WR证书就不接第二rawAW”的循环。

**row1 seq0只有在目前非fall-through换父的完整一周期invalid gap合同下安全。** 若去掉此gap、同拍收旧B与新AW，必须给新父新的generation或显式使旧key/value失效。helper的cache_valid没有比较保存时的完整key，不能把新current-key当作旧证书的tag。

## 1. CONF129与真实R/W分段

[idma_reg64_frontend_reg_top.sv:518](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv:518) 把CONF bit0写入decouple，533–534把[9:6]写入cache。129=1|(2<<6)，所以decouple_rw=1、cache=0010，deburst位未开。

[idma_reg64_frontend.sv:344](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:344) /356实际把cache字段传到source/destination，367把conf.decouple传到decouple_rw。366的decouple_aw仍为0，但这不等于decouple_rw=0。

[idma_legalizer.sv:82](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:82) AXI64、max256beat时内部边界为2048B；209–214分别使用read/write各自距边界的长度，453–458分别推进r_tf和w_tf。对src页起点0、dst页内3072、length2048：

| actual channel parent | addr偏移 | bytes | AXI len | legalizer last |
|---|---:|---:|---:|---|
| source AR | src+0 | 2048 | 255 | 整个read parent |
| first destination AW | dst+0（页内3072） | 1024 | 127 | w_req.last=0 |
| second destination AW | dst+1024（下页起点） | 1024 | 127 | w_req.last=1 |

W metadata的num_beats直接取各自AW.len（411–416）；w_req.last取w_done（432–433）。后一AW不要求read tf再次valid。read datapath metadata在source RLAST才pop，不会因此阻止另一条write metadata FIFO推进。

## 2. 首WRITE WLAST/B与source RLAST独立

[idma_axi_transport_layer.sv:395](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:395) 当前W metadata在自己的last_w被传输时就ready/pop。418–438加载当前AW长度127并按每拍write_happening递减；第128拍last_w=1。它不读取R.last来决定W.last。R的正常payload逐拍进入buffer（254–266），source R.last仅参与read metadata退休（268–282）。

该transport451–455直接取raw BVALID作为w_dp_valid，并以w_dp_ready作为BREADY。实际wrapper [dma_core_wrap.sv:207](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:207) 的1D分支将idma_rsp_ready常置1；backend [idma_backend.sv:519](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519) 的NO_ERROR_HANDLING把w_dp_rsp_ready直接接rsp_ready。第一笔lastflag=0只作用于：

```systemverilog
assign rsp_valid = (ttype_i == idma_pkg::READ) ?
                   r_dp_rsp_valid : (w_dp_rsp_valid & w_last_burst);
assign w_dp_rsp_ready = rsp_ready;
```

所以第一笔B仍握手且pop write-last FIFO（backend637–648），只是对外job rsp_valid=0。它不等待source RLAST，不产生早job done。第二笔B对应lastflag=1，才完成这次copy；此前256拍源数据都必须供应给两个W parent。

### 最小可达握手序列

1. 两条metadata通路接收source AR256与firstAW128；decouple_rw使write legalizer随后产生secondAW128，可暂存在AW fall-through/metadata FIFO。
2. source仅交付beat0..127，最后这一拍的**logical RLAST仍为0**。first W metadata依自己的计数发raw WLAST=1；q adapter的第64个q2子B后返这个raw parent的合并B。
3. iDMA BREADY=1，接收first rawB；外部job done仍0。q writer清first parent_live。
4. 下一时钟私有接受held secondAW128，row1得到第二页真实WR键；此时source仍128/256，没有RLAST，也没有backend_idle。
5. 普通方案可先完成第二WR预准备再发第二段physical sourceAR；candidate可暂退A，让V完成原长R，取得第二WR证书后给A补读。二者都仍最终只给source raw parent一个logical RLAST。

外部实现若把第一rawB等到source RLAST或原job完成才返，步骤3→4会被人为阻断；这不是当前iDMA的要求。

## 3. channel coupler在当前配置中不存在

stage top380/439两DMA均RAWCouplingAvail=0；[dma_core_wrap.sv:314](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:314) 把它传给backend，319同时选择NO_ERROR_HANDLING。

backend811–835只有RAWCouplingAvail为1才实例channelcoupler；当前837–857使用独立AW fall-through，未连接R-first或R-last门。也直接读了 [idma_channel_coupler.sv:105](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_channel_coupler.sv:105)：它的credit来源是**first R handshake**，不是whole RLAST；但不能用这点替当前配置背书，因为此实例根本未被选中。若未来打开coupler、仍使decouple_aw=0，一sourceR、两destinationAW的credit对应需要重新审定，不能继承本结论。

## 4. row1旧B→新AW：精确一周期gap与ABA反例

stage当前 [q_write_splitter.sv:31](/home/xvhaocheng/work/iommu-platform/out/shared-read-permission-frontier-20261009/q_write_splitter.sv:31) 为`aw_ready = !parent_live_q && !b_valid_q`；94–95原B握手才清live。helper [hit_bypass_parent_translation.sv:175](/home/xvhaocheng/work/iommu-platform/out/shared-read-permission-frontier-20261009/hit_bypass_parent_translation.sv:175) 在parent invalid/seq不同时清valid/prepared。top971目前seq固定0。

| 时点 | q writer | helper看到的row1 |
|---|---|---|
| edge t：旧rawB handshake | 旧live=1，AWREADY=0，不能同拍收新AW；edge后live=0 | edge前仍live，旧valid尚未清；edge后cache_valid因parent_valid=0立即为0 |
| t→t+1完整周期 | nextAW可VALID并稳定，READY=1 | parent_valid整个周期为0 |
| edge t+1：收新AW | 新parent_q与live=1在edge后生效 | edge前看到invalid，清旧valid/prepared；edge后新键但cache_valid=0 |
| 后续 | 私有新header保持，global qAW可仍被hold | 对新键真实WR成功后才新valid |

因此当前同步实现即便seq0，旧证书没有一拍穿到新parent。不能省略那个完整invalid周期；它是安全合同的一部分。原parent的late slow response还受helper222–227的full-key与seq一致检查；第二页键不等于旧页键，不能late-fill成第二页。

**零bubble反例**：若AWREADY改为允许旧B与新AW同edge，旧row1valid=1/seq0、旧基址3072，edge后current parent基址改4096但live仍1、seq仍0，helper没有见invalid。cache_valid只检查live/seq/permission，不比保存时key；reuse125–129用**新的current key**去匹配新WRITE请求，却拿value_q中的**旧PA/权限**返回。第二页未取得WR证书也会看似成功。这是准确的证书换代错误，不是普通性能miss。

可接受修复二选一：保留上述强制gap并断言新capture之前valid/prepared已经清；或每次实际rawAW capture提供新row1 generation，并使所有cache_valid、reuse、late-fill检查该generation。后者适合将来流水换父/重复job。不能以globalgroup计数“名字不同”自动保证：必须将已捕获父的身份稳定地连接到helper，且跨job epoch/wrap明确。现helper已有seq字段，但当前top固定0没有提供这项身份。

## 5. 接入时必须兑现的边界

- **私有rawAW capture不看WR hold**：current adapter31可满足；hold只应限制新global qchild创建。新页地址/context必须先存下来才能真实预准备，不能形成key/ready循环。
- **每实际parent各返B**，只等该parent的所有真实q-B，不等source RLAST/job done。第一/第二raw WLAST分别在job byte1024/2048；physical子WLAST仍由q adapter重写，不能反向改iDMA raw语义。
- `raw_written_o`必须是job全局offset。当前stage25还是parent-local `offset_q + got_q`，新parent捕获会归0；需要加实际parent在job中的起点或等效累计cursor。globalgroups目前不随parent capture reset，须保留其job内语义。
- 每个新WRITE证书成功才推进该parent的certified job end；first-parent invalid gap不能使旧128 frontier变成下一页已授权256。已发source R的temporary drop要先排旧A q-W/B及A skid；后续进度不能把未授权新页当作旧certificate的offset。
- copied q writer62仍限制len255/pageoffset0；这是尚需根席接入的限制，不是当前源码已跑过两len127 parent的证据。本审核只证明真实iDMA可以产生并独立退休它们。所有列出的新时序仍须由根席实际DUT验证。

本问题需要的是parent级B/cursor/generation接口正确接入，没有证据要求修改真实iDMA backend或提前伪造source RLAST。因为read/write metadata与first-B退休在当前CONF/RAWCoupling/ErrorCap下独立，candidate不会由原硬件的whole-RLAST依赖产生必然chickenegg；如果外部adapter强制wholejob退休或零bubble无换代，才会分别制造进度环或权限错误。
