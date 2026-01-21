#!/bin/bash
################################################################################
# Common Test Library - Shared Functions
#
# Purpose: Consolidate repetitive test automation functions
# Author: DrivaPi Team
# Version: 1.0.0
################################################################################

# Detect terminal capabilities
if [[ -t 1 ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    CYAN='\033[0;36m'
    BOLD='\033[1m'
    NC='\033[0m'
else
    RED='' GREEN='' YELLOW='' BLUE='' CYAN='' BOLD='' NC=''
fi

# ============================================================================
# LOGGING
# ============================================================================

log_section() {
    echo -e "${BOLD}${BLUE}========================================================${NC}"
    echo -e "${BOLD}${BLUE} $*${NC}"
    echo -e "${BOLD}${BLUE}========================================================${NC}"
}

log_header() {
    echo -e "${BOLD}${CYAN}========================================================${NC}"
    echo -e "${BOLD}${CYAN} $*${NC}"
    echo -e "${BOLD}${CYAN}========================================================${NC}"
}

log_info() {
    echo -e "${GREEN}[INFO]${NC} $*"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $*"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $*" >&2
}

log_success() {
    echo -e "${BOLD}${GREEN}✓ $*${NC}"
}

log_fail() {
    echo -e "${BOLD}${RED}✗ $*${NC}"
}

# ============================================================================
# PREREQUISITE CHECKS
# ============================================================================

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
        log_error "Missing tools: ${missing[*]}"
        log_info "Install: sudo apt-get install ruby gcc gcov lcov && gem install ceedling"
        return 1
    fi

    log_success "All prerequisites satisfied"
    return 0
}

# ============================================================================
# CLEANUP
# ============================================================================

cleanup_build() {
    local build_dir="$1"
    log_header "Cleaning Previous Build"

    if [[ -d "$build_dir" ]]; then
        rm -rf "$build_dir"
    fi

    find . -type f \( -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" \) -delete
    log_success "Cleanup complete"
}

# ============================================================================
# VENDOR MANAGEMENT
# ============================================================================

ensure_vendor() {
    local vendor_dir="$1"
    
    if [[ -d "$vendor_dir/unity/src" ]]; then
        return 0
    fi

    log_header "Restoring Ceedling Vendor Assets"
    
    local gem_path
    gem_path=$(ruby -r rubygems -e "spec = Gem::Specification.find_by_name('ceedling'); puts spec.full_gem_path if spec" 2>/dev/null || true)

    if [[ -n "$gem_path" && -d "$gem_path/vendor" ]]; then
        cp -a "$gem_path/vendor" "$(dirname "$vendor_dir")"
        log_success "Vendor copied from Ceedling gem"
        return 0
    else
        log_fail "Could not locate Ceedling vendor directory"
        return 1
    fi
}

# ============================================================================
# TEST EXECUTION
# ============================================================================

run_ceedling_tests() {
    local log_file="$1"
    log_header "Running Unit Tests"

    local exit_code=0
    ceedling gcov:all 2>&1 | tee "$log_file" || exit_code=$?

    if [[ $exit_code -eq 0 ]]; then
        log_success "Tests PASSED"
        return 0
    else
        log_fail "Tests FAILED"
        grep -A 10 "FAILED TEST SUMMARY" "$log_file" || true
        return 1
    fi
}

# ============================================================================
# COVERAGE GENERATION
# ============================================================================

generate_lcov_coverage() {
    local build_dir="$1"
    local coverage_dir="$2"
    log_header "Generating Coverage Reports"

    mkdir -p "$coverage_dir/html"

    # Capture coverage
    lcov --capture \
         --directory "$build_dir" \
         --output-file "$coverage_dir/coverage.info" \
         --rc lcov_branch_coverage=1 \
         --ignore-errors source,gcov 2>&1 | grep -v "WARNING" || true

    # Filter
    lcov --remove "$coverage_dir/coverage.info" \
         '/usr/*' '*/test/*' '*/mock_*' '*/unity/*' '*/cmock/*' '*vendor*' \
         '*c_exception*' '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' '*/common/*' \
         --output-file "$coverage_dir/coverage_filtered.info" \
         --rc lcov_branch_coverage=1 \
         --ignore-errors unused 2>&1 | grep -v "WARNING" || true

    # Generate HTML
    genhtml "$coverage_dir/coverage_filtered.info" \
            --output-directory "$coverage_dir/html" \
            --title "DrivaPi Coverage" \
            --branch-coverage \
            --function-coverage \
            --rc genhtml_branch_coverage=1 2>/dev/null || true

    log_success "Coverage HTML: $coverage_dir/html/index.html"
}

# ============================================================================
# COVERAGE AGGREGATION
# ============================================================================

aggregate_coverage() {
    local output_dir="$1"
    shift
    local coverage_files=("$@")

    log_header "Aggregating Coverage Reports"

    if [[ ${#coverage_files[@]} -eq 0 ]]; then
        log_warn "No coverage files to aggregate"
        return 1
    fi

    mkdir -p "$output_dir"

    # Check all files exist
    for file in "${coverage_files[@]}"; do
        if [[ ! -f "$file" ]]; then
            log_warn "Coverage file not found: $file"
            return 1
        fi
        log_info "Found: $file"
    done

    # Merge coverage files
    local lcov_args=()
    for file in "${coverage_files[@]}"; do
        lcov_args+=(-a "$file")
    done

    lcov "${lcov_args[@]}" \
         -o "$output_dir/coverage_combined.info" \
         --rc lcov_branch_coverage=1 || return 1

    # Filter
    lcov -r "$output_dir/coverage_combined.info" \
         '/usr/*' '*vendor*' '*cmock*' '*unity*' '*c_exception*' \
         '*build/test/*' '*test/runners*' '*test/mocks*' '/var/lib/gems/*' '*/common/*' \
         -o "$output_dir/coverage_filtered.info" \
         --rc lcov_branch_coverage=1 2>&1 | grep -v "WARNING" || true

    # Generate HTML
    if genhtml "$output_dir/coverage_filtered.info" \
               -o "$output_dir/html" \
               --title "DrivaPi - Unified Coverage" \
               --branch-coverage \
               --function-coverage \
               --rc genhtml_branch_coverage=1 2>/dev/null; then
        log_success "Aggregated coverage: $output_dir/html/index.html"
    fi

    echo ""
    log_info "Coverage Summary:"
    lcov --summary "$output_dir/coverage_filtered.info" --rc lcov_branch_coverage=1 || true
}

# ============================================================================
# UTILITIES
# ============================================================================

count_tests() {
    local output_file="$1"
    grep -oP '^\d+(?= Tests)' "$output_file" | awk '{s+=$1} END {print s}' || echo 0
}

run_test_suite() {
    local test_name="$1"
    local script="$2"
    local output_file="$3"

    log_section "Running $test_name"
    echo ""

    local exit_code=0
    if "$script" 2>&1 | tee "$output_file"; then
        exit_code=$?
    else
        exit_code=$?
    fi

    if [[ $exit_code -eq 0 ]]; then
        log_pass "$test_name PASSED"
        return 0
    else
        log_fail "$test_name FAILED"
        return 1
    fi
}

# ============================================================================
# REPORT GENERATION
# ============================================================================

generate_test_report() {
    local output_file="$1"
    local report_file="$2"
    
    # Extract test counts
    local passed=$(grep -oP 'Tests \d+, \d+ failures' "$output_file" | grep -oP '\d+(?=, \d+ failures)' | head -1 || echo 0)
    local failures=$(grep -oP 'Tests \d+, \d+ failures' "$output_file" | grep -oP '\d+(?= failures)' | head -1 || echo 0)
    local total=$(grep -oP '^\d+(?= Tests)' "$output_file" | awk '{s+=$1} END {print s}' || echo 0)
    
    if [[ -z "$passed" ]] || [[ "$passed" == "0" ]]; then
        passed=$((total - failures))
    fi

    cat > "$report_file" << EOF
{
  "test_suite": "$(basename $(dirname $(dirname "$output_file")))",
  "timestamp": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
  "total_tests": $total,
  "passed": $passed,
  "failed": $failures,
  "status": "$([ $failures -eq 0 ] && echo 'PASSED' || echo 'FAILED')"
}
EOF
}

generate_junit_xml() {
    local output_file="$1"
    local xml_file="$2"
    
    # Extract test info
    local passed=$(grep -oP 'Tests \d+, \d+ failures' "$output_file" | grep -oP '\d+(?=, \d+ failures)' | head -1 || echo 0)
    local failures=$(grep -oP 'Tests \d+, \d+ failures' "$output_file" | grep -oP '\d+(?= failures)' | head -1 || echo 0)
    local total=$(grep -oP '^\d+(?= Tests)' "$output_file" | awk '{s+=$1} END {print s}' || echo 0)
    
    if [[ -z "$passed" ]] || [[ "$passed" == "0" ]]; then
        passed=$((total - failures))
    fi

    cat > "$xml_file" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<testsuite name="$(basename $(dirname $(dirname "$output_file")))" tests="$total" failures="$failures" timestamp="$(date -u +%Y-%m-%dT%H:%M:%SZ)">
  <testcase classname="Unit Tests" name="Test Run" time="0"/>
</testsuite>
EOF
}

merge_junit_xml() {
    local output_file="$1"
    shift
    local xml_files=("$@")

    cat > "$output_file" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<testsuites>
EOF

    for xml_file in "${xml_files[@]}"; do
        if [[ -f "$xml_file" ]]; then
            grep -v '<?xml' "$xml_file" >> "$output_file"
        fi
    done

    cat >> "$output_file" << EOF
</testsuites>
EOF
}
