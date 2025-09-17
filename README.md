# cursor-fpga-forge

> **AI Agent + FPGA Automated Directive**
> Transform months of FPGA verification work into minutes using AI-assisted automation

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Xilinx%20U50-blue.svg)](https://www.xilinx.com/)
[![Vitis](https://img.shields.io/badge/Vitis-2022.1-green.svg)](https://www.xilinx.com/products/design-tools/vitis.html)

## Project Background

**AI Agent + FPGA Automated Directive** represents the next evolution in FPGA development automation. Our approach leverages intelligent AI agents to provide comprehensive automated guidance throughout the FPGA design and verification lifecycle.

This framework supports both traditional FPGA design/verification workflows and high-level synthesis (HLS) workflows. While traditional FPGA flows benefit from automation, HLS workflows particularly require AI assistance due to their specialized syntax, pragma directives, and optimization techniques that present significant professional barriers.

**This Project Focus**: We demonstrate our AI Agent + FPGA Automated Directive concept using **Cursor** (an AI-powered code editor) as the AI agent and **Xilinx U50** as the FPGA platform, specifically showcasing HLS workflow automation. Cursor represents the class of AI agents with capabilities similar to other market solutions like GitHub Copilot, Tabnine, or CodeWhisperer - we chose Cursor for this demonstration due to its comprehensive AI integration and development workflow capabilities.

## What This Does

**Problem:** FPGA development has a massive gap. Engineers spend weeks setting up verification environments, debugging HLS settings, fixing memory allocation issues, and dealing with hard-to-understand error messages.

**Solution:** AI Agent + FPGA Automated Directive automatically generates correct verification frameworks, handles error-prone configurations, and provides smart fallback strategies. The FPGA synthesis, place and route still take the same time (that's physics), but everything else becomes instant.

**Result:** From design idea to running verification environment in minutes. From time-consuming setup to automated, error-free workflows.

## What It Delivers

- **Instant Environment Setup**: AI generates complete verification frameworks in seconds (not weeks of manual config)
- **Smart Error Prevention**: Correct HLS pragma placement, memory interface setup, build configurations
- **Intelligent Fallback**: Multi-tier memory allocation handles different FPGA setups automatically
- **No More Debug Hard Work**: Filters out noise, shows you what actually matters
- **Universal Adaptation**: Works across Vitis versions and FPGA platforms with simple config changes

## Quick Start

**Demo Video**: [Watch completed cursor+FPGA verification demo](https://xxx/demo.mp4) - See the complete workflow in action, or follow the step-by-step guide below.  
**Execution Log**: [View full synthesis & verification log](https://www.dropbox.com/scl/fi/2zua9qlv7qj0nrdeg7fwz/demo.log?rlkey=wbat8307916fzze0437lvcvtt&e=1&dl=0) - Includes csim, csynth, and U50 hardware verification details.

### 1. Prerequisites
```bash
# You need:
- Cursor IDE (https://cursor.com) with SSH extension
- SSH access to FPGA development server
- Xilinx Vitis 2022.1+ (or modify for your version)
- Xilinx U50 FPGA (or adapt for your board)
- Linux environment with FPGA access
```

### 2. Setup

#### 2.1 SSH Connection & Cursor Setup
First, connect to your FPGA development server using Cursor's SSH feature:

```bash
# In Cursor IDE:
# 1. Press Ctrl+Shift+P
# 2. Type "Remote-SSH: Connect to Host"
# 3. Configure your SSH connection
# 4. Connect to your FPGA server
```

For detailed SSH setup with Cursor, ask the SSH connection guide in the [cursor doc website](https://docs.cursor.com/en/welcome).

#### 2.2 Initial Project Structure & HLS Verification
After cloning, your project directory should look like this:
```bash
cursor-fpga-forge/
├── adder_kernel.cpp          # Basic HLS kernel
├── adder_kernel.h            # Kernel header
├── fpga_verification.md      # Automation specification
├── run_hls.tcl               # HLS test configuration
├── tb_add.cpp                # Testbench
├── vitis_env.sh              # Environment setup
└── .vscode/                  # Cursor/VSCode configuration
    └── tasks.json            # Build task configuration
```

First, enable execution and test the basic HLS flow:
```bash
# Make environment script executable
chmod +x vitis_env.sh

# Test basic HLS csim+csynth using Cursor's build task
# Press Ctrl+Shift+B in Cursor IDE to run HLS verification
# This will execute: ./vitis_env.sh vitis_hls -f run_hls.tcl
```

#### 2.3 Generate Verification Scripts
**This is the key step**: Use Cursor AI with the specification document to generate automation scripts:

```bash
# 1. Open this project directory in Cursor
# 2. Give this prompt to Cursor AI:

"please follow fpga_verification.md to build fpga verification needed scripts"

# 3. Cursor will generate the complete verification framework:
#    - setup_verification.sh (environment setup)
#    - run_verification.sh (verification execution)
#    - fpga_verification/ directory with all source files and Makefiles
```

#### 2.4 Run Setup
```bash
# After Cursor generates the scripts:
chmod +x setup_verification.sh run_verification.sh
./setup_verification.sh

# This will create the complete fpga_verification/ directory structure:
# fpga_verification/
# ├── src/
# │   ├── adder_kernel.cpp       # Kernel with HLS pragmas INSIDE function body
# │   ├── adder_kernel.h         # Kernel header file
# │   └── host.cpp               # Host program with multi-tier memory allocation
# ├── kernels/Hardware/
# │   ├── Makefile               # Kernel compilation Makefile
# │   └── adder-compile.cfg      # Kernel compilation configuration
# ├── system_hw_link/Hardware/
# │   └── Makefile               # System linking Makefile
# ├── host/build/
# │   └── Makefile               # Host compilation Makefile
# └── vitis_env.sh               # Environment setup script
```

#### 2.5 Verify Setup
```bash
# Check that all files were created correctly:
find fpga_verification -type f | sort

# Verify the directory structure:
tree fpga_verification

# Test the help function:
./run_verification.sh help
```

### 3. Run Verification
```bash
# Full FPGA verification (synthesis + implementation + test)
./run_verification.sh all

# Quick software-only verification (for code changes)
./run_verification.sh sw
```

## Execution Summary

### What AI Speeds Up (Minutes):
- **Environment Setup**: Complete verification framework generation
- **Configuration**: Build files, HLS settings, memory setup - no more trial and error
- **Error Recovery**: Smart fallbacks when things fail, no manual debugging
- **Platform Adaptation**: Switch between FPGA platforms and Vitis versions instantly

### What Still Takes Time (Physics):
- **Kernel Compilation**: ~1-2 minutes (HLS synthesis)
- **Bitstream Generation**: ~2 hours (place & route can't be rushed)
- **Hardware Testing**: ~5 seconds (actual FPGA execution)

Instead of spending **weeks** setting up environments, debugging configs, and fixing pragma errors, you spend **minutes** generating working frameworks and **hours** on actual FPGA compilation. The tedious, error-prone human work disappears.

### Verification Phase
Complete FPGA flow with real timing data:

1. **Kernel Compilation** (~1-2 minutes)
   - HLS synthesis with pragma optimization
   - Estimated Fmax: 411MHz for simple adder

2. **Bitstream Generation** (~2 hours)
   - System integration and platform linking  
   - Block-level synthesis: 111 parallel jobs
   - Logic placement and routing optimization
   - Final bitstream size: ~30MB

3. **Host Compilation** (~30 seconds)
   - XRT API integration with error handling
   - Multi-tier memory allocation fallback

4. **Hardware Verification** (~5 seconds)
   - Real FPGA execution on U50
   - **Memory allocation**: Falls back to HBM Bank 0
   - **Test Cases**: 4 different scenarios ✓ ALL PASSED

Your adder kernel is now verified on real FPGA hardware with detailed timing and resource reports.

## Environment Customization

### Different Vitis Version?
Edit these files:
```bash
# In setup_verification.sh and vitis_env.sh
VITIS_VERSION="2023.2"  # Change from 2022.1
VITIS_PATH="/opt/Xilinx/Vitis/2023.2"
```

### Different FPGA Platform?
```bash
# Set before running
export FPGA_PLATFORM="xilinx_u250_gen3x16_xdma_4_1_202210_1"
./setup_verification.sh
```

### Different Installation Paths?
```bash
# Customize in setup_verification.sh
XRT_PATH="/your/xrt/path"
VITIS_PATH="/your/vitis/path"
```

## Vision & Future

This is just the beginning. We're building towards:

- **Multi-kernel Orchestration**: Complex FPGA systems with AI-generated interconnects
- **Waveform AI Debugging**: AI analyzes simulation traces and suggests fixes
- **Performance Optimization**: AI recommends HLS pragma optimizations automatically
- **Cross-Platform Generation**: Support for Intel FPGAs, different vendors
- **Real-time Collaboration**: Team-based FPGA development with AI assistance

## Why This Matters

Most FPGA engineers spend about 70% of their time on verification and debugging instead of real innovation. Our mission is to make FPGA development as fast and easy as software, with AI handling the boring parts so engineers can focus on breakthroughs. The result is faster time-to-market, lower barriers to using FPGAs, and more innovation in hardware acceleration.

## Contributing & Services

We offer **AI Agent + Automated FPGA Verification services** through [aicoforge](https://aicoforge.com), where clients can choose different AI agents and FPGA platforms based on their specific requirements. This project demonstrates our capabilities using Cursor and Xilinx U50 as an example implementation.

For enterprise clients with specific security, compliance, or customization needs, we can assist in deploying private, local, or customized LLM services to provide tailored AI agent solutions that meet your organizational requirements.

Our commercial services include:
- Multi-kernel system verification
- Custom IP integration and testing  
- Performance optimization and debugging
- Platform-specific adaptation (Intel FPGAs, custom boards)
- Enterprise deployment and training

For commercial projects, contact us through [aicoforge](https://aicoforge.com).

### Open Source Contributions
This is our initial open-source project as we build aicoforge! We welcome:
- Bug reports and feature requests
- Platform support (Intel FPGAs, different boards)
- Integration with other tools
- Documentation improvements

## Connect With Us

- **Startup:** [aicoforge](https://aicoforge.com) - Building the future of FPGA development
- **Vision:** AI Agent + FPGA Automated Directive technology - AI that understands hardware
- **Mission:** Turning FPGA complexity into simplicity, one automated verification at a time
