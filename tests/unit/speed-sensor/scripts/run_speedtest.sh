#!/bin/bash
################################################################################
# Speed Sensor Unit Test Automation
#
# Purpose: Execute speed sensor unit tests with coverage reporting
# ASIL Level: B/D
# Version: 1.2.0 - Fixed coverage XML generation
################################################################################

set -e
set -o pipefail

# Source common library for logging functions
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/../../common_test_lib.sh"

# Cleanup intermediate files
trap 'rm -f coverage.info coverage_src_only.info speed-sensor.xml' EXIT INT TERM

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
test_output=$(mktemp)
ceedling gcov:all > "$test_output" 2>&1

# Check for failures - if the output does NOT contain "0 Failures", then we have failures
if ! grep -q "0 Failures" "$test_output"; then
    # Test failed - print output and exit
    cat "$test_output"
    rm -f "$test_output"
    echo ""
    echo "✗ Speed Sensor Tests FAILED"
    exit 1
else
    # Test passed
    cat "$test_output"
    rm -f "$test_output"
fi

# 3. Capture coverage
echo ""
echo "📸 Capturing Coverage Data..."
lcov --capture --directory build --output-file coverage.info \
    --rc branch_coverage=1 --quiet

# 4. Filter coverage
echo "📋 Filtering coverage data..."
lcov --extract coverage.info '*/src/*' \
    -o coverage_src_only.info --rc branch_coverage=1 --quiet \
    >/dev/null 2>&1 || cp coverage.info coverage_src_only.info

lcov -r coverage_src_only.info '/usr/*' '*vendor*' '*cmock*' '*unity*' '*c_exception*' \
    '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' '*test/*' \
    --ignore-errors unused \
    -o coverage_filtered.info --rc branch_coverage=1 --quiet \
    >/dev/null 2>&1 || cp coverage_src_only.info coverage_filtered.info

# 5. Generate HTML
echo ""
echo "📊 Generating HTML Report..."
if ! genhtml coverage_filtered.info --output-directory coverage_report \
    --branch-coverage --function-coverage \
    --rc genhtml_branch_coverage=1 --quiet >/dev/null 2>&1; then
    mkdir -p coverage_report
    echo "⚠️ genhtml had issues, but coverage files saved"
fi

echo ""
echo "================================================================"
echo "✅ SUCCESS! Report available at: $PWD/coverage_report/index.html"
echo "================================================================"
echo ""

# 6. Generate coverage XML for dotstop validator
echo "📄 Generating Coverage XML for dotstop validator..."
generate_coverage_xml "coverage_filtered.info" "speed-sensor.xml"

# 7. Save for aggregation if called from master
if [[ "${CALLED_FROM_MASTER:-0}" == "1" ]]; then
    echo "📦 Saving Coverage for Master Aggregation..."
    SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    ABSOLUTE_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
    PERSISTENT_DIR="${ABSOLUTE_ROOT}/build/coverage/speed-sensor"

    mkdir -p "${PERSISTENT_DIR}"
    [[ -f "coverage_filtered.info" ]] && \
        cp coverage_filtered.info "${PERSISTENT_DIR}/coverage_filtered.info"
    [[ -f "speed-sensor.xml" ]] && \
        cp speed-sensor.xml "${PERSISTENT_DIR}/speed-sensor.xml"
    echo "✓ Coverage saved"
fi

# 8. Copy coverage XML to artifacts directory for master script
if [[ "${CALLED_FROM_MASTER:-0}" == "1" ]]; then
    MASTER_ARTIFACTS_DIR="${ABSOLUTE_ROOT}/artifacts/verification/coverage"
    mkdir -p "${MASTER_ARTIFACTS_DIR}"
    if [[ -f "speed-sensor.xml" ]]; then
        cp speed-sensor.xml "${MASTER_ARTIFACTS_DIR}/speed-sensor.xml"
        echo "✓ Coverage XML copied to artifacts directory"
    else
        echo "⚠️ Speed sensor coverage XML not found"
    fi
fi

# 9. Cleanup
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
