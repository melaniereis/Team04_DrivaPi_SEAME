#!/bin/bash
################################################################################
# ISO 26262 Unit Test Automation Script
#
# Purpose: Execute unit tests with robust coverage reporting
# ASIL Level: A/QM (project policy)
# Version: 1.1.1
################################################################################

set -e
set -u
set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE}")" && pwd)"
source "${SCRIPT_DIR}/../../common_test_lib.sh"

PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
COVERAGE_DIR="${BUILD_DIR}/artifacts/gcov"
REPORTS_DIR="${PROJECT_ROOT}/test_reports"
VENDOR_DIR="${PROJECT_ROOT}/vendor"

check_prerequisites() {
    log_header "Checking Prerequisites"

    local missing=()
    command -v ruby >/dev/null 2>&1 || missing+=("ruby")
    command -v gcc >/dev/null 2>&1 || missing+=("gcc")
    command -v gcov >/dev/null 2>&1 || missing+=("gcov")
    command -v lcov >/dev/null 2>&1 || missing+=("lcov")
    command -v genhtml >/dev/null 2>&1 || missing+=("genhtml")

    if ! gem list -i ceedling >/dev/null 2>&1; then
        missing+=("ceedling")
    fi

    if [[ ${#missing[@]} -gt 0 ]]; then
        log_fail "Missing tools: ${missing[*]}"
        exit 1
    fi

    log_success "All prerequisites satisfied"
}

generate_robust_coverage() {
    log_header "Generating Coverage Report"

    cd "${PROJECT_ROOT}"
    mkdir -p "${COVERAGE_DIR}"

    # Call the library function with paths relative to the suite root
    # This uses --ignore-errors to prevent blocking on non-fatal warnings
    generate_lcov_coverage "${BUILD_DIR}" "${COVERAGE_DIR}"

    # Sync result for dotstop validator and CI summary
    if [[ -f "${COVERAGE_DIR}/coverage.xml" ]]; then
        cp "${COVERAGE_DIR}/coverage.xml" "${REPORTS_DIR}/coverage.xml"
        log_info "Coverage XML synced to: ${REPORTS_DIR}/coverage.xml"
    fi
}

main() {
    # 1. Verification of environment
    check_prerequisites

    # 2. Cleanup and setup
    cleanup_build "${BUILD_DIR}"
    ensure_vendor "${VENDOR_DIR}"

    # 3. Test Execution (Unit Testing - Unity/Ceedling)
    # This fulfills the ISO 26262 Part 6 requirement for software unit testing [1]
    run_ceedling_tests "${REPORTS_DIR}/test_output.log"

    # 4. Coverage Analysis (Dynamic Analysis)
    # This fulfills ASIL A requirements for Statement Coverage [2]
    generate_robust_coverage

    log_success "All actions completed successfully"
    log_info "Coverage HTML: ${COVERAGE_DIR}/html/index.html"
}

main "$@"
