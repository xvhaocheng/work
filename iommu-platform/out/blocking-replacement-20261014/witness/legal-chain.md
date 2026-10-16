# BR1合法事件链与门判别（独占witness，2026-10-14）

结论：交付新的19copy／17key冷功能图；它证明真实producer publication、源内顺序和完整R/W/B义务可以共存，但没有给出优选victim随当前progress／依赖反转的正例，因此本产物不支持性能入口。没有运行模型、实验、旧trace、validator或build。全部内容是定向原文核查与手推，数值是合同义务，绝非实测事件。

## 真实冷图

四logical tasks、原两source、每source同时最多一笔aligned64B copy。formal keys k0..k15,q，DID10/11必须由付费配置设成同一可信PSCID/root/epoch，S1 Sv39/4KiB nonglobal RW、U/A/D合法，S2 Bare、DDTP.mode2 extended64B DC；生产默认distinct source身份不自动共享。IoTLB与DDTC全冷，只初始化普通页表、DC和实际数据，不植入translation。setup CPU/MMIO/cache维护成本未测，为null。

- T0/source0：k0..k7分别copy稳定k_i+0→k_i+64，每copy64B；最后B及检查receipt后释放。这些结果是实际record初始化与最终输出，不是丢弃payload的预热。
- T1/source1：T0 receipt后，k8..k15同样初始化，最后B及检查receipt后释放。
- T2/source0：T1 receipt后copy k15+64→q+0；q的B及publisher receipt构成真实发布。
- T3/source1：与T2在同一真实T1 receipt后就绪。先copy k1+64→k2+128；其B以及T2 q发布完成后，再copy q+0→k0+128；最后B和consumer receipt释放。跨两copy的task资源要由付费软件owner界面明确保留，native job注册不会自动证明这一点。当前copy/buffer的更近release另行计入。

初始k_i+0=稳定64B向量V_i。所有k_i+64必须最终等于V_i，q+0及k0+128=V15，k2+128=V1；写块与未消费source块不别名，未提前发布q。T3第二copy同时有真实q数据依赖和同source第一copy顺序。q填入时可令T2 write-q是正常main miss；T2 R与W的生产先后不靠发明改动，native AW-before-AR优先规则仍执行。T3是否已获得private许可/AR接受取决于真实native调度，本稿不给任意时间表。

完整义务：19copy，19 dataAR＋19dataAW，152 payloadR＋152W（各1216B），19B，19Rtranslation＋19Wtranslation，四task收据。保护图中必要resident集合时冷S1成功walk为17、每walk三次dependent8B PTE：51PTE_AR/R、408B；两个cold DID在mode2 extendedDC各一次64B DDT读取：2DDT_AR、16R、128B。因此该参考路径总memory read为219beat／1752B、72AR；dataW为152beat／1216B、19AW/B。任何实际evicted-key rewalk另加三个dependent PTE_AR/R、24B；不把它替换成任意固定delay。全quiet必须清空walk/policy/response/held/native队列、R/W/B、task/consumer/release义务，零error injection不等于fault路径已验。

冷前16key由实际copies访问；正常post-fill hit/touch按原tree-PLRU推进。在未保护全mask且每新key使用后touch的这段cold prefix，四层tree首次16fill的way顺序是0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15。重复同key R/W touch不额外改变该方向，故q确实是full16后的正常新fill，不复制预热snapshot。不声称所有不同admission policy都有该prefix；比较策略必须各自冷执行，本稿未执行。

## Native关键证据

下列路径都相对workspace根，完整SHA256在gate-findings.json。

1. `cva6/corev_apu/tb/ariane_soc_pkg.sv:111,115`（加iommu-platform前缀）：NumOutstandingTrans8、NumIotlbEntries16。`rv_iommu_banked_ctl.sv:158-167`将事务队列split，故每bank AR4/AW4；`rv_iommu_trans_ctl_wrap.sv:121-157`建立原AX FIFO。
2. `rv_iommu_trans_ctl_wrap.sv:331-350`明确physical AW/AR addr取trans_resp_data_q.spaddr；`693-702`在response时保存private结果；`719-726`只在COMPLETE physicalAW/AR acceptance使AX FIFO head ready。因此q walk时原q头仍在其FIFO里；不可把q既算queued head又另加一份。已private许可或physical接受的R/W/B不因cache eviction重新翻译。
3. `rv_iommu_tl_wrap.sv:282-308`：两bank lifecycle、一responseport；fast新probe只在main PT且walker active、S1/Bare2、otherbank时。`346-353`RR单response；`1280-1357`正常walk通过update后hit进入COMPLETE，保留原权限与post-fill路径。`rv_iommu_nonblock_hit.sv:50-60,89-113,146-169`核完整request/DC/leaf合法性及成功accept/touch和持有response，非握手意图不是许可。
4. `rv_iommu_banked_ctl.sv:245-276`：visibleAW或未填W保留read_owner，SELECT前限制另一bank新read；signed AW-minus-WLAST维持原ownership。`rv_iommu_trans_ctl_wrap.sv:623-649`：AW先选；未选AR必须read_admit或原FullBufferDID证书，不能撤回已选VALID。
5. **生产证书纠正**：`iDMA/src/idma_fullbuffer_certificate.sv:16-18`仅在CapacityBytes≥4096、length==4096且src/dst页对齐给permit；64B不符合。`systems/cva6_reg/dma_core_wrap.sv:173-183`从真实job handshake/length给证书。`fpga/src/ariane_peripherals_xilinx.sv:1262-1269`原两source512beatbuffer仍在，但“大buffer”不等于64B取得例外。旧RCS1 limitations中64B-fullbuffer是conditional抽象，不能拿来证明production当前64B链。此图permit=false且guard保留；不改成4KiB或缩buffer寻找胜例。
6. `rv_iommu_iotlb.sv:365-374,405-423`原eligible-mask treePLRU；全mask复现原victim。`ddtw.sv:180-186,220-238`mode2末级DC读取64B；`ptw.sv:277-279`每PTE单8B读。

## Horizon缩界与准确覆盖条件

如果next actual release指当前两active64B copy的B/receipt/buffer release，则每copy最多一个R与一个W formalkey，union≤4；q占其一，所以最多3个resident key直接暴露于本次eviction。加入每source一个下一R/W pair，union≤8、other resident≤7，至少9个resident victims没有直接next-use exposure。已private result/physicalAR/AW移出translation需要集合。原raw容量总16且q仍占1，容量至多15个other resident queued-key；这不是16slot实际占满可达性，更不是以后继续posted请求的免费oracle。

本19copy图即使把T3明确拥有的两copy都纳入其task release horizon，也仅H={k15,q,k1,k2,k0}；四个旧resident key相关，k3..k14共12个outside-H victim。选其中一个后，图内所有后续required keys均hit，没有之后的miss/fill/invalidation。若decision latency相同且选择本身不改变调度，12个outside-H初始替换对整段图的service/release结果相同；普通protect-H＋原maskedPLRU可以选出同样的no-effect tie victim。改变q依赖或当前已accepted progress并不会凭空让这些无使用key参与图。

这只裁决本新图和上述有限静态条件，不能推出BR1整个动态族被覆盖。两项必须保留：初始outside-H victim改变tree后，后续冲突fill可能再淘汰H；共享FIFO/native N-window非单调，强加miss可能改变调度或放慢一条路进而加速另一owner。这里没有把F(v)≥F(no-effect)当native公理，也未证明no-effect victim无条件最优。若要构成正向propagation差异，需交付同一合法cold图中具体后续fill/真实owner服务重排且优选反转的联合因果证据，并计入能观察它的输入与成本；本稿不搜或调该胜例。

## 门决定与未证

本次保留正向合法数据图、byte/beat/B义务和owner/key缩界；拒绝把该图的真实dependency当作BR1独有victim收益证据。没有给出仅依赖/progress改变、同时refs/tasksize/localcost/slack不能复现的优选victim反转。没有native cycle时间、费用headroom或吞吐；面积/Fmax/软件CPU/GPU/FPGA收益均null。性能入口由root结合普通control与成本席裁决，witness不越权。
