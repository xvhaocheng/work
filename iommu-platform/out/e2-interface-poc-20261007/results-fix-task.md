Focused correction against exact supplied acceptance-compact-proposal.py. Return JSON {replacements:[{old:<uniqueexactsnippet>,new:<replacement>}],notes}, not whole script or analysis. Four corrections:
1. row["payload_bytes"] currently t["private_payload_bytes"], which incorrectly reports48 for nonzero modes. Make payload_bytes=private_payload_bytes+pool_capacity_bytes and optionally explicit private_payload_bytes. Keep pools separatetoo.
2. When Ppair present, include P FAST in same row e.g p_fast, and explicitly pn_fast/cycles fields consistently (existingcycles andfast mayremain). Keep baselineP unavailable ratherthaninvent.
3. check_run must confirm e2 equalsname-prefix mapping bypass0ordinary1selective2reserved3; mode0 require pool_slots0 anddepth512, existingcapcounterscalibration checksstay.
4. check_sources result n_man mustequal76 orfail; rejectempty/shortmanifest ratherthansucceedemptyloop.
No otherscope/scans/compatframework, no output hardcodedtable. Existingstylecondensedfine. Do not execute/write files, no new rerun. Explain anyuncertainity succinctly.
