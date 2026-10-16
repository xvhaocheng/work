{
  "replacements": [
    {
      "old": "        row = {\"name\": name, \"e2_mode\": t[\"e2_mode\"], \"v_cold_dst\": w[\"v_cold_dst\"],\n               \"pool_capacity_bytes\": t[\"pool_capacity_bytes\"], \"payload_bytes\": t[\"private_payload_bytes\"],\n               \"cycles\": w[\"W_cycles\"], \"fast\": w[\"window\"][\"fast\"],\n               \"completed_v_per_cycle\": round(64.0 / w[\"W_cycles\"], 9)}",
      "new": "        private_payload_bytes = t[\"private_payload_bytes\"]\n        pool_capacity_bytes = t[\"pool_capacity_bytes\"]\n        row = {\"name\": name, \"e2_mode\": t[\"e2_mode\"], \"v_cold_dst\": w[\"v_cold_dst\"],\n               \"private_payload_bytes\": private_payload_bytes,\n               \"pool_capacity_bytes\": pool_capacity_bytes,\n               \"payload_bytes\": private_payload_bytes + pool_capacity_bytes,\n               \"cycles\": w[\"W_cycles\"], \"fast\": w[\"window\"][\"fast\"],\n               \"pn_cycles\": w[\"W_cycles\"], \"pn_fast\": w[\"window\"][\"fast\"],\n               \"completed_v_per_cycle\": round(64.0 / w[\"W_cycles\"], 9)}"
    },
    {
      "old": "        p_name = name.replace(\"-pn-\", \"-p-\")\n        if p_name in recs:\n            pc = recs[p_name][\"ACTUAL-DMA-WINDOW-JSON\"][\"W_cycles\"]\n            row[\"p_run\"], row[\"p_cycles\"] = p_name, pc\n            row[\"pn_vs_p_time_percent\"] = round(100.0 * (w[\"W_cycles\"] - pc) / pc, 6)",
      "new": "        p_name = name.replace(\"-pn-\", \"-p-\")\n        if p_name in recs:\n            pw = recs[p_name][\"ACTUAL-DMA-WINDOW-JSON\"]\n            pc = pw[\"W_cycles\"]\n            row[\"p_run\"], row[\"p_cycles\"] = p_name, pc\n            row[\"p_fast\"] = pw[\"window\"][\"fast\"]\n            row[\"pn_vs_p_time_percent\"] = round(100.0 * (w[\"W_cycles\"] - pc) / pc, 6)"
    },
    {
      "old": "    mode, e2, slots = w[\"mode\"], t[\"e2_mode\"], t[\"pool_slots\"]",
      "new": "    mode, e2, slots = w[\"mode\"], t[\"e2_mode\"], t[\"pool_slots\"]\n    expect_e2 = {\"bypass\": 0, \"ordinary\": 1, \"selective\": 2, \"reserved\": 3}.get(name.split(\"-\")[0])\n    if expect_e2 is None:\n        fail(name, \"run name prefix not in bypass/ordinary/selective/reserved: \" + name)\n    elif e2 != expect_e2:\n        fail(name, \"e2_mode %r != name-prefix expected %r\" % (e2, expect_e2))"
    },
    {
      "old": "        if t[\"pool_capacity_bytes\"] != 0 or t[\"private_payload_bytes\"] != 8192 or bad:",
      "new": "        if t[\"pool_slots\"] != 0 or t[\"dma_buffer_depth\"] != 512:\n            fail(name, \"mode0 pool_slots/dma_buffer_depth != 0/512\")\n        if t[\"pool_capacity_bytes\"] != 0 or t[\"private_payload_bytes\"] != 8192 or bad:"
    },
    {
      "old": "    man = load(os.path.join(OUT, \"previous-production-source-hashes.json\"))\n    changed, missing = [], []",
      "new": "    man = load(os.path.join(OUT, \"previous-production-source-hashes.json\"))\n    if not man:\n        fail(\"sources\", \"previous-production-source-hashes.json is empty or has no entries\")\n        return 0, [], []\n    if len(man) != 76:\n        fail(\"sources\", \"source manifest entries %d != 76\" % len(man))\n    changed, missing = [], []"
    }
  ],
  "notes": "1) payload_bytes is now private_payload_bytes + pool_capacity_bytes, with explicit private_payload_bytes and pool_capacity_bytes kept as separate columns. 2) Row now always carries pn_cycles and pn_fast (duplicating existing cycles/fast); p_fast/p_cycles/p_run are only set when the -p- run actually exists in recs, so no P baseline is invented. 3) check_run now derives expect_e2 from the first dash-delimited name token and fails on either an unknown prefix or a mismatch; the mode0 pool_slots==0 and dma_buffer_depth==512 requirement is added alongside the existing capacity/private/zero-counter and calibration checks (which are untouched). 4) check_sources fails and returns 0, [], [] on an empty manifest, and records a failure when len(man) != 76; non-empty-but-short manifests are still hashed after the failure is recorded. Uncertainty: I assume every name passed to check_run begins with exactly bypass/ordinary/selective/reserved (the 13-run set and PN_ORDER differ by one -p- variant, but that does not affect prefix mapping); if any run name does not, this will now report a failure rather than silently pass. Enforcing n_man==76 inside check_sources relies on the existing fail()/overall_pass plumbing; main still unpacks n_man unchanged. No executions were performed; these are unverified proposed replacements."
}
