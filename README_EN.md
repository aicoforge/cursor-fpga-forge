**English** | [繁體中文](./README.md)

# Cursor-FPGA-Forge

## AI Agent-Driven FPGA Auto-Verification Platform

> Turn weeks of environment setup and verification into minutes

---

## Project Overview

**Cursor-FPGA-Forge** is AICOFORGE's FPGA auto-verification demo, showcasing how AI agents (Cursor IDE) automatically generate complete FPGA verification environments, including design synthesis, bitstream generation, host test programs, and hardware verification.

### Core Value

| Metric | Achievement |
|:---:|:---:|
| **Minutes** | Auto-generate complete verification environment (traditionally weeks) |
| **2 Hours** | Full FPGA workflow demonstration video |
| **Zero Errors** | AI handles configs and memory allocation automatically |
| **Plug-and-Play** | Works across different FPGA platforms and tool versions |

---

## Quick Start

This project provides a **pre-designed FPGA verification template** that lets you quickly verify your own FPGA circuit designs through AI agents. We use a simple `adder` as an example to demonstrate the complete automation workflow. You can adjust tool versions, FPGA platforms, and other settings based on your actual needs.

### Demo Video & Workflow

#### Demo: Complete AI-Automated FPGA Verification Flow

**Video Link**: [Watch cursor + FPGA verification demo](https://www.youtube.com/watch?v=u_RUluOvOeM) (2-hour complete process)

**Execution Log**: [View full synthesis & verification log](https://www.dropbox.com/scl/fi/2zua9qlv7qj0nrdeg7fwz/demo.log?rlkey=wbat8307916fzze0437lvcvtt&e=1&dl=0)

#### Video Content

This video demonstrates how Cursor AI automatically completes the entire FPGA verification flow:

1. **FPGA Setup File Generation** — AI auto-generates environment configs, Makefiles, compilation configs
2. **FPGA Design Synthesis** — HLS synthesis and optimization (~1-2 minutes)
3. **Bitstream Generation** — System integration, place & route (~2 hours)
4. **Host Test Program** — Auto-generates test program with XRT API integration
5. **Automated Test Execution** — Completes hardware verification on physical U50 FPGA

**Viewing Suggestion**: The video is 2 hours long. You can select segments of interest to see how Cursor responds to requests and automates various tasks.

#### Execution Log Details

The log file contains complete execution details:

- **C Simulation (csim)** — Functional verification results
- **C Synthesis (csynth)** — Synthesis reports, timing analysis, resource usage
- **U50 Hardware Verification** — Physical FPGA test results and performance data

---

### Prerequisites

```bash
# You need:
- Cursor IDE (https://cursor.com) with SSH extension
- SSH access to FPGA development server
- Xilinx Vitis 2022.1+ (or adjust to your version)
- Xilinx U50 FPGA (or adapt to your board)
- Linux environment with FPGA access
```

---

### Steps

#### 1. Connect to FPGA Development Environment

Use Cursor's SSH feature to connect to your FPGA development server:

```bash
# In Cursor IDE:
1. Press Ctrl+Shift+P
2. Type "Remote-SSH: Connect to Host"
3. Configure your SSH connection
4. Connect to FPGA server
```

For detailed SSH setup, refer to [Cursor documentation](https://docs.cursor.com/en/welcome).

---

#### 2. Clone Project

```bash
# Clone the project
git clone https://github.com/aicoforge/cursor-fpga-forge.git
cd cursor-fpga-forge

# Project structure:
cursor-fpga-forge/
├── adder_kernel.cpp          # Basic HLS kernel
├── adder_kernel.h            # Kernel header
├── fpga_verification.md      # Automation specification
├── run_hls.tcl               # HLS test configuration
├── tb_add.cpp                # Testbench
├── vitis_env.sh              # Environment setup
└── .vscode/                  # Cursor/VSCode configuration
    └── tasks.json            # Build task configuration

# Enable execution and test
chmod +x vitis_env.sh

# Press Ctrl+Shift+B in Cursor IDE to run HLS verification
# This executes: ./vitis_env.sh vitis_hls -f run_hls.tcl
```

---

#### 3. Use AI to Auto-Generate FPGA Verification Scripts

**Key Step**: Let Cursor AI automatically generate verification environment based on specification

```bash
# In Cursor IDE:
1. Open this project directory
2. Give Cursor AI this prompt:

"please follow fpga_verification.md to build fpga verification needed scripts"

3. Cursor will auto-generate the complete verification framework:
   - setup_verification.sh (environment setup)
   - run_verification.sh (verification execution)
   - fpga_verification/ directory with all source files and Makefiles
```

---

#### 4. Run Setup and Verification

```bash
# Grant execution permissions
chmod +x setup_verification.sh run_verification.sh

# Run environment setup
./setup_verification.sh

# Generated directory structure:
fpga_verification/
├── src/
│   ├── adder_kernel.cpp       # Kernel with HLS pragmas
│   ├── adder_kernel.h         # Kernel header
│   └── host.cpp               # Host program (multi-tier memory allocation)
├── kernels/Hardware/
│   ├── Makefile               # Kernel compilation Makefile
│   └── adder-compile.cfg      # Kernel compilation config
├── system_hw_link/Hardware/
│   └── Makefile               # System linking Makefile
├── host/build/
│   └── Makefile               # Host compilation Makefile
└── vitis_env.sh               # Environment setup script

# Run full FPGA verification (synthesis + implementation + test)
./run_verification.sh all

# Or run software simulation only (quick code verification)
./run_verification.sh sw
```

---

### Verification Results

Real FPGA flow with timing data:

1. **Kernel Compilation** (~1-2 minutes)
   - HLS synthesis with pragma optimization
   - Estimated Fmax: 411MHz (simple adder)

2. **Bitstream Generation** (~2 hours)
   - System integration and platform linking
   - Block-level synthesis: 111 parallel jobs
   - Logic placement and routing optimization
   - Final bitstream size: ~30MB

3. **Host Compilation** (~30 seconds)
   - XRT API integration with error handling
   - Multi-tier memory allocation fallback

4. **Hardware Verification** (~5 seconds)
   - Execution on physical U50 FPGA
   - **Memory Allocation**: Auto-fallback to HBM Bank 0
   - **Test Cases**: 4 different scenarios ✓ ALL PASSED

---

## Environment Customization

You can adjust environment settings based on actual needs:

### Different Vitis Version?

```bash
# Method 1: Modify fpga_verification.md before generation
# Find Section 1 and change:
VITIS_VERSION="2023.2"  # Change from 2022.1
VITIS_PATH="/opt/Xilinx/Vitis/${VITIS_VERSION}"

# Method 2: Edit scripts after generation
# In setup_verification.sh and vitis_env.sh
VITIS_VERSION="2023.2"
VITIS_PATH="/opt/Xilinx/Vitis/2023.2"
```

### Different FPGA Platform?

```bash
# Method 1: Modify fpga_verification.md specification
# Find Section 1 and change:
DEFAULT_PLATFORM="xilinx_u250_gen3x16_xdma_4_1_202210_1"

# Method 2: Set environment variable before running
export FPGA_PLATFORM="xilinx_u250_gen3x16_xdma_4_1_202210_1"
./setup_verification.sh
```

### Different Installation Paths?

```bash
# Modify paths in fpga_verification.md Section 1
XRT_PATH="/opt/xilinx/xrt"                    # Change to your XRT path
VITIS_PATH="/opt/Xilinx/Vitis/2022.1"         # Change to your Vitis path

# Common alternative paths:
XRT_PATH="/tools/Xilinx/xrt"                  # Alternative XRT location
VITIS_PATH="/tools/Xilinx/Vitis/2023.1"       # Different version/location
```

---

## What AI Speeds Up vs What Takes Time

### What AI Automates (Minutes):
- **Environment Setup**: Complete verification framework generation
- **Configuration**: Build files, HLS settings, memory setup — no trial and error
- **Error Recovery**: Smart fallback mechanisms, no manual debugging
- **Platform Adaptation**: Quick switching between FPGA platforms and tool versions

### What's Still Physics (Hours):
- **Kernel Compilation**: ~1-2 minutes (HLS synthesis)
- **Bitstream Generation**: ~2 hours (place & route can't be rushed)
- **Hardware Testing**: ~5 seconds (physical FPGA execution)

**Impact**: Instead of spending **weeks** on setup, debugging, and fixing pragma errors, you spend **minutes** generating working frameworks and **hours** on actual FPGA compilation. Tedious, error-prone manual work disappears.

---

## Try Pre-Built AI Agent + FPGA Development Environment

If you're interested in this AI agent-automated FPGA verification demo but can't replicate a similar environment, feel free to contact us:

**Contact**: kevinjan@aicoforge.com

We can provide a **pre-built AI Agent + FPGA development environment** so you can actually experience how AI agents automate the complete FPGA verification workflow.

---

## Related Links

- **AICOFORGE Website**: [aicoforge.com](https://aicoforge.com)
- **HLS Optimization Project**: [cursor-hls-forge](https://github.com/aicoforge/cursor-hls-forge)
- **Demo Video Playlist**: [YouTube Channel](https://www.youtube.com/@aicoforge)

---

## License

MIT License

---

## About AICOFORGE

AICOFORGE is an AI Agent + FPGA Auto Design & Verification platform dedicated to making hardware development as agile as software. We collaborate with Professor Jiin Lai from National Taiwan University's Department of Electrical Engineering (former CTO and co-founder of VIA Technologies) to develop LLM2HLS theoretical technology, combining academic innovation with industry expertise.


**Business Inquiries**: kevinjan@aicoforge.com

