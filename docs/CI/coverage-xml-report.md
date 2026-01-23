# Coverage XML Report Generation for dotstop Validator

## Overview

The unit test infrastructure now generates **Cobertura XML** format coverage reports that are compatible with the dotstop validator. These reports are automatically generated for each test suite and aggregated across all suites.

## Generated Files

### Location
All coverage XML files are generated in: `artifacts/verification/coverage/`

### Files Generated

| File | Description | Format | Validation |
|------|-------------|--------|------------|
| `coverage.xml` | Aggregated coverage across all suites (DC Motor, Servo Motor, Speed Sensor) | Cobertura XML 1.9 | ✓ Dotstop compatible |
| `dc-motor.xml` | DC Motor test suite coverage only | Cobertura XML 1.9 | ✓ Dotstop compatible |
| `servo-motor.xml` | Servo Motor test suite coverage only | Cobertura XML 1.9 | ✓ Dotstop compatible |
| `speed-sensor.xml` | Speed Sensor test suite coverage only | Cobertura XML 1.9 | ✓ Dotstop compatible |
| `coverage.info` | LCOV format coverage data (raw, for reference) | LCOV .info | Internal use |

## Generation Process

### 1. Individual Test Suite Coverage (Per Suite)
Each test suite generates coverage during execution:

**Scripts:**
- `tests/unit/dc-motor/scripts/run_tests.sh`
- `tests/unit/servo-motor/scripts/run_tests.sh`
- `tests/unit/speed-sensor/scripts/run_speedtest.sh`

**Output:**
- HTML reports in `build/artifacts/gcov/html/`
- Coverage `.info` files in `build/artifacts/gcov/`
- **NEW:** XML reports in `build/artifacts/gcov/coverage.xml`

### 2. Master Test Runner
`tests/unit/run_all_tests.sh` orchestrates all suites and:

1. Runs each test suite
2. Generates individual coverage XML files for each suite
3. Copies individual XML files to `artifacts/verification/coverage/`
4. Aggregates `.info` files using `lcov`
5. Generates aggregated HTML report
6. **Generates aggregated XML report** → `artifacts/verification/coverage/coverage.xml`

### 3. GitHub Actions Workflow
`.github/workflows/unit_tests.yml` uploads coverage artifacts:

```yaml
- name: Verify Coverage XML (dotstop compatibility)
  if: always()
  run: |
    echo "Verifying Coverage XML reports for dotstop validator..."
    ls -lh artifacts/verification/coverage/*.xml

- name: Upload Coverage XML (for dotstop validator)
  if: always()
  uses: actions/upload-artifact@v6
  with:
    name: coverage-xml-reports
    path: artifacts/verification/coverage/*.xml
    retention-days: 30
```

## XML Format Specification

### Structure
The generated XML follows the **Cobertura 1.9** format with the following structure:

```xml
<?xml version="1.0" ?>
<coverage version="1.9" timestamp="1769015958" line-rate="1.0000" branch-rate="0.9740" ...>
  <packages>
    <package name="drivapi" line-rate="1.0000" branch-rate="0.9740" ...>
      <classes>
        <class name="coverage" filename="coverage" line-rate="1.0000" branch-rate="0.9740" ...>
          <lines>
            <line number="1" hits="1" branch="false"/>
          </lines>
        </class>
      </classes>
    </package>
  </packages>
</coverage>
```

### Attributes
- **line-rate**: Line coverage percentage in decimal format (0.0-1.0)
  - Example: 1.0000 = 100% line coverage
- **branch-rate**: Branch coverage percentage in decimal format (0.0-1.0)
  - Example: 0.9740 = 97.4% branch coverage
- **timestamp**: Unix timestamp of generation
- **version**: Cobertura format version (1.9)

## Coverage Metrics

### Current Coverage (as of latest run)

| Suite | Lines | Functions | Branches |
|-------|-------|-----------|----------|
| DC Motor | 100% | 100% | 97.5% |
| Servo Motor | 100% | 100% | 96.7% |
| Speed Sensor | 100% | 100% | 100% |
| **Aggregated** | **100%** | **100%** | **97.4%** |

## Dotstop Validator Integration

### Configuration

The dotstop validator can be configured to check coverage XML files:

```yaml
validators:
  coverage_threshold_validator:
    path: artifacts/verification/coverage/coverage.xml
    min_line_rate: 90
```

### Validator Function

File: `dotstop/dotstop_extensions/validators.py`

```python
def coverage_threshold_validator(configuration: Dict) -> Tuple[float, List[Exception]]:
    """
    Validates coverage XML files and compares against minimum threshold.
    
    Args:
        configuration: Dict with 'path' and 'min_line_rate' (default: 90)
    
    Returns:
        Tuple[float, List[Exception]]: (score [0-1], errors)
    """
```

**Supported Attributes:**
- `line-rate` (highest priority)
- `lineRate`
- `percent`
- `value`

**Return Value:**
- Score: 1.0 if coverage >= min_line_rate, else (coverage / min_line_rate)
- Errors: List of exceptions (empty if successful)

## Implementation Details

### Key Functions

#### `generate_coverage_xml()` in `tests/unit/common_test_lib.sh`

Converts LCOV `.info` files to Cobertura XML:

```bash
generate_coverage_xml() {
    local coverage_info="$1"          # Input: coverage.info file
    local output_xml="$2"             # Output: coverage.xml file
    
    # 1. Parse lcov summary for metrics
    local lcov_output=$(lcov --summary "$coverage_info" --rc branch_coverage=1)
    
    # 2. Extract line-rate using regex
    line_percent=$(echo "$lcov_output" | grep -oP 'lines[^:]*:\s*\K[0-9.]+(?=%)')
    line_rate=$(awk "BEGIN {printf \"%.4f\", $line_percent / 100}")
    
    # 3. Extract branch-rate (fallback to line-rate if unavailable)
    branch_percent=$(echo "$lcov_output" | grep -oP 'branches[^:]*:\s*\K[0-9.]+(?=%)')
    branch_rate=$(awk "BEGIN {printf \"%.4f\", $branch_percent / 100}")
    
    # 4. Generate Cobertura XML with metrics
    cat > "$output_xml" << EOF
<?xml version="1.0" ?>
<coverage version="1.9" timestamp="$(date +%s)" line-rate="$line_rate" branch-rate="$branch_rate">
  ...
</coverage>
EOF
}
```

#### Modified `run_all_tests.sh`

Changes to always generate coverage XML:

```bash
# Generate individual coverage XML files for each suite
if [[ -f "$dc_cov" ]]; then
    generate_coverage_xml "$dc_cov" "${ARTIFACTS_DIR}/coverage/dc-motor.xml"
fi

# Aggregate coverage reports
aggregate_coverage "${MASTER_COVERAGE_DIR}" "$dc_cov" "$servo_cov" "$speed_cov"

# Generate aggregated XML
generate_coverage_xml "$output_dir/coverage_filtered.info" "$output_dir/coverage.xml"

# Copy to artifacts
cp "${MASTER_COVERAGE_DIR}/coverage.xml" "${ARTIFACTS_DIR}/coverage/coverage.xml"
```

## Testing & Validation

### Manual Testing

```bash
# Run all tests (generates XML files)
cd /home/vboxuser/Team04_DrivaPi
bash tests/unit/run_all_tests.sh

# Verify XML files
ls -lh artifacts/verification/coverage/*.xml

# Test with dotstop validator
python3 << 'PYTHON'
from dotstop_extensions.validators import coverage_threshold_validator
config = {'path': 'artifacts/verification/coverage/coverage.xml', 'min_line_rate': 90}
score, errors = coverage_threshold_validator(config)
print(f"Score: {score:.2f}, Errors: {errors}")
PYTHON
```

### Expected Output

```
✓ Validator successfully parsed coverage.xml
  Score: 1.00
  Errors: None
✓ XML Structure valid
  Root tag: coverage
  Line-rate: 1.0000
  Branch-rate: 0.9740
```

## Continuous Integration

### GitHub Actions Workflow

File: `.github/workflows/unit_tests.yml`

**Steps:**
1. Run all tests (including XML generation)
2. Verify coverage XML files exist
3. Upload coverage XML as artifact
4. Upload full verification artifacts
5. Post results to PR/commit

**Artifacts:**
- `coverage-xml-reports`: Individual coverage XML files
- `verification-artifacts`: Full verification package including XML
- `coverage-html`: HTML coverage reports

## Troubleshooting

### Issue: Coverage XML not generated

**Causes:**
- `.info` files not found
- Coverage filtering removed all data
- LCOV parsing failure

**Solution:**
```bash
# Check .info file existence
find . -name "coverage_filtered.info" -type f

# Test LCOV parsing
lcov --summary /path/to/coverage_filtered.info --rc branch_coverage=1

# Check for errors in test output
bash tests/unit/run_all_tests.sh 2>&1 | grep -i "coverage xml\|error\|warn"
```

### Issue: Dotstop validator reports invalid XML

**Causes:**
- Missing required attributes
- Invalid decimal format for rates
- XML structure mismatch

**Solution:**
```bash
# Validate XML structure
xmllint --noout artifacts/verification/coverage/coverage.xml

# Check attributes
grep -o 'line-rate="[^"]*"' artifacts/verification/coverage/coverage.xml
grep -o 'branch-rate="[^"]*"' artifacts/verification/coverage/coverage.xml

# Verify decimal format (should be 0.0000-1.0000)
```

## References

- **Cobertura XML Format**: http://cobertura.sourceforge.net/
- **LCOV Documentation**: http://ltp.sourceforge.net/coverage/lcov.php
- **Dotstop Validator**: See `dotstop_extensions/validators.py`

## Migration Notes

### Changed Behavior

| Aspect | Before | After |
|--------|--------|-------|
| Coverage Reports | HTML + `.info` only | HTML + `.info` + **XML** |
| Artifact Location | N/A | `artifacts/verification/coverage/` |
| Validator Support | Manual conversion needed | Direct XML reading |
| Test Failure Impact | Coverage not generated | Coverage **always** generated |

### Backward Compatibility

- Existing HTML reports still generated
- `.info` files still available for advanced processing
- New XML files are **additive** (no breaking changes)

---

**Generated by:** Unit Test Infrastructure v2.2.0  
**Last Updated:** 2026-01-21  
**Status:** ✓ Operational
