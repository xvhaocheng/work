"""New LR1 functionality only. No old imports/models, cycles or performance."""
import hashlib
import importlib.util
import json
from pathlib import Path
from reclaim_protocol import Engine, ProtocolError, candidate_step

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('lr1_independent_ordinary', HERE.parent/'controls/ordinary_policy.py')
ordinary = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ordinary)
traces = []
checks = []


def payload(seed):
    return bytes((i*17+seed) % 256 for i in range(4096))


def check(ok, name):
    checks.append(dict(name=name, passed=bool(ok)))
    assert ok, name


def reject(call, name):
    try:
        call()
    except ProtocolError:
        check(True, name)
    else:
        check(False, name)


def fixture(roles=(0,4), shared=False):
    p = Engine(target_source=roles[0])
    a = p.add_job(roles[0], 11, 20, 1, 0x10000, 0x20000, payload(3))
    b = p.add_job(roles[1], 22, 20 if shared else 21, 1,
                  0x10000 if shared else 0x30000, 0x40000, payload(71))
    p.aw_accept(a,0); p.aw_accept(a,1)
    return p,a,b


def read(p,j,burst,error=False):
    p.offer_ar(j,burst,True)
    p.receive_burst(j,burst,error)


def drive(p, policy=candidate_step):
    actions = []
    for _ in range(10):
        action = policy(p.snapshot())
        actions.append(action)
        if action == 'wait':
            break
        p.execute(action)
        if action in ('ack','error'):
            break
    return actions


def finish_write(p,j,already_aw=False):
    if not already_aw:
        p.aw_accept(j,0); p.aw_accept(j,1)
    for burst in (0,1):
        p.write_burst(j,burst)
        p.b_accept(j,burst)
    p.consume(j)


def full(policy=candidate_step, roles=(0,4), error=False):
    p,a,b = fixture(roles)
    first = p.offer_ar(a,0,False)
    check(p.offer_ar(a,0,True)==first, 'held physical AR0 stable')
    p.receive_burst(a,0,error)
    read(p,a,1)
    p.offer_ar(b,0,True)  # Unrelated old AR is accepted, not globally drained.
    p.request_reclaim(1)
    initial = policy(p.snapshot())
    if initial != 'wait':
        p.execute(initial)
    p.receive_burst(b,0)
    p.offer_ar(b,1,True)
    actions = [initial]+drive(p,policy)
    ack_before_target_B = p.released and not any(x[0]==a.source for x in p.b)
    peer_inflight_at_ack = p.released and b.read==2048 and bool(p.inflight)
    if policy is ordinary.whole_copy and not error:
        check(not p.released, 'whole-copy has no early release')
    elif error:
        check(p.quarantined and not p.released, 'read failure quarantine, no successful release')
        reject(lambda:p.reuse_source(payload(99)), 'error source never reused')
    else:
        check(ack_before_target_B and peer_inflight_at_ack, 'phase ACK with W/B and unrelated R pending')
        p.reuse_source(payload(99))
        check(not p.stale_read_allowed(a), 'old source authority no longer grants reads')
        check((b.context,b.src_iova) in p.cache, 'unrelated cached mapping preserved')
    finish_write(p,a,True)
    if policy is ordinary.whole_copy and not error:
        actions += drive(p,policy)
        check(p.released and b.read==2048, 'wholecopy target release does not wait unrelated read')
        p.reuse_source(payload(99))
    p.receive_burst(b,1)
    finish_write(p,b)
    if not error:
        check(bytes(p.memory[a.dst])==payload(3), 'old destination retains old data after source overwrite')
        check(bytes(p.memory[a.src])==payload(99), 'source contains new producer generation')
    else:
        check(a.error and bytes(p.memory[a.dst])==bytes(4096), 'error sink is not successful payload')
    check(bytes(p.memory[b.dst])==payload(71), 'unrelated destination complete')
    check(p.quiet(), 'all accepted data/lease/consumer obligations quiet')
    check(p.counts['r_bytes']==p.counts['w_bytes']==8192, 'full transport bytes')
    check(p.counts['ar']==p.counts['aw']==p.counts['b']==4, 'exact4AR4AW4B')
    check(p.counts['w_strobed_bytes']==(4096 if error else 8192), 'successful W bytes separated from error sink traffic')
    check(p.counts['consumer']==2 and p.counts['b_errors']==0, 'real B statuses and consumers')
    t=dict(policy=policy.__name__, roles=list(roles), error=error, actions=actions,
           ack_before_target_B=ack_before_target_B, unrelated_inflight_at_phase_ack=peer_inflight_at_ack,
           counts=p.counts, events=p.events, quiet=p.quiet(), source_released=p.released,
           source_quarantined=p.quarantined, target_data_sha256=hashlib.sha256(p.memory[a.dst]).hexdigest(),
           new_source_sha256=hashlib.sha256(p.memory[a.src]).hexdigest(), timing_cycles=None,
           native_execution=False)
    traces.append(t)
    return t


def pending_and_held():
    p,a,b = fixture()
    read(p,a,0)
    packet=p.offer_ar(a,1,False)
    p.request_reclaim(1); p.execute('seal')
    check(p.snapshot()['old_obligations']==1 and drive(p)==['wait'], 'held AR still owned after seal')
    check(p.offer_ar(a,1,False)==packet, 'seal cannot withdraw held VALID/data')
    p.offer_ar(a,1,True); p.receive_burst(a,1)
    rows=[p.add_old_ticket(kind) for kind in ('raw','walk','hit_OUT','held_translation','ingress')]
    check(drive(p)==['wait'], 'every old pipeline class blocks withdrawal')
    for row in rows:
        p.retire_ticket(row)
    check(drive(p)==['revoke','sync','invalidate','order','ack'], 'old pipeline join then exact withdrawal')
    # Parent-first seal must allow still-unissued committed second interval.
    p,a,b=fixture();read(p,a,0);p.request_reclaim(1);p.execute('seal')
    check(drive(p)==['wait'] and p.snapshot()['old_obligations']==1, 'unissued accepted-parent interval remains debt')
    read(p,a,1)
    check(drive(p)[-1]=='ack', 'seal does not deadlock accepted4096B parent')


def qualifiers_and_order():
    for field in ('registry_complete','alias_safe','readonly','nonoverlap'):
        p,a,b=fixture();read(p,a,0);read(p,a,1);p.request_reclaim(1)
        setattr(p,field,False)
        check(drive(p)==['wait'] and not p.released, 'negative qualification:'+field)
        reject(lambda:p.execute('ack'), 'unchecked qualification rejects direct ACK:'+field)
    p,a,b=fixture();read(p,a,0);read(p,a,1);p.request_reclaim(99)
    check(drive(p)==['wait'], 'stale mapping generation rejected')
    p,a,b=fixture();read(p,a,0);read(p,a,1);p.request_reclaim(1);p.execute('seal')
    for action in ('sync','invalidate','order','ack'):
        reject(lambda action=action:p.execute(action), 'withdrawal ordering rejects:'+action)
    for action in ('revoke','sync','invalidate'):
        p.execute(action)
    reject(lambda:p.execute('ack'), 'cache invalidate is not target ordering proof')
    p.execute('order');p.execute('ack')
    reject(lambda:p.offer_ar(a,0,True), 'completed old parent cannot offer undeclared reread')


def registered_readers_and_alias():
    p,a,b=fixture(shared=True);read(p,a,0);read(p,a,1);read(p,b,0)
    p.request_reclaim(1)
    check(drive(p)==['seal','wait'], 'remaining registered reader prevents source return')
    read(p,b,1)
    check(drive(p)[-1]=='ack', 'all registered readers true receipts allow return')
    p.reuse_source(payload(99));finish_write(p,a,True);finish_write(p,b)
    check(bytes(p.memory[a.dst])==bytes(p.memory[b.dst])==payload(3), 'both old readers retain original source')
    check(p.quiet(), 'shared-reader jobs and source receipt quiet')
    p,a,b=fixture()
    # Different formal context, same physical object is an unresolved alias.
    b.src=a.src
    p.pte[b.context,b.src_iova]=dict(pa=a.src,epoch=1,read=True,write=False)
    read(p,a,0);read(p,a,1);read(p,b,0);read(p,b,1);p.request_reclaim(1)
    check(not p.snapshot()['alias_safe'] and drive(p)==['wait'], 'samePA differentcontext is not invisible alias')
    p,a,b=fixture();b.dst=a.src;read(p,a,0);read(p,a,1);p.request_reclaim(1)
    check(not p.snapshot()['readonly'] and drive(p)==['wait'], 'other physical writer invalidates readonly source contract')


if __name__=='__main__':
    candidate=full()
    for policy in (ordinary.phased,ordinary.targeted_drain):
        o=full(policy)
        check(o['events']==candidate['events'] and o['counts']==candidate['counts'],
              'independent '+policy.__name__+' entire same adapter sequence')
    full(ordinary.whole_copy)
    swapped=full(roles=(4,0))
    swapped_o=full(ordinary.phased,roles=(4,0))
    check(swapped['events']==swapped_o['events'], 'source role exchange ordinary coverage')
    failed=full(error=True);failed_o=full(ordinary.phased,error=True)
    check(failed['events']==failed_o['events'], 'error terminal ordinary coverage')
    pending_and_held();qualifiers_and_order();registered_readers_and_alias()
    result=dict(status='pass', directed_checks=len(checks), checks=checks, traces=traces,
                evidence_scope='new untimed private ownership/value reference',
                group_names=['source return before W/B with unrelatedR','independent ordinary complete sequences',
                             'whole-copy comparison','source role exchange','readerror quarantine/full terminal',
                             'heldVALID/oldpipeline/unissued-parent drain','alias/registry/epoch/withdrawal ordering',
                             'all registered readers and reused-source payload integrity'],
                performance_conditions=0, old_experiments_run=0, native_cache_PTW_RTL_DDR_run=False,
                area=None,Fmax=None,CPU_application_benefit=None,measured_PTE_requests=None,
                independent_policy_sha256=hashlib.sha256(Path(spec.origin).read_bytes()).hexdigest())
    (HERE/'functional-results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(status='pass', directed_checks=len(checks), groups=8,traces=len(traces),performance_conditions=0)))
