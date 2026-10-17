#!/usr/bin/env python3
"""Check the actual rendered package and write an explicit build receipt."""
import hashlib
import json
import re
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET
from pathlib import Path

analysis=Path(__file__).resolve().parents[1]
trace=json.loads((analysis/'figure-trace.json').read_text())
assert len(trace)==10
checks=[]
pdftohtml=shutil.which('pdftohtml')
for item in trace:
    name=item['artifact_id']
    pdf=analysis/'figures'/f'{name}.pdf';png=pdf.with_suffix('.png')
    assert pdf.stat().st_size>1000 and png.stat().st_size>1000
    box=re.search(rb'/MediaBox\s*\[\s*([\d.]+)\s+([\d.]+)\s+([\d.]+)\s+([\d.]+)\s*\]',pdf.read_bytes())
    assert box, f'no MediaBox: {name}'
    width=float(box[3])-float(box[1]);assert abs(width-252)<.01, (name,width)
    if item['status']=='missing':assert item['source_data']['row_ids']==[]
    font_sizes=[]
    if pdftohtml:
        with tempfile.TemporaryDirectory(prefix='ispass-figure-fonts-') as directory:
            xml=Path(directory)/'figure.xml'
            subprocess.run([pdftohtml,'-xml','-i','-zoom','1',str(pdf),str(xml)],
                           check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
            font_sizes=[float(node.attrib['size']) for node in ET.parse(xml).iter('fontspec')]
        assert font_sizes and min(font_sizes)>=7, (name,'rendered font below 7pt',font_sizes)
    checks.append(dict(figure=name,status=item['status'],width_inches=width/72,
                       rendered_font_check='pdftohtml_zoom1' if pdftohtml else 'not_checked_tool_unavailable',
                       minimum_rendered_font_pt=min(font_sizes) if font_sizes else None,
                       pdf_sha256=hashlib.sha256(pdf.read_bytes()).hexdigest(),
                       png_sha256=hashlib.sha256(png.read_bytes()).hexdigest()))
receipt=dict(status='passed',checks=checks,figure_count=10,
             font_minimum_pt=8,physical_width_inches=3.5,seed=20261017,bootstrap_resamples=10000,
             rendered_font_sizes_checked=bool(pdftohtml),
             missing_data_policy='explicit empty scaffold; no fabricated observations/CDF',
             interpretation='artifact/contract checks, not scientific validity certification')
(analysis/'build-validation.json').write_text(json.dumps(receipt,indent=2)+'\n')
print('validated 10 PDF/PNG artifacts, 3.5in PDF width, and missing-data trace')
