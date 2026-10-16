from pathlib import Path
import json,hashlib,difflib
base=Path('/home/xvhaocheng/work/iommu-platform/out/workflow-stream-contract-20261012')
out=base/'proposed';out.mkdir(parents=True,exist_ok=True)
source=base/'source-evidence/hero/openmp-examples/mm-large/mm-large.c'
lines=source.read_text().splitlines(keepends=True)
original=''.join(lines[43:276])
start='''        #pragma omp single
        {
          trace_compute("compute_start", s, t, a_ptrs[!a_idx],
                        b_ptrs[!b_idx], c_ptrs[c_idx]);
        }
'''
done='''        #pragma omp single
        {
          trace_compute("compute_done", s, t, a_ptrs[!a_idx],
                        b_ptrs[!b_idx], c_ptrs[c_idx]);
        }
'''
assert original.count('        #pragma omp barrier\n')==1
assert original.count('        } // i < stripe_height\n')==1
instrumented=original.replace('        #pragma omp barrier\n','        #pragma omp barrier\n\n'+start).replace('        } // i < stripe_height\n','        } // i < stripe_height\n\n'+done)
pre=r'''/*
 * Private host functional adaptation of HERO mm-large, Apache License 2.0.
 * Upstream copyright 2018 ETH Zurich, University of Bologna.
 * Source: pulp-platform/hero commit c8481a930a15a385d4f58f519bed83d2e75e2a73,
 * openmp-examples/mm-large/mm-large.c lines 44-276 and 335-346.
 * Added observation/shims by Codex, 2026-10-12.
 * Licensed under Apache-2.0: http://www.apache.org/licenses/LICENSE-2.0
 * Provided AS IS, without warranties; retain upstream LICENSE with this file.
 *
 * This executes CPU malloc and synchronous CPU memcpy, NOT hardware DMA.
 * Two added omp-single blocks/barriers per compute tile are functional
 * instrumentation. Host timestamps are NOT accelerator cycles or performance.
 * The full fixed uint32 inner loop and original eight-thread structure remain.
 */
#define _POSIX_C_SOURCE 200809L
#define __host
#define __device
#include <omp.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <pthread.h>

#define W 256u
#define H 256u
#define SH 16u
#define NS 16u
#define STRIPE_BYTES 16384u
#define ELEMENTS 65536u
#define MAX_JOBS 512u
#define SOURCE_COMMIT "c8481a930a15a385d4f58f519bed83d2e75e2a73"

/* Erasing address spaces is local to this host-only translation unit. */
typedef struct { uint32_t id; } hero_dma_job_t;
typedef struct {
  void *ptr;
  size_t size;
  char matrix;
  int slot;
  int64_t generation;
  int active;
} slot_t;
typedef struct {
  uint32_t id;
  char matrix;
  const char *role;
  int stripe, tile, linear_tile, slot, controller;
  int64_t generation, byte_offset;
  uint32_t size;
  int completed, waited;
} job_t;
typedef struct {
  const char *event, *role;
  char matrix;
  int stripe, tile, linear_tile, slot;
  int64_t generation, byte_offset, job_id;
  uint32_t size;
  int a_slot, b_slot, c_slot;
  int64_t a_generation, b_generation, c_generation;
} event_t;
static slot_t slots[6];
static unsigned allocation_count, next_job, a_jobs, b_jobs, c_jobs;
static unsigned compute_started, compute_finished, reference_mismatches;
static job_t jobs[MAX_JOBS];
static uint32_t *main_a, *main_b, *main_c;
static FILE *trace_file;
static uint64_t event_counter;
static pthread_mutex_t trace_lock = PTHREAD_MUTEX_INITIALIZER;

static void fail(const char *message) {
  fprintf(stderr, "HOST_FUNCTIONAL_ERROR: %s\n", message);
  exit(2);
}
static uint64_t host_ns(void) {
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts)) fail("clock_gettime");
  return (uint64_t)ts.tv_sec * UINT64_C(1000000000) + (uint64_t)ts.tv_nsec;
}
static event_t blank_event(const char *kind, const char *role) {
  event_t e = {0};
  e.event=kind; e.role=role; e.matrix='-';
  e.stripe=e.tile=e.linear_tile=e.slot=-1;
  e.generation=e.byte_offset=e.job_id=-1;
  e.a_slot=e.b_slot=e.c_slot=-1;
  e.a_generation=e.b_generation=e.c_generation=-1;
  return e;
}
/* Caller holds the mutex: IDs and observation timestamps serialize together. */
static void emit_locked(event_t e) {
  uint64_t id=++event_counter, stamp=host_ns();
  if (fprintf(trace_file,
      "%" PRIu64 ",%" PRIu64 ",%d,%s,%s,%c,%d,%d,%d,%d,%" PRId64
      ",%" PRId64 ",%u,%" PRId64 ",%d,%" PRId64 ",%d,%" PRId64
      ",%d,%" PRId64 "\n",
      id,stamp,omp_get_thread_num(),e.event,e.role,e.matrix,
      e.stripe,e.tile,e.linear_tile,e.slot,e.generation,e.byte_offset,
      e.size,e.job_id,e.a_slot,e.a_generation,e.b_slot,e.b_generation,
      e.c_slot,e.c_generation)<0) fail("write trace");
}
static slot_t *slot_for(void *ptr) {
  for (unsigned i=0;i<allocation_count;i++)
    if (slots[i].ptr==ptr && slots[i].active) return &slots[i];
  fail("pointer is not a live application slot");
  return NULL;
}
static void *hero_l1malloc(int32_t size) {
  if (size!=(int32_t)STRIPE_BYTES) fail("unexpected slot size");
  void *ptr=malloc((size_t)size);
  if (!ptr) return NULL;
  pthread_mutex_lock(&trace_lock);
  if (allocation_count>=6) fail("more than six slots allocated");
  unsigned index=allocation_count++;
  slot_t *slot=&slots[index];
  *slot=(slot_t){ptr,(size_t)size,"ABC"[index/2],(int)(index%2),-1,1};
  event_t e=blank_event("slot_alloc","allocator");
  e.matrix=slot->matrix;e.slot=slot->slot;e.generation=-1;e.size=(uint32_t)size;
  emit_locked(e);
  pthread_mutex_unlock(&trace_lock);
  return ptr;
}
static void hero_l1free(void *ptr) {
  pthread_mutex_lock(&trace_lock);
  slot_t *slot=slot_for(ptr);
  event_t e=blank_event("slot_free","allocator");
  e.matrix=slot->matrix;e.slot=slot->slot;e.generation=slot->generation;
  e.size=(uint32_t)slot->size;
  emit_locked(e);slot->active=0;
  pthread_mutex_unlock(&trace_lock);
  free(ptr);
}
static event_t job_event(const job_t *job,const char *kind) {
  event_t e=blank_event(kind,job->role);
  e.matrix=job->matrix;e.stripe=job->stripe;e.tile=job->tile;
  e.linear_tile=job->linear_tile;e.slot=job->slot;e.generation=job->generation;
  e.byte_offset=job->byte_offset;e.size=job->size;e.job_id=job->id;
  return e;
}
static hero_dma_job_t copy_cpu(void *dst,const void *src,uint32_t size,int input) {
  pthread_mutex_lock(&trace_lock);
  if (size!=STRIPE_BYTES || next_job+1>=MAX_JOBS) fail("copy bounds");
  slot_t *slot=slot_for(input?dst:(void *)src);
  int controller=omp_get_thread_num();
  char matrix=slot->matrix;
  if ((input && matrix!='A' && matrix!='B') || (!input && matrix!='C'))
    fail("copy direction/matrix mismatch");
  if (controller!=(matrix=='A'?0:matrix=='B'?1:2)) fail("wrong manager thread");
  uintptr_t ext=(uintptr_t)(input?src:dst);
  uintptr_t base=(uintptr_t)(matrix=='A'?main_a:matrix=='B'?main_b:main_c);
  if (ext<base || ext-base>=ELEMENTS*sizeof(uint32_t) ||
      (ext-base)%STRIPE_BYTES) fail("external stripe address");
  int stripe=(int)((ext-base)/STRIPE_BYTES), tile=-1, linear=-1;
  const char *role=matrix=='A'?"controller_A":matrix=='B'?"controller_B":"controller_C";
  if (input) {
    ++slot->generation;
    if (matrix=='A') ++a_jobs;
    else {linear=(int)b_jobs++;tile=linear%NS;}
  } else ++c_jobs;
  uint32_t id=++next_job;
  jobs[id]=(job_t){id,matrix,role,stripe,tile,linear,slot->slot,controller,
                  slot->generation,(int64_t)(ext-base),size,0,0};
  emit_locked(job_event(&jobs[id],"copy_issue"));
  pthread_mutex_unlock(&trace_lock);

  /* Actual data movement on the CPU; synchronous and fully completed here. */
  memcpy(dst,src,size);

  pthread_mutex_lock(&trace_lock);
  jobs[id].completed=1;
  emit_locked(job_event(&jobs[id],"copy_done"));
  pthread_mutex_unlock(&trace_lock);
  return (hero_dma_job_t){id};
}
static hero_dma_job_t hero_memcpy_host2dev_async(void *dst,const void *src,uint32_t size) {
  return copy_cpu(dst,src,size,1);
}
static hero_dma_job_t hero_memcpy_dev2host_async(void *dst,const void *src,uint32_t size) {
  return copy_cpu(dst,src,size,0);
}
static void hero_dma_wait(hero_dma_job_t job) {
  pthread_mutex_lock(&trace_lock);
  if (!job.id || job.id>next_job) fail("wait job id");
  job_t *j=&jobs[job.id];
  if (!j->completed || j->waited || j->controller!=omp_get_thread_num())
    fail("invalid/duplicate/cross-thread wait");
  j->waited=1;
  emit_locked(job_event(j,"wait"));
  pthread_mutex_unlock(&trace_lock);
}
static void hero_memcpy_dev2host(void *dst,const void *src,uint32_t size) {
  hero_dma_wait(hero_memcpy_dev2host_async(dst,src,size));
}
static void trace_compute(const char *kind,uint32_t s,uint32_t t,
                          void *a,void *b,void *c) {
  pthread_mutex_lock(&trace_lock);
  slot_t *as=slot_for(a),*bs=slot_for(b),*cs=slot_for(c);
  if (as->matrix!='A' || bs->matrix!='B' || cs->matrix!='C') fail("compute slot roles");
  if (!strcmp(kind,"compute_start")) {
    if (!t) ++cs->generation;
    ++compute_started;
  } else ++compute_finished;
  event_t e=blank_event(kind,"compute_observer");
  e.matrix='C';e.stripe=(int)s;e.tile=(int)t;e.linear_tile=(int)(s*NS+t);
  e.slot=cs->slot;e.generation=cs->generation;
  /* First element offset: footprint is 16 rows x 16 columns, row-pitch 1024. */
  e.byte_offset=(int64_t)(s*SH*W+t*SH)*sizeof(uint32_t);
  e.size=SH*SH*sizeof(uint32_t);
  e.a_slot=as->slot;e.a_generation=as->generation;
  e.b_slot=bs->slot;e.b_generation=bs->generation;
  e.c_slot=cs->slot;e.c_generation=cs->generation;
  emit_locked(e);
  pthread_mutex_unlock(&trace_lock);
}

/* BEGIN extracted actual upstream function: only trace blocks added below. */
'''
post=r'''
/* END extracted upstream function. */

static void write_u32_le(FILE *file,uint32_t value) {
  unsigned char bytes[4]={(unsigned char)value,(unsigned char)(value>>8),
                         (unsigned char)(value>>16),(unsigned char)(value>>24)};
  if (fwrite(bytes,1,4,file)!=4) fail("write output data");
}
static FILE *open_output(const char *dir,const char *name,const char *mode) {
  char path[4096];
  if (snprintf(path,sizeof(path),"%s/%s",dir,name)>=(int)sizeof(path)) fail("output path too long");
  FILE *file=fopen(path,mode);
  if (!file) fail("open output");
  return file;
}
int main(int argc,char **argv) {
  if (argc!=2) {fprintf(stderr,"Usage: %s private-output-directory\n",argv[0]);return 2;}
  if (mkdir(argv[1],0700) && errno!=EEXIST) fail("create private output directory");
  trace_file=open_output(argv[1],"trace.csv","w");
  fputs("event_id,host_ns,thread_id,event,role,matrix,stripe,tile,linear_tile,slot,generation,byte_offset,size_bytes,job_id,a_slot,a_generation,b_slot,b_generation,c_slot,c_generation\n",trace_file);
  uint32_t height=256;
  /* Same no-argument default stripe derivation as upstream lines 295-305. */
  uint32_t stripe_height=height/2;
  while (stripe_height*height*sizeof(uint32_t)>=32*1024) stripe_height/=2;
  const uint32_t n_stripes=height/stripe_height;
  height=n_stripes*stripe_height;
  uint32_t width=height;
  if (width!=W || height!=H || stripe_height!=SH || n_stripes!=NS) fail("fixed default derivation");
  uint32_t *a=malloc(ELEMENTS*sizeof(uint32_t));
  uint32_t *b=malloc(ELEMENTS*sizeof(uint32_t));
  uint32_t *c=calloc(ELEMENTS,sizeof(uint32_t));
  uint32_t *d=calloc(ELEMENTS,sizeof(uint32_t));
  if (!a || !b || !c || !d) fail("main array allocation");
  main_a=a;main_b=b;main_c=c;
  omp_set_dynamic(0);
  /* BEGIN upstream original initialization lines 320-326. */
'''
init=''.join(lines[319:326])
reference=''.join(lines[334:346])
tail=r'''
  /* END original initialization. */
  int ret=double_buf_mm(a,b,c,width,height,stripe_height);
  if (ret) fail("double_buf_mm returned error");
  /* BEGIN upstream original host-reference computation lines 335-346. */
'''+reference+r'''
  /* END original host-reference computation. */
  uint64_t sum=0;
  for (uint32_t i=0;i<ELEMENTS;i++) {
    if (c[i]!=d[i] || c[i]!=(uint32_t)(2u*a[i])) ++reference_mismatches;
    sum+=c[i];
  }
  for (unsigned id=1;id<=next_job;id++)
    if (!jobs[id].completed || !jobs[id].waited) fail("incomplete/unacknowledged copy at final drain");
  if (a_jobs!=16 || b_jobs!=256 || c_jobs!=16 || next_job!=288 ||
      compute_started!=256 || compute_finished!=256 || allocation_count!=6)
    fail("fixed work conservation");
  for (unsigned i=0;i<6;i++) if (slots[i].active) fail("slot still live after return");
  FILE *values=open_output(argv[1],"output.bin","wb");
  for (uint32_t i=0;i<ELEMENTS;i++) write_u32_le(values,c[i]);
  if (fclose(values)) fail("close output data");
  pthread_mutex_lock(&trace_lock);
  event_t check=blank_event("ref_check","application");check.matrix='C';check.size=ELEMENTS*sizeof(uint32_t);
  emit_locked(check);
  event_t complete=blank_event("app_complete","application");complete.matrix='C';complete.size=ELEMENTS*sizeof(uint32_t);
  emit_locked(complete);
  pthread_mutex_unlock(&trace_lock);
  if (fclose(trace_file)) fail("close trace");
  FILE *result=open_output(argv[1],"result.json","w");
  fprintf(result,
    "{\n  \"execution_mode\": \"HOST_SYNCHRONOUS_COPY_FUNCTIONAL\",\n"
    "  \"source_commit\": \"%s\",\n"
    "  \"width\": 256, \"height\": 256, \"stripe_height\": 16,\n"
    "  \"requested_threads\": 8, \"slots\": 6, \"slot_bytes\": 16384,\n"
    "  \"A_jobs\": %u, \"B_jobs\": %u, \"C_jobs\": %u, \"jobs\": %u,\n"
    "  \"compute_starts\": %u, \"compute_dones\": %u,\n"
    "  \"uint32_multiply_add_steps\": 16777216,\n"
    "  \"extra_single_barriers\": 512, \"all_jobs_completed_and_waited\": true,\n"
    "  \"host_reference_and_2a_mismatches\": %u, \"output_values\": 65536,\n"
    "  \"output_format\": \"uint32_little_endian\", \"output_sum_u64\": %" PRIu64 ",\n"
    "  \"reference_check_pass\": %s,\n"
    "  \"timing_claim\": \"NONE: HOST timestamps are observation order only\"\n}\n",
    SOURCE_COMMIT,a_jobs,b_jobs,c_jobs,next_job,compute_started,compute_finished,
    reference_mismatches,sum,reference_mismatches?"false":"true");
  if (fclose(result)) fail("close result");
  printf("HOST_FUNCTIONAL_REFERENCE_%s values=65536 mismatches=%u jobs=288 tiles=256 mode=CPU_SYNCHRONOUS_COPY_ONLY\n",
         reference_mismatches?"FAIL":"PASS",reference_mismatches);
  free(a);free(b);free(c);free(d);
  return reference_mismatches?1:0;
}
'''
text=pre+instrumented+post+init+tail
c=out/'hero-mm-functional.c';c.write_text(text)
diff=''.join(difflib.unified_diff(original.splitlines(keepends=True),instrumented.splitlines(keepends=True),fromfile='upstream/mm-large.c:44-276',tofile='private/extracted-double_buf_mm-with-two-trace-blocks'))
(out/'functional-instrumentation.diff').write_text(diff)
trace_columns=['event_id','host_ns','thread_id','event','role','matrix','stripe','tile','linear_tile','slot','generation','byte_offset','size_bytes','job_id','a_slot','a_generation','b_slot','b_generation','c_slot','c_generation']
build={
 'status':'NOT_COMPILED_NOT_RUN','cwd':str(out),
 'compile_argv':['cc','-O2','-std=c11','-fopenmp','-pthread','-Wall','-Wextra',str(c),'-o',str(base/'functional-execution/hero-mm-functional')],
 'prepare_output_directory':str(base/'functional-execution'),
 'run_argv':[str(base/'functional-execution/hero-mm-functional'),str(base/'functional-execution/run-default')],
 'run_env':{'OMP_DYNAMIC':'FALSE','OMP_NUM_THREADS':'8','OMP_THREAD_LIMIT':'8'},
 'trace_interface':{
  'filename':'trace.csv','columns':trace_columns,'missing_numeric':-1,
  'events':['slot_alloc','copy_issue','copy_done','wait','compute_start','compute_done','slot_free','ref_check','app_complete'],
  'order':'event_id increments under pthread mutex; host_ns is real CLOCK_MONOTONIC sampled while holding same mutex; not targetcycles',
  'role':'controller_A/B/C are actual copy/wait manager thread0/1/2; compute_observer is actual thread selected by omp single; allocator/application operate before/after original team',
  'stripe':'copyA/C external stripe s; copyB external stripe t; compute output rowstripe s',
  'tile':'copyB t; compute t; A/C copies use-1',
  'linear_tile':'copyB sequential q=s*16+t; compute q=s*16+t; other events-1',
  'generation':'per physical role/slot starts-1; advances to0 for first input copy or Cfirstcompute row; expectedA/Cfloor(s/2), Bfloor(q/2)',
  'byte_offset':'copy external byteoffset s*16384 or t*16384; compute first output element offset(s*16*256+t*16)*4; actual tile footprint16rows*16columns rowpitch1024',
  'size_bytes':'all copies16384; compute writes1024bytes as a strided tile, not a contiguous1024byte interval; metadata value does not reduce MACcount',
  'job_id':'global unique positive copyjob, includes final blockingCcopy; all nonjob events-1',
  'slot_fields':'compute records actual A/B/C slot and current generation after precompute barrier; other events-1',
  'copy_semantics':'synchronous CPU memcpy from call start to completed copy; hero API naming retained solely to execute original control structure'},
 'output_interface':{'data':'output.bin, exactly262144bytes containing all65536uint32 in explicit little endian','marker':'result.json plus stdout HOST_FUNCTIONAL_REFERENCE_PASS/FAIL','metadata':'fixed sourcecommit/defaults/counts, extra512barriers, reference/2amismatchcount; external runner should add executable/compiler/hash metadata'},
 'claims_allowed':['actual fixed kernel functional output','observed CPU program issue/copydone/wait/compute/slot order'],
 'claims_not_supported':['targetcompute cycles','asynchronousDMA performance','AXIBretirement','IOMMUinterference','G0timing gate passed']
}
(out/'functional-build-argv.json').write_text(json.dumps(build,indent=2)+'\n')
manifest={
 'schema':'source-derived-functional-v1','status':'AUTHORED_ONLY_NOT_COMPILED_NOT_RUN',
 'source_repository':'pulp-platform/hero','source_commit':'c8481a930a15a385d4f58f519bed83d2e75e2a73',
 'source_url':'https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/mm-large/mm-large.c',
 'source_local_path':str(source),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
 'license':'Apache-2.0; sourceheader retained by attribution; fulllicense at source-evidence/hero/openmp-examples/LICENSE',
 'original_function_lines':[44,276],'original_function_sha256':hashlib.sha256(original.encode()).hexdigest(),
 'instrumented_function_sha256':hashlib.sha256(instrumented.encode()).hexdigest(),
 'original_initialization_lines':[320,326],'original_host_reference_lines':[335,346],
 'function_changes':'Only two omp-single trace blocks inserted: after original explicit barrier208 and after original omp-for implicit completion at228. All original statements and waits preserved; address-space macros erased outside function.',
 'instrumentation_diff_path':str(out/'functional-instrumentation.diff'),
 'sync_shim_changes':'malloc/free replace unavailable hostL1 allocation; CPU synchronousmemcpy plus uniquejob/waitvalidation replace hardwareDMA; final blockingcopy calls same shim andwait. No original offload main/runtime executed.',
 'fixed_parameter_derivation':'Original noargument256, stripehalving with >=32KiB gives16; original fullunsigned arithmetic and8threads preserved.',
 'extra_sync':'512 additional omp-single blocks with implicit barriers, two pertile; performance invalidated explicitly.',
 'authorship':'Codex adapted and authored shims/instrumentation; original function/init/reference by upstream HERO authors.',
 'proposed_files':[{ 'path':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()} for path in [c,out/'functional-build-argv.json',out/'functional-instrumentation.diff']],
 'source_download_modified':False,'compile_count':0,'functional_run_count':0,'DMA_run_count':0,'production_edits':0
}
(out/'functional-source-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
# Authorship-only integrity checks, not compile/run.
assert text.count(original)==0
assert instrumented.replace('\n'+start,'').replace('\n'+done,'')==original
assert len(list(out.iterdir()))==4
print(json.dumps({'files':[str(f) for f in sorted(out.iterdir())],'lines':len(text.splitlines()),'trace_columns':trace_columns,'source_function_preserved_outside_two_blocks':True,'compile':False,'run':False}))
