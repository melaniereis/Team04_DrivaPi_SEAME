#!/usr/bin/env bash
# generate_coverage_approvals.sh
# Usage: generate_coverage_approvals.sh <labels> <sha> <output_path>
# labels: comma-separated list of PR labels (lowercase)
# sha: commit SHA
# output_path: path to approvals.json

set -e

LABELS="$1"
SHA="$2"
OUTPUT_PATH="$3"

mkdir -p "$(dirname "$OUTPUT_PATH")"

approve_all=false
if [[ "$LABELS" == *"approve-coverage"* ]]; then
  approve_all=true
fi

declare -A SUITES
SUITES["dc-motor"]=$approve_all
SUITES["servo-motor"]=$approve_all
SUITES["speed-sensor"]=$approve_all

if [[ "$LABELS" == *"approve-coverage-dc-motor"* ]]; then SUITES["dc-motor"]=true; fi
if [[ "$LABELS" == *"approve-coverage-servo-motor"* ]]; then SUITES["servo-motor"]=true; fi
if [[ "$LABELS" == *"approve-coverage-speed-sensor"* ]]; then SUITES["speed-sensor"]=true; fi

json="{\"suites\":{"
first=true
for suite in "dc-motor" "servo-motor" "speed-sensor"; do
  if [[ "${SUITES[$suite]}" == true ]]; then
    if ! $first; then json+=","; fi
    json+="\"$suite\":{\"approved\":true,\"approved_sha\":\"$SHA\",\"timestamp\":\"$(date -u +%Y-%m-%dT%H:%M:%SZ)\"}"
    first=false
  fi
done
json+="}}"
echo "$json" > "$OUTPUT_PATH"
echo "Approvals file written at $OUTPUT_PATH"
