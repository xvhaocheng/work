#!/usr/bin/env python3
"""Ten IEEE single-column figures from immutable trial-level/summary sources."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
from pathlib import Path
from statistics import median

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

from data_adapter import (as_number, condition_fields, read_csv, require_one_base,
                          sample_values, stable_bytes, historical_base)
from statistics_common import SEED, N_RESAMPLES, relative_slowdown, summarize

ANALYSIS = Path(__file__).resolve().parents[1]
PAPER = ANALYSIS.parent
MODES = ['Solo','A1','A8','A16','A64']
COLORS = ['#0077BB','#009988','#EE7733','#CC3311','#000000']
MARKERS = ['o','s','^','D','x']
NAMES = ['fig01-throughput','fig02-path-miss','fig03-ds-sweep',
         'fig04-device-count','fig05-workload-size','fig06-page-size',
         'fig07-latency-cdf','fig08-mitigation-history','fig09-capacity',
         'fig10-invalidation']
matplotlib.rcParams.update({
    'font.family':'DejaVu Sans','font.size':8,'axes.titlesize':8,
    'axes.labelsize':8,'xtick.labelsize':8,'ytick.labelsize':8,
    'legend.fontsize':8,'figure.dpi':160,'savefig.dpi':300,
    'axes.spines.top':False,'axes.spines.right':False,
    'pdf.fonttype':42,'ps.fonttype':42,'svg.fonttype':'none',
    'axes.formatter.use_mathtext':False,
})
TRACE = []
STATS = []


def select(rows, metric=None, experiment=None, statistic=None, **conditions):
    out=[]
    for row in rows:
        if metric is not None and row['metric'] != metric: continue
        if experiment is not None and row['experiment'] != experiment: continue
        if statistic is not None and row['statistic'] != statistic: continue
        fields=condition_fields(row)
        if any(fields.get(k) != str(v) for k,v in conditions.items()): continue
        if as_number(row) is not None: out.append(row)
    return out


def baseline(rows, ds, mode, metric):
    return select(rows,metric,'board_baseline100','sample',ds=f'{ds}/{ds}',mode=mode)


def trial_summary(rows, label):
    values=sample_values(rows,context=label)
    summary=summarize(values,strata=[r['boot_id'] for r in rows])
    STATS.append(dict(label=label,base_id=require_one_base(rows,context=label),
                      row_ids=[r['row_id'] for r in rows],**summary))
    return summary


def err(summary):
    value=summary['median']
    low,high=summary['ci_low'],summary['ci_high']
    return [[max(0,value-low)],[max(0,high-value)]] if low is not None else None


def panel_title(ax,text):
    ax.set_title(text,loc='left',pad=5)


def save(fig,index,*,status,question,rows,caption,limitations):
    name=NAMES[index-1]
    fig.set_size_inches(3.5,fig.get_figheight())
    fig.tight_layout(pad=.7,h_pad=1.2)
    out=ANALYSIS/'figures'
    out.mkdir(exist_ok=True)
    # Deliberately no bbox_inches=tight: physical PDF width stays exactly 3.5in.
    for ext in ('pdf','png'):
        fig.savefig(out/f'{name}.{ext}',metadata=({'Creator':'ISPASS reproducible analysis','CreationDate':None,'ModDate':None} if ext=='pdf' else {}))
    plt.close(fig)
    TRACE.append(dict(figure=index,artifact_id=name,status=status,question=question,
                      source_data=dict(dataset_id='ispass2027-evidence',file='merged-board-results.csv',
                                       row_ids=[r['row_id'] for r in rows]),
                      base_ids=sorted({r.get('base_id','unknown') for r in rows}),
                      transformation=dict(script='scripts/plot_figures.py',
                                           hash=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()),
                      caption_claim=caption,supported_manuscript_claims=[],limitations=limitations))


def missing(ax,text,xlabel,ylabel,xticks=None):
    ax.set_xlabel(xlabel); ax.set_ylabel(ylabel)
    ax.set_ylim(0,1); ax.set_yticks([])
    if xticks is None: ax.set_xticks([])
    else: ax.set_xticks(range(len(xticks)),xticks)
    ax.text(.5,.52,text,ha='center',va='center',transform=ax.transAxes,
            bbox=dict(facecolor='white',edgecolor='#777777',pad=5),wrap=True)
    ax.text(.5,.1,'NO MEASURED DATA',ha='center',transform=ax.transAxes,color='#555555')


def figure1(rows):
    fig,axs=plt.subplots(2,1,figsize=(3.5,4.4))
    used=[]; summaries={}
    for i,mode in enumerate(MODES):
        rr=baseline(rows,0,mode,'hardware_MiB_s'); used+=rr
        s=trial_summary(rr,f'fig1.DS0.{mode}.V_throughput')
        axs[0].errorbar(i,s['median'],yerr=err(s),fmt=MARKERS[i],color=COLORS[i],capsize=3)
        rr=baseline(rows,0,mode,'hardware_ms');used+=rr
        summaries[mode]=trial_summary(rr,f'fig1.DS0.{mode}.hardware_ms')
    axs[0].set_xticks(range(5),MODES);axs[0].set_ylim(bottom=0)
    axs[0].set_ylabel('V throughput (MiB/s)');axs[0].set_xlabel('Co-run condition')
    panel_title(axs[0],'(a) DS0 direct wiring; n=10/condition')
    solo=summaries['Solo']['median'];a1=summaries['A1']['median']
    common=[0]+[100*(a1/solo-1)]*4
    additional=[0,0]+[100*(summaries[m]['median']-a1)/solo for m in MODES[2:]]
    axs[1].bar(range(5),common,color='#BBBBBB',edgecolor='black',hatch='///',label='A1 co-run increment')
    axs[1].bar(range(5),additional,bottom=common,color=COLORS[0],edgecolor='black',hatch='...',label='Additional vs A1')
    axs[1].set_xticks(range(5),MODES);axs[1].set_ylabel('Elapsed increment / Solo (%)')
    axs[1].set_xlabel('Co-run condition');axs[1].set_ylim(0,1.5*max(np.asarray(common)+np.asarray(additional)));axs[1].legend(loc='upper left')
    panel_title(axs[1],'(b) Equal 16 MiB V work; medians')
    save(fig,1,status='measured',question='How much throughput is lost without and with translation pressure?',rows=used,
         caption='DS0 V throughput (95% bootstrap median CI) and elapsed increment relative to Solo. The stacked split uses A1 as a measured co-run reference; it is not a pure PTW decomposition.',
         limitations=['Single observed DS0 boot; CI conditional on observed trials.','A1 has zero median HPM misses in software brackets; this does not measure every causal cost.','Additional elapsed is not uniquely attributable to translation.'])


def figure2(rows):
    fig,axs=plt.subplots(3,1,figsize=(3.5,6.2));used=[]
    for j,direction in enumerate(('r','w')):
        for i,mode in enumerate(('A1','A8','A64')):
            yy=[];lo=[];hi=[]
            for stage in range(4):
                rr=baseline(rows,0,mode,f'path_v{direction}{stage}_mean_cycles');used+=rr
                s=trial_summary(rr,f'fig2.DS0.{mode}.v{direction}{stage}')
                yy.append(s['median']);lo.append(s['median']-s['ci_low']);hi.append(s['ci_high']-s['median'])
            axs[j].errorbar(np.arange(4)+(i-1)*.16,yy,yerr=[lo,hi],color=COLORS[i],marker=MARKERS[i],linestyle='none',label=mode,capsize=2)
        axs[j].set_xticks(range(4),['entry','internal','address','return']);axs[j].set_ylim(bottom=0)
        axs[j].set_xlabel('Observed path phase');axs[j].set_ylabel('Mean latency (cycles)')
        panel_title(axs[j],f'({chr(97+j)}) DS0 V {"read" if direction=="r" else "write"}; n=10')
        if j==0:axs[j].legend(ncol=3,loc='upper left')
    for i,(metric,label) in enumerate((('victim_iotlb_miss','V HPM'),('aggressor_iotlb_miss','A HPM'))):
        xx=[];yy=[]
        for x,mode in enumerate(MODES):
            rr=baseline(rows,0,mode,metric)
            if not rr:continue
            used+=rr;s=trial_summary(rr,f'fig2.DS0.{mode}.{metric}');xx.append(x);yy.append(s['median'])
        axs[2].plot(xx,yy,marker=MARKERS[i],linestyle=['-','--'][i],color=COLORS[i],label=label)
    axs[2].set_yscale('symlog',linthresh=2);axs[2].set_ylim(bottom=0)
    axs[2].set_yticks([0,1,10,100,1000,10000],['0','1','10','100','1e3','1e4'])
    axs[2].set_xticks(range(5),MODES);axs[2].set_xlabel('Co-run condition');axs[2].set_ylabel('Median HPM misses (count)')
    axs[2].legend(ncol=2);panel_title(axs[2],'(c) Non-atomic software HPM window')
    save(fig,2,status='measured',question='How do phase latency and observed misses change?',rows=used,
         caption='DS0 per-trial sum/full phase means and HPM miss medians. Entry: DMA input assertion to IOMMU ingress handshake; internal: ingress handshake to output address assertion; address: output assertion to address acceptance; return: address acceptance to final response. HPM counts use separate software brackets.',
         limitations=['Stage means are not latency quantiles or fractions of total runtime.','Phase latencies overlap and must not be added as a causal decomposition.','DS0 A8/A16/A64 V HPM median is 2, not zero.'])


def figure3(rows):
    fig,ax=plt.subplots(figsize=(3.5,2.7));used=[];missing_groups=[]
    for i,mode in enumerate(MODES):
        xx=[];yy=[];lo=[];hi=[]
        for ds in (0,25,50,100):
            rr=baseline(rows,ds,mode,'hardware_ms')
            if not rr:
                rr=select(rows,'hardware_ms','board_ds_sweep','sample',ds=f'{ds}/{ds}',mode=mode)
            if not rr:missing_groups.append((ds,mode));continue
            xx.append(ds);used+=rr;s=trial_summary(rr,f'fig3.DS{ds}.{mode}.hardware_ms')
            yy.append(s['median']);lo.append(s['median']-s['ci_low']);hi.append(s['ci_high']-s['median'])
        series=[r for r in used if condition_fields(r).get('mode')==mode]
        require_one_base(series,context=f'fig3 DS series {mode}')
        ax.errorbar(xx,yy,yerr=[lo,hi],marker=MARKERS[i],linestyle='none',color=COLORS[i],label=mode,capsize=2)
    ax.set_xticks([0,25,50,100]);ax.set_xlim(-5,105);ax.set_ylim(bottom=0)
    ax.set_xlabel('Added diagnostic delay DS (cycles)');ax.set_ylabel('V elapsed (ms)')
    ax.legend(ncol=3);panel_title(ax,'E1b four-point DS sweep'+(' (partial)' if missing_groups else ''))
    missing_ds=sorted({ds for ds,_ in missing_groups})
    if missing_ds:ax.text(.98,.05,'/'.join(f'DS{ds}' for ds in missing_ds)+' pending',ha='right',transform=ax.transAxes)
    save(fig,3,status='partial' if missing_groups else 'measured',question='How sensitive is interference to added DS delay?',rows=used,
         caption='Frozen-baseline DS sensitivity; accepted trial groups are shown without interpolation across missing groups. DS changes multiple shared request classes and is not a pure PTW-latency sweep.',
         limitations=['DS0/50 each n=10 per condition; DS50 spans two observed boots.']+([f'Missing accepted DS x condition groups: {missing_groups}.'] if missing_groups else []))


def placeholder(index,text,xlabel,ylabel,xticks,question):
    fig,ax=plt.subplots(figsize=(3.5,2.6));missing(ax,text,xlabel,ylabel,xticks)
    save(fig,index,status='missing',question=question,rows=[],caption=text,
         limitations=['Explicit empty figure scaffold; contains no measured values.'])


def native_trial_figure(rows,index,experiment,xkey,xvalues,xlabels,xlabel,question,missing_text,facet=None):
    """Narrow future-data contract; never borrows unrelated historical anchors."""
    rr=select(rows,'hardware_MiB_s',experiment,'sample')
    if not rr:
        return placeholder(index,missing_text,xlabel,'V throughput (MiB/s)',xlabels,question)
    require_one_base(rr,context=f'figure {index} native experiment')
    facets=sorted({condition_fields(r).get(facet,'unknown') for r in rr}) if facet else [None]
    fig,axs=plt.subplots(len(facets),1,figsize=(3.5,2.6*len(facets)),squeeze=False)
    used=[];complete=True
    for k,fac in enumerate(facets):
        ax=axs[k,0]
        for i,mode in enumerate(MODES):
            group=[r for r in rr if condition_fields(r).get('mode')==mode and (facet is None or condition_fields(r).get(facet)==fac)]
            if not group:continue
            xx=[];yy=[];lo=[];hi=[]
            for x in xvalues:
                points=[r for r in group if condition_fields(r).get(xkey)==str(x)]
                if not points:complete=False;continue
                s=trial_summary(points,f'fig{index}.{fac}.{mode}.{xkey}{x}')
                xx.append(xvalues.index(x));yy.append(s['median']);lo.append(s['median']-s['ci_low']);hi.append(s['ci_high']-s['median']);used+=points
            if xx:ax.errorbar(xx,yy,yerr=[lo,hi],fmt=MARKERS[i],linestyle='none',color=COLORS[i],capsize=2,label=mode)
        ax.set_xticks(range(len(xvalues)),xlabels);ax.set_ylim(bottom=0)
        ax.set_xlabel(xlabel);ax.set_ylabel('V throughput (MiB/s)');ax.legend(ncol=3)
        panel_title(ax,f'Accepted {experiment}'+(f'; {fac}' if fac else ''))
    save(fig,index,status='measured' if complete else 'partial',question=question,rows=used,
         caption=f'Accepted {experiment} trial-level V throughput; medians and 95% bootstrap CI conditional on the observed boots. Missing cells are not interpolated.',
         limitations=['Limited to the declared source base, workloads and accepted cells.','Future hardware results require a complete hash-bound import receipt.'])


def figure7(rows):
    rr=select(rows,'request_latency_cycles','board_E5_latency','request_sample')
    if not rr:
        return placeholder(7,'E5 request-latency distribution absent\nMean/max cannot reconstruct a CDF','Request latency (cycles)','Empirical cumulative fraction',None,'What is the request-latency distribution?')
    require_one_base(rr,context='fig7 request latency CDF')
    if any(int(r['n'])!=1 for r in rr):raise ValueError('CDF requires individual request samples, not summary rows')
    fig,ax=plt.subplots(figsize=(3.5,2.8))
    groups=sorted({(condition_fields(r).get('page_bytes','unknown'),condition_fields(r).get('mode','unknown')) for r in rr})
    for i,key in enumerate(groups):
        points=[r for r in rr if (condition_fields(r).get('page_bytes','unknown'),condition_fields(r).get('mode','unknown'))==key]
        data=np.sort([as_number(r) for r in points]);fraction=np.arange(1,len(data)+1)/len(data)
        ax.step(data,fraction,where='post',color=COLORS[i%5],linestyle=['-','--',':','-.'][i%4],label=f'{key[0]}B {key[1]} (n={len(data)})')
    ax.set_xlabel('Request latency (cycles)');ax.set_ylabel('Empirical cumulative fraction')
    ax.set_xlim(left=0);ax.set_ylim(0,1);ax.legend();panel_title(ax,'Observed requests only; no reconstructed tails')
    save(fig,7,status='measured',question='What is the request-latency distribution?',rows=rr,
         caption='Empirical CDF of accepted individual request latency samples, grouped by page size and co-run condition. The request count is not an independent trial count.',
         limitations=['Request-level dependence and collection/sampling scope must accompany native run metadata.','No CDF is reconstructed from means or maxima.'])


def figure8(rows):
    fig,axs=plt.subplots(3,1,figsize=(3.5,6.4));used=[]
    composition_audit=json.loads(stable_bytes(ANALYSIS/'history-composition-inputs.json'))
    byte_inputs={entry['w_row_id']:entry for entry in composition_audit['entries']}
    # One acquisition only: earlier acquisition versions remain in source data.
    acq='acq29_newboot'
    p=select(rows,'v_mib_s','p4m','median',acq=acq)
    require_one_base(p,context='fig8 P4M acquisition')
    gaps=sorted({int(condition_fields(r)['gap']) for r in p})
    for i,(metric,label) in enumerate((('v_mib_s','V'),('a_window_mib_s','A common window'))):
        yy=[];low=[];high=[]
        for gap in gaps:
            rr=select(rows,metric,'p4m',acq=acq,gap=gap)
            used+=rr;d={r['statistic']:as_number(r) for r in rr}
            yy.append(d['median']);low.append(d['median']-d['q1']);high.append(d['q3']-d['median'])
        axs[0].errorbar(range(len(gaps)),yy,yerr=[low,high],color=COLORS[i],marker=MARKERS[i],linestyle=['-','--'][i],capsize=2,label=label)
    axs[0].set_xticks(range(len(gaps)),gaps);axs[0].set_ylim(bottom=0)
    axs[0].set_xlabel('P4M gap (cycles)');axs[0].set_ylabel('Throughput (MiB/s)');axs[0].legend()
    panel_title(axs[0],'(a) Historical P4M; 00089; newboot n=29')
    for j,side in enumerate(('clean_history','isolated_history'),1):
        for i,mode in enumerate(('S','N','P','PN')):
            rr=select(rows,'W','composition','sample',side=side,A_pages=8,mode=mode)
            aa=select(rows,'a_completed_window','composition','sample',side=side,A_pages=8,mode=mode)
            if not rr:continue
            require_one_base(rr+aa,context=f'fig8 {side}/{mode}')
            s=trial_summary(rr,f'fig8.{side}.{mode}.W')
            a_by_id={r['row_id']:r for r in aa};rates=[]
            for row in rr:
                identity=byte_inputs[row['row_id']]
                ar=a_by_id[identity['a_completed_row_id']]
                if identity['W']!=as_number(row):raise ValueError('historical W/raw snapshot mismatch')
                rates.append(as_number(ar)*identity['chunk_bytes']*identity['clock_hz']/as_number(row)/(1024**2))
            a=summarize(rates,strata=[r['boot_id'] for r in rr])
            STATS.append(dict(label=f'fig8.{side}.{mode}.A_common_window_MiB_s',
                base_id=require_one_base(rr+aa,context='composition per-trial rate'),
                formula='per trial: A_completed_window*chunk_bytes*clock_hz/W/2**20',
                row_ids=[r['row_id'] for r in rr+aa],source_identity='history-composition-inputs.json',**a))
            used+=rr+aa
            axs[j].errorbar(s['median']/1e6,a['median'],
                xerr=np.asarray(err(s))/1e6,yerr=err(a),fmt=MARKERS[i],color=COLORS[i],capsize=2,label=mode)
        axs[j].margins(x=.15);axs[j].set_xlim(left=0);axs[j].set_ylim(bottom=0)
        axs[j].set_xlabel('V hardware window (million cycles)');axs[j].set_ylabel('A common-window rate (MiB/s)')
        axs[j].legend(ncol=4,loc='lower left')
        panel_title(axs[j],f'({chr(97+j)}) Historical {"clean cpn3" if j==1 else "isolated cpni1"}; n=3/mode')
    save(fig,8,status='historical_only',question='What V/A tradeoff is observed in historical mitigation versions?',rows=used,
         caption='Historical P4M newboot acquisition (median/IQR) and separately rendered clean/isolated S/N/P/PN implementations (median/95% conditional bootstrap CI). A common-window service rate is calculated per trial from A completed jobs x 4096 bytes divided by W/50MHz; it is not A full-task throughput. The composition panels are distinct versions/boots.',
         limitations=['Historical loaded-byte identity is unknown; dirty kernels retained.','P4M panel selects only acq29_newboot; earlier acquisitions remain separate.','A rate excludes post-V tail and uses the original fixed-50MHz assumption; independent A full-task completion time is not measured by this panel.','Composition common-window completion counts equal 4096 in these trials, so the derived A rate shares the measured W denominator.','Unified-baseline mitigation reruns remain missing.'])


def figure9(experiment_root):
    p=experiment_root/'E7-capacity-sim/results.csv'
    receipt_path=p.with_name('analysis-import.json')
    if not p.exists() or not receipt_path.exists():
        return placeholder(9,'Capacity sweep pending\nRTL simulation only','IOTLB capacity (entries)','V elapsed (simulation cycles)',['16','32','64'],'How does IOTLB capacity change interference in simulation?')
    receipt=json.loads(stable_bytes(receipt_path))
    if receipt.get('status')!='passed' or receipt.get('complete') is not True:
        return placeholder(9,'Capacity sweep not complete\nRTL simulation only','IOTLB capacity (entries)','V elapsed (simulation cycles)',['16','32','64'],'How does IOTLB capacity change interference in simulation?')
    raw,fields,content=read_csv(p)
    if hashlib.sha256(content).hexdigest()!=receipt.get('results_sha256'):
        raise ValueError('E7 results do not match the completed snapshot receipt')
    if not raw or any(r.get('checks_passed','').lower() not in ('true','1','passed') for r in raw):
        raise ValueError('E7 result exists but checks_passed is not true for all rows')
    if require_one_base(raw,context='E7 simulation capacity sweep')!=receipt.get('base_id'):
        raise ValueError('E7 result and receipt base_id mismatch')
    config_ids=[(r['capacity'],r['condition']) for r in raw]
    if len(config_ids)!=len(set(config_ids)):
        raise ValueError('E7 duplicate capacity/condition configuration')
    expected={(str(c),m) for c in (16,32,64) for m in ('Solo','A1','A8','A64')}
    if set(config_ids)!=expected:raise ValueError('E7 requires complete 12-cell 16/32/64 x Solo/A1/A8/A64 matrix')
    fig,axs=plt.subplots(2,1,figsize=(3.5,4.5))
    synthetic=[]
    for condition in ('Solo','A1','A8','A64'):
        i=MODES.index(condition)
        group=sorted([r for r in raw if r['condition']==condition],key=lambda r:int(r['capacity']))
        xs=[int(r['capacity']) for r in group]
        for j,metric in enumerate(('v_cycles','a_walks')):
            if j==1 and condition=='Solo':continue
            axs[j].plot(xs,[float(r[metric]) for r in group],marker=MARKERS[i%5],color=COLORS[i%5],linestyle=['-','--',':','-.'][i%4],label=condition)
        synthetic.extend(dict(row_id=f'E7.{condition}.c{r["capacity"]}',base_id=r['base_id']) for r in group)
    for ax in axs:
        ax.set_xlabel('IOTLB capacity (entries)');ax.set_ylim(bottom=0)
        ax.set_xticks([16,32,64])
    axs[0].set_ylabel('V elapsed (simulation cycles)');axs[1].set_ylabel('A walks (count)')
    axs[0].legend(ncol=2,loc='lower left');panel_title(axs[0],'(a) RTL simulation; behavioral FIFO DDR')
    vzero=all(float(r['v_walks'])==0 for r in raw)
    panel_title(axs[1],'(b) A walks; V walks=0 in all cells' if vzero else '(b) A walks; one run/configuration')
    save(fig,9,status='simulation',question='How does capacity change interference?',rows=synthetic,
         caption='Capacity sensitivity in RTL simulation with fixed FIFO behavioral DDR: 16-cycle head startup, then one 8-byte beat/cycle. V complete-task cycles and A walks are shown; V walks are zero in the accepted matrix. Each point is one deterministic configuration; no statistical CI or FPGA timing is inferred.',
         limitations=['Simulation only; model service differs from FPGA DDR.','No repeated trials or bootstrap CI.','A256/V64+2 warmup and the simulation supply model differ from the board campaign; calibration supports qualitative trends only.'])
    TRACE[-1]['source_data']['file']=str(p)
    TRACE[-1]['source_data']['sha256']=hashlib.sha256(content).hexdigest()
    TRACE[-1]['source_data']['configuration_rows']=[dict(input_line=i,capacity=r['capacity'],condition=r['condition'],raw_summary=r.get('source','unknown')) for i,r in enumerate(raw,2)]


def figure10(sim):
    fig,axs=plt.subplots(2,1,figsize=(3.5,4.6));used=[]
    modes=['m0','m1','m2','m3','m4-16','m4-64'];xx=np.arange(6)
    for j,domain in enumerate(('same','different')):
        group=select(sim,experiment='invalidation_sim',domain=domain,B_pages=8,map_policy='full-api')
        require_one_base(group,context='fig10 invalidation simulation')
        for k,(metric,label) in enumerate((('T_B','B complete'),('T_A','A last COPYDONE'),('A_final_flush_end','A incl. final flush'))):
            vals=[]
            for mode in modes:
                rr=[r for r in group if r['metric']==metric and condition_fields(r)['mode']==mode]
                if len(rr)!=1:raise ValueError(f'invalidation metric ambiguous/missing {domain}/{mode}/{metric}')
                used+=rr;vals.append(as_number(rr[0]))
            base=vals[0]
            axs[j].plot(xx,np.asarray(vals)/base,marker=MARKERS[k],color=COLORS[k],linestyle=['-','--',':'][k],label=label)
        axs[j].set_xticks(xx,['M0','M1','M2','M3','M4/16','M4/64'])
        axs[j].set_ylim(bottom=0);axs[j].set_ylabel('Completion / matching M0 (ratio)')
        axs[j].set_xlabel('Invalidation mode');panel_title(axs[j],f'({chr(97+j)}) {domain.capitalize()} domain; B=8 pages; simulation')
        if j==0:axs[j].legend(fontsize=8)
    save(fig,10,status='simulation',question='What completion tradeoffs follow invalidation scope?',rows=used,
         caption='Fixed-work invalidation RTL simulation: B completion, A last COPYDONE, and A including final flush, each divided by its matching same-domain/page-count M0 metric. B=8-page panels are descriptive deterministic configurations.',
         limitations=['Behavioral DDR and simulation steps only; CPU/Linux not executed.','Selected B=8-page full-api subset; B=4 and unmap-only remain in source table.','No repeated trials or confidence intervals.'])
    TRACE[-1]['source_data']['file']='../data/invalidation_sim_results.csv'


def write_numbers(rows):
    all_trials=select(rows,'hardware_ms','board_baseline100','sample')
    counts={len(baseline(rows,ds,mode,'hardware_ms')) for ds in (0,50) for mode in MODES}
    if len(counts)!=1:raise ValueError('baseline per-condition trial count is not uniform')
    macros={'BaselineTotalRuns':len(all_trials),'RunsPerCondition':counts.pop()}
    mapping={
        'BaselineTotalRuns':dict(formula='count distinct accepted hardware_ms sample rows (one/trial)',row_ids=[r['row_id'] for r in all_trials]),
        'RunsPerCondition':dict(formula='uniform count per DS x condition',groups=[dict(ds=ds,mode=mode,row_ids=[r['row_id'] for r in baseline(rows,ds,mode,'hardware_ms')]) for ds in (0,50) for mode in MODES])}
    for ds,word in ((0,'Zero'),(50,'Fifty')):
        for mode,mword in (('A8','AEight'),('A64','ASixtyFour')):
            shared=baseline(rows,ds,mode,'hardware_ms');base=baseline(rows,ds,'A1','hardware_ms')
            require_one_base(shared+base,context=f'number DS{ds} {mode}/A1')
            value=relative_slowdown(sample_values(shared),sample_values(base),
                    shared_strata=[r['boot_id'] for r in shared],baseline_strata=[r['boot_id'] for r in base])
            name=f'DS{word}{mword}ElapsedExtra';macros[name]=round(100*value['value'],2)
            mapping[name]=dict(formula='100*(median(hardware_ms, condition)/median(hardware_ms, A1)-1)',
                               base_id=require_one_base(shared+base,context=name),
                               numerator_row_ids=[r['row_id'] for r in shared],
                               denominator_row_ids=[r['row_id'] for r in base],statistics=value)
    def export_macro(name,selected,operation,formula=None):
        values=[as_number(r) for r in selected]
        if not values or any(v is None for v in values):raise ValueError(f'missing macro sources: {name}')
        macros[name]=operation(values)
        mapping[name]=dict(formula=formula or operation.__name__+' of explicit selected original scalar values',row_ids=[r['row_id'] for r in selected])
    pairs=[2,4,6,8]
    e3=[r for r in rows if r['row_id'] in [f'e3v6.main-case{k}.delta_V_remaining.paired_difference' for k in pairs]]
    export_macro('EThreeDeltaMin',e3,min);export_macro('EThreeDeltaMax',e3,max)
    macros['EThreePairCount']=len(e3);mapping['EThreePairCount']=dict(formula='count main complementary triggered pairs 2/4/6/8',row_ids=[r['row_id'] for r in e3])
    snapshot_path=ANALYSIS/'history-number-inputs.json';snapshot=json.loads(stable_bytes(snapshot_path))
    own={x['value'] for x in snapshot['values']}
    if own!={4} or len(snapshot['values'])!=8:raise ValueError('historical own-service export no longer matches 4 paired x 2 rounds')
    macros['EThreeOwnService']=own.pop();mapping['EThreeOwnService']=dict(formula='common measured V_own across 8 rounds of four main pairs',snapshot='analysis/history-number-inputs.json',source=snapshot['source'],values=snapshot['values'])
    pn=[r for r in rows if r['row_id'] in [f'composition.isolated_history-b{k}-a8.w_pct_change.paired_difference' for k in (1,2,3)]]
    export_macro('PNPenaltyMin',pn,lambda x:round(min(x),2),'round(min(three paired PN/P window changes in percent),2)')
    export_macro('PNPenaltyMax',pn,lambda x:round(max(x),2),'round(max(three paired PN/P window changes in percent),2)')
    for word,w in (('Seven',7),('Eight',8)):
        export_macro(f'CapacityW{word}',[r for r in rows if r['row_id']==f'e2.boundary-w{w}-forward-n112.d87_done_issue_cycles.median'],lambda x:int(x[0]),'original scalar trial median of done-to-issue probe cycles for this exact layout; no reaggregation')
    extra_path=ANALYSIS/'baseline100-extra.json'
    if extra_path.exists():
        extra=json.loads(stable_bytes(extra_path));lookup={x['result_id']:x for x in extra['statistics']}
        for name,key in [('DSZeroAOneAggregatePercent','baseline100-extra.ds0-middle.A1.total_over_solo_percent'),('DSZeroAEightAggregateLoss','baseline100-extra.ds0-middle.A8.aggregate_loss_vs_solo_percent'),('DSZeroASixtyFourAggregateLoss','baseline100-extra.ds0-middle.A64.aggregate_loss_vs_solo_percent')]:
            entry=lookup[key];macros[name]=round(entry['median'],2)
            mapping[name]=dict(formula='median from reviewed B1 analysis; '+str(entry['formula_id']),result_id=key,
                               source='analysis/baseline100-extra.json',source_sha256=hashlib.sha256(extra_path.read_bytes()).hexdigest(),
                               contributing_sample_ids=entry['contributing_sample_ids'])
    target=PAPER/'tex/numbers.tex';target.parent.mkdir(exist_ok=True)
    target.write_text('% Generated by analysis/scripts/plot_figures.py; values contain no percent sign.\n'+''.join(f'\\newcommand{{\\{k}}}{{{v}}}\n' for k,v in macros.items()))
    provenance=json.dumps(dict(macros=macros,derivations=mapping,
         count_definition='accepted baseline100 run/sequence identities; trials are not independent boots'),indent=2)+'\n'
    (ANALYSIS/'numbers-provenance.json').write_text(provenance)
    (PAPER/'tex/numbers-provenance.json').write_text(provenance)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--experiment-root',type=Path,default=PAPER/'exp')
    args=parser.parse_args()
    rows,_,_=read_csv(ANALYSIS/'merged-board-results.csv')
    sim,_,_=read_csv(PAPER/'data/invalidation_sim_results.csv')
    for row in sim:row['base_id'],row['base_scope']=historical_base(row)
    figure1(rows);figure2(rows);figure3(rows)
    native_trial_figure(rows,4,'board_E2_scaling','devices',[2,3,4],['2','3','4'],'Concurrent DMA devices (count)','How does device count affect interference?','E2 device-count matrix pending\nCurrent interface has one aggressor')
    native_trial_figure(rows,5,'board_E3_workloads','chunk_bytes',[64,1024,4096],['64','1024','4096'],'V transfer size (bytes)','How do workload and transfer size affect interference?','E3 workload x size matrix pending\nCurrent batch is fixed at 4096B',facet='shape')
    native_trial_figure(rows,6,'board_E4_pages','page_bytes',[4096,2097152],['4 KiB','2 MiB'],'Translation leaf page size','How does page size change interference?','E4 page-size pair pending\nNo verified 2 MiB leaf sample')
    figure7(rows)
    figure8(rows);figure9(args.experiment_root);figure10(sim);write_numbers(rows)
    (ANALYSIS/'figure-trace.json').write_text(json.dumps(TRACE,indent=2)+'\n')
    (ANALYSIS/'figure-statistics.json').write_text(json.dumps(dict(seed=SEED,n_resamples=N_RESAMPLES,statistics=STATS),indent=2)+'\n')
    print(f'built {len(TRACE)} figures (3.5in; fonts >=8pt); statuses: '+', '.join(f'{x["figure"]}:{x["status"]}' for x in TRACE))


if __name__=='__main__':main()
