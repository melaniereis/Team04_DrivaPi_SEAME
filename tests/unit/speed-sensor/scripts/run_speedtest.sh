#!/bin/bash
set -e
set -u
set -o pipefail

readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
readonly REPO_ROOT="$(cd "${PROJECT_ROOT}/../../.." && pwd)"
readonly BUILD_DIR="${PROJECT_ROOT}/build"
readonly COVERAGE_DIR="${BUILD_DIR}/artifacts/gcov"
readonly AGGREGATION_DIR="${REPO_ROOT}/tests/unit/build/coverage/speed-sensor"
readonly GLOBAL_AGGREGATION_DIR="${REPO_ROOT}/build/coverage/speed-sensor"
readonly VENDOR_DIR="${PROJECT_ROOT}/vendor"
readonly COMMON_LIB="${SCRIPT_DIR}/../../common_test_lib.sh"

source "${COMMON_LIB}"

main() {
    log_info "🚀 Starting Speed Sensor Test Suite (White-Box Mode)"

    check_prerequisites
    cd "${PROJECT_ROOT}"
    cleanup_build "${BUILD_DIR}"
    ensure_vendor "${VENDOR_DIR}"

    rm -rf "${COVERAGE_DIR}" "${AGGREGATION_DIR}"
     mkdir -p "${COVERAGE_DIR}/html" "${AGGREGATION_DIR}" "${GLOBAL_AGGREGATION_DIR}" "${BUILD_DIR}/gcov/out"

    log_info "Executing tests..."
    ceedling gcov:all

    log_info "Capturing LCOV data..."
     if [[ ! -s "${COVERAGE_DIR}/coverage.info" ]]; then
          lcov --capture \
                --directory "${BUILD_DIR}/gcov/out" \
                --output-file "${COVERAGE_DIR}/coverage.info" \
                --rc lcov_branch_coverage=1 \
                --ignore-errors source,gcov --quiet
     fi

    log_info "Filtering coverage data..."
    lcov --extract "${COVERAGE_DIR}/coverage.info" \
         "*/firmware/Core/Src/speed_sensor.c" \
         --output-file "${COVERAGE_DIR}/coverage_filtered.info" \
         --rc lcov_branch_coverage=1 \
         --ignore-errors unused --quiet || cp "${COVERAGE_DIR}/coverage.info" "${COVERAGE_DIR}/coverage_filtered.info"

    cp "${COVERAGE_DIR}/coverage_filtered.info" "${AGGREGATION_DIR}/coverage_filtered.info"
     cp "${COVERAGE_DIR}/coverage_filtered.info" "${GLOBAL_AGGREGATION_DIR}/coverage_filtered.info"

    genhtml "${COVERAGE_DIR}/coverage_filtered.info" \
            --output-directory "${COVERAGE_DIR}/html" \
            --branch-coverage --quiet --rc genhtml_branch_coverage=1

    log_success "Tests passed and coverage saved for aggregation!"
}

main "$@"
