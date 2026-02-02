#!/usr/bin/env python3
"""Generate a version-controlled coverage baseline (baseline.json)."""

import argparse
import json
import os
import xml.etree.ElementTree as ET
from datetime import datetime, timezone


def read_rates(path: str):
    tree = ET.parse(path)
    root = tree.getroot()
    lr = root.attrib.get("line-rate") or root.attrib.get("lineRate")
    br = root.attrib.get("branch-rate") or root.attrib.get("branchRate")
    if lr is None:
        raise ValueError(f"Missing line-rate in XML: {path}")
    line_rate = float(lr)
    branch_rate = float(br) if br is not None else line_rate
    return line_rate, branch_rate


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate coverage baseline JSON from Cobertura XML files.")
    parser.add_argument("--base-dir", default="artifacts/verification/coverage")
    parser.add_argument("--suites", nargs="+", default=["dc-motor", "servo-motor", "speed-sensor"])
    parser.add_argument("--sha", default=None)
    parser.add_argument("--out", default=None)
    args = parser.parse_args()

    base_dir = args.base_dir
    os.makedirs(base_dir, exist_ok=True)

    sha = args.sha or os.environ.get("GITHUB_SHA") or "unknown"
    out_path = args.out or os.path.join(base_dir, "baseline.json")

    data = {
        "timestamp": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "commit_sha": sha,
        "suites": {},
    }

    missing = []
    for suite in args.suites:
        xml_path = os.path.join(base_dir, f"{suite}.xml")
        if not os.path.exists(xml_path):
            missing.append(xml_path)
            continue
        lr, br = read_rates(xml_path)
        data["suites"][suite] = {"line-rate": lr, "branch-rate": br}

    if missing:
        print("::error::Missing coverage XML files:\n  - " + "\n  - ".join(missing))
        return 1

    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2)
        f.write("\n")

    print(f"Baseline written: {out_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
