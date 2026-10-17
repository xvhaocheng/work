#define _GNU_SOURCE
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/mman.h>
#include <signal.h>
static unsigned int os_open_calls;
static int blocked_open(const char *name, int flags, ...)
{
    (void)name;(void)flags;os_open_calls++;
    errno = EACCES;
    return -1;
}
#define PN_CONFIG_HOST_TEST 1
#define PN_CONFIG_ACCESSOR_TEST 1
#define PN_CONFIG_ENTRY_TEST 1
#define open blocked_open
#define main pn_program_main
#include "../pn_config.c"
#undef main
#undef open

static unsigned int checks, cases;
#define CHECK(x) do { checks++; if (!(x)) { fprintf(stderr,"check failed %s:%d: %s\n",__FILE__,__LINE__,#x); exit(2); } } while (0)
struct event { char type; unsigned int offset; uint64_t value; int error; };
struct fake {
    uint64_t cap, mode, status, gap, gapcap, obs;
    uint64_t after_cap, after_mode, after_status, after_gap, after_gapcap, after_obs;
    unsigned int override_after;
    unsigned int selected, call_count, loads, selectors, functions, accepted, clear_count;
    int fault_call, wrong_index_call;
    bool post, reject, write_fault_after, verified;
    struct event events[64];
};
static struct fake fresh(void)
{
    struct fake f = {0};
    f.cap=PN_CAP;f.gapcap=PN_GAP_CAP;f.status=PN_HISTORY|PN_REQUIRED;
    f.selected=UINT_MAX;f.fault_call=f.wrong_index_call=-1;
    return f;
}
static int start_event(struct fake *f,char type,unsigned int offset,uint64_t value)
{
    unsigned int n=f->call_count++;
    CHECK(n<64);f->events[n]=(struct event){type,offset,value,0};
    if ((int)n==f->fault_call) { f->events[n].error=EIO;return EIO; }
    return 0;
}
static uint64_t scalar(struct fake *f,unsigned int bit,uint64_t before,uint64_t after)
{
    return f->post && (f->override_after&bit) ? after : before;
}
static int fake_load(void *context,unsigned int offset,uint64_t *value)
{
    struct fake *f=context;int e;uint64_t v=0;
    CHECK(offset==PN_OBS||offset==PN_INDEX||offset==PN_DATA||offset==PN_GAP||offset==PN_GAPCAP);
    f->loads++;e=start_event(f,'R',offset,0);if(e)return e;
    switch(offset) {
    case PN_OBS:v=scalar(f,PN_HAVE_OBS,f->obs,f->after_obs);break;
    case PN_GAP:v=scalar(f,PN_HAVE_GAP,f->gap,f->after_gap);break;
    case PN_GAPCAP:v=scalar(f,PN_HAVE_GAPCAP,f->gapcap,f->after_gapcap);break;
    case PN_INDEX:
        v=f->selected;
        if ((int)(f->call_count-1)==f->wrong_index_call)v|=UINT64_C(1)<<32;
        f->verified=v==f->selected;
        break;
    case PN_DATA:
        CHECK(f->verified && pn_index_allowed(f->selected));
        if(f->selected==192)v=scalar(f,PN_HAVE_CAP,f->cap,f->after_cap);
        if(f->selected==193)v=scalar(f,PN_HAVE_MODE,f->mode,f->after_mode);
        if(f->selected==194)v=scalar(f,PN_HAVE_STATUS,f->status,f->after_status);
        break;
    default:CHECK(false);
    }
    *value=v;f->events[f->call_count-1].value=v;
    return 0;
}
static int fake_select(void *context,unsigned int index)
{
    struct fake *f=context;int e;
    CHECK(pn_index_allowed(index));f->selectors++;f->verified=false;
    e=start_event(f,'I',PN_INDEX,index);if(e)return e;f->selected=index;return 0;
}
static int fake_mode(void *context,uint64_t value)
{
    struct fake *f=context;int e;
    CHECK(pn_mode_allowed(value)&&f->selected==193&&f->verified);
    CHECK(f->functions==0);f->functions++;f->verified=false;
    e=start_event(f,'M',PN_DATA,value);if(e)return e;
    f->post=true;
    if(f->reject)f->status|=PN_REJECT;
    else {f->mode=value;f->status&=~PN_REJECT;f->accepted++;f->clear_count++;}
    if(f->write_fault_after){f->events[f->call_count-1].error=EIO;return EIO;}
    return 0;
}
static int fake_gap(void *context,uint64_t value)
{
    struct fake *f=context;int e;
    CHECK(pn_gap_allowed(value)&&f->functions==0);f->functions++;
    e=start_event(f,'G',PN_GAP,value);if(e)return e;
    f->post=true;
    if(!f->reject){f->gap=value;f->accepted++;}
    if(f->write_fault_after){f->events[f->call_count-1].error=EIO;return EIO;}
    return 0;
}
static struct pn_io fake_io(struct fake *f)
{
    struct pn_io io={f,fake_load,fake_select,fake_mode,fake_gap};return io;
}
static void report(const char *name,const struct pn_result *r,const struct fake *f)
{
    unsigned int i;
    printf("{\"case\":\"%s\",\"result\":",name);pn_json(stdout,r);
    printf(",\"fake_accepted_writes\":%u,\"fixture_clear_count\":%u,\"trace\":[",f->accepted,f->clear_count);
    for(i=0;i<f->call_count;i++)printf("%s{\"type\":\"%c\",\"offset\":%u,\"value\":%"PRIu64",\"error\":%d}",i?",":"",f->events[i].type,f->events[i].offset,f->events[i].value,f->events[i].error);
    printf("]}\n");cases++;
}
static struct pn_result execute(const char *name,struct fake *f,enum pn_operation op,uint64_t value,bool success,unsigned int functions)
{
    struct pn_result r;struct pn_io io=fake_io(f);
    struct pn_request req={op,value,true};
    int error=pn_configure(&io,req,&r);
    CHECK((error==0)==success);CHECK((r.error==0)==success);CHECK(r.readback_verified==success);
    CHECK(f->functions==functions && r.functional_attempts==functions && functions<=1);
    CHECK(r.index_attempts==f->selectors);CHECK(os_open_calls==0);
    report(name,&r,f);return r;
}
static void transition_tests(void)
{
    static const uint64_t modes[]={0,1,2,3,6},gaps[]={0,1000,4000};
    unsigned int a,b,reject;char name[96];
    for(a=0;a<5;a++)for(b=0;b<5;b++){
        struct fake f=fresh();f.mode=modes[a];f.status|=PN_REJECT;
        snprintf(name,sizeof(name),"mode_%u_to_%u",(unsigned)modes[a],(unsigned)modes[b]);
        execute(name,&f,PN_SET_MODE,modes[b],true,1);
        CHECK(f.mode==modes[b]&&f.clear_count==1&&!(f.status&PN_REJECT));
    }
    for(a=0;a<5;a++)for(b=0;b<3;b++)for(reject=0;reject<2;reject++){
        struct fake f=fresh();f.mode=modes[a];f.gap=1000;if(reject)f.status|=PN_REJECT;
        snprintf(name,sizeof(name),"gap_%u_mode_%u_reject_%u",(unsigned)gaps[b],(unsigned)modes[a],reject);
        execute(name,&f,PN_SET_GAP,gaps[b],true,1);
        CHECK(f.mode==modes[a]&&f.gap==gaps[b]&&f.clear_count==0&&!!(f.status&PN_REJECT)==!!reject);
    }
    {struct fake f=fresh();f.status=PN_REQUIRED;execute("nonhistory_hardware_mode2",&f,PN_SET_MODE,2,true,1);}
    {struct fake f=fresh();f.gap=42;execute("existing_nonwhitelist_gap_is_u32",&f,PN_SET_MODE,3,true,1);CHECK(f.gap==42);}
    {struct fake f=fresh();f.obs=UINT64_C(0x12340008);execute("observer_nonbusy_fields_not_sample_gate",&f,PN_SET_GAP,0,true,1);}
}
static void negative_tests(void)
{
    struct fake f;unsigned int i;char name[96];
    f=fresh();f.cap^=1;execute("bad_cap",&f,PN_SET_MODE,1,false,0);
    f=fresh();f.gapcap^=1;execute("bad_gapcap",&f,PN_SET_MODE,1,false,0);
    f=fresh();f.mode=6;f.status=PN_REQUIRED;execute("current6_unsupported",&f,PN_SET_MODE,2,false,0);
    f=fresh();f.status=PN_REQUIRED;execute("requested6_unsupported",&f,PN_SET_MODE,6,false,0);
    f=fresh();f.gap=UINT64_C(1)<<32;execute("current_gap_high_alias",&f,PN_SET_GAP,1000,false,0);
    {static const uint64_t bad[]={4,5,7,8,UINT64_C(0x100000006),UINT64_MAX};
        for(i=0;i<sizeof(bad)/sizeof(bad[0]);i++){f=fresh();f.mode=bad[i];snprintf(name,sizeof(name),"bad_current_mode_%u",i);execute(name,&f,PN_SET_MODE,0,false,0);}}
    for(i=6;i<63;i++){f=fresh();f.status|=UINT64_C(1)<<i;snprintf(name,sizeof(name),"reserved_status_bit_%u",i);execute(name,&f,PN_SET_GAP,0,false,0);}
    for(i=0;i<2;i++){f=fresh();f.obs=1U<<i;snprintf(name,sizeof(name),"observer_busy_bit_%u",i);execute(name,&f,PN_SET_GAP,0,false,0);}
    {static const uint64_t masks[]={1,16,4,8,32};
        for(i=0;i<5;i++){f=fresh();if(i<2)f.status&=~masks[i];else f.status|=masks[i];snprintf(name,sizeof(name),"bad_eligibility_%u",i);execute(name,&f,PN_SET_MODE,2,false,0);}}
    f=fresh();f.reject=true;execute("rejected_same_mode_not_just_readback",&f,PN_SET_MODE,0,false,1);CHECK(f.clear_count==0);
    f=fresh();f.mode=2;f.reject=true;execute("rejected_different_mode",&f,PN_SET_MODE,6,false,1);CHECK(f.mode==2);
    f=fresh();f.reject=true;execute("rejected_gap_different_value",&f,PN_SET_GAP,1000,false,1);CHECK(f.gap==0);
    for(i=0;i<2;i++){f=fresh();f.write_fault_after=true;snprintf(name,sizeof(name),"uncertain_after_effect_%u",i);execute(name,&f,i?PN_SET_GAP:PN_SET_MODE,i?1000:3,false,1);CHECK(f.accepted==1);}
    f=fresh();f.override_after=PN_HAVE_CAP;f.after_cap=0;execute("after_cap_corrupt",&f,PN_SET_MODE,1,false,1);
    f=fresh();f.override_after=PN_HAVE_GAPCAP;f.after_gapcap=0;execute("after_gapcap_corrupt",&f,PN_SET_GAP,1000,false,1);
    f=fresh();f.override_after=PN_HAVE_MODE;f.after_mode=2;execute("after_mode_mismatch",&f,PN_SET_MODE,3,false,1);
    f=fresh();f.mode=6;f.override_after=PN_HAVE_MODE;f.after_mode=2;execute("mode_changed_during_gap_full193",&f,PN_SET_GAP,1000,false,1);
    f=fresh();f.override_after=PN_HAVE_STATUS;f.after_status=PN_HISTORY|PN_REQUIRED|PN_REJECT;execute("gap_cannot_claim_clear_or_change_reject",&f,PN_SET_GAP,1000,false,1);
    f=fresh();f.override_after=PN_HAVE_STATUS;f.after_status=PN_REQUIRED;execute("history_support_changed",&f,PN_SET_MODE,2,false,1);
    f=fresh();f.override_after=PN_HAVE_STATUS;f.after_status=PN_HISTORY|PN_REQUIRED|64;execute("after_reserved_status",&f,PN_SET_GAP,0,false,1);
    f=fresh();f.override_after=PN_HAVE_STATUS;f.after_status=PN_HISTORY|PN_REQUIRED|4;execute("after_certificate_fault",&f,PN_SET_MODE,1,false,1);
    f=fresh();f.override_after=PN_HAVE_OBS;f.after_obs=1;execute("after_observer_armed",&f,PN_SET_GAP,0,false,1);
    f=fresh();f.override_after=PN_HAVE_GAP;f.after_gap=4000;execute("gap_changed_during_mode",&f,PN_SET_MODE,3,false,1);
    f=fresh();f.override_after=PN_HAVE_GAP;f.after_gap=4000;execute("gap_readback_mismatch",&f,PN_SET_GAP,1000,false,1);
}
static void fault_tests(void)
{
    unsigned int op,i;char name[96];
    for(op=0;op<2;op++){
        struct fake baseline=fresh();struct pn_io io=fake_io(&baseline);struct pn_result r;
        struct pn_request req={op?PN_SET_GAP:PN_SET_MODE,op?1000:3,true};
        CHECK(pn_configure(&io,req,&r)==0);
        for(i=0;i<baseline.call_count;i++){
            struct fake f=fresh();unsigned int expected_functions=0,j;
            for(j=0;j<=i;j++)if(baseline.events[j].type=='M'||baseline.events[j].type=='G')expected_functions++;
            f.fault_call=(int)i;snprintf(name,sizeof(name),"fault_each_access_%u_%u_%c",op,i,baseline.events[i].type);
            execute(name,&f,req.operation,req.value,false,expected_functions);CHECK(f.call_count==i+1);
            if(baseline.events[i].type=='R'&&baseline.events[i].offset==PN_INDEX){
                f=fresh();f.wrong_index_call=(int)i;snprintf(name,sizeof(name),"index_alias_%u_%u",op,i);
                execute(name,&f,req.operation,req.value,false,expected_functions);CHECK(f.call_count==i+1);
            }
        }
    }
}
static void argument_tests(void)
{
    static const char *const bad[]={"","4","5","7","+0","-0","00","01","06","0x0","6 "," 6","6\n","6x","4294967302","18446744073709551616","$(echo 0)","0;true"};
    unsigned int i,op;char name[96];
    {
        static const char *const commands[]={"set-mode","set-mode","set-mode","set-mode","set-mode","set-gap","set-gap","set-gap"};
        static const char *const values[]={"0","1","2","3","6","0","1000","4000"};
        static const uint64_t expected[]={0,1,2,3,6,0,1000,4000};
        for(i=0;i<8;i++){
            char *argv[]={"pn-config",(char *)commands[i],(char *)values[i],NULL};
            struct pn_request req=pn_arguments(3,argv);struct fake f=fresh();
            CHECK(req.valid&&req.value==expected[i]);
            snprintf(name,sizeof(name),"valid_exact_argv_%u",i);execute(name,&f,req.operation,req.value,true,1);
        }
    }
    for(op=0;op<2;op++)for(i=0;i<sizeof(bad)/sizeof(bad[0]);i++){
        char *argv[]={"pn-config",op?"set-gap":"set-mode",(char *)bad[i],NULL};
        struct pn_request req=pn_arguments(3,argv);struct fake f=fresh();struct pn_io io=fake_io(&f);struct pn_result r;
        CHECK(!req.valid);CHECK(pn_configure(&io,req,&r)==EINVAL);CHECK(f.call_count==0);
        snprintf(name,sizeof(name),"invalid_argv_%u_%u",op,i);report(name,&r,&f);
    }
    for(i=0;i<6;i++){
        char *argv[]={"pn-config",i==4?"write":"set-mode",i==5?"0":"4","extra",NULL};
        int argc=i<4?(int)i:3;struct pn_request req=pn_arguments(argc,argv);struct fake f=fresh();struct pn_io io=fake_io(&f);struct pn_result r;
        if(i==5){argv[1]="set-mode";argc=4;req=pn_arguments(argc,argv);}
        CHECK(!req.valid);CHECK(pn_configure(&io,req,&r)==EINVAL);CHECK(!f.call_count);
        snprintf(name,sizeof(name),"invalid_argc_or_command_%u",i);report(name,&r,&f);
    }
    /* Invoke the actual production main flow with only open interposed. All
       malformed forms must return before that mock OS call, with real JSON. */
    for(i=0;i<sizeof(bad)/sizeof(bad[0]);i++){
        char *argv[]={"pn-config","set-mode",(char *)bad[i],NULL};
        char *text=NULL;size_t size=0;FILE *original=stdout;FILE *capture=open_memstream(&text,&size);int rc;
        CHECK(capture!=NULL);stdout=capture;rc=pn_program_main(3,argv);CHECK(fclose(capture)==0);stdout=original;
        CHECK(rc==1&&os_open_calls==0&&size>0);
        if(size&&text[size-1]=='\n')text[size-1]='\0';
        printf("{\"case\":\"actual_main_invalid_%u\",\"os_open_calls\":%u,\"result\":%s}\n",i,os_open_calls,text);free(text);cases++;
    }
}
static void accessor_tests(void)
{
    _Alignas(8) uint64_t memory[512],original[512];struct pn_mmio m;uint64_t value;unsigned int i,j;
    const uint64_t modes[]={0,1,2,3,6},gaps[]={0,1000,4000};
    for(i=0;i<512;i++)memory[i]=UINT64_C(0xabcd000000000000)|i;
    memcpy(original,memory,sizeof memory);
    memset(&m,0,sizeof m);m.mapping=(volatile unsigned char *)memory;
    for(i=0;i<4096;i++)if(i!=PN_OBS&&i!=PN_INDEX&&i!=PN_DATA&&i!=PN_GAP&&i!=PN_GAPCAP){CHECK(pn_mmio_load64(&m,i,&value)==EINVAL);}
    CHECK(pn_mmio_load64(&m,PN_DATA,&value)==EACCES);
    for(i=0;i<256;i++)if(!pn_index_allowed(i))CHECK(pn_mmio_select_index(&m,i)==EINVAL);
    CHECK(memcmp(memory,original,sizeof memory)==0);
    for(i=0;i<5;i++){
        memcpy(memory,original,sizeof memory);memset(&m,0,sizeof m);m.mapping=(volatile unsigned char *)memory;
        CHECK(pn_mmio_write_mode(&m,modes[i])==EACCES);CHECK(pn_mmio_select_index(&m,193)==0);
        CHECK(pn_mmio_write_mode(&m,modes[i])==EACCES);CHECK(pn_mmio_load64(&m,PN_INDEX,&value)==0&&value==193);
        CHECK(pn_mmio_write_mode(&m,modes[i])==0&&memory[PN_DATA/8]==modes[i]);
        CHECK(pn_mmio_write_mode(&m,modes[i])==EACCES&&pn_mmio_write_gap(&m,0)==EACCES);
        for(j=0;j<512;j++)if(j!=PN_INDEX/8&&j!=PN_DATA/8)CHECK(memory[j]==original[j]);
    }
    for(i=0;i<3;i++){
        memcpy(memory,original,sizeof memory);memset(&m,0,sizeof m);m.mapping=(volatile unsigned char *)memory;
        CHECK(pn_mmio_write_gap(&m,gaps[i])==0&&memory[PN_GAP/8]==gaps[i]);CHECK(pn_mmio_write_gap(&m,gaps[i])==EACCES);
        for(j=0;j<512;j++)if(j!=PN_GAP/8)CHECK(memory[j]==original[j]);
    }
    memset(&m,0,sizeof m);m.mapping=(volatile unsigned char *)memory;
    for(i=4;i<32;i++)if(!pn_mode_allowed(i))CHECK(pn_mmio_write_mode(&m,i)==EINVAL);
    CHECK(pn_mmio_write_mode(&m,UINT64_C(0x100000006))==EINVAL&&pn_mmio_write_gap(&m,UINT64_C(0x100000000))==EINVAL);
    CHECK(pn_mmio_write_gap(&m,1)==EINVAL&&pn_mmio_write_gap(&m,4001)==EINVAL);
    CHECK(!m.functional_attempted);
    {
        struct sigaction action={.sa_handler=pn_fault_handler},old_bus,old_segv;
        void *page=mmap(NULL,4096,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
        CHECK(page!=MAP_FAILED);sigemptyset(&action.sa_mask);CHECK(sigaction(SIGBUS,&action,&old_bus)==0);CHECK(sigaction(SIGSEGV,&action,&old_segv)==0);
        memset(&m,0,sizeof m);m.mapping=page;CHECK(pn_mmio_load64(&m,PN_OBS,&value)==EIO&&pn_access_active==0);
        CHECK(pn_mmio_select_index(&m,193)==EIO&&!m.selection_verified&&pn_access_active==0);
        m.selected=193;m.selection_verified=true;CHECK(pn_mmio_write_mode(&m,3)==EIO&&m.functional_attempted&&pn_access_active==0);
        CHECK(pn_mmio_write_gap(&m,0)==EACCES);memset(&m,0,sizeof m);m.mapping=page;
        CHECK(pn_mmio_write_gap(&m,1000)==EIO&&m.functional_attempted&&pn_access_active==0);
        CHECK(sigaction(SIGSEGV,&old_segv,NULL)==0&&sigaction(SIGBUS,&old_bus,NULL)==0);CHECK(munmap(page,4096)==0);
    }
    printf("{\"case\":\"actual_production_accessor_whitelists_and_faults\",\"fixture_only\":true,\"source_board_cases\":0,\"success\":true}\n");cases++;
}
int main(void)
{
    transition_tests();negative_tests();fault_tests();argument_tests();accessor_tests();
    printf("{\"summary\":true,\"cases\":%u,\"checks\":%u,\"fixture_only\":true,\"source_board_cases\":0,\"os_open_calls\":%u}\n",cases,checks,os_open_calls);
    return fflush(stdout)||ferror(stdout)?1:0;
}
