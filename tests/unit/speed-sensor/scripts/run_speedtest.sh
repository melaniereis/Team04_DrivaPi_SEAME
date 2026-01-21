#!/bin/bash
################################################################################
# Speed Sensor Unit Test Automation
#
# Purpose: Execute speed sensor unit tests with coverage reporting
# ASIL Level: B/D
# Version: 1.1.0
################################################################################

set -e
set -o pipefail

# Cleanup intermediate files
trap 'rm -f coverage.info coverage_src_only.info' EXIT INT TERM

echo "================================================================"
echo "🚀 STARTING SPEED SENSOR TEST SUITE"
echo "================================================================"

# 1. Cleanup
echo ""
echo "🧹 Cleaning up..."
ceedling clobber

# 2. Run tests
echo ""
echo "🧪 Running Tests..."
ceedling gcov:all

# 3. Capture coverage
echo ""
echo "📸 Capturing Coverage Data..."
lcov --capture --directory build --output-file coverage.info \
     --rc lcov_branch_coverage=1 --quiet

# 4. Filter coverage
echo "📋 Filtering coverage data..."
lcov --extract coverage.info '*/src/*' \
     -o coverage_src_only.info --rc lcov_branch_coverage=1 --quiet || \
    cp coverage.info coverage_src_only.info

lcov -r coverage_src_only.info '/usr/*' '*vendor*' '*cmock*' '*unity*' '*c_exception*' \
     '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' '*test/*' \
     --ignore-errors unused \
     -o coverage_filtered.info --rc lcov_branch_coverage=1 --quiet || \
    cp coverage_src_only.info coverage_filtered.info

# 5. Generate HTML
echo ""
echo "📊 Generating HTML Report..."
if ! genhtml coverage_filtered.info --output-directory coverage_report \
     --branch-coverage --function-coverage \
     --rc genhtml_branch_coverage=1 --quiet 2>/dev/null; then
    mkdir -p coverage_report
    echo "⚠️ genhtml had issues, but coverage files saved"
fi

echo ""
echo "================================================================"
echo "✅ SUCCESS! Report available at: $PWD/coverage_report/index.html"
echo "================================================================"
echo ""

# 6. Save for aggregation if called from master
if [[ "${CALLED_FROM_MASTER:-0}" == "1" ]]; then
    echo "📦 Saving Coverage for Master Aggregation..."
    SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    ABSOLUTE_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
    PERSISTENT_DIR="${ABSOLUTE_ROOT}/build/coverage/speed-sensor"
    
    mkdir -p "${PERSISTENT_DIR}"
    [[ -f "coverage_filtered.info" ]] && \
        cp coverage_filtered.info "${PERSISTENT_DIR}/coverage_filtered.info"
    echo "✓ Coverage saved"
fi

# 7. Cleanup
if [[ "${CI:-false}" == "true" || "${CALLED_FROM_MASTER:-0}" == "1" ]]; then
    echo ""
    echo "ℹ️ Skipping cleanup (CI/Master mode)"
else
    echo ""
    echo "🧹 Cleaning up..."
    ceedling clobber || true
fi

echo ""
echo "✓ Speed Sensor Tests PASSED"