#!/bin/bash
################################################################################
# ISO 26262 Unit Test Automation Script
#
# Purpose: Execute unit tests with coverage reporting
# ASIL Level: A/QM (project policy)
# Author: DrivaPi Team
# Version: 1.0.1
#
# Notes:
# - Coverage gating is enforced in CI (scripts/ci/validate_lltc_coverage.py)
# - Project gate: >= 90% line coverage per suite (ASIL A/QM decision)
# - Regressions vs baseline require approvals rather than lowering the threshold
################################################################################

set -e
set -u
set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/../../common_test_lib.sh"

PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
COVERAGE_DIR="${BUILD_DIR}/artifacts/gcov"
REPORTS_DIR="${PROJECT_ROOT}/test_reports"
VENDOR_DIR="${PROJECT_ROOT}/vendor"

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

log_header() {
    echo -e "${BOLD}${CYAN}============================================================================${NC}"
    echo -e "${BOLD}${CYAN} $*${NC}"
    echo -e "${BOLD}${CYAN}============================================================================${NC}"
}

log_info() { echo -e "${GREEN}[INFO]${NC} $*"; }
log_warn() { echo -e "${YELLOW}[WARN]${NC} $*"; }
log_error() { echo -e "${RED}[ERROR]${NC} $*" >&2; }
log_success() { echo -e "${BOLD}${GREEN}✓ $*${NC}"; }
log_fail() { echo -e "${BOLD}${RED}✗ $*${NC}"; }

check_prerequisites() {
    log_header "Checking Prerequisites"

    local missing=()
    command -v ruby >/dev/null 2>&1 || missing+=("ruby")
    command -v gcc >/dev/null 2>&1 || missing+=("gcc")
    command -v gcov >/dev/null 2>&1 || missing+=("gcov")
    command -v gcovr >/dev/null 2>&1 || missing+=("gcovr")
    command -v lcov >/dev/null 2>&1 || missing+=("lcov")
    command -v genhtml >/dev/null 2>&1 || missing+=("genhtml")

    if ! gem list -i ceedling >/dev/null 2>&1; then
        missing+=("ceedling")
    fi

    if [ ${#missing[@]} -gt 0 ]; then
        log_error "Missing tools: ${missing[*]}"
        log_info "Install: sudo apt-get install ruby gcc gcov lcov && pip3 install gcovr && gem install ceedling"
        exit 1
    fi

    log_success "All prerequisites satisfied"
}

cleanup_build() {
    log_header "Cleaning Previous Build"

    cd "${PROJECT_ROOT}"

    if [ -d "${BUILD_DIR}" ]; then
        rm -rf "${BUILD_DIR}"
    fi

    find . -type f \( -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" \) -delete

    rm -rf "${REPORTS_DIR}"
    mkdir -p "${REPORTS_DIR}"

    mkdir -p "${BUILD_DIR}/test/mocks" "${BUILD_DIR}/gcov/out"

    log_success "Cleanup complete"
}

ensure_vendor() {
    if [[ -d "${VENDOR_DIR}/unity/src" ]]; then
        return
    fi

    log_header "Restoring Ceedling vendor assets"
    local gem_path
    gem_path=$(ruby -r rubygems -e "spec = Gem::Specification.find_by_name('ceedling'); puts spec.full_gem_path if spec" 2>/dev/null || true)

    if [[ -n "$gem_path" && -d "$gem_path/vendor" ]]; then
        cp -a "$gem_path/vendor" "$PROJECT_ROOT/"
        log_success "Vendor copied from Ceedling gem"
    else
        log_fail "Could not locate Ceedling vendor directory (gem path: '$gem_path')"
        exit 1
    fi
}

run_tests() {
    log_header "Running Unit Tests"

    cd "${PROJECT_ROOT}"
    ensure_vendor

    local log_file="${REPORTS_DIR}/test_output.log"

    if ceedling gcov:all 2>&1 | tee "$log_file"; then
        log_success "All tests PASSED"
    else
        log_fail "Tests FAILED"
        grep -A 20 "FAILED TEST SUMMARY" "$log_file" || true
        exit 1
    fi
}

run_speed_sensor_tests() {
    log_header "Speed Sensor Coverage Note"

    log_info "Speed sensor tests executed:"
    log_info "  ✓ test_speed_sensor_functions.c"
    log_info "  ✓ test_threadx_speed_sensor.c"
    log_info ""
    log_info "For the comprehensive speed-sensor/ test suite:"
    log_info "  cd speed-sensor && ./run_speedtest.sh"
}

generate_coverage_report() {
    log_header "Generating Coverage Report"

    cd "${PROJECT_ROOT}"
    mkdir -p "${COVERAGE_DIR}"

    lcov --capture --directory "${BUILD_DIR}" --output-file "${COVERAGE_DIR}/coverage.info" >/dev/null 2>&1
    lcov --remove "${COVERAGE_DIR}/coverage.info" "${PROJECT_ROOT}/vendor/*" "${PROJECT_ROOT}/test/*" --output-file "${COVERAGE_DIR}/coverage_filtered.info" >/dev/null 2>&1

    genhtml -o "${COVERAGE_DIR}/html" "${COVERAGE_DIR}/coverage_filtered.info" >/dev/null 2>&1

    gcovr --xml-pretty --output "${REPORTS_DIR}/coverage.xml" >/dev/null 2>&1

    log_info "Coverage report generated at ${COVERAGE_DIR}/html/index.html"
}

main() {
    check_prerequisites
    cleanup_build
    run_tests
    run_speed_sensor_tests
    generate_coverage_report

    log_info "All actions completed successfully"
    log_info "Coverage XML: ${REPORTS_DIR}/coverage.xml"
    log_info "Coverage HTML: ${COVERAGE_DIR}/html/index.html"
}

main "$@"
