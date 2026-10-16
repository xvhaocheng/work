#!/usr/bin/env python3
"""Codex: ordinary ready-result bypass of background TL, preserving held offers."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent
s=(OUT/'observer_parent_translation.sv').read_text()
changes={
    'm_valid_o=!slow_q && !hit_q && (prep_offer_q || (s_valid_i && !reuse));':
    'm_valid_o=!slow_q && (prep_offer_q || (!hit_q && s_valid_i && !reuse));',
    's_ready_o=!slow_q && !hit_q && !prep_offer_q && (reuse || m_ready_i);':
    '''s_ready_o=!hit_q && ((reuse && (!slow_q || slow_prepare_q)) ||
            (!reuse && !slow_q && !prep_offer_q && m_ready_i));''',
    'm_resp_ready_o=!hit_q && (slow_prepare_q || s_resp_ready_i);':
    'm_resp_ready_o=slow_prepare_q || (!hit_q && s_resp_ready_i);',
    'if (slow_q || hit_q || m_valid_o) $fatal(1,"reuse overlaps slow request");':
    '''if ((slow_q && !slow_prepare_q) || hit_q || (m_valid_o && !prep_offer_q))
                    $fatal(1,"reuse overlaps formal slow request");''',
    'if (slow_q || hit_q) $fatal(1,"multi-parent forward overwritten");':
    '''if (slow_q || (hit_q && !prep_offer_q))
                    $fatal(1,"multi-parent forward overwritten");''',
    'if (!slow_q || hit_q || m_resp_i.bank!=slow_req_q.bank)':
    'if (!slow_q || (hit_q && !slow_prepare_q) || m_resp_i.bank!=slow_req_q.bank)'
}
for old,new in changes.items():
    assert s.count(old)==1,old
    s=s.replace(old,new)
s=s.replace('// This prototype does not establish fault, invalidation or epoch semantics.',
'''// This prototype does not establish fault, invalidation or epoch semantics.
// Hits can return while a background offer/request is pending. Normal slow
// requests remain serialized. A held background VALID never depends on hit_q;
// its response is consumed internally, independently of the TC hit response.''')
# Observation/assertion state, excluded from the functional state ledger.
needle='    logic [31:0] forwarded_q, filled_q, reused_q, prepared_count_q, cycle_q;'
s=s.replace(needle,needle+'''
    logic held_bg_q;
    req_t held_bg_req_q;
    logic [31:0] overlap_hits_q;
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin held_bg_q<=0; held_bg_req_q<='0; overlap_hits_q<=0; end
        else begin
            if (held_bg_q && (!m_valid_o || m_req_o!=held_bg_req_q))
                $fatal(1,"hit bypass changed held background request");
            held_bg_q<=m_valid_o && !m_ready_i && prep_offer_q;
            if (m_valid_o && !m_ready_i && prep_offer_q) held_bg_req_q<=m_req_o;
            if (s_valid_i && s_ready_o && reuse && (slow_q || prep_offer_q))
                overlap_hits_q<=overlap_hits_q+1;
        end
    end
    final $display("BYPASS38-STATS overlap_hits=%0d held=%0d observation_bits=%0d",
        overlap_hits_q,held_bg_q,33+$bits(req_t));''')
(OUT/'hit_bypass_parent_translation.sv').write_text(s)
for tc,core,mdir in [('aw-first','out/natural-write-order-20261008/trans_ctl_write_limit.sv','obj-bypass-aw'),
                     ('read-first',str(OUT/'trans_ctl_observer.sv'),'obj-bypass-read')]:
    argv=json.loads((OUT/'build-observer-command.json').read_text())
    repl={str(OUT/'trans_ctl_observer.sv'):str(ROOT/core) if not core.startswith('/') else core,
          str(OUT/'observer_parent_translation.sv'):str(OUT/'hit_bypass_parent_translation.sv'),
          str(OUT/'obj-observer'):str(OUT/mdir)}
    argv=[repl.get(x,x) for x in argv]
    (OUT/f'build-bypass-{tc}-command.json').write_text(json.dumps(argv,indent=2)+'\n')
