# cursor-fpga-forge

> **AI Agent + FPGA Auto Design & Verification**
> Turn months of FPGA development work into minutes or hours with AI-powered automation

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Xilinx%20U50-blue.svg)](https://www.xilinx.com/)
[![Vitis](https://img.shields.io/badge/Vitis-2022.1-green.svg)](https://www.xilinx.com/products/design-tools/vitis.html)

## About This Project

**This is AICOFORGE's first open-source project!** We're building the future of FPGA development by making it as fast and easy as software development.

**Our Vision**: AI Agent + FPGA Auto Design & Verification - intelligent AI agents that guide you through the complete FPGA design and verification lifecycle, from initial concept to running hardware.

**What This Project Delivers**: We're starting with an **automated verification template** that demonstrates our approach. This foundation showcases how AI can eliminate weeks of manual setup and configuration. Based on this proven template, we're extending capabilities to cover the full design and verification lifecycle including intelligent error diagnosis, performance optimization, and interactive debugging assistance.

**Technology Stack**: This demonstration uses **Cursor** (AI-powered code editor) as the AI agent and **Xilinx U50** as the FPGA platform, specifically showcasing HLS workflow automation. We chose Cursor for its comprehensive AI integration, but our approach works with similar AI assistants like GitHub Copilot, Tabnine, or CodeWhisperer.

## The Problem We Solve

**Pain Points**: FPGA development has huge efficiency gaps. Engineers spend weeks setting up verification environments, debugging HLS configs, fixing memory issues, and dealing with confusing error messages. About 70% of development time goes to setup and debugging instead of innovation.

**Our Solution**: AI Agent + FPGA automation creates correct design and verification frameworks, handles tricky configs, and provides smart fallback strategies. FPGA synthesis and place-and-route still take the same time (that's physics), but everything else becomes instant.

**Results**: Go from design idea to running verification in minutes or hours. Turn time-consuming setup into automated, error-free workflows.

<div align="center">
    <img src="https://github.com/user-attachments/assets/681c07cc-6a0a-415c-b284-3e9783bc0eff" width="60%">
</div>

## Key Features

- **Instant Environment Setup**: AI generates complete verification frameworks in seconds (instead of weeks of manual config)
- **Smart Configuration**: Correct HLS pragma placement, memory interface settings, and build configurations
- **Automated Memory Access**: Multi-tier memory allocation automatically handles different FPGA setups
- **Intelligent Error Handling**: Filters noise and shows what actually matters
- **Universal Adaptation**: Works across tool versions and FPGA platforms with simple config changes

## Use Cases

Our technology is perfect for applications that need ultra-low latency and high throughput:

- **High-Frequency Trading**: Microsecond-level trading decisions and execution using FPGA hardware acceleration
- **Implied Volatility Calculation**: FPGA-powered microsecond implied volatility calculations for options trading
- **Real-Time Signal Processing**: Digital signal processing, image processing, radar signal analysis
- **Neural Network Inference**: Lower latency and better power efficiency than GPUs for edge computing

## Quick Start

**Demo Video**: [Watch completed cursor+FPGA verification demo](https://www.youtube.com/watch?v=u_RUluOvOeM) - See the complete workflow in action, or follow the step-by-step guide below.  
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
1. Press Ctrl+Shift+P
2. Type "Remote-SSH: Connect to Host"
3. Configure your SSH connection
4. Connect to your FPGA server
```

For detailed SSH setup with Cursor, find the SSH connection guide in the [cursor doc website](https://docs.cursor.com/en/welcome).

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

First, enable execution and test the basic HLS csim+csynth flow:
```bash
# Make environment script executable
chmod +x vitis_env.sh

# Test basic HLS csim+csynth using Cursor's build task
# Press Ctrl+Shift+B in Cursor IDE to run HLS verification
# This will execute: ./vitis_env.sh vitis_hls -f run_hls.tcl
```

#### 2.3 Generate FPGA Verification Scripts
**This is the key step**: Use Cursor AI with the specification document to generate automation scripts:

```bash
# In Cursor IDE:
1. Open this project directory in Cursor
2. Give this prompt to Cursor AI:

"please follow fpga_verification.md to build fpga verification needed scripts"

3. Cursor will generate the complete verification framework:
- setup_verification.sh (environment setup)
- run_verification.sh (verification execution)
- fpga_verification/ directory with all source files and Makefiles
```

#### 2.4 Run Setup
```bash
# After Cursor generates the scripts:
chmod +x setup_verification.sh run_verification.sh
./setup_verification.sh

# This will create the complete fpga_verification/ directory structure:
fpga_verification/
├── src/
│   ├── adder_kernel.cpp       # Kernel with HLS pragmas INSIDE function body
│   ├── adder_kernel.h         # Kernel header file
│   └── host.cpp               # Host program with multi-tier memory allocation
├── kernels/Hardware/
│   ├── Makefile               # Kernel compilation Makefile
│   └── adder-compile.cfg      # Kernel compilation configuration
├── system_hw_link/Hardware/
│   └── Makefile               # System linking Makefile
├── host/build/
│   └── Makefile               # Host compilation Makefile
└── vitis_env.sh               # Environment setup script
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

## What AI Speeds Up vs. What Takes Time

### What AI Automates (Minutes):
- **Environment Setup**: Complete verification framework generation
- **Configuration**: Build files, HLS settings, memory setup - no more trial and error
- **Error Recovery**: Smart fallbacks when things fail, no manual debugging
- **Platform Adaptation**: Switch between FPGA platforms and toolkit versions instantly

### What's Still Physics (Hours):
- **Kernel Compilation**: ~1-2 minutes (HLS synthesis)
- **Bitstream Generation**: ~2 hours (place & route can't be rushed)
- **Hardware Testing**: ~5 seconds (actual FPGA execution)

**The Impact**: Instead of spending **weeks** on setup, debugging configs, and fixing pragma errors, you spend **minutes** generating working frameworks and **hours** on actual FPGA compilation. The tedious, error-prone human work disappears.

### Complete Verification Results

Real FPGA flow with actual timing data:

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

## Environment Customization

You can customize the environment in two ways: 
1. **Before generation**: Modify the template specifications in `fpga_verification.md` before asking Cursor AI to generate scripts
2. **After generation**: Edit the generated scripts directly

### Different Vitis Version?
```bash
# Option 1: Edit fpga_verification.md before generation
# Find Section 1 and change:
VITIS_VERSION="2023.2"  # Change from 2022.1
VITIS_PATH="/opt/Xilinx/Vitis/${VITIS_VERSION}"

# Option 2: Edit generated files after creation
# In setup_verification.sh and vitis_env.sh
VITIS_VERSION="2023.2"  # Change from 2022.1
VITIS_PATH="/opt/Xilinx/Vitis/2023.2"
```

### Different FPGA Platform?
```bash
# Option 1: Edit fpga_verification.md specification 
# Find Section 1 and change:
DEFAULT_PLATFORM="xilinx_u250_gen3x16_xdma_4_1_202210_1"

# Option 2: Set environment variable before running
export FPGA_PLATFORM="xilinx_u250_gen3x16_xdma_4_1_202210_1"
./setup_verification.sh
```

### Different Installation Paths?
```bash
# Option 1: Modify paths in fpga_verification.md Section 1
XRT_PATH="/opt/xilinx/xrt"                    # Change to your XRT path
VITIS_PATH="/opt/Xilinx/Vitis/2022.1"         # Change to your Vitis path

# Common alternative paths:
XRT_PATH="/tools/Xilinx/xrt"                  # Alternative XRT location
VITIS_PATH="/tools/Xilinx/Vitis/2023.1"       # Different version/location

# Option 2: Customize in generated setup_verification.sh
XRT_PATH="/home/user/xilinx/xrt"              # Custom user installation
VITIS_PATH="/home/user/xilinx/Vitis/2022.2"   # Custom user installation
```

## Technology Stack

**AI Tools**:
- Cursor IDE (AI code editor) or similar AI assistants
- Public LLMs like OpenAI and Claude
- Private LLM services (for enterprise security needs)
- Automated error diagnosis and fixes

**FPGA Platform**:
- Xilinx Alveo U50/U250/U280 and AIE series
- Vitis HLS 2022.1+
- Custom FPGA platform support
- Cross-platform compatibility

## Roadmap & Future Development

Based on this automated verification foundation, we're building towards comprehensive design and verification automation:

**Near-term Extensions**:
- **Intelligent Error Diagnosis**: AI analyzes error messages and suggests fixes automatically
- **Performance Optimization**: AI recommends HLS pragma optimizations and resource allocation
- **Interactive Debugging**: AI-assisted waveform analysis and debugging guidance
- **Design Template Generation**: AI creates optimized design structures from specifications

**Long-term Vision**:
- **Multi-kernel System Design**: Complex FPGA systems with AI-generated interconnects
- **Advanced Waveform Analysis**: AI analyzes simulation traces and identifies issues
- **Cross-Platform Generation**: Support for Intel FPGAs, different vendors, and custom boards
- **Real-time Collaboration**: Team-based FPGA development with AI assistance
- **Full Lifecycle Automation**: From concept to optimized, verified hardware implementation

## Why This Matters

Most FPGA engineers spend about 70% of their time on verification and debugging instead of real innovation. Our mission is to make FPGA development as fast and easy as software development, with AI handling the tedious work so engineers can focus on breakthroughs. The result:

- **Faster Time-to-Market**: Reduce development cycles from months to weeks
- **Lower Barriers**: Make FPGA technology accessible to more developers
- **More Innovation**: Free engineers to focus on solving real problems
- **Better Quality**: Automated frameworks reduce human error

## Contributing & Services

### Commercial Services

We offer **AI Agent + FPGA Auto Design & Verification services** through [aicoforge.com](https://aicoforge.com), where clients can choose different AI agents and FPGA platforms based on their specific requirements. This GitHub project demonstrates our capabilities using Cursor and Xilinx U50 as an example implementation.

**Our Services Include**:
- Multi-core system verification
- Custom IP integration and testing
- Performance optimization and debugging
- Platform-specific adaptation (Intel FPGA, custom boards)
- Enterprise deployment and training
- Private LLM deployment (for security and compliance needs)

For commercial projects, contact us through [aicoforge.com](https://aicoforge.com).

### Open Source Contributions

**This is our first open-source project as we build AICOFORGE!** We welcome:
- Bug reports and feature requests
- Platform support (Intel FPGAs, different boards)
- Integration with other AI tools and workflows
- Documentation improvements
- Sharing your success stories

Join us in making FPGA development accessible to everyone!

## Connect With Us

- **Website**: [aicoforge.com](https://aicoforge.com) - Building the future of FPGA development
- **Vision**: AI Agent + FPGA Auto Design & Verification - AI that understands hardware
- **Mission**: Turning FPGA complexity into simplicity, one automated workflow at a time
- **Contact**: kevinjan@aicoforge.com

---

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

Built with the AICOFORGE team. As a Xilinx certified partner, we're dedicated to pushing the boundaries of FPGA development and making it accessible to all engineers.
