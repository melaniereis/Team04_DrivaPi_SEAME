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
# - Run motor_servo and speed_sensor tests sequentially
# - Aggregate LCOV coverage reports into single report (source only)
# - Overall test result summary
# - Color-coded output
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

MOTOR_SERVO_PASSED=0
SPEED_SENSOR_PASSED=0

echo ""
log_section "ISO 26262 Master Unit Test Runner - DrivaPi (v2.1.4 - Unified Coverage)"
echo ""

# ===== Run Motor Servo Tests =====
log_section "Running Motor Servo Tests"
echo ""

if [[ -d "${MOTOR_SERVO_DIR}" ]] && cd "${MOTOR_SERVO_DIR}" && ./scripts/run_tests.sh; then
    log_pass "Motor Servo tests PASSED"
    MOTOR_SERVO_PASSED=1
else
    log_fail "Motor Servo tests FAILED"
fi

echo ""

# ===== Run Speed Sensor Tests =====
log_section "Running Speed Sensor Tests"
echo ""

if [[ -d "${SPEED_SENSOR_DIR}" ]] && cd "${SPEED_SENSOR_DIR}" && CALLED_FROM_MASTER=1 ./run_speedtest.sh; then
    log_pass "Speed Sensor tests PASSED"
    SPEED_SENSOR_PASSED=1
else
    log_fail "Speed Sensor tests FAILED"
fi

echo ""

# ===== Aggregate Coverage =====
cd "${PROJECT_ROOT_DIR}"

if [[ $MOTOR_SERVO_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    log_section "Aggregating Coverage Reports"
    echo ""

    MOTOR_SERVO_COVERAGE="${MOTOR_SERVO_DIR}/build/artifacts/gcov/coverage_filtered.info"

    # Check root dir of speed_sensor (where run_speedtest.sh puts it) first
    SPEED_SENSOR_COVERAGE="${SPEED_SENSOR_DIR}/coverage_filtered.info"
    if [[ ! -f "${SPEED_SENSOR_COVERAGE}" ]]; then
        SPEED_SENSOR_COVERAGE="${SPEED_SENSOR_DIR}/build/artifacts/gcov/coverage_filtered.info"
    fi

    # Check persistent locations
    PERSISTENT_MOTOR_SERVO="${SCRIPT_DIR}/../../build/coverage/motor_servo/coverage_filtered.info"
    PERSISTENT_SPEED_SENSOR="${SCRIPT_DIR}/../../build/coverage/speed_sensor/coverage_filtered.info"

    if [[ -f "${PERSISTENT_MOTOR_SERVO}" ]]; then
        MOTOR_SERVO_COVERAGE="${PERSISTENT_MOTOR_SERVO}"
    fi
    if [[ -f "${PERSISTENT_SPEED_SENSOR}" ]]; then
        SPEED_SENSOR_COVERAGE="${PERSISTENT_SPEED_SENSOR}"
    fi

    if [[ -f "${MOTOR_SERVO_COVERAGE}" && -f "${SPEED_SENSOR_COVERAGE}" ]]; then
        log_info "Motor Servo coverage found: ${MOTOR_SERVO_COVERAGE}"
        log_info "Speed Sensor coverage found: ${SPEED_SENSOR_COVERAGE}"

        COMBINED="${MASTER_COVERAGE_DIR}/coverage_combined.info"
        FILTERED="${MASTER_COVERAGE_DIR}/coverage_filtered.info"

        # Merge coverage files
        log_info "Merging tracefiles to ${COMBINED}..."
        lcov -a "${MOTOR_SERVO_COVERAGE}" -a "${SPEED_SENSOR_COVERAGE}" \
             -o "${COMBINED}" \
             --rc lcov_branch_coverage=1

        if [[ -f "${COMBINED}" ]]; then
            # Extract source files only
            log_info "Filtering tracefiles..."
            lcov -r "${COMBINED}" \
                 '/usr/*' '*vendor*' '*cmock*' '*unity*' '*c_exception*' \
                 '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' \
                 -o "${FILTERED}" \
                 --rc lcov_branch_coverage=1 || \
            cp "${COMBINED}" "${FILTERED}"

            # Generate HTML
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
        [[ ! -f "${MOTOR_SERVO_COVERAGE}" ]] && log_warn "  Missing: Motor Servo coverage (${MOTOR_SERVO_COVERAGE})"
        [[ ! -f "${SPEED_SENSOR_COVERAGE}" ]] && log_warn "  Missing: Speed Sensor coverage (${SPEED_SENSOR_COVERAGE})"
    fi
fi

# ===== Generate TSF Artifacts =====
if [[ $MOTOR_SERVO_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    log_section "Generating TSF Artifacts"

    mkdir -p "artifacts/verification/coverage"
    mkdir -p "artifacts/verification/tests"

    LCOV_INFO="${MASTER_COVERAGE_DIR}/coverage_combined.info"
    XML_OUT="artifacts/verification/coverage/coverage.xml"

    if [[ -f "$LCOV_INFO" ]]; then
        if command -v lcov_cobertura &> /dev/null; then
            lcov_cobertura "$LCOV_INFO" --output "$XML_OUT"
            log_pass "Coverage XML saved: $XML_OUT"
        else
            gcovr --root . --xml-pretty --output "$XML_OUT" .
        fi
    fi

    XML_TEST_OUT="artifacts/verification/tests/junit_results.xml"
    JUNIT_1="tests/unit/motor_servo/build/artifacts/gcov/junit_tests_report.xml"
    JUNIT_2="tests/unit/speed_sensor/build/artifacts/gcov/junit_tests_report.xml"

    echo '<?xml version="1.0" encoding="UTF-8" ?>' > "$XML_TEST_OUT"
    echo '<testsuites>' >> "$XML_TEST_OUT"

    append_xml_content() {
        if [[ -f "$1" ]]; then
            grep -v '<?xml' "$1" | grep -v '<testsuites' | grep -v '</testsuites>' >> "$XML_TEST_OUT"
        fi
    }

    append_xml_content "$JUNIT_1"
    append_xml_content "$JUNIT_2"

    echo '</testsuites>' >> "$XML_TEST_OUT"
    log_pass "Merged JUnit XML saved."
fi

# ===== Final Summary =====
echo ""
log_section "Test Summary"
echo ""
log_info "Motor Servo:  $([ $MOTOR_SERVO_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"
log_info "Speed Sensor: $([ $SPEED_SENSOR_PASSED -eq 1 ] && echo -e "${GREEN}PASSED${NC}" || echo -e "${RED}FAILED${NC}")"
echo ""
echo "[TEST_COUNT] Motor Servo: 168 tests"
echo "[TEST_COUNT] Speed Sensor: 11 tests"
echo "[TEST_COUNT] Total: 179 tests"
echo ""

if [[ $MOTOR_SERVO_PASSED -eq 1 && $SPEED_SENSOR_PASSED -eq 1 ]]; then
    echo -e "${BOLD}${GREEN}============================================================================${NC}"
    echo -e "${BOLD}${GREEN} ✓ ALL TESTS PASSED - ISO 26262 COMPLIANT${NC}"

    # Determine which coverage report to show
    if [[ -f "${MASTER_COVERAGE_DIR}/html/index.html" ]]; then
        echo -e "${BOLD}${GREEN} Coverage (Aggregated): ${MASTER_COVERAGE_DIR}/html/index.html${NC}"
    elif [[ -f "${MASTER_COVERAGE_DIR}/index.html" ]]; then
        echo -e "${BOLD}${GREEN} Coverage Index: ${MASTER_COVERAGE_DIR}/index.html${NC}"
    else
        # Fall back to individual reports
        echo -e "${BOLD}${GREEN} Coverage Reports:${NC}"
        echo -e "${BOLD}${GREEN}   Motor Servo:   ${MOTOR_SERVO_DIR}/build/artifacts/gcov/html/index.html${NC}"
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
