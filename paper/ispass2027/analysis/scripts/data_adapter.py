"""Read-only historical adapter and strict, staged experiment-result merger."""
from __future__ import annotations

import csv
import hashlib
import io
import json
import re
from collections import defaultdict
from pathlib import Path

ADDED_FIELDS = ['base_id', 'base_scope', 'input_table', 'input_line']
BASELINE_BASE = 'baseline100-fixed-885ed0a7-45bf76b1-pathv3'
BASELINE_COMMITS = dict(platform='885ed0a7ea90ed42dae2b7a2dadfdd0acba15273',
                        linux='45bf76b15e36cf608dff95013d91f8bedb7b1b8e')
BASELINE_BITS = {'0/0':'e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6',
                 '50/50':'580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77'}


def stable_bytes(path):
    path = Path(path)
    before = path.stat()
    content = path.read_bytes()
    after = path.stat()
    if (before.st_size, before.st_mtime_ns, before.st_ino) != (after.st_size, after.st_mtime_ns, after.st_ino):
        raise ValueError(f'input changed while reading: {path}')
    return content


def condition_fields(row):
    return dict(p.split('=', 1) for p in row.get('condition', '').split(';') if '=' in p)


def as_number(row):
    if row.get('value_status') not in ('observed', 'derived', ''):
        return None
    try:
        value = float(row['value'])
    except (ValueError, TypeError):
        return None
    return value if __import__('math').isfinite(value) else None


def historical_base(row):
    """Conservative comparison scopes, never a retroactive loaded-byte claim."""
    ex = row['experiment']
    fields = condition_fields(row)
    if ex == 'board_baseline100':
        return BASELINE_BASE, 'fixed source/package; DS is a declared BIT variant; boots retained'
    if ex == 'composition':
        return 'history-composition-' + fields['side'], 'one historical side/boot; loaded-byte identity unknown'
    if ex == 'p4m':
        return 'history-p4m-' + fields.get('acq', row['sample_set']), 'one historical acquisition; no cross-acquisition pooling'
    if ex == 'invalidation_sim':
        return 'invalidation-frozen-ba8cdd1-85b157e', 'fixed RTL behavioral DDR simulation; Linux not executed'
    return 'history-' + ex, 'one existing experiment family; retains recorded identity limitations'


def read_csv(path):
    content = stable_bytes(path)
    reader = csv.DictReader(io.StringIO(content.decode('utf-8-sig')))
    fields = reader.fieldnames or []
    return list(reader), fields, content


def require_one_base(rows, *, context):
    bases = {r.get('base_id') for r in rows}
    if not rows:
        raise ValueError(f'{context}: missing samples')
    if len(bases) != 1 or None in bases or '' in bases:
        raise ValueError(f'{context}: mixed or missing base_id: {sorted(str(x) for x in bases)}')
    return next(iter(bases))


def reject_duplicate_ids(rows):
    seen = set()
    for row in rows:
        rid = row.get('row_id')
        if not rid or rid in seen:
            raise ValueError(f'duplicate or empty row_id: {rid}')
        seen.add(rid)


def sample_values(rows, *, context='sample selection'):
    """Summary rows cannot masquerade as observations for a bootstrap/CDF."""
    require_one_base(rows, context=context)
    if any(r['statistic'] != 'sample' or int(r['n']) != 1 for r in rows):
        raise ValueError(f'{context}: trial samples required; summary scalar rows are not repetitions')
    result = [as_number(row) for row in rows]
    if any(v is None for v in result):
        raise ValueError(f'{context}: unknown/nonfinite sample')
    return result


def source_entry(path, content, *, kind):
    return dict(path=str(Path(path).resolve()), bytes=len(content),
                sha256=hashlib.sha256(content).hexdigest(), kind=kind)


def load_historical(path):
    rows, fields, content = read_csv(path)
    for line, row in enumerate(rows, 2):
        row['base_id'], row['base_scope'] = historical_base(row)
        row['input_table'], row['input_line'] = str(Path(path).resolve()), str(line)
    return rows, fields, source_entry(path, content, kind='canonical_historical_long_table')


def validate_baseline_identity(canonical, rows):
    package=Path(canonical).parent.parent/'board/packages/baseline100-20261017'
    source_bytes=stable_bytes(package/'source.json')
    source=json.loads(source_bytes)
    for component,commit in BASELINE_COMMITS.items():
        if source['actual_commits'].get(component)!=commit:
            raise ValueError(f'fixed baseline base_id no longer matches {component} source commit')
    for row in rows:
        if row['experiment']!='board_baseline100':continue
        ds=condition_fields(row).get('ds')
        if ds not in BASELINE_BITS or row['bitstream_id']!='program_file_sha256:'+BASELINE_BITS[ds]:
            raise ValueError(f'baseline row BIT/DS mismatch: {row["row_id"]}')
        if row['kernel_id']!='6.5.0-rc2-ispass2027-pathv3':
            raise ValueError(f'baseline row kernel/base mismatch: {row["row_id"]}')
    return source_entry(package/'source.json',source_bytes,kind='actual_fixed_baseline_source_identity')


def load_experiment_table(path):
    """Import only an explicit complete run; native E7 wide simulation is separate.

    The adjacent analysis-import.json is a publication receipt, not execution
    authority. It binds CSV hash, base ID and completion; pending files are never
    copied into the merged table. See merge-notes.md for the input contract.
    """
    path = Path(path)
    receipt_path = path.with_name('analysis-import.json')
    if not receipt_path.exists():
        return [], [], dict(path=str(path), status='not_imported', reason='missing analysis-import.json completion receipt')
    receipt_bytes = stable_bytes(receipt_path)
    receipt = json.loads(receipt_bytes)
    if receipt.get('status') != 'passed' or receipt.get('complete') is not True:
        return [], [], dict(path=str(path), status='not_imported', reason='run not explicitly complete/passed')
    rows, fields, content = read_csv(path)
    required = {'row_id','experiment','condition','metric','statistic','value','unit','n','raw_path','source_locator','base_id','evidence_level',
                'boot_id','value_status','bitstream_id','kernel_id','identity_status','sample_set'}
    if required - set(fields):
        raise ValueError(f'{path}: missing columns {sorted(required-set(fields))}')
    if hashlib.sha256(content).hexdigest() != receipt.get('results_sha256'):
        raise ValueError(f'{path}: completion receipt CSV hash does not match')
    base = receipt.get('base_id')
    if not base or not re.fullmatch(r'[A-Za-z0-9_.-]+', base):
        raise ValueError(f'{path}: invalid base_id in receipt')
    if not receipt.get('source_manifest') or not receipt.get('run_manifest'):
        raise ValueError(f'{path}: missing source/run manifest locators')
    for line, row in enumerate(rows, 2):
        if row['base_id'] != base:
            raise ValueError(f'{path}:{line}: row/receipt base_id mismatch')
        if row['evidence_level'] != 'fpga_board':
            raise ValueError(f'{path}:{line}: simulation cannot enter board table')
        row['base_scope'] = 'explicit completed experiment receipt; see merge manifest'
        row['input_table'], row['input_line'] = str(path.resolve()), str(line)
    return rows, fields, dict(status='imported', receipt=receipt,
                             csv=source_entry(path,content,kind='completed_experiment'),
                             receipt_source=source_entry(receipt_path,receipt_bytes,kind='completion_receipt'))


def merge_tables(canonical, experiment_roots=()):
    rows, fields, historical = load_historical(canonical)
    inputs, decisions = [historical, validate_baseline_identity(canonical, rows)], []
    snapshots=[]
    seen_files = set()
    for root in experiment_roots:
        root = Path(root)
        if not root.exists():
            decisions.append(dict(path=str(root), status='missing_optional_experiment_root'))
            continue
        # Existing Root A offline summaries are identity-snapshotted but never
        # appended as fresh trials. No broad scan of a live experiment tree.
        audit_paths=set(root.glob('*/historical-*-analysis.json')) | set(root.glob('*/analysis-validation.json'))
        for audit_path in sorted(audit_paths):
            try:
                audit_content=stable_bytes(audit_path)
                audit=json.loads(audit_content)
                snapshots.append(dict(**source_entry(audit_path,audit_content,kind='read_only_offline_experiment_summary'),
                    status=audit.get('status','not_a_measurement'),
                    new_board_measurements=audit.get('new_board_measurements','not_claimed'),
                    import_decision='not_appended; offline summary/validation is not new raw trial data'))
            except (ValueError,OSError) as exc:
                snapshots.append(dict(path=str(audit_path),import_decision='not_read_or_imported',reason=str(exc)))
        for path in sorted(set(root.glob('**/results.csv')) | set(root.glob('**/board_results.csv'))):
            if path.resolve() in seen_files:
                continue
            seen_files.add(path.resolve())
            # Simulation wide tables are read only by the corresponding plot.
            if path.parent.name == 'E7-capacity-sim':
                decisions.append(dict(path=str(path),status='separate_simulation_not_board'))
                continue
            addition, more_fields, decision = load_experiment_table(path)
            decisions.append(decision)
            fields += [f for f in more_fields if f not in fields]
            rows.extend(addition)
    reject_duplicate_ids(rows)
    fields += [f for f in ADDED_FIELDS if f not in fields]
    rows.sort(key=lambda r:r['row_id'])
    return rows, fields, dict(inputs=inputs, experiment_imports=decisions,
                            experiment_input_snapshots=snapshots,
                            scalar_rows=len(rows), independent_trials='not scalar-row count',
                            bases={b:sum(r['base_id']==b for r in rows) for b in sorted({r['base_id'] for r in rows})})
