# Checked S1 flush API — implementation proposal

Scope note: this stays inside the allowed files. No DMA/RTL work, no changes to `riscv_iommu_post`, `riscv_iommu_flush_iotlb_*`, or queue IRQ behavior beyond the new seq counter bookkeeping. The helper is placed in `iommu.c` (it needs `riscv_iommu_post`, `riscv_iommu_iofence_sync`, the command constructors and the cmdq internals); the public wrapper stays in `iommu-hpm.c`.

I could not run anything. All statements below are proposals for Codex to apply/verify.

---

## 1. `linux/include/linux/riscv_iommu_hpm.h`

```diff
--- a/linux/include/linux/riscv_iommu_hpm.h
+++ b/linux/include/linux/riscv_iommu_hpm.h
@@ -31,9 +31,17 @@ struct riscv_iommu_hpm_sample {
  * Event counts start at acquire; cycles are never reset. Samples are read
  * sequentially, not atomically. Use differences around the desired window.
  * Release restores saved selectors, event counts and inhibit state. Restored
  * event counts exclude activity during the owned session; cycles retain it.
+ *
+ * flush_s1 issues a global host first-stage invalidation barrier for the
+ * session's devices. It must be called by the task that acquired the session
+ * while it still owns hpm_lock, after the caller has quiesced the relevant DMA
+ * and serialized binding changes. It does not reset ATS/PR/PW or any other
+ * internal IOMMU state and does not retry or reset on failure. A NULL session
+ * returns -EINVAL; when no RISC-V IOMMU is reachable it returns -EOPNOTSUPP.
  */
 #if IS_REACHABLE(CONFIG_RISCV_IOMMU)
 bool riscv_iommu_hpm_supported(struct device *dev);
 int riscv_iommu_hpm_acquire(struct device *victim, struct device *aggressor,
 			   struct riscv_iommu_hpm_session **out);
+int riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *session);
 void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *session,
 			  struct riscv_iommu_hpm_sample *sample);
 void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *session);
@@ -54,6 +62,12 @@ riscv_iommu_hpm_acquire(struct device *victim, struct device *aggressor,
 	return -EOPNOTSUPP;
 }
 
+static inline int
+riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *session)
+{
+	return -EOPNOTSUPP;
+}
+
 static inline void
 riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *session,
 		     struct riscv_iommu_hpm_sample *sample)
```

---

## 2. `linux/drivers/iommu/riscv/iommu.h`

```diff
--- a/linux/drivers/iommu/riscv/iommu.h
+++ b/linux/drivers/iommu/riscv/iommu.h
@@ -100,6 +100,9 @@ struct riscv_iommu_device {
 	/* Serializes owned HPM sessions and raw sysfs HPM writes. */
 	struct mutex hpm_lock;
 	bool hpm_ready;
+
+	/* Incremented by the CQ IRQ thread whenever it reports a CQ error. */
+	atomic64_t cq_error_seq;
 };
 
 struct riscv_iommu_domain {
@@ -152,6 +155,17 @@ struct riscv_iommu_endpoint {
 	bool ir_enabled;
 };
 
+/*
+ * Acquire-time snapshot of one device targeted by an HPM session flush: the
+ * bound device, the S1 domain attached to it and its device id. The session
+ * holds no reference; the caller keeps the device bound and serializes work.
+ */
+struct riscv_iommu_s1_flush_target {
+	struct device *dev;
+	struct iommu_domain *domain;
+	u32 did;
+};
+
 /* Helper functions and macros */
 
 static inline u32 riscv_iommu_readl(struct riscv_iommu_device *iommu,
@@ -185,6 +199,10 @@ int riscv_iommu_sysfs_add(struct riscv_iommu_device *iommu);
 
 struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev);
 
+int riscv_iommu_flush_s1_checked(struct riscv_iommu_device *iommu,
+				 const struct riscv_iommu_s1_flush_target *targets,
+				 unsigned int nr_targets);
+
 static inline bool riscv_iommu_hpm_register(unsigned int offset)
 {
 	return offset >= RISCV_IOMMU_REG_IOCOUNTOVF &&
```

---

## 3. `linux/drivers/iommu/riscv/iommu.c`

```diff
--- a/linux/drivers/iommu/riscv/iommu.c
+++ b/linux/drivers/iommu/riscv/iommu.c
@@ -19,6 +19,7 @@
 #include <linux/pci.h>
 #include <linux/pci-ats.h>
 #include <linux/init.h>
+#include <linux/atomic.h>
 #include <linux/completion.h>
 #include <linux/uaccess.h>
 #include <linux/iommu.h>
@@ -644,6 +645,7 @@ static irqreturn_t riscv_iommu_cmdq_process(int irq, void *data)
 	ctrl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQCSR);
 	if (ctrl & (RISCV_IOMMU_CQCSR_CQMF |
 		    RISCV_IOMMU_CQCSR_CMD_TO | RISCV_IOMMU_CQCSR_CMD_ILL)) {
+		atomic64_inc(&iommu->cq_error_seq);
 		riscv_iommu_queue_ctrl(iommu, &iommu->cmdq, ctrl);
 		dev_warn_ratelimited(iommu->dev,
 				     "Command queue error: fault: %d tout: %d err: %d\n",
@@ -2119,6 +2121,7 @@ int riscv_iommu_init(struct riscv_iommu_device *iommu)
 		   RISCV_IOMMU_IPSR_PMIP | RISCV_IOMMU_IPSR_PIP);
 	spin_lock_init(&iommu->cq_lock);
 	mutex_init(&iommu->eps_mutex);
+	atomic64_set(&iommu->cq_error_seq, 0);
 	ret = riscv_iommu_queue_init(iommu, RISCV_IOMMU_COMMAND_QUEUE);
 	if (ret)
 		goto fail;
@@ -2053,6 +2056,169 @@ struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev)
 		return NULL;
 	return ep;
 }
 
+/*
+ * Accept only a plain ordinary Sv39 host first-stage domain whose device
+ * context still matches that domain. Called with ep->lock held, and without
+ * domain->lock (attach takes domain->lock before ep->lock).
+ */
+static int riscv_iommu_flush_s1_check_ep(struct riscv_iommu_endpoint *ep,
+					 struct riscv_iommu_domain *domain)
+{
+	struct riscv_iommu_dc *dc = ep->dc;
+	u64 tc, fsc, iohgatp;
+
+	if (!dc)
+		return -EOPNOTSUPP;
+
+	/* No S2, no nesting, no SVA/PASID: explicit host S1 only. */
+	if (domain->g_stage || domain->nested ||
+	    domain->domain.type == IOMMU_DOMAIN_SVA || ep->pasid_enabled ||
+	    domain->mode == RISCV_IOMMU_DC_FSC_MODE_BARE)
+		return -EOPNOTSUPP;
+
+	tc = le64_to_cpu(dc->tc);
+	fsc = le64_to_cpu(dc->fsc);
+	iohgatp = le64_to_cpu(dc->iohgatp);
+
+	if (!(tc & RISCV_IOMMU_DC_TC_V))
+		return -EOPNOTSUPP;
+	if (tc & (RISCV_IOMMU_DC_TC_EN_ATS | RISCV_IOMMU_DC_TC_EN_PRI |
+		  RISCV_IOMMU_DC_TC_PDTV | RISCV_IOMMU_DC_TC_T2GPA |
+		  RISCV_IOMMU_DC_TC_SXL))
+		return -EOPNOTSUPP;
+	if (FIELD_GET(RISCV_IOMMU_DC_IOHGATP_MODE, iohgatp) !=
+	    RISCV_IOMMU_DC_IOHGATP_MODE_BARE)
+		return -EOPNOTSUPP;
+	if (domain->mode != RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39)
+		return -EOPNOTSUPP;
+
+	/* Device context no longer matches the live domain: stale snapshot. */
+	if (FIELD_GET(RISCV_IOMMU_DC_FSC_MODE, fsc) != domain->mode ||
+	    FIELD_GET(RISCV_IOMMU_DC_FSC_PPN, fsc) !=
+		    virt_to_pfn(domain->pgd_root))
+		return -ESTALE;
+
+	return 0;
+}
+
+/*
+ * Global host first-stage invalidation barrier for one or two devices of the
+ * same IOMMU. The caller owns the HPM session (hpm_lock), has quiesced the
+ * relevant DMA and serializes binding changes. This only issues IOTINVAL.VMA
+ * with GV/AV/PSCV clear followed by a fence; it is not an ATS/PR/PW or
+ * general internal-state reset, and it never retries or resets on failure.
+ *
+ * Note on the current RTL: CQH advances when the fetch of the last beat
+ * completes, before decode. For this strict no-ATS/no-S2, quiesced
+ * experiment the CQ decodes the earlier VMA (its invalidations are issued
+ * before the fetch of the following IOFENCE), so CQH passing the following
+ * fence is sufficient evidence that the VMA pulse was issued. This is not a
+ * general fence-execution proof.
+ */
+int riscv_iommu_flush_s1_checked(struct riscv_iommu_device *iommu,
+				 const struct riscv_iommu_s1_flush_target *targets,
+				 unsigned int nr_targets)
+{
+	struct riscv_iommu_endpoint *ep[2] = { };
+	struct riscv_iommu_command cmd;
+	unsigned long flags;
+	unsigned int first, second, i;
+	u32 ctrl, tail;
+	u64 seq;
+	int ret;
+
+	if (!iommu || !targets || (nr_targets != 1 && nr_targets != 2))
+		return -EINVAL;
+	if (nr_targets == 2 &&
+	    (targets[0].dev == targets[1].dev ||
+	     targets[0].did == targets[1].did))
+		return -EINVAL;
+
+	for (i = 0; i < nr_targets; i++) {
+		if (!targets[i].dev || !targets[i].domain)
+			return -EINVAL;
+
+		ep[i] = riscv_iommu_hpm_endpoint(targets[i].dev);
+		if (!ep[i])
+			return -EOPNOTSUPP;
+		if (ep[i]->iommu != iommu)
+			return -EXDEV;
+		if (ep[i]->devid != targets[i].did)
+			return -ESTALE;
+	}
+	if (nr_targets == 2 && ep[0] == ep[1])
+		return -EINVAL;
+
+	/* Take both endpoint mutexes, always in ascending device id order. */
+	if (nr_targets == 2 && ep[0]->devid > ep[1]->devid) {
+		first = 1;
+		second = 0;
+	} else {
+		first = 0;
+		second = nr_targets - 1;
+	}
+	mutex_lock(&ep[first]->lock);
+	if (nr_targets == 2)
+		mutex_lock(&ep[second]->lock);
+
+	ret = 0;
+	for (i = 0; i < nr_targets; i++) {
+		/*
+		 * Re-read the live attachment under ep->lock, then dereference
+		 * the domain snapshot taken by acquire.
+		 */
+		if (!ep[i]->domain ||
+		    &ep[i]->domain->domain != targets[i].domain) {
+			ret = -ESTALE;
+			break;
+		}
+		ret = riscv_iommu_flush_s1_check_ep(ep[i], ep[i]->domain);
+		if (ret)
+			break;
+	}
+	if (ret)
+		goto unlock;
+
+	seq = atomic64_read(&iommu->cq_error_seq);
+
+	if (!iommu->cmdq.base || !iommu->cmdq.cnt ||
+	    (iommu->cmdq.cnt & (iommu->cmdq.cnt - 1)) ||
+	    iommu->cmdq.lui >= iommu->cmdq.cnt) {
+		ret = -EIO;
+		goto unlock;
+	}
+
+	ctrl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQCSR);
+	if (!(ctrl & RISCV_IOMMU_CQCSR_CQEN) ||
+	    !(ctrl & RISCV_IOMMU_CQCSR_CQON) ||
+	    (ctrl & (RISCV_IOMMU_CQCSR_CQMF | RISCV_IOMMU_CQCSR_CMD_TO |
+		     RISCV_IOMMU_CQCSR_CMD_ILL))) {
+		ret = -EIO;
+		goto unlock;
+	}
+
+	/* Nothing may be posted but not yet fetched. cq_lock is dropped. */
+	spin_lock_irqsave(&iommu->cq_lock, flags);
+	tail = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQT) &
+	       (iommu->cmdq.cnt - 1);
+	spin_unlock_irqrestore(&iommu->cq_lock, flags);
+	if (tail != iommu->cmdq.lui) {
+		ret = -EIO;
+		goto unlock;
+	}
+
+	/* IOTINVAL.VMA with GV, AV and PSCV clear: every host S1 entry. */
+	riscv_iommu_cmd_inval_vma(&cmd);
+	if (!riscv_iommu_post(iommu, &cmd)) {
+		ret = -EIO;
+		goto unlock;
+	}
+	if (!riscv_iommu_iofence_sync(iommu)) {
+		ret = -EIO;
+		goto unlock;
+	}
+
+	ctrl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQCSR);
+	if (atomic64_read(&iommu->cq_error_seq) != seq ||
+	    (ctrl & (RISCV_IOMMU_CQCSR_CQMF | RISCV_IOMMU_CQCSR_CMD_TO |
+		     RISCV_IOMMU_CQCSR_CMD_ILL))) {
+		ret = -EIO;
+		goto unlock;
+	}
+
+unlock:
+	if (nr_targets == 2)
+		mutex_unlock(&ep[second]->lock);
+	mutex_unlock(&ep[first]->lock);
+	return ret;
+}
+
 void riscv_iommu_remove(struct riscv_iommu_device *iommu)
 {
 	/* An owner finishes restoring its HPM state before teardown proceeds. */
```

---

## 4. `linux/drivers/iommu/riscv/iommu-hpm.c`

```diff
--- a/linux/drivers/iommu/riscv/iommu-hpm.c
+++ b/linux/drivers/iommu/riscv/iommu-hpm.c
@@ -15,6 +15,8 @@ struct riscv_iommu_hpm_session {
 	struct riscv_iommu_device *iommu;
 	u32 saved_inhibit;
 	u64 saved_event[HPM_SESSION_COUNTERS];
 	u64 saved_counter[HPM_SESSION_COUNTERS];
+	unsigned int nr_targets;
+	struct riscv_iommu_s1_flush_target targets[2];
 };
@@ -159,6 +161,18 @@ int riscv_iommu_hpm_acquire(struct device *victim, struct device *aggressor,
 		ret = -EOPNOTSUPP;
 		goto restore;
 	}
+	/*
+	 * Snapshot the targets for a later riscv_iommu_hpm_flush_s1(). No
+	 * device/domain reference is taken: the caller keeps both devices
+	 * bound and calls flush from the task that acquired the session.
+	 */
+	session->nr_targets = aggressor ? 2 : 1;
+	for (i = 0; i < session->nr_targets; i++) {
+		session->targets[i].dev = i ? aggressor : victim;
+		session->targets[i].domain =
+		    ep[i]->domain ? &ep[i]->domain->domain : NULL;
+		session->targets[i].did = ep[i]->devid;
+	}
 	*out = session;
 	return 0;
 
@@ -204,3 +218,12 @@ void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *session)
 	kfree(session);
 }
 EXPORT_SYMBOL_GPL(riscv_iommu_hpm_release);
+
+int riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *session)
+{
+	if (!session)
+		return -EINVAL;
+	lockdep_assert_held(&session->iommu->hpm_lock);
+	return riscv_iommu_flush_s1_checked(session->iommu, session->targets,
+					    session->nr_targets);
+}
+EXPORT_SYMBOL_GPL(riscv_iommu_hpm_flush_s1);
```

---

## 5. `tests/test_iommu_hpm_session.c`

```diff
--- a/tests/test_iommu_hpm_session.c
+++ b/tests/test_iommu_hpm_session.c
@@ -27,13 +27,48 @@ struct riscv_iommu_device {
 	struct mutex hpm_lock;
 	bool hpm_ready;
 };
+
+struct iommu_domain { int unused; };
+struct riscv_iommu_domain { struct iommu_domain domain; };
+
 struct riscv_iommu_endpoint {
 	struct riscv_iommu_device *iommu;
 	u32 devid;
+	struct riscv_iommu_domain *domain;
 };
 struct device { struct riscv_iommu_endpoint *ep; };
+struct riscv_iommu_s1_flush_target {
+	struct device *dev;
+	struct iommu_domain *domain;
+	u32 did;
+};
 
 static struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev)
 {
 	return dev ? dev->ep : NULL;
 }
+
+/* Captures what the production wrapper forwards to the private helper. */
+static struct riscv_iommu_device *flushed_iommu;
+static struct riscv_iommu_s1_flush_target flushed_targets[2];
+static unsigned int flushed_nr;
+static int flush_result;
+
+static int riscv_iommu_flush_s1_checked(struct riscv_iommu_device *iommu,
+		const struct riscv_iommu_s1_flush_target *targets,
+		unsigned int nr_targets)
+{
+	unsigned int i;
+
+	flushed_iommu = iommu;
+	flushed_nr = nr_targets;
+	for (i = 0; i < nr_targets && i < 2; i++)
+		flushed_targets[i] = targets[i];
+	return flush_result;
+}
@@ -91,6 +126,7 @@ static struct mmio banks[2];
 static struct riscv_iommu_device iommus[2];
 static struct riscv_iommu_endpoint endpoints[3];
 static struct device devices[4];
+static struct riscv_iommu_domain domains[3];
 static unsigned int cases;
@@ -211,6 +247,8 @@ static void setup(void)
 	memset(banks, 0, sizeof(banks));
 	memset(iommus, 0, sizeof(iommus));
 	allocation_fail = 0;
+	flush_result = 0;
+	flushed_nr = 0;
 	for (lane = 0; lane < 2; lane++) {
 		iommus[lane].reg = banks[lane].regs;
 		iommus[lane].hpm_ready = true;
@@ -223,9 +261,9 @@ static void setup(void)
 			raw_write(&banks[lane], RISCV_IOMMU_REG_IOHPMCTR(i), 0x100000010ULL + i);
 		}
 	}
-	endpoints[0] = (struct riscv_iommu_endpoint){ .iommu = &iommus[0], .devid = 10 };
-	endpoints[1] = (struct riscv_iommu_endpoint){ .iommu = &iommus[0], .devid = 11 };
-	endpoints[2] = (struct riscv_iommu_endpoint){ .iommu = &iommus[1], .devid = 12 };
+	endpoints[0] = (struct riscv_iommu_endpoint){ .iommu = &iommus[0], .devid = 10, .domain = &domains[0] };
+	endpoints[1] = (struct riscv_iommu_endpoint){ .iommu = &iommus[0], .devid = 11, .domain = &domains[1] };
+	endpoints[2] = (struct riscv_iommu_endpoint){ .iommu = &iommus[1], .devid = 12, .domain = &domains[2] };
 	for (i = 0; i < 3; i++) devices[i].ep = &endpoints[i];
 	devices[3].ep = NULL;
 }
@@ -270,6 +308,24 @@ static void check_trial(bool solo)
 	assert(sample.s2_ptw[1] == (solo ? 0 : 0x8000000000000025ULL));
+
+	/* The wrapper forwards the acquire-time snapshot to the helper. */
+	assert(riscv_iommu_hpm_flush_s1(session) == 0);
+	assert(flushed_iommu == &iommus[0]);
+	assert(flushed_nr == (solo ? 1U : 2U));
+	assert(flushed_targets[0].dev == &devices[0]);
+	assert(flushed_targets[0].domain == &domains[0].domain);
+	assert(flushed_targets[0].did == 10);
+	if (!solo) {
+		assert(flushed_targets[1].dev == &devices[1]);
+		assert(flushed_targets[1].domain == &domains[1].domain);
+		assert(flushed_targets[1].did == 11);
+	}
+	flush_result = -EIO;
+	assert(riscv_iommu_hpm_flush_s1(session) == -EIO);
+	flush_result = 0;
+
 	riscv_iommu_hpm_release(session);
 	assert(!memcmp(saved, banks[0].regs, sizeof(saved)));
 	assert(!banks[0].cycle_writes && !iommus[0].hpm_lock.held && !allocated);
@@ -309,6 +365,7 @@ static void check_rejections(void)
 	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, NULL) == -EINVAL);
+	assert(riscv_iommu_hpm_flush_s1(NULL) == -EINVAL);
 	assert(riscv_iommu_hpm_acquire(&devices[3], NULL, &session) == -EOPNOTSUPP && !session);
```

`tests/test_iommu_hpm_session.py` needs no change: only kernel includes are stripped and the new wrapper references no new `RISCV_IOMMU_*` macros. Existing expectations are preserved (extra snapshot/forwarding asserts are additive).

---

## 6. `newtests/test_iommu_s1_flush.c` (new)

```c
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Host harness for the production riscv_iommu_flush_s1_checked() path.
 *
 * The production helper bodies (riscv_iommu_cmd_inval_vma,
 * riscv_iommu_flush_s1_check_ep and riscv_iommu_flush_s1_checked) are
 * extracted from iommu.c and compiled unchanged; only their kernel
 * dependencies are mocked. This models control flow, command encoding and
 * locking, not RTL behavior.
 */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned long pgd_t;
typedef unsigned long resource_size_t;
typedef unsigned long dma_addr_t;
typedef u32 ioasid_t;

#define PAGE_SHIFT 12
#define __iomem
#define BIT(n) (UINT64_C(1) << (n))
#define BIT_ULL(n) BIT(n)
#define GENMASK(h, l) ((UINT64_MAX >> (63 - (h))) & (UINT64_MAX << (l)))
#define GENMASK_ULL(h, l) GENMASK(h, l)
#define FIELD_PREP(mask, val) (((u64)(val) << __builtin_ctzll(mask)) & (mask))
#define FIELD_GET(mask, val) (((u64)(val) & (mask)) >> __builtin_ctzll(mask))

#define le64_to_cpu(v) ((u64)(v))
#define cpu_to_le64(v) ((u64)(v))
#define virt_to_pfn(p) ((unsigned long)(uintptr_t)(p) >> PAGE_SHIFT)

/*
 * Values from <linux/iommu.h> enum iommu_domain_type. They are the only
 * constants not extracted from the real headers; the harness only needs a
 * non-SVA value and the SVA value used by the production rejection.
 */
#define IOMMU_DOMAIN_DMA 3
#define IOMMU_DOMAIN_SVA 5

struct device;
struct iopf_queue;
struct iommu_device { int unused; };
struct mutex { bool held; };
struct rb_node { int unused; };
struct rb_root { struct rb_node *rb_node; };
struct list_head { struct list_head *next, *prev; };
struct io_pgtable { int unused; };
struct mmu_notifier { int unused; };
struct iommu_domain { unsigned int type; };
typedef struct { bool held; } spinlock_t;
typedef struct { long counter; } atomic64_t;

#define atomic64_read(a) ((a)->counter)
#define atomic64_inc(a) ((a)->counter++)
#define atomic64_set(a, v) ((a)->counter = (v))

static u8 s1_regs[2][0x100];

static u32 readl_relaxed(const void *addr)
{
	u32 value;

	memcpy(&value, addr, sizeof(value));
	return value;
}

static void writel_relaxed(u32 value, void *addr)
{
	memcpy(addr, &value, sizeof(value));
}

static u64 readq_relaxed(const void *addr)
{
	u64 value;

	memcpy(&value, addr, sizeof(value));
	return value;
}

static void writeq_relaxed(u64 value, void *addr)
{
	memcpy(addr, &value, sizeof(value));
}

#include "production_bits.h"
#include "production_iommu.h"

struct device { struct riscv_iommu_endpoint *s1_ep; };

/* ---- mocked dependencies ---- */

static int post_result = 1, fence_result = 1, post_bump_seq;
static unsigned int post_count, fence_count;
static struct riscv_iommu_command posted_cmd;
static struct riscv_iommu_endpoint *locked_eps[2];
static unsigned int locked_nr;

static void mutex_lock(struct mutex *lock)
{
	assert(!lock->held);
	lock->held = true;
}

static void mutex_unlock(struct mutex *lock)
{
	assert(lock->held);
	lock->held = false;
}

static bool riscv_iommu_post(struct riscv_iommu_device *iommu,
			     struct riscv_iommu_command *cmd)
{
	unsigned int i;

	for (i = 0; i < locked_nr; i++)
		assert(locked_eps[i]->lock.held);
	assert(post_count == 0);
	posted_cmd = *cmd;
	post_count++;
	if (post_bump_seq)
		atomic64_inc(&iommu->cq_error_seq);
	return post_result;
}

static bool riscv_iommu_iofence_sync(struct riscv_iommu_device *iommu)
{
	unsigned int i;

	(void)iommu;
	for (i = 0; i < locked_nr; i++)
		assert(locked_eps[i]->lock.held);
	assert(post_count == 1);
	assert(fence_count == 0);
	fence_count++;
	return fence_result;
}

static struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev)
{
	return dev ? dev->s1_ep : NULL;
}

#include "production_s1_flush.c"

/* ---- fixtures ---- */

static struct riscv_iommu_device iommus[2];
static struct riscv_iommu_endpoint eps[3];
static struct device devices[3];
static struct riscv_iommu_domain domains[3];
static struct riscv_iommu_dc dcs[3];
static pgd_t pgd_roots[3];
static u8 cmdq_mem[2][64];
static unsigned int cases;

static void reg_write(unsigned int bank, unsigned int off, u32 value)
{
	assert(off + sizeof(value) <= sizeof(s1_regs[bank]));
	memcpy(s1_regs[bank] + off, &value, sizeof(value));
}

static void setup(void)
{
	unsigned int i;

	memset(s1_regs, 0, sizeof(s1_regs));
	memset(iommus, 0, sizeof(iommus));
	memset(eps, 0, sizeof(eps));
	memset(devices, 0, sizeof(devices));
	memset(domains, 0, sizeof(domains));
	memset(dcs, 0, sizeof(dcs));

	post_count = fence_count = 0;
	post_result = fence_result = 1;
	post_bump_seq = 0;
	locked_nr = 0;

	for (i = 0; i < 2; i++) {
		iommus[i].reg = s1_regs[i];
		iommus[i].cmdq.base = cmdq_mem[i];
		iommus[i].cmdq.cnt = 8;
		iommus[i].cmdq.lui = 0;
		reg_write(i, RISCV_IOMMU_REG_CQCSR,
			  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON);
		reg_write(i, RISCV_IOMMU_REG_CQT, 0);
	}

	for (i = 0; i < 3; i++) {
		devices[i].s1_ep = &eps[i];
		eps[i].iommu = &iommus[i == 2 ? 1 : 0];
		eps[i].devid = 10 + i;
		eps[i].domain = &domains[i];
		eps[i].dc = &dcs[i];
		eps[i].pasid_enabled = false;

		domains[i].domain.type = IOMMU_DOMAIN_DMA;
		domains[i].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;
		domains[i].pgd_root = &pgd_roots[i];
		domains[i].g_stage = false;
		domains[i].nested = NULL;

		dcs[i].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V);
		dcs[i].iohgatp = 0;
		dcs[i].fsc = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
				   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
			FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
				   virt_to_pfn(domains[i].pgd_root)));
	}
}

static struct riscv_iommu_s1_flush_target mk(unsigned int idx)
{
	struct riscv_iommu_s1_flush_target target;

	target.dev = &devices[idx];
	target.domain = &domains[idx].domain;
	target.did = 10 + idx;
	return target;
}

static int run(unsigned int iommu,
	       const struct riscv_iommu_s1_flush_target *targets,
	       unsigned int nr_targets)
{
	int ret;

	post_count = fence_count = 0;
	ret = riscv_iommu_flush_s1_checked(&iommus[iommu], targets, nr_targets);
	assert(!eps[0].lock.held && !eps[1].lock.held && !eps[2].lock.held);
	cases++;
	return ret;
}

/* Rejection that must never reach command submission. */
static void reject(unsigned int iommu,
		   const struct riscv_iommu_s1_flush_target *targets,
		   unsigned int nr_targets, int expected)
{
	assert(run(iommu, targets, nr_targets) == expected);
	assert(!post_count && !fence_count);
}

int main(void)
{
	struct riscv_iommu_s1_flush_target t[2];
	u64 expected;

	setup();
	locked_eps[0] = &eps[0];
	locked_eps[1] = &eps[1];
	locked_nr = 1;

	/* Solo success: exactly one global VMA command, then a fence. */
	t[0] = mk(0);
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);
	expected = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE,
			      RISCV_IOMMU_CMD_IOTINVAL_OPCODE) |
		   FIELD_PREP(RISCV_IOMMU_CMD_FUNC,
			      RISCV_IOMMU_CMD_IOTINVAL_FUNC_VMA);
	assert(posted_cmd.dword0 == expected);
	assert(posted_cmd.dword1 == 0);
	assert(!(posted_cmd.dword0 & (RISCV_IOMMU_CMD_IOTINVAL_AV |
				      RISCV_IOMMU_CMD_IOTINVAL_PSCV |
				      RISCV_IOMMU_CMD_IOTINVAL_GV)));

	/* Pair success, exercising both lock orders. */
	t[0] = mk(0);
	t[1] = mk(1);
	locked_nr = 2;
	assert(run(0, t, 2) == 0);
	assert(post_count == 1 && fence_count == 1);
	t[0] = mk(1);
	t[1] = mk(0);
	assert(run(0, t, 2) == 0);
	assert(post_count == 1 && fence_count == 1);
	locked_nr = 1;

	/* Parameter and duplicate rejection. */
	t[0] = mk(0);
	assert(run(0, t, 0) == -EINVAL);
	assert(run(0, t, 3) == -EINVAL);
	assert(run(0, NULL, 1) == -EINVAL);
	t[0] = mk(0);
	t[0].dev = NULL;
	reject(0, t, 1, -EINVAL);
	t[0] = mk(0);
	t[0].domain = NULL;
	reject(0, t, 1, -EINVAL);
	t[0] = mk(0);
	t[1] = mk(0);
	reject(0, t, 2, -EINVAL);
	t[0] = mk(0);
	t[1] = mk(1);
	t[1].did = t[0].did;
	reject(0, t, 2, -EINVAL);

	/* Identity: unbound device, other IOMMU, changed DID, changed domain. */
	t[0] = mk(0);
	devices[0].s1_ep = NULL;
	reject(0, t, 1, -EOPNOTSUPP);
	devices[0].s1_ep = &eps[0];
	reject(1, t, 1, -EXDEV);
	t[0] = mk(0);
	t[0].did = eps[0].devid + 1;
	reject(0, t, 1, -ESTALE);
	t[0] = mk(0);
	eps[0].domain = &domains[1];
	reject(0, t, 1, -ESTALE);
	eps[0].domain = NULL;
	reject(0, t, 1, -ESTALE);
	eps[0].domain = &domains[0];

	/* Domain shape: BARE, S2, nesting, SVA/PASID. */
	t[0] = mk(0);
	domains[0].mode = RISCV_IOMMU_DC_FSC_MODE_BARE;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;
	domains[0].g_stage = true;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].g_stage = false;
	domains[0].nested = &domains[1];
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].nested = NULL;
	domains[0].domain.type = IOMMU_DOMAIN_SVA;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].domain.type = IOMMU_DOMAIN_DMA;
	eps[0].pasid_enabled = true;
	reject(0, t, 1, -EOPNOTSUPP);
	eps[0].pasid_enabled = false;

	/* Device context bits. */
	t[0] = mk(0);
	dcs[0].tc = cpu_to_le64(0);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_EN_ATS);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_EN_PRI);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_PDTV);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_T2GPA);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_SXL);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V);
	dcs[0].iohgatp = cpu_to_le64(
		FIELD_PREP(RISCV_IOMMU_DC_IOHGATP_MODE,
			   RISCV_IOMMU_DC_IOHGATP_MODE_SV39X4));
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].iohgatp = 0;
	eps[0].dc = NULL;
	reject(0, t, 1, -EOPNOTSUPP);
	eps[0].dc = &dcs[0];

	/* Non-Sv39 mode, then FSC/domain disagreement. */
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV48;
	dcs[0].fsc = cpu_to_le64(
		FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
			   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV48) |
		FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
			   virt_to_pfn(domains[0].pgd_root)));
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;
	dcs[0].fsc = cpu_to_le64(
		FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
			   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
		FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN, virt_to_pfn(&pgd_roots[1])));
	reject(0, t, 1, -ESTALE);
	dcs[0].fsc = cpu_to_le64(
		FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
			   RISCV_IOMMU_DC_FSC_MODE_BARE) |
		FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
			   virt_to_pfn(domains[0].pgd_root)));
	reject(0, t, 1, -ESTALE);
	dcs[0].fsc = cpu_to_le64(
		FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
			   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
		FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
			   virt_to_pfn(domains[0].pgd_root)));

	/* Command queue gating. */
	t[0] = mk(0);
	reg_write(0, RISCV_IOMMU_REG_CQCSR, RISCV_IOMMU_CQCSR_CQON);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR, RISCV_IOMMU_CQCSR_CQEN);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CQMF);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CMD_ILL);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CMD_TO);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON);

	reg_write(0, RISCV_IOMMU_REG_CQT, 1);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQT, 0);

	iommus[0].cmdq.base = NULL;
	reject(0, t, 1, -EIO);
	iommus[0].cmdq.base = cmdq_mem[0];
	iommus[0].cmdq.cnt = 0;
	reject(0, t, 1, -EIO);
	iommus[0].cmdq.cnt = 6;
	reject(0, t, 1, -EIO);
	iommus[0].cmdq.cnt = 8;
	iommus[0].cmdq.lui = 8;
	reject(0, t, 1, -EIO);
	iommus[0].cmdq.lui = 0;

	/* Submission outcomes. */
	post_result = 0;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 0);
	post_result = 1;

	fence_result = 0;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 1);
	fence_result = 1;

	/* IRQ thread observed a CQ error even though CQCSR is now clean. */
	post_bump_seq = 1;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 1);
	post_bump_seq = 0;
	assert(!(riscv_iommu_readl(&iommus[0], RISCV_IOMMU_REG_CQCSR) &
		 (RISCV_IOMMU_CQCSR_CQMF | RISCV_IOMMU_CQCSR_CMD_TO |
		  RISCV_IOMMU_CQCSR_CMD_ILL)));

	/* Everything restored still flushes once. */
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);

	printf("PASS: %u production S1 flush cases (host MMIO/lock/command model)\n",
	       cases);
	return 0;
}
```

## 7. `newtests/test_iommu_s1_flush.py` (new)

```python
#!/usr/bin/env python3
"""Run the production host S1 flush helper against host-controlled MMIO/locks.

Only kernel includes are removed; the production helper bodies are extracted
from iommu.c and compiled unchanged. This checks driver control flow, command
encoding and locking, not RTL behavior or board counters.
"""

import hashlib
import os
from pathlib import Path
import re
import resource
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]

HELPERS = (
    "riscv_iommu_cmd_inval_vma",
    "riscv_iommu_flush_s1_check_ep",
    "riscv_iommu_flush_s1_checked",
)


def without_includes(text):
    return re.sub(r"^\s*#include[^\n]*", "", text, flags=re.MULTILINE)


def extract_function(text, name):
    match = re.search(r"^[A-Za-z_][^\n]*\b" + re.escape(name) + r"\s*\(",
                      text, flags=re.MULTILINE)
    assert match, "function not found: " + name
    start = match.start()
    open_brace = text.index("{", match.end())
    depth = 0
    for index in range(open_brace, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return text[start:index + 1]
    raise AssertionError("unterminated function: " + name)


def main():
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    source = ROOT / "linux/drivers/iommu/riscv/iommu.c"
    contents = source.read_text()
    helpers = "\n\n".join(extract_function(contents, name)
                          for name in HELPERS)
    with tempfile.TemporaryDirectory(prefix="iommu-s1-flush-") as temporary:
        temp = Path(temporary)
        (temp / "production_bits.h").write_text(without_includes(
            (ROOT / "linux/drivers/iommu/riscv/iommu-bits.h").read_text()))
        (temp / "production_iommu.h").write_text(without_includes(
            (ROOT / "linux/drivers/iommu/riscv/iommu.h").read_text()))
        (temp / "production_s1_flush.c").write_text(helpers + "\n")
        binary = temp / "s1-flush-test"
        subprocess.run([
            os.environ.get("HOSTCC", "cc"), "-std=gnu11", "-O2", "-g",
            "-Wall", "-Wextra", "-Werror", "-I", str(temp),
            str(ROOT / "newtests/test_iommu_s1_flush.c"), "-o", str(binary),
        ], check=True)
        subprocess.run([str(binary)], check=True)
    print(f"Production iommu.c (S1 helpers) SHA256: "
          f"{hashlib.sha256(contents.encode()).hexdigest()}")
    print("Host control/command/lock checks only; no RTL or board result.")


if __name__ == "__main__":
    main()
```

---

## Assumptions and design decisions (flag for Codex)

1. **Translation unit for the helper.** `riscv_iommu_flush_s1_checked` lives in `iommu.c` because it needs file-static `riscv_iommu_post`/`riscv_iommu_iofence_sync` and the command constructors. This keeps the existing `tests/test_iommu_hpm_session.*` shim change tiny and confines the new harness to one file. If Codex prefers the helper in `iommu-hpm.c`, `riscv_iommu_post`, `riscv_iommu_iofence_sync` and `riscv_iommu_cmd_inval_vma` would have to be exposed via `iommu.h` and the existing test's shim would grow substantially.
2. **`atomic64_t` in `iommu.h`** comes from `<linux/types.h>` (already included). Only `iommu.c` performs atomic ops, so `<linux/atomic.h>` is added there only.
3. **`riscv_iommu_iofence_sync` is assumed unchanged** and its "CQH passed" wait is treated as sufficient only for this quiesced no-ATS/no-S2 experiment, as documented in the helper comment. This is not a general fence-execution proof.
4. **Endpoint locking.** Both `ep->lock` are taken in ascending `devid` order and released on every path; no `domain->lock` is taken, matching the existing `domain->lock -> ep->lock` attach order (no ABBA). `hpm_lock` is held by the caller (public wrapper), giving `hpm_lock -> ep->lock -> cq_lock`.
5. **Snapshot semantics.** `acquire` stores `ep->domain ? &ep->domain->domain : NULL` without dereferencing when NULL, so acquire acceptance is unchanged. `flush` returns `-EINVAL` for a NULL stored `dev`/`domain`, `-ESTALE` for changed DID/domain/FSC/PPN, `-EXDEV` for another IOMMU, `-EOPNOTSUPP` for unsupported shape/mode, `-EIO` for any CQ/post/fence failure.
6. **Harness constants.** `IOMMU_DOMAIN_DMA`/`IOMMU_DOMAIN_SVA` are mirrored from `<linux/iommu.h>` (not extracted); all `RISCV_IOMMU_*` constants come from the real `iommu-bits.h` (extracted whole, includes stripped), so mode/field values are not invented.
7. **Extraction fragility.** `extract_function` relies on kernel brace/indentation style (definition at column 0, closing brace at column 0, no braces in comments/strings inside these functions). If Codex refactors these functions, the extractor must be revisited.
8. **Not claimed:** nothing here was compiled or run; the tests must be executed by Codex. The FTQ/NV/interrupt classification is intentionally not modeled ("no assumed timeout classification").

## Suggested acceptance checks

- `gcc -std=gnu11 -O2 -Wall -Wextra -Werror` builds `newtests/test_iommu_s1_flush.c` and the binary prints `PASS`.
- `python3 newtests/test_iommu_s1_flush.py` passes (build + run) and prints the iommu.c hash.
- `python3 tests/test_iommu_hpm_session.py` still passes with the added snapshot/forwarding assertions and unchanged existing expectations.
- Kernel build: `riscv_iommu_hpm_flush_s1` is exported GPL; `cq_error_seq` initialized before `riscv_iommu_queue_init`; `git grep riscv_iommu_post` shows only the original static definition and no behavior change in `riscv_iommu_post_sync`/flush paths.
- Manual review that `riscv_iommu_cmdq_process` increments `cq_error_seq` before `riscv_iommu_queue_ctrl` clears the error.