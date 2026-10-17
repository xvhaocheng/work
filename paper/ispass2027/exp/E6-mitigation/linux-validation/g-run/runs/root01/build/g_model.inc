/* The MMIO job model runs real source/destination copies. Observer words below
 * are declared synthetic fixtures for driver decoding, not measured events.
 */
static void g_refresh(void)
{
    for (unsigned lane=0;lane<2;lane++) {
        struct model *h=&hw[lane];
        if (!h->pending || !g_started[lane] || drain_failure==(int)lane || clock_ns<g_finish_at[lane]) continue;
        h->pending=false;h->regs[0x30/8]=h->next_id;h->regs[0x88/8]=9;
        h->regs[0xa0/8]=h->completed;h->regs[0xa8/8]=h->completed+(g_inventory_lane==(int)lane);
        if (lane==0) hw[0].regs[IDMA_OBS_CONTROL_OFFSET/8]=4;
        else g_A_done_event=++g_event;
    }
}
static u64 g_word(unsigned slot)
{
    u64 w[32]={0};
    w[0]=IDMA_G_BANK_MAGIC;w[2]=g_has_a?0x3c:0x0c;w[3]=g_hw_error|(g_aborted?128:0);
    w[4]=4096;w[8]=hw[0].regs[0xa8/8];w[9]=hw[0].regs[0xa0/8];
    w[10]=w[16]=20000;w[27]=20000;w[28]=40000;
    if (g_has_a) {
        w[5]=4096;w[7]=g_latched_gap;w[11]=hw[1].regs[0xa8/8];w[12]=hw[1].regs[0xa0/8];
        w[15]=4095*g_latched_gap;w[13]=40000+w[15];w[29]=10000;w[30]=w[29]+w[13];
        if(g_scenario==0){w[13]=9000+w[15];w[29]=1000;w[30]=w[29]+w[13];}
        else if(g_scenario==1){w[13]=10000+w[15];w[29]=25000;w[30]=w[29]+w[13];w[17]=w[18]=4096;w[25]=1;w[26]=4096;}
        else if(g_scenario==5){w[19]=w[20]=1;w[23]=w[24]=1;}
        else {w[17]=w[18]=2048;w[25]=1025;w[26]=3072;
            if(g_scenario==3){w[19]=w[20]=w[21]=1;w[23]=1025;w[24]=3073;}
            if(g_scenario==4){w[19]=w[20]=w[22]=1;w[23]=1025;w[24]=3073;}
        }
    }
    return g_corrupt_word==(int)slot?g_corrupt_value:w[slot];
}
static u64 g_A_window(void){return !g_has_a||g_scenario==0||g_scenario==5?0:g_scenario==1?4096:2048;}
static u64 g_path(unsigned stage,unsigned field)
{
    u64 n=stage<8?4096:g_A_window();
    const u64 fields[]={n,n,0,n,n?1:0,0,0,n};return fields[field];
}
static u64 g_old(unsigned i)
{
    u64 v[35]={0};v[0]=20000;v[1]=18000;v[3]=2000;v[4]=10;v[6]=1;
    v[7]=v[8]=4096;v[9]=1;v[11]=20000;
    v[14]=v[15]=v[16]=v[17]=4096;
    v[20]=v[21]=v[22]=v[23]=g_A_window();return v[i];
}
static void writeq(u64 value,void *p)
{
    int lane;unsigned off;struct model *h=model_at(p,&lane,&off);g_refresh();trace(lane,off,value,1);
    if(off==IDMA_OBS_CONTROL_OFFSET){
        assert(lock_depth);unsigned action=value&7;
        if(action==4){g_clears++;h->regs[off/8]=0;}
        else if(action==1){g_arms++;g_armed=true;g_started_measurement=true;g_has_a=!!(value&BIT(24));
            assert(hw[0].regs[0x90/8]==4096&&hw[0].regs[0x88/8]==1&&!hw[0].regs[0xe0/8]);
            assert(g_software_issues[0]==256&&(!g_has_a||g_software_issues[1]==256));
            if(g_has_a)assert(hw[1].regs[0x90/8]==4096&&hw[1].regs[0x88/8]==1);
            g_latched_gap=g_has_a?hw[1].regs[0xe0/8]:0;h->regs[off/8]=arm_failure?0:1;
        }else if(action==2){g_abort_writes++;g_abort_event=++g_event;
            if(g_armed&&(hw[0].pending||(g_has_a&&hw[1].pending)))g_aborted=true;
            h->regs[off/8]=4;
        }else assert(0);
        return;
    }
    if(off==IDMA_BATCH_CONTROL_OFFSET){
        if(value==0){assert(!h->pending);h->regs[off/8]=0;h->regs[0xa0/8]=h->regs[0xa8/8]=0;h->regs[0xe0/8]=0;}
        else if(value==1)h->regs[off/8]=1;
        else if(value==2){stop_writes++;g_stop_event=++g_event;assert(g_abort_event&&g_abort_event<g_stop_event);
            stop_seen[lane]=true;if(h->pending&&drain_failure!=lane){h->pending=false;h->regs[0x30/8]=h->next_id;
                h->regs[off/8]=5;h->regs[0xa0/8]=h->regs[0xa8/8]=h->completed;}}
        else assert(0);
    }else h->regs[off/8]=value;
}
static u64 readq(void *p)
{
    int lane;unsigned off;struct model *h=model_at(p,&lane,&off);g_refresh();trace(lane,off,h->regs[off/8],0);
    if(off==IDMA_OBS_CONTROL_OFFSET)return h->regs[off/8]|(g_armed&&h->regs[off/8]==4?g_status_extra:0);
    if(off==IDMA_OBS_DATA_OFFSET){unsigned index=h->regs[IDMA_OBS_INDEX_OFFSET/8];
        if(index==192)return hist_info;
        if(index>=768&&index<800){if(g_armed)g_word_reads[index-768]++;return g_word(index-768);}
        if(index>=256&&index<768){if(g_armed)g_hist_reads++;unsigned k=index-256,stage=k/32,bin=k%32;
            return (bin==1?g_path(stage,1):0)+(g_bad_hist_bin==(int)k);}
        if(index>=64&&index<192)return g_path((index-64)/8,(index-64)%8);
        assert(index<35);return g_old(index);
    }
    if(off==IDMA_GAP_CYCLES_OFFSET&&lane==1&&g_gap_bad&&h->regs[0x88/8]&1)return h->regs[off/8]+1;
    if(off==IDMA_BATCH_TARGET_OFFSET&&lane==1&&g_target_bad&&h->regs[0x88/8]&1)return h->regs[off/8]-1;
    if(off==IDMA_NEXT_ID_REG_OFFSET){
        h->next_id++;bool batch=h->regs[0x88/8]&1;u64 count=batch?h->regs[0x90/8]:1;
        if(!batch)g_software_issues[lane]++;
        else {g_started[lane]=true;if(lane==0)g_V_launch_event=++g_event;}
        if(batch&&g_early_lane==lane)count--;
        copy_tasks(lane,count);
        if(batch){h->pending=true;h->regs[0x88/8]=3;
            g_finish_at[lane]=clock_ns+(lane==0?40000:g_scenario==0?1000:g_scenario==1?20000:80000);
            if(signal_on_batch==lane)signal_flag=true;
        }else h->regs[0x30/8]=h->next_id;
        if(zero_id==lane||(batch&&batch_zero_id==lane))return 0;
        return h->next_id;
    }
    return h->regs[off/8];
}
static int g_nofault(u64 *value,const void *address)
{
    int lane;unsigned off;model_at((void*)address,&lane,&off);assert(off==0xe8&&lock_depth);
    if(lane==g_nofault_lane)return -EFAULT;
    *value=readq((void*)address);return 0;
}
static void dev_info(struct device *d,const char *format,...)
{
    (void)d;va_list ap;va_start(ap,format);
    if(strstr(format,"idma_batch_obs seq=%llu part=time")){(void)va_arg(ap,u64);g_global_valid=va_arg(ap,int);}
    if(strstr(format,"idma_path_obs seq=")||strstr(format,"idma_path_hist seq=")){
        (void)va_arg(ap,u64);for(unsigned i=0;i<3;i++)(void)va_arg(ap,int);
        if(!va_arg(ap,int)){if(strstr(format,"idma_path_hist"))g_hist_invalid=true;else g_path_invalid=true;}}
    if(strstr(format,"idma_g_word seq=")){(void)va_arg(ap,u64);unsigned index=va_arg(ap,unsigned);u64 value=va_arg(ap,u64);
        assert(index>=768&&index<800);g_raw_logged[index-768]=value;g_raw_log_count++;}
    va_end(ap);
}
