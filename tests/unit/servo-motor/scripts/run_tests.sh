#!/bin/bash
################################################################################
# ISO 26262 Unit Test Automation Script
#
# Purpose: Execute unit tests with 100% branch coverage validation
# ASIL Level: B/D
################################################################################

set -e
set -u
set -o pipefail

readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
readonly BUILD_DIR="${PROJECT_ROOT}/build"
readonly COVERAGE_DIR="${BUILD_DIR}/artifacts/gcov"
readonly REPORTS_DIR="${PROJECT_ROOT}/test_reports"
readonly VENDOR_DIR="${PROJECT_ROOT}/vendor"
readonly COMMON_LIB="${SCRIPT_DIR}/../../common_test_lib.sh"

source "${COMMON_LIB}"

main() {
    local start_time
    start_time=$(date +%s)

    log_header "ISO 26262 Unit Test Automation - DrivaPi"
    echo -e "${BOLD}ASIL Level:${NC} B/D"
    echo -e "${BOLD}Coverage Requirement:${NC} 100% Branch Coverage"
    echo ""

    check_prerequisites
    cd "${PROJECT_ROOT}"
    cleanup_build "${BUILD_DIR}"
    rm -rf "${REPORTS_DIR}"
    mkdir -p "${REPORTS_DIR}" "${BUILD_DIR}/test/mocks" "${BUILD_DIR}/gcov/out"

    ensure_vendor "${VENDOR_DIR}"
    run_ceedling_tests "${REPORTS_DIR}/test_output.log"
    print_speed_sensor_coverage_note
    generate_lcov_coverage "${BUILD_DIR}" "${COVERAGE_DIR}"
    generate_sonarqube_xml_report "${PROJECT_ROOT}" "${BUILD_DIR}" "${REPORTS_DIR}/coverage-sonar.xml"
    save_coverage_for_module "${COVERAGE_DIR}" "${PROJECT_ROOT}" "servo-motor"

    if ! validate_coverage_file "${COVERAGE_DIR}"; then
        log_fail "VALIDATION FAILED"
        exit 1
    fi

    local end_time
    end_time=$(date +%s)
    local duration=$((end_time - start_time))

    echo ""
    log_header "✓ ALL TESTS PASSED - ISO 26262 COMPLIANT"
    log_info "Execution time: ${duration}s"
    log_info "Reports: ${REPORTS_DIR}"
    log_info "Coverage HTML: ${COVERAGE_DIR}/html/index.html"
}

main "$@"
