#!/usr/bin/env python3
"""Stage canonical + completed experiment rows without touching data/board_results.csv."""
import argparse
import csv
import json
from pathlib import Path
from data_adapter import merge_tables


def main():
    analysis = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--canonical', type=Path, default=analysis.parent/'data/board_results.csv')
    parser.add_argument('--experiment-root', type=Path, action='append', default=[])
    parser.add_argument('--output', type=Path, default=analysis/'merged-board-results.csv')
    args = parser.parse_args()
    roots = [analysis.parent/'exp'] + args.experiment_root
    rows, fields, manifest = merge_tables(args.canonical, roots)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open('w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=fields, lineterminator='\n')
        writer.writeheader()
        writer.writerows(rows)
    (analysis/'merge-manifest.json').write_text(json.dumps(manifest, indent=2, ensure_ascii=False)+'\n')
    print(f'merged {len(rows)} scalar rows into {args.output}; canonical table unchanged')


if __name__ == '__main__':
    main()
