/* SPDX-License-Identifier: MIT
 * R107 finite ordinary pilot: userspace receipt writer, not a CPU replay.
 * The one FINITE_RUN transaction and every raw output survive ioctl failure.
 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "../source/linux/include/uapi/linux/idma.h"

#define SCHEMA "r107-ordinary-finite-pilot-receipt-v1"
#define U(x) ((uint64_t)(x))
_Static_assert(sizeof(struct idma_finite_caps) == 104, "frozen caps ABI size");
_Static_assert(sizeof(struct idma_finite_run) == 4520, "frozen run ABI size");
_Static_assert(sizeof(struct idma_finite_batch) == 136, "frozen batch ABI size");
_Static_assert(sizeof(struct idma_finite_leaf) == 88, "frozen leaf ABI size");
#if defined(__riscv) && __riscv_xlen == 64
#define RUN_ONLY
#else
#define RUN_ONLY __attribute__((unused))
#endif
static const char *const asset_names[6] = {
 "worker0-initial_gpu.bin", "worker0-cpu_source.bin", "worker0-restored_gpu.bin",
 "worker1-initial_gpu.bin", "worker1-cpu_source.bin", "worker1-restored_gpu.bin"};
static const char *const asset_hashes[6] = {
 "4e65c7d3200cc915171578c958a1dc0215c58cc0eaaad59a2ec83dae8a024b6d",
 "4e65c7d3200cc915171578c958a1dc0215c58cc0eaaad59a2ec83dae8a024b6d",
 "ac113ba4522ae30e6b189135b5b34cfbb1fe4183708ec2bb8db7ceed7792f97b",
 "e36600660f944b7c353025f6f9481e97045910d9b02e113ee0218ebf6a86ceea",
 "e36600660f944b7c353025f6f9481e97045910d9b02e113ee0218ebf6a86ceea",
 "f58651864a5311a5c69c7e471b61adbb726e5c164cff9036f152d4ac2e12a00d"};
static const char *const pool_names[4] RUN_ONLY = {
 "pool0-cpu-home-worker0.bin", "pool1-gpu-resident-worker0.bin",
 "pool2-cpu-home-worker1.bin", "pool3-gpu-resident-worker1.bin"};

/* FIPS 180-4 SHA-256; self-contained to avoid a new RV64 crypto dependency. */
struct sha256_ctx { uint32_t h[8]; uint64_t count; unsigned char block[64]; size_t used; };
static uint32_t ror(uint32_t x, unsigned n) { return (x >> n) | (x << (32-n)); }
static void sha_block(struct sha256_ctx *c, const unsigned char *p)
{
 static const uint32_t k[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
 uint32_t w[64],a,b,d,e,f,g,h,t1,t2,v;
 for (unsigned i=0;i<16;i++) w[i]=((uint32_t)p[4*i]<<24)|((uint32_t)p[4*i+1]<<16)|((uint32_t)p[4*i+2]<<8)|p[4*i+3];
 for (unsigned i=16;i<64;i++) w[i]=(ror(w[i-2],17)^ror(w[i-2],19)^(w[i-2]>>10))+w[i-7]+(ror(w[i-15],7)^ror(w[i-15],18)^(w[i-15]>>3))+w[i-16];
 a=c->h[0];b=c->h[1];v=c->h[2];d=c->h[3];e=c->h[4];f=c->h[5];g=c->h[6];h=c->h[7];
 for(unsigned i=0;i<64;i++) {t1=h+(ror(e,6)^ror(e,11)^ror(e,25))+((e&f)^(~e&g))+k[i]+w[i];t2=(ror(a,2)^ror(a,13)^ror(a,22))+((a&b)^(a&v)^(b&v));h=g;g=f;f=e;e=d+t1;d=v;v=b;b=a;a=t1+t2;}
 c->h[0]+=a;c->h[1]+=b;c->h[2]+=v;c->h[3]+=d;c->h[4]+=e;c->h[5]+=f;c->h[6]+=g;c->h[7]+=h;
}
static void sha_init(struct sha256_ctx *c)
{
 const uint32_t init[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
 memcpy(c->h,init,sizeof(init));c->count=0;c->used=0;
}
static void sha_update(struct sha256_ctx *c,const unsigned char *p,size_t n)
{
 c->count+=n;
 while(n) {size_t take=64-c->used;if(take>n)take=n;memcpy(c->block+c->used,p,take);c->used+=take;p+=take;n-=take;if(c->used==64){sha_block(c,c->block);c->used=0;}}
}
static void sha_finish(struct sha256_ctx *c,char out[65])
{
 uint64_t bits=c->count*8;c->block[c->used++]=0x80;
 if(c->used>56){memset(c->block+c->used,0,64-c->used);sha_block(c,c->block);c->used=0;}
 memset(c->block+c->used,0,56-c->used);
 for(unsigned i=0;i<8;i++)c->block[63-i]=(unsigned char)(bits>>(8*i));
 sha_block(c,c->block);
 for(unsigned i=0;i<8;i++)snprintf(out+8*i,9,"%08"PRIx32,c->h[i]);
 out[64]=0;
}
static void RUN_ONLY hash_mem(const void *p,size_t n,char out[65]) {struct sha256_ctx c;sha_init(&c);sha_update(&c,p,n);sha_finish(&c,out);}
static uint64_t userspace_nominal_now_ns(void) {struct timespec ts;if(clock_gettime(CLOCK_MONOTONIC,&ts))return 0;return U(ts.tv_sec)*1000000000ULL+U(ts.tv_nsec);}
static int path_join(char *out,size_t n,const char *dir,const char *name) {int rc=snprintf(out,n,"%s/%s",dir,name);if(rc<0||(size_t)rc>=n){errno=ENAMETOOLONG;return -1;}return 0;}
static void js(FILE *f,const char *s) {fputc('"',f);for(;*s;s++){unsigned char c=(unsigned char)*s;if(c=='"'||c=='\\'){fputc('\\',f);fputc(c,f);}else if(c<32)fprintf(f,"\\u%04x",c);else fputc(c,f);}fputc('"',f);}
static int read_asset(const char *dir,unsigned index,unsigned char *target,char hash[65])
{
 char path[4096];unsigned char buf[65536];size_t total=0,n;struct sha256_ctx s;FILE *f;
 if(path_join(path,sizeof(path),dir,asset_names[index]))return -1;
 f=fopen(path,"rb");if(!f)return -1;sha_init(&s);
 while((n=fread(buf,1,sizeof(buf),f))!=0){if(total+n>IDMA_FINITE_POOL_BYTES){fclose(f);errno=EFBIG;return -1;}if(target)memcpy(target+total,buf,n);sha_update(&s,buf,n);total+=n;}
 if(ferror(f)){int e=errno?errno:EIO;fclose(f);errno=e;return -1;}if(fclose(f))return -1;
 sha_finish(&s,hash);if(total!=IDMA_FINITE_POOL_BYTES||strcmp(hash,asset_hashes[index])){errno=EBADMSG;return -1;}return 0;
}
static int RUN_ONLY compare_asset(const char *dir,unsigned index,const unsigned char *actual,uint64_t *compared,uint64_t *mismatch)
{
 char path[4096];unsigned char buf[65536];FILE *f;size_t n,total=0;*compared=0;*mismatch=UINT64_MAX;
 if(path_join(path,sizeof(path),dir,asset_names[index]))return -1;
 f=fopen(path,"rb");if(!f)return -1;
 while((n=fread(buf,1,sizeof(buf),f))!=0){if(total+n>IDMA_FINITE_POOL_BYTES){fclose(f);errno=EFBIG;return -1;}if(memcmp(actual+total,buf,n)&&*mismatch==UINT64_MAX){for(size_t i=0;i<n;i++)if(actual[total+i]!=buf[i]){*mismatch=total+i;break;}}total+=n;}
 if(ferror(f)){int e=errno?errno:EIO;fclose(f);errno=e;return -1;}if(fclose(f))return -1;
 *compared=total;if(total!=IDMA_FINITE_POOL_BYTES){errno=EBADMSG;return -1;}return *mismatch==UINT64_MAX?0:1;
}
static int RUN_ONLY write_binary(const char *dir,const char *name,const void *data,size_t size)
{
 char path[4096];FILE *f;int e;
 if(path_join(path,sizeof(path),dir,name))return -1;
 f=fopen(path,"wbx");if(!f)return -1;
 if(fwrite(data,1,size,f)!=size){e=errno?errno:EIO;fclose(f);errno=e;return -1;}return fclose(f);
}
static void stamp(FILE *f,const struct idma_finite_stamp *s) {fprintf(f,"{\"cpu_cycles\":%"PRIu64",\"time_ticks\":%"PRIu64",\"nominal_ns\":%"PRIu64"}",U(s->cpu_cycles),U(s->time_ticks),U(s->nominal_ns));}
static void words(FILE *f,const __u64 *v,size_t n) {fputc('[',f);for(size_t i=0;i<n;i++)fprintf(f,"%s%"PRIu64,i?",":"",U(v[i]));fputc(']',f);}
static void RUN_ONLY request_json(FILE *f,const struct idma_finite_run *r)
{
 fprintf(f,"{\"abi_version\":%u,\"struct_size\":%u,\"flags\":%u,\"timeout_ms\":%u,\"batch_capacity\":%u,\"leaf_capacity\":%u,\"reserved\":[%u,%u],\"gpu_seed_user\":",r->abi_version,r->struct_size,r->flags,r->timeout_ms,r->batch_capacity,r->leaf_capacity,r->reserved[0],r->reserved[1]);words(f,r->gpu_seed_user,2);fputs(",\"pool_user\":",f);words(f,r->pool_user,4);fprintf(f,",\"batches_user\":%"PRIu64",\"leaves_user\":%"PRIu64"}",U(r->batches_user),U(r->leaves_user));
}
static void RUN_ONLY run_json(FILE *f,const struct idma_finite_run *r,const struct idma_finite_batch *b,const struct idma_finite_leaf *l)
{
 fprintf(f,"{\"status\":%d,\"release_status\":%d,\"stage\":%u,\"quarantined\":%u,\"allocation_kind\":%u,\"extent_count\":%u,\"batch_count\":%u,\"leaf_count\":%u,\"worker_layers\":%u,\"joint_layers\":%u",r->status,r->release_status,r->stage,r->quarantined,r->allocation_kind,r->extent_count,r->batch_count,r->leaf_count,r->worker_layers,r->joint_layers);
#define FIELD(name) fprintf(f,",\""#name"\":%"PRIu64,U(r->name))
 FIELD(backing_bytes);FIELD(reference_bytes);FIELD(source_bytes_verified);FIELD(destination_bytes_verified);FIELD(guard_bytes_verified);FIELD(data_errors);FIELD(swapout_bytes);FIELD(restore_bytes);FIELD(swapout_tasks);FIELD(restore_tasks);FIELD(swapout_logical_runs);FIELD(restore_logical_runs);
#undef FIELD
 fputs(",\"stages\":[",f);for(unsigned i=0;i<IDMA_FINITE_STAGES;i++){if(i)fputc(',',f);fprintf(f,"{\"index\":%u,\"reached\":%u,\"status\":%d,\"begin\":",i,r->stages[i].reached,r->stages[i].status);stamp(f,&r->stages[i].begin);fputs(",\"end\":",f);stamp(f,&r->stages[i].end);fputc('}',f);}fputs("],\"layers\":[",f);
 for(unsigned i=0;i<IDMA_FINITE_LAYERS;i++){if(i)fputc(',',f);fprintf(f,"{\"worker\":%u,\"layer\":%u,\"ready\":",r->layers[i].worker,r->layers[i].layer);stamp(f,&r->layers[i].ready);fputc('}',f);}fputs("],\"batches\":[",f);
 unsigned bc=r->batch_count>IDMA_FINITE_MAX_BATCHES?IDMA_FINITE_MAX_BATCHES:r->batch_count;
 for(unsigned i=0;i<bc;i++){const struct idma_finite_batch *p=&b[i];if(i)fputc(',',f);fprintf(f,"{\"index\":%u,\"phase\":%u,\"worker\":%u,\"layer\":%u,\"component\":%u,\"source_offset\":%"PRIu64",\"destination_offset\":%"PRIu64",\"bytes\":%"PRIu64",\"first_id\":%"PRIu64",\"observed_done\":%"PRIu64",\"issued\":%"PRIu64",\"completed\":%"PRIu64",\"control\":%"PRIu64",\"status\":%d,\"quiet\":%u,\"begin\":",i,p->phase,p->worker,p->layer,p->component,U(p->source_offset),U(p->destination_offset),U(p->bytes),U(p->first_id),U(p->observed_done),U(p->issued),U(p->completed),U(p->control),p->status,p->quiet);stamp(f,&p->begin);fputs(",\"end\":",f);stamp(f,&p->end);fputc('}',f);}fputs("],\"leaves\":[",f);
 unsigned lc=r->leaf_count>IDMA_FINITE_MAX_LEAVES?IDMA_FINITE_MAX_LEAVES:r->leaf_count;
 for(unsigned i=0;i<lc;i++){const struct idma_finite_leaf *p=&l[i];if(i)fputc(',',f);fprintf(f,"{\"index\":%u,\"pool\":%u,\"extent\":%u,\"iova\":%"PRIu64",\"pa\":%"PRIu64",\"bytes\":%"PRIu64",\"pte\":%"PRIu64",\"table_count\":%u,\"reserved\":%u,\"table_pa\":",i,p->pool,p->extent,U(p->iova),U(p->pa),U(p->bytes),U(p->pte),p->table_count,p->reserved);words(f,p->table_pa,5);fputc('}',f);}fputs("],\"hpm_before\":[",f);
 for(unsigned i=0;i<2;i++){if(i)fputc(',',f);words(f,r->hpm_before[i],7);}fputs("],\"hpm_after\":[",f);for(unsigned i=0;i<2;i++){if(i)fputc(',',f);words(f,r->hpm_after[i],7);}fputs("],\"observer\":[",f);for(unsigned i=0;i<2;i++){if(i)fputc(',',f);words(f,r->observer[i],35);}fputs("],\"observer_status\":",f);words(f,r->observer_status,2);fputs(",\"composition\":[",f);for(unsigned i=0;i<2;i++){if(i)fputc(',',f);words(f,r->composition[i],IDMA_COMPOSITION_WORDS);}fputs("]}",f);
}
static void header(FILE *f,const char *kind) {fputs("{\"schema\":\""SCHEMA"\",\"kind\":",f);js(f,kind);fputs(",\"timing_semantics\":{\"kernel_counters\":\"raw CSR_CYCLE and CSR_TIME\",\"nominal_ns\":\"configured kernel clock; physical rate uncalibrated\",\"userspace_clock\":\"board CLOCK_MONOTONIC based on configured kernel clock; not an independent Dell reference\",\"physical_rate_uncalibrated\":true,\"independent_dell_reference\":false,\"physical_calibration_claim\":false},",f);}
static int describe(void)
{
 header(stdout,"describe");fprintf(stdout,"\"abi_version\":%u,\"caps_ioctl\":%lu,\"run_ioctl\":%lu,\"pool_bytes\":%u,\"backing_bytes\":%u,\"batch_capacity\":%u,\"leaf_capacity\":%u,\"timeout_ms\":300000,\"sizeof_caps\":%zu,\"sizeof_run\":%zu,\"sizeof_batch\":%zu,\"sizeof_leaf\":%zu,\"hardware_ioctls_enabled\":%s,\"phases\":{\"swapout\":{\"logical_runs\":48,\"hardware_tasks\":432,\"bytes\":1769472},\"restore\":{\"logical_runs\":96,\"hardware_tasks\":432,\"bytes\":1769472}},\"batch_id_semantics\":\"one NEXT ID for the whole hardware batch; no per-segment acceptance evidence\"}\n",IDMA_FINITE_ABI,(unsigned long)IDMA_IOC_FINITE_CAPS,(unsigned long)IDMA_IOC_RUN_FINITE,IDMA_FINITE_POOL_BYTES,IDMA_FINITE_BACKING_BYTES,IDMA_FINITE_MAX_BATCHES,IDMA_FINITE_MAX_LEAVES,sizeof(struct idma_finite_caps),sizeof(struct idma_finite_run),sizeof(struct idma_finite_batch),sizeof(struct idma_finite_leaf),
#if defined(__riscv) && __riscv_xlen == 64
 "true"
#else
 "false"
#endif
 );return 0;
}
static int check_data(const char *dir)
{
 int ok=1;uint64_t begin=userspace_nominal_now_ns();header(stdout,"data-check");fputs("\"assets\":[",stdout);
 for(unsigned i=0;i<6;i++){char h[65]={0};int rc=read_asset(dir,i,NULL,h),e=rc?errno:0;if(i)fputc(',',stdout);fputs("{\"file\":",stdout);js(stdout,asset_names[i]);fputs(",\"expected_sha256\":",stdout);js(stdout,asset_hashes[i]);fputs(",\"actual_sha256\":",stdout);js(stdout,h);fprintf(stdout,",\"expected_bytes\":%u,\"accepted_raw_asset\":%s,\"errno\":%d}",IDMA_FINITE_POOL_BYTES,rc?"false":"true",e);if(rc)ok=0;}
 fprintf(stdout,"],\"all_assets_accepted\":%s,\"userspace_nominal_ns\":%"PRIu64",\"ioctl_attempted\":false}\n",ok?"true":"false",userspace_nominal_now_ns()-begin);return ok?0:1;
}
static int caps_json(const char *device)
{
 struct idma_finite_caps c={0};int rc=-1,e=0,fd=-1;uint64_t begin=userspace_nominal_now_ns();c.abi_version=IDMA_FINITE_ABI;c.struct_size=sizeof(c);
#if defined(__riscv) && __riscv_xlen == 64
 fd=open(device,O_RDWR|O_CLOEXEC);if(fd<0)e=errno;else {rc=ioctl(fd,IDMA_IOC_FINITE_CAPS,&c);if(rc<0)e=errno;close(fd);}
#else
 (void)fd;e=ENOTSUP;
#endif
 header(stdout,"caps");fputs("\"device\":",stdout);js(stdout,device);fprintf(stdout,",\"ioctl\":{\"attempted\":%s,\"return\":%d,\"errno\":%d},\"caps\":{\"abi_version\":%u,\"struct_size\":%u,\"magic\":%"PRIu64",\"observer_magic\":%"PRIu64",\"composition_magic\":%"PRIu64",\"composition_abi\":%"PRIu64",\"composition_config\":%"PRIu64",\"composition_idle\":%"PRIu64",\"batch_magic\":%"PRIu64",\"device_id\":%u,\"did\":%u,\"quarantined\":%u,\"online_cpus\":%u,\"sample\":",
#if defined(__riscv) && __riscv_xlen == 64
 fd>=0?"true":"false",
#else
 "false",
#endif
 rc,e,c.abi_version,c.struct_size,U(c.magic),U(c.observer_magic),U(c.composition_magic),U(c.composition_abi),U(c.composition_config),U(c.composition_idle),U(c.batch_magic),c.device_id,c.did,c.quarantined,c.online_cpus);stamp(stdout,&c.sample);fprintf(stdout,"},\"userspace_nominal_ns\":%"PRIu64"}\n",userspace_nominal_now_ns()-begin);return rc?1:0;
}
static int run(const char *device,const char *data,const char *output)
{
 struct idma_finite_run r={0};struct idma_finite_batch *b=NULL;struct idma_finite_leaf *l=NULL;
 unsigned char *seed[2]={NULL,NULL},*pool[4]={NULL,NULL,NULL,NULL};char input_hash[6][65]={{0}},pool_hash[4][65]={{0}},receipt[4096];uint64_t compared[4]={0},mismatch[4]={0};int cmp_rc[4]={0},cmp_errno[4]={0},write_errno[4]={0};
 uint64_t start=userspace_nominal_now_ns(),load_end,io_begin,io_end,verify_begin,verify_end,write_begin,write_end;int rc=-1,io_errno=0,fd=-1,ok=0;FILE *f=NULL;
 /* Host builds deliberately refuse hardware; --describe/--check-data work. */
#if !defined(__riscv) || __riscv_xlen != 64
 (void)device;(void)data;(void)output;(void)r;(void)b;(void)l;(void)seed;(void)pool;(void)input_hash;(void)pool_hash;(void)receipt;(void)compared;(void)mismatch;(void)cmp_rc;(void)cmp_errno;(void)write_errno;(void)start;(void)load_end;(void)io_begin;(void)io_end;(void)verify_begin;(void)verify_end;(void)write_begin;(void)write_end;(void)rc;(void)io_errno;(void)fd;(void)ok;(void)f;
 fprintf(stderr,"host build refuses finite hardware ioctls; use --describe or --check-data\n");return 2;
#else
 if(mkdir(output,0700)){perror("create new output directory");goto out;}
 for(unsigned i=0;i<2;i++){seed[i]=malloc(IDMA_FINITE_POOL_BYTES);if(!seed[i]){perror("allocate seed reference");goto out;}}
 for(unsigned i=0;i<4;i++){pool[i]=calloc(1,IDMA_FINITE_POOL_BYTES);if(!pool[i]){perror("allocate output pool");goto out;}}
 b=calloc(IDMA_FINITE_MAX_BATCHES,sizeof(*b));l=calloc(IDMA_FINITE_MAX_LEAVES,sizeof(*l));if(!b||!l){perror("allocate records");goto out;}
 for(unsigned i=0;i<6;i++)if(read_asset(data,i,(i%3==0)?seed[i/3]:NULL,input_hash[i])){perror(asset_names[i]);goto out;}
 load_end=userspace_nominal_now_ns();r.abi_version=IDMA_FINITE_ABI;r.struct_size=sizeof(r);r.timeout_ms=300000;r.batch_capacity=IDMA_FINITE_MAX_BATCHES;r.leaf_capacity=IDMA_FINITE_MAX_LEAVES;r.status=-EINPROGRESS;r.release_status=-EINPROGRESS;
 for(unsigned i=0;i<2;i++)r.gpu_seed_user[i]=(uintptr_t)seed[i];
 for(unsigned i=0;i<4;i++)r.pool_user[i]=(uintptr_t)pool[i];
 r.batches_user=(uintptr_t)b;r.leaves_user=(uintptr_t)l;
 io_begin=userspace_nominal_now_ns();fd=open(device,O_RDWR|O_CLOEXEC);if(fd<0)io_errno=errno;else {rc=ioctl(fd,IDMA_IOC_RUN_FINITE,&r);if(rc<0)io_errno=errno;close(fd);}io_end=userspace_nominal_now_ns();
 /* Do not turn syscall failure into success; preserve every copied output. */
 verify_begin=userspace_nominal_now_ns();for(unsigned i=0;i<4;i++){unsigned a=(i/2)*3+(i%2?2:1);hash_mem(pool[i],IDMA_FINITE_POOL_BYTES,pool_hash[i]);cmp_rc[i]=compare_asset(data,a,pool[i],&compared[i],&mismatch[i]);cmp_errno[i]=cmp_rc[i]<0?errno:0;}verify_end=userspace_nominal_now_ns();
 write_begin=userspace_nominal_now_ns();for(unsigned i=0;i<4;i++)if(write_binary(output,pool_names[i],pool[i],IDMA_FINITE_POOL_BYTES))write_errno[i]=errno;
 int raw_errno[3]={0,0,0};
 if(write_binary(output,"uapi-run.bin",&r,sizeof(r)))raw_errno[0]=errno;
 if(write_binary(output,"uapi-batches.bin",b,IDMA_FINITE_MAX_BATCHES*sizeof(*b)))raw_errno[1]=errno;
 if(write_binary(output,"uapi-leaves.bin",l,IDMA_FINITE_MAX_LEAVES*sizeof(*l)))raw_errno[2]=errno;
 write_end=userspace_nominal_now_ns();if(path_join(receipt,sizeof(receipt),output,"receipt.json")){perror("receipt path");goto out;}f=fopen(receipt,"wx");if(!f){perror("create receipt");goto out;}
 header(f,"run");fputs("\"device\":",f);js(f,device);fprintf(f,",\"ioctl\":{\"attempted\":%s,\"run_calls\":%u,\"return\":%d,\"errno\":%d,\"errno_name\":",fd>=0?"true":"false",fd>=0?1U:0U,rc,io_errno);js(f,io_errno?strerror(io_errno):"Success");fputs("},\"request\":",f);request_json(f,&r);fputs(",\"run\":",f);run_json(f,&r,b,l);fputs(",\"input_assets\":[",f);
 for(unsigned i=0;i<6;i++){if(i)fputc(',',f);fputs("{\"file\":",f);js(f,asset_names[i]);fputs(",\"sha256\":",f);js(f,input_hash[i]);fprintf(f,",\"bytes\":%u}",IDMA_FINITE_POOL_BYTES);}fputs("],\"pools\":[",f);
 for(unsigned i=0;i<4;i++){unsigned a=(i/2)*3+(i%2?2:1);if(i)fputc(',',f);fprintf(f,"{\"pool\":%u,\"worker\":%u,\"file\":",i,i/2);js(f,pool_names[i]);fputs(",\"sha256\":",f);js(f,pool_hash[i]);fputs(",\"expected_asset\":",f);js(f,asset_names[a]);fputs(",\"expected_sha256\":",f);js(f,asset_hashes[a]);fprintf(f,",\"bytes\":%u,\"memcmp_bytes\":%"PRIu64",\"memcmp_equal\":%s,\"first_mismatch\":",IDMA_FINITE_POOL_BYTES,compared[i],cmp_rc[i]==0?"true":"false");if(mismatch[i]==UINT64_MAX)fputs("null",f);else fprintf(f,"%"PRIu64,mismatch[i]);fprintf(f,",\"memcmp_errno\":%d,\"write_errno\":%d}",cmp_errno[i],write_errno[i]);}
 fprintf(f,"],\"raw_write_errno\":[%d,%d,%d],\"userspace_cost_nominal_ns\":{\"data_load_and_allocation\":%"PRIu64",\"device_open_ioctl_close\":%"PRIu64",\"hash_and_full_memcmp\":%"PRIu64",\"raw_output_write\":%"PRIu64",\"total_before_receipt\":%"PRIu64"},\"userspace_memory\":{\"seed_reference_bytes\":%u,\"output_pool_bytes\":%u,\"batch_record_bytes\":%zu,\"leaf_record_bytes\":%zu},\"evidence_limit\":\"batch NEXT/DONE and raw aggregate counters; no invented per-beat or per-task independent NEXT acceptance\"}\n",raw_errno[0],raw_errno[1],raw_errno[2],load_end-start,io_end-io_begin,verify_end-verify_begin,write_end-write_begin,userspace_nominal_now_ns()-start,2U*IDMA_FINITE_POOL_BYTES,4U*IDMA_FINITE_POOL_BYTES,IDMA_FINITE_MAX_BATCHES*sizeof(*b),IDMA_FINITE_MAX_LEAVES*sizeof(*l));
 if(fclose(f)){f=NULL;perror("flush receipt");goto out;}
 f=NULL;ok=(rc==0&&r.status==0&&r.release_status==0&&r.quarantined==0);
 for(unsigned i=0;i<4;i++)if(cmp_rc[i]||write_errno[i])ok=0;
 for(unsigned i=0;i<3;i++)if(raw_errno[i])ok=0;
 puts(receipt);
out:
 if(f)fclose(f);
 for(unsigned i=0;i<2;i++)free(seed[i]);
 for(unsigned i=0;i<4;i++)free(pool[i]);
 free(b);free(l);return ok?0:1;
#endif
}
int main(int argc,char **argv)
{
 const char *device="/dev/idma0",*data=NULL,*output=NULL,*mode=NULL;
 for(int i=1;i<argc;i++){if(!strcmp(argv[i],"--caps")||!strcmp(argv[i],"--run")||!strcmp(argv[i],"--describe")||!strcmp(argv[i],"--check-data")){if(mode){fprintf(stderr,"choose one mode\n");return 2;}mode=argv[i];}else if(!strcmp(argv[i],"--device")&&i+1<argc)device=argv[++i];else if(!strcmp(argv[i],"--data-dir")&&i+1<argc)data=argv[++i];else if(!strcmp(argv[i],"--output-dir")&&i+1<argc)output=argv[++i];else {fprintf(stderr,"invalid argument: %s\n",argv[i]);return 2;}}
 if(mode&&!strcmp(mode,"--describe"))return describe();
 if(mode&&!strcmp(mode,"--caps"))return caps_json(device);
 if(mode&&!strcmp(mode,"--check-data")&&data)return check_data(data);
 if(mode&&!strcmp(mode,"--run")&&data&&output)return run(device,data,output);
 fprintf(stderr,"usage: %s --describe | --check-data --data-dir DIR | --caps [--device DEV] | --run [--device DEV] --data-dir DIR --output-dir NEWDIR\n",argv[0]);return 2;
}
