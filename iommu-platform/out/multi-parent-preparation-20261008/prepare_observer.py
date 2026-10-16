#!/usr/bin/env python3
"""Codex critical causal observation. Prints/counters only; no DUT control."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent
s=(OUT/'trans_ctl_read_first.sv').read_text()
assert s.count('endmodule')==1
s=s.replace('endmodule', '''    // Observation only. SV-local cycles align with helper cycle_q after reset.
    logic [31:0] obs_cycle_q, obs_pending_q, obs_write_wait_q, obs_read_wait_q,
        obs_write_complete_q, obs_read_complete_q, obs_idle_q;
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            obs_cycle_q<=0; obs_pending_q<=0; obs_write_wait_q<=0;
            obs_read_wait_q<=0; obs_write_complete_q<=0; obs_read_complete_q<=0; obs_idle_q<=0;
        end else begin
            obs_cycle_q<=obs_cycle_q+1;
            if (ar_valid) begin
                obs_pending_q<=obs_pending_q+1;
                if (state_q==IDLE) obs_idle_q<=obs_idle_q+1;
                if (state_q==AWAIT_RESP && is_write) obs_write_wait_q<=obs_write_wait_q+1;
                if (state_q==AWAIT_RESP && is_read) obs_read_wait_q<=obs_read_wait_q+1;
                if (state_q==COMPLETE && is_write) obs_write_complete_q<=obs_write_complete_q+1;
                if (state_q==COMPLETE && is_read) obs_read_complete_q<=obs_read_complete_q+1;
            end
            if (state_q==IDLE && ar_valid)
                $display("CTL38-READY cycle=%0d ar=%h did=%0d aw_valid=%0d select_read=%0d",obs_cycle_q,
                    ar_data.addr,ar_data.stream_id,aw_valid,state_n==REQ_TRANS && !e3_o.select_write);
            if (trans_req_valid_o && trans_req_ready_i)
                $display("CTL38-REQUEST cycle=%0d read=%0d addr=%h did=%0d pending=%0d ar=%h",obs_cycle_q,
                    is_read,trans_req_data_q.iova,trans_req_data_q.did,ar_valid,ar_data.addr);
            if (trans_resp_valid_i && trans_resp_ready_o)
                $display("CTL38-RESPONSE cycle=%0d read=%0d addr=%h did=%0d pending=%0d ar=%h",obs_cycle_q,
                    is_read,trans_req_data_q.iova,trans_req_data_q.did,ar_valid,ar_data.addr);
        end
    end
    final $display("CTL38-STATS pending=%0d write_wait=%0d read_wait=%0d write_complete=%0d read_complete=%0d idle=%0d",
        obs_pending_q,obs_write_wait_q,obs_read_wait_q,obs_write_complete_q,obs_read_complete_q,obs_idle_q);
endmodule''')
(OUT/'trans_ctl_observer.sv').write_text(s)
s=(OUT/'multi_parent_translation.sv').read_text()
old='                    filled_q<=filled_q+1;'
assert s.count(old)==1
s=s.replace(old,old+'''
                    $display("PREP4-FILL cycle=%0d owner=%0d parent=%0d iova=%h",cycle_q,
                        slow_owner_q,slow_seq_q,slow_req_q.iova);''')
(OUT/'observer_parent_translation.sv').write_text(s)
argv=json.loads((OUT/'build-read-first-command.json').read_text())
replace={str(OUT/'trans_ctl_read_first.sv'):str(OUT/'trans_ctl_observer.sv'),
    str(OUT/'multi_parent_translation.sv'):str(OUT/'observer_parent_translation.sv'),
    str(OUT/'obj-read-first'):str(OUT/'obj-observer')}
argv=[replace.get(x,x) for x in argv]
(OUT/'build-observer-command.json').write_text(json.dumps(argv,indent=2)+'\n')
