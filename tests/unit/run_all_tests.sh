#!/bin/bash
################################################################################
# Master Unit Test Automation Script with Aggregated Coverage
#
# Purpose: Execute all unit tests and generate unified coverage/junit reports
# ASIL Level: B/D
# Author: DrivaPi Team
# Version: 2.2.0 (Fixed JUnit Merging - No Python)
################################################################################

set -e
set -u
set -o pipefail

# Ensure we have absolute paths
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly PROJECT_ROOT_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"

# Define sub-project directories
readonly MOTOR_SERVO_DIR="${SCRIPT_DIR}/motor_servo"
readonly SPEED_SENSOR_DIR="${SCRIPT_DIR}/speed_sensor"

# Handle coverage directory (Absolute Path)
if [[ -n "${COVERAGE_DIR:-}" ]]; then
    if [[ "$COVERAGE_DIR" == /* ]]; then
        MASTER_COVERAGE_DIR="$COVERAGE_DIR"
    else
        MASTER_COVERAGE_DIR="${PROJECT_ROOT_DIR}/${COVERAGE_DIR}"
    fi
else
    MASTER_COVERAGE_DIR="${SCRIPT_DIR}/coverage"
fi

readonly ARTIFACTS_DIR="${PROJECT_ROOT_DIR}/artifacts/verification"

# Create directories
mkdir -p "${MASTER_COVERAGE_DIR}"
mkdir -p "${ARTIFACTS_DIR}/tests"
mkdir -p "${ARTIFACTS_DIR}/coverage"

# Colors
if [[ -t 1 ]]; then
    readonly GREEN='\033[0;32m'
    readonly RED='\033[0;31m'
    readonly YELLOW='\033[1;33m'
    readonly BLUE='\033[0;34m'
    readonly BOLD='\033[1m'
    readonly NC='\033[0m'
else
    readonly GREEN='' RED='' YELLOW='' BLUE='' BOLD='' NC=''
fi

log_section() { echo -e "${BOLD}${BLUE}=== $1 ===${NC}"; }
log_pass() { echo -e "${GREEN}✓ $1${NC}"; }
log_fail() { echo -e "${RED}✗ $1${NC}"; }
log_info() { echo -e "${BLUE}[INFO]${NC} $1"; }
log_warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }

MOTOR_SERVO_PASSED=0
SPEED_SENSOR_PASSED=0

echo ""
log_section "ISO 26262 Master Unit Test Runner - DrivaPi (v2.2.0)"
echo ""

# ===== Run Motor Servo Tests =====
log_section "Running Motor Servo Tests"
if [[ -d "${MOTOR_SERVO_DIR}" ]] && cd "${MOTOR_SERVO_DIR}" && ./scripts/run_tests.sh; then
    log_pass "Motor Servo tests PASSED"
    MOTOR_SERVO_PASSED=1
else
    log_fail "Motor Servo tests FAILED"
fi

# ===== Run Speed Sensor Tests =====
log_section "Running Speed Sensor Tests"
if [[ -d "${SPEED_SENSOR_DIR}" ]] && cd "${SPEED_SENSOR_DIR}" && CALLED_FROM_MASTER=1 ./run_speedtest.sh; then
    log_pass "Speed Sensor tests PASSED"
    SPEED_SENSOR_PASSED=1
else
    log_fail "Speed Sensor tests FAILED"
fi

# ===== Aggregate Coverage =====
cd "${PROJECT_ROOT_DIR}"
COMBINED_LCOV="${MASTER_COVERAGE_DIR}/coverage_combined.info"
FILTERED_LCOV="${MASTER_COVERAGE_DIR}/coverage_filtered.info"

if [[ $MOTOR_SERVO_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    log_section "Aggregating Coverage Reports"

    # Find coverage files (support both build locations)
    FILE_1="${MOTOR_SERVO_DIR}/build/artifacts/gcov/coverage_filtered.info"
    if [[ -f "${SPEED_SENSOR_DIR}/coverage_filtered.info" ]]; then
        FILE_2="${SPEED_SENSOR_DIR}/coverage_filtered.info"
    else
        FILE_2="${SPEED_SENSOR_DIR}/build/artifacts/gcov/coverage_filtered.info"
    fi

    if [[ -f "$FILE_1" && -f "$FILE_2" ]]; then
        log_info "Merging tracefiles..."
        # Merge
        lcov -a "$FILE_1" -a "$FILE_2" -o "$COMBINED_LCOV" --rc lcov_branch_coverage=1

        # Filter (Source only)
        lcov -r "$COMBINED_LCOV" \
             '/usr/*' '*vendor*' '*cmock*' '*unity*' '*c_exception*' \
             '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' \
             -o "$FILTERED_LCOV" --rc lcov_branch_coverage=1 || cp "$COMBINED_LCOV" "$FILTERED_LCOV"

        # Generate HTML
        if command -v genhtml &> /dev/null; then
            genhtml -o "${MASTER_COVERAGE_DIR}/html" "$FILTERED_LCOV" \
                --title "DrivaPi Unified Coverage" \
                --branch-coverage --function-coverage \
                --rc genhtml_branch_coverage=1 > /dev/null
            log_pass "HTML Report: ${MASTER_COVERAGE_DIR}/html/index.html"
        fi
    else
        log_warn "One or more coverage files missing. Skipping aggregation."
    fi
fi

# ===== Generate TSF Artifacts (FIXED) =====
if [[ $MOTOR_SERVO_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    log_section "Generating TSF Artifacts"

    # --- 1. Coverage XML ---
    XML_COV_OUT="${ARTIFACTS_DIR}/coverage/coverage.xml"
    if [[ -f "$FILTERED_LCOV" ]]; then
        if command -v lcov_cobertura &> /dev/null; then
            lcov_cobertura "$FILTERED_LCOV" --output "$XML_COV_OUT"
            log_pass "Coverage XML: $XML_COV_OUT"
        else
            log_warn "lcov_cobertura missing. Using gcovr fallback..."
            gcovr --root . --filter ".*src/.*" --xml --output "$XML_COV_OUT" .
        fi
    fi

    # --- 2. JUnit XML Merging (Bash Only) ---
    XML_TEST_OUT="${ARTIFACTS_DIR}/tests/junit_results.xml"
    JUNIT_1="${MOTOR_SERVO_DIR}/build/artifacts/gcov/junit_tests_report.xml"
    JUNIT_2="${SPEED_SENSOR_DIR}/build/artifacts/gcov/junit_tests_report.xml"

    # Initialize combined XML file
    echo '<?xml version="1.0" encoding="UTF-8" ?>' > "$XML_TEST_OUT"
    echo '<testsuites>' >> "$XML_TEST_OUT"

    # Append Motor Servo (Skip first line '<?xml...', dump the rest)
    if [[ -f "$JUNIT_1" ]]; then
        tail -n +2 "$JUNIT_1" >> "$XML_TEST_OUT"
        log_info "Added Motor Servo tests"
    fi

    # Append Speed Sensor (Skip first line '<?xml...', dump the rest)
    if [[ -f "$JUNIT_2" ]]; then
        tail -n +2 "$JUNIT_2" >> "$XML_TEST_OUT"
        log_info "Added Speed Sensor tests"
    fi

    # Close the root tag
    echo '</testsuites>' >> "$XML_TEST_OUT"
    log_pass "Merged JUnit XML: $XML_TEST_OUT"
fi

# ===== Final Summary =====
echo ""
log_section "Test Summary"
log_info "Motor Servo:  $([ $MOTOR_SERVO_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"
log_info "Speed Sensor: $([ $SPEED_SENSOR_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"

if [[ $MOTOR_SERVO_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    exit 0
else
    exit 1
fi
