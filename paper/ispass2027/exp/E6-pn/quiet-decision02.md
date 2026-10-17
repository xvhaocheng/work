# Root decision02：deferred DDTP writes are pending control work

2026-10-18 JST / ARS experiment-agent. Source-confirmed design decision, not
implemented or tested evidence. Codex-only. Root directly read actual fixed
IOMMU5b7 files from the isolated SW tree; relevant bytes equal original85b.

`rtl/software_interface/regmap/rv_iommu_regmap.sv:3185–3289` accepts DDTP low/high
writes while `in_flight_i=1`, stores `write_l_q/write_h_q` and values, and commits
when in_flight falls. At3198–3225, the pending q remains high before the actual
commit edge; the program bridge may already be empty and have returnedB. SW
wrapper150–167 has no existing output for those q. The DDTP busy field at645–683
is internal read data, not present in the current reg2hw DDTP interface. Thus
program quiet and existing nine-file SW predicates do not cover this state.
MODE quiet cannot substitute its own new meaning for the original in_flight.

Root accepts exactly one additional production file beyond the previously
accepted nine: regmap, appended default-off `PnEnable` and readonly `pn_idle_o`:

```text
PnEnable ? (rst_ni && !write_l_q && !write_h_q && !reg_req_i.valid) : 0
```

No new state, flush, functional feedback, fields or changed original assignments.
The existing SW regmap instance propagates PnEnable, collects this output and
ANDs it with its other real child idle facts. Do not gate original deferred
commit, original busy or change the old in_flight connection. No production
hierarchical reference. No top, package, frontend or constraint change here.
This is the necessary minimal readonly completion boundary within authorized
PN integration; no new experiment or mechanism is introduced.

Finite complete program+SW candidate/original gold must use real AXI CSR
requests to create pending low and high DDTP writes, retain in_flight1 after
programB retirement, then lower it. Verify nonquiet through pending/commit edge
and idle recovery after natural original commit, with all original outputs
equal every pre/post edge. Readonly test taps may report existing q as evidence;
no force or manual CSR injection. Preserve prior nine-file source proposal and
all failed attempts, update the actual10-file execution/commit identity. This
decision is not a claim that the expanded DUT has compiled or passed.
