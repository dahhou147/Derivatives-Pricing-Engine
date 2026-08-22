#!/bin/bash

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== C++20 Build Script ===${NC}\n"

# Clean old build files
echo -e "${YELLOW}Cleaning old build files...${NC}"
rm -rf build/
rm -rf bin/
echo -e "${GREEN}✓ Cleaned${NC}\n"

# Create build directory
echo -e "${YELLOW}Creating build directory...${NC}"
mkdir -p build
cd build || { echo -e "${RED}Failed to cd into build${NC}"; exit 1; }

# Conda env paths (for QuantLib via pkg-config)
CONDA_PREFIX="${CONDA_PREFIX:-$HOME/miniconda3}"
export PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig:${PKG_CONFIG_PATH:-}"
export LD_LIBRARY_PATH="$CONDA_PREFIX/lib:${LD_LIBRARY_PATH:-}"

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake .. || { echo -e "${RED}CMake configuration failed${NC}"; exit 1; }
echo -e "${GREEN}✓ Configuration done${NC}\n"

# Build
echo -e "${YELLOW}Building project...${NC}"
cmake --build . -- -j$(nproc) || { echo -e "${RED}Build failed${NC}"; exit 1; }
echo -e "${GREEN}✓ Build successful${NC}\n"

# Run executable
echo -e "${YELLOW}Running executable...${NC}"
if [ -f ./bin/pricer ]; then
    ./bin/pricer
    echo -e "\n${GREEN}✓ Execution finished${NC}"
else
    echo -e "${RED}Error: Executable not found${NC}"
    exit 1
fi
