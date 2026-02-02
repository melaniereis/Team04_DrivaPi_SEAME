import os
import sys
import argparse

def main():
    parser = argparse.ArgumentParser(description='Validate static analysis SARIF results.')
    parser.add_argument('--base-dir', default='artifacts/verification/static-analysis', help='Static analysis directory')
    parser.add_argument('--sarif-candidates', nargs='+', default=[
        'results/cpp.sarif',
        os.path.join('artifacts/verification/static-analysis', 'cpp.sarif'),
        os.path.join('artifacts/verification/static-analysis', 'results/cpp.sarif'),
    ], help='SARIF file candidates')
    args = parser.parse_args()
    try:
        sys.path.insert(0, 'dotstop')
        from dotstop_extensions.validators import codeql_sarif_validator
        has_validator = True
    except ImportError:
        has_validator = False
        print("Warning: dotstop_extensions.validators not found, skipping SARIF validation")
    if has_validator:
        sarif_path = None
        for p in args.sarif_candidates:
            if os.path.exists(p):
                sarif_path = p
                break
        if sarif_path is None:
            print('No SARIF file found; skipping static analysis validation.')
            sys.exit(0)
        score, errs = codeql_sarif_validator({'path': sarif_path, 'hard_fail_on_error': True})
        print(f'Static Analysis SARIF: path={sarif_path} score={score} errs={errs}')
        if score <= 0.0 and not (errs is None or len(errs)==0):
            print('Blocking due to static analysis errors.')
            sys.exit(1)
    else:
        print("Skipping SARIF validation - dotstop_extensions not available")

if __name__ == '__main__':
    main()
