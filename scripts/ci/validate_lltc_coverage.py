import sys
import json
import os
import argparse

def main():
    parser = argparse.ArgumentParser(description='Validate per-suite coverage changes vs baseline and approvals.')
    parser.add_argument('--base-dir', default='artifacts/verification/coverage', help='Coverage directory')
    parser.add_argument('--suites', nargs='+', default=['dc-motor','servo-motor','speed-sensor'], help='List of suite names')
    parser.add_argument('--min-line-rate', type=float, default=90, help='Minimum line rate')
    args = parser.parse_args()

    base_dir = args.base_dir
    try:
        sys.path.insert(0, 'dotstop')
        from dotstop_extensions.validators import lltc_coverage_change_validator
        has_validator = True
    except ImportError:
        has_validator = False
        print("Warning: dotstop_extensions.validators not found, skipping LLTC validation")

    if has_validator:
        inputs = [(suite, os.path.join(base_dir, f'{suite}.xml')) for suite in args.suites]
        baseline = os.path.join(base_dir, 'baseline.json')
        approvals = os.path.join(base_dir, 'approvals.json')
        overall_ok = True
        summary = []
        for suite, path in inputs:
            if os.path.exists(path):
                cfg = {
                    'path': path,
                    'suite': suite,
                    'baseline_path': baseline if os.path.exists(baseline) else None,
                    'approval_path': approvals if os.path.exists(approvals) else None,
                    'min_line_rate': args.min_line_rate,
                    'require_review_on_change': True,
                    'enforce_individual': True,
                }
                score, issues = lltc_coverage_change_validator(cfg)
                suite_ok = (score >= 1.0) and (not issues)
                summary.append((suite, score, [str(e) for e in issues]))
                if issues:
                    overall_ok = False
            else:
                summary.append((suite, 0.0, [f"Coverage file not found: {path}"]))
                overall_ok = False
        with open('LLTC Coverage Validation Summary.txt','w') as f:
            for s, sc, iss in summary:
                f.write(f"- {s}: score={sc:.2f} issues={iss}\n")
        print('LLTC Coverage Validation Summary:')
        for s, sc, iss in summary:
            print(f"  - {s}: score={sc:.2f} issues={iss}")
        if not overall_ok:
            print('Blocking due to coverage change requiring approval or parse errors.')
            sys.exit(1)
    else:
        print("Skipping LLTC coverage validation - dotstop_extensions not available")

if __name__ == '__main__':
    main()
