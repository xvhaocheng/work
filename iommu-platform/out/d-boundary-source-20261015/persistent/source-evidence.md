# 固定SPDK来源：精确原文摘录

## Material Passport

ARS-Codex / experiment-agent / plan；源码证据，不是运行trace。行号保留保存原件编号；本轮无联网。引用的两个文件均固定于 SPDK commit `19524ad45373e35d5fdc06f4e081444d87ceed80`。完整hash/阅读范围见 [source-manifest.json](source-manifest.json)。

## PERF-OBJECTS

原件：[spdk_nvme_perf.c](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c)。

原件 L129–L184：

```c
  129 struct ns_worker_ctx {
  130 	struct ns_entry		*entry;
  131 	struct ns_worker_stats	stats;
  132 	uint64_t		current_queue_depth;
  133 	uint64_t		offset_in_ios;
  134 	bool			is_draining;
  135 
  136 	union {
  137 		struct {
  138 			int				num_active_qpairs;
  139 			int				num_all_qpairs;
  140 			struct spdk_nvme_qpair		**qpair;
  141 			struct spdk_nvme_poll_group	*group;
  142 			int				last_qpair;
  143 		} nvme;
  144 
  145 #ifdef SPDK_CONFIG_URING
  146 		struct {
  147 			struct io_uring		ring;
  148 			uint64_t		io_inflight;
  149 			uint64_t		io_pending;
  150 			struct io_uring_cqe	**cqes;
  151 
  152 		} uring;
  153 #endif
  154 #if HAVE_LIBAIO
  155 		struct {
  156 			struct io_event		*events;
  157 			io_context_t		ctx;
  158 		} aio;
  159 #endif
  160 	} u;
  161 
  162 	TAILQ_ENTRY(ns_worker_ctx)	link;
  163 
  164 	TAILQ_HEAD(, perf_task)		queued_tasks;
  165 
  166 	struct spdk_histogram_data	*histogram;
  167 	int				status;
  168 };
  169 
  170 struct perf_task {
  171 	struct ns_worker_ctx	*ns_ctx;
  172 	struct iovec		*iovs; /* array of iovecs to transfer. */
  173 	int			iovcnt; /* Number of iovecs in iovs array. */
  174 	int			iovpos; /* Current iovec position. */
  175 	uint32_t		iov_offset; /* Offset in current iovec. */
  176 	struct iovec		md_iov;
  177 	uint64_t		submit_tsc;
  178 	bool			is_read;
  179 	struct spdk_dif_ctx	dif_ctx;
  180 #if HAVE_LIBAIO
  181 	struct iocb		iocb;
  182 #endif
  183 	TAILQ_ENTRY(perf_task)	link;
  184 };
```

## PERF-ALLOC

原件：[spdk_nvme_perf.c](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c)。

原件 L804–L844：

```c
  804 nvme_setup_payload(struct perf_task *task, uint8_t pattern)
  805 {
  806 	struct spdk_nvme_ctrlr *ctrlr;
  807 	uint32_t max_io_size_bytes, max_io_md_size;
  808 	int32_t numa_id;
  809 	void *buf;
  810 	int rc;
  811 
  812 	ctrlr = task->ns_ctx->entry->u.nvme.ctrlr;
  813 	numa_id = spdk_nvme_ctrlr_get_numa_id(ctrlr);
  814 
  815 	/* maximum extended lba format size from all active namespace,
  816 	 * it's same with g_io_size_bytes for namespace without metadata.
  817 	 */
  818 	max_io_size_bytes = g_io_size_bytes + g_max_io_md_size * g_max_io_size_blocks;
  819 	buf = spdk_dma_zmalloc_socket(max_io_size_bytes, g_io_align, NULL, numa_id);
  820 	if (buf == NULL) {
  821 		fprintf(stderr, "task->buf spdk_dma_zmalloc failed\n");
  822 		exit(1);
  823 	}
  824 	memset(buf, pattern, max_io_size_bytes);
  825 
  826 	rc = nvme_perf_allocate_iovs(task, buf, max_io_size_bytes);
  827 	if (rc < 0) {
  828 		fprintf(stderr, "perf task failed to allocate iovs\n");
  829 		spdk_dma_free(buf);
  830 		exit(1);
  831 	}
  832 
  833 	max_io_md_size = g_max_io_md_size * g_max_io_size_blocks;
  834 	if (max_io_md_size != 0) {
  835 		task->md_iov.iov_base = spdk_dma_zmalloc(max_io_md_size, g_io_align, NULL);
  836 		task->md_iov.iov_len = max_io_md_size;
  837 		if (task->md_iov.iov_base == NULL) {
  838 			fprintf(stderr, "task->md_buf spdk_dma_zmalloc failed\n");
  839 			spdk_dma_free(task->iovs[0].iov_base);
  840 			free(task->iovs);
  841 			exit(1);
  842 		}
  843 	}
  844 }
```

原件 L890–L904：

```c
  890 	if (task->is_read) {
  891 		if (task->iovcnt == 1) {
  892 			return spdk_nvme_ns_cmd_read_with_md(entry->u.nvme.ns, ns_ctx->u.nvme.qpair[qp_num],
  893 							     task->iovs[0].iov_base, task->md_iov.iov_base,
  894 							     lba,
  895 							     entry->io_size_blocks, io_complete,
  896 							     task, entry->io_flags,
  897 							     task->dif_ctx.apptag_mask, task->dif_ctx.app_tag);
  898 		} else {
  899 			return spdk_nvme_ns_cmd_readv_with_md(entry->u.nvme.ns, ns_ctx->u.nvme.qpair[qp_num],
  900 							      lba, entry->io_size_blocks,
  901 							      io_complete, task, entry->io_flags,
  902 							      nvme_perf_reset_sgl, nvme_perf_next_sge,
  903 							      task->md_iov.iov_base,
  904 							      task->dif_ctx.apptag_mask, task->dif_ctx.app_tag);
```

## PERF-SUBMIT-COMPLETE

原件：[spdk_nvme_perf.c](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c)。

原件 L1471–L1529：

```c
 1471 submit_single_io(struct perf_task *task)
 1472 {
 1473 	uint64_t		rand_value, offset_in_ios;
 1474 	int			rc;
 1475 	struct ns_worker_ctx	*ns_ctx = task->ns_ctx;
 1476 	struct ns_entry		*entry = ns_ctx->entry;
 1477 
 1478 	assert(!ns_ctx->is_draining);
 1479 
 1480 	if (entry->zipf) {
 1481 		offset_in_ios = spdk_zipf_generate(entry->zipf);
 1482 	} else if (g_is_random) {
 1483 		/* rand_r() returns int, so we need to use two calls to ensure
 1484 		 * we get a large enough value to cover a very large block
 1485 		 * device.
 1486 		 */
 1487 		rand_value = (uint64_t)rand_r(&entry->seed) *
 1488 			     ((uint64_t)RAND_MAX + 1) +
 1489 			     rand_r(&entry->seed);
 1490 		offset_in_ios = rand_value % entry->size_in_ios;
 1491 	} else {
 1492 		offset_in_ios = ns_ctx->offset_in_ios++;
 1493 		if (ns_ctx->offset_in_ios == entry->size_in_ios) {
 1494 			ns_ctx->offset_in_ios = 0;
 1495 		}
 1496 	}
 1497 
 1498 	task->submit_tsc = spdk_get_ticks();
 1499 
 1500 	if ((g_rw_percentage == 100) ||
 1501 	    (g_rw_percentage != 0 && ((rand_r(&entry->seed) % 100) < g_rw_percentage))) {
 1502 		task->is_read = true;
 1503 	} else {
 1504 		task->is_read = false;
 1505 	}
 1506 
 1507 	rc = entry->fn_table->submit_io(task, ns_ctx, entry, offset_in_ios);
 1508 
 1509 	if (spdk_unlikely(rc != 0)) {
 1510 		if (g_continue_on_error) {
 1511 			/* We can't just resubmit here or we can get in a loop that
 1512 			 * stack overflows. */
 1513 			TAILQ_INSERT_TAIL(&ns_ctx->queued_tasks, task, link);
 1514 		} else {
 1515 			RATELIMIT_LOG("starting I/O failed: %d\n", rc);
 1516 			spdk_dma_free(task->iovs[0].iov_base);
 1517 			free(task->iovs);
 1518 			spdk_dma_free(task->md_iov.iov_base);
 1519 			task->ns_ctx->status = 1;
 1520 			free(task);
 1521 		}
 1522 	} else {
 1523 		ns_ctx->current_queue_depth++;
 1524 		ns_ctx->stats.io_submitted++;
 1525 	}
 1526 
 1527 	if (spdk_unlikely(g_number_ios && ns_ctx->stats.io_submitted >= g_number_ios)) {
 1528 		ns_ctx->is_draining = true;
 1529 	}
```

原件 L1539–L1544：

```c
 1539 	ns_ctx = task->ns_ctx;
 1540 	entry = ns_ctx->entry;
 1541 	ns_ctx->current_queue_depth--;
 1542 	ns_ctx->stats.io_completed++;
 1543 	tsc_diff = spdk_get_ticks() - task->submit_tsc;
 1544 	ns_ctx->stats.total_tsc += tsc_diff;
```

原件 L1560–L1600：

```c
 1560 	/*
 1561 	 * is_draining indicates when time has expired or io_submitted exceeded
 1562 	 * g_number_ios for the test run and we are just waiting for the previously
 1563 	 * submitted I/O to complete. In this case, do not submit a new I/O to
 1564 	 * replace the one just completed.
 1565 	 */
 1566 	if (spdk_unlikely(ns_ctx->is_draining)) {
 1567 		spdk_dma_free(task->iovs[0].iov_base);
 1568 		free(task->iovs);
 1569 		spdk_dma_free(task->md_iov.iov_base);
 1570 		free(task);
 1571 	} else {
 1572 		submit_single_io(task);
 1573 	}
 1574 }
 1575 
 1576 static void
 1577 io_complete(void *ctx, const struct spdk_nvme_cpl *cpl)
 1578 {
 1579 	struct perf_task *task = ctx;
 1580 
 1581 	if (spdk_unlikely(spdk_nvme_cpl_is_error(cpl))) {
 1582 		if (task->is_read) {
 1583 			RATELIMIT_LOG("Read completed with error (sct=%d, sc=%d)\n",
 1584 				      cpl->status.sct, cpl->status.sc);
 1585 		} else {
 1586 			RATELIMIT_LOG("Write completed with error (sct=%d, sc=%d)\n",
 1587 				      cpl->status.sct, cpl->status.sc);
 1588 		}
 1589 		if (!g_continue_on_error) {
 1590 			if (cpl->status.sct == SPDK_NVME_SCT_GENERIC &&
 1591 			    cpl->status.sc == SPDK_NVME_SC_INVALID_NAMESPACE_OR_FORMAT) {
 1592 				/* The namespace was hotplugged.  Stop trying to send I/O to it. */
 1593 				task->ns_ctx->is_draining = true;
 1594 			}
 1595 
 1596 			task->ns_ctx->status = 1;
 1597 		}
 1598 	}
 1599 
 1600 	task_complete(task);
```

原件 L1603–L1648：

```c
 1603 static struct perf_task *
 1604 allocate_task(struct ns_worker_ctx *ns_ctx, int queue_depth)
 1605 {
 1606 	struct perf_task *task;
 1607 
 1608 	task = calloc(1, sizeof(*task));
 1609 	if (task == NULL) {
 1610 		fprintf(stderr, "Out of memory allocating tasks\n");
 1611 		exit(1);
 1612 	}
 1613 
 1614 	task->ns_ctx = ns_ctx;
 1615 	ns_ctx->entry->fn_table->setup_payload(task, queue_depth % 8 + 1);
 1616 
 1617 	return task;
 1618 }
 1619 
 1620 static void
 1621 submit_io(struct ns_worker_ctx *ns_ctx, int queue_depth)
 1622 {
 1623 	struct perf_task *task;
 1624 
 1625 	while (queue_depth-- > 0) {
 1626 		task = allocate_task(ns_ctx, queue_depth);
 1627 		submit_single_io(task);
 1628 	}
 1629 }
 1630 
 1631 static int
 1632 init_ns_worker_ctx(struct ns_worker_ctx *ns_ctx)
 1633 {
 1634 	TAILQ_INIT(&ns_ctx->queued_tasks);
 1635 	return ns_ctx->entry->fn_table->init_ns_worker_ctx(ns_ctx);
 1636 }
 1637 
 1638 static void
 1639 cleanup_ns_worker_ctx(struct ns_worker_ctx *ns_ctx)
 1640 {
 1641 	struct perf_task *task, *ttask;
 1642 
 1643 	TAILQ_FOREACH_SAFE(task, &ns_ctx->queued_tasks, link, ttask) {
 1644 		TAILQ_REMOVE(&ns_ctx->queued_tasks, task, link);
 1645 		task_complete(task);
 1646 	}
 1647 	ns_ctx->entry->fn_table->cleanup_ns_worker_ctx(ns_ctx);
 1648 }
```

## PERF-SETUP

原件：[spdk_nvme_perf.c](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c)。

原件 L1011–L1028：

```c
 1011 	ns_ctx->u.nvme.num_active_qpairs = g_nr_io_queues_per_ns;
 1012 	ns_ctx->u.nvme.num_all_qpairs = g_nr_io_queues_per_ns + g_nr_unused_io_queues;
 1013 	ns_ctx->u.nvme.qpair = calloc(ns_ctx->u.nvme.num_all_qpairs, sizeof(struct spdk_nvme_qpair *));
 1014 	if (!ns_ctx->u.nvme.qpair) {
 1015 		return -1;
 1016 	}
 1017 
 1018 	spdk_nvme_ctrlr_get_default_io_qpair_opts(entry->u.nvme.ctrlr, &opts, sizeof(opts));
 1019 	if (opts.io_queue_requests < entry->num_io_requests) {
 1020 		opts.io_queue_requests = entry->num_io_requests;
 1021 	}
 1022 	opts.delay_cmd_submit = true;
 1023 	opts.create_only = true;
 1024 
 1025 	ctrlr_opts = spdk_nvme_ctrlr_get_opts(entry->u.nvme.ctrlr);
 1026 	opts.async_mode = !(spdk_nvme_ctrlr_get_transport_id(entry->u.nvme.ctrlr)->trtype ==
 1027 			    SPDK_NVME_TRANSPORT_PCIE
 1028 			    && ns_ctx->u.nvme.num_all_qpairs > ctrlr_opts->admin_queue_size);
```

原件 L1030–L1039：

```c
 1030 	ns_ctx->u.nvme.group = spdk_nvme_poll_group_create(ns_ctx, NULL);
 1031 	if (ns_ctx->u.nvme.group == NULL) {
 1032 		goto poll_group_failed;
 1033 	}
 1034 
 1035 	group = ns_ctx->u.nvme.group;
 1036 	for (i = 0; i < ns_ctx->u.nvme.num_all_qpairs; i++) {
 1037 		ns_ctx->u.nvme.qpair[i] = spdk_nvme_ctrlr_alloc_io_qpair(entry->u.nvme.ctrlr, &opts,
 1038 					  sizeof(opts));
 1039 		qpair = ns_ctx->u.nvme.qpair[i];
```

原件 L1058–L1069：

```c
 1058 	/* Busy poll here until all qpairs are connected - this ensures once we start
 1059 	 * I/O we aren't still waiting for some qpairs to connect. Limit the poll to
 1060 	 * 10 seconds though.
 1061 	 */
 1062 	poll_timeout_tsc = spdk_get_ticks() + 10 * spdk_get_ticks_hz();
 1063 	rc = -EAGAIN;
 1064 	while (spdk_get_ticks() < poll_timeout_tsc && rc == -EAGAIN) {
 1065 		spdk_nvme_poll_group_process_completions(group, 0, perf_disconnect_cb);
 1066 		rc = spdk_nvme_poll_group_all_connected(group);
 1067 		if (rc == 0) {
 1068 			return 0;
 1069 		}
```

原件 L1086–L1097：

```c
 1086 static void
 1087 nvme_cleanup_ns_worker_ctx(struct ns_worker_ctx *ns_ctx)
 1088 {
 1089 	int i;
 1090 
 1091 	for (i = 0; i < ns_ctx->u.nvme.num_all_qpairs; i++) {
 1092 		spdk_nvme_ctrlr_free_io_qpair(ns_ctx->u.nvme.qpair[i]);
 1093 	}
 1094 
 1095 	spdk_nvme_poll_group_destroy(ns_ctx->u.nvme.group);
 1096 	free(ns_ctx->u.nvme.qpair);
 1097 }
```

## PERF-PHASES

原件：[spdk_nvme_perf.c](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c)。

原件 L1737–L1758：

```c
 1737 	rc = pthread_barrier_wait(&g_worker_sync_barrier);
 1738 	if (rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {
 1739 		printf("ERROR: failed to wait on thread sync barrier\n");
 1740 		ns_ctx->status = 1;
 1741 		return 1;
 1742 	}
 1743 
 1744 	tsc_start = spdk_get_ticks();
 1745 	tsc_current = tsc_start;
 1746 	tsc_next_print = tsc_current + g_tsc_rate;
 1747 
 1748 	if (g_warmup_time_in_sec) {
 1749 		warmup = true;
 1750 		tsc_end = tsc_current + g_warmup_time_in_sec * g_tsc_rate;
 1751 	} else {
 1752 		tsc_end = tsc_current + g_time_in_sec * g_tsc_rate;
 1753 	}
 1754 
 1755 	/* Submit initial I/O for each namespace. */
 1756 	TAILQ_FOREACH(ns_ctx, &worker->ns_ctx, link) {
 1757 		submit_io(ns_ctx, g_queue_depth);
 1758 	}
```

原件 L1760–L1768：

```c
 1760 	while (spdk_likely(!g_exit)) {
 1761 		bool all_draining = true;
 1762 
 1763 		/*
 1764 		 * Check for completed I/O for each controller. A new
 1765 		 * I/O will be submitted in the io_complete callback
 1766 		 * to replace each I/O that is completed.
 1767 		 */
 1768 		TAILQ_FOREACH(ns_ctx, &worker->ns_ctx, link) {
```

原件 L1785–L1802：

```c
 1785 			check_now = spdk_get_ticks();
 1786 			check_rc = ns_ctx->entry->fn_table->check_io(ns_ctx);
 1787 
 1788 			if (check_rc > 0) {
 1789 				ns_ctx->stats.busy_tsc += check_now - ns_ctx->stats.last_tsc;
 1790 			} else {
 1791 				ns_ctx->stats.idle_tsc += check_now - ns_ctx->stats.last_tsc;
 1792 			}
 1793 			ns_ctx->stats.last_tsc = check_now;
 1794 
 1795 			if (!ns_ctx->is_draining) {
 1796 				all_draining = false;
 1797 			}
 1798 		}
 1799 
 1800 		if (spdk_unlikely(all_draining)) {
 1801 			break;
 1802 		}
```

原件 L1811–L1829：

```c
 1811 		if (tsc_current > tsc_end) {
 1812 			if (warmup) {
 1813 				/* Update test start and end time, clear statistics */
 1814 				tsc_start = spdk_get_ticks();
 1815 				tsc_end = tsc_start + g_time_in_sec * g_tsc_rate;
 1816 
 1817 				TAILQ_FOREACH(ns_ctx, &worker->ns_ctx, link) {
 1818 					memset(&ns_ctx->stats, 0, sizeof(ns_ctx->stats));
 1819 					ns_ctx->stats.min_tsc = UINT64_MAX;
 1820 					spdk_histogram_data_reset(ns_ctx->histogram);
 1821 				}
 1822 
 1823 				if (worker->lcore == g_main_core && isatty(STDOUT_FILENO)) {
 1824 					/* warmup stage prints a longer string to stdout, need to erase it */
 1825 					printf("%c[2K", 27);
 1826 				}
 1827 
 1828 				warmup = false;
 1829 			} else {
```

原件 L1835–L1871：

```c
 1835 	/* Capture the actual elapsed time when we break out of the main loop. This will account
 1836 	 * for cases where we exit prematurely due to a signal. We only need to capture it on
 1837 	 * one core, so use the main core.
 1838 	 */
 1839 	if (worker->lcore == g_main_core) {
 1840 		g_elapsed_time_in_usec = (tsc_current - tsc_start) * SPDK_SEC_TO_USEC / g_tsc_rate;
 1841 	}
 1842 
 1843 	/* drain the io of each ns_ctx in round robin to make the fairness */
 1844 	do {
 1845 		unfinished_ns_ctx = 0;
 1846 		TAILQ_FOREACH(ns_ctx, &worker->ns_ctx, link) {
 1847 			/* first time will enter into this if case */
 1848 			if (!ns_ctx->is_draining) {
 1849 				ns_ctx->is_draining = true;
 1850 			}
 1851 
 1852 			if (ns_ctx->current_queue_depth > 0) {
 1853 				ns_ctx->entry->fn_table->check_io(ns_ctx);
 1854 				if (ns_ctx->current_queue_depth > 0) {
 1855 					unfinished_ns_ctx++;
 1856 				}
 1857 			}
 1858 		}
 1859 	} while (unfinished_ns_ctx > 0);
 1860 
 1861 	if (g_dump_transport_stats) {
 1862 		pthread_mutex_lock(&g_stats_mutex);
 1863 		perf_dump_transport_statistics(worker);
 1864 		pthread_mutex_unlock(&g_stats_mutex);
 1865 	}
 1866 
 1867 	TAILQ_FOREACH(ns_ctx, &worker->ns_ctx, link) {
 1868 		cleanup_ns_worker_ctx(ns_ctx);
 1869 	}
 1870 
 1871 	return 0;
```

## PERF-WARM-CONTRACT

原件：[spdk_nvme_perf.c](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c)。

原件 L239–L246：

```c
  239 static uint32_t g_queue_depth;
  240 static int g_nr_io_queues_per_ns = 1;
  241 static int g_nr_unused_io_queues;
  242 static int g_time_in_sec;
  243 static uint64_t g_number_ios;
  244 static uint64_t g_elapsed_time_in_usec;
  245 static int g_warmup_time_in_sec;
  246 static uint32_t g_max_completions;
```

原件 L1882–L1890：

```c
 1882 	printf("==== BASIC OPTIONS ====\n\n");
 1883 	printf("\t-q, --io-depth <val> io depth\n");
 1884 	printf("\t-o, --io-size <val> io size in bytes\n");
 1885 	printf("\t-w, --io-pattern <pattern> io pattern type, must be one of\n");
 1886 	printf("\t\t(read, write, randread, randwrite, rw, randrw)\n");
 1887 	printf("\t-M, --rwmixread <0-100> rwmixread (100 for reads, 0 for writes)\n");
 1888 	printf("\t-t, --time <sec> time in seconds\n");
 1889 	printf("\t-a, --warmup-time <sec> warmup time in seconds\n");
 1890 	printf("\t-c, --core-mask <mask> core mask for I/O submission/completion.\n");
```

原件 L1917–L1920：

```c
 1917 	printf("\t-C, --max-completion-per-poll <val> max completions per poll\n");
 1918 	printf("\t\t(default: 0 - unlimited)\n");
 1919 	printf("\t-i, --shmem-grp-id <id> shared memory group ID\n");
 1920 	printf("\t-d, --number-ios <val> number of I/O to perform per thread on each namespace. Note: this is additional exit criteria.\n");
```

原件 L2561–L2564：

```c
 2561 			switch (op) {
 2562 			case PERF_WARMUP_TIME:
 2563 				g_warmup_time_in_sec = val;
 2564 				break;
```

原件 L2833–L2837：

```c
 2833 	if (!g_time_in_sec) {
 2834 		fprintf(stderr, "missing -t (--time) operand\n");
 2835 		usage(argv[0]);
 2836 		return 1;
 2837 	}
```

原件 L2886–L2894：

```c
 2886 	if (g_number_ios && g_warmup_time_in_sec) {
 2887 		fprintf(stderr, "-d (--number-ios) with -a (--warmup-time) is not supported\n");
 2888 		return 1;
 2889 	}
 2890 
 2891 	if (g_number_ios && g_number_ios < g_queue_depth) {
 2892 		fprintf(stderr, "-d (--number-ios) less than -q (--io-depth) is not supported\n");
 2893 		return 1;
 2894 	}
```

## PERF-OWNERS-END

原件：[spdk_nvme_perf.c](/home/xvhaocheng/work/iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/sources/spdk_nvme_perf.c)。

原件 L3103–L3119：

```c
 3103 static int
 3104 allocate_ns_worker(struct ns_entry *entry, struct worker_thread *worker)
 3105 {
 3106 	struct ns_worker_ctx	*ns_ctx;
 3107 
 3108 	ns_ctx = calloc(1, sizeof(struct ns_worker_ctx));
 3109 	if (!ns_ctx) {
 3110 		return -1;
 3111 	}
 3112 
 3113 	printf("Associating %s with lcore %d\n", entry->name, worker->lcore);
 3114 	ns_ctx->stats.min_tsc = UINT64_MAX;
 3115 	ns_ctx->entry = entry;
 3116 	ns_ctx->histogram = spdk_histogram_data_alloc();
 3117 	TAILQ_INSERT_TAIL(&worker->ns_ctx, ns_ctx, link);
 3118 
 3119 	return 0;
```

原件 L3129–L3144：

```c
 3129 	/* Each core contains single worker, and namespaces are associated as follows:
 3130 	 * --use-every-core not specified (default):
 3131 	 * 2) equal workers and namespaces - each worker associated with single namespace
 3132 	 * 3) more workers than namespaces - each namespace is associated with one or more workers
 3133 	 * 4) more namespaces than workers - each worker is associated with one or more namespaces
 3134 	 * --use-every-core option enabled - every worker is associated with all namespaces
 3135 	 */
 3136 	if (g_use_every_core) {
 3137 		TAILQ_FOREACH(worker, &g_workers, link) {
 3138 			TAILQ_FOREACH(entry, &g_namespaces, link) {
 3139 				if (allocate_ns_worker(entry, worker) != 0) {
 3140 					return -1;
 3141 				}
 3142 			}
 3143 		}
 3144 		return 0;
```

原件 L3322–L3339：

```c
 3322 	rc = pthread_create(&thread_id, NULL, &nvme_poll_ctrlrs, NULL);
 3323 	if (rc != 0) {
 3324 		fprintf(stderr, "Unable to spawn a thread to poll admin queues.\n");
 3325 		goto cleanup;
 3326 	}
 3327 
 3328 	if (associate_workers_with_ns() != 0) {
 3329 		rc = -1;
 3330 		goto cleanup;
 3331 	}
 3332 
 3333 	rc = pthread_barrier_init(&g_worker_sync_barrier, NULL, g_num_workers);
 3334 	if (rc != 0) {
 3335 		fprintf(stderr, "Unable to initialize thread sync barrier\n");
 3336 		goto cleanup;
 3337 	}
 3338 
 3339 	printf("Initialization complete. Launching workers.\n");
```

原件 L3341–L3390：

```c
 3341 	/* Launch all of the secondary workers */
 3342 	g_main_core = spdk_env_get_current_core();
 3343 	main_worker = NULL;
 3344 	TAILQ_FOREACH(worker, &g_workers, link) {
 3345 		if (worker->lcore != g_main_core) {
 3346 			spdk_env_thread_launch_pinned(worker->lcore, work_fn, worker);
 3347 		} else {
 3348 			assert(main_worker == NULL);
 3349 			main_worker = worker;
 3350 		}
 3351 	}
 3352 
 3353 	assert(main_worker != NULL);
 3354 	work_fn(main_worker);
 3355 
 3356 	spdk_env_thread_wait_all();
 3357 
 3358 	print_stats();
 3359 
 3360 	pthread_barrier_destroy(&g_worker_sync_barrier);
 3361 
 3362 cleanup:
 3363 	fflush(stdout);
 3364 
 3365 	if (thread_id && pthread_cancel(thread_id) == 0) {
 3366 		pthread_join(thread_id, NULL);
 3367 	}
 3368 
 3369 	/* Collect errors from all workers and namespaces */
 3370 	TAILQ_FOREACH(worker, &g_workers, link) {
 3371 		if (rc != 0) {
 3372 			break;
 3373 		}
 3374 
 3375 		TAILQ_FOREACH(ns_ctx, &worker->ns_ctx, link) {
 3376 			if (ns_ctx->status != 0) {
 3377 				rc = ns_ctx->status;
 3378 				break;
 3379 			}
 3380 		}
 3381 	}
 3382 
 3383 	unregister_trids();
 3384 	unregister_namespaces();
 3385 	unregister_controllers();
 3386 	unregister_workers();
 3387 
 3388 	free_key(&g_psk);
 3389 	spdk_keyring_cleanup();
 3390 	spdk_env_fini();
```

## PCIE-EVENT-CALLBACK

原件：[nvme_pcie_common.c](/home/xvhaocheng/work/iommu-platform/out/d-spdk-contract-20261015/events/nvme_pcie_common.c)。

原件 L624–L637：

```c
  624 void
  625 nvme_pcie_qpair_submit_tracker(struct spdk_nvme_qpair *qpair, struct nvme_tracker *tr)
  626 {
  627 	struct nvme_request	*req;
  628 	struct nvme_pcie_qpair	*pqpair = nvme_pcie_qpair(qpair);
  629 	struct spdk_nvme_ctrlr	*ctrlr = qpair->ctrlr;
  630 
  631 	req = tr->req;
  632 	assert(req != NULL);
  633 
  634 	spdk_trace_record(TRACE_NVME_PCIE_SUBMIT, qpair->id, 0, (uintptr_t)req, req->cb_arg,
  635 			  (uint32_t)req->cmd.cid, (uint32_t)req->cmd.opc,
  636 			  req->cmd.cdw10, req->cmd.cdw11, req->cmd.cdw12,
  637 			  pqpair->qpair.queue_depth);
```

原件 L650–L666：

```c
  650 	if (spdk_unlikely((ctrlr->quirks & NVME_QUIRK_MAXIMUM_PCI_ACCESS_WIDTH) && pqpair->sq_in_cmb)) {
  651 		nvme_pcie_copy_command_mmio(&pqpair->cmd[pqpair->sq_tail], &req->cmd);
  652 	} else {
  653 		/* Copy the command from the tracker to the submission queue. */
  654 		nvme_pcie_copy_command(&pqpair->cmd[pqpair->sq_tail], &req->cmd);
  655 	}
  656 
  657 	if (spdk_unlikely(++pqpair->sq_tail == pqpair->num_entries)) {
  658 		pqpair->sq_tail = 0;
  659 	}
  660 
  661 	if (spdk_unlikely(pqpair->sq_tail == pqpair->sq_head)) {
  662 		SPDK_ERRLOG("sq_tail is passing sq_head!\n");
  663 	}
  664 
  665 	if (!pqpair->flags.delay_cmd_submit) {
  666 		nvme_pcie_qpair_ring_sq_doorbell(qpair);
```

原件 L679–L712：

```c
  679 	req = tr->req;
  680 
  681 	spdk_trace_record(TRACE_NVME_PCIE_COMPLETE, qpair->id, 0, (uintptr_t)req, req->cb_arg,
  682 			  (uint32_t)req->cmd.cid, (uint32_t)cpl->status_raw, pqpair->qpair.queue_depth);
  683 
  684 	assert(req != NULL);
  685 
  686 	error = spdk_nvme_cpl_is_error(cpl);
  687 	retry = error && nvme_completion_is_retry(cpl) &&
  688 		req->retries < pqpair->retry_count;
  689 	print_error = error && print_on_error && !qpair->ctrlr->opts.disable_error_logging;
  690 
  691 	if (print_error) {
  692 		spdk_nvme_qpair_print_command(qpair, &req->cmd);
  693 	}
  694 
  695 	if (print_error || SPDK_DEBUGLOG_FLAG_ENABLED("nvme")) {
  696 		spdk_nvme_qpair_print_completion(qpair, cpl);
  697 	}
  698 
  699 	assert(cpl->cid == req->cmd.cid);
  700 
  701 	if (retry) {
  702 		req->retries++;
  703 		nvme_pcie_qpair_submit_tracker(qpair, tr);
  704 	} else {
  705 		TAILQ_REMOVE(&pqpair->outstanding_tr, tr, tq_list);
  706 		pqpair->qpair.queue_depth--;
  707 
  708 		/* Only check admin requests from different processes. */
  709 		if (nvme_qpair_is_admin_queue(qpair) && req->pid != getpid()) {
  710 			nvme_pcie_qpair_insert_pending_admin_request(qpair, req, cpl);
  711 		} else {
  712 			nvme_complete_request(tr->cb_fn, tr->cb_arg, qpair, req, cpl);
```

原件 L887–L891：

```c
  887 	while (1) {
  888 		cpl = &pqpair->cpl[pqpair->cq_head];
  889 
  890 		if (!next_is_valid && cpl->status.p != pqpair->flags.phase) {
  891 			break;
```

原件 L923–L931：

```c
  923 		tr = &pqpair->tr[cpl->cid];
  924 		pqpair->sq_head = cpl->sqhd;
  925 
  926 		if (tr->req) {
  927 			/* Prefetch the req's STAILQ_ENTRY since we'll need to access it
  928 			 * as part of putting the req back on the qpair's free list.
  929 			 */
  930 			__builtin_prefetch(&tr->req->stailq);
  931 			nvme_pcie_qpair_complete_tracker(qpair, tr, cpl, true);
```

原件 L1834–L1854：

```c
 1834 	struct spdk_trace_tpoint_opts opts[] = {
 1835 		{
 1836 			"NVME_PCIE_SUBMIT", TRACE_NVME_PCIE_SUBMIT,
 1837 			OWNER_TYPE_NVME_PCIE_QP, OBJECT_NVME_PCIE_REQ, 1,
 1838 			{	{ "ctx", SPDK_TRACE_ARG_TYPE_PTR, 8 },
 1839 				{ "cid", SPDK_TRACE_ARG_TYPE_INT, 4 },
 1840 				{ "opc", SPDK_TRACE_ARG_TYPE_INT, 4 },
 1841 				{ "dw10", SPDK_TRACE_ARG_TYPE_PTR, 4 },
 1842 				{ "dw11", SPDK_TRACE_ARG_TYPE_PTR, 4 },
 1843 				{ "dw12", SPDK_TRACE_ARG_TYPE_PTR, 4 },
 1844 				{ "qd", SPDK_TRACE_ARG_TYPE_INT, 4 }
 1845 			}
 1846 		},
 1847 		{
 1848 			"NVME_PCIE_COMPLETE", TRACE_NVME_PCIE_COMPLETE,
 1849 			OWNER_TYPE_NVME_PCIE_QP, OBJECT_NVME_PCIE_REQ, 0,
 1850 			{	{ "ctx", SPDK_TRACE_ARG_TYPE_PTR, 8 },
 1851 				{ "cid", SPDK_TRACE_ARG_TYPE_INT, 4 },
 1852 				{ "cpl", SPDK_TRACE_ARG_TYPE_PTR, 4 },
 1853 				{ "qd", SPDK_TRACE_ARG_TYPE_INT, 4 }
 1854 			}
```
