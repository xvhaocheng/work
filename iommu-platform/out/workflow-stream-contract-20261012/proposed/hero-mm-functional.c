/*
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
static unsigned observed_team_threads;
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
  observed_team_threads=(unsigned)omp_get_num_threads();
  if (observed_team_threads!=8) fail("actual compute team is not eight threads");
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
int double_buf_mm(__host uint32_t * __restrict__ a,
                  __host uint32_t * __restrict__ b,
                  __host uint32_t * __restrict__ c,
                  uint32_t width, uint32_t height, uint32_t stripe_height)
{
  const uint32_t n_stripes = height / stripe_height;
  const uint32_t stripe_size_b = width * stripe_height * sizeof(uint32_t);

  __device uint32_t * a_ptrs[2];
  __device uint32_t * b_ptrs[2];
  __device uint32_t * c_ptrs[2];

  hero_dma_job_t a_dma[2];
  hero_dma_job_t b_dma[2];
  hero_dma_job_t c_dma[2];

  uint32_t a_idx = 0;
  uint32_t c_idx = 0;
  uint32_t b_idx = 0;

  // allocate the buffers
  a_ptrs[0] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
  a_ptrs[1] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
  b_ptrs[0] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
  b_ptrs[1] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
  c_ptrs[0] = (__device uint32_t *)hero_l1malloc(stripe_size_b);
  c_ptrs[1] = (__device uint32_t *)hero_l1malloc(stripe_size_b);

  if ( (a_ptrs[0] == NULL) || (a_ptrs[1] == NULL) ||
       (b_ptrs[0] == NULL) || (b_ptrs[1] == NULL) ||
       (c_ptrs[0] == NULL) || (c_ptrs[1] == NULL) ) {
    printf("ERROR: Memory allocation failed!\n");
    return -ENOMEM;
  }

  #pragma omp parallel \
    firstprivate(a_ptrs, b_ptrs, c_ptrs, width, height, stripe_height) \
    firstprivate(a_dma, b_dma, c_dma) \
    shared(a_idx, b_idx, c_idx) \
    shared(a, b, c) num_threads(8)
  {
    const int thread_id = omp_get_thread_num();

#ifdef TIME_DMA_AND_COMP
    unsigned comp_cycles = 0;
    unsigned dma_setup_cycles = 0;
    unsigned dma_wait_cycles = 0;
#endif

    // get the first stripes
    if (thread_id == 0) {
#ifdef TIME_DMA_AND_COMP
      const unsigned cycles_prev = hero_get_clk_counter();
#endif
      a_dma[a_idx] = hero_memcpy_host2dev_async(a_ptrs[a_idx], a, stripe_size_b);
#ifdef TIME_DMA_AND_COMP
      dma_setup_cycles += hero_get_clk_counter() - cycles_prev;
#endif
    }
    else if (thread_id == 1) {
#ifdef TIME_DMA_AND_COMP
      const unsigned cycles_prev = hero_get_clk_counter();
#endif
      b_dma[b_idx] = hero_memcpy_host2dev_async(b_ptrs[b_idx], b, stripe_size_b);
#ifdef TIME_DMA_AND_COMP
      dma_setup_cycles += hero_get_clk_counter() - cycles_prev;
#endif
    }

    // horizontal a and c stripes
    for (uint32_t s=0; s<n_stripes; s++) {

      if (thread_id == 0) {
        // swap buffer
        a_idx = a_idx ? 0 : 1;

        if (s < n_stripes-1) {
          // determine next DMA XFER
          const uint64_t ext_addr = (uint64_t)a + (s+1)*stripe_size_b;

          // set up DMA XFER
#ifdef TIME_DMA_AND_COMP
          const unsigned cycles_prev = hero_get_clk_counter();
#endif
          a_dma[a_idx] = hero_memcpy_host2dev_async(a_ptrs[a_idx], (__host void *)ext_addr, stripe_size_b);
#ifdef TIME_DMA_AND_COMP
          dma_setup_cycles += hero_get_clk_counter() - cycles_prev;
#endif
        }

        // wait for previous DMA XFER
#ifdef TIME_DMA_AND_COMP
        const unsigned cycles_prev = hero_get_clk_counter();
#endif
        hero_dma_wait(a_dma[!a_idx]);
#ifdef TIME_DMA_AND_COMP
        dma_wait_cycles += hero_get_clk_counter() - cycles_prev;
#endif
      }
      else if ( (thread_id == 2) && (s > 0) ) {
        // swap buffer
        c_idx = c_idx ? 0 : 1;

        // determine next DMA XFER
        const uint64_t ext_addr = (uint64_t)c + (s-1)*stripe_size_b;

        // set up DMA XFER
#ifdef TIME_DMA_AND_COMP
        const unsigned cycles_prev = hero_get_clk_counter();
#endif
        c_dma[!c_idx] = hero_memcpy_dev2host_async((__host void *)ext_addr, c_ptrs[!c_idx], stripe_size_b);
#ifdef TIME_DMA_AND_COMP
        dma_setup_cycles += hero_get_clk_counter() - cycles_prev;
#endif

        // wait for previous DMA XFER
        if (s > 1)
          hero_dma_wait(c_dma[c_idx]);
      }

      // vertical b stripes
      for (uint32_t t=0; t<n_stripes; t++) {

        if ( (thread_id == 1) ) {
          // swap buffer
          b_idx = b_idx ? 0 : 1;

          if (t < n_stripes-1) {
            // determine next DMA XFER
            const uint64_t ext_addr = (uint64_t)b + (t+1)*stripe_size_b;

            // set up DMA XFER
#ifdef TIME_DMA_AND_COMP
            const unsigned cycles_prev = hero_get_clk_counter();
#endif
            b_dma[b_idx] = hero_memcpy_host2dev_async(b_ptrs[b_idx], (__host void *)ext_addr, stripe_size_b);
#ifdef TIME_DMA_AND_COMP
            dma_setup_cycles += hero_get_clk_counter() - cycles_prev;
#endif
          }
          else if (s < n_stripes-1) {
            // determine next DMA XFER
            const uint64_t ext_addr = (uint64_t)b;

            // set up DMA XFER
#ifdef TIME_DMA_AND_COMP
            const unsigned cycles_prev = hero_get_clk_counter();
#endif
            b_dma[b_idx] = hero_memcpy_host2dev_async(b_ptrs[b_idx], (__host void *)ext_addr, stripe_size_b);
#ifdef TIME_DMA_AND_COMP
            dma_setup_cycles += hero_get_clk_counter() - cycles_prev;
#endif
          }

          // wait for previous DMA XFER
#ifdef TIME_DMA_AND_COMP
          const unsigned cycles_prev = hero_get_clk_counter();
#endif
          hero_dma_wait(b_dma[!b_idx]);
#ifdef TIME_DMA_AND_COMP
          dma_wait_cycles += hero_get_clk_counter() - cycles_prev;
#endif
        }

        #pragma omp barrier

        #pragma omp single
        {
          trace_compute("compute_start", s, t, a_ptrs[!a_idx],
                        b_ptrs[!b_idx], c_ptrs[c_idx]);
        }

#ifdef TIME_DMA_AND_COMP
        const unsigned cycles_prev = hero_get_clk_counter();
#endif

        #pragma omp for collapse(2)

        // horizontal a and c rows
        for (uint32_t i=0; i<stripe_height; i++) {

          // vertical b columns
          for (uint32_t j=0; j<stripe_height; j++) {

            uint32_t sum = 0;
            for (uint32_t k=0; k<width; k++) {
              sum = sum + a_ptrs[!a_idx][i*width+k] * b_ptrs[!b_idx][j*width+k];
            } // k < width
            c_ptrs[c_idx][i*width+t*stripe_height+j] = sum;
          } // j < stripe_height
        } // i < stripe_height

        #pragma omp single
        {
          trace_compute("compute_done", s, t, a_ptrs[!a_idx],
                        b_ptrs[!b_idx], c_ptrs[c_idx]);
        }

#ifdef TIME_DMA_AND_COMP
        comp_cycles += hero_get_clk_counter() - cycles_prev;
#endif

      } // t < n_stripes

    } // n_stripes

    // wait for second to last c stripe
    if (thread_id == 2) {
#ifdef TIME_DMA_AND_COMP
      const unsigned cycles_prev = hero_get_clk_counter();
#endif
      hero_dma_wait(c_dma[!c_idx]);
#ifdef TIME_DMA_AND_COMP
      dma_wait_cycles += hero_get_clk_counter() - cycles_prev;
#endif
    }

    // copy out last c stripe
    if (thread_id == 2) {
#ifdef TIME_DMA_AND_COMP
      const unsigned cycles_prev = hero_get_clk_counter();
#endif
      hero_memcpy_dev2host((__host void *)((uint64_t)c+(n_stripes-1)*stripe_size_b), c_ptrs[c_idx], stripe_size_b);
#ifdef TIME_DMA_AND_COMP
      dma_setup_cycles += hero_get_clk_counter() - cycles_prev;
#endif
    }

#ifdef TIME_DMA_AND_COMP
    printf("Computation cycles: %d %10d\n", comp_cycles);
    printf("DMA setup cycles:   %d %10d\n", dma_setup_cycles);
    printf("DMA wait cycles:    %d %10d\n", dma_wait_cycles);
#endif

  } // parallel

  hero_l1free(a_ptrs[0]);
  hero_l1free(a_ptrs[1]);
  hero_l1free(b_ptrs[0]);
  hero_l1free(b_ptrs[1]);
  hero_l1free(c_ptrs[0]);
  hero_l1free(c_ptrs[1]);

  return 0;
}

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
  // Init matrices
  for (uint32_t i=0; i<width; i++) {
    for (uint32_t j=0; j<height; j++) {
      a[i*width+j] = i*width+j;
      b[i*width+j] = i == j ? 2 : 0;
    }
  }

  /* END original initialization. */
  int ret=double_buf_mm(a,b,c,width,height,stripe_height);
  if (ret) fail("double_buf_mm returned error");
  /* BEGIN upstream original host-reference computation lines 335-346. */
  #pragma omp parallel firstprivate(a, b, d, width, height) num_threads(1)
  {
    #pragma omp for collapse(2)
    for (uint32_t i=0; i<width; i++) {
      for (uint32_t j=0; j<height; j++) {
        uint32_t sum = 0;
        for (uint32_t k=0; k<width; k++)
          sum = sum + a[i*width+k] * b[j*width+k];
        d[i*width+j] = sum;
      }
    }
  }

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
    "  \"requested_threads\": 8, \"observed_team_threads\": %u, \"slots\": 6, \"slot_bytes\": 16384,\n"
    "  \"A_jobs\": %u, \"B_jobs\": %u, \"C_jobs\": %u, \"jobs\": %u,\n"
    "  \"compute_starts\": %u, \"compute_dones\": %u,\n"
    "  \"uint32_multiply_add_steps\": 16777216,\n"
    "  \"extra_single_barriers\": 512, \"all_jobs_completed_and_waited\": true,\n"
    "  \"host_reference_and_2a_mismatches\": %u, \"output_values\": 65536,\n"
    "  \"output_format\": \"uint32_little_endian\", \"output_sum_u64\": %" PRIu64 ",\n"
    "  \"reference_check_pass\": %s,\n"
    "  \"timing_claim\": \"NONE: HOST timestamps are observation order only\"\n}\n",
    SOURCE_COMMIT,observed_team_threads,a_jobs,b_jobs,c_jobs,next_job,compute_started,compute_finished,
    reference_mismatches,sum,reference_mismatches?"false":"true");
  if (fclose(result)) fail("close result");
  printf("HOST_FUNCTIONAL_REFERENCE_%s values=65536 mismatches=%u jobs=288 tiles=256 mode=CPU_SYNCHRONOUS_COPY_ONLY\n",
         reference_mismatches?"FAIL":"PASS",reference_mismatches);
  free(a);free(b);free(c);free(d);
  return reference_mismatches?1:0;
}
