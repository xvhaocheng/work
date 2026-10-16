    // Raw pre-edge snapshots: direct actual CPU/cache/controller and DDR ports.
    FILE *cpu_trace=nullptr,*control_trace=nullptr,*cache_axi_trace=nullptr,*memory_axi_trace=nullptr;
    std::string cpu_phase="idle";unsigned cpu_packet=0;uint64_t cpu_op_seq=0;
    bool cache_gnt_seen=false,cache_rvalid_seen=false,cache_ack_seen=false;
    uint64_t cache_rdata_seen=0;
    uint64_t cpu_grants=0,cpu_load_responses=0,cache_read_requests=0,cache_read_beats=0,cache_write_requests=0,cache_write_beats=0,cache_write_responses=0;
    uint64_t cache_misses=0,icache_flush_cycles=0,dcache_flush_cycles=0,controller_flushes=0;
    void cache_open(const std::string& out) {
      cpu_trace=std::fopen((out+"/cpu_ports.csv").c_str(),"w");control_trace=std::fopen((out+"/cache_control.csv").c_str(),"w");
      cache_axi_trace=std::fopen((out+"/cache_axi.csv").c_str(),"w");memory_axi_trace=std::fopen((out+"/memory_axi.csv").c_str(),"w");
      CHECK(cpu_trace&&control_trace&&cache_axi_trace&&memory_axi_trace,"raw cache traces open");
      std::fprintf(cpu_trace,"cycle,phase,packet,op_seq,port,req,gnt,rvalid,we,address,be,size,wdata,rdata,tag_valid,kill,rst\n");
      std::fprintf(control_trace,"cycle,phase,packet,fence_i,flush_i,flush_d,ack,halt,wb_empty,mshr,mshr_q1,dcache_miss,icache_miss,i_flush_en,d_flush_en,i_flush_idx,d_flush_idx,rst\n");
      for(FILE* f:{cache_axi_trace,memory_axi_trace})std::fprintf(f,"cycle,phase,packet,channel,valid,ready,id,address,len,size,burst,data,strb,last,resp,rst\n");
    }
    void cache_close(){for(FILE** f:{&cpu_trace,&control_trace,&cache_axi_trace,&memory_axi_trace})if(*f){std::fclose(*f);*f=nullptr;}}
    void cache_axi_row(FILE* f,const char* channel,bool valid,bool ready,uint64_t id,uint64_t address,uint64_t len,uint64_t size,uint64_t burst,uint64_t data,uint64_t strb,bool last,uint64_t resp){
      if(f)std::fprintf(f,"%ld,%s,%u,%s,%u,%u,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%u,%llu,%u\n",g_cycle,cpu_phase.c_str(),cpu_packet,channel,valid,ready,(unsigned long long)id,(unsigned long long)address,(unsigned long long)len,(unsigned long long)size,(unsigned long long)burst,(unsigned long long)data,(unsigned long long)strb,last,(unsigned long long)resp,unsigned(top->rst_ni));
    }
    void cache_observe(){
      cache_gnt_seen=top->c_gnt;cache_rvalid_seen=top->c_rvalid;cache_rdata_seen=top->c_rdata;cache_ack_seen=top->c_flush_ack;
      if(!cpu_trace)return;
      std::fprintf(cpu_trace,"%ld,%s,%u,%llu,%u,%u,%u,%u,%u,%llu,%u,%u,%llu,%llu,%u,%u,%u\n",g_cycle,cpu_phase.c_str(),cpu_packet,(unsigned long long)cpu_op_seq,top->c_we?2:1,unsigned(top->c_req),unsigned(top->c_gnt),unsigned(top->c_rvalid),unsigned(top->c_we),(unsigned long long)top->c_addr,unsigned(top->c_be),unsigned(top->c_size),(unsigned long long)top->c_wdata,(unsigned long long)top->c_rdata,unsigned(top->c_tag_valid),unsigned(top->c_kill),unsigned(top->rst_ni));
      std::fprintf(control_trace,"%ld,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",g_cycle,cpu_phase.c_str(),cpu_packet,unsigned(top->c_fence),unsigned(top->c_flush_i),unsigned(top->c_flush_d),unsigned(top->c_flush_ack),unsigned(top->c_halt),unsigned(top->c_wb_empty),unsigned(top->c_mshr),unsigned(top->c_mshr1),unsigned(top->c_dmiss),unsigned(top->c_imiss),unsigned(top->c_i_flush_en),unsigned(top->c_d_flush_en),unsigned(top->c_i_flush_idx),unsigned(top->c_d_flush_idx),unsigned(top->rst_ni));
      if(top->rst_ni){cpu_grants+=top->c_req&&top->c_gnt;cpu_load_responses+=top->c_rvalid;cache_read_requests+=top->c_ar_valid&&top->c_ar_ready;cache_read_beats+=top->c_r_valid&&top->c_r_ready;cache_write_requests+=top->c_aw_valid&&top->c_aw_ready;cache_write_beats+=top->c_w_valid&&top->c_w_ready;cache_write_responses+=top->c_b_valid&&top->c_b_ready;cache_misses+=top->c_dmiss;icache_flush_cycles+=top->c_i_flush_en;dcache_flush_cycles+=top->c_d_flush_en;controller_flushes+=top->c_fence;}
#define CACHE_AXI_ROWS(F,P) \
      cache_axi_row(F,"AR",top->P##ar_valid,top->P##ar_ready,top->P##ar_id,top->P##ar_addr,top->P##ar_len,top->P##ar_size,top->P##ar_burst,0,0,0,0); \
      cache_axi_row(F,"AW",top->P##aw_valid,top->P##aw_ready,top->P##aw_id,top->P##aw_addr,top->P##aw_len,top->P##aw_size,top->P##aw_burst,0,0,0,0); \
      cache_axi_row(F,"W",top->P##w_valid,top->P##w_ready,0,0,0,0,0,top->P##w_data,top->P##w_strb,top->P##w_last,0); \
      cache_axi_row(F,"R",top->P##r_valid,top->P##r_ready,top->P##r_id,0,0,0,0,top->P##r_data,0,top->P##r_last,top->P##r_resp); \
      cache_axi_row(F,"B",top->P##b_valid,top->P##b_ready,top->P##b_id,0,0,0,0,0,0,0,top->P##b_resp)
      CACHE_AXI_ROWS(cache_axi_trace,c_);
      CACHE_AXI_ROWS(memory_axi_trace,ddr_);
#undef CACHE_AXI_ROWS
    }
