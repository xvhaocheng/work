#!/usr/bin/env python3
"""Retain the sourced objects; compile capacity-safe ordinary descriptors."""
import hashlib
import json
from pathlib import Path

BATCH = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    original = json.loads((BATCH / 'task-contract.json').read_text())
    register = json.loads((BATCH / 'segmented-g0-register.json').read_text())
    assert register['transport_revision']['buffer_capacity_bytes'] == 4096
    cases, generated = [], []
    for case in register['cases']:
        rows = []
        for role in case['offered_roles']:
            count = 0
            for tensor in original['tensors']:
                for offset in range(0, tensor['bytes'], 4096):
                    rows.append(dict(id=role*2309+count, role=role,
                        channel=0 if case['dispatch']=='single' else role,
                        src_iova=original['roles'][role]['source_iova']+tensor['offset']+offset,
                        dst_iova=original['roles'][role]['destination_iova']+tensor['offset']+offset,
                        bytes=min(4096,tensor['bytes']-offset), tensor=tensor['index'], ready=0))
                    count += 1
            assert count == 2309
        p = BATCH / f"segmented-commands-{case['id']}.tsv"
        columns = ['id','role','channel','src_iova','dst_iova','bytes','tensor','ready']
        with p.open('w') as f:
            f.write('\t'.join(columns)+'\n')
            for row in rows:
                f.write('\t'.join(str(row[k]) for k in columns)+'\n')
        generated.append(p)
        cases.append(dict(**case,commands=p.name,jobs=rows,
                          offered_useful_bytes=sum(row['bytes'] for row in rows),command_sha256=sha(p)))
    contract = dict(original)
    contract.update(schema=2,status='frozen before segmented build and execution',
        original_contract='task-contract.json',original_contract_sha256=sha(BATCH/'task-contract.json'),
        register_sha256=sha(BATCH/'segmented-g0-register.json'),
        transport=register['work']['transport'],descriptor_jobs_per_role=2309,
        source_tensors_per_role=6,maximum_descriptor_bytes=4096,
        transport_revision=register['transport_revision'],controller=register['ordinary_rules'],cases=cases)
    p=BATCH/'segmented-task-contract.json'
    p.write_text(json.dumps(contract,indent=2)+'\n');generated.append(p)
    manifest=dict(stage=106,scope='functional transport revision only; unchanged fixture data',
        files=[dict(path=str(p.relative_to(BATCH)),bytes=p.stat().st_size,sha256=sha(p)) for p in generated],
        payloads=[dict(path=f'rank{role}-payload.bin',sha256=sha(BATCH/f'rank{role}-payload.bin')) for role in [0,1]])
    (BATCH/'segmented-fixture-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(json.dumps(dict(cases=len(cases),descriptors_per_role=2309,source_tensors_per_role=6,
                         useful_bytes_per_role=9447936)))


if __name__=='__main__':
    main()
