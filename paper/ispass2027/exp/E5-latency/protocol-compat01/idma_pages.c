// SPDX-License-Identifier: GPL-2.0
/* One fixed-work ABI4 E4 trial. No fallback, retries, or direct MMIO. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/idma_pages.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

_Static_assert(sizeof(struct idma_pages_caps) == 64, "ABI4 caps");
_Static_assert(sizeof(struct idma_pages_buffer) == 128, "ABI4 buffer proof");
_Static_assert(sizeof(struct idma_pages_experiment) == 984, "ABI4 request");
_Static_assert(offsetof(struct idma_pages_experiment, status) == 80, "ABI4 status");
_Static_assert(offsetof(struct idma_pages_experiment, experiment) == 144, "ABI4 experiment");
_Static_assert(offsetof(struct idma_pages_experiment, buffers) == 472, "ABI4 proofs");

static void string(const char *s)
{
	const unsigned char *p = (const unsigned char *)s;
	putchar('"');
	for (; *p; p++) {
		if (*p == '"' || *p == '\\') printf("\\%c", *p);
		else if (*p < 32) printf("\\u%04x", *p);
		else putchar(*p);
	}
	putchar('"');
}

static int decimal(const char *text, unsigned int maximum, unsigned int *result)
{
	uint64_t n = 0;
	if (!*text) return -1;
	for (; *text; text++) {
		if (*text < '0' || *text > '9') return -1;
		n = n * 10 + (unsigned int)(*text - '0');
		if (n > maximum) return -1;
	}
	*result = (unsigned int)n;
	return 0;
}

static void stat(const struct idma_experiment_stats *s)
{
	printf("{\"submitted\":%" PRIu64 ",\"completed\":%" PRIu64 ",\"completed_bytes\":%" PRIu64
	       ",\"active_cycles\":%" PRIu64 ",\"iotlb_miss\":%" PRIu64 ",\"s1_ptw\":%" PRIu64
	       ",\"s2_ptw\":%" PRIu64 ",\"refill_gap_ns_sum\":%" PRIu64
	       ",\"refill_gap_ns_max\":%" PRIu64 ",\"refill_gap_samples\":%" PRIu64
	       ",\"in_flight_at_start\":%u,\"in_flight_at_end\":%u,\"data_errors\":%u,\"quarantined\":%u}",
	       (uint64_t)s->submitted, (uint64_t)s->completed, (uint64_t)s->completed_bytes,
	       (uint64_t)s->active_cycles, (uint64_t)s->iotlb_miss, (uint64_t)s->s1_ptw,
	       (uint64_t)s->s2_ptw, (uint64_t)s->refill_gap_ns_sum, (uint64_t)s->refill_gap_ns_max,
	       (uint64_t)s->refill_gap_samples, s->in_flight_at_start, s->in_flight_at_end,
	       s->data_errors, s->quarantined);
}

static void u32s(const __u32 *values, unsigned int nr)
{
	putchar('[');
	for (unsigned int i = 0; i < nr; i++) {
		if (i) putchar(',');
		printf("%u", values[i]);
	}
	putchar(']');
}

static int validate(const struct idma_pages_experiment *p,
                    const struct idma_pages_experiment *request)
{
    const struct idma_experiment *e = &p->experiment;
    unsigned int shift = request->mapping_page_bytes == 2097152 ? 21 : 12;
    if (memcmp(p, request, offsetof(struct idma_pages_experiment, status)) || p->status ||
        p->prepared_lane_mask != 3 || p->verified_before_mask != 15 || p->verified_after_mask != 15 ||
        p->quiet_lane_mask != 3 || p->unmapped_buffer_mask != 15 || p->freed_buffer_mask != 15 ||
        p->retained_lane_mask || !p->observer_sequence || p->A_status_valid != 1 || p->reserved_output ||
        !(p->pre_stop_A_control & 2) || p->pre_stop_A_control >> 32 ||
        p->pre_stop_A_issued >= IDMA_EXPERIMENT_MAX_TRANSFERS) return -1;
    if (e->abi_version != 1 || e->struct_size != sizeof(*e) || e->flags || e->aggressor_id != 1 ||
        e->victim_pages != 512 || e->aggressor_pages != 512 || e->chunk_bytes != 4096 ||
        e->run_timeout_ms != request->run_timeout_ms || e->victim_transfers != 4096 ||
        e->warmup_transfers != 256 || e->status || e->victim_did != 10 || e->aggressor_did != 11 ||
        e->hpm_configured != 1 || e->measurement_started != 1 || e->measurement_complete != 1)
        return -1;
    for (unsigned int i = 0; i < 8; i++) if (e->reserved[i]) return -1;
    for (unsigned int i = 0; i < 2; i++) {
        const struct idma_experiment_stats *st = i ? &e->aggressor : &e->victim;
        if (st->data_errors || st->quarantined || st->submitted != st->completed || !st->completed ||
            st->completed > IDMA_EXPERIMENT_MAX_TRANSFERS || st->completed_bytes != st->completed * 4096 ||
            st->in_flight_at_start || st->in_flight_at_end != i) return -1;
    }
    if (e->victim.completed != 4096) return -1;
    for (unsigned int i = 0; i < 4; i++) {
        const struct idma_pages_buffer *b = &p->buffers[i];
        uint64_t last_leaf = b->iova + 2097152 - request->mapping_page_bytes;
        if ((b->iova & (2097152 - 1)) || (b->phys & (2097152 - 1)) ||
            b->bytes != 2097152 || b->mapped_bytes != 2097152 || b->unmapped_bytes != 2097152 ||
            b->requested_page_bytes != request->mapping_page_bytes || b->leaf_shift_min != shift ||
            b->leaf_shift_max != shift || b->verified_before_4k_pages != 512 || b->verified_after_4k_pages != 512 ||
            b->empty_after_cleanup != 1 || b->before_error || b->after_error || b->unmap_error || b->reserved ||
            b->first_leaf_iova_base != b->iova || b->last_leaf_iova_base != last_leaf ||
            (b->before_first_pte & 7) != 7 || (b->before_last_pte & 7) != 7 ||
            (b->after_first_pte & 7) != 7 || (b->after_last_pte & 7) != 7 ||
            ((b->before_first_pte >> 10) & ((1ULL << 44) - 1)) != (b->phys >> 12) ||
            ((b->after_first_pte >> 10) & ((1ULL << 44) - 1)) != (b->phys >> 12) ||
            ((b->before_last_pte >> 10) & ((1ULL << 44) - 1)) !=
                ((b->phys + 2097152 - request->mapping_page_bytes) >> 12) ||
            ((b->after_last_pte >> 10) & ((1ULL << 44) - 1)) !=
                ((b->phys + 2097152 - request->mapping_page_bytes) >> 12))
            return -1;
    }
    return 0;
}

static void input(const struct idma_pages_experiment *p)
{
    printf("{\"abi_version\":%u,\"struct_size\":%u,\"flags\":%u,\"mapping_page_bytes\":%u,"
           "\"buffer_bytes\":%u,\"chunk_bytes\":%u,\"run_timeout_ms\":%u,\"reserved0\":%u,"
           "\"victim_transfers\":%" PRIu64 ",\"warmup_transfers\":%" PRIu64 ",\"reserved\":",
           p->abi_version,p->struct_size,p->flags,p->mapping_page_bytes,p->buffer_bytes,p->chunk_bytes,
           p->run_timeout_ms,p->reserved0,(uint64_t)p->victim_transfers,(uint64_t)p->warmup_transfers);
    u32s(p->reserved,8); putchar('}');
}

/* Emit every ABI field, including failures and partial cleanup/proof progress. */
#define U32(o,f) printf("\"" #f "\":%u,",(o)->f)
#define I32(o,f) printf("\"" #f "\":%d,",(o)->f)
#define U64(o,f) printf("\"" #f "\":%" PRIu64 ",",(uint64_t)(o)->f)
static void experiment(const struct idma_experiment *e)
{
    putchar('{'); U32(e,abi_version); U32(e,struct_size); U32(e,flags); U32(e,aggressor_id);
    U32(e,victim_pages); U32(e,aggressor_pages); U32(e,chunk_bytes); U32(e,run_timeout_ms);
    U64(e,victim_transfers); U64(e,warmup_transfers); printf("\"reserved\":"); u32s(e->reserved,8); putchar(',');
    I32(e,status); U32(e,victim_did); U32(e,aggressor_did); U32(e,hpm_configured);
    U32(e,measurement_started); U32(e,measurement_complete); U64(e,victim_elapsed_ns);
    U64(e,snapshot_before_ns); U64(e,snapshot_after_ns); U64(e,drain_ns);
    printf("\"victim\":"); stat(&e->victim); printf(",\"aggressor\":"); stat(&e->aggressor); putchar('}');
}
static void proof(const struct idma_pages_buffer *b)
{
    putchar('{'); U64(b,iova); U64(b,phys); U64(b,bytes); U64(b,mapped_bytes); U64(b,unmapped_bytes);
    U64(b,before_first_pte); U64(b,before_last_pte); U64(b,after_first_pte); U64(b,after_last_pte);
    U64(b,first_leaf_iova_base); U64(b,last_leaf_iova_base); U32(b,requested_page_bytes);
    U32(b,leaf_shift_min); U32(b,leaf_shift_max); U32(b,verified_before_4k_pages);
    U32(b,verified_after_4k_pages); U32(b,empty_after_cleanup);
    I32(b,before_error); I32(b,after_error); I32(b,unmap_error); printf("\"reserved\":%u}",b->reserved);
}
int main(int argc, char **argv)
{
    unsigned int pg = 4096, timeout = 60000, seen = 0;
    int caps_only = 0, fd = -1, error = 0, success = 0;
    const char *message = "";
    struct idma_pages_caps caps = {};
    struct idma_pages_experiment p = {}, request = {};
    for (int i = 1; i < argc; i++) {
        unsigned int bit, value;
        if (!strcmp(argv[i], "--help")) {
            puts("idma_pages --mapping-page-bytes 4096|2097152 [--timeout-ms N] [--caps]\n"
                 "Fixed V0/A1, 2MiB per buffer (8MiB total), COPY4096, transfers4096, warmup256; ABI4 only.");
            return 0;
        }
        if (!strcmp(argv[i], "--caps")) {
            if (seen & 4) { error = EINVAL; break; }
            seen |= 4; caps_only = 1; continue;
        }
        if (!strcmp(argv[i], "--mapping-page-bytes")) bit = 1;
        else if (!strcmp(argv[i], "--timeout-ms")) bit = 2;
        else { error = EINVAL; break; }
        if ((seen & bit) || ++i == argc || decimal(argv[i], 2097152, &value)) { error = EINVAL; break; }
        seen |= bit;
        if (bit == 1) pg = value; else timeout = value;
    }
    if (error || (pg != 4096 && pg != 2097152) || !timeout || timeout > 300000) {
        error = EINVAL; message = "invalid or duplicate E4 option"; goto output;
    }
    p.abi_version=4; p.struct_size=sizeof(p); p.mapping_page_bytes=pg;
    p.buffer_bytes=2097152; p.chunk_bytes=4096; p.run_timeout_ms=timeout;
    p.victim_transfers=4096; p.warmup_transfers=256; request=p;
    fd = open("/dev/idma0", O_RDWR | O_CLOEXEC);
    if (fd < 0) { error=errno; message="open"; goto output; }
    if (ioctl(fd, IDMA_IOC_CAPS_PAGES, &caps) < 0) { error=errno; message="CAPS_PAGES"; goto output; }
    if (caps.abi_version != 4 || caps.struct_size != sizeof(caps) || caps.device_id || caps.did != 10 ||
        caps.kernel_page_bytes != 4096 || caps.buffer_bytes != 2097152 || caps.chunk_bytes != 4096 ||
        caps.max_run_timeout_ms < timeout) { error=EPROTO; message="capability layout"; goto output; }
    for (unsigned int i=0;i<6;i++)
        if (caps.reserved[i]) { error=EPROTO; message="capability reserved"; goto output; }
    if (caps_only) { success=1; goto output; }
    if ((caps.capabilities & 15) != 15 || !(caps.mapping_page_mask & (pg==4096?1:2))) {
        error=EOPNOTSUPP; message="required capabilities"; goto output;
    }
    if (ioctl(fd, IDMA_IOC_RUN_BATCH_PAGES, &p) < 0) { error=errno; message="RUN_BATCH_PAGES"; goto output; }
    if (validate(&p,&request)) { error=EPROTO; message="result contract"; goto output; }
    success=1;
output:
    if (fd>=0) close(fd);
    printf("{\"schema\":\"idma.pages.run.v1\",\"success\":%s,\"errno\":%d,\"message\":",success?"true":"false",error);
    string(message); printf(",\"device\":\"/dev/idma0\",\"caps_only\":%s,\"input\":",caps_only?"true":"false");
    input(&request); printf(",\"returned_input\":"); input(&p); printf(",\"capabilities\":{");
    U32(&caps,abi_version); U32(&caps,struct_size); U32(&caps,capabilities); U32(&caps,device_id); U32(&caps,did);
    U32(&caps,kernel_page_bytes); U32(&caps,buffer_bytes); U32(&caps,chunk_bytes); U32(&caps,mapping_page_mask);
    U32(&caps,max_run_timeout_ms); printf("\"reserved\":"); u32s(caps.reserved,6); printf("},\"result\":{");
    I32(&p,status); U32(&p,prepared_lane_mask); U32(&p,verified_before_mask); U32(&p,verified_after_mask);
    U32(&p,quiet_lane_mask); U32(&p,unmapped_buffer_mask); U32(&p,freed_buffer_mask); U32(&p,retained_lane_mask);
    U64(&p,observer_sequence); U64(&p,pre_stop_A_control); U64(&p,pre_stop_A_issued); U32(&p,A_status_valid);
    U32(&p,reserved_output); printf("\"experiment\":"); experiment(&p.experiment); printf(",\"buffers\":[");
    for(unsigned int i=0;i<4;i++){if(i)putchar(',');proof(&p.buffers[i]);}
    printf("]}}\n"); return success?0:1;
}
