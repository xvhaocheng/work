# Exact finite information diagnostic

This analysis consumes completed model rows without executing the model. `evaluate.py` uses exact rational arithmetic and uniform weight on the 35 distinct `(k,r)` tuples, retaining repeated terminal states. It reports each public configuration independently.

O0 is exactly `(public_config, reference.O0_events)`, including all timestamped event fields. Private prehistory and provider state appear only in labeled diagnostic output. Z consists of the three prior reference counters and their three overflow flags. Future outcomes and debug fields cannot enter observation keys.

The registered ordinary action set is both modes crossed with waits 0,1,2,3. The report contains fixed P0/PN0, the best fixed action, full-menu exact O0 and O0+Z offline bounds, and a separate paid-Z wait2/3 bound. Wait2/3 already charges the proposed read+decision delay in its recomputed trajectory; no constant fee is subtracted. Delay cooling and pure information value are distinct.

`software-checks.txt` records mathematical diagnostic software tests; these are not research results. The independent Cartesian control has two explicitly separate forms: mathematical resampling of existing target vectors, and (when `--negative-controls` is supplied) actual simulated independent-refresh future trajectories crossed with every old prehistory. Neither is a physical reset experiment.

Run only after complete inputs are declared ready:

```
python3 evaluate.py ../model/worlds.json --negative-controls ../model/negative-controls.json --output results.json
```

All values are offline finite-grid synthetic-model bounds. They establish no deployed inference cost, natural workload distribution, native performance, or novelty. No policy fitting is performed. A zero full-menu information bound stops fitting/grid expansion under the root contract; positive bounds require a further root decision.

Requested seat configuration: gpt-6.1-sol / medium. Actual runtime metadata: unknown. Authorship/execution: Codex-only bounded diagnostic implementation.
