#!/usr/bin/env python3
"""E5 stage1 V read/write: actual per-trial quantile bucket bounds, no pooling."""
import argparse
import csv
import hashlib
import io
import json
import math
from pathlib import Path
import platform
import sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

HERE=Path(__file__).resolve().parent
EXPECTED={'e5-stage1-actual-intervals.csv':'cc71716cb3313071bf0b3fbdad7da9b18ecf034cbb8d64b75963c88e71e83666',
          'e5-stage1-actual-inputs.json':'ac53605965f293eb41709736f2cf05f85c9d9b43a1be7bb75833feff6c6002f2'}


def require(ok,message):
    if not ok:raise ValueError(message)
def sha(raw):return hashlib.sha256(raw).hexdigest()
def encoded(value):return json.dumps(value,indent=2,allow_nan=False)+'\n'
def bounds(index):
    if index<2:return index,index
    return 1<<(index-1),(1<<index)-1 if index<31 else (1<<32)-1


def load(source):
    raw={n:(source/n).read_bytes() for n in EXPECTED};require(all(sha(raw[n])==v for n,v in EXPECTED.items()),'frozen E5 input bytes differ')
    receipt=json.loads(raw['e5-stage1-actual-inputs.json']);rows=list(csv.DictReader(io.StringIO(raw['e5-stage1-actual-intervals.csv'].decode())))
    require(not receipt['fixture_only'] and receipt['source_board_cases']==20 and receipt['new_board_measurements']==0 and len(rows)==40,'actual20/40 input required')
    # Rebind original full exports; no raw collection or request populations are merged.
    original={Path(n):Path(n).read_bytes() for n in receipt['inputs']}
    require(all(sha(value)==receipt['inputs'][str(n)] for n,value in original.items()),'original E5 export changed')
    require(len({(r['boot_id'],r['sequence']) for r in rows})==20 and len({r['boot_id'] for r in rows})==1,'exact20 trial identities required')
    require(len({(r['condition'],r['block'],r['channel']) for r in rows})==40,'duplicate trial/channel')
    display=[]
    for channel in ('r','w'):
        for condition,page in [('leaf4k',4096),('leaf2m',2097152)]:
            cell=[r for r in rows if r['channel']==channel and r['condition']==condition]
            require(len(cell)==10 and {int(r['block']) for r in cell}==set(range(1,11)),'ten distinct blocks per cell')
            for r in cell:
                require(r['role']=='v' and r['stage']=='1' and int(r['mapping_page_bytes'])==page and r['fixture_only']=='False'
                    and int(r['source_board_cases'])==int(r['n_valid'])==1 and int(r['observer_hz'])==50000000,'V stage1 actual input')
                require(int(r['full'])==8192 and all(int(r[k])==0 for k in ('carry','opening','closing')),'full-only V window population')
            for percentile in (50,95,99):
                prefix='p'+str(percentile);intervals=set()
                for r in cell:
                    lo,hi=(int(r[prefix+'_'+name+'_cycles']) for name in ('lo','hi'));index=int(r[prefix+'_bin'])
                    require(int(r[prefix+'_rank'])==math.ceil(percentile*int(r['full'])/100) and (lo,hi)==bounds(index),'original nearest-rank bucket boundaries required')
                    intervals.add((lo,hi))
                require(len(intervals)==1,'this figure is limited to the accepted10/10 identical-interval data')
                lo,hi=intervals.pop();display.append({'condition':condition,'role':'v','channel':channel,'stage':1,'percentile':percentile,
                    'n_trials':10,'identical_trial_intervals':10,'lo_cycles':lo,'hi_cycles':hi,'interval_meaning':'bucket quantization bounds, not trial confidence interval'})
    return raw,receipt,original,rows,display


def draw(output,display):
    plt.rcParams.update({'font.family':'DejaVu Sans','font.size':8,'axes.titlesize':9,'axes.labelsize':8,
        'xtick.labelsize':8,'ytick.labelsize':7.5,'pdf.fonttype':42,'ps.fonttype':42,'svg.fonttype':'none',
        'axes.spines.top':False,'axes.spines.right':False})
    fig,axes=plt.subplots(1,2,figsize=(7.15,3.15),sharey=True)
    fig.subplots_adjust(left=.10,right=.985,bottom=.29,top=.82,wspace=.15)
    colors={'leaf4k':'#0072B2','leaf2m':'#D55E00'}
    for axis,channel,title in zip(axes,('r','w'),('(a) V read, stage 1','(b) V write, stage 1')):
        axis.set_yscale('log',base=2);axis.set_ylim(4,4096);axis.set_xlim(-.45,2.45)
        for row in [r for r in display if r['channel']==channel]:
            x={50:0,95:1,99:2}[row['percentile']]+(-.12 if row['condition']=='leaf4k' else .12)
            lo,hi=row['lo_cycles'],row['hi_cycles'];color=colors[row['condition']]
            axis.vlines(x,lo,hi,color=color,lw=2)
            axis.hlines([lo,hi],x-.075,x+.075,color=color,lw=1.5)
        axis.set_xticks([0,1,2],['p50','p95','p99']);axis.set_xlabel('Within-trial request quantile');axis.set_title(title,pad=10)
        axis.set_yticks([8,16,32,64,128,256,512,1024,2048],[str(x) for x in [8,16,32,64,128,256,512,1024,2048]])
        axis.grid(axis='y',color='.90',lw=.5,zorder=0)
    axes[0].set_ylabel('Stage 1 residence interval (cycles)')
    fig.legend([Line2D([0],[0],color=colors[k],lw=2) for k in ('leaf4k','leaf2m')],['4 KiB leaves','2 MiB leaves'],
        loc='upper center',ncol=2,frameon=False,bbox_to_anchor=(.55,1.0))
    fig.text(.54,.115,'10/10 trial intervals identical in every cell; n = 10 trials per leaf condition.',ha='center',fontsize=7)
    fig.text(.54,.055,'Spans are bucket bounds, not trial confidence intervals. 50 MHz: 1 cycle = 20 ns.',ha='center',fontsize=7)
    for extension in ('pdf','svg','png'):
        metadata={'CreationDate':None,'ModDate':None} if extension=='pdf' else {'Date':None} if extension=='svg' else {}
        fig.savefig(output/('e5-stage1-intervals.'+extension),dpi=300,metadata=metadata)
    plt.close(fig)


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,default=HERE);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    output=a.output.resolve();require(output.is_relative_to(HERE.resolve()) and not output.exists(),'new figures-local output required')
    raw,receipt,original,rows,display=load(a.source.resolve());output.mkdir();inputs=output/'inputs';inputs.mkdir()
    for n,value in raw.items():(inputs/n).write_bytes(value)
    for i,(path,value) in enumerate(original.items()):(inputs/(f'original-{i}-'+path.name)).write_bytes(value)
    (output/'plot_stage1_intervals.as-executed.py').write_bytes(Path(__file__).read_bytes())
    with (output/'displayed-intervals.csv').open('x',newline='') as stream:
        writer=csv.DictWriter(stream,list(display[0]),lineterminator='\n');writer.writeheader();writer.writerows(display)
    draw(output,display)
    report={'schema':'ispass2027.e5.stage1.interval_figure.v1','status':'passed','fixture_only':False,'source_board_cases':20,'new_board_measurements':0,
        'source_selected_trial_stages':40,'source_quantile_intervals':120,'displayed_intervals':12,'identical_trial_intervals_per_displayed_interval':10,
        'script_sha256':sha(Path(__file__).read_bytes()),'input_sha256':EXPECTED,'original_exports':receipt['inputs'],
        'runtime':{'python':platform.python_version(),'executable':sys.executable,'matplotlib':matplotlib.__version__,'backend':'Agg'},
        'request_pooling':False,'exact_p99_inferred':False,'interval_is_confidence_interval':False,'observer_hz':50000000,'ns_per_cycle':20,
        'trial_references':[{k:r[k] for k in ('case','block','condition','channel','stage','boot_id','sequence','raw_command_path','bit_sha256','kernel_release')} for r in rows],
        'scope':'TR handshake to first IOMMU request output offer, V stage1 read/write bursts; full-only window, not pure PTW or descriptor latency',
        'artifacts':{str(f.relative_to(output)):{'sha256':sha(f.read_bytes()),'bytes':f.stat().st_size} for f in sorted(output.rglob('*')) if f.is_file()}}
    (output/'manifest.json').write_text(encoded(report));print(json.dumps({'status':'passed','source_board_cases':20,'new_board_measurements':0,'output':str(output)}))

if __name__=='__main__':main()
