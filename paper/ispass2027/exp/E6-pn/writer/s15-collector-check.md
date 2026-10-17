# S15 collector scheduling check

Root's collector is copied without modification to `../tools/collect_s15.py`.
SHA256: `288570159b9c11f973b2894bb04a81cc455c1847937870a181f1ce54ed6eee53`.
The exact original is also retained in `inputs/collect_s15.root-original.py`.
Accepted s15_gate, source-map and pn-control are unchanged.

One concentrated execution passed four scenarios /120 assertions:

- Success: pre-read,15 original fixed-order commands,post-read; one console and
  one close; sequential observer43..57 from pre-watermark42.
- First CLI failure at point4: exactly3 successful records remain, no retry,
  no later point or post-read, failed manifest, cleanup once.
- Cleanup failure after15 successes: all15 records and brackets remain, but
  manifest status becomes failed and the cleanup exception propagates.
- Actual early preflight rejection: invalid output scope fails before console
  construction, any command or output-directory creation.

All injected records, readbacks, manifests and reports explicitly carry
fixture/synthetic markers and source_board_cases0. The actual collector and
accepted gate readback routine run; CLI parsing uses a scheduling-only stub.
This does not repeat the gate51/parser matrix or qualify hardware identities.
No UART/device was opened. Actual S15 still has zero new board cases.

Evidence: `artifacts/s15-collector-host01/summary.json`, scenario-specific
`console-events.json`, saved raw command files, records/manifests and stdout.
Exact executed collector/gate/uart/test bytes are under its `source/`.
No production collector diff was necessary.

Reproduce into a new output (the dependency root provides accepted gate and
uart source bytes; its collector bytes must equal the copied candidate):

```text
python3 paper/ispass2027/exp/E6-pn/writer/tests/check_s15_collector.py --dependency-root /home/xvhaocheng/work-root-a --output /absolute/new/host-output
```
