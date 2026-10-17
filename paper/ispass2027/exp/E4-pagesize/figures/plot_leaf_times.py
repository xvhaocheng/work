#!/usr/bin/env python3
"""Static E4 hardware-time figure from the immutable accepted complete20 export."""
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
EXPECTED={'summary.json':'7c1db23987d7f887ffb58284a9c8e39920b748d1d03ff0ed45f2605b3192b3d7',
          'paper.csv':'8fd180adc94b135060e6d86d2f6b3c4e6050a772438a67e69f8f2d80f454a22c',
          'analysis-manifest.json':'c6dbfaecee3ddf0f11bf6c56ec0a0a36b1cce7ed68fb46214fd95bb584dd3ef6'}
STATS=('n','min','median','max','q1','q3','iqr')


def require(ok,message):
    if not ok:raise ValueError(message)
def sha(raw):return hashlib.sha256(raw).hexdigest()
def encoded(value):return json.dumps(value,indent=2,ensure_ascii=False,allow_nan=False)+'\n'
def locate(value,pointer):
    for field in pointer[1:].split('/'):
        key=field.replace('~1','/').replace('~0','~');value=value[int(key)] if isinstance(value,list) else value[key]
    return value
def synthetic(value):
    if isinstance(value,dict):
        for k,v in value.items():
            if k in ('fixture_only','is_fixture','simulation_only','synthetic_fixture','mock_fixture') and (v is True or str(v).lower() in ('1','true')):return True
            if k in ('evidence_level','evidence_kind','source_kind') and isinstance(v,str) and any(t in v.lower() for t in ('fixture','synthetic','mock')):return True
            if k=='fixtures' and v:return True
            if synthetic(v):return True
    elif isinstance(value,list):return any(synthetic(v) for v in value)
    return False
def stats(values):
    values=sorted(values);require(values and all(math.isfinite(x) and x>0 for x in values),'positive finite durations required')
    def q(p):
        h=(len(values)-1)*p;return values[math.floor(h)]+(values[math.ceil(h)]-values[math.floor(h)])*(h-math.floor(h))
    return dict(n=len(values),min=values[0],median=q(.5),max=values[-1],q1=q(.25),q3=q(.75),iqr=q(.75)-q(.25))
def check_stats(values,accepted):
    fresh=stats(values)
    require(all(math.isclose(fresh[k],accepted[k],rel_tol=1e-12,abs_tol=1e-12) for k in STATS),'accepted Type7 statistics disagree with actual points')
    return fresh


def load(source):
    raw={name:(source/name).read_bytes() for name in EXPECTED}
    require(all(sha(raw[name])==digest for name,digest in EXPECTED.items()),'frozen complete20 export bytes differ')
    data=json.loads(raw['summary.json']);manifest=json.loads(raw['analysis-manifest.json'])
    require(not synthetic(data) and not synthetic(manifest),'synthetic paper input forbidden')
    require(data['status']=='complete_raw_bound_analysis' and data['source_cases']==data['source_board_cases']==20
            and data['campaign_complete'] is True and data['validated_plan_complete'] is True and data['evidence_level']=='fpga_board','complete20 actual board input required')
    groups={g['condition']:g for g in data['groups']};require(set(groups)=={'leaf4k','leaf2m'},'exact two leaf conditions required')
    samples={};numerics={}
    for condition,page in [('leaf4k',4096),('leaf2m',2097152)]:
        g=groups[condition];require(g['mapping_page_bytes']==page and g['n']==len(g['samples'])==10,'ten actual points per leaf required')
        samples[condition]={x['block']:x for x in g['samples']};require(set(samples[condition])==set(range(1,11)),'ten distinct actual blocks required')
        numerics[condition]=check_stats([x['metrics']['hardware_ms'] for x in g['samples']],g['statistics']['hardware_ms'])
    paired=data['paired_leaf2m_over_leaf4k'];require(paired['n']==len(paired['pairs'])==10,'ten pairs required');pairs=[]
    for b,item in enumerate(paired['pairs'],1):
        require(item['block']==b and item['boot_id']==data['boot_id'],'pair ordering/boot differs')
        small,large=samples['leaf4k'][b],samples['leaf2m'][b]
        require(small==item['leaf4k'] and large==item['leaf2m'] and small['boot_id']==large['boot_id']==data['boot_id'],'actual pair reference differs')
        ratio=large['metrics']['hardware_ms']/small['metrics']['hardware_ms']
        require(math.isclose(ratio,item['time_ratio_leaf2m_over_leaf4k'],rel_tol=1e-14),'actual same-block ratio differs')
        pair={'block':b,'boot_id':data['boot_id'],'leaf2m_over_leaf4k_time_ratio':ratio}
        for name,sample in [('leaf4k',small),('leaf2m',large)]:
            pair.update({name+'_'+key:sample[key] for key in ('case','sequence','raw_command_path','raw_sha256')})
            pair[name+'_hardware_ms']=sample['metrics']['hardware_ms'];pair[name+'_hardware_cycles']=sample['metrics']['hardware_cycles']
        pairs.append(pair)
    require(len({(x['boot_id'],x[name+'_sequence']) for x in pairs for name in ('leaf4k','leaf2m')})==20,'reused boot/sequence')
    numerics['paired_ratio']=check_stats([x['leaf2m_over_leaf4k_time_ratio'] for x in pairs],paired['statistics'])
    reader=csv.DictReader(io.StringIO(raw['paper.csv'].decode()));rows=list(reader);require(len(reader.fieldnames)==17 and len(rows)==3824,'accepted common CSV dimensions differ')
    relevant=[x for x in rows if x['metric'] in ('hardware_ms','paired_hardware_time_ratio_leaf2m_over_leaf4k')]
    require(len([x for x in relevant if x['statistic']=='sample'])==30,'CSV must expose all20 times and10 ratios')
    for row in relevant:
        require(row['evidence_level']=='fpga_board' and row['boot_id']==data['boot_id'],'CSV evidence/boot differs')
        require(math.isclose(float(row['value']),locate(data,row['source_locator']),rel_tol=1e-12,abs_tol=1e-12),'CSV locator/value differs')
    return data,raw,pairs,numerics


def draw(pairs,numerics,output):
    plt.rcParams.update({'font.family':'DejaVu Sans','font.size':8,'axes.titlesize':9,'axes.labelsize':8,
        'xtick.labelsize':7.5,'ytick.labelsize':7.5,'pdf.fonttype':42,'ps.fonttype':42,'svg.fonttype':'none','axes.spines.top':False,'axes.spines.right':False})
    fig,(left,right)=plt.subplots(1,2,figsize=(7.15,2.8),gridspec_kw={'width_ratios':[1,1.2]},layout='constrained')
    colors=['#0072B2','#D55E00'];markers=['o','D']
    for pair in pairs:
        offset=(pair['block']-5.5)*.035
        x=[offset,1+offset];y=[pair['leaf4k_hardware_ms'],pair['leaf2m_hardware_ms']]
        left.plot(x,y,color='.72',lw=.65,zorder=1)
        for j in range(2):left.scatter(x[j],y[j],s=16,marker=markers[j],facecolor=colors[j],edgecolor='white',linewidth=.35,zorder=3)
    for j,name in enumerate(('leaf4k','leaf2m')):
        s=numerics[name];x=j+.25
        left.errorbar(x,s['median'],yerr=[[s['median']-s['q1']],[s['q3']-s['median']]],fmt='_',color='black',markersize=10,capsize=4,lw=1.5,zorder=4)
        left.text(j,.97,f"median {s['median']:.3f}\nIQR {s['iqr']:.4f} ms",ha='center',va='top',transform=left.get_xaxis_transform(),fontsize=7)
    left.set(xlim=(-.28,1.4),ylim=(100,150),xticks=[0,1],xticklabels=['4 KiB leaves','2 MiB leaves'],ylabel='V hardware time (ms)',title='(a) All 10 paired blocks')
    left.grid(axis='y',color='.9',lw=.5,zorder=0)
    ratio=numerics['paired_ratio'];right.axhspan(ratio['q1'],ratio['q3'],color='.87',zorder=0)
    right.axhline(ratio['median'],color='black',lw=1,zorder=1)
    right.scatter([x['block'] for x in pairs],[x['leaf2m_over_leaf4k_time_ratio'] for x in pairs],s=22,color='#333333',marker='o',zorder=3)
    right.text(.5,.97,f"paired median {ratio['median']:.6f}; IQR {ratio['iqr']:.6f}",ha='center',va='top',transform=right.transAxes,fontsize=7)
    right.set(xlim=(.5,10.5),ylim=(.760,.775),xticks=range(1,11),xlabel='Paired block',ylabel='2 MiB / 4 KiB hardware time',title='(b) Same-block time ratios (n = 10)')
    right.grid(axis='y',color='.92',lw=.5,zorder=0)
    for extension in ('pdf','svg','png'):
        metadata={'CreationDate':None,'ModDate':None} if extension=='pdf' else {'Date':None} if extension=='svg' else {}
        fig.savefig(output/('e4-leaf-times.'+extension),dpi=300,metadata=metadata)
    plt.close(fig)


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',required=True,type=Path);p.add_argument('--output',required=True,type=Path);a=p.parse_args()
    output=a.output.resolve();require(output.is_relative_to(HERE.resolve()) and not output.exists(),'new figures-local output required')
    data,raw,pairs,numerics=load(a.source.resolve());output.mkdir();inputs=output/'inputs';inputs.mkdir()
    for name,value in raw.items():(inputs/name).write_bytes(value)
    (output/'plot_leaf_times.as-executed.py').write_bytes(Path(__file__).read_bytes())
    with (output/'paired-hardware-times.csv').open('x',newline='') as stream:
        writer=csv.DictWriter(stream,list(pairs[0]),lineterminator='\n');writer.writeheader();writer.writerows(pairs)
    (output/'numerical-verification.json').write_text(encoded({'status':'passed','fixture_only':False,'new_board_measurements':0,'source_board_cases':20,'groups_and_paired_type7':numerics,'visible_trial_points':20,'visible_pair_ratio_points':10}))
    draw(pairs,numerics,output)
    manifest={'schema':'ispass2027.e4.hardware_time_figure.v1','status':'passed','fixture_only':False,'source_board_cases':20,'new_board_measurements':0,
        'source':str(a.source.resolve()),'inputs_sha256':EXPECTED,'script_sha256':sha(Path(__file__).read_bytes()),
        'runtime':{'python':platform.python_version(),'executable':sys.executable,'matplotlib':matplotlib.__version__,'backend':'Agg'},
        'bit_sha256':data['bit_sha256'],'kernel_release':data['kernel_release'],'boot_id':data['boot_id'],
        'paired_references':pairs,'numerical_statistics':numerics,
        'scope':'fixed V0/A1 workload; 20 actual hardware-time samples and10 same-block pairs; all shown, no discarded first point',
        'rendering':'left horizontal offsets identify blocks, gray lines pair actual observations, black marks median/IQR; right gray band IQR and black median of paired ratios; ratio axis explicitly zoomed',
        'artifacts':{str(f.relative_to(output)):{'sha256':sha(f.read_bytes()),'bytes':f.stat().st_size} for f in sorted(output.rglob('*')) if f.is_file()}}
    (output/'manifest.json').write_text(encoded(manifest));print(json.dumps({'status':'passed','source_board_cases':20,'new_board_measurements':0,'output':str(output)}))

if __name__=='__main__':main()
