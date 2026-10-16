#!/usr/bin/env python3
"""Prepare one passive observation. Does not build or run a model."""
import hashlib
import json
from pathlib import Path

PLATFORM = Path('/home/xvhaocheng/work/iommu-platform')
OUT = PLATFORM / 'out/aw-natural-entry-observation-20261014'
OLD = PLATFORM / 'out/pte-safe-stage-causality-20261014/model'
MODEL = OUT / 'model'

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def save(path, value):
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n')

fields = []
def tap(name, width, expression):
    assert width <= 64 and name not in [x['name'] for x in fields]
    fields.append({'name': name, 'width': width, 'expression': expression})

def header(prefix, expression, extended=False, id_width=4, aw=False):
    members = [('id',id_width),('addr',64),('len',8),('size',3),('burst',2),
               ('lock',1),('cache',4),('prot',3),('qos',4),('region',4),('user',1)]
    if aw:
        members.append(('atop',6))
    if extended:
        members += [('stream_id',24),('ss_id_valid',1),('substream_id',20),
                    ('mmu_atst',1),('mmu_valid',1)]
    for member,width in members:
        tap(prefix+'_'+member,width,expression+'.'+member)

def bus(prefix, request, response, extended=False, id_width=4):
    for channel in ('aw','ar'):
        tap(prefix+'_'+channel+'_valid',1,request+'.'+channel+'_valid')
        tap(prefix+'_'+channel+'_ready',1,response+'.'+channel+'_ready')
        header(prefix+'_'+channel,request+'.'+channel,extended,id_width,channel=='aw')
    tap(prefix+'_w_valid',1,request+'.w_valid')
    tap(prefix+'_w_ready',1,response+'.w_ready')
    for member,width in [('data',64),('strb',8),('last',1),('user',1)]:
        tap(prefix+'_w_'+member,width,request+'.w.'+member)
    tap(prefix+'_r_valid',1,response+'.r_valid')
    tap(prefix+'_r_ready',1,request+'.r_ready')
    for member,width in [('id',id_width),('data',64),('resp',2),('last',1),('user',1)]:
        tap(prefix+'_r_'+member,width,response+'.r.'+member)
    tap(prefix+'_b_valid',1,response+'.b_valid')
    tap(prefix+'_b_ready',1,request+'.b_ready')
    for member,width in [('id',id_width),('resp',2),('user',1)]:
        tap(prefix+'_b_'+member,width,response+'.b.'+member)

tap('rst',1,'rst_ni')
for b in range(2):
    d = f'i_dma.i_dma{b}'
    for suffix,expression in [('front_valid','be_valid_d'),('front_ready','be_ready_d'),
                              ('back_valid','be_valid'),('back_ready','be_ready'),
                              ('done_valid','idma_rsp_valid'),('done_ready','idma_rsp_ready')]:
        tap(f'job{b}_{suffix}',1,d+'.'+expression)
    tap(f'job{b}_ttype',3,d+'.ttype')
    for stage,request in [('front','burst_req_d'),('back','burst_req')]:
        for member in ('src_addr','dst_addr','length'):
            tap(f'job{b}_{stage}_{member}',64,d+'.'+request+'.'+member)
        for member in ('decouple_aw','decouple_rw','src_reduce_len','dst_reduce_len'):
            tap(f'job{b}_{stage}_{member}',1,d+'.'+request+'.opt.beo.'+member)
    tap(f'job{b}_done_error',1,d+'.idma_rsp.error')
    tap(f'job{b}_last_meta',1,d+'.idma_rsp.last')
    bus(f'src{b}',f'i_dma.dma{b}_mst_req',f'i_dma.dma{b}_mst_rsp',True,2)

bank = 'i_dma.i_sys.gen_banked.i_banked'
bus('input','i_dma.mux_mst_req','i_dma.mux_mst_resp',True)
for b in range(2):
    tc = f'{bank}.gen_bank[{b}].i_bank'
    bus(f'target{b}',tc+'.axi_comp_req',tc+'.axi_comp_resp')
    bus(f'bankout{b}',f'{bank}.bank_comp_req[{b}]',f'{bank}.bank_comp_resp[{b}]')
    tap(f'bank{b}_trans_valid',1,f'{bank}.bank_trans_req_valid[{b}]')
    tap(f'bank{b}_trans_ready',1,f'{bank}.bank_trans_req_ready[{b}]')
    for member,width in [('did',24),('iova',64),('ttype',6),('pid',20),
                         ('pid_valid',1),('priv',1),('is_debug',1),('bank',1)]:
        tap(f'bank{b}_req_{member}',width,f'{bank}.bank_trans_req_data[{b}].{member}')
    tap(f'bank{b}_resp_valid',1,f'{bank}.bank_trans_resp_valid[{b}]')
    tap(f'bank{b}_resp_ready',1,f'{bank}.bank_trans_resp_ready[{b}]')
    tap(f'bank{b}_resp_error',1,f'{bank}.bank_trans_resp_data[{b}].error')
    header(f'bank{b}_raw_ar',tc+'.ar_data',True)

mux = 'i_dma.i_dma_iommu_tr_mux.gen_mux'
spill = mux+'.i_aw_spill_reg.spill_register_flushable_i.gen_spill_reg'
tap('up_aw_offer',1,mux+'.mst_aw_valid')
tap('up_aw_offer_ready',1,mux+'.mst_aw_ready')
tap('up_aw_reserve',1,mux+'.w_fifo_push')
tap('up_aw_locked',1,mux+'.lock_aw_valid_q')
header('up_aw_header',mux+'.mst_aw_chan',True,4,True)
for letter in ('a','b'):
    tap('up_spill_'+letter+'_full',1,spill+'.'+letter+'_full_q')
    header('up_spill_'+letter,spill+'.'+letter+'_data_q',True,4,True)

omux = bank+'.i_out_axi_mux.gen_mux'
tap('out_aw_reserve',1,omux+'.w_fifo_push')
tap('out_aw_locked',1,omux+'.lock_aw_valid_q')
header('out_reservation',omux+'.mst_aw_chan',False,4,True)
tap('out_w_owner_valid',1,'!'+omux+'.w_fifo_empty')
tap('out_w_owner_bank',1,omux+'.w_fifo_data[0]')
tap('raw_w_owner_valid',1,'!'+mux+'.w_fifo_empty')
tap('raw_w_owner_source',2,mux+'.w_fifo_data')

tl = 'i_dma.i_sys.i_tl'
for member,width in [('did',24),('iova',64),('ttype',6),('pid',20),
                     ('pid_valid',1),('priv',1),('is_debug',1),('bank',1)]:
    tap('main_'+member,width,tl+'.req_data.'+member)
for name in ('req_valid','slow_resp_valid','slow_resp_ready','en_1S_q','en_2S_q',
             'e3_ptw_active','n_window','n_accept','n_intercept','n_busy',
             'fast_resp_valid','fast_resp_ready','fault_valid_o','policy_quiet_o',
             'cache_ddtc_hit','cache_iotlb_hit'):
    tap('tl_'+name,1,tl+'.'+name)
tap('tl_fast_error',1,tl+'.fast_resp_data.error')
tap('tl_fast_bank',1,tl+'.fast_resp_data.bank')
tap('invalidate_vma',1,tl+'.iotinval_i.inval_vma')
tap('invalidate_gvma',1,tl+'.iotinval_i.inval_gvma')
tap('invalidate_ddtc',1,tl+'.iodirinval_i.inval_ddtc')
tap('invalidate_pdtc',1,tl+'.iodirinval_i.inval_pdtc')

save(MODEL/'observer-fields.json',{'sampling':'pre-rising-edge; each row handshakes at that rising edge',
      'fields':fields,'added_read_only_tap_bits':sum(x['width'] for x in fields),
      'core_storage_added':0,'note':'Simulation taps, not a free implementable hardware interface.'})

original_top=(OLD/'safe_pte_stage_top.sv').read_text()
original_cpp=(OLD/'safe_pte_stage_probe.cpp').read_text()
top=original_top.replace('module safe_pte_stage_top','module aw_natural_entry_top',1)
ports=''.join('    output logic '+(f"[{x['width']-1}:0] " if x['width']>1 else '')+'obs_'+x['name']+',\n' for x in fields)
top=top.replace('    output logic [1:0] probe_pair,',ports+'    output logic [1:0] probe_pair,',1)
assigns='\n    // Passive observation: these outputs never feed a DUT input.\n'+''.join('    assign obs_'+x['name']+' = '+x['expression']+';\n' for x in fields)
top=top.replace('endmodule',assigns+'endmodule',1)
cpp=original_cpp.replace('Vsafe_pte_stage_top','Vaw_natural_entry_top')
emit=''.join('        std::fprintf(probe_file,",%llu",(unsigned long long)(top->obs_'+x['name']+'));\n' for x in fields)
needle="        std::fputc('\\n',probe_file);"
assert cpp.count(needle)==1
cpp=cpp.replace(needle,emit+needle,1)
old_header='memq,prior_mheld\\n'
new_header='memq,prior_mheld,'+','.join(x['name'] for x in fields)+'\\n'
assert cpp.count(old_header)==1
cpp=cpp.replace(old_header,new_header,1)
# Fixed diagnostic upper bound; no new traffic and no graceful traffic cancellation.
cap='        CHECK(!probe_record || probe_samples++ < 20000, "observation cap20000 reached; record bounded incomplete result");\n'
old_tick='    void tick() {\n'
new_tick='    uint64_t probe_samples = 0;\n\n'+old_tick+cap
cpp=cpp.replace(old_tick,new_tick,1)
(MODEL/'aw_natural_entry_top.sv').write_text(top)
(MODEL/'aw_natural_entry_probe.cpp').write_text(cpp)

# Exact reverse edit proof, before any compiler/model execution.
recovered_top=top.replace(assigns,'',1).replace(ports,'',1).replace('module aw_natural_entry_top','module safe_pte_stage_top',1)
recovered_cpp=cpp.replace(emit,'',1).replace(new_header,old_header,1).replace(new_tick,old_tick,1).replace('Vaw_natural_entry_top','Vsafe_pte_stage_top')
assert recovered_top==original_top and recovered_cpp==original_cpp

prior=json.loads((OLD/'build-manifest.json').read_text())
old_sources={x['path']:x['sha256'] for x in prior['sources']}
argv=prior['argv'][:]
for i,a in enumerate(argv):
    if a=='safe_pte_stage_top': argv[i]='aw_natural_entry_top'
    if a==str(OLD/'obj'): argv[i]=str(MODEL/'obj')
    if a==str(OLD/'safe_pte_stage_top.sv'): argv[i]=str(MODEL/'aw_natural_entry_top.sv')
    if a==str(OLD/'safe_pte_stage_probe.cpp'): argv[i]=str(MODEL/'aw_natural_entry_probe.cpp')
sources=[]
for a in argv:
    if a.endswith(('.sv','.cpp')):
        p=Path(a)
        assert p.is_file(),f'missing dependency:{p}'
        sha=digest(p)
        unchanged=a in old_sources
        if unchanged:
            assert sha==old_sources[a],f'original dependency changed:{p}'
        sources.append({'path':a,'sha256':sha,'bytes':p.stat().st_size,'original_dependency':unchanged})
save(MODEL/'build-manifest.json',{'schema':'aw-natural-observation-build-v1','argv':argv,'sources':sources,
     'old_build_will_not_run':True,'production_sources_edited':False})
save(OUT/'root/probe-admission.json',{
    'admitted':True,'author':'root actual Sol6.1/ultra, direct source check',
    'goal_createdAt':1791965564,'original_source_entry':str(OLD/'safe_pte_stage_probe.cpp')+':3232',
    'supply_semantics_byte_equivalent_after_removing_recording_edits':True,
    'top_semantics_byte_equivalent_after_removing_output_taps':True,
    'all_original_build_dependency_hashes_match':True,
    'model_only':True,'MIG_or_board':False,
    'production_vendor_vs_runner':'Runner retains original mixed but unique vendor selection. All build SHA retained; do not transplant board-only capacity claims.',
    'actual_condition':'Two real DEFAULT sole4096B jobs, accepted configuration must be recorded; native 4-input mux with AW/ARspill, N on; original serial source0/source1 triggers; no warmup/batch/CPU; original DSpriority head-only DDR law31 and queue16; delays50/50.',
    'runs_max':1,'phase_retries':0,'cap_cycles':20000,
    'output_cost':'Pure combinational sim taps; no selectable hardware feedback implementation or synthesized area/Fmax claim.',
    'reverse_edit_proofs':{'top':recovered_top==original_top,'supply_and_DDR_cpp':recovered_cpp==original_cpp}})
print(json.dumps({'prepared':True,'tap_fields':len(fields),'tap_bits':sum(x['width'] for x in fields),
      'sources':len(sources),'probe_admitted':True,'runs_so_far':0}))
