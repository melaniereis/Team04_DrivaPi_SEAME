#!/usr/bin/env bash
# summarize_test_results.sh
# Usage: summarize_test_results.sh <test_output_log> <lltc_summary_file> [summary_json]

set -e

TEST_LOG="$1"
LLTC_SUMMARY_FILE="$2"
SUMMARY_JSON="${3:-artifacts/verification/tests/summary.json}"

if [[ ! -f "$TEST_LOG" ]]; then
  echo "::error::Test log not found: $TEST_LOG"
  echo "🧪 Unit tests were NOT summarized because the log file is missing."
  echo "Check the workflow steps that generate '$TEST_LOG' (tests likely failed or did not run)."
  exit 1
fi

log_content=$(cat "$TEST_LOG")

dcMotorCount=$(echo "$log_content" | grep -oE '\[TEST_COUNT\] DC Motor:[[:space:]]*[0-9]+' | grep -oE '[0-9]+')
servoMotorCount=$(echo "$log_content" | grep -oE '\[TEST_COUNT\] Servo Motor:[[:space:]]*[0-9]+' | grep -oE '[0-9]+')
speedSensorCount=$(echo "$log_content" | grep -oE '\[TEST_COUNT\] Speed Sensor:[[:space:]]*[0-9]+' | grep -oE '[0-9]+')

dcMotorCount=${dcMotorCount:-0}
servoMotorCount=${servoMotorCount:-0}
speedSensorCount=${speedSensorCount:-0}

testPassed=$((dcMotorCount + servoMotorCount + speedSensorCount))

dcMotorStatus=""
servoMotorStatus=""
speedSensorStatus=""
overallFromSummary=""

if [[ -f "$SUMMARY_JSON" ]]; then
  parsed_statuses=$(python3 - "$SUMMARY_JSON" <<'PY'
import json
import sys

path = sys.argv[1]
try:
    with open(path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    tests = data.get('tests', {})
    dc = tests.get('dc_motor', {}).get('status', '')
    sv = tests.get('servo_motor', {}).get('status', '')
    sp = tests.get('speed_sensor', {}).get('status', '')
    ov = data.get('overall_status', '')
    print(f"{dc}|{sv}|{sp}|{ov}")
except Exception:
    print("|||")
PY
)
  IFS='|' read -r dcMotorStatus servoMotorStatus speedSensorStatus overallFromSummary <<< "$parsed_statuses"
fi

if [[ -z "$dcMotorStatus" || -z "$servoMotorStatus" || -z "$speedSensorStatus" ]]; then
  summaryLine=$(echo "$log_content" | grep -oE '\[INFO\][[:space:]]+Summary:[[:space:]]+DC Motor=(PASSED|FAILED),[[:space:]]+Servo Motor=(PASSED|FAILED),[[:space:]]+Speed Sensor=(PASSED|FAILED)' | tail -n1)
  if [[ -n "$summaryLine" ]]; then
    dcMotorStatus=$(echo "$summaryLine" | sed -E 's/.*DC Motor=(PASSED|FAILED),.*/\1/')
    servoMotorStatus=$(echo "$summaryLine" | sed -E 's/.*Servo Motor=(PASSED|FAILED),.*/\1/')
    speedSensorStatus=$(echo "$summaryLine" | sed -E 's/.*Speed Sensor=(PASSED|FAILED).*/\1/')
  fi
fi

if [[ -z "$dcMotorStatus" ]]; then
  dcMotorStatus=$(echo "$log_content" | grep -qE 'DC Motor:[[:space:]]*FAILED' && echo FAILED || echo PASSED)
fi
if [[ -z "$servoMotorStatus" ]]; then
  servoMotorStatus=$(echo "$log_content" | grep -qE 'Servo Motor:[[:space:]]*FAILED' && echo FAILED || echo PASSED)
fi
if [[ -z "$speedSensorStatus" ]]; then
  speedSensorStatus=$(echo "$log_content" | grep -qE 'Speed Sensor:[[:space:]]*FAILED' && echo FAILED || echo PASSED)
fi

if [[ "$dcMotorStatus" == "FAILED" || "$servoMotorStatus" == "FAILED" || "$speedSensorStatus" == "FAILED" ]]; then
  testFailed=1
else
  testFailed=0
fi

failedSuites=0
[[ "$dcMotorStatus" == "FAILED" ]] && failedSuites=$((failedSuites + 1))
[[ "$servoMotorStatus" == "FAILED" ]] && failedSuites=$((failedSuites + 1))
[[ "$speedSensorStatus" == "FAILED" ]] && failedSuites=$((failedSuites + 1))

totalSuites=3
passedSuites=$((totalSuites - failedSuites))

dcMotorFailedCount=$(echo "$log_content" | awk -F':' '/:FAIL:/ && $1 ~ /test_dc_motor\.c$/ {print $3}' | sed '/^$/d' | sort -u | wc -l | tr -d ' ')
servoMotorFailedCount=$(echo "$log_content" | awk -F':' '/:FAIL:/ && $1 ~ /test_servo_motor\.c$/ {print $3}' | sed '/^$/d' | sort -u | wc -l | tr -d ' ')
speedSensorFailedCount=$(echo "$log_content" | awk -F':' '/:FAIL:/ && $1 ~ /test_speed_sensor\.c$/ {print $3}' | sed '/^$/d' | sort -u | wc -l | tr -d ' ')

dcMotorFailedCount=${dcMotorFailedCount:-0}
servoMotorFailedCount=${servoMotorFailedCount:-0}
speedSensorFailedCount=${speedSensorFailedCount:-0}

if [[ "$dcMotorStatus" == "FAILED" ]]; then
  dcMotorDisplayCount="${dcMotorFailedCount}/${dcMotorCount}"
else
  dcMotorDisplayCount="${dcMotorCount}/${dcMotorCount}"
fi

if [[ "$servoMotorStatus" == "FAILED" ]]; then
  servoMotorDisplayCount="${servoMotorFailedCount}/${servoMotorCount}"
else
  servoMotorDisplayCount="${servoMotorCount}/${servoMotorCount}"
fi

if [[ "$speedSensorStatus" == "FAILED" ]]; then
  speedSensorDisplayCount="${speedSensorFailedCount}/${speedSensorCount}"
else
  speedSensorDisplayCount="${speedSensorCount}/${speedSensorCount}"
fi

coverageMatch=$(echo "$log_content" | grep -Poz 'lines.*?(\d+\.\d+)%.*?functions.*?(\d+\.\d+)%.*?branches.*?(\d+\.\d+)%' | head -n1)
if [[ -n "$coverageMatch" ]]; then
  lines=$(echo "$coverageMatch" | grep -oP 'lines.*?\K(\d+\.\d+)(?=%)')
  functions=$(echo "$coverageMatch" | grep -oP 'functions.*?\K(\d+\.\d+)(?=%)')
  branches=$(echo "$coverageMatch" | grep -oP 'branches.*?\K(\d+\.\d+)(?=%)')
  coverageInfo=$(cat <<EOF
#### 📊 Coverage
- **Lines**: $lines%
- **Functions**: $functions%
- **Branches**: $branches%
EOF
)
else
  coverageInfo=$(cat <<'EOF'
#### 📊 Coverage
Coverage report available in artifacts.
EOF
)
fi

lltcSummary=""
if [[ -f "$LLTC_SUMMARY_FILE" ]]; then
  lltcItems=""
  while IFS= read -r line; do
    [[ -z "$line" ]] && continue
    if [[ "$line" =~ ^-\  ]]; then
      lltcItems+="${line}"$'\n'
    else
      lltcItems+="- ${line}"$'\n'
    fi
  done < "$LLTC_SUMMARY_FILE"

  lltcSummary=$(cat <<EOF
#### 🔍 Coverage Change Validation
$lltcItems
EOF
)
fi

if [[ "$overallFromSummary" == "PASSED" ]]; then
  overallStatus="✅ PASSED (${failedSuites} failed suites, ${passedSuites} passed suites, ${testPassed} total tests)"
elif [[ "$overallFromSummary" == "FAILED" ]]; then
  overallStatus="❌ FAILED (${failedSuites} failed suites, ${passedSuites} passed suites, ${testPassed} total tests)"
else
  if [[ $testPassed -gt 0 && $testFailed -eq 0 ]]; then
    overallStatus="✅ PASSED (${failedSuites} failed suites, ${passedSuites} passed suites, ${testPassed} total tests)"
  else
    overallStatus="❌ FAILED (${failedSuites} failed suites, ${passedSuites} passed suites, ${testPassed} total tests)"
  fi
fi

cat <<EOF
### 🧪 Unit Test Results

**Overall Status**: $overallStatus

- **DC Motor Tests**: $dcMotorStatus ($dcMotorDisplayCount)
- **Servo Motor Tests**: $servoMotorStatus ($servoMotorDisplayCount)
- **Speed Sensor Tests**: $speedSensorStatus ($speedSensorDisplayCount)
- **Total**: $testPassed tests

$coverageInfo

$lltcSummary
---
Full coverage reports available in workflow artifacts
EOF
