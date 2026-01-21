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

    cd "${DC_MOTOR_DIR}" && ./scripts/run_tests.sh 2>&1 | tee "$DC_MOTOR_OUTPUT" && dc_pass=1 || true
    [[ $dc_pass -eq 1 ]] && log_pass "DC Motor: PASSED" || log_fail "DC Motor: FAILED"
    echo ""

    cd "${SERVO_MOTOR_DIR}" && ./scripts/run_tests.sh 2>&1 | tee "$SERVO_MOTOR_OUTPUT" && servo_pass=1 || true
    [[ $servo_pass -eq 1 ]] && log_pass "Servo Motor: PASSED" || log_fail "Servo Motor: FAILED"
    echo ""

    cd "${SPEED_SENSOR_DIR}" && CALLED_FROM_MASTER=1 ./scripts/run_speedtest.sh 2>&1 | tee "$SPEED_SENSOR_OUTPUT" && speed_pass=1 || true
    [[ $speed_pass -eq 1 ]] && log_pass "Speed Sensor: PASSED" || log_fail "Speed Sensor: FAILED"
    echo ""

    # Generate reports
    generate_junit_xml "$DC_MOTOR_OUTPUT" "${ARTIFACTS_DIR}/tests/dc-motor.xml"
    generate_junit_xml "$SERVO_MOTOR_OUTPUT" "${ARTIFACTS_DIR}/tests/servo-motor.xml"
    generate_junit_xml "$SPEED_SENSOR_OUTPUT" "${ARTIFACTS_DIR}/tests/speed-sensor.xml"
    
    merge_junit_xml "${ARTIFACTS_DIR}/tests/junit_results.xml" \
        "${ARTIFACTS_DIR}/tests/dc-motor.xml" \
        "${ARTIFACTS_DIR}/tests/servo-motor.xml" \
        "${ARTIFACTS_DIR}/tests/speed-sensor.xml"

    # Generate test summary report
    cat > "${ARTIFACTS_DIR}/tests/summary.json" << EOF
{
  "timestamp": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
  "tests": {
    "dc_motor": {"status": "$([ $dc_pass -eq 1 ] && echo 'PASSED' || echo 'FAILED')"},
    "servo_motor": {"status": "$([ $servo_pass -eq 1 ] && echo 'PASSED' || echo 'FAILED')"},
    "speed_sensor": {"status": "$([ $speed_pass -eq 1 ] && echo 'PASSED' || echo 'FAILED')"}
  },
  "overall_status": "$([ $dc_pass -eq 1 ] && [ $servo_pass -eq 1 ] && [ $speed_pass -eq 1 ] && echo 'PASSED' || echo 'FAILED')"
}
EOF

    log_success "Test reports generated: ${ARTIFACTS_DIR}/tests/"

    # Aggregate coverage if all passed
    if [[ $dc_pass -eq 1 && $servo_pass -eq 1 && $speed_pass -eq 1 ]]; then
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

        if [[ -f "$dc_cov" && -f "$servo_cov" && -f "$speed_cov" ]]; then
            aggregate_coverage "${MASTER_COVERAGE_DIR}" "$dc_cov" "$servo_cov" "$speed_cov"
            
            # Copy aggregated coverage report to artifacts
            cp "${MASTER_COVERAGE_DIR}/coverage_filtered.info" "${ARTIFACTS_DIR}/coverage/coverage.info"
            log_success "Coverage report saved: ${ARTIFACTS_DIR}/coverage/"
        else
            log_warn "Some coverage files not available"
        fi

        echo ""
        log_section "✓ ALL TESTS PASSED - ISO 26262 COMPLIANT"
        log_info "Reports: ${ARTIFACTS_DIR}/"
        log_info "Test Results: ${ARTIFACTS_DIR}/tests/junit_results.xml"
        log_info "Coverage: ${MASTER_COVERAGE_DIR}/html/index.html"
        exit 0
    else
        echo ""
        log_section "✗ SOME TESTS FAILED"
        log_info "Reports: ${ARTIFACTS_DIR}/"
        exit 1
    fi
}

main "$@"
