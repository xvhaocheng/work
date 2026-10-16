# RCS1 frozen v1: completion-set-aware IOTLB insertion / replacement

2026-10-14; root-authored mechanism. Single normal PTW, original 16 ways, original queues and data ownership guards. This is a new decision at successful **full walk completion**, not a reopening of CO1/R2 or a saved translation pool. No new permission, cancellation, parallel walk or data admission action.

## Question and condition

Per-key reference protection treats every future user alike. Does preserving more whole remaining translation sets improve the **complete fixed work episode**, after the cost of exact task/key registration, result delivery and selection? E2 replacement / first reuse and E3 local waiting are inherited evidence; they do not answer this new hypothesis.

Exact trusted registration is paid input. `U[t]` is the set of formal keys for which task t still requires at least one shared lookup. A private permission already delivered for a request removes that request, not all future same-page uses. A key leaves U only after the last R/W lookup is proven to have completed its native handoff. Data/consumer/buffer retirement remains separate. Registered task boundaries and real data dependencies are frozen; dividing tasks to inflate a score is forbidden.

## Legal point and program

After a successful final full walk, retain the native request owner/context and validated result. Error / invalidate / stale-generation processing wins. The current accepted request receives the same ordinary permission checking and physical address formation regardless of whether the cache inserts. Score the snapshot after accounting for that one permission's proven final shared-lookup use (if any); no other consumer permission is fabricated.

For every one of the 16 legal victim ways w and one bypass action:

```
S[action] = actual resident valid-current formal keys after that action
missing[t, action] = U[t] minus S[action]
complete[action] = count of live registered tasks with nonempty U[t]
                   and missing[t, action] empty
union_missing[action] = cardinality(union over live t of missing[t, action])
```

Choose greatest complete, then least union_missing, then insertion over bypass. Equal-scoring victim ways use the original masked tree-PLRU. Empty-U tasks are already translation-retired and do not contribute a constant fake completion count. Superseded/error/canceled tasks do not count as successful consumers. No active native data transfer waits for U to become all resident. Union_missing is neither a PTE count nor a DDR latency estimate.

All normal insertion hits retain the original PLRU touch behavior; bypass has no resident-way touch. All comparison programs receive identical registration, exact remaining-use feedback, resident state, full-walk direct-result path and native capacity. No future PTE contents, hidden DDR state or offline next-use oracle is input.

## Minimal finite boundary

Four trusted task slots; 18 formal keys in this new finite paper/model contract, versus physical cache16. CO1's <=16-key contract cannot supply this witness: if all registered keys including incoming fit in16, some zero-reference victim can preserve every resident set. The two extra registered keys and all ports/software are paid; this is not a 17th translation entry. No native 64B HOLD is enlarged into a free multi-page scheduler.

## Full result forwarding is ordinary and paid

Current normal TL finishes PT through cache update **and hit**, not merely update_o.update. Mask zero alone cannot complete a normal miss. The bounded conditional model must add the same ordinary full-result forwarding path to every rule. A production design would latch full successful content/request epoch, reuse native permission/address logic, suppress new PTW init while selection/result is pending, hold the complete response stable until accepted, and respect invalidation/error priority. This path is not the proposed novelty, is not implemented RTL, and is not free storage/latency.

## Gate and verdict

Enter at most four new fixed-work conditions only after a concrete difference from frozen PLRU, per-key pin/OC1-min, minimum distinct-task reference count, and whole-task oldest / shortest-missing / fraction rules, a paid ownership interface and a cold-prefix reachability construction. No old experiment or trace is replayed. A selection difference does not imply novelty or speedup. Full task completion (including producer publication, R/W/B, consumer receipt, owner release, quiet), actual model bytes and PTE services, policy latency and unmeasured physical costs must remain visible. A conditional DDR result is not FPGA or GPU application evidence.

If full benefit fails against a concrete ordinary rule, close this frozen performance hypothesis; keep the rule and exact counterexample. Do not automatically transform it into another candidate or change the paper positioning.

## Entry snapshot and paid selection timing

Full-cache resident keys a,b,p0..p13; q successful fullwalk belongs to T0's last q lookup. After prospectively accounting for this one result, U0=U1={p0..p13,r}, U2={a,q}, U3={b,q}; r is still absent. T0 oldest. RCS1 evicts p: two small-task sets complete, big sets already incomplete. Min-ref evicts a/b (one user versus two p users), completing only one small set. Oldest/fraction protect p; shortest-missing chooses U2 and protects a. This is the second-missing-key conjunctive difference. The withdrawn 17key draft lacked r; a concrete minimum-task-reference rule covers its one-missing-key family. No experiment ran that draft.

A paid one-tick choice pipeline is used by every rule in the conditional model, assuming its logic meets that model clock. Actual frequency / combinational timing is unmeasured, so no hardware net gain is inferred. Root adopts the exact minimal algebra below rather than forcing17 virtual caches or17 serial ticks. Full-result holder, generation/snapshot, fanout, logic and ports remain paid. If validity/epoch or task-use generation changes while the choice is pending, discard the hint and apply ordinary current PLRU; hits/data keep progressing.

## Exact cheaper representation of the same rule

Let R be resident keys, q the newly completed absent key, M=(union U)-R. Define E_q={live t with nonempty U_t and (U_t-R) subset of {q}}. Then for any valid victim v:

- complete_insert(v)=|E_q|-popcount(users(v) AND E_q).
- union_missing_insert(v)=|M minus {q}| + 1[users(v) nonempty].
- complete_bypass=|{live t: nonempty U_t and U_t subset of R}|; union_missing_bypass=|M|.

Invalid ways have users(v)=0 and no victim key. This exactly implements all17 logical action scores with one eligible-task mask and filtered reference counts. A per-key bitmap alone cannot supply E_q without the current whole-task missing relation; min blind refs is covered only in the specified one-missing-key subfamily. This algebra is root/worker design simplification, not a claim that an existing publication implements the exact program.
