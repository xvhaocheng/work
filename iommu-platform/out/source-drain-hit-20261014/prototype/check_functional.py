"""Fresh directed functionality, no timing/performance or old-model imports."""
import json
from pathlib import Path
from protocol import Protocol, ProtocolError, candidate_choose, n_read_permission, fullbuffer_permit
from page_swap import PageSwap, scratch_swap

HERE = Path(__file__).resolve().parent
results = []
traces = []


def record(name, action):
    action()
    results.append(dict(name=name, passed=True))


def rejects(action):
    try:
        action()
    except ProtocolError:
        return
    raise AssertionError('invalid transition was accepted')


def observation():
    return dict(dc_hit=True, dc_valid=True, pdtv=False, sxl=False, t2gpa=False,
                en_ats=False, en_pri=False, pid_valid=False, priv=False, debug=False,
                canonical=True, s1_sv39=True, s2_bare=True, ddtp_did_in_range=True,
                msi_input_safe=True, leaf_hit=True, leaf_valid=True, leaf_u=True,
                leaf_r=True, is_1G=False, is_2M=False, msi_leaf_safe=True,
                context_stable=True, leaf_ppn=0x20)


def fixture(choose=candidate_choose, roles=(0, 8), accepted_aw=True):
    p = Protocol(choose)
    a, b = roles
    p.add_job(a, 1, 0x10000, 0x30000, bytes(range(24)))
    p.add_job(b, 2, 0x20000, 0x40000, bytes(range(80, 104)))
    if accepted_aw:
        p.aw_accept(b, 2, 0x40000)
    ha = p.enqueue(a, 1, 0x1000, 24)
    hb = p.enqueue(b, 2, 0x2000, 24)
    p.start_normal(ha.tag)
    p.observe(hb, observation())
    return p, ha, hb


def deliver_r(p, h, response='OKAY'):
    r = p.inflight[h.tag]
    for i in range(0, h.size, 8):
        data = bytes(p.memory[r['pa'] + i + k] for k in range(8))
        p.r_accept(h.tag, h.source, data, i + 8 == h.size, response if i == 0 else 'OKAY')


def drain(p, h):
    for i in range(0, h.size, 8):
        p.w_accept(h.source, h.job, i + 8 == h.size)
    p.b_accept(h.source, h.job)
    return p.consume(h.source, h.job)


def full_chain(choose=candidate_choose, roles=(0, 8), error=False):
    p, a, b = fixture(choose, roles)
    grant = p.probe()
    assert grant['tag'] == b.tag
    # Directed READY stalls are functionality checks, not searched service delays.
    assert p.physical_ar(False) == grant
    p.normal_complete(0x10000)  # Existing normal TC register is independent.
    assert p.physical_ar(False) == grant
    assert p.normal == a and p.normal_result['tag'] == a.tag
    p.physical_ar(True)
    assert p.normal == a and p.slots[a.bank][a.slot] == a
    p.invalidate()  # Must not acknowledge until owned obligations finish.
    rejects(p.finish_invalidation)
    deliver_r(p, b, 'SLVERR' if error else 'OKAY')
    result_b, error_b = drain(p, b)
    assert result_b == bytes(range(80, 104)) and error_b == error
    p.aw_accept(a.source, a.job, 0x30000)
    packet = p.physical_ar(True)
    assert packet['tag'] == a.tag and packet['pa'] == 0x10000
    deliver_r(p, a)
    result_a, error_a = drain(p, a)
    assert result_a == bytes(range(24)) and not error_a and p.quiet()
    p.finish_invalidation()
    assert p.quiet() and not p.invalidation_pending
    assert p.counts == dict(ar=2, r_beats=6, r_bytes=48, aw=2, w_beats=6,
                            w_bytes=48, b=2, consumer=2, errors=int(error))
    traces.append(dict(kind='functional_only', roles=list(roles), error=error,
                       counts=p.counts, events=p.events, quiet=p.quiet()))
    return p


def exclusion_cases():
    p, a, b = fixture(accepted_aw=False)
    assert p.probe() is None  # VALID is not actual AW accept.
    p, a, b = fixture()
    p.lookup_normal_pending = True
    assert p.probe() is None
    p.lookup_normal_pending = False
    p.invalidate()
    assert p.probe() is None
    p, a, b = fixture()
    p.probe()
    assert p.probe() is None  # No second result slot.
    for field in ('leaf_hit', 'leaf_r', 'leaf_u', 'msi_input_safe', 'msi_leaf_safe', 'context_stable'):
        p, a, b = fixture()
        o = observation(); o[field] = False
        p.observe(b, o)
        assert p.probe() is None, field
    for field in ('en_ats', 'en_pri', 'priv', 'is_1G', 'is_2M', 'pid_valid'):
        p, a, b = fixture()
        o = observation(); o[field] = True
        p.observe(b, o)
        assert p.probe() is None, field
    p, a, b = fixture()
    s = p.state(); s['heads'][1]['job'] = 99
    assert candidate_choose(s) is None
    s = p.state(); s['heads'][1]['needed_for_head'] = False
    assert candidate_choose(s) is None
    s = p.state(); s['heads'][1]['oldest'] = False
    assert candidate_choose(s) is None


def ownership_errors():
    p, a, b = fixture()
    rejects(lambda: p.b_accept(b.source, b.job))
    rejects(lambda: p.consume(b.source, b.job))
    rejects(lambda: p.w_accept(a.source, a.job, False))
    p.probe(); p.physical_ar(True)
    rejects(lambda: p.owned(b.tag))
    # Reuse exact raw slot with a new generation, stale tag cannot target it.
    c = p.enqueue(b.source, b.job, 0x3000, 8, 16)
    assert c.slot == b.slot and c.generation != b.generation and not p.oldest(c)
    rejects(lambda: p.owned(b.tag))
    data = bytes(range(80, 88))
    rejects(lambda: p.r_accept(b.tag, a.source, data, False))
    rejects(lambda: p.r_accept(b.tag, b.source, data, True))
    rejects(lambda: p.r_accept(b.tag, b.source, bytes(8), False))
    deliver_r(p, b)
    # Small original source2 buffer is now full, not granted a 64B permit.
    assert len(p.buffers[b.source]) == 24
    assert not fullbuffer_permit(8, 64, 0x1000, 0x2000)
    for source in (0, 4, 8, 12):
        assert not fullbuffer_permit(source, 64, 0x1000, 0x2000)
        assert fullbuffer_permit(source, 4096, 0x1000, 0x2000) == (source in (0, 4))
    p, a, b = fixture()
    o = observation(); o['leaf_ppn'] = 0x99
    p.observe(b, o)
    rejects(p.probe)


def swap_chain():
    a = bytes((i * 17 + 3) % 256 for i in range(4096))
    b = bytes((i * 29 + 11) % 256 for i in range(4096))
    x = PageSwap(a, b, ('physical-A', 'physical-B'), epoch=5)
    rejects(lambda: x.aw_first_valid(0, 0, 5))
    x.read_burst(0, 0, 5)
    x.read_burst(1, 0, 5)
    rejects(lambda: x.aw_first_valid(0, 0, 5))  # One burst RLAST is not page receipt.
    x.read_burst(0, 1, 5)
    rejects(lambda: x.read_burst(1, 1, 6))
    x.read_burst(1, 1, 5)
    for burst in (0, 1):
        for source in (0, 1):
            x.aw_first_valid(source, burst, 5)
            x.write_burst(source, burst)
            x.b_accept(source, burst)
    for source in (0, 1):
        x.consume(source)
    assert bytes(x.memory[0]) == b and bytes(x.memory[1]) == a and x.quiet()
    assert x.counts == dict(ar=4, aw=4, r_bytes=8192, w_bytes=8192,
                            r_beats=1024, w_beats=1024, b=4, consumer=2)
    final, scratch = scratch_swap(a, b)
    assert final == (b, a)
    assert sum(e['r_bytes'] for e in scratch) == sum(e['w_bytes'] for e in scratch) == 12288
    traces.append(dict(kind='conditional_swap_functionality_only', counts=x.counts,
                       events=x.events, quiet=x.quiet(), scratch=scratch,
                       native_gate_and_frontend_execution=False))


if __name__ == '__main__':
    record('full_source_samebank_route_heldVALID_fullbytes_B_consumer_quiet', full_chain)
    record('full_source_role_exchange_functionality', lambda: full_chain(roles=(8, 0)))
    record('read_error_sticky_terminal_not_success_publication', lambda: full_chain(error=True))
    record('permission_AW_ID_job_lookup_invalidate_result_exclusions', exclusion_cases)
    record('wrong_owner_beats_generation_reuse_native_fullbuffer', ownership_errors)
    record('two_original_pages_value_exchange_before_first_AW_and_scratch', swap_chain)
    out = dict(status='pass', groups=results, traces=traces,
               evidence_scope='untimed private functional reference',
               native_rtl_run=False, real_cache_or_ptw_run=False,
               performance_conditions_run=0, old_experiments_run=0,
               timing_cycles=None, area=None, fmax=None, cpu_application_benefit=None)
    (HERE / 'functional-results.json').write_text(json.dumps(out, indent=2) + '\n')
    print(json.dumps(dict(status=out['status'], groups=len(results),
                         traces=len(traces), performance_conditions=0)))
