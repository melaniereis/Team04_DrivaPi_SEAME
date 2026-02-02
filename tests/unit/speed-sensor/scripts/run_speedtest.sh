#!/bin/bash
################################################################################
# Speed Sensor Unit Test Automation
#
# Purpose: Execute speed sensor unit tests with coverage reporting
# ASIL Level: B/D
# Version: 1.3.0
################################################################################

set -e -u -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/../../common_test_lib.sh"

PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
COVERAGE_DIR="${BUILD_DIR}/artifacts/gcov"
REPORTS_DIR="${PROJECT_ROOT}/test_reports"
VENDOR_DIR="${PROJECT_ROOT}/vendor"
mkdir -p "${REPORTS_DIR}"

main() {
  log_section "ISO 26262 Speed Sensor Tests - DrivaPi"
  echo -e "${BOLD}ASIL Level:${NC} B/D"
  echo -e "${BOLD}Coverage Requirement:${NC} 100% Branch Coverage"

  check_prerequisites || exit 1
  cleanup_build "${BUILD_DIR}"
  ensure_vendor "${VENDOR_DIR}" || exit 1

  cd "${PROJECT_ROOT}"
  run_ceedling_tests "${REPORTS_DIR}/test_output.log" || exit 1

  # Common coverage generation (captures, filters, generates HTML and XML)
  generate_lcov_coverage "${BUILD_DIR}" "${COVERAGE_DIR}"

  # Copy filtered coverage to a predictable location (for run_all_tests.sh)
  cp -f "${COVERAGE_DIR}/coverage_filtered.info" "${PROJECT_ROOT}/coverage_filtered.info"
  cp -f "${COVERAGE_DIR}/coverage.xml"          "${PROJECT_ROOT}/speed-sensor.xml"

  # Save persistent copy (used for baseline comparisons)
  ABS_ROOT="$(cd "${PROJECT_ROOT}/../.." && pwd)"
  PERSIST_DIR="${ABS_ROOT}/build/coverage/speed-sensor"
  mkdir -p "${PERSIST_DIR}"
  cp -f "${COVERAGE_DIR}/coverage_filtered.info" "${PERSIST_DIR}/coverage_filtered.info"
  cp -f "${COVERAGE_DIR}/coverage.xml"          "${PERSIST_DIR}/speed-sensor.xml"

  # Copy into artifacts/verification/coverage if invoked from run_all_tests.sh
  if [[ "${CALLED_FROM_MASTER:-0}" = "1" ]]; then
    ART_DIR="${ABS_ROOT}/artifacts/verification/coverage"
    mkdir -p "${ART_DIR}"
    cp -f "${COVERAGE_DIR}/coverage.xml" "${ART_DIR}/speed-sensor.xml"
  fi

  log_section "✓ SPEED SENSOR TESTS PASSED"
}

main "$@"
