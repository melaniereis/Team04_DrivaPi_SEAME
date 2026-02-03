#!/bin/bash
################################################################################
# ISO 26262 Unit Test Automation Script - Robust Version
#
# Purpose: Execute unit tests with robust coverage reporting
# ASIL Level: A/QM (project policy)
# Version: 1.1.0
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

# Initialize Colors for non-TTY runners
if [[ -t 1 ]]; then
    readonly RED='\033]'; then
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
    # This fulfills ASIL A requirements for Statement Coverage [1]
    generate_robust_coverage

    log_success "All actions completed successfully"
    log_info "Coverage HTML: ${COVERAGE_DIR}/html/index.html"
}

main "$@"
