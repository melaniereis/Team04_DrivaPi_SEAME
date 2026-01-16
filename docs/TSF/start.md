# Getting Started with TSF

**Goal:** Install tools and validate existing requirements
**Time:** 15 minutes

---

## 🚀 Quick Setup

### 1. Install trudag (5 min)

```bash
# Clone
git clone git@github.com:SEAME-pt/Team04_DrivaPi.git
cd Team04_DrivaPi

# Create virtual environment
python3 -m venv .venv
source .venv/bin/activate

# Install dependencies
pip install --upgrade pip pyyaml

# Install trudag (official TSF tool)
git clone https://gitlab.com/CodethinkLabs/trustable/trustable.git /tmp/trustable
cd /tmp/trustable
pip install .
cd -

# Verify installation
trudag --version
```

Note: If you have some issues with `pip install `, try upgrading pip first:
```bash
pip install --upgrade pip
```
---

## ✅ Verification Checklist

Before starting work:

- [ ] `source .venv/bin/activate` works
- [ ] `trudag --version` shows version
- [ ] `trudag manage lint` passes without errors
- [ ] `.dotstop.dot` file exists
- [ ] Folders `reqs/urd/`, `reqs/srd/`, `reqs/swd/`, `reqs/lltc/` exist

---

## 🎯 Next Steps

**To start working:**
- Basic commands → [reference.md](reference.md)
- Create requirements → [workflow.md](workflow.md) (Workflow 1)
- Understand theory → [training.md](training.md)

**Problems?**
- Error `trudag: command not found` → Re-activate environment: `source .venv/bin/activate`
- Validation error → Read error message, usually YAML syntax
- Other issues → Ask in stand-up or check [workflow.md](workflow.md) "Fix Errors" section
