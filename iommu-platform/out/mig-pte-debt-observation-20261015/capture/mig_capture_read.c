#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

/* Private readback only: writes observer INDEX and the cache-invalidating
 * DATA command at research 0xf00. No ARM, mode, DMA, mapping or task writes. */
#define BASE 0x50000000ULL
#define WORDS 4608u
#define NATIVE_WORDS 3584u
#define BUILD_TAG 0x4d444f3130313502ULL
#define NATIVE_MAGIC 0x4d49474445425402ULL
#define CACHE_MAGIC 0x4d434301u
static volatile uint64_t *regs;
static void fence_io(void) {
#ifdef __riscv
    __asm__ __volatile__("fence iorw,iorw" ::: "memory");
#else
    atomic_thread_fence(memory_order_seq_cst);
#endif
}
static void set_index(unsigned research_index) {
    regs[0x70/8] = research_index + 512u;
    fence_io();
    (void)regs[0x70/8];
    fence_io();
}
static uint64_t data_read(void) { uint64_t v = regs[0x78/8]; fence_io(); return v; }
static uint64_t direct_read(unsigned research_index) {
    set_index(research_index);
    (void)data_read();
    return data_read();
}
static uint64_t monotonic_ns(void) {
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC,&t)) { perror("clock_gettime"); exit(2); }
    return (uint64_t)t.tv_sec*1000000000ULL+(uint64_t)t.tv_nsec;
}
static uint64_t native_read(unsigned offset) {
    set_index(0xf00);
    regs[0x78/8] = 1;
    fence_io();
    set_index(0x1000+offset);
    set_index(0xf00);
    uint64_t deadline=monotonic_ns()+1000000000ULL, status;
    do {
        status=data_read();
        if ((uint32_t)(status>>32)!=CACHE_MAGIC) {
            fprintf(stderr,"wrong cache magic: %016"PRIx64"\n",status);exit(2);
        }
        if ((status&3)==1 && ((status>>16)&0xfff)==offset) {
            set_index(0x1000+offset);
            return data_read();
        }
    } while(monotonic_ns()<deadline);
    fprintf(stderr,"cache timeout offset=%x status=%016"PRIx64"\n",offset,status);exit(2);
}
static void write_all(int fd,const void *data,size_t len) {
    const unsigned char *p=data;
    while(len) { ssize_t n=write(fd,p,len); if(n<0&&errno==EINTR)continue;
        if(n<=0){perror("write capture");exit(2);}p+=n;len-=(size_t)n; }
}
int main(int argc,char **argv) {
    int probe=argc==2&&!strcmp(argv[1],"--probe");
    if(!probe && !(argc==3&&!strcmp(argv[1],"--dump"))) {
        fprintf(stderr,"usage: %s --probe | --dump NEW-FILE\n",argv[0]);return 2;
    }
    int fd=open("/dev/mem",O_RDWR|O_SYNC|O_CLOEXEC);
    if(fd<0){perror("open /dev/mem");return 2;}
    void *m=mmap(NULL,16384,PROT_READ|PROT_WRITE,MAP_SHARED,fd,BASE);
    if(m==MAP_FAILED){perror("mmap DMA observer");return 2;}
    regs=m;
    uint64_t tag=direct_read(0xf01),cache=direct_read(0xf00);
    if(tag!=BUILD_TAG || (uint32_t)(cache>>32)!=CACHE_MAGIC){
        fprintf(stderr,"private observer identity mismatch tag=%016"PRIx64" cache=%016"PRIx64"\n",tag,cache);return 2;
    }
    uint64_t magic=native_read(0);
    if(magic!=NATIVE_MAGIC){fprintf(stderr,"native magic mismatch %016"PRIx64"\n",magic);return 2;}
    if(probe){
        uint64_t errors=native_read(2),status=native_read(1),source=direct_read(0x801);
        printf("{\"schema\":\"mig-readback-probe-v2\",\"build_tag\":\"%016"PRIx64"\",\"cache_magic\":\"%08x\",\"native_magic\":\"%016"PRIx64"\",\"native_status\":\"%016"PRIx64"\",\"native_errors\":\"%016"PRIx64"\",\"source_errors\":%"PRIu64"}\n",tag,CACHE_MAGIC,magic,status,errors,source>>32);
        if(errors || (status&0x801)!=0x801 || (source>>32)) {
            fprintf(stderr,"tracker not valid/calibrated from reset; workload must not start\n");return 2;
        }
        return 0;
    }
    uint64_t words[WORDS];
    for(unsigned i=0;i<WORDS;i++)words[i]=i<NATIVE_WORDS?native_read(i):direct_read(0x800+i-NATIVE_WORDS);
    uint64_t final_quiet=direct_read(0xd25), frontend_status[4];
    for(unsigned lane=0;lane<4;lane++){ frontend_status[lane]=regs[(lane*0x1000+0x20)/8]; fence_io(); }
    int out=open(argv[2],O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC,0600);
    if(out<0){perror("create new dump");return 2;}
    write_all(out,words,sizeof(words));
    if(fsync(out)||close(out)){perror("finish dump");return 2;}
    printf("{\"schema\":\"mig-readback-receipt-v2\",\"build_tag\":\"%016"PRIx64"\",\"first_research_index\":4096,\"words\":%u,\"bytes\":%zu,\"native_status\":\"%016"PRIx64"\",\"native_errors\":\"%016"PRIx64"\",\"source_status\":\"%016"PRIx64"\",\"final_composition_quiet\":%"PRIu64",\"frontend_backend_busy\":[%"PRIu64",%"PRIu64",%"PRIu64",%"PRIu64"],\"byte_order\":\"little-endian-rv64\"}\n",tag,WORDS,sizeof(words),words[1],words[2],words[NATIVE_WORDS+1],final_quiet,frontend_status[0],frontend_status[1],frontend_status[2],frontend_status[3]);
    munmap(m,16384);close(fd);return 0;
}
