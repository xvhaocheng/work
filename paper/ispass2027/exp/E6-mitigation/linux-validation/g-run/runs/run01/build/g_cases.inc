static unsigned g_checks;
#define G_CHECK(x) do {g_checks++;if(!(x)){fprintf(stderr,"G check line%u: %s\n",__LINE__,#x);assert(x);}} while(0)
static void g_reset(unsigned scenario)
{
    reset(IDMA_G_BATCH_MAGIC);idma_path_observe=true;idma_observation_sequence=0;
    g_scenario=scenario;g_event=g_hpm_reads=g_hpm_after_event=0;
    g_A_done_event=g_V_launch_event=g_abort_event=g_stop_event=0;
    g_abort_writes=g_arms=g_clears=g_hist_reads=g_raw_log_count=0;
    memset(g_software_issues,0,sizeof(g_software_issues));memset(g_word_reads,0,sizeof(g_word_reads));
    memset(g_raw_logged,0,sizeof(g_raw_logged));memset(g_started,0,sizeof(g_started));
    memset(g_finish_at,0,sizeof(g_finish_at));
    g_hpm_A_pending=g_armed=g_has_a=g_aborted=g_started_measurement=false;
    g_global_valid=g_path_invalid=g_hist_invalid=g_gap_bad=g_target_bad=false;
    g_latched_gap=g_hw_error=g_status_extra=g_corrupt_value=0;
    g_nofault_lane=g_early_lane=g_inventory_lane=g_corrupt_word=g_bad_hist_bin=-1;
    hist_info=0x20202001;
    hw[0].regs[0x58/8]=IDMA_G_OBSERVER_MAGIC;
    for(unsigned i=0;i<2;i++)hw[i].regs[0xe8/8]=IDMA_G_GAP_MAGIC;
}
static struct idma_g_experiment g_request(bool has_a,unsigned gap)
{
    return (struct idma_g_experiment){.abi_version=6,.struct_size=sizeof(struct idma_g_experiment),
        .aggressor_id=has_a?1:IDMA_EXPERIMENT_NO_AGGRESSOR,.victim_pages=1,.aggressor_pages=has_a?8:0,
        .chunk_bytes=4096,.run_timeout_ms=1000,.victim_transfers=4096,.aggressor_transfers=has_a?4096:0,
        .warmup_transfers=256,.aggressor_gap_cycles=gap};
}
static void g_assert_logs(const struct idma_g_experiment *p)
{
    G_CHECK(g_raw_log_count==32&&g_hist_reads==512);
    for(unsigned i=0;i<32;i++)G_CHECK(g_word_reads[i]==1&&g_raw_logged[i]==p->words[i]);
}
static void g_positive(void)
{
    for(unsigned scenario=0;scenario<6;scenario++)for(unsigned gap=0;gap<=2000;gap+=1000){
        /* With nonzero gaps the synthetic whole interval must extend past V. */
        if(gap&&scenario<2)continue;
        g_reset(scenario);struct idma_g_experiment p=g_request(true,gap);
        G_CHECK(idma_run_g(&idmas[0],&p)==0&&p.status==0);
        G_CHECK(p.victim.completed==4096&&p.aggressor.completed==4096);
        G_CHECK(p.victim.completed_bytes==16777216&&p.aggressor.completed_bytes==16777216);
        G_CHECK(p.warmup_completed[0]==256&&p.warmup_completed[1]==256);
        G_CHECK(p.warmup_bytes[0]==1048576&&p.warmup_bytes[1]==1048576);
        G_CHECK(g_software_issues[0]==256&&g_software_issues[1]==256);
        G_CHECK(p.prepared_lane_mask==3&&p.quiet_lane_mask==3&&p.verified_lane_mask==3&&!p.retained_lane_mask);
        G_CHECK(p.measurement_started&&p.measurement_complete&&p.observer_sequence==1);
        G_CHECK(p.old_observer_status==4&&!p.observer_validation_errors&&!p.words[3]);
        G_CHECK(g_global_valid&&!g_path_invalid&&!g_hist_invalid);
        G_CHECK(g_hpm_reads==2&&g_arms==1&&g_clears==1&&g_abort_writes==0&&stop_writes==0);
        G_CHECK(g_hpm_after_event&&p.whole_software_elapsed_ns>=p.victim_elapsed_ns);
        if(scenario==0)G_CHECK(g_A_done_event<g_V_launch_event);
        if(scenario>=2)G_CHECK(g_hpm_A_pending&&g_A_done_event>g_hpm_after_event);
        G_CHECK(allocs==frees&&contexts==context_frees&&!lock_depth);
        g_assert_logs(&p);
    }
    g_reset(2);struct idma_g_experiment p=g_request(false,0);
    G_CHECK(idma_run_g(&idmas[0],&p)==0&&p.status==0&&p.measurement_complete);
    G_CHECK(p.prepared_lane_mask==1&&p.quiet_lane_mask==1&&p.verified_lane_mask==1);
    G_CHECK(p.aggressor.completed==0&&p.warmup_completed[1]==0&&p.words[5]==0&&p.words[13]==0);
    G_CHECK(!stop_writes&&!g_abort_writes&&allocs==frees&&g_global_valid);
    g_assert_logs(&p);
}
static void g_negative(void)
{
    struct idma_g_experiment p;
    /* Every hardware error bit must invalidate old batch/PATH logs too. */
    for(unsigned bit=0;bit<10;bit++){
        g_reset(2);g_hw_error=1ULL<<bit;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&p.status==-EIO);
        G_CHECK(p.observer_validation_errors&IDMA_G_HARDWARE_ERROR);
        G_CHECK(!g_global_valid&&g_path_invalid&&p.words[3]==(1ULL<<bit));
        G_CHECK(p.old_observer_status==4&&allocs==frees);g_assert_logs(&p);
    }
    const unsigned corrupt[]={1,2,4,5,7,8,9,10,11,12,13,15,16,17,19,20,21,23,24,25,26,28,30,31};
    for(unsigned j=0;j<sizeof(corrupt)/sizeof(corrupt[0]);j++){
        g_reset(3);g_corrupt_word=corrupt[j];g_corrupt_value=~0ULL;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&p.observer_validation_errors);
        G_CHECK(!g_global_valid&&g_path_invalid&&p.words[corrupt[j]]==~0ULL);
        G_CHECK(!p.words[3]&&p.old_observer_status==4&&allocs==frees);
    }
    for(int lane=0;lane<2;lane++){
        g_reset(2);g_nofault_lane=lane;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-EOPNOTSUPP&&allocs==0&&g_arms==0&&g_clears==0);
        g_reset(2);g_early_lane=lane;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&!g_global_valid&&g_path_invalid);
        G_CHECK(p.quiet_lane_mask==3&&!p.retained_lane_mask&&allocs==frees);
        g_reset(2);g_inventory_lane=lane;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&p.retained_lane_mask==(1U<<lane));
        G_CHECK(idmas[lane].quarantined&&allocs>frees&&!g_global_valid);
        g_reset(2);batch_zero_id=lane;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&idmas[lane].quarantined&&allocs>frees);
        g_reset(2);drain_failure=lane;p=g_request(true,0);p.run_timeout_ms=10;
        G_CHECK(idma_run_g(&idmas[0],&p)==-ETIMEDOUT&&idmas[lane].quarantined&&allocs>frees);
        G_CHECK(g_abort_writes==1&&stop_writes&&g_abort_event<g_stop_event);
        g_reset(2);signal_on_batch=lane;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-EINTR&&g_abort_writes==1&&stop_writes);
        G_CHECK(g_abort_event<g_stop_event&&!p.measurement_complete);
        g_reset(2);verify_failure=lane;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-EILSEQ&&!g_global_valid&&g_path_invalid);
        G_CHECK((lane?p.aggressor.data_errors:p.victim.data_errors)==1&&allocs==frees);
        G_CHECK(!p.words[3]); /* Software data fault must not fabricate a HW error. */
    }
    g_reset(2);idma_path_observe=false;p=g_request(true,0);
    G_CHECK(idma_run_g(&idmas[0],&p)==-EOPNOTSUPP&&allocs==0&&!g_arms);
    g_reset(2);g_gap_bad=true;p=g_request(true,1000);
    G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&!g_arms&&p.warmup_completed[0]==256&&p.warmup_completed[1]==256);
    g_reset(2);g_target_bad=true;p=g_request(true,0);
    G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&!g_arms);
    for(int failure=0;failure<4;failure++){
        g_reset(2);alloc_failure=failure;p=g_request(true,0);
        G_CHECK(idma_run_g(&idmas[0],&p)==-ENOMEM&&allocs==frees&&!g_arms&&!lock_depth);
    }
    g_reset(2);hpm_failure=-EBUSY;p=g_request(true,0);
    G_CHECK(idma_run_g(&idmas[0],&p)==-EBUSY&&allocs==frees&&!g_arms);
    g_reset(2);g_bad_hist_bin=3;p=g_request(true,0);
    G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&!g_global_valid&&g_hist_invalid&&!p.observer_validation_errors);
    g_reset(2);g_status_extra=1ULL<<40;p=g_request(true,0);
    G_CHECK(idma_run_g(&idmas[0],&p)==-EIO&&p.old_observer_status==(4|(1ULL<<40)));
    g_reset(2);from_failure=1;p=g_request(true,0);G_CHECK(idma_run_g(&idmas[0],&p)==-EFAULT&&allocs==0);
    g_reset(2);to_failure=1;p=g_request(true,0);G_CHECK(idma_run_g(&idmas[0],&p)==-EFAULT&&allocs==frees);
    g_reset(2);context_failure=1;p=g_request(true,0);G_CHECK(idma_run_g(&idmas[0],&p)==-ENOMEM&&allocs==0);
    g_reset(2);p=g_request(true,0);p.reserved[7]=1;G_CHECK(idma_run_g(&idmas[0],&p)==-EINVAL&&allocs==0);
}
int main(void)
{
    g_positive();g_negative();g_reset(2);
    printf("PASS %u G actual-function checks; fixture_only, synthetic OBS/PATH/HIST, real host data copies; no board evidence\n",g_checks);
    return 0;
}
