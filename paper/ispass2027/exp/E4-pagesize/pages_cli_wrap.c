// SPDX-License-Identifier: GPL-2.0
/* Finite host-only syscalls; cannot open hardware. */
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/idma_pages.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned opened,caps_calls,run_calls,closed;
static int is(const char *v){const char *m=getenv("E4_CLI_CASE");return m&&!strcmp(m,v);}
static void report(void){fprintf(stderr,"MOCK_SYSCALLS open=%u caps=%u run=%u close=%u\n",opened,caps_calls,run_calls,closed);}
__attribute__((constructor)) static void setup(void){atexit(report);}
int __wrap_open(const char *p,int flags,...){assert(!strcmp(p,"/dev/idma0")&&flags==(O_RDWR|O_CLOEXEC));opened++;
    if(is("open_error")){errno=ENOENT;return -1;}return 42;}
int __wrap_close(int fd){assert(fd==42);closed++;return 0;}
int __wrap_ioctl(int fd,unsigned long cmd,...){va_list args;void *arg;assert(fd==42);
    va_start(args,cmd);arg=va_arg(args,void *);va_end(args);
    if(cmd==IDMA_IOC_CAPS_PAGES){struct idma_pages_caps *c=arg;caps_calls++;
        if(is("caps_error")){errno=ENOTTY;return -1;}
        *c=(struct idma_pages_caps){.abi_version=4,.struct_size=sizeof(*c),.capabilities=15,.did=10,
            .kernel_page_bytes=4096,.buffer_bytes=2097152,.chunk_bytes=4096,.mapping_page_mask=3,.max_run_timeout_ms=300000};
        if(is("caps_bad_abi"))c->abi_version=3;
        if(is("caps_bad_reserved"))c->reserved[5]=1;
        if(is("caps_no_leaf"))c->capabilities&=~8U;
        if(is("caps_no_2m"))c->mapping_page_mask=1;
        return 0;
    }
    assert(cmd==IDMA_IOC_RUN_BATCH_PAGES);run_calls++;
    struct idma_pages_experiment *p=arg;struct idma_experiment *e=&p->experiment;
    assert(p->abi_version==4&&p->struct_size==984&&p->buffer_bytes==2097152&&p->chunk_bytes==4096);
    assert((p->mapping_page_bytes==4096||p->mapping_page_bytes==2097152)&&!p->flags);
    p->prepared_lane_mask=p->quiet_lane_mask=3;
    p->verified_before_mask=p->verified_after_mask=p->unmapped_buffer_mask=p->freed_buffer_mask=15;
    p->observer_sequence=17;p->A_status_valid=1;p->pre_stop_A_control=3;p->pre_stop_A_issued=4200;
    *e=(struct idma_experiment){.abi_version=1,.struct_size=sizeof(*e),.aggressor_id=1,.victim_pages=512,
        .aggressor_pages=512,.chunk_bytes=4096,.run_timeout_ms=p->run_timeout_ms,.victim_transfers=4096,
        .warmup_transfers=256,.victim_did=10,.aggressor_did=11,.hpm_configured=1,.measurement_started=1,
        .measurement_complete=1,.victim_elapsed_ns=700};
    for(unsigned i=0;i<2;i++){struct idma_experiment_stats *s=i?&e->aggressor:&e->victim;
        s->submitted=s->completed=i?4200:4096;s->completed_bytes=s->completed*4096;s->in_flight_at_end=i;
        s->iotlb_miss=100+i;s->s1_ptw=200+i;s->s2_ptw=300+i;}
    for(unsigned i=0;i<4;i++){struct idma_pages_buffer *b=&p->buffers[i];
        b->iova=0x1000000ULL+i*2097152ULL;b->phys=0x2000000ULL+i*2097152ULL;
        b->bytes=b->mapped_bytes=b->unmapped_bytes=2097152;b->requested_page_bytes=p->mapping_page_bytes;
        b->leaf_shift_min=b->leaf_shift_max=p->mapping_page_bytes==4096?12:21;
        b->verified_before_4k_pages=b->verified_after_4k_pages=512;b->empty_after_cleanup=1;
        b->first_leaf_iova_base=b->iova;b->last_leaf_iova_base=b->iova+2097152-p->mapping_page_bytes;
        b->before_first_pte=b->after_first_pte=((b->phys>>12)<<10)|7;
        b->before_last_pte=b->after_last_pte=(((b->phys+2097152-p->mapping_page_bytes)>>12)<<10)|7;
    }
    if(is("run_error")){p->status=-ENOMEM;p->retained_lane_mask=2;p->freed_buffer_mask=3;
        p->buffers[2].unmap_error=-EIO;errno=ENOMEM;return -1;}
    if(is("ad_change"))p->buffers[0].after_first_pte|=0xc0;
    if(is("bad_input"))p->mapping_page_bytes=8192;
    if(is("bad_bytes"))e->aggressor.completed_bytes--;
    if(is("bad_complete"))e->victim.completed--;
    if(is("bad_mask"))p->verified_after_mask=7;
    if(is("bad_retain"))p->retained_lane_mask=2;
    if(is("bad_active"))p->pre_stop_A_control=1;
    if(is("bad_target"))p->pre_stop_A_issued=IDMA_EXPERIMENT_MAX_TRANSFERS;
    if(is("bad_error"))p->pre_stop_A_control|=1ULL<<32;
    if(is("bad_end"))e->aggressor.in_flight_at_end=0;
    if(is("bad_start"))e->victim.in_flight_at_start=1;
    if(is("bad_did"))e->aggressor_did=99;
    if(is("bad_data"))e->victim.data_errors=1;
    if(is("bad_quarantine"))e->aggressor.quarantined=1;
    if(is("bad_sequence"))p->observer_sequence=0;
    if(is("bad_shift"))p->buffers[0].leaf_shift_max=30;
    if(is("bad_pte"))p->buffers[0].after_last_pte^=1ULL<<10;
    if(is("bad_rw"))p->buffers[0].after_first_pte&=~4ULL;
    if(is("bad_alignment"))p->buffers[0].phys+=4096;
    if(is("bad_hole"))p->buffers[0].empty_after_cleanup=0;
    if(is("bad_proof_count"))p->buffers[0].verified_after_4k_pages=511;
    if(is("bad_unmap"))p->buffers[0].unmapped_bytes-=4096;
    if(is("bad_base"))p->buffers[0].last_leaf_iova_base+=4096;
    if(is("bad_reserved"))p->buffers[0].reserved=1;
    return 0;
}
