import os
import json
import xml.etree.ElementTree as ET
import argparse

def read_rates(p):
    tree = ET.parse(p)
    r = tree.getroot()
    lr = float(r.attrib.get('line-rate') or r.attrib.get('lineRate'))
    br = float(r.attrib.get('branch-rate') or r.attrib.get('branchRate', lr))
    return lr, br

def main():
    parser = argparse.ArgumentParser(description='Generate coverage baseline JSON.')
    parser.add_argument('--base-dir', default='artifacts/verification/coverage', help='Coverage directory')
    parser.add_argument('--suites', nargs='+', default=['dc-motor','servo-motor','speed-sensor'], help='List of suite names')
    parser.add_argument('--sha', default=None, help='Commit SHA (default: GITHUB_SHA env)')
    args = parser.parse_args()

    base_dir = args.base_dir
    os.makedirs(base_dir, exist_ok=True)
    data = {'suites': {}, 'sha': args.sha or os.environ.get('GITHUB_SHA')}
    for suite in args.suites:
        f = os.path.join(base_dir, f"{suite}.xml")
        if os.path.exists(f):
            lr, br = read_rates(f)
            data['suites'][suite] = {'line_rate': lr, 'branch_rate': br}
    with open(os.path.join(base_dir,'baseline.json'),'w') as fo:
        json.dump(data, fo, indent=2)
    print('Baseline written:', os.path.join(base_dir,'baseline.json'))

if __name__ == '__main__':
    main()
