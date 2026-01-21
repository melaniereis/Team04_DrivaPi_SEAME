#!/bin/bash
################################################################################
# DC Motor Unit Test Automation
#
# Purpose: Execute DC motor unit tests with coverage reporting
# ASIL Level: B/D
# Version: 1.1.0
################################################################################

set -e
set -u
set -o pipefail

# Source common library
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/../../common_test_lib.sh"

PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
COVERAGE_DIR="${BUILD_DIR}/artifacts/gcov"
REPORTS_DIR="${PROJECT_ROOT}/test_reports"
VENDOR_DIR="${PROJECT_ROOT}/vendor"

mkdir -p "${REPORTS_DIR}"

# ============================================================================
# MAIN
# ============================================================================

main() {
    local start_time=$(date +%s)

    log_section "ISO 26262 DC Motor Tests - DrivaPi"
    echo -e "${BOLD}ASIL Level:${NC} B/D"
    echo -e "${BOLD}Coverage Requirement:${NC} 100% Branch Coverage"
    echo ""

    check_prerequisites || exit 1
    cleanup_build "${BUILD_DIR}"
    ensure_vendor "${VENDOR_DIR}" || exit 1

    cd "${PROJECT_ROOT}"
    run_ceedling_tests "${REPORTS_DIR}/test_output.log" || exit 1

    generate_lcov_coverage "${BUILD_DIR}" "${COVERAGE_DIR}"

    # Save coverage for aggregation
    ABSOLUTE_ROOT="$(cd "${PROJECT_ROOT}/../.." && pwd)"
    PERSISTENT_DIR="${ABSOLUTE_ROOT}/build/coverage/dc-motor"
    mkdir -p "${PERSISTENT_DIR}"
    [[ -f "${COVERAGE_DIR}/coverage_filtered.info" ]] && \
        cp "${COVERAGE_DIR}/coverage_filtered.info" "${PERSISTENT_DIR}/coverage_filtered.info"

    local end_time=$(date +%s)
    local duration=$((end_time - start_time))

    echo ""
    log_section "✓ DC MOTOR TESTS PASSED"
    log_info "Execution time: ${duration}s"
}

main "$@"
