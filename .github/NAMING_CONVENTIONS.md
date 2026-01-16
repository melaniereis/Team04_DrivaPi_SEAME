# Repository Structure and Naming Conventions

This document defines the naming conventions and structure rules for the Team04_DrivaPi repository.

## Directory Naming

### Acronym Directories
Directories that represent acronyms should be in **ALL CAPS**.

**Examples:**
- `TSF/` - Technical Safety Framework
- `CAN/` - Controller Area Network

### Multi-word Directories
Regular directories with multiple words should use **kebab-case** (lowercase with hyphens).

**Examples:**
- `qt-app/`
- `data-transfer/`
- `speed-sensor/`
- `motor-servo/`

### Single-word Directories
Single-word directories should be **lowercase**.

**Examples:**
- `docs/`
- `scripts/`
- `tests/`
- `rust/`
- `archive/`

## File Naming

### C/C++ and  Rust Files
C/C++ (`.c`, `.cpp`, `.h`, `hpp`) and Rust (`.rs`) files should use **snake_case**.

**Examples:**
- `test_speed_sensor.c`
- `speed_sensor.h`
- `dc_motor_functions.h`
- `pca9685_testable.c`
- `can_latency_send_test.c`
- `main.rs`

### Markdown Documentation
Markdown files should use **snake_case**.

**Examples:**
- `project_guidelines.md`
- `branch_guidelines.md`
- `issue_template.md`
- `genAI_justification.md` (acronym preserved in camelCase)

### Template Files
Template files should clearly indicate they are templates using the `_template` suffix.

**Examples:**
- `epic_template.md`
- `issue_template.md`
- `sub_issue_template.md`

## Special Cases

### Language-Specific Conventions
While the repository follows kebab-case for directories, language-specific naming follows their respective conventions:
- **C/C++/Rust projects**: Use snake_case for all files

### GitHub Directories
GitHub-specific directories follow GitHub conventions:
- `.github/`
- `.github/workflows/`
- `.github/ISSUE_TEMPLATE/`

### Configuration Files
Configuration files follow their ecosystem conventions:
- `README.md` (uppercase)
- `Gemfile` (capitalized)
- `CODE_OF_CONDUCT.md` (uppercase)

## Summary Table

| Type | Convention | Example |
|------|-----------|---------|
| Acronym directories | ALL CAPS | `TSF/`, `CAN/` |
| Multi-word directories | kebab-case | `qt-app/`, `motor-servo/` |
| Single-word directories | lowercase | `docs/`, `scripts/`, `rust/` |
| C files | snake_case | `speed_sensor.c`, `test_speed_sensor.c` |
| Rust files | snake_case | `main.rs` |
| C++ files | snake_case | `can_reader.cpp`, `vehicle_data.cpp` |
| Markdown files | snake_case | `project_guidelines.md` |
| Template files | snake_case + `_template` | `issue_template.md` |

## Guidelines

1. **Be consistent**: Follow these conventions for all new files and directories
2. **Acronyms**: Keep acronyms in CAPS for directories, preserve meaning in files
3. **Readability**: Prioritize clarity over brevity
4. **C/C++ and Rust**: Always use snake_case for files (primary languages in this project)
6. **Don't rename existing files**: Only apply these rules to new additions unless refactoring

## Questions?

If you're unsure about naming something, refer to existing similar files/directories or ask the team during standup.
