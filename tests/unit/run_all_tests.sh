#!/bin/bash
################################################################################
# Master Unit Test Automation Script with Aggregated Coverage
#
# Purpose: Execute all unit tests and generate unified coverage report
# ASIL Level: B/D
# Author: DrivaPi Team
# Version: 2.1.4
#
# Features:
# - Run dc-motor, servo-motor and speed-sensor tests sequentially
# - Aggregate LCOV coverage reports into single report (source only)
# - Overall test result summary
# - Color-coded output
################################################################################

set -e
set -u
set -o pipefail

readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly PROJECT_ROOT_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"

readonly DC_MOTOR_DIR="${SCRIPT_DIR}/dc-motor"
readonly SERVO_MOTOR_DIR="${SCRIPT_DIR}/servo-motor"
readonly SPEED_SENSOR_DIR="${SCRIPT_DIR}/speed-sensor"

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

mkdir -p "${MASTER_COVERAGE_DIR}"
mkdir -p "${ARTIFACTS_DIR}/tests"
mkdir -p "${ARTIFACTS_DIR}/coverage"

if [[ -t 1 ]]; then
    readonly RED='\033[0;31m'
    readonly GREEN='\033[0;32m'
    readonly YELLOW='\033[1;33m'
    readonly BLUE='\033[0;34m'
    readonly CYAN='\033[0;36m'
    readonly BOLD='\033[1m'
    readonly NC='\033[0m'
else
    readonly RED='' GREEN='' YELLOW='' BLUE='' CYAN='' BOLD='' NC=''
fi

log_section() {
    echo -e "${BOLD}${BLUE}============================================================================${NC}"
    echo -e "${BOLD}${BLUE} $1${NC}"
    echo -e "${BOLD}${BLUE}============================================================================${NC}"
}

log_pass() {
    echo -e "${GREEN}✓ $1${NC}"
}

log_fail() {
    echo -e "${RED}✗ $1${NC}"
}

log_info() {
    echo -e "${CYAN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

DC_MOTOR_PASSED=0
SERVO_MOTOR_PASSED=0
SPEED_SENSOR_PASSED=0

DC_MOTOR_TEST_COUNT=0
SERVO_MOTOR_TEST_COUNT=0
SPEED_SENSOR_TEST_COUNT=0

echo ""
log_section "ISO 26262 Master Unit Test Runner - DrivaPi (v2.1.4 - Unified Coverage)"
echo ""

# ===== Run DC Motor Tests =====
log_section "Running DC Motor Tests"
echo ""

DC_MOTOR_OUTPUT=$(mktemp)
if [[ -d "${DC_MOTOR_DIR}" ]] && cd "${DC_MOTOR_DIR}" && ./scripts/run_tests.sh 2>&1 | tee "$DC_MOTOR_OUTPUT"; then
    log_pass "DC Motor tests PASSED"
    DC_MOTOR_PASSED=1
    # Sum all test counts from all test files (format: "X Tests Y Failures Z Ignored")
    DC_MOTOR_TEST_COUNT=$(grep -oP '^\d+(?= Tests)' "$DC_MOTOR_OUTPUT" | awk '{s+=$1} END {print s}')
else
    log_fail "DC Motor tests FAILED"
fi
rm -f "$DC_MOTOR_OUTPUT"

echo ""

# ===== Run Servo Motor Tests =====
log_section "Running Servo Motor Tests"
echo ""

SERVO_MOTOR_OUTPUT=$(mktemp)
if [[ -d "${SERVO_MOTOR_DIR}" ]] && cd "${SERVO_MOTOR_DIR}" && ./scripts/run_tests.sh 2>&1 | tee "$SERVO_MOTOR_OUTPUT"; then
    log_pass "Servo Motor tests PASSED"
    SERVO_MOTOR_PASSED=1
    # Sum all test counts from all test files
    SERVO_MOTOR_TEST_COUNT=$(grep -oP '^\d+(?= Tests)' "$SERVO_MOTOR_OUTPUT" | awk '{s+=$1} END {print s}')
else
    log_fail "Servo Motor tests FAILED"
fi
rm -f "$SERVO_MOTOR_OUTPUT"

echo ""

# ===== Run Speed Sensor Tests =====
log_section "Running Speed Sensor Tests"
echo ""

SPEED_SENSOR_OUTPUT=$(mktemp)
if [[ -d "${SPEED_SENSOR_DIR}" ]] && cd "${SPEED_SENSOR_DIR}" && CALLED_FROM_MASTER=1 ./scripts/run_speedtest.sh 2>&1 | tee "$SPEED_SENSOR_OUTPUT"; then
    log_pass "Speed Sensor tests PASSED"
    SPEED_SENSOR_PASSED=1
    # Extract test count from output
    SPEED_SENSOR_TEST_COUNT=$(grep -oP '^\d+(?= Tests)' "$SPEED_SENSOR_OUTPUT" | awk '{s+=$1} END {print s}')
else
    log_fail "Speed Sensor tests FAILED"
fi
rm -f "$SPEED_SENSOR_OUTPUT"

echo ""

# ===== Aggregate Coverage =====
cd "${PROJECT_ROOT_DIR}"

if [[ $DC_MOTOR_PASSED -eq 1 && $SERVO_MOTOR_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    log_section "Aggregating Coverage Reports"
    echo ""

    DC_MOTOR_COVERAGE="${DC_MOTOR_DIR}/build/artifacts/gcov/coverage_filtered.info"
    SERVO_MOTOR_COVERAGE="${SERVO_MOTOR_DIR}/build/artifacts/gcov/coverage_filtered.info"

    SPEED_SENSOR_COVERAGE="${SPEED_SENSOR_DIR}/coverage_filtered.info"
    if [[ ! -f "${SPEED_SENSOR_COVERAGE}" ]]; then
        SPEED_SENSOR_COVERAGE="${SPEED_SENSOR_DIR}/build/artifacts/gcov/coverage_filtered.info"
    fi

    PERSISTENT_DC_MOTOR="${SCRIPT_DIR}/../../build/coverage/dc-motor/coverage_filtered.info"
    PERSISTENT_SERVO_MOTOR="${SCRIPT_DIR}/../../build/coverage/servo-motor/coverage_filtered.info"
    PERSISTENT_SPEED_SENSOR="${SCRIPT_DIR}/../../build/coverage/speed-sensor/coverage_filtered.info"

    if [[ -f "${PERSISTENT_DC_MOTOR}" ]]; then
        DC_MOTOR_COVERAGE="${PERSISTENT_DC_MOTOR}"
    fi
    if [[ -f "${PERSISTENT_SERVO_MOTOR}" ]]; then
        SERVO_MOTOR_COVERAGE="${PERSISTENT_SERVO_MOTOR}"
    fi
    if [[ -f "${PERSISTENT_SPEED_SENSOR}" ]]; then
        SPEED_SENSOR_COVERAGE="${PERSISTENT_SPEED_SENSOR}"
    fi

    if [[ -f "${DC_MOTOR_COVERAGE}" && -f "${SERVO_MOTOR_COVERAGE}" && -f "${SPEED_SENSOR_COVERAGE}" ]]; then
        log_info "DC Motor coverage found: ${DC_MOTOR_COVERAGE}"
        log_info "Servo Motor coverage found: ${SERVO_MOTOR_COVERAGE}"
        log_info "Speed Sensor coverage found: ${SPEED_SENSOR_COVERAGE}"

        COMBINED="${MASTER_COVERAGE_DIR}/coverage_combined.info"
        FILTERED="${MASTER_COVERAGE_DIR}/coverage_filtered.info"

        log_info "Merging tracefiles to ${COMBINED}..."
        lcov -a "${DC_MOTOR_COVERAGE}" -a "${SERVO_MOTOR_COVERAGE}" -a "${SPEED_SENSOR_COVERAGE}" \
             -o "${COMBINED}" \
             --rc lcov_branch_coverage=1

        if [[ -f "${COMBINED}" ]]; then
            log_info "Filtering tracefiles..."
            lcov -r "${COMBINED}" \
                 '/usr/*' '*vendor*' '*cmock*' '*unity*' '*c_exception*' \
                 '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' \
                 '*/common/*' \
                 -o "${FILTERED}" \
                 --rc lcov_branch_coverage=1 || \
            cp "${COMBINED}" "${FILTERED}"

            if command -v genhtml &> /dev/null; then
                HTML="${MASTER_COVERAGE_DIR}/html"
                log_info "Generating HTML to ${HTML}..."
                genhtml -o "${HTML}" "${FILTERED}" \
                    --title "DrivaPi - Unified Test Coverage" \
                    --branch-coverage \
                    --function-coverage \
                    --rc genhtml_branch_coverage=1 \
                    --demangle-cpp > /dev/null

                if [[ -f "${HTML}/index.html" ]]; then
                    log_pass "Coverage report generated: ${HTML}/index.html"
                else
                    log_warn "HTML generation failed"
                fi
            else
                log_warn "genhtml not found"
            fi

            echo ""
            log_info "Combined Coverage Summary:"
            lcov --summary "${FILTERED}" --rc lcov_branch_coverage=1 || true
            echo ""
        else
            log_fail "Failed to create combined coverage file at ${COMBINED}"
        fi

    else
        log_warn "Coverage files not available for aggregation"
        [[ ! -f "${DC_MOTOR_COVERAGE}" ]] && log_warn "  Missing: DC Motor coverage (${DC_MOTOR_COVERAGE})"
        [[ ! -f "${SERVO_MOTOR_COVERAGE}" ]] && log_warn "  Missing: Servo Motor coverage (${SERVO_MOTOR_COVERAGE})"
        [[ ! -f "${SPEED_SENSOR_COVERAGE}" ]] && log_warn "  Missing: Speed Sensor coverage (${SPEED_SENSOR_COVERAGE})"
    fi
fi

# ===== Generate TSF Artifacts =====
if [[ $DC_MOTOR_PASSED -eq 1 && $SERVO_MOTOR_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    log_section "Generating TSF Artifacts"

    mkdir -p "artifacts/verification/coverage"
    mkdir -p "artifacts/verification/tests"

    LCOV_INFO="${MASTER_COVERAGE_DIR}/coverage_combined.info"
    XML_OUT="artifacts/verification/coverage/coverage.xml"

    if [[ -f "$LCOV_INFO" ]]; then
        gcovr --root . --xml-pretty --output "$XML_OUT" .
        log_pass "Coverage XML saved: $XML_OUT"
    fi

    XML_TEST_OUT="artifacts/verification/tests/junit_results.xml"
    JUNIT_1="tests/unit/dc-motor/build/artifacts/gcov/junit_tests_report.xml"
    JUNIT_2="tests/unit/servo-motor/build/artifacts/gcov/junit_tests_report.xml"
    JUNIT_3="tests/unit/speed-sensor/build/artifacts/gcov/junit_tests_report.xml"

    echo '<?xml version="1.0" encoding="UTF-8" ?>' > "$XML_TEST_OUT"
    echo '<testsuites>' >> "$XML_TEST_OUT"

    append_xml_content() {
        if [[ -f "$1" ]]; then
            grep -v '<?xml' "$1" | grep -v '<testsuites' | grep -v '</testsuites>' >> "$XML_TEST_OUT"
        fi
    }

    append_xml_content "$JUNIT_1"
    append_xml_content "$JUNIT_2"
    append_xml_content "$JUNIT_3"

    echo '</testsuites>' >> "$XML_TEST_OUT"
    log_pass "Merged JUnit XML saved."
fi

# ===== Final Summary =====
echo ""
log_section "Test Summary"
echo ""
log_info "DC Motor:     $([ $DC_MOTOR_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"
log_info "Servo Motor:  $([ $SERVO_MOTOR_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"
log_info "Speed Sensor: $([ $SPEED_SENSOR_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"
echo ""

# Calculate total test count
TOTAL_TEST_COUNT=$((DC_MOTOR_TEST_COUNT + SERVO_MOTOR_TEST_COUNT + SPEED_SENSOR_TEST_COUNT))

echo "[TEST_COUNT] DC Motor: ${DC_MOTOR_TEST_COUNT} tests"
echo "[TEST_COUNT] Servo Motor: ${SERVO_MOTOR_TEST_COUNT} tests"
echo "[TEST_COUNT] Speed Sensor: ${SPEED_SENSOR_TEST_COUNT} tests"
echo "[TEST_COUNT] Total: ${TOTAL_TEST_COUNT} tests"
echo ""

if [[ $DC_MOTOR_PASSED -eq 1 && $SERVO_MOTOR_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    echo -e "${BOLD}${GREEN}============================================================================${NC}"
    echo -e "${BOLD}${GREEN} ✓ ALL TESTS PASSED - ISO 26262 COMPLIANT${NC}"

    if [[ -f "${MASTER_COVERAGE_DIR}/html/index.html" ]]; then
        echo -e "${BOLD}${GREEN} Coverage (Aggregated): ${MASTER_COVERAGE_DIR}/html/index.html${NC}"
    elif [[ -f "${MASTER_COVERAGE_DIR}/index.html" ]]; then
        echo -e "${BOLD}${GREEN} Coverage Index: ${MASTER_COVERAGE_DIR}/index.html${NC}"
    else
        echo -e "${BOLD}${GREEN} Coverage Reports:${NC}"
        echo -e "${BOLD}${GREEN}   DC Motor:      ${DC_MOTOR_DIR}/build/artifacts/gcov/html/index.html${NC}"
        echo -e "${BOLD}${GREEN}   Servo Motor:   ${SERVO_MOTOR_DIR}/build/artifacts/gcov/html/index.html${NC}"
        echo -e "${BOLD}${GREEN}   Speed Sensor:  ${SPEED_SENSOR_DIR}/coverage_report/index.html${NC}"
    fi

    echo -e "${BOLD}${GREEN}============================================================================${NC}"
    exit 0
else
    echo -e "${BOLD}${RED}============================================================================${NC}"
    echo -e "${BOLD}${RED} ✗ SOME TESTS FAILED${NC}"
    echo -e "${BOLD}${RED}============================================================================${NC}"
    exit 1
fi
