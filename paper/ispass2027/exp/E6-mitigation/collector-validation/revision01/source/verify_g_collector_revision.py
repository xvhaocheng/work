#!/usr/bin/env python3
"""Focused raw-tag/software-binding checks; explicit host fixture boundary."""
import argparse
from contextlib import redirect_stdout
from copy import deepcopy
import hashlib
import io
import json
from pathlib import Path
import sys
from unittest.mock import patch

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
import collect_g as collector
import g_records as records
from verify_g_collector import BOOT, fixture


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cli-report', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args(); out = a.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    checks = []
    top = {'schema':'ispass2027-g-focused-host-fixture-v1', 'fixture_only':True,
        'evidence_level':'synthetic_protocol_fixture', 'status':'running', 'hardware_executed':False,
        'context':'Production-loop negative tests mock only bindings and Console. Actual collect/save/raw guard/parse execute; no qualified BIT is fabricated.',
        'boundary_mock':{'bindings_return':'synthetic BOOT and bit string plus fixed backend counts; not a load/build receipt',
            'Console':'two synthetic raw commands; CLI generated from prior finite syscall fixtures, only raw-level tag on second command',
            'production_negative_outputs':'child manifests intentionally use fixture_only=false to exercise actual production guards; each is failed, enclosed by this fixture package, forbidden from paper exports'},
        'sources':{'cli_report':{'path':str(a.cli_report.resolve()), 'sha256':collector.sha(a.cli_report)},
            'original_100_checks':str((HERE/'run02/report.json').resolve()),
            'fixture_renderer_sha256':collector.sha(HERE/'verify_g_collector.py'),
            'formal_software_manifest_sha256':collector.SOFTWARE_MANIFEST}}
    collector.save(out/'fixture-context.json', top)  # Classify before any child output.
    old = HERE/'run02/inputs'; inputs = out/'fixture-inputs'; inputs.mkdir()
    paths = {}
    for name in ['load','fpga','backend']:
        path = inputs/(name+'.json'); path.write_bytes((old/(name+'.json')).read_bytes()); paths[name]=path
    original = (HERE.parent/'software-source/build01/build-manifest.json').read_bytes()
    sw = inputs/'software.json'; sw.write_bytes(original); paths['software']=sw
    assert collector.sha(sw) == collector.SOFTWARE_MANIFEST

    def reject_binding(label, expected, fixture_mode=False):
        try: collector.bindings(paths, fixture_mode)
        except ValueError as e:
            assert expected in str(e), (label, str(e)); checks.append({'case':label,'result':'rejected','reason':str(e)})
        else: raise AssertionError(label)

    reject_binding('exact formal software reaches existing synthetic-FPGA rejection', 'synthetic source')
    sw.write_bytes(original+b'\n')
    reject_binding('reformatted same software JSON fails exact original manifest binding', 'original formal software manifest')
    sw.write_bytes(original)
    # All source consistency checks also apply with explicit fixture=True.
    original_value = json.loads(original)
    mutations = [
        ('OpenSBI', lambda x:x.__setitem__('selected_opensbi','0'*40), 'OpenSBI/reference role'),
        ('reference role', lambda x:x.__setitem__('selected_cva6_role','loaded_bit'), 'OpenSBI/reference role'),
        ('prepared CLI source', lambda x:x['prepared_inputs'].__setitem__('bench/idma_g.c','0'*64), 'CLI prepared/built source'),
        ('built CLI source', lambda x:x['handwritten_inputs'].__setitem__('bench/idma_g.c','0'*64), 'CLI prepared/built source'),
        ('prepared CLI ELF', lambda x:x.__setitem__('g_cli_sha256','0'*64), 'CLI prepared/built source'),
        ('built CLI ELF', lambda x:x['artifacts'].__setitem__('bin/idma_g','0'*64), 'CLI prepared/built source'),
    ]
    for header in collector.UAPI:
        mutations += [
            ('prepared UAPI '+header, lambda x,h=header:x['prepared_uapi'].__setitem__('linux/'+h,'0'*64), 'UAPI source identity'),
            ('kernel UAPI '+header, lambda x,h=header:x['kernel_critical_sources'].__setitem__('include/uapi/linux/'+h,'0'*64), 'UAPI source identity')]
    for label, change, reason in mutations:
        value=deepcopy(original_value); value['fixture_only']=True; change(value)
        sw.write_text(json.dumps(value)+'\n')
        reject_binding('fixture still rejects '+label, reason, True)
    value=deepcopy(original_value);value['fixture_only']=True
    sw.write_text(json.dumps(value)+'\n')
    reject_binding('synthetic software never accepted without explicit fixture', 'original formal software manifest')
    collector.bindings(paths, True)
    checks.append({'case':'explicit fixture accepts synthetic software only with unchanged frozen identities', 'result':'passed'})
    cli=json.loads(a.cli_report.read_text())
    by_condition={c['record']['condition']:c['record'] for c in cli['cases']
        if c.get('mode')=='' and 'record' in c and c['record']['success'] and c['record']['run_attempted']}
    bursts={64:1,1024:1,4096:2}
    class Console:
        def __init__(self, marker=None):self.calls=0;self.closed=False;self.marker=marker
        def command(self, command, timeout):
            point=collector.plan()[self.calls]; self.calls+=1
            assert timeout==120 and command==collector.command(point)
            lines=fixture(by_condition[point['condition']], self.calls).splitlines()
            for i,line in enumerate(lines):
                if line.startswith('{'):
                    row=json.loads(line);row.pop('fixture_only');lines[i]=json.dumps(row)
            raw=dict(command=command,stdout='\n'.join(lines)+'\n',exit_code=0)
            if self.calls==2 and self.marker=='top':raw['fixture_only']=True
            if self.calls==2 and self.marker=='nested':raw['metadata']={'origin':{'evidence_level':'synthetic_protocol_fixture'}}
            return raw
        def close(self):self.closed=True
    # Verify explicit fixture execution labels manifest, all raw commands and
    # all records, even when the injected raw commands themselves have no tag.
    console=Console()
    with redirect_stdout(io.StringIO()):
        m=collector.collect(out/'explicit-fixture40', paths, Path('/NO_HARDWARE'),
            _factory=lambda path:console, _fixture=True)
    assert console.calls==40 and console.closed and m['fixture_only'] is True and m['status']=='passed'
    for path in (out/'explicit-fixture40').glob('*.json'):
        value=json.loads(path.read_text()); values=value if isinstance(value,list) else [value]
        assert all(x['fixture_only'] is True and x['evidence_level']=='synthetic_protocol_fixture' for x in values), path
    checks.append({'case':'explicit fixture marks manifest/all40 raw/all40 records', 'result':'passed'})
    # No invented production-qualified BIT/load manifest is supplied. The
    # bindings boundary alone is mocked, independently tested above. Child raw
    # and failed manifest originals are kept as executed under fixture-context.
    for marker in ['top','nested']:
        directory=out/('production-negative-raw-'+marker);console=Console(marker)
        with patch.object(collector,'bindings',return_value=({'boot_id':BOOT,'bit_sha256':'MOCK-NOT-A-BIT'},bursts)) as mocked_binding:
            with patch.object(collector,'parse',wraps=records.parse) as actual_parse:
                try:
                    with redirect_stdout(io.StringIO()):
                        collector.collect(directory, {}, Path('/NO_HARDWARE'), _factory=lambda path:console)
                except ValueError as e:assert 'synthetic raw command' in str(e)
                else:raise AssertionError('production accepted raw-only '+marker)
                assert actual_parse.call_count==1  # Second point never parsed.
            assert mocked_binding.call_args.args[1] is False
        m=json.loads((directory/'manifest.json').read_text())
        raw=json.loads((directory/'raw-02.json').read_text())
        assert m['status']=='failed' and m['recorded_cases']==1 and m['fixture_only'] is False
        assert console.calls==2 and console.closed and records.synthetic_markers(raw)
        assert not records.synthetic_markers(records.parse(raw['stdout'], collector.plan()[1]['condition'], bursts))
        checks.append({'case':'actual production loop rejects second raw-only '+marker+' tag before parse; raw retained',
            'result':'passed','command_calls':console.calls,'parse_calls':1,'failed_original_manifest':str(directory/'manifest.json')})
    top.update(status='passed', check_count=len(checks), checks=checks,
        source_sha256={p.name:collector.sha(p) for p in [HERE.parent/'collect_g.py',Path(__file__)]})
    collector.save(out/'fixture-context.json',top)
    print(json.dumps({'status':'passed','checks':len(checks),'fixture_context':str(out/'fixture-context.json')}))


if __name__=='__main__':main()
