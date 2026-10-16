#!/usr/bin/env python3
"""Codex: pre-fragment already certifies W drain; ordinary TC read-first control."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent
s=(ROOT/'out/natural-write-order-20261008/trans_ctl_write_limit.sv').read_text()
# Only IDLE arbitration changes. Existing held TC/AXI offers are untouched.
assert s.count('if (aw_valid && !w_slots_full) begin')==1
s=s.replace('if (aw_valid && !w_slots_full) begin','if (aw_valid && !w_slots_full && !ar_valid) begin')
s=s.replace('((aw_valid && !w_slots_full) || ar_valid)', '((aw_valid && !w_slots_full && !ar_valid) || ar_valid)')
s=s.replace('e3_o.select_write = aw_valid && !w_slots_full;', 'e3_o.select_write = aw_valid && !w_slots_full && !ar_valid;')
(OUT/'trans_ctl_read_first.sv').write_text(s)
argv=json.loads((OUT/'build-command.json').read_text())
argv=[str(OUT/'trans_ctl_read_first.sv') if x==str(ROOT/'out/natural-write-order-20261008/trans_ctl_write_limit.sv')
    else str(OUT/'obj-read-first') if x==str(OUT/'obj-multi') else x for x in argv]
(OUT/'build-read-first-command.json').write_text(json.dumps(argv,indent=2)+'\n')
