#!/usr/bin/env python3
"""Remove only the explicit passive tap additions, then byte-compare original source."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

GOLD='85b157ebe71a623e326e51fff7b7126fe77e8eb9'
FIXED='5b7dfbc70e458244a8e5682e5ae3f8571320254c'

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--iommu-root',type=Path,required=True)
    ap.add_argument('--source-map',type=Path,required=True)
    ap.add_argument('--out',type=Path,required=True)
    a=ap.parse_args();a.out.parent.mkdir(parents=True,exist_ok=True)
    if a.out.exists():raise FileExistsError(a.out)
    report={'scope':'text identity only; does not substitute for RTL gold execution','status':'running','files':[]}
    try:
        for entry in json.loads(a.source_map.read_text())['files']:
            rel=entry['path'];current=(a.iommu_root/rel).read_bytes();s=current.decode()
            if rel.endswith('/regmap/rv_iommu_regmap.sv'):
                marker='\n  // Passive tap includes deferred DDTP writes, after the program response retires.\n'
                start=s.index(marker);end=s.index('endmodule',start);s=s[:start]+s[end:]
                s=s.replace('input  logic in_flight_i, // The IOMMU is currently processing a translation\n  output logic pn_idle_o','input  logic in_flight_i  // The IOMMU is currently processing a translation',1)
            else:
                marker='\n    // Passive PN eligibility tap; never feeds the original functional logic.\n'
                start=s.index(marker);end=s.index('endmodule',start);s=s[:start]+s[end:]
                s,n=re.subn(r',\n[ \t]*output logic pn_idle_o\b','',s,count=1);assert n==1,rel
            s,n=re.subn(r',\n[ \t]*parameter bit PnEnable = 1\'b0\b','',s,count=1);assert n==1,rel
            s=re.sub(r',\n[ \t]*\.PnEnable[ \t]*\(PnEnable\)','',s)
            s=re.sub(r',\n[ \t]*\.pn_idle_o[ \t]*\(pn_[a-z_]+\)','',s)
            s=re.sub(r'(?m)^[ \t]*assign pn_(msi|dbg)_idle = 1\'b1;\n','',s)
            original=subprocess.check_output(['git','show',GOLD+':'+rel],cwd=a.iommu_root)
            fixed=subprocess.check_output(['git','show',FIXED+':'+rel],cwd=a.iommu_root)
            item={'path':rel,'candidate_sha256':hashlib.sha256(current).hexdigest(),'original_sha256':hashlib.sha256(original).hexdigest(),'recovered_sha256':hashlib.sha256(s.encode()).hexdigest(),'original_equals_fixed':original==fixed,'recovered_byte_equal_original':s.encode()==original}
            report['files'].append(item);assert item['original_equals_fixed'] and item['recovered_byte_equal_original'],rel
        report['status']='passed_original_bytes_after_explicit_tap_removal'
    except Exception as exc:
        report['status']='failed';report['error']=repr(exc);raise
    finally:
        a.out.write_text(json.dumps(report,indent=2)+'\n')
        print(json.dumps({'status':report['status'],'files':len(report['files'])}))

if __name__=='__main__':main()
