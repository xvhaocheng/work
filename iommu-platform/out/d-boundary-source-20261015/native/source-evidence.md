# 当前源码逐段证据

SOURCE_ONLY。以下是当前文件在生成时的精确原文与一基行号；完整文件hash、实际阅读范围见 source-manifest.json。片段只证明当前控制流，不证明已经构建或在设备执行。

## 设备、stream与全局锁；open/release

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:113–154`

```c
  113  struct idma_device {
  114  	struct miscdevice miscdev;
  115  	char name[16];
  116  	int id;
  117  	u32 did;
  118  	void __iomem *reg;
  119  	bool quarantined;
  120  };
  121  
  122  struct idma_stream {
  123  	struct idma_device *idma;
  124  	struct device *dev;
  125  	struct iommu_domain *domain;
  126  	void *src, *dst;
  127  	dma_addr_t src_dma, dst_dma;
  128  	unsigned long src_iova, dst_iova;
  129  	size_t size, offset, next_offset, touched_end, touched_tail;
  130  	u32 chunk;
  131  	u64 transfer_id, deadline_ns, last_complete_ns;
  132  	bool mapped_src, mapped_dst, in_flight, retain;
  133  	bool batch_configured, batch_started, reverse;
  134  	bool composition_trace;
  135  	struct idma_experiment_stats *stats;
  136  	struct idma_e3_diag_lane *diagnostic;
  137  };
  138  
  139  struct idma_batch_start {
  140  	bool attempted;
  141  	bool has_a;
  142  	bool a_triggered;
  143  	bool v_triggered;
  144  	u32 v_did, a_did;
  145  	u64 a_id, v_id;
  146  	u64 a_begin_ns, a_end_ns;
  147  	u64 v_begin_ns, v_end_ns;
  148  	u64 irq_span_ns;
  149  };
  150  
  151  static DEFINE_MUTEX(idma_devices_lock);
  152  static struct idma_device *idma_devices[IDMA_MAX_DEVS];
  153  /* All experiment preparation, IOVA mappings and DMA execution are serialized. */
  154  static DEFINE_MUTEX(idma_experiment_lock);
```

## open并不创建per-fd pool

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:215–225`

```c
  215  static int idma_open(struct inode *inode, struct file *fp)
  216  {
  217  	struct miscdevice *miscdev = fp->private_data;
  218  
  219  	fp->private_data = container_of(miscdev, struct idma_device, miscdev);
  220  	return 0;
  221  }
  222  
  223  static int idma_release(struct inode *inode, struct file *fp)
  224  {
  225  	return 0;
```

## IOVA查找与显式逐页映射

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:229–300`

```c
  229  static bool idma_iova_range_free(struct iommu_domain *domain,
  230  				 unsigned long iova, size_t size)
  231  {
  232  	size_t off;
  233  
  234  	for (off = 0; off < size; off += PAGE_SIZE) {
  235  		if (iommu_iova_to_phys(domain, iova + off))
  236  			return false;
  237  	}
  238  
  239  	return true;
  240  }
  241  
  242  static int idma_find_iova(struct iommu_domain *domain, size_t size,
  243  			  unsigned long *src_iova, unsigned long *dst_iova)
  244  {
  245  	unsigned long start = domain->geometry.aperture_start;
  246  	unsigned long end = domain->geometry.aperture_end;
  247  	unsigned long iova;
  248  	size_t span = size * 2;
  249  
  250  	if (end < start || span < size || span - 1 > end - start)
  251  		return -EBUSY;
  252  
  253  	iova = ALIGN_DOWN(end - span + 1, SZ_2M);
  254  	for (;;) {
  255  		if (!idma_iova_range_free(domain, iova, span)) {
  256  			if (iova < start + SZ_2M)
  257  				break;
  258  			iova -= SZ_2M;
  259  			continue;
  260  		}
  261  
  262  		*src_iova = iova;
  263  		*dst_iova = iova + size;
  264  		return 0;
  265  	}
  266  
  267  	return -EBUSY;
  268  }
  269  
  270  static int idma_iommu_map_range(struct iommu_domain *domain,
  271  				unsigned long iova, phys_addr_t phys,
  272  				size_t size, size_t pgsize)
  273  {
  274  	size_t off;
  275  	int ret;
  276  
  277  	for (off = 0; off < size; off += pgsize) {
  278  		ret = iommu_map(domain, iova + off, phys + off, pgsize,
  279  				IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
  280  		if (ret) {
  281  			while (off) {
  282  				off -= pgsize;
  283  				iommu_unmap(domain, iova + off, pgsize);
  284  			}
  285  			return ret;
  286  		}
  287  	}
  288  
  289  	return 0;
  290  }
  291  
  292  static void idma_iommu_unmap_range(struct iommu_domain *domain,
  293  				   unsigned long iova, size_t size,
  294  				   size_t pgsize)
  295  {
  296  	size_t off;
  297  
  298  	for (off = 0; off < size; off += pgsize)
  299  		iommu_unmap(domain, iova + off, pgsize);
  300  }
```

## buffer分配、初始化、显式map及cleanup、quarantine

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:320–409`

```c
  320  static struct page *idma_buffer_page(void *ptr)
  321  {
  322  	return is_vmalloc_addr(ptr) ? vmalloc_to_page(ptr) : virt_to_page(ptr);
  323  }
  324  
  325  /* Each byte distinguishes the DMA instance; words also encode position. */
  326  static u64 idma_pattern(struct idma_stream *s, size_t word)
  327  {
  328  	return (0xa53c691df027b84eULL ^
  329  		((u64)s->idma->id * 0x0101010101010101ULL) ^
  330  		((u64)word * 0x9e3779b97f4a7c15ULL));
  331  }
  332  
  333  static void idma_stream_cleanup(struct idma_stream *s)
  334  {
  335  	if (!s->dev || s->retain)
  336  		return;
  337  	if (s->mapped_dst)
  338  		idma_iommu_unmap_range(s->domain, s->dst_iova, s->size, PAGE_SIZE);
  339  	if (s->mapped_src)
  340  		idma_iommu_unmap_range(s->domain, s->src_iova, s->size, PAGE_SIZE);
  341  	if (s->dst)
  342  		dma_free_attrs(s->dev, s->size, s->dst, s->dst_dma,
  343  			       DMA_ATTR_FORCE_CONTIGUOUS);
  344  	if (s->src)
  345  		dma_free_attrs(s->dev, s->size, s->src, s->src_dma,
  346  			       DMA_ATTR_FORCE_CONTIGUOUS);
  347  }
  348  
  349  /*
  350   * Partial-failure safe: fields are assigned as resources are acquired, and the
  351   * caller always runs idma_stream_cleanup() on this stream afterwards, which
  352   * tolerates src-only, dst-only and unmapped states.
  353   */
  354  static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
  355  			       u32 pages, u32 chunk,
  356  			       struct idma_experiment_stats *stats)
  357  {
  358  	struct page *src_page, *dst_page;
  359  	size_t i;
  360  	int ret;
  361  
  362  	s->idma = idma;
  363  	s->dev = idma->miscdev.parent;
  364  	s->stats = stats;
  365  	s->domain = iommu_get_domain_for_dev(s->dev);
  366  	if (!s->domain || s->domain->type == IOMMU_DOMAIN_IDENTITY)
  367  		return -EOPNOTSUPP;
  368  	s->size = (size_t)pages * PAGE_SIZE;
  369  	s->chunk = chunk;
  370  	s->src = dma_alloc_attrs(s->dev, s->size, &s->src_dma, GFP_KERNEL,
  371  				 DMA_ATTR_FORCE_CONTIGUOUS);
  372  	if (!s->src)
  373  		return -ENOMEM;
  374  	s->dst = dma_alloc_attrs(s->dev, s->size, &s->dst_dma, GFP_KERNEL,
  375  				 DMA_ATTR_FORCE_CONTIGUOUS);
  376  	if (!s->dst)
  377  		return -ENOMEM;
  378  	for (i = 0; i < s->size / sizeof(u64); i++) {
  379  		((u64 *)s->src)[i] = idma_pattern(s, i);
  380  		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
  381  	}
  382  	src_page = idma_buffer_page(s->src);
  383  	dst_page = idma_buffer_page(s->dst);
  384  	if (!src_page || !dst_page)
  385  		return -EFAULT;
  386  	ret = idma_find_iova(s->domain, s->size, &s->src_iova, &s->dst_iova);
  387  	if (ret)
  388  		return ret;
  389  	ret = idma_iommu_map_range(s->domain, s->src_iova,
  390  				   page_to_phys(src_page), s->size, PAGE_SIZE);
  391  	if (ret)
  392  		return ret;
  393  	s->mapped_src = true;
  394  	ret = idma_iommu_map_range(s->domain, s->dst_iova,
  395  				   page_to_phys(dst_page), s->size, PAGE_SIZE);
  396  	if (ret)
  397  		return ret;
  398  	s->mapped_dst = true;
  399  	return 0;
  400  }
  401  
  402  static void idma_quarantine(struct idma_stream *s)
  403  {
  404  	s->retain = true;
  405  	WRITE_ONCE(s->idma->quarantined, true);
  406  	if (s->stats)
  407  		s->stats->quarantined = 1;
  408  	dev_crit(s->dev, "experiment DMA not safely drained; buffers retained until reboot\n");
  409  }
```

## 每软件COPY提交只改寄存器与ring状态

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:411–467`

```c
  411  static void idma_submit_config(struct idma_stream *s)
  412  {
  413  	if (s->diagnostic && s->batch_started)
  414  		s->diagnostic->launch_begin_ns = ktime_get_ns();
  415  	s->offset = s->next_offset;
  416  	writeq(s->src_iova + s->offset, s->idma->reg + IDMA_SRC_ADDR_REG_OFFSET);
  417  	writeq(s->dst_iova + s->offset, s->idma->reg + IDMA_DST_ADDR_REG_OFFSET);
  418  	writeq(s->chunk, s->idma->reg + IDMA_NUM_BYTES_REG_OFFSET);
  419  	writeq(0, s->idma->reg + IDMA_CONF_REG_OFFSET);
  420  }
  421  
  422  static void idma_submit_commit(struct idma_stream *s, u64 id, bool measured, u64 now)
  423  {
  424  	s->transfer_id = id;
  425  	if (s->diagnostic) {
  426  		if (s->batch_started)
  427  			s->diagnostic->launch_end_ns = now;
  428  		else
  429  			s->diagnostic->warm_submitted++;
  430  	}
  431  	if (s->last_complete_ns && measured) {
  432  		u64 gap = now - s->last_complete_ns;
  433  
  434  		s->stats->refill_gap_ns_sum += gap;
  435  		s->stats->refill_gap_ns_max = max(s->stats->refill_gap_ns_max, gap);
  436  		s->stats->refill_gap_samples++;
  437  	}
  438  	s->last_complete_ns = 0;
  439  	s->deadline_ns = now + (u64)IDMA_XFER_TIMEOUT_US * NSEC_PER_USEC;
  440  	s->in_flight = true;
  441  	s->next_offset += s->chunk;
  442  	if (s->next_offset == s->size)
  443  		s->next_offset = 0;
  444  	if (measured)
  445  		s->stats->submitted++;
  446  }
  447  
  448  static int idma_submit(struct idma_stream *s, bool measured)
  449  {
  450  	u64 now, id;
  451  
  452  	idma_submit_config(s);
  453  	id = readq(s->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
  454  	if (!id) {
  455  		/*
  456  		 * Without a valid ID we cannot prove that buffers are no longer
  457  		 * used.  A failed submit must not keep the previous transfer ID,
  458  		 * because the drain path matches DONE against transfer_id.
  459  		 */
  460  		s->transfer_id = 0;
  461  		idma_quarantine(s);
  462  		return -EIO;
  463  	}
  464  	/* Include configuration writes and the NEXT_ID trigger in refill cost. */
  465  	now = ktime_get_ns();
  466  	idma_submit_commit(s, id, measured, now);
  467  	return 0;
```

## 成对batch启动的局部保护

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:522–558`

```c
  522  	/*
  523  	 * Ownership and configuration are outside the protected trigger section.
  524  	 * Mark batch_started before the trigger so failure handling keeps the
  525  	 * same STOP/drain retention ownership as the ordinary batch path.
  526  	 */
  527  	if (a) {
  528  		a->batch_started = true;
  529  		idma_submit_config(a);
  530  	}
  531  	v->batch_started = true;
  532  	idma_submit_config(v);
  533  
  534  	start->attempted = true;
  535  	idma_comp_stage(v->composition_trace, v->dev, "pair-enter", 0);
  536  	preempt_disable();
  537  	local_irq_save(flags);
  538  
  539  	/*
  540  	 * Configuration consumed time: recheck both conditions before the first
  541  	 * NEXT_ID read so an expired or interrupted request never triggers either
  542  	 * device.
  543  	 */
  544  	if (ktime_get_ns() >= deadline) {
  545  		ret = -ETIMEDOUT;
  546  		goto out_untriggered;
  547  	}
  548  	if (signal_pending(current)) {
  549  		ret = -EINTR;
  550  		goto out_untriggered;
  551  	}
  552  
  553  	if (a) {
  554  		start->a_begin_ns = ktime_get_ns();
  555  		a_id = readq(a->idma->reg + IDMA_NEXT_ID_REG_OFFSET);
  556  		start->a_end_ns = ktime_get_ns();
  557  		start->a_triggered = true;
  558  		start->a_id = a_id;
```

## DONE观察及warm反馈

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:667–749`

```c
  667  /* 1: completion observed; 0: pending; negative: failed. Never refills itself. */
  668  static int idma_poll(struct idma_stream *s, bool measured)
  669  {
  670  	u64 done;
  671  
  672  	if (!s->in_flight)
  673  		return 0;
  674  	done = readq(s->idma->reg + IDMA_DONE_REG_OFFSET);
  675  	if (done != s->transfer_id)
  676  		return ktime_get_ns() > s->deadline_ns ? -ETIMEDOUT : 0;
  677  	s->last_complete_ns = ktime_get_ns();
  678  	if (s->diagnostic && !s->batch_started)
  679  		s->diagnostic->warm_completed++;
  680  	s->in_flight = false;
  681  	if (!s->batch_configured)
  682  		s->touched_end = max(s->touched_end, s->offset + s->chunk);
  683  	if (measured) {
  684  		s->stats->completed++;
  685  		s->stats->completed_bytes += s->chunk;
  686  	}
  687  	return 1;
  688  }
  689  
  690  static int idma_run_window(struct idma_stream *v, struct idma_stream *a,
  691  			   u64 count, u64 deadline, bool measured, u64 *end_ns)
  692  {
  693  	u64 completed = 0;
  694  	u32 spins = 0;
  695  	int ret;
  696  
  697  	if (!count)
  698  		return 0;
  699  	if (ktime_get_ns() >= deadline) {
  700  		ret = -ETIMEDOUT;
  701  		goto out;
  702  	}
  703  	if (signal_pending(current)) {
  704  		ret = -EINTR;
  705  		goto out;
  706  	}
  707  	ret = idma_submit(v, measured);
  708  	if (ret)
  709  		goto out;
  710  	for (;;) {
  711  		if (ktime_get_ns() >= deadline) {
  712  			ret = -ETIMEDOUT;
  713  			break;
  714  		}
  715  		if (signal_pending(current)) {
  716  			ret = -EINTR;
  717  			break;
  718  		}
  719  		ret = idma_poll(v, measured);
  720  		if (ret < 0)
  721  			break;
  722  		if (ret == 1) {
  723  			if (++completed == count) {
  724  				/* Capture V's end before polling/refilling A or taking snapshots. */
  725  				*end_ns = v->last_complete_ns;
  726  				return 0;
  727  			}
  728  			ret = idma_submit(v, measured);
  729  			if (ret)
  730  				break;
  731  		}
  732  		if (a) {
  733  			ret = idma_poll(a, measured);
  734  			if (ret < 0)
  735  				break;
  736  			if (ret == 1) {
  737  				ret = idma_submit(a, measured);
  738  				if (ret)
  739  					break;
  740  			}
  741  		}
  742  		if (!(++spins & 1023))
  743  			cond_resched();
  744  		cpu_relax();
  745  	}
  746   out:
  747  	*end_ns = ktime_get_ns();
  748  	return ret;
  749  }
```

## drain与验证整个buffer

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:751–787`

```c
  751  static int idma_drain(struct idma_stream *s)
  752  {
  753  	u64 done;
  754  	int ret;
  755  
  756  	if (!s->in_flight)
  757  		return s->retain ? -EIO : 0;
  758  	ret = readq_poll_timeout(s->idma->reg + IDMA_DONE_REG_OFFSET, done,
  759  				 done == s->transfer_id, 0, IDMA_XFER_TIMEOUT_US);
  760  	if (ret) {
  761  		idma_quarantine(s);
  762  		return ret;
  763  	}
  764  	s->in_flight = false;
  765  	if (s->diagnostic && !s->batch_started)
  766  		s->diagnostic->warm_drained++;
  767  	if (!s->batch_configured)
  768  		s->touched_end = max(s->touched_end, s->offset + s->chunk);
  769  	return 0;
  770  }
  771  
  772  static int idma_verify(struct idma_stream *s)
  773  {
  774  	size_t i;
  775  
  776  	if (s->retain)
  777  		return -EIO;
  778  	/* Verify untouched bytes as well, and source integrity independently. */
  779  	for (i = 0; i < s->size; i++) {
  780  		u64 pattern = idma_pattern(s, i / sizeof(u64));
  781  		u8 value = ((u8 *)&pattern)[i % sizeof(u64)];
  782  		u8 expected = (i < s->touched_end ||
  783  			       i >= s->size - s->touched_tail) ? value : (u8)~value;
  784  		u8 src = READ_ONCE(((u8 *)s->src)[i]);
  785  		u8 dst = READ_ONCE(((u8 *)s->dst)[i]);
  786  
  787  		if (src != value || dst != expected) {
```

## batch配置、STOP、collect、disable

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:821–880`

```c
  821  /* Batch mode still uses DEFAULT read/write copies. STOP never cancels AXI. */
  822  static int idma_batch_configure(struct idma_stream *s, u64 count)
  823  {
  824  	void __iomem *base = s->idma->reg;
  825  
  826  	if (readq(base + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC)
  827  		return -EOPNOTSUPP;
  828  	if (readq(base + IDMA_BATCH_CONTROL_OFFSET) & 2)
  829  		return -EBUSY;
  830  	writeq(0, base + IDMA_BATCH_CONTROL_OFFSET);
  831  	writeq(count, base + IDMA_BATCH_TARGET_OFFSET);
  832  	writeq(s->size / PAGE_SIZE, base + IDMA_BATCH_PAGES_OFFSET);
  833  	writeq(1, base + IDMA_BATCH_CONTROL_OFFSET);
  834  	s->batch_configured = true;
  835  	s->next_offset = 0;
  836  	return readq(base + IDMA_BATCH_CONTROL_OFFSET) == 1 ? 0 : -EIO;
  837  }
  838  
  839  static void idma_batch_stop(struct idma_stream *s)
  840  {
  841  	if (s && s->batch_configured && s->batch_started)
  842  		writeq(2, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
  843  }
  844  
  845  static int idma_batch_collect(struct idma_stream *s)
  846  {
  847  	u64 status, issued, completed;
  848  
  849  	if (!s || !s->batch_configured)
  850  		return 0;
  851  	if (s->retain)
  852  		return -EIO;
  853  	status = readq(s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
  854  	if (status & 2) {
  855  		idma_quarantine(s);
  856  		return -EIO;
  857  	}
  858  	if (s->batch_started) {
  859  		issued = readq(s->idma->reg + IDMA_BATCH_ISSUED_OFFSET);
  860  		completed = readq(s->idma->reg + IDMA_BATCH_COMPLETED_OFFSET);
  861  		s->stats->submitted = issued;
  862  		s->stats->completed = completed;
  863  		s->stats->completed_bytes = completed * PAGE_SIZE;
  864  		if (s->reverse)
  865  			s->touched_tail = max(s->touched_tail,
  866  				      (size_t)min(completed, (u64)(s->size / PAGE_SIZE)) * PAGE_SIZE);
  867  		else
  868  			s->touched_end = max(s->touched_end,
  869  				     (size_t)min(completed, (u64)(s->size / PAGE_SIZE)) * PAGE_SIZE);
  870  		if (issued != completed || issued > IDMA_EXPERIMENT_MAX_TRANSFERS)
  871  			return -EIO;
  872  	}
  873  	return status >> 32 ? -EIO : 0;
  874  }
  875  
  876  static void idma_batch_disable(struct idma_stream *s)
  877  {
  878  	if (s && s->batch_configured && !s->retain)
  879  		writeq(0, s->idma->reg + IDMA_BATCH_CONTROL_OFFSET);
  880  }
```

## observer每次准备清理旧结果

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:986–1027`

```c
  986  static int idma_observation_prepare(struct idma_observation *o, bool enabled)
  987  {
  988  	u64 magic;
  989  
  990  	if (!enabled)
  991  		return 0;
  992  	o->sequence = ++idma_observation_sequence;
  993  	o->control = idma_get_instance(0);
  994  	if (!o->control)
  995  		return -ENODEV;
  996  	magic = readq(o->control->reg + IDMA_OBS_MAGIC_OFFSET);
  997  	/* V9 retains window accounting but physically omits the old path timers. */
  998  	o->path = magic != IDMA_OBS_MAGIC_V9 &&
  999  		(o->p4m || o->e3 || (o->batch && READ_ONCE(idma_path_observe)));
 1000  	if (o->p4m && magic != IDMA_OBS_MAGIC_V9 &&
 1001  	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
 1002  		return -EOPNOTSUPP;
 1003  	if (o->controlled && magic != IDMA_OBS_MAGIC_V6 &&
 1004  	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
 1005  		return -EOPNOTSUPP;
 1006  	if (o->queue_pair && magic != IDMA_OBS_MAGIC_V5 &&
 1007  	    magic != IDMA_OBS_MAGIC_V6 && magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
 1008  		return -EOPNOTSUPP;
 1009  	o->e3_magic = (magic == IDMA_OBS_MAGIC_V5 || magic == IDMA_OBS_MAGIC_V6 ||
 1010  		       magic == IDMA_OBS_MAGIC_V8 || magic == IDMA_OBS_MAGIC_V7) ? IDMA_E3_MAGIC_V2 : IDMA_E3_MAGIC;
 1011  	if (o->e3 && magic != IDMA_OBS_MAGIC_V4 &&
 1012  	    magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V6 &&
 1013  	    magic != IDMA_OBS_MAGIC_V8 && magic != IDMA_OBS_MAGIC_V7)
 1014  		return -EOPNOTSUPP;
 1015  	if (o->control->did != 10 ||
 1016  	    (magic != IDMA_OBS_MAGIC_V9 && magic != IDMA_OBS_MAGIC_V8 &&
 1017  	     magic != IDMA_OBS_MAGIC_V7 && magic != IDMA_OBS_MAGIC_V6 &&
 1018  	     magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V4 &&
 1019  	     magic != IDMA_OBS_MAGIC_V3 &&
 1020  	     (o->path || (magic != IDMA_OBS_MAGIC_V2 && (o->batch || magic != IDMA_OBS_MAGIC)))))
 1021  		return -EOPNOTSUPP;
 1022  	/* Clear previous frozen results; never take ownership of an active session. */
 1023  	if (readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) & 3)
 1024  		return -EBUSY;
 1025  	writeq(4, o->control->reg + IDMA_OBS_CONTROL_OFFSET);
 1026  	return readq(o->control->reg + IDMA_OBS_CONTROL_OFFSET) ? -EIO : 0;
 1027  }
```

## E3 reseed保留mapping却重写buffer

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:1243–1257`

```c
 1243  /* Called only after both devices have drained; mappings remain allocated. */
 1244  static void idma_stream_reseed(struct idma_stream *s)
 1245  {
 1246  	size_t i;
 1247  
 1248  	if (!s)
 1249  		return;
 1250  	for (i = 0; i < s->size / sizeof(u64); i++) {
 1251  		((u64 *)s->src)[i] = idma_pattern(s, i);
 1252  		((u64 *)s->dst)[i] = ~idma_pattern(s, i);
 1253  	}
 1254  	s->offset = s->next_offset = s->touched_end = s->touched_tail = 0;
 1255  	s->transfer_id = s->deadline_ns = s->last_complete_ns = 0;
 1256  	s->batch_configured = s->batch_started = false;
 1257  }
```

## 合法composition承载的P4M验证

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:1313–1339`

```c
 1313  static int idma_p4m_validate(struct idma_device *victim,
 1314  			     const struct idma_p4m *p)
 1315  {
 1316  	const struct idma_experiment *e = &p->experiment;
 1317  	bool has_a;
 1318  
 1319  	if (victim->id != 0 || victim->did != 10)
 1320  		return -EINVAL;
 1321  	if (memchr_inv(p->reserved, 0, sizeof(p->reserved)))
 1322  		return -EINVAL;
 1323  	if (e->abi_version != IDMA_EXPERIMENT_ABI || e->struct_size != sizeof(*e))
 1324  		return -EPROTO;
 1325  	if (e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
 1326  		return -EINVAL;
 1327  	has_a = e->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
 1328  	if (has_a && e->aggressor_id != 1)
 1329  		return -EINVAL;
 1330  	if (!has_a && p->aggressor_gap_cycles)
 1331  		return -EINVAL;
 1332  	if (e->chunk_bytes != PAGE_SIZE)
 1333  		return -EINVAL;
 1334  	if (!e->victim_transfers || e->victim_transfers > IDMA_BATCH_MAX_TRANSFERS)
 1335  		return -EINVAL;
 1336  	if (e->warmup_transfers > IDMA_EXPERIMENT_MAX_TRANSFERS)
 1337  		return -EINVAL;
 1338  	return 0;
 1339  }
```

## mode configure检查/ack；硬件语义交root

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2274–2307`

```c
 2274  /* New, independent hardware bank. All calls hold idma_experiment_lock. */
 2275  #define IDMA_COMPOSITION_BASE (512U + 0xd20U)
 2276  
 2277  static u64 idma_composition_read(struct idma_device *d, unsigned int word)
 2278  {
 2279  	writeq(IDMA_COMPOSITION_BASE + word, d->reg + IDMA_OBS_INDEX_OFFSET);
 2280  	return readq(d->reg + IDMA_OBS_DATA_OFFSET);
 2281  }
 2282  
 2283  static int idma_composition_configure(struct idma_device *d, u32 mode, u32 did,
 2284  				      u64 *generation)
 2285  {
 2286  	u64 old, now, status, config = ((u64)did << 8) | mode;
 2287  
 2288  	if (idma_composition_read(d, 0) != IDMA_COMPOSITION_MAGIC ||
 2289  	    idma_composition_read(d, 1) != IDMA_COMPOSITION_ABI)
 2290  		return -EOPNOTSUPP;
 2291  	old = idma_composition_read(d, 3);
 2292  	if (old == U64_MAX)
 2293  		return -EOVERFLOW;
 2294  	if (idma_composition_read(d, 5) != 1)
 2295  		return -EBUSY;
 2296  	writeq(IDMA_COMPOSITION_BASE + 2, d->reg + IDMA_OBS_INDEX_OFFSET);
 2297  	writeq(config, d->reg + IDMA_OBS_DATA_OFFSET);
 2298  	/* Same-clock atomic clear/config/ack: ordered read observes that edge. */
 2299  	status = idma_composition_read(d, 4);
 2300  	now = idma_composition_read(d, 3);
 2301  	if (status == 1)
 2302  		return -EBUSY;
 2303  	if (status || now != old + 1 || idma_composition_read(d, 2) != config)
 2304  		return -EIO;
 2305  	*generation = now;
 2306  	return 0;
 2307  }
```

## capture与restore

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2309–2355`

```c
 2309  static int idma_composition_capture(struct idma_device *d,
 2310  				    struct idma_composition *p, int error)
 2311  {
 2312  	u64 config = ((u64)p->protected_did << 8) | p->mode;
 2313  	unsigned int i;
 2314  
 2315  	for (i = 0; i < IDMA_COMPOSITION_WORDS; i++)
 2316  		p->raw[i] = idma_composition_read(d, i);
 2317  	p->raw_captured = 1;
 2318  	if (error)
 2319  		return error;
 2320  	if (p->raw[0] != IDMA_COMPOSITION_MAGIC || p->raw[1] != 1 ||
 2321  	    p->raw[2] != config || p->raw[3] != p->generation ||
 2322  	    p->raw[6] != 4 || p->raw[9] || p->raw[10] != config ||
 2323  	    p->raw[11] != p->generation || !p->raw[7] ||
 2324  	    p->raw[8] != p->p4m.window[1] || p->raw[34] != 3)
 2325  		return -EIO;
 2326  	for (i = 12; i <= 18; i += 3)
 2327  		if (p->raw[i] != p->raw[i + 1] + p->raw[i + 2])
 2328  			return -EIO;
 2329  	if (p->raw[24] > p->raw[8] || p->raw[25] ||
 2330  	    (!p->victim_gap_cycles && p->raw[24]) ||
 2331  	    (p->victim_gap_cycles && p->p4m.experiment.victim_transfers > 1 &&
 2332  	     !p->raw[24]) || p->raw[30] || p->raw[32] || p->raw[33])
 2333  		return -EIO;
 2334  	/* Reserved words are an ABI check, not experimental outcome filtering. */
 2335  	if (p->raw[35])
 2336  		return -EIO;
 2337  	for (i = 40; i < 64; i++)
 2338  		if (p->raw[i])
 2339  			return -EPROTO;
 2340  	return 0;
 2341  }
 2342  
 2343  static int idma_composition_restore(struct idma_device *d,
 2344  				    struct idma_composition *p)
 2345  {
 2346  	u64 deadline = ktime_get_ns() + NSEC_PER_SEC;
 2347  
 2348  	/* Never force a cache clear across an unfinished DMA or response. */
 2349  	while (idma_composition_read(d, 5) != 1) {
 2350  		if (ktime_get_ns() >= deadline)
 2351  			return -ETIMEDOUT;
 2352  		cond_resched();
 2353  	}
 2354  	return idma_composition_configure(d, 0, 10, &p->restored_generation);
 2355  }
```

## 每调用局部stream与上锁

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2357–2396`

```c
 2357  static long idma_run_session2(struct idma_device *victim,
 2358  			      struct idma_experiment *request, bool batch, struct idma_e3_pair *pair,
 2359  			      struct idma_e3_diag_round *diagnostic, bool queue_pair,
 2360  			      struct idma_p4m *p4m, struct idma_research *research,
 2361  			      const struct idma_research2_config *cfg,
 2362  			      struct idma_composition *composition)
 2363  {
 2364  	struct idma_experiment e = *request;
 2365  	struct idma_stream streams[2] = {};
 2366  	struct idma_observation observation = { .batch = batch, .e3 = !!pair, .queue_pair = queue_pair,
 2367  		.delay128 = pair && pair->first_delay == 128, .p4m = !!p4m };
 2368  	struct idma_stream *v = &streams[0], *a = NULL;
 2369  	struct idma_batch_start batch_start = {};
 2370  	struct idma_device *aggressor = NULL;
 2371  	struct riscv_iommu_hpm_session *hpm = NULL;
 2372  	struct riscv_iommu_hpm_sample before, after;
 2373  	struct idma_e3_diag_round *diag = NULL;
 2374  	u64 active_before[2], active_after[2];
 2375  	u64 start, end = 0, deadline, stamp;
 2376  	int ret, drain_ret, verify_ret, observation_ret, p4m_ret;
 2377  	unsigned int round = 0;
 2378  	bool comp_trace = composition != NULL &&
 2379  		READ_ONCE(idma_composition_trace);
 2380  
 2381  	memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
 2382  	e.victim_did = victim->did;
 2383  	e.aggressor_did = IDMA_EXPERIMENT_NO_AGGRESSOR;
 2384  	ret = idma_validate(&e);
 2385  	if (!ret && batch && (e.chunk_bytes != PAGE_SIZE ||
 2386  			      e.victim_transfers > IDMA_BATCH_MAX_TRANSFERS))
 2387  		ret = -EINVAL;
 2388  	if (ret)
 2389  		goto out_copy;
 2390  	ret = mutex_lock_interruptible(&idma_experiment_lock);
 2391  	if (ret)
 2392  		goto out_copy;
 2393  	if (idma_any_quarantined()) {
 2394  		ret = -EIO;
 2395  		goto out_unlock;
 2396  	}
```

## preflight、configure在stream_prepare之前

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2418–2458`

```c
 2418  	if (p4m) {
 2419  		ret = idma_gap_preflight(victim, aggressor);
 2420  		if (ret)
 2421  			goto out_put;
 2422  	}
 2423  	ret = idma_observation_prepare(&observation, batch || READ_ONCE(idma_observe));
 2424  	if (ret)
 2425  		goto out_put;
 2426  	if (batch && (readq(victim->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC ||
 2427  	    (aggressor && readq(aggressor->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC))) {
 2428  		ret = -EOPNOTSUPP;
 2429  		goto out_put;
 2430  	}
 2431  	if (batch && ((readq(victim->reg + IDMA_BATCH_CONTROL_OFFSET) & 2) ||
 2432  	    (aggressor && (readq(aggressor->reg + IDMA_BATCH_CONTROL_OFFSET) & 2)))) {
 2433  		ret = -EBUSY;
 2434  		goto out_put;
 2435  	}
 2436  	if (composition) {
 2437  		idma_comp_stage(comp_trace, victim->miscdev.parent,
 2438  				"pre-config", 0);
 2439  		ret = idma_composition_configure(victim, composition->mode,
 2440  				composition->protected_did, &composition->generation);
 2441  		idma_comp_stage(comp_trace, victim->miscdev.parent,
 2442  				"post-config", ret);
 2443  		if (ret)
 2444  			goto out_put;
 2445  		composition->configured = 1;
 2446  		composition->restore_status = -EINPROGRESS;
 2447  	}
 2448  	ret = idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
 2449  	if (ret)
 2450  		goto out_cleanup;
 2451  	v->composition_trace = comp_trace;
 2452  	if (a) {
 2453  		ret = idma_stream_prepare(a, aggressor, e.aggressor_pages,
 2454  					  e.chunk_bytes, &e.aggressor);
 2455  		if (ret)
 2456  			goto out_cleanup;
 2457  		a->reverse = cfg && cfg->reverse;
 2458  	}
```

## HPM/flush/warm请求

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2484–2507`

```c
 2484  	if (pair)
 2485  		pair->rounds_attempted++;
 2486  	ret = riscv_iommu_hpm_acquire(v->dev, a ? a->dev : NULL, &hpm);
 2487  	if (ret)
 2488  		goto out_cleanup;
 2489  	e.hpm_configured = 1;
 2490  	/*
 2491  	 * Platform workaround: this CVA6 has no DMA snoop of the WT data cache.
 2492  	 * Its FENCE.I drains CPU writes and invalidates D-cache as well as I-cache.
 2493  	 * Hand initialized buffers to DMA before warmup; never flush per transfer.
 2494  	 */
 2495  	local_flush_icache_all();
 2496  	deadline = ktime_get_ns() + (u64)e.run_timeout_ms * NSEC_PER_MSEC;
 2497  	if (diag) {
 2498  		idma_snapshot(hpm, v, a, &before, active_before);
 2499  		diag->warm_begin_ns = ktime_get_ns();
 2500  	}
 2501  	idma_comp_stage(v->composition_trace, v->dev, "warm-start", 0);
 2502  	if (a && (!batch || e.warmup_transfers)) {
 2503  		ret = idma_submit(a, false);
 2504  		if (ret)
 2505  			goto out_drain;
 2506  	}
 2507  	ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);
```

## warm后A drain及batch配置

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2519–2559`

```c
 2519  	idma_comp_stage(v->composition_trace, v->dev, "warm-done", ret);
 2520  	if (ret)
 2521  		goto out_drain;
 2522  	/* Every measured V task starts at the same address after warmup. */
 2523  	v->next_offset = 0;
 2524  	v->last_complete_ns = 0;
 2525  	if (a)
 2526  		a->last_complete_ns = 0;
 2527  	if (batch) {
 2528  		/* Software warmup is drained before enabling the autonomous mode. */
 2529  		if (a) {
 2530  			ret = idma_drain(a);
 2531  			if (ret)
 2532  				goto out_drain;
 2533  		}
 2534  		if (diag) {
 2535  			diag->warm_drained_ns = ktime_get_ns();
 2536  			idma_snapshot(hpm, v, a, &after, active_after);
 2537  			idma_snapshot_delta(&diag->lane[0].warm_hpm, 0, &before, &after,
 2538  					    active_before[0], active_after[0]);
 2539  			if (a)
 2540  				idma_snapshot_delta(&diag->lane[1].warm_hpm, 1, &before, &after,
 2541  						    active_before[1], active_after[1]);
 2542  			diag->warm_complete = 1;
 2543  		}
 2544  		ret = idma_batch_configure(v, e.victim_transfers);
 2545  		if (!ret && a)
 2546  			ret = idma_batch_configure(a, IDMA_EXPERIMENT_MAX_TRANSFERS);
 2547  		idma_comp_stage(v->composition_trace, v->dev,
 2548  				"batch-config-done", ret);
 2549  		if (ret)
 2550  			goto out_drain;
 2551  	}
 2552  	if (p4m) {
 2553  		ret = idma_gap_program_readback(v, composition ? composition->victim_gap_cycles : 0);
 2554  		if (!ret && a)
 2555  			ret = idma_gap_program_readback(a,
 2556  							p4m->aggressor_gap_cycles);
 2557  		idma_comp_stage(v->composition_trace, v->dev, "gap-done", ret);
 2558  		if (ret)
 2559  			goto out_drain;
```

## 测量时间边界与STOP

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2579–2612`

```c
 2579  	ret = idma_observation_arm(&observation, victim, aggressor, e.victim_transfers);
 2580  	idma_comp_stage(v->composition_trace, v->dev, "arm-done", ret);
 2581  	if (ret)
 2582  		goto out_drain;
 2583  	stamp = ktime_get_ns();
 2584  	if (diag)
 2585  		diag->arm_end_ns = stamp;
 2586  	idma_snapshot(hpm, v, a, &before, active_before);
 2587  	start = ktime_get_ns();
 2588  	e.snapshot_before_ns = start - stamp;
 2589  	e.measurement_started = 1;
 2590  	idma_comp_stage(v->composition_trace, v->dev, "prewindow", 0);
 2591  	ret = batch ? idma_batch_window(v, a, deadline, &end, &batch_start) :
 2592  		idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
 2593  	e.victim_elapsed_ns = end - start;
 2594  	e.measurement_complete = !ret;
 2595  	idma_comp_stage(v->composition_trace, v->dev, "window-return", ret);
 2596  	e.victim.in_flight_at_end = v->in_flight;
 2597  	e.aggressor.in_flight_at_end = a && a->in_flight;
 2598  	/* Stop autonomous A after V observation; hardware window is already frozen. */
 2599  	if (batch) {
 2600  		idma_batch_stop(a);
 2601  		if (ret)
 2602  			idma_batch_stop(v);
 2603  	}
 2604  	/* No more software A refills after V's final observed completion. */
 2605  	idma_snapshot(hpm, v, a, &after, active_after);
 2606  	e.snapshot_after_ns = ktime_get_ns() - end;
 2607  	idma_snapshot_delta(&e.victim, 0, &before, &after,
 2608  			    active_before[0], active_after[0]);
 2609  	if (a)
 2610  		idma_snapshot_delta(&e.aggressor, 1, &before, &after,
 2611  				    active_before[1], active_after[1]);
 2612  	idma_comp_stage(v->composition_trace, v->dev, "snapshot-after", ret);
```

## drain计时与CPU visibility/verify

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2613–2654`

```c
 2613   out_drain:
 2614  	idma_comp_stage(v->composition_trace, v->dev, "drain-start", ret);
 2615  	if (batch) {
 2616  		idma_batch_stop(v);
 2617  		idma_batch_stop(a);
 2618  	}
 2619  	if (cfg && ret)
 2620  		idma_research2_method_abort(victim, cfg, ret);
 2621  	stamp = ktime_get_ns();
 2622  	drain_ret = idma_drain(v);
 2623  	if (drain_ret && !ret)
 2624  		ret = drain_ret;
 2625  	if (a) {
 2626  		drain_ret = idma_drain(a);
 2627  		if (drain_ret && !ret)
 2628  			ret = drain_ret;
 2629  	}
 2630  	e.drain_ns = ktime_get_ns() - stamp;
 2631  	if (batch) {
 2632  		drain_ret = idma_batch_collect(v);
 2633  		if (drain_ret && !ret)
 2634  			ret = drain_ret;
 2635  		drain_ret = idma_batch_collect(a);
 2636  		if (drain_ret && !ret)
 2637  			ret = drain_ret;
 2638  		if (!ret && e.victim.completed != e.victim_transfers)
 2639  			ret = -EIO;
 2640  	}
 2641  	idma_comp_stage(v->composition_trace, v->dev, "drain-end", ret);
 2642  	riscv_iommu_hpm_release(hpm);
 2643  	if (!ret) {
 2644  		/* Both lanes are drained: reacquire CPU visibility before any check. */
 2645  		local_flush_icache_all();
 2646  		verify_ret = idma_verify(v);
 2647  		if (verify_ret)
 2648  			ret = verify_ret;
 2649  		if (a) {
 2650  			verify_ret = idma_verify(a);
 2651  			if (verify_ret && !ret)
 2652  				ret = verify_ret;
 2653  		}
 2654  	}
```

## finisher、capture、restore

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2655–2664`

```c
 2655   out_cleanup:
 2656  	observation_ret = idma_observation_finish(&observation, &e, ret);
 2657  	if (observation_ret && !ret)
 2658  		ret = observation_ret;
 2659  	idma_comp_stage(v->composition_trace, v->dev, "obs-finish", ret);
 2660  	if (p4m) {
 2661  		p4m_ret = idma_p4m_finish(&observation, &e, p4m, ret);
 2662  		if (p4m_ret && !ret)
 2663  			ret = p4m_ret;
 2664  	}
```

## batch disable及composition capture/restore

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2688–2702`

```c
 2688  	if (batch) {
 2689  		idma_batch_disable(v);
 2690  		idma_batch_disable(a);
 2691  	}
 2692  	if (composition && composition->configured) {
 2693  		int capture_ret = idma_composition_capture(victim, composition, ret);
 2694  
 2695  		if (!ret)
 2696  			ret = capture_ret;
 2697  		composition->restore_status = idma_composition_restore(victim, composition);
 2698  		if (!ret)
 2699  			ret = composition->restore_status;
 2700  		idma_comp_stage(v->composition_trace, v->dev, "restored",
 2701  				composition->restore_status);
 2702  	}
```

## E3两轮分支与正常cleanup/unlock

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2715–2748`

```c
 2715  		if (!ret)
 2716  			pair->rounds_completed++;
 2717  		if (!ret && round == 0) {
 2718  			round = 1;
 2719  			idma_stream_reseed(v);
 2720  			idma_stream_reseed(a);
 2721  			memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
 2722  			e.victim_did = victim->did;
 2723  			e.aggressor_did = a ? aggressor->did : IDMA_EXPERIMENT_NO_AGGRESSOR;
 2724  			idma_put_instance(observation.control);
 2725  			observation = (struct idma_observation) { .batch = true, .e3 = true,
 2726  				.delay128 = (!diagnostic || queue_pair) && pair->first_delay == 0,
 2727  				.queue_pair = queue_pair };
 2728  			ret = idma_observation_prepare(&observation, true);
 2729  			if (ret)
 2730  				goto out_cleanup;
 2731  			goto run_round;
 2732  		}
 2733  	} else if (batch) {
 2734  		idma_batch_start_log(victim->miscdev.parent, &batch_start,
 2735  				     observation.sequence, ret);
 2736  	}
 2737  	if (a)
 2738  		idma_stream_cleanup(a);
 2739  	idma_stream_cleanup(v);
 2740   out_put:
 2741  	idma_put_instance(observation.control);
 2742  	idma_put_instance(aggressor);
 2743   out_unlock:
 2744  	mutex_unlock(&idma_experiment_lock);
 2745   out_copy:
 2746  	e.status = ret;
 2747  	*request = e;
 2748  	return ret;
```

## 普通session wrapper不提供composition

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2751–2758`

```c
 2751  static long idma_run_session(struct idma_device *victim,
 2752  			     struct idma_experiment *request, bool batch, struct idma_e3_pair *pair,
 2753  			     struct idma_e3_diag_round *diagnostic, bool queue_pair,
 2754  			     struct idma_p4m *p4m, struct idma_research *research)
 2755  {
 2756  	return idma_run_session2(victim, request, batch, pair, diagnostic,
 2757  				 queue_pair, p4m, research, NULL, NULL);
 2758  }
```

## composition实际入口/权限/请求/调用/输出释放

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2780–2817`

```c
 2780  static long idma_run_composition(struct idma_device *victim, void __user *uarg)
 2781  {
 2782  	struct idma_composition *p;
 2783  	struct idma_experiment *e;
 2784  	long ret;
 2785  
 2786  	if (!capable(CAP_SYS_RAWIO))
 2787  		return -EPERM;
 2788  	p = memdup_user(uarg, sizeof(*p));
 2789  	if (IS_ERR(p))
 2790  		return PTR_ERR(p);
 2791  	memset(&p->status, 0, sizeof(*p) - offsetof(struct idma_composition, status));
 2792  	idma_p4m_clear_outputs(&p->p4m);
 2793  	e = &p->p4m.experiment;
 2794  	ret = -EPROTO;
 2795  	if (p->abi_version != IDMA_COMPOSITION_ABI || p->struct_size != sizeof(*p))
 2796  		goto out;
 2797  	ret = -EINVAL;
 2798  	if (p->mode > 3 || (p->protected_did != 10 && p->protected_did != 11) ||
 2799  	    (p->victim_gap_cycles != 0 && p->victim_gap_cycles != 1000 &&
 2800  	     p->victim_gap_cycles != 4000) ||
 2801  	    memchr_inv(p->reserved, 0, sizeof(p->reserved)) ||
 2802  	    p->p4m.aggressor_gap_cycles || e->victim_pages != 1 ||
 2803  	    (e->aggressor_pages != 1 && e->aggressor_pages != 8 && e->aggressor_pages != 16) ||
 2804  	    e->victim_transfers > 4096 || e->warmup_transfers > 4096)
 2805  		goto out;
 2806  	ret = idma_p4m_validate(victim, &p->p4m);
 2807  	if (!ret)
 2808  		ret = idma_run_session2(victim, e, true, NULL, NULL, false,
 2809  				       &p->p4m, NULL, NULL, p);
 2810   out:
 2811  	p->status = ret;
 2812  	e->status = ret;
 2813  	if (copy_to_user(uarg, p, sizeof(*p)))
 2814  		ret = -EFAULT;
 2815  	kfree(p);
 2816  	return ret;
 2817  }
```

## E3入口不传composition

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2831–2866`

```c
 2831  static long idma_run_e3_pair(struct idma_device *victim, void __user *uarg, unsigned int mode)
 2832  {
 2833  	bool diagnostic = mode != 0, queue_pair = mode == 2;
 2834  	struct idma_e3_pair *p;
 2835  	struct idma_e3_diag *d;
 2836  	size_t size = diagnostic ? sizeof(*d) : sizeof(*p);
 2837  	long ret;
 2838  
 2839  	p = kzalloc(size, GFP_KERNEL);
 2840  	d = (struct idma_e3_diag *)p;
 2841  	if (!p)
 2842  		return -ENOMEM;
 2843  	if (copy_from_user(p, uarg, size)) {
 2844  		ret = -EFAULT;
 2845  		goto out_free;
 2846  	}
 2847  	memset(&p->status, 0, size - offsetof(struct idma_e3_pair, status));
 2848  	if (p->abi_version != 1 || p->struct_size != sizeof(*p) || p->reserved ||
 2849  	    (p->first_delay != 0 && ((diagnostic && !queue_pair) || p->first_delay != 128)) || victim->id != 0 ||
 2850  	    (p->request.aggressor_id != 1 && p->request.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR) ||
 2851  	    p->request.victim_pages != 1 ||
 2852  	    (p->request.aggressor_pages != 1 && p->request.aggressor_pages != 8) ||
 2853  	    p->request.chunk_bytes != 4096 || p->request.victim_transfers > 4096 ||
 2854  	    p->request.warmup_transfers > 4096 || p->request.run_timeout_ms > 60000) {
 2855  		ret = -EINVAL;
 2856  	} else {
 2857  		struct idma_experiment e = p->request;
 2858  
 2859  		ret = idma_run_session(victim, &e, true, p, diagnostic ? d->diagnostic : NULL, queue_pair, NULL, NULL);
 2860  	}
 2861  	p->status = ret;
 2862  	if (copy_to_user(uarg, p, size))
 2863  		ret = -EFAULT;
 2864   out_free:
 2865  	kfree(p);
 2866  	return ret;
```

## ioctl分派与fops

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2875–2937`

```c
 2875  #include "idma_research.h"
 2876  #include "idma_sva_task.h"
 2877  
 2878  static long idma_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
 2879  {
 2880  	struct idma_device *idma = fp->private_data;
 2881  	struct idma_experiment_caps caps = {
 2882  		.abi_version = IDMA_EXPERIMENT_ABI,
 2883  		.struct_size = sizeof(caps),
 2884  		.capabilities = IDMA_EXPERIMENT_CAP_SW_REFILL | IDMA_EXPERIMENT_CAP_HW_BATCH |
 2885  				IDMA_EXPERIMENT_CAP_E3_PAIR | IDMA_EXPERIMENT_CAP_E3_DIAG |
 2886  				IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR |
 2887  				IDMA_EXPERIMENT_CAP_E3_CONTROLLED |
 2888  				IDMA_EXPERIMENT_CAP_P4M | IDMA_EXPERIMENT_CAP_RESEARCH,
 2889  		.device_id = idma->id,
 2890  		.did = idma->did,
 2891  		.max_pages = IDMA_MAX_PAGES,
 2892  		.page_bytes = PAGE_SIZE,
 2893  		.max_run_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS,
 2894  	};
 2895  
 2896  	switch (cmd) {
 2897  	case IDMA_IOC_SVA_CAPS:
 2898  		return idma_sva_caps_ioctl(idma, (void __user *)arg);
 2899  	case IDMA_IOC_SVA_RUN:
 2900  		return idma_sva_run_ioctl(idma, (void __user *)arg);
 2901  	case IDMA_IOC_RUN_RESEARCH:
 2902  		return idma_run_research(idma, (void __user *)arg);
 2903  	case IDMA_IOC_RUN_RESEARCH2:
 2904  		return idma_run_research2(idma, (void __user *)arg);
 2905  	case IDMA_IOC_RESEARCH2_CAPS:
 2906  		return idma_research2_caps_ioctl(idma, (void __user *)arg);
 2907  	case IDMA_IOC_RUN_E3_CONTROLLED:
 2908  		return idma_run_e3_controlled(idma, (void __user *)arg);
 2909  	case IDMA_IOC_RUN_COMPOSITION:
 2910  		return idma_run_composition(idma, (void __user *)arg);
 2911  	case IDMA_IOC_RUN_P4M:
 2912  		return idma_run_p4m(idma, (void __user *)arg);
 2913  	case IDMA_IOC_RUN_E3_QUEUE_PAIR:
 2914  		return idma_run_e3_pair(idma, (void __user *)arg, 2);
 2915  	case IDMA_IOC_RUN_E3_DIAG:
 2916  		return idma_run_e3_pair(idma, (void __user *)arg, 1);
 2917  	case IDMA_IOC_RUN_E3_PAIR:
 2918  		return idma_run_e3_pair(idma, (void __user *)arg, 0);
 2919  	case IDMA_IOC_EXPERIMENT_CAPS:
 2920  		if (riscv_iommu_hpm_supported(idma->miscdev.parent))
 2921  			caps.capabilities |= IDMA_EXPERIMENT_CAP_DID_HPM;
 2922  		return copy_to_user((void __user *)arg, &caps, sizeof(caps)) ? -EFAULT : 0;
 2923  	case IDMA_IOC_RUN_BATCH:
 2924  		return idma_run_experiment_mode(idma, (struct idma_experiment __user *)arg, true);
 2925  	case IDMA_IOC_RUN_EXPERIMENT:
 2926  		return idma_run_experiment(idma, (struct idma_experiment __user *)arg);
 2927  	default:
 2928  		return -ENOTTY;
 2929  	}
 2930  }
 2931  
 2932  static const struct file_operations idma_fops = {
 2933  	.owner = THIS_MODULE,
 2934  	.open = idma_open,
 2935  	.release = idma_release,
 2936  	.unlocked_ioctl = idma_ioctl,
 2937  };
```

## 设备probe与remove的寿命

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma.c:2939–3005`

```c
 2939  static int idma_probe(struct platform_device *pdev)
 2940  {
 2941  	struct device *dev = &pdev->dev;
 2942  	struct idma_device *idma_dev;
 2943  	struct resource *res;
 2944  	int ret;
 2945  
 2946  	idma_dev = devm_kzalloc(dev, sizeof(*idma_dev), GFP_KERNEL);
 2947  	if (!idma_dev)
 2948  		return -ENOMEM;
 2949  
 2950  	idma_dev->id = idma_reserve_instance_id();
 2951  	if (idma_dev->id < 0)
 2952  		return idma_dev->id;
 2953  
 2954  	snprintf(idma_dev->name, sizeof(idma_dev->name), "idma%d", idma_dev->id);
 2955  
 2956  	idma_dev->miscdev.fops = &idma_fops;
 2957  	idma_dev->miscdev.parent = dev;
 2958  	idma_dev->miscdev.minor = MISC_DYNAMIC_MINOR;
 2959  	idma_dev->miscdev.name = idma_dev->name;
 2960  
 2961  	idma_dev->reg = devm_platform_get_and_ioremap_resource(pdev, 0, &res);
 2962  	if (IS_ERR(idma_dev->reg)) {
 2963  		ret = dev_err_probe(dev, PTR_ERR(idma_dev->reg),
 2964  				    "could not map iDMA registers\n");
 2965  		goto err_unregister_id;
 2966  	}
 2967  
 2968  	ret = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(64));
 2969  	if (ret) {
 2970  		dev_err(dev, "no suitable DMA mask\n");
 2971  		goto err_unregister_id;
 2972  	}
 2973  
 2974  	ret = idma_read_did(pdev, &idma_dev->did);
 2975  	if (ret)
 2976  		goto err_unregister_id;
 2977  	dev_set_drvdata(dev, idma_dev);
 2978  
 2979  	ret = misc_register(&idma_dev->miscdev);
 2980  	if (ret) {
 2981  		dev_err(dev, "could not register misc device\n");
 2982  		goto err_unregister_id;
 2983  	}
 2984  
 2985  	idma_publish_instance(idma_dev);
 2986  	dev_info(dev, "registered %s\n", idma_dev->name);
 2987  
 2988  	return 0;
 2989  
 2990  err_unregister_id:
 2991  	idma_unregister_instance(idma_dev);
 2992  	return ret;
 2993  }
 2994  
 2995  static int idma_remove(struct platform_device *pdev)
 2996  {
 2997  	struct idma_device *idma_dev = dev_get_drvdata(&pdev->dev);
 2998  
 2999  	/* Do not release devm MMIO while a synchronous task owns the devices. */
 3000  	mutex_lock(&idma_experiment_lock);
 3001  	idma_unregister_instance(idma_dev);
 3002  	misc_deregister(&idma_dev->miscdev);
 3003  	mutex_unlock(&idma_experiment_lock);
 3004  
 3005  	return 0;
```

## 实验输入不含用户buffer/pool句柄

`/home/xvhaocheng/work/iommu-platform/linux/include/uapi/linux/idma.h:54–91`

```c
   54  struct idma_experiment {
   55  	/* Input: all reserved words and flags must be zero in ABI 1. */
   56  	__u32 abi_version;
   57  	__u32 struct_size;
   58  	__u32 flags;
   59  	__u32 aggressor_id;
   60  	__u32 victim_pages;
   61  	__u32 aggressor_pages;
   62  	__u32 chunk_bytes;
   63  	__u32 run_timeout_ms;
   64  	__u64 victim_transfers;
   65  	__u64 warmup_transfers;
   66  	__u32 reserved[8];
   67  	/* Output: status is a negative errno or zero, even when ioctl fails. */
   68  	__s32 status;
   69  	__u32 victim_did;
   70  	__u32 aggressor_did;
   71  	__u32 hpm_configured;
   72  	__u32 measurement_started;
   73  	__u32 measurement_complete;
   74  	__u64 victim_elapsed_ns;
   75  	/* Snapshot intervals bracket V's window; HPM is not an atomic snapshot. */
   76  	__u64 snapshot_before_ns;
   77  	__u64 snapshot_after_ns;
   78  	__u64 drain_ns;
   79  	struct idma_experiment_stats victim;
   80  	struct idma_experiment_stats aggressor;
   81  };
   82  
   83  /* Old commands 0x01..0x05 are retired and return ENOTTY. */
   84  #define IDMA_IOC_EXPERIMENT_CAPS _IOR('i', 0x06, struct idma_experiment_caps)
   85  #define IDMA_IOC_RUN_EXPERIMENT _IOWR('i', 0x07, struct idma_experiment)
   86  
   87  /* Separate operation/JSON semantics, same validated input layout; reserved=0.
   88   * Batch stats count hardware tasks; refill gaps are not sampled. A counts include
   89   * its startup and final drain, rather than just the V hardware window.
   90   */
   91  #define IDMA_IOC_RUN_BATCH _IOWR('i', 0x08, struct idma_experiment)
```

## E3单次映射两轮合同

`/home/xvhaocheng/work/iommu-platform/linux/include/uapi/linux/idma.h:93–110`

```c
   93  #define IDMA_E3_WORDS 256U
   94  /* One allocation/mapping lifetime, two separately warmed/drained/verified rounds.
   95   * first_delay is 0 or 128; the other value is used for the second round.
   96   * Raw words are versioned observer-v4 bank indices 192..447, read after drain.
   97   */
   98  struct idma_e3_pair {
   99  	__u32 abi_version, struct_size;
  100  	__u32 first_delay, reserved;
  101  	struct idma_experiment request;
  102  	__s32 status;
  103  	__u32 rounds_attempted, rounds_completed, reserved_output;
  104  	__u64 sequence[2];
  105  	/* V src/dst IOVA, V src/dst PA, A src/dst IOVA, A src/dst PA. */
  106  	__u64 mapping[8];
  107  	struct idma_experiment result[2];
  108  	__u64 observation[2][IDMA_E3_WORDS];
  109  };
  110  #define IDMA_IOC_RUN_E3_PAIR _IOWR('i', 0x09, struct idma_e3_pair)
```

## composition UAPI

`/home/xvhaocheng/work/iommu-platform/linux/include/uapi/linux/idma.h:256–273`

```c
  256  /* Same-BIT composition experiment. This is independent of RESEARCH2 roles.
  257   * Inputs: first eight u32s and the existing p4m inputs. Outputs start at status.
  258   * Four logical modes: 0 S, 1 N, 2 P, 3 PN. P reserves entries 0..1 for DID.
  259   * Old HPM fields remain raw observations, not validated banked-mode metrics.
  260   */
  261  #define IDMA_COMPOSITION_ABI 1U
  262  #define IDMA_COMPOSITION_WORDS 128U
  263  #define IDMA_COMPOSITION_MAGIC 0x49444d4143504e01ULL
  264  struct idma_composition {
  265  	__u32 abi_version, struct_size, mode, protected_did;
  266  	__u32 victim_gap_cycles, reserved[3];
  267  	struct idma_p4m p4m;
  268  	__s32 status, restore_status;
  269  	__u32 configured, raw_captured;
  270  	__u64 generation, restored_generation;
  271  	__u64 raw[IDMA_COMPOSITION_WORDS];
  272  };
  273  #define IDMA_IOC_RUN_COMPOSITION _IOWR('i', 0x11, struct idma_composition)
```

## 现成工具恰好一次composition trial

`/home/xvhaocheng/work/iommu-platform/bench/idma_composition.c:1–15`

```c
    1  // SPDX-License-Identifier: GPL-2.0
    2  /*
    3   * idma_composition.c - one bounded same-BIT composition trial.
    4   *
    5   * Issues exactly one IDMA_IOC_RUN_COMPOSITION ioctl and prints one single-line
    6   * JSON record (schema idma-composition-v1).  It never re-runs a case, never
    7   * opens /dev/mem and never touches the research2 role interface.  A failure
    8   * still prints the JSON record and exits non-zero.
    9   *
   10   * Frozen composition ABI: four logical modes (0=S 1=N 2=P 3=PN), victim pages
   11   * 1, aggressor none or DMA instance 1 with 1/8/16 pages, 4096-byte chunks,
   12   * victim gap 0/1000/4000 cycles, aggressor gap 0.  protected_did is fixed to 10
   13   * and no other value can be requested from this tool.  Old HPM fields are
   14   * carried as raw observations only; they are not validated banked-mode metrics.
   15   */
```

## 可请求warm范围

`/home/xvhaocheng/work/iommu-platform/bench/idma_composition.c:68–82`

```c
   68  static void usage(void)
   69  {
   70  	puts("Usage: idma_composition [options]\n"
   71  	     "  --device PATH       Victim device (default /dev/idma0)\n"
   72  	     "  --mode N            Logical mode 0=S 1=N 2=P 3=PN (required)\n"
   73  	     "  --a-pages N         Aggressor pages 0/1/8/16; 0 means Solo (required)\n"
   74  	     "  --v-gap N           Victim gap cycles 0/1000/4000 (required)\n"
   75  	     "  --transfers N       Victim measured transfers 1..4096 (required)\n"
   76  	     "  --warmup N          Warmup transfers 0..4096 (required)\n"
   77  	     "  --timeout-ms N      Run timeout (default 15000)\n"
   78  	     "  --abi               Print the tool ABI as one JSON line, no hardware\n"
   79  	     "  --help              Show this help\n"
   80  	     "protected_did is fixed to 10; the aggressor is DMA instance 1 with DID 11.\n"
   81  	     "One JSON record per run; a failure still prints the record and exits non-zero.");
   82  }
```

## request字段来源

`/home/xvhaocheng/work/iommu-platform/bench/idma_composition.c:393–415`

```c
  393  static void build_request(const struct options *o, struct idma_composition *r)
  394  {
  395  	memset(r, 0, sizeof(*r));
  396  	r->abi_version = IDMA_COMPOSITION_ABI;
  397  	r->struct_size = (__u32)sizeof(*r);
  398  	r->mode = o->mode;
  399  	r->protected_did = PROTECTED_DID;
  400  	r->victim_gap_cycles = o->v_gap;
  401  
  402  	r->p4m.experiment.abi_version = IDMA_EXPERIMENT_ABI;
  403  	r->p4m.experiment.struct_size = (__u32)sizeof(struct idma_experiment);
  404  	r->p4m.experiment.flags = 0;
  405  	r->p4m.experiment.aggressor_id = o->a_pages ? AGGRESSOR_INSTANCE
  406  						    : IDMA_EXPERIMENT_NO_AGGRESSOR;
  407  	r->p4m.experiment.victim_pages = 1;
  408  	/* Solo keeps a nonzero page count; the kernel ignores it without an aggressor. */
  409  	r->p4m.experiment.aggressor_pages = o->a_pages ? o->a_pages : 1;
  410  	r->p4m.experiment.chunk_bytes = DEFAULT_CHUNK_BYTES;
  411  	r->p4m.experiment.run_timeout_ms = o->timeout_ms;
  412  	r->p4m.experiment.victim_transfers = o->transfers;
  413  	r->p4m.experiment.warmup_transfers = o->warmup;
  414  	r->p4m.aggressor_gap_cycles = 0;
  415  }
```

## 一次open/ioctl/close

`/home/xvhaocheng/work/iommu-platform/bench/idma_composition.c:461–528`

```c
  461  int main(int argc, char **argv)
  462  {
  463  	struct options o;
  464  	struct idma_composition req;
  465  	struct idma_composition result;
  466  	char err[256] = "";
  467  	const char *stage = "arguments";
  468  	int fd = -1, saved_errno = 0;
  469  	int have_result = 0, attempted = 0, success = 0, rc;
  470  
  471  	memset(&o, 0, sizeof(o));
  472  	memset(&req, 0, sizeof(req));
  473  	memset(&result, 0, sizeof(result));
  474  	o.device = "/dev/idma0";
  475  	o.timeout_ms = DEFAULT_TIMEOUT_MS;
  476  
  477  	rc = parse_args(argc, argv, &o, err, sizeof(err));
  478  	if (rc == 1) {
  479  		usage();
  480  		return EXIT_SUCCESS;
  481  	}
  482  	if (rc == 2) {
  483  		print_abi();
  484  		return EXIT_SUCCESS;
  485  	}
  486  	if (rc < 0)
  487  		goto done;
  488  
  489  	build_request(&o, &req);
  490  
  491  	stage = "open";
  492  	fd = open(o.device, O_RDWR | O_CLOEXEC);
  493  	if (fd < 0)
  494  		goto syscall_error;
  495  
  496  	stage = "composition";
  497  	attempted = 1;
  498  	result = req;
  499  	rc = ioctl(fd, IDMA_IOC_RUN_COMPOSITION, &result);
  500  	saved_errno = rc < 0 ? errno : 0;
  501  	have_result = rc >= 0 || result.status < 0;
  502  	if (rc < 0) {
  503  		snprintf(err, sizeof(err), "composition ioctl: %s", strerror(saved_errno));
  504  		goto done;
  505  	}
  506  	if (validate_result(&o, &result, err, sizeof(err)))
  507  		goto done;
  508  	success = 1;
  509  	goto done;
  510  
  511  syscall_error:
  512  	saved_errno = errno;
  513  	snprintf(err, sizeof(err), "%s: %s", stage, strerror(saved_errno));
  514  done:
  515  	if (fd >= 0 && close(fd) < 0 && success) {
  516  		saved_errno = errno;
  517  		snprintf(err, sizeof(err), "close: %s", strerror(saved_errno));
  518  		stage = "close";
  519  		success = 0;
  520  	}
  521  	if (*err)
  522  		fprintf(stderr, "idma_composition: %s\n", err);
  523  	report(argc, argv, &o, &result, have_result, attempted, stage, err,
  524  	       saved_errno, success);
  525  	if (fflush(stdout) == EOF)
  526  		return EXIT_FAILURE;
  527  	return success ? EXIT_SUCCESS : EXIT_FAILURE;
  528  }
```

## private fixed research及固定任务

`/home/xvhaocheng/work/iommu-platform/linux/include/uapi/linux/idma_sva.h:8–24`

```c
    8  /* Private, synchronous, fixed-task research ABI; not generic Linux SVA. */
    9  #define IDMA_SVA_ABI 1U
   10  #define IDMA_SVA_PAGE_BYTES 4096U
   11  #define IDMA_SVA_ARENA_PAGES 8U
   12  #define IDMA_SVA_DATA_PAGES 7U
   13  #define IDMA_SVA_ARENA_BYTES (IDMA_SVA_PAGE_BYTES * IDMA_SVA_ARENA_PAGES)
   14  #define IDMA_SVA_DESCRIPTORS 4U
   15  #define IDMA_SVA_PACKETS 3U
   16  #define IDMA_SVA_PAYLOAD_BYTES 1639U
   17  #define IDMA_SVA_RECORD_BYTES 1675U
   18  #define IDMA_SVA_HEADER_BYTES 12U
   19  #define IDMA_SVA_SOURCE_OFFSET 32U
   20  
   21  #define IDMA_SVA_MODE_IOVA 1U
   22  #define IDMA_SVA_MODE_SHADOW 2U
   23  #define IDMA_SVA_MODE_BORROWED_MM 3U
   24  #define IDMA_SVA_MODE_BIT(mode) (1U << ((mode) - 1U))
```

## 用户arena与同步结束合同/字段

`/home/xvhaocheng/work/iommu-platform/linux/include/uapi/linux/idma_sva.h:49–77`

```c
   49  /* Fixed descriptor offsets: 32->16396/60, 4128->20492/65,
   50   * 8224->20557/503, 12320->24588/1011; lanes 0,1,1,0.
   51   * Destination headers are page+0..11, payload starts page+12.
   52   * RUN consumes initialized private anonymous RW memory of exactly eight pages.
   53   * RUN never returns while hardware can still access arena/mm/page tables.
   54   * A timeout is not cancellation: unsafe hardware drain must retain resources
   55   * and block the syscall rather than report a releasable failed task.
   56   * Four public-DONE jobs run globally in order d0,d1,d2,d3. Success transfers
   57   * all bytes to CPU ownership, restores contexts, and releases all task pins.
   58   * Userspace independently checks data/guards before the actual file consumer.
   59   */
   60  struct idma_sva_run {
   61  	__u32 abi_version, struct_size, flags, address_mode;
   62  	__u32 peer_index, timeout_ms, reserved0[2];
   63  	__aligned_u64 arena_user;
   64  	__aligned_u64 arena_bytes;
   65  	__u32 reserved[4];
   66  	/* All output fields cleared by the kernel before validating input. */
   67  	__s32 status, restore_status;
   68  	__u32 result_flags, submitted, completed, primary_did, peer_did;
   69  	__u32 pinned_pages, mapped_pages, domain_pt_pages;
   70  	__aligned_u64 transfer_id[IDMA_SVA_DESCRIPTORS];
   71  	__aligned_u64 device_src[IDMA_SVA_DESCRIPTORS];
   72  	__aligned_u64 device_dst[IDMA_SVA_DESCRIPTORS];
   73  	__u32 reserved_output[8];
   74  };
   75  
   76  #define IDMA_IOC_SVA_CAPS _IOWR('i', 0x12, struct idma_sva_caps)
   77  #define IDMA_IOC_SVA_RUN _IOWR('i', 0x13, struct idma_sva_run)
```

## 默认禁用、session资源及profile

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma_sva_task.h:13–70`

```c
   13  static bool idma_sva_enable;
   14  module_param_named(sva_enable, idma_sva_enable, bool, 0600);
   15  MODULE_PARM_DESC(sva_enable,
   16  	"Opt in only after single-hart CVA6 WT/cache, default DMA profile and all-source quiescence acceptance");
   17  
   18  struct idma_sva_descriptor {
   19  	u32 src, dst, bytes, lane;
   20  };
   21  
   22  static const struct idma_sva_descriptor idma_sva_descriptors[] = {
   23  	{ 32, 16396, 60, 0 },
   24  	{ 4128, 20492, 65, 1 },
   25  	{ 8224, 20557, 503, 1 },
   26  	{ 12320, 24588, 1011, 0 },
   27  };
   28  
   29  struct idma_sva_session {
   30  	struct idma_device *lane[2];
   31  	struct device *dev[2];
   32  	struct iommu_domain *saved[2], *domain;
   33  	struct mm_struct *mm;
   34  	struct page *pages[IDMA_SVA_ARENA_PAGES];
   35  	unsigned int pinned, mapped;
   36  	bool claimed[2], attached[2], binding_changed;
   37  	unsigned long base, device_base;
   38  	struct riscv_iommu_research_certificate certificate;
   39  };
   40  
   41  /* This is a platform property, not the architectural FENCE.I DMA contract. */
   42  static void idma_sva_cache_sync(void *unused)
   43  {
   44  	mb();
   45  	local_flush_icache_all();
   46  	mb();
   47  }
   48  
   49  /* No HW abort/PR-PW drain exists. Returning would permit mm teardown/UAF. */
   50  static void __noreturn idma_sva_retain(struct idma_sva_session *s,
   51  				     const char *reason)
   52  {
   53  	WRITE_ONCE(s->lane[0]->quarantined, true);
   54  	WRITE_ONCE(s->lane[1]->quarantined, true);
   55  	dev_crit(s->dev[0],
   56  		 "SVA task unsafe to release (%s); retaining pins/mm/mmap lock/device ownership until platform recovery\n",
   57  		 reason);
   58  	for (;;)
   59  		schedule_timeout_uninterruptible(HZ);
   60  }
   61  
   62  static bool idma_sva_profile(struct idma_device *d, struct idma_device *peer)
   63  {
   64  	return READ_ONCE(idma_sva_enable) && PAGE_SIZE == 4096 &&
   65  		num_online_cpus() == 1 &&
   66  		of_machine_is_compatible("eth,ariane-bare-dev") &&
   67  		satp_mode == SATP_MODE_39 && d->id == 0 && d->did == 10 &&
   68  		peer && peer->did == 11 &&
   69  		riscv_iommu_research_supported(d->miscdev.parent,
   70  					       peer->miscdev.parent);
```

## 每次RUN的pin/domain/map/execute/restore/unpin

`/home/xvhaocheng/work/iommu-platform/linux/drivers/misc/idma_sva_task.h:308–464`

```c
  308  static long idma_sva_run_ioctl(struct idma_device *primary, void __user *uarg)
  309  {
  310  	struct idma_sva_run r;
  311  	struct idma_sva_session *s;
  312  	unsigned long end;
  313  	int ret, cleanup = 0, i;
  314  	bool mmap_locked = false;
  315  
  316  	if (copy_from_user(&r, uarg, sizeof(r)))
  317  		return -EFAULT;
  318  	memset(&r.status, 0, sizeof(r) - offsetof(struct idma_sva_run, status));
  319  	ret = -EINVAL;
  320  	if (r.abi_version != IDMA_SVA_ABI || r.struct_size != sizeof(r) ||
  321  	    r.flags || memchr_inv(r.reserved0, 0, sizeof(r.reserved0)) ||
  322  	    memchr_inv(r.reserved, 0, sizeof(r.reserved)) ||
  323  	    r.address_mode < IDMA_SVA_MODE_IOVA ||
  324  	    r.address_mode > IDMA_SVA_MODE_BORROWED_MM ||
  325  	    r.arena_bytes != IDMA_SVA_ARENA_BYTES ||
  326  	    !IS_ALIGNED(r.arena_user, PAGE_SIZE) ||
  327  	    check_add_overflow((unsigned long)r.arena_user,
  328  			       (unsigned long)r.arena_bytes, &end) ||
  329  	    end > TASK_SIZE || !r.timeout_ms || r.timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
  330  		goto copy_result;
  331  	ret = -EPERM;
  332  	if (!capable(CAP_SYS_RAWIO) || !current->mm ||
  333  	    get_nr_threads(current) != 1 ||
  334  	    atomic_read(&current->mm->mm_users) != 1)
  335  		goto copy_result;
  336  	s = kzalloc(sizeof(*s), GFP_KERNEL);
  337  	if (!s) {
  338  		ret = -ENOMEM;
  339  		goto copy_result;
  340  	}
  341  	s->lane[0] = idma_get_instance(primary->id);
  342  	s->lane[1] = idma_get_instance(r.peer_index);
  343  	ret = -EOPNOTSUPP;
  344  	if (!s->lane[0] || !s->lane[1])
  345  		goto free_session;
  346  	s->dev[0] = s->lane[0]->miscdev.parent;
  347  	s->dev[1] = s->lane[1]->miscdev.parent;
  348  	s->base = r.arena_user;
  349  	s->device_base = r.address_mode == IDMA_SVA_MODE_IOVA ?
  350  		0x10000000UL : s->base;
  351  	s->mm = current->mm;
  352  	mmget(s->mm);
  353  	ret = mutex_lock_interruptible(&idma_experiment_lock);
  354  	if (ret)
  355  		goto put_mm;
  356  	cpus_read_lock();
  357  	if (!idma_sva_profile(primary, s->lane[1])) {
  358  		ret = -EOPNOTSUPP;
  359  		goto unlock_experiment;
  360  	}
  361  	ret = idma_sva_preflight(s);
  362  	if (ret)
  363  		goto unlock_experiment;
  364  	mmap_read_lock(s->mm);
  365  	mmap_locked = true;
  366  	ret = idma_sva_pin(s, r.address_mode == IDMA_SVA_MODE_BORROWED_MM);
  367  	if (ret)
  368  		goto release_pins;
  369  	r.pinned_pages = s->pinned;
  370  	r.primary_did = primary->did;
  371  	r.peer_did = s->lane[1]->did;
  372  	r.domain_pt_pages = U32_MAX; /* Unknown includes no fabricated allocation fee. */
  373  	s->domain = riscv_iommu_research_domain_alloc(s->dev[0],
  374  		r.address_mode == IDMA_SVA_MODE_BORROWED_MM ? s->mm : NULL);
  375  	if (IS_ERR(s->domain)) {
  376  		ret = PTR_ERR(s->domain);
  377  		s->domain = NULL;
  378  		goto release_pins;
  379  	}
  380  	if (r.address_mode == IDMA_SVA_MODE_BORROWED_MM)
  381  		r.domain_pt_pages = 0;
  382  	else {
  383  		for (i = 0; i < IDMA_SVA_DATA_PAGES; ++i) {
  384  			ret = iommu_map(s->domain, s->device_base + i * PAGE_SIZE,
  385  				page_to_phys(s->pages[i]), PAGE_SIZE,
  386  				IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
  387  			if (ret)
  388  				goto restore;
  389  			s->mapped++;
  390  		}
  391  		r.mapped_pages = s->mapped;
  392  	}
  393  	/* Preserve backing/default domains and claim both before attaching either. */
  394  	for (i = 0; i < 2; ++i) {
  395  		s->saved[i] = iommu_get_domain_for_dev(s->dev[i]);
  396  		ret = iommu_device_claim_dma_owner(s->dev[i], s);
  397  		if (ret)
  398  			goto restore;
  399  		s->claimed[i] = true;
  400  		s->binding_changed = true;
  401  	}
  402  	for (i = 0; i < 2; ++i) {
  403  		ret = iommu_attach_device(s->domain, s->dev[i]);
  404  		if (ret)
  405  			goto restore;
  406  		s->attached[i] = true;
  407  	}
  408  	ret = idma_sva_barrier(s);
  409  	if (ret)
  410  		idma_sva_retain(s, "initial context barrier failed");
  411  	idma_sva_cache_sync(NULL);
  412  	ret = idma_sva_execute(s, &r);
  413  restore:
  414  	/* No CPU/page/context release until every published job actually retired. */
  415  	for (i = 1; i >= 0; --i) {
  416  		if (s->attached[i])
  417  			iommu_detach_device(s->domain, s->dev[i]);
  418  		if (s->claimed[i]) {
  419  			iommu_device_release_dma_owner(s->dev[i]);
  420  			if (iommu_get_domain_for_dev(s->dev[i]) != s->saved[i])
  421  				idma_sva_retain(s, "default domain restore failed");
  422  		}
  423  	}
  424  	if (s->binding_changed) {
  425  		cleanup = idma_sva_barrier(s);
  426  		if (cleanup)
  427  			idma_sva_retain(s, "restored context barrier failed");
  428  	}
  429  	while (s->mapped) {
  430  		--s->mapped;
  431  		if (iommu_unmap(s->domain,
  432  			s->device_base + s->mapped * PAGE_SIZE, PAGE_SIZE) != PAGE_SIZE)
  433  			idma_sva_retain(s, "short unmap");
  434  	}
  435  	if (s->domain) {
  436  		cleanup = riscv_iommu_research_domain_release(s->domain);
  437  		if (cleanup)
  438  			idma_sva_retain(s, "domain release not certified");
  439  	}
  440  	idma_sva_cache_sync(NULL);
  441  	r.result_flags |= IDMA_SVA_RESULT_CONTEXT_RESTORED |
  442  		IDMA_SVA_RESULT_CPU_OWNED;
  443  release_pins:
  444  	if (s->pinned)
  445  		unpin_user_pages_dirty_lock(s->pages, s->pinned, true);
  446  	r.result_flags |= IDMA_SVA_RESULT_PINS_RELEASED;
  447  	if (mmap_locked)
  448  		mmap_read_unlock(s->mm);
  449  unlock_experiment:
  450  	cpus_read_unlock();
  451  	mutex_unlock(&idma_experiment_lock);
  452  put_mm:
  453  	mmput(s->mm);
  454  free_session:
  455  	idma_put_instance(s->lane[1]);
  456  	idma_put_instance(s->lane[0]);
  457  	kfree(s);
  458  copy_result:
  459  	r.status = ret;
  460  	r.restore_status = cleanup;
  461  	if (copy_to_user(uarg, &r, sizeof(r)))
  462  		return -EFAULT;
  463  	return ret;
  464  }
```

## 每repeat新arena并初始化

`/home/xvhaocheng/work/iommu-platform/bench/idma_sva_task.c:431–463`

```c
  431  	if (options.mode < MODE_CPU_COPY && !options.host_model) {
  432  		fd = open(options.device, O_RDWR | O_CLOEXEC);
  433  		if (fd < 0) { perror("open DMA device"); goto done; }
  434  		if (check_caps(fd, &options, &caps)) { perror("CAPS"); goto done; }
  435  	}
  436  	for (iteration = 0; iteration < options.repeat; iteration++) {
  437  		unsigned char *arena = MAP_FAILED;
  438  		struct idma_sva_run run = { 0 };
  439  		int arena_locked = 0;
  440  		unsigned int i;
  441  		if (read_input(options.input, input)) { perror("input frames"); goto done; }
  442  		if (options.mode != MODE_SOURCE_WRITEV) {
  443  			if (sysconf(_SC_PAGESIZE) != IDMA_SVA_PAGE_BYTES) {
  444  				fprintf(stderr, "This fixed task requires 4096-byte OS pages\n");
  445  				goto done;
  446  			}
  447  			arena = mmap(NULL, IDMA_SVA_ARENA_BYTES, PROT_READ | PROT_WRITE,
  448  				     MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
  449  			if (arena == MAP_FAILED) { perror("arena mmap"); goto done; }
  450  			/* Only borrowed native CPU page tables need the VM_LOCKED profile.
  451  			 * Establish it before writing every page's source/poison/guards so
  452  			 * the subsequent A/D check runs inside the locked-page window.
  453  			 * The host model performs no real pin/cache/mm lifetime operation.
  454  			 */
  455  			if (!options.host_model && options.mode == IDMA_SVA_MODE_BORROWED_MM) {
  456  				if (mlock(arena, IDMA_SVA_ARENA_BYTES)) {
  457  					perror("arena mlock");
  458  					munmap(arena, IDMA_SVA_ARENA_BYTES);
  459  					goto done;
  460  				}
  461  				arena_locked = 1;
  462  			}
  463  			initialize_arena(arena, input);
```

## 检查、consumer与释放arena

`/home/xvhaocheng/work/iommu-platform/bench/idma_sva_task.c:469–498`

```c
  469  			} else {
  470  				int run_failed = run_dma(fd, &options, arena, &run);
  471  
  472  				if (run_failed) perror("DMA RUN");
  473  				/* The synchronous ABI guarantees no hardware references on
  474  				 * every return, including a safely drained failed RUN.
  475  				 */
  476  				if (arena_locked && munlock(arena, IDMA_SVA_ARENA_BYTES)) {
  477  					perror("arena munlock");
  478  					munmap(arena, IDMA_SVA_ARENA_BYTES);
  479  					goto done;
  480  				}
  481  				arena_locked = 0;
  482  				if (run_failed) {
  483  					munmap(arena, IDMA_SVA_ARENA_BYTES);
  484  					goto done;
  485  				}
  486  			}
  487  			if (verify_arena(arena, input)) {
  488  				perror("independent bytes/guards");
  489  				munmap(arena, IDMA_SVA_ARENA_BYTES);
  490  				goto done;
  491  			}
  492  		}
  493  		if (consume_file(&options, iteration, arena, input)) {
  494  			perror("file consumer");
  495  			if (arena != MAP_FAILED) munmap(arena, IDMA_SVA_ARENA_BYTES);
  496  			goto done;
  497  		}
  498  		if (arena != MAP_FAILED && munmap(arena, IDMA_SVA_ARENA_BYTES)) { perror("arena munmap"); goto done; }
```

## 打包源码条目不等于已部署

`/home/xvhaocheng/work/iommu-platform/scripts/rootfs.py:8–24`

```c
    8  EXECUTABLES={
    9      'idma_e3_controlled': 'out/bench/idma_e3_controlled',
   10      'platform-e3-controlled': 'tests/board-e3-controlled.sh',
   11      'idma_e3_pair': 'out/bench/idma_e3_pair',
   12      'idma_experiment': 'out/bench/idma_experiment',
   13      'platform-smoke': 'tests/board-smoke.sh',
   14      'platform-p4m': 'tests/board-p4m.sh',
   15      'platform-p4m-start': 'tests/board-p4m-start.sh',
   16      'platform-timeout': 'out/bench/platform-timeout',
   17      'idma_research': 'out/bench/idma_research',
   18      'idma_composition': 'out/bench/idma_composition',
   19      'idma_sva_task': 'out/bench/idma_sva_task',
   20      'platform-research': 'tests/board-research.sh',
   21      'platform-sd-update': 'out/bench/platform-sd-update',
   22      'platform-net-up': 'tests/board-net-up.sh',
   23      'platform-fetch-update': 'tests/board-fetch-update.sh',
   24  }
```
