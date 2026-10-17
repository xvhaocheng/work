#!/usr/bin/env python3
"""Export and actually exercise a bounded, labeled offline analysis replay.

Copies an explicit data/control whitelist only. No hardware, synthesis, simulator,
publisher PDF, credential, event log, or installed environment is exported.
"""
from __future__ import annotations

import argparse
import csv
import difflib
import getpass
import gzip
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tarfile
import tempfile
import time

ROOT = Path(__file__).resolve().parents[3]
PAPER = ROOT / 'paper/ispass2027'
PREFIX = Path('paper/ispass2027')
CONTROLS = ['analysis/build.sh', 'analysis/requirements.txt'] + [
    'analysis/scripts/' + name for name in (
        'test_analysis.py', 'baseline_extra.py', 'statistics_common.py',
        'merge_results.py', 'data_adapter.py', 'plot_figures.py',
        'write_figure_notes.py', 'validate_outputs.py')]
EXTRA_INPUTS = [
    'data/invalidation_sim_results.csv', 'analysis/history-number-inputs.json',
    'analysis/history-composition-inputs.json',
    'exp/E7-capacity-sim/results.csv', 'exp/E7-capacity-sim/analysis-import.json']
SENSITIVE_KEYS = {'target_serial', 'jtag_serial', 'serial', 'hostname', 'host',
                  'board_host', 'ssh_host', 'username', 'user'}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def saved(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, ensure_ascii=False, allow_nan=False) + '\n')


def read_json(path):
    return json.loads(path.read_bytes())


def numeric_leaves(value, pointer=''):
    """Every JSON numeric/bool value must survive identifier-only redaction."""
    result = {}
    if isinstance(value, dict):
        for key, item in value.items():
            result.update(numeric_leaves(item, pointer + '/' + str(key)))
    elif isinstance(value, list):
        for index, item in enumerate(value):
            result.update(numeric_leaves(item, pointer + '/' + str(index)))
    elif isinstance(value, (int, float, bool)):
        result[pointer] = value
    return result


class Deriver:
    def __init__(self):
        self.changes = []
        self.username = getpass.getuser()
        # Only known development roots and the current local account are scrubbed.
        self.roots = sorted({str(ROOT), str(ROOT.parent / 'work'),
                             str(ROOT.parent / 'work-root-a')}, key=len, reverse=True)

    def text(self, text, *, locator):
        old = text
        text = re.sub(r'(?:https?://|git@)github\.com[:/]' + re.escape(self.username) + r'/[A-Za-z0-9_.-]+',
                      'SOURCE_REPOSITORY_REDACTED', text)
        for root in self.roots:
            text = text.replace(root, 'ORIGINAL_DEVELOPMENT_ROOT')
        text = text.replace('/home/' + self.username, 'ORIGINAL_ACCOUNT_ROOT')
        text = text.replace(self.username, 'artifact-user')
        # In this input class dotted IPv4 strings are host metadata, not numbers.
        def ipv4(match):
            return 'HOST_ADDRESS_REDACTED' if all(int(x) <= 255 for x in match[0].split('.')) else match[0]
        text = re.sub(r'(?<![\w.])(?:\d{1,3}\.){3}\d{1,3}(?![\w.])', ipv4, text)
        if old != text:
            self.changes.append(dict(locator=locator, original=old, derived=text, rule='local_identifier_string_redaction'))
        return text

    def value(self, value, *, locator=''):
        if isinstance(value, dict):
            result = {}
            for key, item in value.items():
                child = locator + '/' + key
                if key.lower() in SENSITIVE_KEYS and isinstance(item, str) and item:
                    replacement = 'DERIVED_REDACTED_' + key.upper()
                    self.changes.append(dict(locator=child, original=item, derived=replacement,
                                             rule='explicit_board_or_account_identifier'))
                    result[key] = replacement
                else:
                    result[key] = self.value(item, locator=child)
            return result
        if isinstance(value, list):
            return [self.value(item, locator=locator + '/' + str(i)) for i, item in enumerate(value)]
        if isinstance(value, str):
            return self.text(value, locator=locator)
        return value


def adapt_script(relative, text):
    """Only path serialization and input-boundary changes; no statistics edits."""
    if relative.endswith('/data_adapter.py'):
        anchor = "\n\ndef stable_bytes(path):\n"
        if text.count(anchor) != 1:
            raise ValueError('data_adapter path adaptation anchor changed')
        helper = '''

REPLAY_ROOT = Path(__file__).resolve().parents[4]


def replay_label(path):
    return Path(path).resolve().relative_to(REPLAY_ROOT).as_posix()
'''
        text = text.replace(anchor, helper + anchor, 1)
        text = text.replace('    path = Path(path)\n    before = path.stat()',
                            '    path = Path(path)\n    path.resolve().relative_to(REPLAY_ROOT)  # package input boundary\n    before = path.stat()', 1)
        for old in ('str(Path(path).resolve())', 'str(path.resolve())', 'str(path)', 'str(root)', 'str(audit_path)'):
            text = text.replace(old, 'replay_label(' + ('root' if old == 'str(root)' else 'audit_path' if old == 'str(audit_path)' else 'path') + ')')
    elif relative.endswith('/plot_figures.py'):
        old = "TRACE[-1]['source_data']['file']=str(p)"
        new = "TRACE[-1]['source_data']['file']=p.relative_to(PAPER.parent.parent).as_posix()"
        if text.count(old) != 1:
            raise ValueError('figure source-path adaptation anchor changed')
        text = text.replace(old, new, 1)
    return text


GUARD = '''"""Runtime guard for this data-only replay; configured paths are not bundled."""
import os
from pathlib import Path
import sys

root = Path(os.environ['REPLAY_ROOT']).resolve()
tool = Path(os.environ['REPLAY_TOOL_ROOT']).resolve()
allowed = [root, tool, Path('/usr'), Path('/lib'), Path('/lib64'),
           Path('/etc/fonts'), Path('/etc/localtime'), Path('/var/cache/fontconfig'),
           Path('/dev/null'), Path('/dev/urandom'), Path('/proc')]

def audit(event, args):
    if event in ('socket.connect', 'socket.connect_ex', 'socket.getaddrinfo'):
        raise RuntimeError('Offline replay forbids network access')
    if event == 'open' and args and isinstance(args[0], (str, bytes, os.PathLike)):
        path = Path(os.fsdecode(args[0])).resolve()
        if not any(path == prefix or path.is_relative_to(prefix) for prefix in allowed):
            raise RuntimeError('Offline replay forbids non-package input: ' + str(path))

sys.addaudithook(audit)
'''


README = '''# DERIVED_REDACTED offline analysis replay

This archive reconstructs published numerical analysis and ten figure slots from
saved data. It runs no board collector, FPGA tool, RTL simulator, or network client.
Original evidence is unchanged outside this package. Local paths, account/host
identifiers and JTAG serial strings in selected copies have been redacted; the
package is explicitly a derivative, not an original-byte raw-data archive.

Keep the paper/ispass2027 directory depth. Install Python 3.10+, requirements in
paper/ispass2027/analysis/requirements.txt and Poppler pdftohtml separately. This
archive does not vendor a Python environment or the complete tool dependency set.
Then run from this root:

    ANALYSIS_PYTHON=/path/to/python bash paper/ispass2027/analysis/build.sh
    /path/to/python paper/ispass2027/analysis/scripts/baseline_extra.py --check

The exporter has executed the standard build in a clean temporary copy with a
network/file-access guard and compared the scientific numeric objects, all 17
macros and all ten figure statuses against the accepted source version. Byte
hashes and local locator metadata legitimately differ after labeled redaction.
The same guard can be enabled with PYTHONPATH=artifact/runtime and environment
REPLAY_ROOT=<this root>, REPLAY_TOOL_ROOT=<the installed Python environment>.
System libraries, font files and the separately installed Python tool environment
are dependencies, not experiment inputs. No original working tree is required.

Some raw_path fields remain neutral provenance locators to historical data not in
this minimum package. The committed history snapshots contain the exact operands
used here; reconstructing all historical long-table producers is a separate tier.
Neither missing logs nor this numeric replay proves end-to-end hardware
reproduction or retrospectively verifies historical loaded BIT bytes.

Figure 3 remains partial; figures 4--7 remain explicit missing-data scaffolds;
figure 8 uses separate historical versions; figures 9--10 are simulations.
Preserved scientific commits, hashes and dataset fingerprints can correlate with
public sources. Identifier scanning is bounded and is not venue anonymity review.
Publisher PDFs, credentials, event/UART/stdout logs, BIT/Image binaries, RTL build
outputs and the installed environment are deliberately absent.
'''


def science_view(value):
    return {key: value[key] for key in (
        'schema', 'analysis_date', 'seed', 'resamples', 'clock_hz', 'burst_bytes',
        'n_trials', 'n_groups', 'formulas', 'actual_commits', 'bootstrap_scope',
        'statistics', 'throughput', 'path', 'boot_comparison')} | {
        'samples': [{k: v for k, v in sample.items() if k not in
                     {'raw_path', 'raw_record_line', 'source_operands', 'paired_solo_source'}}
                    for sample in value['samples']]}


def macros(path):
    return dict(re.findall(r'\\newcommand\{\\(\w+)\}\{([^{}]+)\}', path.read_text()))


def archive_tree(root, target):
    with target.open('wb') as raw:
        with gzip.GzipFile(filename='', fileobj=raw, mode='wb', mtime=0) as zipped:
            with tarfile.open(fileobj=zipped, mode='w') as archive:
                for path in sorted(root.rglob('*')):
                    if path.is_symlink():
                        raise ValueError('A portable bundle must not contain symlinks')
                    if not path.is_file():
                        continue
                    info = archive.gettarinfo(str(path), arcname=path.relative_to(root).as_posix())
                    info.uid = info.gid = info.mtime = 0
                    info.uname = info.gname = ''
                    info.mode = 0o755 if path.suffix == '.sh' else 0o644
                    with path.open('rb') as source:
                        archive.addfile(info, source)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--python', type=Path, default=PAPER/'analysis/.venv/bin/python')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    python = args.python.absolute()
    if not python.is_file():
        raise ValueError('Provide an installed Python with analysis requirements via --python')
    accepted = read_json(PAPER/'analysis/baseline100-extra.json')
    inputs = sorted(set(accepted['inputs_sha256']) |
                    {str(PREFIX/rel) for rel in EXTRA_INPUTS})
    if len(inputs) != 17:
        raise ValueError('The approved minimum numeric input closure changed')
    code = [str(PREFIX/rel) for rel in CONTROLS]
    output = args.output or PAPER/'artifact/packages'/('analysis-' + time.strftime('%Y%m%d-%H%M%S'))
    output = output.absolute()
    output.mkdir(parents=True, exist_ok=False)
    private = output/'private-map';private.mkdir(mode=0o700)
    stage = output/'bundle';stage.mkdir()
    deriver = Deriver();mapping=[];patches=[]
    for relative in inputs + code:
        source = ROOT/relative
        if source.is_symlink():
            raise ValueError('Whitelist input unexpectedly became a symlink: ' + relative)
        original = source.read_bytes()
        if relative in accepted['inputs_sha256'] and sha(original) != accepted['inputs_sha256'][relative]:
            raise ValueError('Accepted B1 source hash changed: ' + relative)
        before_changes = len(deriver.changes)
        if source.suffix == '.json':
            value = json.loads(original)
            derived_value = deriver.value(value, locator=relative)
            if numeric_leaves(value) != numeric_leaves(derived_value):
                raise ValueError('Identifier redaction changed numeric JSON operands: ' + relative)
            derived = (json.dumps(derived_value, indent=2, ensure_ascii=False, allow_nan=False)+'\n').encode()
        elif source.suffix == '.csv':
            table = list(csv.DictReader(io.StringIO(original.decode())))
            fieldnames = list(table[0])
            for i, row in enumerate(table):
                # Scientific values and identifiers in all other columns remain literal.
                for field in ('raw_path','source_locator'):
                    if field in row:
                        row[field] = deriver.text(row[field], locator=f'{relative}/{i}/{field}')
            target = io.StringIO(newline='')
            writer=csv.DictWriter(target,fieldnames=fieldnames,lineterminator='\n')
            writer.writeheader();writer.writerows(table);derived=target.getvalue().encode()
        else:
            text=deriver.text(original.decode(),locator=relative)
            text=adapt_script(relative,text)
            derived=text.encode()
            if derived != original:
                patches.append(''.join(difflib.unified_diff(original.decode().splitlines(True),text.splitlines(True),
                    fromfile='original/'+relative,tofile='derived/'+relative)))
        destination=stage/relative;destination.parent.mkdir(parents=True,exist_ok=True);destination.write_bytes(derived)
        if source.suffix=='.sh':destination.chmod(0o755)
        mapping.append(dict(path=relative,original_path=str(source),original_sha256=sha(original),
            derived_sha256=sha(derived),original_bytes=len(original),derived_bytes=len(derived),
            status='DERIVED_REDACTED' if original!=derived else 'UNCHANGED_SELECTED_BYTES',
            redactions=deriver.changes[before_changes:]))
    # Hash binding is to the derivative CSV, without pretending it is original bytes.
    receipt_path=stage/PREFIX/'exp/E7-capacity-sim/analysis-import.json'
    receipt=read_json(receipt_path)
    receipt['results_sha256']=sha((stage/PREFIX/'exp/E7-capacity-sim/results.csv').read_bytes())
    saved(receipt_path,receipt)
    for entry in mapping:
        if entry['path']==str(receipt_path.relative_to(stage)):
            entry['derived_sha256']=sha(receipt_path.read_bytes());entry['derived_bytes']=receipt_path.stat().st_size
    (stage/'artifact/runtime').mkdir(parents=True)
    (stage/'artifact/runtime/sitecustomize.py').write_text(GUARD)
    (stage/'README.md').write_text(README)
    saved(stage/'artifact/DERIVED_REDACTED.json',dict(status='DERIVED_REDACTED',
        scope='saved-data numerical replay only; original evidence unchanged',
        preserved=['scientific numeric fields','boot groups','source commits','BIT SHA256','statistics formulas and seeds'],
        limitations=['tool dependencies installed separately','not end-to-end hardware reproduction','not conference anonymity certification']))
    (private/'adaptation.patch').write_text(''.join(patches))
    saved(private/'original-derived.json',dict(scope='PRIVATE; do not redistribute with public derivative',files=mapping))
    for path in private.iterdir():path.chmod(0o600)
    tool_root=python.parent.parent
    staging_archive=private/'input-staging.tar.gz'
    archive_tree(stage,staging_archive)
    staging_archive.chmod(0o600)
    with tempfile.TemporaryDirectory(prefix='ispass-analysis-replay-') as temp:
        replay=Path(temp)/'replay';replay.mkdir()
        # Exercise actual archive extraction, not a development-tree symlink/copy.
        with tarfile.open(staging_archive,'r:gz') as packed:
            for member in packed.getmembers():
                name=Path(member.name)
                if name.is_absolute() or '..' in name.parts or not member.isfile():
                    raise ValueError('Unexpected staging archive member')
            packed.extractall(replay)
        scratch=replay/'.runtime';(scratch/'tmp').mkdir(parents=True)
        environment=dict(os.environ,ANALYSIS_PYTHON=str(python),PYTHONDONTWRITEBYTECODE='1',PYTHONNOUSERSITE='1',
            PYTHONPATH=str(replay/'artifact/runtime'),REPLAY_ROOT=str(replay),REPLAY_TOOL_ROOT=str(tool_root),
            MPLCONFIGDIR=str(scratch/'mpl'),TMPDIR=str(scratch/'tmp'))
        started=time.time()
        with (private/'replay-tool-output.txt').open('w') as log:
            result=subprocess.run(['bash','paper/ispass2027/analysis/build.sh'],cwd=replay,
                env=environment,stdout=log,stderr=subprocess.STDOUT,timeout=900)
        if result.returncode:
            raise RuntimeError('Clean replay failed; see private-map/replay-tool-output.txt')
        actual=read_json(replay/PREFIX/'analysis/baseline100-extra.json')
        if science_view(actual)!=science_view(accepted):raise ValueError('B1 scientific numeric objects changed')
        if actual['n_trials']!=100 or len(actual['statistics'])!=600:raise ValueError('B1 acceptance counts changed')
        if read_json(replay/PREFIX/'analysis/figure-statistics.json')!=read_json(PAPER/'analysis/figure-statistics.json'):
            raise ValueError('Figure numeric objects changed')
        expected_macros=macros(PAPER/'tex/numbers.tex');actual_macros=macros(replay/PREFIX/'tex/numbers.tex')
        if expected_macros!=actual_macros or len(actual_macros)!=17:raise ValueError('Numerical macros changed')
        expected_status=[(t['artifact_id'],t['status'],t['base_ids']) for t in read_json(PAPER/'analysis/figure-trace.json')]
        actual_status=[(t['artifact_id'],t['status'],t['base_ids']) for t in read_json(replay/PREFIX/'analysis/figure-trace.json')]
        if expected_status!=actual_status:raise ValueError('Ten-figure status/base contract changed')
        merged=read_json(replay/PREFIX/'analysis/merge-manifest.json')
        if merged['scalar_rows']!=17404:raise ValueError('Canonical scalar count changed')
        validation=read_json(replay/PREFIX/'analysis/build-validation.json')
        if not validation['rendered_font_sizes_checked'] or any(t['minimum_rendered_font_pt']<8 for t in validation['checks']):
            raise ValueError('Actual 8pt figure font check did not pass')
        generated=[]
        for path in sorted(replay.rglob('*')):
            if path.is_file() and '.runtime' not in path.relative_to(replay).parts:
                relative=path.relative_to(replay);destination=stage/relative
                destination.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(path,destination)
                if str(relative) not in inputs+code:generated.append(str(relative))
        seconds=time.time()-started
    forbidden=[deriver.username.encode(),*(r.encode() for r in deriver.roots)]
    findings=[]
    for path in sorted(stage.rglob('*')):
        if not path.is_file():continue
        data=path.read_bytes()
        if any(token and token in data for token in forbidden):findings.append(str(path.relative_to(stage)))
        if re.search(rb'github\.com[:/]'+re.escape(deriver.username.encode())+rb'/',data):findings.append(str(path.relative_to(stage)))
    if findings:raise ValueError('Local identity scan failed: '+str(sorted(set(findings))))
    package_files=[dict(path=str(p.relative_to(stage)),bytes=p.stat().st_size,sha256=sha(p.read_bytes()))
                   for p in sorted(stage.rglob('*')) if p.is_file()]
    saved(stage/'artifact/file-manifest.json',dict(status='DERIVED_REDACTED',files=package_files,
        self_hash='recorded in outside-package receipt, avoiding self-reference'))
    archive=output/'analysis-replay.tar.gz';archive_tree(stage,archive)
    accepted_receipt=dict(schema='analysis-bundle-replay-v1',status='passed',classification='DERIVED_REDACTED',
        archive=archive.name,archive_sha256=sha(archive.read_bytes()),archive_bytes=archive.stat().st_size,
        numeric_input_count=len(inputs),control_count=len(code),accepted_counts=dict(trials=100,scalar_rows=17404,statistics=600,macros=17,figures=10),
        scientific_numeric_comparison='exact objects: B1 samples/statistics/tables, figure statistics, macro values',
        figure_status_comparison=actual_status,rendered_minimum_font_pt=min(x['minimum_rendered_font_pt'] for x in validation['checks']),
        guard='Python audit hook blocks sockets and non-package/non-tool/system file opens; actual staging archive extracted to clean temporary directory; no original experiment input access',
        network='disabled by Python audit hook; no package code invokes a network client',
        tool_python=str(python),tools_vendored=False,elapsed_seconds=seconds,
        local_identity_scan='passed bounded current username/development roots/author repository URL scan',
        logs_in_archive=False,private_map_in_archive=False,
        limitation='Preserved scientific commits/fingerprints can correlate with public sources; not venue anonymity certification or end-to-end hardware replay',
        file_manifest_sha256=sha((stage/'artifact/file-manifest.json').read_bytes()))
    saved(output/'receipt.json',accepted_receipt)
    print(json.dumps({k:accepted_receipt[k] for k in ('status','classification','archive_sha256','archive_bytes','accepted_counts','rendered_minimum_font_pt')},indent=2))
    print('OUTPUT '+str(output))


if __name__=='__main__':main()
