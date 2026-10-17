// SPDX-License-Identifier: GPL-2.0
/* One bounded ABI5 trial. Hardware batch only; no implicit syscall retries. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/idma_workload.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

_Static_assert(sizeof(struct idma_workload_caps)==64,"ABI5 caps");
_Static_assert(sizeof(struct idma_workload_geometry)==64,"ABI5 geometry");
_Static_assert(sizeof(struct idma_workload_experiment)==544,"ABI5 run");
_Static_assert(offsetof(struct idma_workload_experiment,status)==192,"ABI5 input end");
_Static_assert(offsetof(struct idma_workload_experiment,victim)==256,"ABI5 V stats");
_Static_assert(offsetof(struct idma_workload_experiment,victim_coverage)==480,"ABI5 V coverage");
#define EFFECTIVE_BYTES (16ULL*1024*1024)
#define GRANULES (256U*4096/64)
struct options {
    const char *device,*shape[2];
    uint32_t size[2],pages[2];
    uint64_t operations,warmup;
    int caps_only,explicit_counts;
    struct idma_workload_experiment request;
};
static void usage(void){puts(
"Usage: idma_workload [--victim-shape ring|nic|storage|tile] [--victim-size N]\n"
"  --victim-pages N        Optional power-of-two allocation override, <=256\n"
"  --aggressor none|1      Solo (default), or independent DMA1\n"
"  --aggressor-shape S --aggressor-size N --aggressor-pages N\n"
"  --operations N --warmup-operations N  Explicit alternate count protocol\n"
"  --timeout-ms N          1..60000 (default 60000)\n"
"  --device /dev/idma0 --caps --help\n"
"Six sizes: 64/256/1024/4096/16384/65536. NIC additionally allows1536.\n"
"Default: 16MiB effective V bytes and one complete V geometry ring warmup.\n"
"1536 requires explicit --operations and is outside the equal-byte protocol.\n"
"Ring defaults to a minimal ring; NIC8 pages, storage/tile256 pages.\n"
"Long NIC sizes are linear COPY controls. Tile64/256/1024 are degenerate controls.\n"
"Requires ABI5 BTC3 and OBS5/Depth64. Hardware batch; no software-refill fallback.");}
static int decimal(const char *s,uint64_t *out){
    uint64_t n=0;const unsigned char *p=(const unsigned char*)s;
    if(!*p)return -1;
    for(;*p;p++){unsigned d=*p-'0';if(d>9||n>(UINT64_MAX-d)/10)return -1;n=n*10+d;}
    *out=n;return 0;
}
static int power_size(uint32_t n){return n==64||n==256||n==1024||n==4096||n==16384||n==65536;}
static uint64_t ring_rows(const struct idma_workload_geometry *g){
    if(g->geometry==2)return (uint64_t)g->matrix_rows*g->columns;
    if(g->geometry==1)return (uint64_t)g->ring_pages*(4096/g->row_bytes);
    return (uint64_t)g->ring_pages*4096/g->row_bytes;
}
static int geometry(const char *shape,uint32_t n,uint32_t pages,struct idma_workload_geometry *g){
    *g=(struct idma_workload_geometry){.row_bytes=n,.rows_per_operation=1};
    if(!strcmp(shape,"tile")){
        if(!power_size(n)||(pages&&pages!=256))return -1;
        g->geometry=2;g->row_bytes=n>1024?1024:n;g->rows_per_operation=n/g->row_bytes;
        g->ring_pages=256;g->matrix_rows=64;g->row_pitch_bytes=16384;g->columns=16384/g->row_bytes;return 0;
    }
    if(!strcmp(shape,"nic")){
        if(!power_size(n)&&n!=1536)return -1;
        g->geometry=n<=4096?1:0;g->ring_pages=pages?pages:(n>32768?n/4096:8);
    }else if(!strcmp(shape,"storage")){
        if(!power_size(n))return -1;
        g->ring_pages=pages?pages:256;
    }else if(!strcmp(shape,"ring")){
        if(!power_size(n))return -1;
        g->ring_pages=pages?pages:(n>4096?n/4096:1);
    }else return -1;
    if(!g->ring_pages||g->ring_pages>256||(g->ring_pages&(g->ring_pages-1)))return -1;
    if(g->geometry==0&&(n>(uint64_t)g->ring_pages*4096||(uint64_t)g->ring_pages*4096%n))return -1;
    return 0;
}
static int parse(int argc,char **argv,struct options *o,char *error,size_t cap){
    const char *const names[]={"--device","--victim-shape","--victim-size","--victim-pages",
        "--aggressor","--aggressor-shape","--aggressor-size","--aggressor-pages",
        "--operations","--warmup-operations","--timeout-ms","--caps","--help"};
    unsigned seen=0;
    for(int i=1;i<argc;i++){
        unsigned k;uint64_t n;const char *v;
        for(k=0;k<sizeof(names)/sizeof(names[0]);k++)if(!strcmp(argv[i],names[k]))break;
        if(k==sizeof(names)/sizeof(names[0])||(seen&(1U<<k))){snprintf(error,cap,"unknown/duplicate argument: %s",argv[i]);return -1;}
        seen|=1U<<k;
        if(k==11){o->caps_only=1;continue;}if(k==12){usage();return 1;}
        if(++i==argc){snprintf(error,cap,"missing value: %s",names[k]);return -1;}v=argv[i];
        if(k==0){o->device=v;continue;}if(k==1||k==5){o->shape[k==5]=v;continue;}
        if(k==4&&!strcmp(v,"none")){o->request.aggressor_id=IDMA_EXPERIMENT_NO_AGGRESSOR;continue;}
        if(decimal(v,&n)||(k!=8&&k!=9&&n>UINT32_MAX)){snprintf(error,cap,"invalid decimal: %s",names[k]);return -1;}
        if((k==3||k==7)&&!n)goto invalid;
        switch(k){case 2:o->size[0]=n;break;case 3:o->pages[0]=n;break;case 4:if(n!=1)goto invalid;o->request.aggressor_id=1;break;
        case 6:o->size[1]=n;break;case 7:o->pages[1]=n;break;case 8:o->operations=n;break;
        case 9:o->warmup=n;break;case 10:o->request.run_timeout_ms=n;break;}
    }
    if(strcmp(o->device,"/dev/idma0")||!o->request.run_timeout_ms||o->request.run_timeout_ms>60000)goto invalid;
    if(geometry(o->shape[0],o->size[0],o->pages[0],&o->request.victim_geometry))goto invalid;
    if(o->request.aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR){if(seen&((1U<<5)|(1U<<6)|(1U<<7)))goto invalid;}
    else if(geometry(o->shape[1],o->size[1],o->pages[1],&o->request.aggressor_geometry))goto invalid;
    if(!(seen&(1U<<8))){if(EFFECTIVE_BYTES%o->size[0])goto invalid;o->operations=EFFECTIVE_BYTES/o->size[0];}
    if(!(seen&(1U<<9)))o->warmup=ring_rows(&o->request.victim_geometry)/o->request.victim_geometry.rows_per_operation;
    if(!o->operations||o->operations>IDMA_WORKLOAD_MAX_DESCRIPTORS/o->request.victim_geometry.rows_per_operation||
        o->warmup>IDMA_WORKLOAD_MAX_WARMUP_DESCRIPTORS/o->request.victim_geometry.rows_per_operation)goto invalid;
    o->request.victim_operations=o->operations;o->request.victim_warmup_operations=o->warmup;
    o->explicit_counts=!!(seen&((1U<<8)|(1U<<9)));return 0;
invalid:snprintf(error,cap,"arguments exceed bounded geometry/count protocol");return -1;
}
static void string(const char *s){putchar('"');for(const unsigned char *p=(const unsigned char*)s;*p;p++){
    if(*p=='"'||*p=='\\')printf("\\%c",*p);else if(*p<32)printf("\\u%04x",*p);else putchar(*p);}putchar('"');}
static void number(const char *k,uint64_t n){printf("\"%s\":%"PRIu64,k,n);}
static void print_geometry(const struct idma_workload_geometry *g){printf(
    "{\"geometry\":%u,\"row_bytes\":%u,\"rows_per_operation\":%u,\"row_pitch_bytes\":%u,\"ring_pages\":%u,\"matrix_rows\":%u,\"columns\":%u,\"reserved0\":%u,\"reserved\":[%llu,%llu,%llu,%llu]}",
    g->geometry,g->row_bytes,g->rows_per_operation,g->row_pitch_bytes,g->ring_pages,g->matrix_rows,g->columns,g->reserved0,
    (unsigned long long)g->reserved[0],(unsigned long long)g->reserved[1],(unsigned long long)g->reserved[2],(unsigned long long)g->reserved[3]);}
static void stats(const struct idma_experiment_stats *s,int started,int hpm){
    putchar('{');
#define S(name) do{number(#name,s->name);putchar(',');}while(0)
    if(started){S(submitted);S(completed);S(completed_bytes);S(active_cycles);}
    else printf("\"submitted\":null,\"completed\":null,\"completed_bytes\":null,\"active_cycles\":null,");
    if(started&&hpm){S(iotlb_miss);S(s1_ptw);S(s2_ptw);}
    else printf("\"iotlb_miss\":null,\"s1_ptw\":null,\"s2_ptw\":null,");
    printf("\"refill_gap_ns_sum\":null,\"refill_gap_ns_max\":null,\"refill_gap_samples\":null,");
    S(in_flight_at_start);S(in_flight_at_end);S(data_errors);number("quarantined",s->quarantined);putchar('}');
#undef S
}
static void coverage(const struct idma_workload_coverage *c){putchar('{');
    number("measured_distinct_bytes",c->measured_distinct_bytes);putchar(',');number("measured_distinct_pages",c->measured_distinct_pages);
    putchar(',');number("warmup_distinct_pages",c->warmup_distinct_pages);putchar(',');number("verified_union_bytes",c->verified_union_bytes);
    putchar(',');number("reserved",c->reserved);putchar('}');}
/* Bounded independent user-side prefix enumeration for result validation. */
static uint64_t offset(const struct idma_workload_geometry *g,uint64_t i){
    i%=ring_rows(g);
    if(g->geometry==2){uint64_t op=i/g->rows_per_operation;return ((op/g->columns)*g->rows_per_operation+i%g->rows_per_operation)*g->row_pitch_bytes+(op%g->columns)*g->row_bytes;}
    if(g->geometry==1){unsigned slots=4096/g->row_bytes;return i/slots*4096+i%slots*g->row_bytes;}
    return i*g->row_bytes;
}
static int valid_coverage(const struct idma_workload_geometry *g,uint64_t n,uint64_t w,const struct idma_workload_coverage *c){
    unsigned char bits[GRANULES]={0};uint64_t ring=ring_rows(g),bytes=0,united=0;unsigned pages=0,wpages=0;
    for(unsigned lane=0;lane<2;lane++){uint64_t count=lane?w:n;if(count>ring)count=ring;
        for(uint64_t i=0;i<count;i++){uint64_t start=offset(g,i)/64;for(unsigned k=0;k<g->row_bytes/64;k++)bits[start+k]|=1U<<lane;}}
    for(unsigned p=0;p<g->ring_pages;p++){unsigned m=0,z=0;for(unsigned k=0;k<64;k++){
        unsigned b=bits[p*64+k];m|=b&1;z|=b&2;bytes+=!!(b&1)*64;united+=!!b*64;}pages+=!!m;wpages+=!!z;}
    return c->reserved||c->measured_distinct_bytes!=bytes||c->measured_distinct_pages!=pages||c->warmup_distinct_pages!=wpages||c->verified_union_bytes!=united;
}
static int valid_caps(const struct idma_workload_caps *c){return
    c->abi_version!=5||c->struct_size!=sizeof(*c)||c->capabilities!=(IDMA_EXPERIMENT_CAP_HW_BATCH|IDMA_EXPERIMENT_CAP_DID_HPM)||
    c->device_id||c->did!=10||c->max_pages!=256||c->max_rows_per_op!=64||c->max_operation_bytes!=65536||
    c->batch_magic!=IDMA_WORKLOAD_BATCH_MAGIC||c->observer_magic!=IDMA_WORKLOAD_OBSERVER_MAGIC||
    c->geometry_mask!=7||c->row_length_mask!=127||c->path_queue_depth!=64||c->max_run_timeout_ms!=60000;}
static int valid_result(const struct idma_workload_experiment *r,const struct options *o){
    uint64_t count=o->operations*o->request.victim_geometry.rows_per_operation,warm=o->warmup*o->request.victim_geometry.rows_per_operation;
    if(memcmp(r,&o->request,offsetof(struct idma_workload_experiment,status))||r->status||r->victim_did!=10||
        r->hpm_configured!=1||r->measurement_started!=1||r->measurement_complete!=1||!r->observer_sequence||!r->victim_elapsed_ns||
        r->victim.submitted!=count||r->victim.completed!=count||r->victim.completed_bytes!=count*o->request.victim_geometry.row_bytes||
        r->victim_warmup_descriptors!=warm||r->victim_warmup_bytes!=warm*o->request.victim_geometry.row_bytes||
        r->victim.in_flight_at_start||r->victim.in_flight_at_end||
        r->victim.data_errors||r->victim.quarantined||r->aggressor.data_errors||r->aggressor.quarantined||
        valid_coverage(&r->victim_geometry,count,warm,&r->victim_coverage))return -1;
    if(o->request.aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR){struct idma_experiment_stats s={};struct idma_workload_coverage c={};
        return r->aggressor_did!=IDMA_EXPERIMENT_NO_AGGRESSOR||r->aggressor_warmup_descriptors||r->aggressor_warmup_bytes||
            memcmp(&r->aggressor,&s,sizeof(s))||memcmp(&r->aggressor_coverage,&c,sizeof(c));}
    if(r->aggressor_did!=11||!r->aggressor.completed||r->aggressor.completed!=r->aggressor.submitted||
        r->aggressor.in_flight_at_start||r->aggressor.in_flight_at_end!=1||
        r->aggressor.completed>IDMA_EXPERIMENT_MAX_TRANSFERS||
        r->aggressor.completed_bytes!=r->aggressor.completed*r->aggressor_geometry.row_bytes||
        r->aggressor_warmup_descriptors>UINT64_MAX/r->aggressor_geometry.row_bytes||
        r->aggressor_warmup_bytes!=r->aggressor_warmup_descriptors*r->aggressor_geometry.row_bytes||
        valid_coverage(&r->aggressor_geometry,r->aggressor.completed,r->aggressor_warmup_descriptors,&r->aggressor_coverage))return -1;
    return 0;
}
static void print_result(const struct idma_workload_experiment *r){
    printf("{\"abi_version\":%u,\"struct_size\":%u,\"status\":%d,\"victim_did\":%u,\"aggressor_did\":",r->abi_version,r->struct_size,r->status,r->victim_did);
    if(r->aggressor_did==IDMA_EXPERIMENT_NO_AGGRESSOR)printf("null");else printf("%u",r->aggressor_did);
#define R(name) do{putchar(',');number(#name,r->name);}while(0)
    R(hpm_configured);R(measurement_started);R(measurement_complete);R(observer_sequence);R(victim_elapsed_ns);R(snapshot_before_ns);R(snapshot_after_ns);R(drain_ns);
    R(victim_warmup_descriptors);R(aggressor_warmup_descriptors);R(victim_warmup_bytes);R(aggressor_warmup_bytes);
#undef R
    printf(",\"victim\":");stats(&r->victim,r->measurement_started,r->hpm_configured);printf(",\"aggressor\":");
    if(r->aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR)printf("null");else stats(&r->aggressor,r->measurement_started,r->hpm_configured);
    printf(",\"victim_coverage\":");coverage(&r->victim_coverage);printf(",\"aggressor_coverage\":");coverage(&r->aggressor_coverage);putchar('}');
}
int main(int argc,char **argv){
    struct options o={.device="/dev/idma0",.shape={"ring","ring"},.size={4096,4096},.request={.abi_version=5,
        .struct_size=sizeof(struct idma_workload_experiment),.aggressor_id=IDMA_EXPERIMENT_NO_AGGRESSOR,.run_timeout_ms=60000,.supply=1}};
    struct idma_workload_caps c={};struct idma_workload_experiment r={};
    const char *stage="arguments";char error[256]="";int fd=-1,rc,saved_errno=0,success=0,have_caps=0,have_result=0,attempted=0;
    rc=parse(argc,argv,&o,error,sizeof(error));if(rc>0)return 0;if(rc<0)goto done;
    stage="open";fd=open(o.device,O_RDWR|O_CLOEXEC);if(fd<0)goto syscall_error;
    stage="capabilities";rc=ioctl(fd,IDMA_IOC_CAPS_WORKLOAD,&c);if(rc<0)goto syscall_error;have_caps=1;
    if(rc||valid_caps(&c)){snprintf(error,sizeof(error),"ABI5/BTC3/OBS5/Depth64 capabilities mismatch");goto done;}
    if(o.caps_only){success=1;goto done;}
    stage="run";r=o.request;attempted=1;rc=ioctl(fd,IDMA_IOC_RUN_WORKLOAD,&r);saved_errno=rc<0?errno:0;have_result=rc>=0||r.status<0;
    if(rc<0){snprintf(error,sizeof(error),"run ioctl: %s",strerror(saved_errno));goto done;}
    if(rc||valid_result(&r,&o)){snprintf(error,sizeof(error),"ABI5 result/input/data/drain/coverage mismatch");goto done;}
    success=1;goto done;
syscall_error:saved_errno=errno;snprintf(error,sizeof(error),"%s: %s",stage,strerror(saved_errno));
done:
    if(fd>=0&&close(fd)<0&&success){saved_errno=errno;success=0;snprintf(error,sizeof(error),"close: %s",strerror(saved_errno));}
    printf("{\"schema\":\"idma.batch.workload.v5\",\"success\":%s,\"stage\":",success?"true":"false");string(stage);
    printf(",\"errno\":%d,\"error\":",saved_errno);if(*error)string(error);else printf("null");
    printf(",\"run_attempted\":%s,\"argv\":[",attempted?"true":"false");for(int i=0;i<argc;i++){if(i)putchar(',');string(argv[i]);}
    printf("],\"input\":{\"device\":");string(o.device);printf(",\"protocol\":");string(o.explicit_counts?"explicit_counts_v1":"equal16MiB_fullVring_v1");
    printf(",\"caps_only\":%s,\"abi_version\":5,\"struct_size\":544,\"supply\":1,\"flags\":0,\"aggressor_id\":",o.caps_only?"true":"false");
    if(o.request.aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR)printf("null");else printf("%u",o.request.aggressor_id);
    printf(",\"run_timeout_ms\":%u,\"victim_shape\":",o.request.run_timeout_ms);string(o.shape[0]);printf(",\"aggressor_shape\":");
    if(o.request.aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR)printf("null");else string(o.shape[1]);
    printf(",\"victim_size\":%u,\"aggressor_size\":",o.size[0]);
    if(o.request.aggressor_id==IDMA_EXPERIMENT_NO_AGGRESSOR)printf("null,");else printf("%u,",o.size[1]);
    number("victim_operations",o.request.victim_operations);putchar(',');number("victim_warmup_operations",o.request.victim_warmup_operations);
    printf(",\"victim_geometry\":");print_geometry(&o.request.victim_geometry);printf(",\"aggressor_geometry\":");print_geometry(&o.request.aggressor_geometry);
    printf(",\"reserved0\":0,\"reserved1\":0,\"reserved\":[0,0]},\"capabilities\":");
    if(!have_caps)printf("null");else printf("{\"abi_version\":%u,\"struct_size\":%u,\"flags\":%u,\"device_id\":%u,\"did\":%u,\"max_pages\":%u,\"max_rows_per_op\":%u,\"max_operation_bytes\":%u,\"batch_magic\":%llu,\"observer_magic\":%llu,\"geometry_mask\":%u,\"row_length_mask\":%u,\"path_queue_depth\":%u,\"max_run_timeout_ms\":%u}",
        c.abi_version,c.struct_size,c.capabilities,c.device_id,c.did,c.max_pages,c.max_rows_per_op,c.max_operation_bytes,
        (unsigned long long)c.batch_magic,(unsigned long long)c.observer_magic,c.geometry_mask,c.row_length_mask,c.path_queue_depth,c.max_run_timeout_ms);
    printf(",\"result\":");if(have_result)print_result(&r);else printf("null");
    puts(",\"semantics\":{\"supply\":\"hardware batch, one row COPY at a time\",\"hardware_time\":\"matched observer; not victim_elapsed_ns\",\"hpm\":\"non-atomic software brackets\",\"aggressor_completed\":\"actual rows including startup/final drain; partial tiles preserved\",\"refill_gap_ns\":\"not sampled\"}}");
    return success?0:1;
}
