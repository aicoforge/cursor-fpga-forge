# FPGA Verification Environment Automation Specification

## Overview

This specification defines a complete, working FPGA verification environment for Xilinx Vitis that generates functional scripts with proper error handling, memory allocation fallback strategies, and clear diagnostic output. Based on proven working implementations for the Alveo U50 platform.

## Critical Success Factors

1. **Directory Structure**: All files must be under `fpga_verification/` subdirectory
2. **HLS Pragmas**: Must be placed INSIDE function bodies only
3. **Memory Allocation**: Implement 3-tier fallback (Default→HBM→DDR)
4. **Script Execution**: Must handle both `all` and `sw` modes correctly
5. **Error Filtering**: Remove XRT noise from output

## Section 1: Reusable Vitis Environment Setup Template

### Core Environment Variables

```bash
# === Configuration Variables ===
VITIS_VERSION="2022.1"
VITIS_PATH="/opt/Xilinx/Vitis/${VITIS_VERSION}"
VITIS_HLS_PATH="/opt/Xilinx/Vitis_HLS/${VITIS_VERSION}"
XRT_PATH="/opt/xilinx/xrt"
DEFAULT_PLATFORM="xilinx_u50_gen3x16_xdma_5_202210_1"

# FPGA Platform (can be overridden)
FPGA_PLATFORM="${FPGA_PLATFORM:-$DEFAULT_PLATFORM}"
```

### Environment Setup Function

```bash
setup_environment() {
    echo ""
    echo "Checking and setting up Xilinx environment..."
    
    # Check Vitis installation path
    if [ ! -d "$VITIS_PATH" ]; then
        echo "✗ Vitis installation path does not exist: $VITIS_PATH"
        echo "Please check Vitis version or installation path"
        echo "You can set: export VITIS_VERSION=<version> or export VITIS_PATH=<path>"
        exit 1
    fi
    
    if [ -z "$XILINX_VITIS" ]; then
        echo "Setting XILINX_VITIS environment variable..."
        if [ -f "$VITIS_PATH/settings64.sh" ]; then
            source "$VITIS_PATH/settings64.sh"
        else
            echo "✗ settings64.sh does not exist: $VITIS_PATH/settings64.sh"
            exit 1
        fi
    fi
    
    # Check XRT installation path
    if [ ! -d "$XRT_PATH" ]; then
        echo "✗ XRT installation path does not exist: $XRT_PATH"
        echo "Please check XRT installation"
        echo "You can set: export XRT_PATH=<path>"
        exit 1
    fi
    
    if [ -z "$XILINX_XRT" ]; then
        echo "Setting XILINX_XRT environment variable..."
        export XILINX_XRT="$XRT_PATH"
    fi
    
    echo "XILINX_VITIS: $XILINX_VITIS"
    echo "XILINX_XRT: $XILINX_XRT"
    echo ""
}
```

### FPGA Device Check Function

```bash
check_fpga_device() {
    echo ""
    echo "Checking U50 device status..."
    if command -v xbutil &> /dev/null; then
        xbutil examine | grep "BDF" | head -2
        if [ $? -eq 0 ]; then
            echo "✓ U50 device check passed"
        else
            echo "⚠ U50 device status abnormal, but continuing setup"
        fi
    else
        echo "⚠ xbutil not found, please confirm XRT installation"
    fi
}
```

### Tool Availability Check

```bash
check_tools() {
    echo ""
    echo "Checking required tools..."
    
    if command -v v++ &> /dev/null; then
        echo "✓ v++ compiler available"
    else
        echo "✗ v++ compiler not found!"
        return 1
    fi
    
    if command -v g++ &> /dev/null; then
        echo "✓ g++ compiler available"
    else
        echo "✗ g++ compiler not found!"
        return 1
    fi
    
    return 0
}
```

## Section 2: Reusable Directory Structure Template

### Directory Creation Function

```bash
create_directories() {
    echo ""
    echo "Creating directory structure..."
    
    # Create fpga_verification as main directory
    local dirs=(
        "fpga_verification/src"
        "fpga_verification/kernels/Hardware"
        "fpga_verification/system_hw_link/Hardware"
        "fpga_verification/host/build"
    )
    
    for dir in "${dirs[@]}"; do
        if [ ! -d "$dir" ]; then
            mkdir -p "$dir"
            echo "✓ Created directory: ${dir#fpga_verification/}"
        else
            echo "✓ Directory already exists: ${dir#fpga_verification/}"
        fi
    done
}
```

### Vitis Environment Script

```bash
create_vitis_env() {
    cat > fpga_verification/vitis_env.sh << 'EOF'
#!/bin/bash
# Xilinx environment setup script

# Set Xilinx Vitis environment
if [ -z "$XILINX_VITIS" ]; then
    echo "Setting XILINX_VITIS environment variable..."
    source /opt/Xilinx/Vitis/2022.1/settings64.sh
fi

# Set XRT environment
if [ -z "$XILINX_XRT" ]; then
    echo "Setting XILINX_XRT environment variable..."
    export XILINX_XRT=/opt/xilinx/xrt
fi

echo "Environment setup complete"
echo ""
EOF
    chmod +x fpga_verification/vitis_env.sh
}
```

## Section 3: Design Application Template (ADDER Example)

### Kernel Source Files

```bash
create_source_files() {
    echo ""
    echo "Creating/updating source files..."
    
    # Create adder_kernel.cpp with pragmas INSIDE function
    cat > fpga_verification/src/adder_kernel.cpp << 'EOF'
#include "adder_kernel.h"

void adder_kernel(int a, int b, int *out) {
    #pragma HLS INTERFACE m_axi port=out bundle=gmem0
    #pragma HLS INTERFACE s_axilite port=a bundle=control
    #pragma HLS INTERFACE s_axilite port=b bundle=control
    #pragma HLS INTERFACE s_axilite port=out bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    
    *out = a + b;
}
EOF
    echo "✓ Updated src/adder_kernel.cpp"
    
    # Create header file
    cat > fpga_verification/src/adder_kernel.h << 'EOF'
#ifndef ADDER_KERNEL_H
#define ADDER_KERNEL_H

void adder_kernel(int a, int b, int *out);

#endif
EOF
    echo "✓ Created src/adder_kernel.h"
    
    # Create host program with comprehensive test cases matching tb_adder.cpp
    cat > fpga_verification/src/host.cpp << 'EOF'
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>

// XRT includes
#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"

// Test case structure matching tb_adder.cpp
struct TestCase {
    int a, b, expected;
    std::string description;
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <xclbin>" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== FPGA XRT Verification Test ===" << std::endl;
    
    // Test cases matching tb_adder.cpp exactly
    std::vector<TestCase> test_cases = {
        {3, 5, 8, "Basic addition"},
        {-10, 7, -3, "Negative numbers"},
        {0, 0, 0, "Zero values"},
        {1000, 2000, 3000, "Large numbers"}
    };

    try {
        std::string binaryFile = argv[1];
        int device_index = 0;

        std::cout << "Open the device " << device_index << std::endl;
        auto device = xrt::device(device_index);

        std::cout << "Load the xclbin " << binaryFile << std::endl;
        auto uuid = device.load_xclbin(binaryFile);

        std::cout << "Create kernel object" << std::endl;
        auto krnl = xrt::kernel(device, uuid, "adder_kernel");

        std::cout << "Allocate Buffer in Global Memory" << std::endl;

        // 3-tier memory allocation strategy
        xrt::bo bo_out;
        try {
            bo_out = xrt::bo(device, sizeof(int));
            std::cout << "✓ Buffer allocated successfully (default memory)" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Default memory allocation failed, trying HBM bank 0..." << std::endl;
            try {
                bo_out = xrt::bo(device, sizeof(int), 0);
                std::cout << "✓ Buffer allocated successfully (HBM bank 0)" << std::endl;
            } catch (const std::exception& e2) {
                std::cout << "HBM allocation also failed, trying DDR..." << std::endl;
                bo_out = xrt::bo(device, sizeof(int), xrt::bo::flags::normal, 0);
                std::cout << "✓ Buffer allocated successfully (DDR)" << std::endl;
            }
        }

        auto bo_out_map = bo_out.map<int*>();

        // Run test cases matching tb_adder.cpp
        int passed_tests = 0;
        int total_tests = test_cases.size();
        
        for (size_t i = 0; i < test_cases.size(); i++) {
            const auto& test = test_cases[i];
            std::cout << "\nTest " << (i+1) << ": " << test.description << std::endl;
            std::cout << "Input: " << test.a << " + " << test.b << " = ?" << std::endl;
            
            // Initialize output buffer
            *bo_out_map = 0;

            // Sync buffer to device
            std::cout << "Sync buffer to device" << std::endl;
            bo_out.sync(XCL_BO_SYNC_BO_TO_DEVICE);

            // Execute the kernel
            std::cout << "Execution of the kernel" << std::endl;
            auto run = krnl(test.a, test.b, bo_out);
            run.wait();

            // Get the output
            std::cout << "Get the output data from the device" << std::endl;
            bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

            int result = *bo_out_map;
            std::cout << "Result: " << result << std::endl;

            // Verify result
            if (result == test.expected) {
                std::cout << "✓ Test " << (i+1) << " PASSED: " << test.a << " + " << test.b << " = " << result << std::endl;
                passed_tests++;
            } else {
                std::cout << "✗ Test " << (i+1) << " FAILED: Expected " << test.expected << ", got " << result << std::endl;
            }
        }
        
        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Passed: " << passed_tests << "/" << total_tests << " tests" << std::endl;
        
        if (passed_tests == total_tests) {
            std::cout << "✓ All tests PASSED! FPGA verification successful!" << std::endl;
            return EXIT_SUCCESS;
        } else {
            std::cout << "✗ Some tests FAILED! FPGA verification failed!" << std::endl;
            return EXIT_FAILURE;
        }

    } catch (const std::runtime_error& e) {
        std::cout << "Runtime Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cout << "Unknown exception occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
EOF
    echo "✓ Created src/host.cpp"
}
```

## Section 4: Makefile Templates

### Kernel Compilation Makefile

```bash
create_makefiles() {
    echo ""
    echo "Creating Makefile and configuration files..."
    
    # Kernel compilation Makefile
    cat > fpga_verification/kernels/Hardware/Makefile << 'EOF'
# Kernel compilation Makefile
XILINX_VITIS ?= /opt/Xilinx/Vitis/2022.1
XILINX_XRT ?= /opt/xilinx/xrt
VPP ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf
VITIS_PLATFORM = xilinx_u50_gen3x16_xdma_5_202210_1
TARGET = hw
VPP_OPTS = --target $(TARGET)

BUILD_SUBDIRS += build
HW_KERNEL_OBJS += build/adder_kernel.xo
SRC_KERNEL_OBJS += build/adder_kernel.xo
ALL_MESSAGE_FILES = $(subst .xo,.mdb,$(SRC_KERNEL_OBJS))

.PHONY: all clean incremental
all: $(HW_KERNEL_OBJS)

clean:
	-$(RM) $(SRC_KERNEL_OBJS) $(ALL_MESSAGE_FILES) 
	-$(RMDIR) $(BUILD_SUBDIRS)
	-$(RMDIR) .Xil

incremental: all

build/adder_kernel.xo: ../../src/adder_kernel.cpp ../../src/adder_kernel.h adder-compile.cfg
	-@mkdir -p $(@D)
	-@$(RM) $@
	$(VPP) $(VPP_OPTS) --compile -I"$(<D)" --config adder-compile.cfg -o"$@" "$<"
EOF
    echo "✓ Created kernels/Hardware/Makefile"
    
    # Kernel compile configuration
    cat > fpga_verification/kernels/Hardware/adder-compile.cfg << 'EOF'
platform=xilinx_u50_gen3x16_xdma_5_202210_1
save-temps=1
kernel=adder_kernel
messageDb=build/adder_kernel.mdb
temp_dir=build
report_dir=build/reports
log_dir=build/logs

[advanced]
misc=solution_name=adder_kernel
EOF
    echo "✓ Created kernels/Hardware/adder-compile.cfg"
    
    # System link Makefile
    cat > fpga_verification/system_hw_link/Hardware/Makefile << 'EOF'
# System hardware linking Makefile
XILINX_VITIS ?= /opt/Xilinx/Vitis/2022.1
VPP ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf
VITIS_PLATFORM = xilinx_u50_gen3x16_xdma_5_202210_1
TARGET = hw
VPP_OPTS = --target $(TARGET)

KERNEL_OBJS = ../../kernels/Hardware/build/adder_kernel.xo
XCLBIN = adder_system.xclbin

.PHONY: all clean
all: $(XCLBIN)

clean:
	-$(RM) $(XCLBIN)
	-$(RMDIR) .Xil

$(XCLBIN): $(KERNEL_OBJS)
	$(VPP) $(VPP_OPTS) --link --platform $(VITIS_PLATFORM) -o $@ $(KERNEL_OBJS)
EOF
    echo "✓ Created system_hw_link/Hardware/Makefile"
    
    # Host compilation Makefile
    cat > fpga_verification/host/build/Makefile << 'EOF'
# Host compilation Makefile
XILINX_VITIS ?= /opt/Xilinx/Vitis/2022.1
XILINX_XRT ?= /opt/xilinx/xrt
HOST_CXX ?= g++
RM = rm -f
VITIS_PLATFORM = xilinx_u50_gen3x16_xdma_5_202210_1

CXXFLAGS += -std=c++1y -DVITIS_PLATFORM=$(VITIS_PLATFORM) -D__USE_XOPEN2K8
CXXFLAGS += -I$(XILINX_XRT)/include/ -I/opt/Xilinx/Vitis_HLS/2022.1/include/
CXXFLAGS += -O2 -g -Wall -c -fmessage-length=0

LDFLAGS += -luuid -lxrt_coreutil -lxilinxopencl -lpthread -lrt -lstdc++
LDFLAGS += -L$(XILINX_XRT)/lib/ -Wl,-rpath-link,$(XILINX_XRT)/lib

HOST_OBJECTS += src/host.o
HOST_EXE = adder_test
BUILD_SUBDIRS += src/

.PHONY: all clean incremental
all: $(HOST_EXE)

clean:
	-$(RM) $(HOST_EXE) $(HOST_OBJECTS)

incremental: all

src/host.o: ../../src/host.cpp
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

$(HOST_EXE): $(HOST_OBJECTS)
	$(HOST_CXX) -o "$@" $(+) $(LDFLAGS)
EOF
    echo "✓ Created host/build/Makefile"
}
```

## Section 5: Complete Setup Script (setup_verification.sh)

```bash
#!/bin/bash
set -e

# === Configuration Variables ===
VITIS_VERSION="2022.1"
VITIS_PATH="/opt/Xilinx/Vitis/${VITIS_VERSION}"
VITIS_HLS_PATH="/opt/Xilinx/Vitis_HLS/${VITIS_VERSION}"
XRT_PATH="/opt/xilinx/xrt"
DEFAULT_PLATFORM="xilinx_u50_gen3x16_xdma_5_202210_1"
FPGA_PLATFORM="${FPGA_PLATFORM:-$DEFAULT_PLATFORM}"

echo "=== Adder U50 Verification Environment Setup and Check ==="

# [Insert all functions from Sections 1-4 here]
# setup_environment()
# check_fpga_device()
# check_tools()
# create_directories()
# create_source_files()
# create_makefiles()
# create_vitis_env()

# Final verification
final_verification() {
    echo ""
    echo "Final environment verification..."
    
    local required_files=(
        "fpga_verification/src/adder_kernel.cpp"
        "fpga_verification/src/adder_kernel.h"
        "fpga_verification/src/host.cpp"
        "fpga_verification/kernels/Hardware/Makefile"
        "fpga_verification/kernels/Hardware/adder-compile.cfg"
        "fpga_verification/system_hw_link/Hardware/Makefile"
        "fpga_verification/host/build/Makefile"
        "fpga_verification/vitis_env.sh"
    )
    
    local all_good=true
    for file in "${required_files[@]}"; do
        if [ -f "$file" ]; then
            echo "✓ ${file#fpga_verification/}"
        else
            echo "✗ ${file#fpga_verification/} missing"
            all_good=false
        fi
    done
    
    if [ "$all_good" = true ]; then
        echo ""
        echo "✓ Environment setup complete! All files are ready"
        echo ""
        echo "You can now execute:"
        echo "  ./run_verification.sh all"
        echo ""
        echo "Or step-by-step execution:"
        echo "  1. cd kernels/Hardware && make"
        echo "  2. cd ../../system_hw_link/Hardware && make"
        echo "  3. cd ../../host/build && make"
        echo "  4. cp ../../system_hw_link/Hardware/*.xclbin ./"
        echo "  5. ./adder_test adder_system.xclbin"
    else
        echo ""
        echo "✗ Environment setup incomplete, please check the missing files above"
        return 1
    fi
}

# Main execution flow
setup_environment
check_fpga_device

if ! check_tools; then
    echo "✗ Required tools check failed, please install Xilinx toolchain first"
    exit 1
fi

create_directories
create_source_files
create_makefiles
create_vitis_env
final_verification
```

## Section 6: Complete Run Script (run_verification.sh)

```bash
#!/bin/bash
set -e

# Display usage
show_usage() {
    echo "Usage:"
    echo "  $0 all    # Complete flow: FPGA synthesis + Bitstream generation + SW compilation + FPGA verification"
    echo "  $0 sw     # Quick flow: SW compilation + FPGA verification only (for SW-only changes)"
    echo "  $0 help   # Show this help information"
    echo ""
    echo "Flow description:"
    echo "  all flow: Kernel compilation → xclbin generation → Host program compilation → FPGA verification"
    echo "  sw flow:  Host program compilation → FPGA verification (requires existing xclbin file)"
}

# Check parameters
if [ $# -eq 0 ]; then
    echo "Error: Missing parameter"
    echo ""
    show_usage
    exit 1
fi

FLOW_TYPE="$1"

case "$FLOW_TYPE" in
    "all"|"sw"|"help")
        ;;
    *)
        echo "Error: Unsupported parameter '$FLOW_TYPE'"
        echo ""
        show_usage
        exit 1
        ;;
esac

if [ "$FLOW_TYPE" = "help" ]; then
    show_usage
    exit 0
fi

echo "=== Adder U50 Verification Flow ($FLOW_TYPE) ==="

# Check if in correct directory
if [ ! -d "fpga_verification" ]; then
    echo "Error: Please run this script from a location containing the fpga_verification directory"
    echo "Suggested commands:"
    echo "  cd fpga_verification"
    echo "  cp ../run_verification.sh ."
    echo "  ./run_verification.sh $FLOW_TYPE"
    exit 1
fi

# Enter fpga_verification directory
cd fpga_verification

# Set environment
if [ -f "./vitis_env.sh" ]; then
    echo "Setting up Vitis environment..."
    source ./vitis_env.sh
else
    echo "Warning: vitis_env.sh not found, attempting direct environment setup"
    VITIS_VERSION="2022.1"
    VITIS_PATH="/opt/Xilinx/Vitis/${VITIS_VERSION}"
    XRT_PATH="/opt/xilinx/xrt"
    
    if [ -z "$XILINX_VITIS" ]; then
        if [ -f "$VITIS_PATH/settings64.sh" ]; then
            source "$VITIS_PATH/settings64.sh"
        fi
    fi
    
    if [ -z "$XILINX_XRT" ]; then
        export XILINX_XRT="$XRT_PATH"
    fi
    echo ""
fi

# Function: Compile kernel
compile_kernel() {
    echo "Step 1: Compiling adder kernel..."
    cd kernels/Hardware
    make clean
    make
    if [ $? -eq 0 ]; then
        echo "✓ Kernel compilation successful"
    else
        echo "✗ Kernel compilation failed"
        exit 1
    fi
    cd ../..
    echo ""
}

# Function: Generate xclbin
generate_xclbin() {
    echo "Step 2: Generating xclbin file..."
    cd system_hw_link/Hardware  
    make clean
    make
    if [ $? -eq 0 ]; then
        echo "✓ xclbin generation successful"
    else
        echo "✗ xclbin generation failed" 
        exit 1
    fi
    cd ../..
    echo ""
}

# Function: Compile host
compile_host() {
    local step_number="$1"
    echo "Step $step_number: Compiling host program..."
    cd host/build
    make clean
    make
    if [ $? -eq 0 ]; then
        echo "✓ Host program compilation successful"
    else
        echo "✗ Host program compilation failed"
        exit 1
    fi
    echo ""
}

# Function: Prepare test environment
prepare_test_env() {
    local step_number="$1"
    echo "Step $step_number: Preparing test environment..."
    
    # Ensure we're in host/build directory
    if [ ! -f "adder_test" ]; then
        echo "Error: Cannot find compiled adder_test executable, please compile host program first"
        exit 1
    fi
    
    # Check xclbin exists
    if [ ! -f "../../system_hw_link/Hardware/adder_system.xclbin" ]; then
        echo "✗ Cannot find xclbin file! Please run complete flow first (./run_verification.sh all)"
        exit 1
    fi
    
    # Copy xclbin to execution directory
    cp ../../system_hw_link/Hardware/*.xclbin ./
    ls -la *.xclbin
    echo ""
}

# Function: Execute FPGA test
run_fpga_test() {
    local step_number="$1"
    echo "Step $step_number: Executing adder functionality verification..."
    echo "Starting hardware test..."
    
    # Ensure we're in host/build directory with executable
    if [ ! -f "adder_test" ]; then
        echo "Error: Cannot find adder_test executable"
        exit 1
    fi
    
    # Execute test and capture output
    TEST_OUTPUT=$(./adder_test adder_system.xclbin 2>&1)
    TEST_RESULT=$?
    
    # Filter unnecessary error messages
    FILTERED_OUTPUT=$(echo "$TEST_OUTPUT" | grep -v "xclImportBO.*failed" | grep -v "\[XRT\] ERROR:")
    echo "$FILTERED_OUTPUT"
    
    # Analyze output to determine memory type used
    if [ $TEST_RESULT -eq 0 ]; then
        echo ""
        echo "=== Memory Allocation Analysis ==="
        
        if echo "$TEST_OUTPUT" | grep -q "Buffer allocated successfully (HBM bank"; then
            HBM_BANK=$(echo "$TEST_OUTPUT" | grep "Buffer allocated successfully (HBM bank" | sed 's/.*HBM bank \([0-9]\+\).*/\1/')
            echo "✓ Memory type used: HBM Bank $HBM_BANK"
        elif echo "$TEST_OUTPUT" | grep -q "Buffer allocated successfully (DDR"; then
            DDR_BANK=$(echo "$TEST_OUTPUT" | grep "Buffer allocated successfully (DDR" | sed 's/.*DDR bank \([0-9]\+\).*/\1/')
            echo "✓ Memory type used: DDR Bank $DDR_BANK"
        elif echo "$TEST_OUTPUT" | grep -q "default memory"; then
            echo "✓ Memory type used: Default global memory"
        else
            echo "✓ Memory type used: Unknown (but verification successful)"
        fi
        
        echo ""
        echo "✓ Adder U50 verification complete! All tests passed!"
        echo "✓ Functionality verification result: PASS"
        
        # Return to fpga_verification main directory
        cd ../..
    else
        echo ""
        echo "=== Verification Failure Analysis ==="
        echo "✗ All memory type allocations failed"
        
        echo ""
        echo "Failed memory access methods:"
        if echo "$TEST_OUTPUT" | grep -q "Default memory allocation failed"; then
            echo "  - Default global memory: Failed"
        fi
        if echo "$TEST_OUTPUT" | grep -q "HBM.*failed\|trying HBM"; then
            echo "  - HBM memory: Failed"
        fi
        if echo "$TEST_OUTPUT" | grep -q "DDR.*failed\|trying DDR"; then
            echo "  - DDR memory: Failed"
        fi
        
        echo ""
        echo "Complete error message:"
        echo "$TEST_OUTPUT"
        
        echo ""
        echo "✗ Adder U50 verification failed!" 
        exit 1
    fi
}

# Main execution flow
case "$FLOW_TYPE" in
    "all")
        echo "Executing complete verification flow..."
        echo ""
        
        # Full flow: FPGA synthesis + Bitstream + SW compile + verification
        compile_kernel
        generate_xclbin
        compile_host "3"
        prepare_test_env "4"
        run_fpga_test "5"
        ;;
        
    "sw")
        echo "Executing quick SW verification flow..."
        echo ""
        
        # Check if xclbin exists
        if [ ! -f "system_hw_link/Hardware/adder_system.xclbin" ]; then
            echo "✗ Cannot find xclbin file!"
            echo "   Please run complete flow first to generate FPGA bitstream:"
            echo "   ./run_verification.sh all"
            exit 1
        fi
        
        echo "✓ Found existing xclbin file, skipping FPGA compilation steps"
        echo ""
        
        # Quick flow: SW compile + verification only
        compile_host "1"
        prepare_test_env "2"
        run_fpga_test "3"
        ;;
esac

echo "Verification flow completed"
```

## Section 7: Cursor Generation Instructions

### CRITICAL: Generation Rules for Cursor

1. **ALWAYS place all files under `fpga_verification/` directory**
2. **NEVER place HLS pragmas outside function bodies**
3. **ALWAYS implement 3-tier memory allocation in exact order: Default → HBM → DDR**
4. **ALWAYS filter XRT error messages from output**
5. **NEVER generate temporary files or build artifacts during setup**

### Cursor Prompt Template

```
Generate FPGA verification environment based on Specification.

REQUIREMENTS:
1. Create setup_verification.sh in current directory
2. Create run_verification.sh in current directory  
3. All generated files MUST be under fpga_verification/ subdirectory
4. HLS pragmas MUST be inside adder_kernel function body
5. Host program MUST implement 3-tier memory allocation
6. Scripts MUST support both 'all' and 'sw' modes

FILE STRUCTURE:
./setup_verification.sh
./run_verification.sh
./fpga_verification/src/adder_kernel.cpp
./fpga_verification/src/adder_kernel.h
./fpga_verification/src/host.cpp
./fpga_verification/kernels/Hardware/Makefile
./fpga_verification/kernels/Hardware/adder-compile.cfg
./fpga_verification/system_hw_link/Hardware/Makefile
./fpga_verification/host/build/Makefile
./fpga_verification/vitis_env.sh

CRITICAL IMPLEMENTATION DETAILS:
- In adder_kernel.cpp: ALL #pragma HLS lines INSIDE function body
- In host.cpp: Try default memory, then HBM bank 0, then DDR
- In run_verification.sh: cd into fpga_verification first, source vitis_env.sh
- Filter output: grep -v "xclImportBO.*failed" | grep -v "\[XRT\] ERROR:"
```

### Verification Checklist

After generation, verify:
- [ ] `./setup_verification.sh` runs without errors
- [ ] All files created under `fpga_verification/` directory
- [ ] HLS pragmas are inside function body in adder_kernel.cpp
- [ ] Host program has 3-tier memory allocation with try-catch blocks
- [ ] run_verification.sh has both 'all' and 'sw' modes
- [ ] Scripts change to correct directories and return properly
- [ ] Output filtering removes XRT noise