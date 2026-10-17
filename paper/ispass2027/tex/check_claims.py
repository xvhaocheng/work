#!/usr/bin/env python3
"""Check declared claim/citation/number joins; not semantic completeness."""
from __future__ import annotations

import json
import re
from pathlib import Path


def main() -> int:
    root = Path(__file__).resolve().parent
    sources = [root / 'main.tex', *sorted((root / 'sections').glob('*.tex'))]
    text = '\n'.join(p.read_text() for p in sources)
    # Strip ordinary TeX comments, preserving escaped percent signs.
    text = re.sub(r'(?<!\\)%[^\n]*', '', text)
    raw_claims = set(re.findall(r'\\claim\{([^}]+)\}', text))
    claims = {key for key in raw_claims if re.fullmatch(r'[A-Z][A-Z0-9-]+', key)}
    registered = set(re.findall(r'^\| ([A-Z][A-Z0-9-]+) \|',
                                (root / 'claims.md').read_text(), re.M))
    registered.discard('ID')  # Markdown table header.
    citations = {key.strip() for group in re.findall(r'\\cite\{([^}]+)\}', text)
                 for key in group.split(',')}
    entries = set(re.findall(r'@\w+\s*\{\s*([^,]+),',
                             (root / 'references.bib').read_text()))
    problems = []
    if raw_claims - claims:
        problems.append('malformed claim IDs: ' + ', '.join(sorted(raw_claims - claims)))
    if claims - registered:
        problems.append('unregistered claim IDs: ' + ', '.join(sorted(claims - registered)))
    if registered - claims:
        problems.append('unused claim IDs: ' + ', '.join(sorted(registered - claims)))
    if citations - entries:
        problems.append('missing bibliography entries: ' + ', '.join(sorted(citations - entries)))
    if entries - citations:
        problems.append('uncited bibliography entries: ' + ', '.join(sorted(entries - citations)))
    numbers = (root / 'numbers.tex').read_text()
    definitions = dict(re.findall(r'\\newcommand\{\\(\w+)\}\{([^}]+)\}', numbers))
    expected = set(re.findall(r'\\providecommand\{\\(\w+)\}\{\\textit\{pending\}\}', text))
    used_numbers = {key for key in expected if re.search(r'\\' + re.escape(key) + r'(?![A-Za-z])',
                     '\n'.join(p.read_text() for p in sources if p.name != 'main.tex'))
                    or key in ('BaselineTotalRuns', 'RunsPerCondition')}
    missing_numbers = used_numbers - definitions.keys()
    if missing_numbers:
        problems.append('numerical macros still pending: ' + ', '.join(sorted(missing_numbers)))
    provenance_file = root / 'numbers-provenance.json'
    if not provenance_file.exists():
        problems.append('missing numerical macro provenance')
    else:
        provenance = json.loads(provenance_file.read_text())
        derivations = provenance.get('derivations', {})
        if used_numbers - derivations.keys():
            problems.append('macros without provenance: ' + ', '.join(sorted(used_numbers - derivations.keys())))
        for key in used_numbers & definitions.keys():
            value = provenance.get('macros', {}).get(key)
            if value is None or str(value) != definitions[key]:
                problems.append('macro/provenance value mismatch: ' + key)
    for phrase in (r'\bthe first (?:study|paper|work|evaluation)\b',
                   r'\bno prior work\b', r'\bthe only study\b'):
        if re.search(phrase, text, re.I):
            problems.append('priority wording requires review: ' + phrase)
    report = dict(scope='structural joins only; no proof of semantic or extraction completeness',
                  claims=len(claims), references=len(entries), numeric_macros_used=len(used_numbers),
                  problems=problems, status='passed' if not problems else 'failed')
    (root / 'claim-check.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))
    return 1 if problems else 0


if __name__ == '__main__':
    raise SystemExit(main())
