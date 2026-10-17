#!/usr/bin/env python3
"""P0 OBS4/PATH validation body reused mechanically for fixed G COPY4096.

Only the old requirement for A-positive traffic/overlap is replaced: a G lane
may finish before the V window. G whole/window accounting is checked separately.
"""
def require(value, message):
    if not value: raise ValueError(message)


def validate(item, bursts, has_a):
    n, length = 4096, 4096
    obs, paths = item['observer'], item['path']
    require(set(obs) == {'time', 'axi_v', 'axi_a'}, 'P0 incomplete observer parts')
    t, v, a = obs['time'], obs['axi_v'], obs['axi_a']
    require(t['valid'] == 1 and t['status'] == 4 and t['batch_polled'] == 1
            and t['issued'] == t['done'] == n and t['cycles'] > 0
            and t['cycles'] == t['h'] + t['gap'] and t['gap'] == n-1,
            'P0 invalid hardware window/task accounting')
    require(t['v_did'] == 10 and t['a_did'] == (11 if has_a else 0xffffffff), 'P0 observer role mismatch')
    require(sum(v[k] for k in ('neither', 'v_only', 'a_only', 'both')) == t['cycles'], 'P0 activity partition mismatch')
    transactions = n*bursts[length]
    require(v['ar'] == v['aw'] == v['rlast'] == v['b'] == transactions
            and v['start'] == v['end'] == 0, 'P0 victim AXI count mismatch')
    for lane in (v, a):
        require((lane['start'] >> 16) + lane['ar'] == lane['rlast'] + (lane['end'] >> 16)
                and (lane['start'] & 65535) + lane['aw'] == lane['b'] + (lane['end'] & 65535),
                'P0 private AXI stock/flow mismatch')
    # G A whole completion is independent of its V-window overlap. The raw
    # window may be empty even though all 4096 A COPYs completed before V.
    if not has_a:
        require(v['both'] == v['a_only'] == a['ar'] == a['aw'] == 0,
                'G Solo aggressor activity mismatch')
    expected = {role+channel+str(stage) for role in 'va' for channel in 'rw' for stage in range(4)}
    require(set(paths) == expected, 'P0 requires 16 matched PATH records')
    for key, path in paths.items():
        require(all(isinstance(x, int) and x >= 0 for x in path.values()), 'P0 invalid PATH counter')
        require(path['valid'] == 1 and path['start'] + path['entered'] == path['full'] + path['carry'] + path['end'],
                'P0 PATH stock/flow mismatch')
        require(path['carry'] <= path['start'] and path['full'] <= path['entered']
                and path['max'] <= path['sum'] <= path['full']*path['max'] and path['area'] >= path['sum'],
                'P0 PATH latency accounting mismatch')
        if int(key[-1]) < 3:
            require(path['full'] + path['carry'] == paths[key[:-1]+str(int(key[-1])+1)]['entered'],
                    'P0 PATH stage continuity mismatch')
        if key.startswith('v'):
            require(path['start'] == path['end'] == path['carry'] == 0
                    and path['entered'] == path['full'] == transactions and path['area'] == path['sum'],
                    'P0 victim PATH window mismatch')
        elif not has_a:
            require(all(path[k] == 0 for k in ('entered', 'full', 'carry', 'sum', 'max', 'start', 'end', 'area')),
                    'P0 Solo aggressor PATH nonempty')
    # Stage0 leave is exactly the shared AR/AW handshake event. PATH0 enter is
    # first offer, not private accept: do not equate their opening/closing stock.
    for channel, shared in (('r', 'shared_ar'), ('w', 'shared_aw')):
        leave = sum(paths[role+channel+'0']['full'] + paths[role+channel+'0']['carry'] for role in 'va')
        require(a[shared] == leave, 'P0 shared/PATH0 leave mismatch')
