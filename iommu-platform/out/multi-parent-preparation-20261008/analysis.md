# multi-parent-preparation summary

Run directory: `/home/xvhaocheng/work/iommu-platform/out/multi-parent-preparation-20261008`
Build completed: `True`
All runs DUT completed: `True`
All runs evidence accepted: `True`

## Runs

| run | DUT completed | evidence accepted | reuse | order | prep/order | V | A | mean | TL accept/resp | hit/miss | walk ddtw/ptw | filled | reused/prepared | future v/a | raw hw v/a |
|---|---|---|---:|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| lazy-v-32768 | yes | yes | 1 | 0 | 0/0 | 24084 | 27340 | 25712.0 | 66/66 | 32/34 | 2/102 | 32 | 32/0 | —/— | 4/4 |
| lazy-a-32768 | yes | yes | 1 | 0 | 0/0 | 27340 | 24084 | 25712.0 | 66/66 | 32/34 | 2/102 | 32 | 32/0 | —/— | 4/4 |
| prepare0-v-32768 | yes | yes | 2 | 0 | 1/0 | 24464 | 27275 | 25869.5 | 66/66 | 32/34 | 2/102 | 32 | 64/32 | 15/15 | 4/4 |
| prepare0-a-32768 | yes | yes | 2 | 0 | 1/0 | 27275 | 24464 | 25869.5 | 66/66 | 32/34 | 2/102 | 32 | 64/32 | 15/15 | 4/4 |
| prepare1-v-32768 | yes | yes | 2 | 1 | 1/1 | 24464 | 27275 | 25869.5 | 66/66 | 32/34 | 2/102 | 32 | 64/32 | 15/15 | 4/4 |
| prepare1-a-32768 | yes | yes | 2 | 1 | 1/1 | 27275 | 24464 | 25869.5 | 66/66 | 32/34 | 2/102 | 32 | 64/32 | 15/15 | 4/4 |
| prepare2-v-32768 | yes | yes | 2 | 2 | 1/2 | 24464 | 27275 | 25869.5 | 66/66 | 32/34 | 2/102 | 32 | 64/32 | 15/15 | 4/4 |
| prepare2-a-32768 | yes | yes | 2 | 2 | 1/2 | 27275 | 24464 | 25869.5 | 66/66 | 32/34 | 2/102 | 32 | 64/32 | 15/15 | 4/4 |

## V-first comparison against old V-first (matched issuer)

Old V-first baseline: V=17485 A=20163 mean=18824.0

## A-first comparison against old A-first (matched issuer)

Old A-first baseline: V=20163 A=17485 mean=18824.0

| mode | V-first V | V-first A | V-first mean | dV vs old V-first | dA vs old V-first | dMean vs old V-first | A-first V | A-first A | A-first mean | dV vs old A-first | dA vs old A-first | dMean vs old A-first | exact swap |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| lazy | 24084 | 27340 | 25712.0 | +6599 | +7177 | +6888.0 | 27340 | 24084 | 25712.0 | +7177 | +6599 | +6888.0 | True |
| prepare0 | 24464 | 27275 | 25869.5 | +6979 | +7112 | +7045.5 | 27275 | 24464 | 25869.5 | +7112 | +6979 | +7045.5 | True |
| prepare1 | 24464 | 27275 | 25869.5 | +6979 | +7112 | +7045.5 | 27275 | 24464 | 25869.5 | +7112 | +6979 | +7045.5 | True |
| prepare2 | 24464 | 27275 | 25869.5 | +6979 | +7112 | +7045.5 | 27275 | 24464 | 25869.5 | +7112 | +6979 | +7045.5 | True |

Best completed mode by measured V-first mean: `lazy` mean=25712.0 (no hardcoded victory).

## Declared-state ledger (separate; NOT measured area)

- PREP4 helper declared functional bits: 1281
- RAW4 declared functional bits: 3316
- Declared total: 4597 bits
- Reference post-window4-depth4+credit: 4602 bits
- Declared difference: -5 bits
- Declared only; not synthesized; no total-area equality and no 5-bit area-advantage claim.

## Failures / missing files

- None.

## Notes

- TL accept/resp count (66) is not the same thing as iOTLB hit (32) and is not the walker count.
- Walker evidence is separate: n_ddtw_ar=2, n_ptw_ar=102.
- NATURAL-SUMMARY var=64 is pre-TL child requests; POSTTL-STATS parents=32 is the original exclusive parent count.
- A-first is compared against the old A-first matched issuer; individual V/A deltas are not mixed with V-first.
- p and future-preparation counts are reported as measured; p=32 or future>0 is not required.
- No board/production/fault/epoch/invalidation claim is made.
- Normal-response field short cache does not authorize ignoring MRIF in production.
- Source-hash/production audit is handled by Codex and is not duplicated here.
