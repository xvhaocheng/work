#!/usr/bin/env python3
"""Conceptual request path, with workload counts read from the existing CSV.

Topology sources: invalidation runner/invalidation_dma_top.sv:130-183,276-310;
runner/README.md:3,24-26,39-43. CVA6 parent platform: iommu-platform/README.md:3-5.
This is a schematic, not a measured topology graph or timing diagram.
"""
import csv
import os
from pathlib import Path
PAPER=Path(__file__).resolve().parents[1]
os.environ.setdefault('MPLCONFIGDIR',str(PAPER/'build/matplotlib'))
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch
ROOT=PAPER.parents[1]

def main():
    rows=list(csv.DictReader((ROOT/'iommu-platform/out/invalidation-interference-20261017/analysis/results.csv').open()))
    r=next(x for x in rows if x['case_id']=='same-b4-m1-full-api')
    plt.rcParams.update({'font.size':8,'font.family':'DejaVu Sans','pdf.fonttype':42})
    fig,ax=plt.subplots(figsize=(3.35,2.35))
    ax.set(xlim=(0,10),ylim=(0,7));ax.axis('off')
    def box(x,y,w,h,t,c='#e8f0f7'):
        ax.add_patch(FancyBboxPatch((x,y),w,h,boxstyle='round,pad=0.06',facecolor=c,edgecolor='#44515c',linewidth=.7))
        ax.text(x+w/2,y+h/2,t,ha='center',va='center',fontsize=8)
    def arrow(a,b,dashed=False):
        ax.add_patch(FancyArrowPatch(a,b,arrowstyle='-|>',mutation_scale=8,color='#44515c',linewidth=.8,linestyle='--' if dashed else '-'))
    box(.2,5.55,3.15,1.1,'Test driver\nMMIO + PTE writes','#f8eadc')
    box(4.4,5.55,2.35,1.1,f'iDMA A\n{r["A_jobs"]} COPY')
    box(7.35,5.55,2.35,1.1,f'iDMA B\n{r["B_jobs"]} COPY')
    box(4.4,3.5,5.3,1.35,'RISC-V IOMMU\nTranslation cache\nPage walker')
    box(.2,3.5,3.15,1.35,'Command queue\nIOTINVAL\nIOFENCE','#f8eadc')
    box(.2,.75,9.5,1.15,'Behavioral DDR: payload + page tables + CQ','#e4f1e8')
    arrow((1.77,5.55),(1.77,4.85));arrow((3.35,4.16),(4.4,4.16))
    arrow((5.57,5.55),(5.57,4.85));arrow((8.53,5.55),(8.53,4.85))
    arrow((1.77,3.5),(1.77,1.9));arrow((7.05,3.5),(7.05,1.9))
    ax.text(4.65,2.65,'payload / walks / CQ fetch',fontsize=8,ha='center')
    ax.text(4.95,.15,'CVA6 context; CPU and Linux not executed',ha='center',fontsize=8)
    fig.subplots_adjust(left=.01,right=.99,top=.99,bottom=.02)
    for ext in ('pdf','png'):fig.savefig(PAPER/f'figures/platform.{ext}',dpi=220)
    plt.close(fig)

if __name__=='__main__':main()
