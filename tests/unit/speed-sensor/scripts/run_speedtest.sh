#!/bin/bash
set -e
set -u
set -o pipefail

# --- Paths ---
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
readonly REPO_ROOT="$(cd "${PROJECT_ROOT}/../../.." && pwd)"

readonly BUILD_DIR="${PROJECT_ROOT}/build"
readonly COVERAGE_DIR="${BUILD_DIR}/artifacts/gcov"
readonly AGGREGATION_DIR="${REPO_ROOT}/tests/unit/build/coverage/speed-sensor"

echo -e "\033[0;36m[INFO]\033[0m 🚀 Starting Speed Sensor Test Suite (White-Box Mode)"

cd "${PROJECT_ROOT}"

# --- Clean & Build ---
ceedling clobber
rm -rf "${COVERAGE_DIR}" "${AGGREGATION_DIR}"
mkdir -p "${COVERAGE_DIR}/html" "${AGGREGATION_DIR}"

echo -e "\033[0;36m[INFO]\033[0m Executing tests..."
ceedling gcov:all

# --- Coverage Capture (The Real Fix) ---
echo -e "\033[0;36m[INFO]\033[0m Capturing LCOV data..."
# We point lcov EXACTLY to the 'out' folder where the .gcda files live
lcov --capture \
     --directory "${BUILD_DIR}/gcov/out" \
     --output-file "${COVERAGE_DIR}/coverage.info" \
     --rc lcov_branch_coverage=1 \
     --ignore-errors empty,source,gcov --quiet

# --- Coverage Filter ---
echo -e "\033[0;36m[INFO]\033[0m Filtering coverage data..."
# Even though the code is included in the test file, GCC tags the coverage
# with the absolute path of speed_sensor.c. This extracts just the driver data!
lcov --extract "${COVERAGE_DIR}/coverage.info" \
     "*/firmware/Core/Src/speed_sensor.c" \
     --output-file "${COVERAGE_DIR}/coverage_filtered.info" \
     --rc lcov_branch_coverage=1 \
     --ignore-errors unused,empty --quiet || cp "${COVERAGE_DIR}/coverage.info" "${COVERAGE_DIR}/coverage_filtered.info"

# --- Export & Report ---
cp "${COVERAGE_DIR}/coverage_filtered.info" "${AGGREGATION_DIR}/coverage_filtered.info"

genhtml "${COVERAGE_DIR}/coverage_filtered.info" \
        --output-directory "${COVERAGE_DIR}/html" \
        --branch-coverage --quiet --rc genhtml_branch_coverage=1

echo -e "\033[1;32m✓ Tests passed and coverage saved for aggregation!\033[0m"