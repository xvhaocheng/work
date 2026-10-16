"""Codex: bounded parent-capacity experiment; original files stay untouched."""
import json
from pathlib import Path

ROOT=Path('/home/xvhaocheng/work/iommu-platform')
OLD=ROOT/'out/posttl-independent-20261008'
OUT=ROOT/'out/parent-capacity-20261008'

def one(s,a,b):
    if s.count(a)!=1: raise RuntimeError(f'interface occurrence: {a[:80]} = {s.count(a)}')
    return s.replace(a,b,1)

s=(OLD/'posttl_owner_fragment.sv').read_text()
s=one(s,'    parameter type ar_t = logic, parameter type r_t = logic',
      '    parameter int unsigned ParentDepth = 8,\n    parameter type ar_t = logic, parameter type r_t = logic')
s=one(s,'    ar_t parent_q [2][D];','    ar_t parent_q [2][ParentDepth];')
s=one(s,'    logic [63:0] offset_q [2][D], virtual_offset_q [2][D];',
      '    logic [63:0] offset_q [2][ParentDepth], virtual_offset_q [2][D];')
s=one(s,'    logic [8:0] returned_q [2][D];','    logic [8:0] returned_q [2][ParentDepth];')
s=one(s,'pa_q[s_ar_i.id[2]] - done_q[s_ar_i.id[2]] < D;',
      'pa_q[s_ar_i.id[2]] - done_q[s_ar_i.id[2]] < ParentDepth;')
s=one(s,"            n = int'(done_q[s][2:0]);", "            n = p_index(done_q[s]);")
s=s.replace('done_q[child_owner_q][2:0]', 'p_index(done_q[child_owner_q])')
s=one(s,'pa_q[s]-done_q[s] > D)', 'pa_q[s]-done_q[s] > ParentDepth)')
s=one(s,"s = int'(s_ar_i.id[2]); n = int'(pa_q[s][2:0]);", "s = int'(s_ar_i.id[2]); n = p_index(pa_q[s]);")
s=one(s,"s = int'(child_owner_q); n = int'(done_q[s][2:0]);", "s = int'(child_owner_q); n = p_index(done_q[s]);")
s=s.replace('done_q[candidate_owner][2:0]', 'p_index(done_q[candidate_owner])')
s=one(s,"                    parent_q[s][n] <= '0; offset_q[s][n] <= 0;", "                    if (n < ParentDepth) begin\n                        parent_q[s][n] <= '0; offset_q[s][n] <= 0;\n                        returned_q[s][n] <= 0;\n                    end")
s=one(s,'                    returned_q[s][n] <= 0;\n                end', '                end')
s=one(s,'    logic [31:0] cycle_q;', '''    logic [31:0] cycle_q;
    function automatic int unsigned p_index(input logic [31:0] seq);
        return int'(seq % ParentDepth);
    endfunction
    initial begin
        if (!(ParentDepth == 1 || ParentDepth == 2 || ParentDepth == 4 || ParentDepth == 8))
            $fatal(1,"parent capacity supports 1/2/4/8 only");
    end''')
s=one(s,'            cycle_q <= cycle_q + 1;', '''            cycle_q <= cycle_q + 1;
            if (cycle_q[13:0] == 0 && cycle_q != 0 &&
                !offered_q && !busy_q && !candidate_valid &&
                s_ar_valid_i && !s_ar_ready_o)
                $display("POSTTL-STATE cycle=%0d depth=%0d input_id=%0d pa0=%0d done0=%0d pa1=%0d done1=%0d va0=%0d va1=%0d vw=%0d vr=%0d pw=%0d pr=%0d vowner=%0d powner=%0d vseq=%0d pseq=%0d got=%0d tail0=%0d tail1=%0d",
                    cycle_q, ParentDepth, s_ar_i.id, pa_q[0], done_q[0], pa_q[1], done_q[1],
                    va_q[0], va_q[1], vw_q, vr_q, pw_q, pr_q,
                    v_owner_q[vr_q[2:0]], p_owner_q[pr_q[2:0]],
                    v_seq_q[vr_q[2:0]], p_seq_q[pr_q[2:0]], v_got_q,
                    returned_q[0][p_index(done_q[0])], returned_q[1][p_index(done_q[1])]);''')
(OUT/'posttl_owner_fragment.sv').write_text(s)
t=(OLD/'system_top.sv').read_text()
t=one(t, '    parameter bit          NonblockEnable       = 1,', '    parameter int unsigned ParentDepth = 8,\n    parameter bit          NonblockEnable       = 1,')
t=one(t, 'posttl_owner_fragment #(.ar_t(slv_ar_chan_t), .r_t(slv_r_chan_t))', 'posttl_owner_fragment #(.ParentDepth(ParentDepth), .ar_t(slv_ar_chan_t), .r_t(slv_r_chan_t))')
(OUT/'system_top.sv').write_text(t)
for depth in [1,2,4]:
    a=json.loads((OLD/'build-command.json').read_text())
    a[a.index('--Mdir')+1]=str(OUT/f'obj-depth{depth}')
    a.insert(a.index('--top-module'),f'-GParentDepth={depth}')
    a[a.index(str(OLD/'posttl_owner_fragment.sv'))]=str(OUT/'posttl_owner_fragment.sv')
    a[a.index(str(OLD/'system_top.sv'))]=str(OUT/'system_top.sv')
    (OUT/f'build-depth{depth}-command.json').write_text(json.dumps(a,indent=2)+'\n')
