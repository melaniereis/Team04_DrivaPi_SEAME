#!/bin/bash
################################################################################
# Master Unit Test Automation Script
#
# Purpose: Execute all unit tests and generate unified coverage report
# ASIL Level: B/D
# Version: 2.2.0
################################################################################

set -e
set -u
set -o pipefail

# Source common library
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/common_test_lib.sh"

# Cleanup temp files
trap 'rm -f "${DC_MOTOR_OUTPUT:-}" "${SERVO_MOTOR_OUTPUT:-}" "${SPEED_SENSOR_OUTPUT:-}"' EXIT INT TERM

readonly PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
readonly DC_MOTOR_DIR="${SCRIPT_DIR}/dc-motor"
readonly SERVO_MOTOR_DIR="${SCRIPT_DIR}/servo-motor"
readonly SPEED_SENSOR_DIR="${SCRIPT_DIR}/speed-sensor"
readonly ARTIFACTS_DIR="${PROJECT_ROOT}/artifacts/verification"

# Coverage directory
MASTER_COVERAGE_DIR="${SCRIPT_DIR}/coverage"
[[ -n "${COVERAGE_DIR:-}" ]] && MASTER_COVERAGE_DIR="$COVERAGE_DIR"
[[ "$MASTER_COVERAGE_DIR" != /* ]] && MASTER_COVERAGE_DIR="${PROJECT_ROOT}/${MASTER_COVERAGE_DIR}"

mkdir -p "${MASTER_COVERAGE_DIR}"
mkdir -p "${ARTIFACTS_DIR}/tests"
mkdir -p "${ARTIFACTS_DIR}/coverage"

# ============================================================================
# MAIN
# ============================================================================

main() {
    echo ""
    log_section "ISO 26262 Master Test Runner - DrivaPi (v2.2.0)"
    echo ""

    # Run all test suites
    local dc_pass=0 servo_pass=0 speed_pass=0
    DC_MOTOR_OUTPUT=$(mktemp)
    SERVO_MOTOR_OUTPUT=$(mktemp)
    SPEED_SENSOR_OUTPUT=$(mktemp)

    (cd "${DC_MOTOR_DIR}" && ./scripts/run_tests.sh > >(tee "$DC_MOTOR_OUTPUT") 2>&1) || dc_pass=$?
    [[ ${dc_pass:-0} -eq 0 ]] && log_pass "DC Motor: PASSED" || log_fail "DC Motor: FAILED"
    echo ""

    (cd "${SERVO_MOTOR_DIR}" && ./scripts/run_tests.sh > >(tee "$SERVO_MOTOR_OUTPUT") 2>&1) || servo_pass=$?
    [[ ${servo_pass:-0} -eq 0 ]] && log_pass "Servo Motor: PASSED" || log_fail "Servo Motor: FAILED"
    echo ""

    (cd "${SPEED_SENSOR_DIR}" && CALLED_FROM_MASTER=1 ./scripts/run_speedtest.sh > >(tee "$SPEED_SENSOR_OUTPUT") 2>&1) || speed_pass=$?
    [[ ${speed_pass:-0} -eq 0 ]] && log_pass "Speed Sensor: PASSED" || log_fail "Speed Sensor: FAILED"
    echo ""

    # Generate reports
    generate_junit_xml "$DC_MOTOR_OUTPUT" "${ARTIFACTS_DIR}/tests/dc-motor.xml"
    generate_junit_xml "$SERVO_MOTOR_OUTPUT" "${ARTIFACTS_DIR}/tests/servo-motor.xml"
    generate_junit_xml "$SPEED_SENSOR_OUTPUT" "${ARTIFACTS_DIR}/tests/speed-sensor.xml"
    
    merge_junit_xml "${ARTIFACTS_DIR}/tests/junit_results.xml" \
        "${ARTIFACTS_DIR}/tests/dc-motor.xml" \
        "${ARTIFACTS_DIR}/tests/servo-motor.xml" \
        "${ARTIFACTS_DIR}/tests/speed-sensor.xml"

        # Count tests for workflow parsing
        local dc_tests=$(count_tests "$DC_MOTOR_OUTPUT")
        local servo_tests=$(count_tests "$SERVO_MOTOR_OUTPUT")
        local speed_tests=$(count_tests "$SPEED_SENSOR_OUTPUT")

        # Generate test summary report
    cat > "${ARTIFACTS_DIR}/tests/summary.json" << EOF
{
  "timestamp": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
  "tests": {
        "dc_motor": {"status": "$([ $dc_pass -eq 0 ] && echo 'PASSED' || echo 'FAILED')"},
        "servo_motor": {"status": "$([ $servo_pass -eq 0 ] && echo 'PASSED' || echo 'FAILED')"},
        "speed_sensor": {"status": "$([ $speed_pass -eq 0 ] && echo 'PASSED' || echo 'FAILED')"}
  },
    "overall_status": "$([ $dc_pass -eq 0 ] && [ $servo_pass -eq 0 ] && [ $speed_pass -eq 0 ] && echo 'PASSED' || echo 'FAILED')"
}
EOF

        # Console summary + markers for workflow parsing
        local dc_status=$([ $dc_pass -eq 0 ] && echo "PASSED" || echo "FAILED")
        local servo_status=$([ $servo_pass -eq 0 ] && echo "PASSED" || echo "FAILED")
        local speed_status=$([ $speed_pass -eq 0 ] && echo "PASSED" || echo "FAILED")
        echo ""
        echo "[TEST_COUNT] DC Motor: ${dc_tests} tests"
        echo "[TEST_COUNT] Servo Motor: ${servo_tests} tests"
        echo "[TEST_COUNT] Speed Sensor: ${speed_tests} tests"
        echo "[INFO] DC Motor: ${dc_status}"
        echo "[INFO] Servo Motor: ${servo_status}"
        echo "[INFO] Speed Sensor: ${speed_status}"
        log_info "Summary: DC Motor=${dc_status}, Servo Motor=${servo_status}, Speed Sensor=${speed_status}"

    log_success "Test reports generated: ${ARTIFACTS_DIR}/tests/"

    # Generate coverage reports (regardless of test status)
    local dc_cov="${DC_MOTOR_DIR}/build/artifacts/gcov/coverage_filtered.info"
    local servo_cov="${SERVO_MOTOR_DIR}/build/artifacts/gcov/coverage_filtered.info"
    local speed_cov="${SPEED_SENSOR_DIR}/coverage_filtered.info"

    # Check persistent locations
    [[ -f "${PROJECT_ROOT}/build/coverage/dc-motor/coverage_filtered.info" ]] && \
        dc_cov="${PROJECT_ROOT}/build/coverage/dc-motor/coverage_filtered.info"
    [[ -f "${PROJECT_ROOT}/build/coverage/servo-motor/coverage_filtered.info" ]] && \
        servo_cov="${PROJECT_ROOT}/build/coverage/servo-motor/coverage_filtered.info"
    [[ -f "${PROJECT_ROOT}/build/coverage/speed-sensor/coverage_filtered.info" ]] && \
        speed_cov="${PROJECT_ROOT}/build/coverage/speed-sensor/coverage_filtered.info"

    # Generate individual coverage XML files for each suite
    mkdir -p "${ARTIFACTS_DIR}/coverage"
    if [[ -f "$dc_cov" ]]; then
        generate_coverage_xml "$dc_cov" "${ARTIFACTS_DIR}/coverage/dc-motor.xml" || true
    fi
    if [[ -f "$servo_cov" ]]; then
        generate_coverage_xml "$servo_cov" "${ARTIFACTS_DIR}/coverage/servo-motor.xml" || true
    fi
    if [[ -f "$speed_cov" ]]; then
        generate_coverage_xml "$speed_cov" "${ARTIFACTS_DIR}/coverage/speed-sensor.xml" || true
    fi

    # Aggregate coverage if files available
    if [[ -f "$dc_cov" && -f "$servo_cov" && -f "$speed_cov" ]]; then
        aggregate_coverage "${MASTER_COVERAGE_DIR}" "$dc_cov" "$servo_cov" "$speed_cov"
        
        # Copy aggregated coverage reports to artifacts
        cp "${MASTER_COVERAGE_DIR}/coverage_filtered.info" "${ARTIFACTS_DIR}/coverage/coverage.info"
        [[ -f "${MASTER_COVERAGE_DIR}/coverage.xml" ]] && cp "${MASTER_COVERAGE_DIR}/coverage.xml" "${ARTIFACTS_DIR}/coverage/coverage.xml"
        log_success "Coverage reports saved: ${ARTIFACTS_DIR}/coverage/"
    else
        log_warn "Some coverage files not available for aggregation"
    fi

    echo ""
    if [[ $dc_pass -eq 0 && $servo_pass -eq 0 && $speed_pass -eq 0 ]]; then
        log_section "✓ ALL TESTS PASSED - ISO 26262 COMPLIANT"
        log_info "Reports: ${ARTIFACTS_DIR}/"
        log_info "Test Results: ${ARTIFACTS_DIR}/tests/junit_results.xml"
        log_info "Coverage: ${MASTER_COVERAGE_DIR}/html/index.html"
        exit 0
    else
        log_section "✗ SOME TESTS FAILED"
        log_info "Reports: ${ARTIFACTS_DIR}/"
        log_info "Coverage reports still generated at: ${ARTIFACTS_DIR}/coverage/"
        exit 1
    fi
}

main "$@"
