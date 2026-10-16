# I预筛原始证据与SHA

2026-10-15。仅复用既有原件和定向读当前工作树；本轮未发网络请求、未采集/运行新trace，未改原件。SHA256覆盖整个原件文件；行号对应本次读取字节。生产树本来dirty，git HEAD不能代替这些文件哈希。

## 决定性原文入口

1. **正常生命周期**：I-S1 `ixgbe_main.c:8317–8329`保存DMA地址，`:1142–1165`先查DD后调用unmap。只证代码存在，不证本机NIC或跨设备共置。
2. **条件后端与批处理**：I-S3 `mapping.c:159–181`分direct和ops；I-S4 `dma-iommu.c:720–727`区分FQ与非FQ，`:127–152`在flush counter完成后释放IOVA/页表链。不能推每包一条fence。
3. **先排普通对照缺口**：I-S5 `iommu.c:1804,1807–1817`省略PSCID/ADDR；I-S6 `iotlb.sv:225–244`已有定向分支。I-S5 `:1620`的普通host PSCID是domain->id，PASID路径`:1683,1693`另用pasid。
4. **作者强反解释**：I-P2 `:28–40`指出现有等时流量应容忍偶发完整walk，不能把每个额外miss认定为deadline失败。
5. **现有trace不够**：I-E1 `:10–31`明确数据源事先quiet且不证时间/数据drain；I-E2前3行只有CQ相关字段。

### 短代码原文（完整上下文按下方原件入口）

**I-S1** `/home/xvhaocheng/work/iommu-platform/linux/drivers/net/ethernet/intel/ixgbe/ixgbe_main.c`

```text
1142: 		/* if DD is not set pending work has not been completed */
1143: 		if (!(eop_desc->wb.status & cpu_to_le32(IXGBE_TXD_STAT_DD)))
1144: 			break;
1161: 		/* unmap skb header data */
1162: 		dma_unmap_single(tx_ring->dev,
1163: 				 dma_unmap_addr(tx_buffer, dma),
1164: 				 dma_unmap_len(tx_buffer, len),
1165: 				 DMA_TO_DEVICE);
8317: 	dma = dma_map_single(tx_ring->dev, skb->data, size, DMA_TO_DEVICE);
8325: 		/* record length, and DMA address */
8326: 		dma_unmap_len_set(tx_buffer, len, size);
8327: 		dma_unmap_addr_set(tx_buffer, dma, dma);
8328: 
8329: 		tx_desc->read.buffer_addr = cpu_to_le64(dma);
```

**I-S4** `/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/dma-iommu.c`

```text
720: 	iotlb_gather.queued = READ_ONCE(cookie->fq_domain);
721: 
722: 	unmapped = iommu_unmap_fast(domain, dma_addr, size, &iotlb_gather);
723: 	WARN_ON(unmapped != size);
724: 
725: 	if (!iotlb_gather.queued)
726: 		iommu_iotlb_sync(domain, &iotlb_gather);
727: 	iommu_dma_free_iova(cookie, dma_addr, size, &iotlb_gather);
```

**I-S5** `/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/iommu.c`

```text
1803: 		riscv_iommu_cmd_inval_vma(&cmd);
1804: 		// riscv_iommu_cmd_inval_set_pscid(&cmd, domain->id);
1805: 	}
1806: 
1807: 	// if (start && end && pgsize) {
1808: 	// 	/* Cover only the range that is needed */
1809: 	// 	for (iova = *start; iova <= *end; iova += *pgsize) {
1810: 	// 		printk("Sending IOTINVAL with ADDR");
1811: 	// 		riscv_iommu_cmd_inval_set_addr(&cmd, iova);
1812: 	// 		riscv_iommu_post(domain->iommu, &cmd);
1813: 	// 	}
1814: 	// } else {
1815: 		riscv_iommu_post(domain->iommu, &cmd);
1816: 	// }
1817: 	riscv_iommu_iofence_sync(domain->iommu);
1618: 	} else {
1619: 		dc->iohgatp = 0; /* no stale guest-stage root on a host transition */
1620: 		dc->ta = cpu_to_le64(FIELD_PREP(RISCV_IOMMU_DC_TA_PSCID, domain->id));
1621: 		dc->fsc = cpu_to_le64(riscv_iommu_domain_atp(domain));
1681: 	/* register mm notifier */
1682: 	mm = iommu_domain->mm;
1683: 	domain->pscid = pasid;
1684: 	domain->iommu = ep->iommu;
1685: 	domain->mn.ops = &riscv_iommu_mmuops;
```

**I-S6** `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv`

```text
225:                     3'b001: begin
226:                         // 2S disabled, 1S enabled, PSCID match, exclude global entries
227:                         if((!tags_q[i].en_2S && tags_q[i].en_1S) && (tags_q[i].pscid == inval_i.pscid) && !content_q[i].content_1S.g) begin
228:                             tags_n[i].valid = 1'b0;
229:                         end
230:                     end
231:                     3'b010: begin
232:                         // 2S disabled, 1S enabled, IOVA match, include global entries
233:                         if((!tags_q[i].en_2S && tags_q[i].en_1S) && 
234:                             (vaddr_4k_match[i] || vaddr_2M_match[i] || vaddr_1G_match[i])) begin
235:                                 tags_n[i].valid = 1'b0;
236:                             end
237:                     end
238:                     3'b011: begin
239:                         // 2S disabled, 1S enabled, IOVA match, PSCID match, exclude global entries
240:                         if((!tags_q[i].en_2S && tags_q[i].en_1S) && 
241:                             (vaddr_4k_match[i] || vaddr_2M_match[i] || vaddr_1G_match[i]) &&
242:                               tags_q[i].pscid == inval_i.pscid && !content_q[i].content_1S.g) begin
243:                                 tags_n[i].valid = 1'b0;
244:                             end
```

**I-S7** `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv`

```text
276:             REGISTER: begin
277:                 if (mem_resp_i.r_valid) begin
278:                     mem_req_o.r_ready   = 1'b1;
279:                     if (mem_resp_i.r.last) begin
280:                         cmd_n[127:64]   = mem_resp_i.r.data[((64*1) &
281:                                                 (RVIOMMUCfg.AxiDataWidth-1))+:64];
282:                         cq_head_o       = (cq_head_i + 1) & idx_mask;
283:                         state_n         = DECODE;
359:                         // Valid IOFENCE.C command
360:                         else begin
361: 
362:                             // TODO: Add IOFENCE support with PR and PW using R/W counters
363: 
```

**I-S9** `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv`

```text
119:     InclPC              : 1'b1,
120:     InclAxiBC           : 1'b1,
121:     InclDbg             : 1'b1,
122:     InclATS             : 1'b0,
123:     MSITrans            : rv_iommu_cfg::MSI_BT_MRIF,
```

### 外部原文的限定引用

来自I-P1:63的原话：“over invalidating might impact live DMA”。其上下文只是reviewer提出摄像/显示担忧，未给实际预算/轨迹。
来自I-P2:29的原话：“lookups and has to be tolerant of an occasional full walk.” 此处保留作者的反解释，未把它升级为目标设备实测事实。

## 原件清单

### I-S1

- 原件：[ixgbe_main.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/net/ethernet/intel/ixgbe/ixgbe_main.c:1133)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/linux/drivers/net/ethernet/intel/ixgbe/ixgbe_main.c`
- 本次核读行：1133–1165, 1181–1187, 8317–8329, 8369–8370
- SHA256：`dc2b37e252f805799c7475eee468b0c16a23d671ea07f4701a7c54f181c55834`
- 大小：324308 bytes；范围：正常TX map、descriptor保存、DD检查和unmap；不是当前已部署NIC证明

### I-S2

- 原件：[dma-mapping.h](/home/xvhaocheng/work/iommu-platform/linux/include/linux/dma-mapping.h:328)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/linux/include/linux/dma-mapping.h`
- 本次核读行：328–344, 412–417
- SHA256：`13939a6846084a2b4fc0a4791694d517be19318594b794b496e4e8f5f4b73186`
- 大小：20491 bytes；范围：single API到page API包装

### I-S3

- 原件：[mapping.c](/home/xvhaocheng/work/iommu-platform/linux/kernel/dma/mapping.c:141)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/linux/kernel/dma/mapping.c`
- 本次核读行：141–184
- SHA256：`698bf2e08a5429319dd278219ac437d5d5198537907c7c2e0d31d9610192c559`
- 大小：25184 bytes；范围：运行时direct与ops后端选择，不能从API名称推出IOMMU命令

### I-S4

- 原件：[dma-iommu.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/dma-iommu.c:127)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/dma-iommu.c`
- 本次核读行：127–172, 178–219, 690–727, 1015–1025, 1070–1083, 1598–1599
- SHA256：`68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`
- 大小：48385 bytes；范围：条件iommu-dma路径；非FQ同步，FQ按counter归还IOVA/页表链

### I-S5

- 原件：[iommu.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/iommu.c:333)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/iommu.c`
- 本次核读行：333–363, 492–544, 571–575, 588–602, 1610–1626, 1681–1694, 1777–1845
- SHA256：`62c137d53cfaeac5374aefebd84f08457a362ddfde681192f550aed6a608ef0c`
- 大小：82381 bytes；范围：host flush省略PSCID/ADDR；普通host与PASID context身份不同；CQ轮询不是全平台drain证明

### I-S6

- 原件：[rv_iommu_iotlb.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:215)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv`
- 本次核读行：215–244, 315–331
- SHA256：`2868d6de26c8d98d2a750a0d78f6ce96cb896b9253b82b070a3e3772f37e9d26`
- 大小：24218 bytes；范围：真实PSCID/VA失效匹配和inval优先分支；未证明性能/丢fill

### I-S7

- 原件：[rv_iommu_cq_handler.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:276)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv`
- 本次核读行：276–283, 310–338, 348–392
- SHA256：`9ce744255c82c72769ff666cebd67311ae60ce65b06940c2b693f93ee4c94026`
- 大小：21247 bytes；范围：CQH在fetch末拍推进；VMA/GVMA脉冲、PR/PW TODO和ATS条件等待

### I-S8

- 原件：[rv_iommu_top.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv:788)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv`
- 本次核读行：788–803
- SHA256：`d0ceda29c99904c8d8f3567133b17bf9be784bdc8eb67a3b5cc5542ccba56669`
- 大小：29845 bytes；范围：无ATS分支inflight置零

### I-S9

- 原件：[ariane_soc_pkg.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv:110)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv`
- 本次核读行：110–137
- SHA256：`88227b92e8796df4a7bedef59208b963fe09a64cb3a5e1255b5f55521f978d55`
- 大小：5508 bytes；范围：FPGA来源配置InclATS=0；不是已加载bit身份证明

### I-S10

- 原件：[ariane_peripherals_xilinx.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1394)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`
- 本次核读行：1394–1408
- SHA256：`9eaa9edbadef37c5323e15f3c2b201723af1cc58806517ab5f36f92c0d591666`
- 大小：69637 bytes；范围：nonblock wrapper复用同Cfg，仅改MSITrans

### I-P1

- 原件：[smmu-range-concern.txt](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-concern.txt:16)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-concern.txt`
- 本次核读行：16–66, 70–79
- SHA256：`66f0dd840c42901b460adb3389786403e22417135e83f5e7e09c0659c6da7d2d`
- 大小：6220 bytes；范围：SMMUv3命令时延优化与live DMA担忧的双方原讨论
- 原始来源：[官方/作者发布页](https://lists.infradead.org/pipermail/linux-arm-kernel/2026-July/1146330.html)

### I-P1-raw

- 原件：[smmu-range-concern.html](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-concern.html)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-concern.html`
- 本次核读行：原始字节，定位见对应txt
- SHA256：`58eafb3429af104cc278929dd582b221f987c5bc258deed29ee3ef7d7dd6f425`
- 大小：10067 bytes；范围：上一轮保存的原始HTML
- 原始来源：[官方/作者发布页](https://lists.infradead.org/pipermail/linux-arm-kernel/2026-July/1146330.html)

### I-P2

- 原件：[smmu-range-response.txt](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-response.txt:16)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-response.txt`
- 本次核读行：16–45
- SHA256：`05177acebbc50464f467f04299231d28c4071155f08c570a62ada35f368b93b3`
- 大小：3314 bytes；范围：作者full-walk余量反解释与有条件formal isochronous后继
- 原始来源：[官方/作者发布页](https://lists.infradead.org/pipermail/linux-arm-kernel/2026-July/1146543.html)

### I-P2-raw

- 原件：[smmu-range-response.html](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-response.html)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-response.html`
- 本次核读行：原始字节，定位见对应txt
- SHA256：`c2dc0aaf66ab4e93cc1b4e2e87da30074a1af5c9753c32fce979668321835231`
- 大小：6129 bytes；范围：上一轮保存的原始HTML
- 原始来源：[官方/作者发布页](https://lists.infradead.org/pipermail/linux-arm-kernel/2026-July/1146543.html)

### I-P3

- 原件：[riscv-queues.txt](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/riscv-queues.txt:280)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/riscv-queues.txt`
- 本次核读行：280–282, 302–355
- SHA256：`86ea6cf16a439b8f1ce99aff6a21e0cbebb27fa6780e72817e0480bed3bd7084`
- 大小：33152 bytes；范围：允许扩大失效；CQ完成前缀；PR/PW范围
- 原始来源：[官方/作者发布页](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)

### I-P3-raw

- 原件：[riscv-queues.html](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/riscv-queues.html)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/riscv-queues.html`
- 本次核读行：原始字节，定位见对应txt
- SHA256：`6e647a5bc316c03f60eb45485d499e31df76258ccb5e5afad03dd878e11b2f1d`
- 大小：82755 bytes；范围：上一轮保存的官方HTML
- 原始来源：[官方/作者发布页](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)

### I-P4

- 原件：[intel-vtd-rev5.txt](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/intel-vtd-rev5.txt:7496)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/intel-vtd-rev5.txt`
- 本次核读行：7496–7527, 7628–7653
- SHA256：`c3011e0397ee146133442654dd2b477b9c93baf874a5ea671f9cc15c86bb1be3`
- 大小：1338167 bytes；范围：Intel wait最低组保证、FN选择和timeout约束
- 原始来源：[官方/作者发布页](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf)

### I-P4-raw

- 原件：[intel-vtd-rev5.pdf](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/intel-vtd-rev5.pdf)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/intel-vtd-rev5.pdf`
- 本次核读行：原始字节，定位见对应txt
- SHA256：`a136448f2a14d49aa524106f58a139a5add90f78eef8a6abaabcc917ebc753b6`
- 大小：12811634 bytes；范围：上一轮保存的Intel官方Rev5 PDF
- 原始来源：[官方/作者发布页](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf)

### I-E1

- 原件：[cq-check-results.json](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/model/cq-check-results.json:1)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/model/cq-check-results.json`
- 本次核读行：1–34
- SHA256：`3936ce0e19334c178c4df723f9f6ed181710b99b4c4442db03f054ff38623db0`
- 大小：4562 bytes；范围：已有CQ功能迹的明确条件和未验证范围

### I-E2

- 原件：[cq-normal.trace.jsonl](/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/model/cq-normal.trace.jsonl:1)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/model/cq-normal.trace.jsonl`
- 本次核读行：1–3
- SHA256：`8c20ea603718d542927dde16d43dda6f6f737f11bedec49af2973e807550f308`
- 大小：41960 bytes；范围：CQ AXI/失效/marker/head字段；不能替代正常TX/live联合trace

### I-E3

- 原件：[a2-sva-implementation-20261015.md](/home/xvhaocheng/work/iommu-platform/docs/a2-sva-implementation-20261015.md:55)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/docs/a2-sva-implementation-20261015.md`
- 本次核读行：55–78, 104–115
- SHA256：`edb0d5ad2a51780271eb5a0ad8f194c430f3b517a1a77658cb95393b56ba753c`
- 大小：17570 bytes；范围：A2功能证据与原生未运行边界，复用不重跑

### I-E4

- 原件：[assessment.md](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/assessment.md:23)
- 绝对路径：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/assessment.md`
- 本次核读行：23–27, 49–92
- SHA256：`678ed72ce2010968c5ac09711f77459cd2d1d96c6447dcd1bd85e57310aafdb8`
- 大小：21058 bytes；范围：前轮范围、候选卡与未知项，保留原件不改写

