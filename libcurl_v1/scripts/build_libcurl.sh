#!/bin/bash

# libcurl Build Script
# Compiles libcurl from source with customizable options

set -e  # Exit on any error

# Default configuration
BUILD_TYPE="${BUILD_TYPE:-release}"
INSTALL_PREFIX="${INSTALL_PREFIX:-/usr/local}"
BUILD_SSL="${BUILD_SSL:-yes}"
BUILD_LIBSSH2="${BUILD_LIBSSH2:-yes}"
BUILD_ZLIB="${BUILD_ZLIB:-yes}"
BUILD_SHARED="${BUILD_SHARED:-yes}"
BUILD_STATIC="${BUILD_STATIC:-yes}"
PARALLEL_JOBS="${PARALLEL_JOBS:-$(nproc 2>/dev/null || echo 4)}"
SOURCE_DIR="${SOURCE_DIR:-}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to detect package manager and install dependencies
install_dependencies() {
    log_info "Checking and installing build dependencies..."
    
    # Detect package manager
    if command_exists apt-get; then
        log_info "Detected APT package manager (Debian/Ubuntu)"
        sudo apt-get update
        
        local packages="build-essential"
        
        if [ "$BUILD_SSL" = "yes" ]; then
            packages="$packages libssl-dev"
        fi
        
        if [ "$BUILD_ZLIB" = "yes" ]; then
            packages="$packages zlib1g-dev"
        fi
        
        if [ "$BUILD_LIBSSH2" = "yes" ]; then
            packages="$packages libssh2-1-dev"
        fi
        
        packages="$packages pkg-config autoconf automake libtool"
        
        log_info "Installing packages: $packages"
        sudo apt-get install -y $packages
        
    elif command_exists yum; then
        log_info "Detected YUM package manager (CentOS/RHEL)"
        
        local packages="gcc gcc-c++ make"
        
        if [ "$BUILD_SSL" = "yes" ]; then
            packages="$packages openssl-devel"
        fi
        
        if [ "$BUILD_ZLIB" = "yes" ]; then
            packages="$packages zlib-devel"
        fi
        
        if [ "$BUILD_LIBSSH2" = "yes" ]; then
            packages="$packages libssh2-devel"
        fi
        
        packages="$packages pkgconfig autoconf automake libtool"
        
        log_info "Installing packages: $packages"
        sudo yum install -y $packages
        
    elif command_exists apk; then
        log_info "Detected APK package manager (Alpine)"
        
        local packages="build-base"
        
        if [ "$BUILD_SSL" = "yes" ]; then
            packages="$packages openssl-dev"
        fi
        
        if [ "$BUILD_ZLIB" = "yes" ]; then
            packages="$packages zlib-dev"
        fi
        
        if [ "$BUILD_LIBSSH2" = "yes" ]; then
            packages="$packages libssh2-dev"
        fi
        
        packages="$packages pkgconfig autoconf automake libtool"
        
        log_info "Installing packages: $packages"
        sudo apk add $packages
        
    else
        log_warning "Package manager not detected. Please install build dependencies manually:"
        echo "  - C compiler (gcc or clang)"
        echo "  - Make"
        echo "  - pkg-config"
        echo "  - autoconf, automake, libtool"
        if [ "$BUILD_SSL" = "yes" ]; then
            echo "  - OpenSSL development headers"
        fi
        if [ "$BUILD_ZLIB" = "yes" ]; then
            echo "  - zlib development headers"
        fi
        if [ "$BUILD_LIBSSH2" = "yes" ]; then
            echo "  - libssh2 development headers"
        fi
    fi
}

# Function to find source directory
find_source_directory() {
    if [ -n "$SOURCE_DIR" ] && [ -d "$SOURCE_DIR" ]; then
        echo "$SOURCE_DIR"
        return 0
    fi
    
    # Look for curl source directory
    local found_dirs=()
    
    # Check current directory and subdirectories
    for dir in . ./libcurl-build ./curl-* ./libcurl-build/curl-*; do
        if [ -d "$dir" ] && [ -f "$dir/configure" ] && [ -f "$dir/src/tool_main.c" ]; then
            found_dirs+=("$(realpath "$dir")")
        fi
    done
    
    if [ ${#found_dirs[@]} -eq 0 ]; then
        log_error "No libcurl source directory found"
        log_info "Please ensure you have downloaded the source code first"
        return 1
    elif [ ${#found_dirs[@]} -eq 1 ]; then
        echo "${found_dirs[0]}"
        return 0
    else
        log_warning "Multiple source directories found:"
        for i in "${!found_dirs[@]}"; do
            echo "  $((i+1)): ${found_dirs[$i]}"
        done
        
        read -p "Select directory [1]: " -r choice
        choice=${choice:-1}
        
        if [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge 1 ] && [ "$choice" -le ${#found_dirs[@]} ]; then
            echo "${found_dirs[$((choice-1))]}"
            return 0
        else
            log_error "Invalid selection"
            return 1
        fi
    fi
}

# Function to configure build
configure_build() {
    local source_dir="$1"
    
    log_info "Configuring build in $source_dir"
    cd "$source_dir"
    
    # Build configure options
    local configure_opts="--prefix=$INSTALL_PREFIX"
    
    if [ "$BUILD_SHARED" = "yes" ]; then
        configure_opts="$configure_opts --enable-shared"
    else
        configure_opts="$configure_opts --disable-shared"
    fi
    
    if [ "$BUILD_STATIC" = "yes" ]; then
        configure_opts="$configure_opts --enable-static"
    else
        configure_opts="$configure_opts --disable-static"
    fi
    
    if [ "$BUILD_SSL" = "yes" ]; then
        configure_opts="$configure_opts --with-ssl"
    else
        configure_opts="$configure_opts --without-ssl"
    fi
    
    if [ "$BUILD_ZLIB" = "yes" ]; then
        configure_opts="$configure_opts --with-zlib"
    else
        configure_opts="$configure_opts --without-zlib"
    fi
    
    if [ "$BUILD_LIBSSH2" = "yes" ]; then
        configure_opts="$configure_opts --with-libssh2"
    else
        configure_opts="$configure_opts --without-libssh2"
    fi
    
    # Additional useful options
    configure_opts="$configure_opts --enable-libcurl-option"
    configure_opts="$configure_opts --enable-optimize"
    
    if [ "$BUILD_TYPE" = "debug" ]; then
        configure_opts="$configure_opts --enable-debug"
    fi
    
    log_info "Configure options: $configure_opts"
    
    # Run configure
    if ! ./configure $configure_opts; then
        log_error "Configuration failed"
        return 1
    fi
    
    log_success "Configuration completed successfully"
    return 0
}

# Function to compile source
compile_source() {
    local source_dir="$1"
    
    log_info "Compiling libcurl with $PARALLEL_JOBS parallel jobs"
    cd "$source_dir"
    
    # Clean previous build if it exists
    if [ -f "Makefile" ]; then
        log_info "Cleaning previous build..."
        make clean || true
    fi
    
    # Compile
    if ! make -j"$PARALLEL_JOBS"; then
        log_error "Compilation failed"
        log_info "Trying with single job to get clearer error messages..."
        make -j1
        return 1
    fi
    
    log_success "Compilation completed successfully"
    return 0
}

# Function to run tests
run_tests() {
    local source_dir="$1"
    
    log_info "Running tests..."
    cd "$source_dir"
    
    if ! make test; then
        log_warning "Some tests failed, but this might not be critical"
        read -p "Continue with installation anyway? [Y/n]: " -r
        if [[ $REPLY =~ ^[Nn]$ ]]; then
            return 1
        fi
    else
        log_success "All tests passed"
    fi
    
    return 0
}

# Function to install libcurl
install_libcurl() {
    local source_dir="$1"
    
    log_info "Installing libcurl to $INSTALL_PREFIX"
    cd "$source_dir"
    
    # Check if we need sudo
    if [ ! -w "$INSTALL_PREFIX" ]; then
        log_info "Installation requires root privileges"
        if ! sudo make install; then
            log_error "Installation failed"
            return 1
        fi
    else
        if ! make install; then
            log_error "Installation failed"
            return 1
        fi
    fi
    
    # Update library cache on Linux
    if command_exists ldconfig && [ -w "/etc/ld.so.cache" ]; then
        log_info "Updating library cache..."
        sudo ldconfig
    fi
    
    log_success "Installation completed successfully"
    return 0
}

# Function to display build summary
display_summary() {
    local source_dir="$1"
    
    echo
    log_success "Build Summary"
    echo "=============="
    echo "Source directory: $source_dir"
    echo "Install prefix: $INSTALL_PREFIX"
    echo "SSL support: $BUILD_SSL"
    echo "libssh2 support: $BUILD_LIBSSH2"
    echo "zlib support: $BUILD_ZLIB"
    echo "Shared library: $BUILD_SHARED"
    echo "Static library: $BUILD_STATIC"
    echo "Parallel jobs: $PARALLEL_JOBS"
    echo
    
    if command_exists curl; then
        log_info "curl version:"
        curl --version
    fi
    
    if command_exists pkg-config; then
        log_info "libcurl pkg-config:"
        pkg-config --modversion libcurl 2>/dev/null || echo "Not found in pkg-config"
        pkg-config --cflags --libs libcurl 2>/dev/null || echo "pkg-config flags not available"
    fi
    
    echo
    log_info "Installation locations:"
    echo "  Binary: $INSTALL_PREFIX/bin/curl"
    echo "  Library: $INSTALL_PREFIX/lib/libcurl.*"
    echo "  Headers: $INSTALL_PREFIX/include/curl/"
    echo "  Man pages: $INSTALL_PREFIX/share/man/man1/"
}

# Main build function
main() {
    log_info "Starting libcurl build process"
    
    # Find source directory
    local source_dir
    if ! source_dir=$(find_source_directory); then
        return 1
    fi
    
    log_info "Using source directory: $source_dir"
    
    # Install dependencies
    if [ "$SKIP_DEPS" != "yes" ]; then
        install_dependencies
    fi
    
    # Configure
    if ! configure_build "$source_dir"; then
        return 1
    fi
    
    # Compile
    if ! compile_source "$source_dir"; then
        return 1
    fi
    
    # Run tests if requested
    if [ "$RUN_TESTS" = "yes" ]; then
        run_tests "$source_dir"
    fi
    
    # Install
    if ! install_libcurl "$source_dir"; then
        return 1
    fi
    
    # Display summary
    display_summary "$source_dir"
    
    log_success "libcurl build and installation completed successfully!"
    return 0
}

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo "Build libcurl from source code"
    echo ""
    echo "OPTIONS:"
    echo "  --prefix PATH           Installation prefix (default: $INSTALL_PREFIX)"
    echo "  --source-dir PATH       Source directory (auto-detected if not specified)"
    echo "  --build-type TYPE       Build type: release or debug (default: $BUILD_TYPE)"
    echo "  --enable-ssl            Enable SSL support (default: yes)"
    echo "  --disable-ssl           Disable SSL support"
    echo "  --enable-libssh2        Enable libssh2 support (default: yes)"
    echo "  --disable-libssh2       Disable libssh2 support"
    echo "  --enable-zlib           Enable zlib support (default: yes)"
    echo "  --disable-zlib          Disable zlib support"
    echo "  --enable-shared         Build shared library (default: yes)"
    echo "  --disable-shared        Don't build shared library"
    echo "  --enable-static         Build static library (default: yes)"
    echo "  --disable-static        Don't build static library"
    echo "  --jobs N                Number of parallel jobs (default: $PARALLEL_JOBS)"
    echo "  --skip-deps             Skip dependency installation"
    echo "  --run-tests             Run tests after compilation"
    echo "  --help                  Show this help message"
    echo ""
    echo "ENVIRONMENT VARIABLES:"
    echo "  INSTALL_PREFIX          Same as --prefix"
    echo "  BUILD_SSL               Enable SSL (yes/no)"
    echo "  BUILD_LIBSSH2           Enable libssh2 (yes/no)"
    echo "  BUILD_ZLIB              Enable zlib (yes/no)"
    echo "  BUILD_SHARED            Build shared library (yes/no)"
    echo "  BUILD_STATIC            Build static library (yes/no)"
    echo "  PARALLEL_JOBS           Number of parallel jobs"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        --source-dir)
            SOURCE_DIR="$2"
            shift 2
            ;;
        --build-type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        --enable-ssl)
            BUILD_SSL="yes"
            shift
            ;;
        --disable-ssl)
            BUILD_SSL="no"
            shift
            ;;
        --enable-libssh2)
            BUILD_LIBSSH2="yes"
            shift
            ;;
        --disable-libssh2)
            BUILD_LIBSSH2="no"
            shift
            ;;
        --enable-zlib)
            BUILD_ZLIB="yes"
            shift
            ;;
        --disable-zlib)
            BUILD_ZLIB="no"
            shift
            ;;
        --enable-shared)
            BUILD_SHARED="yes"
            shift
            ;;
        --disable-shared)
            BUILD_SHARED="no"
            shift
            ;;
        --enable-static)
            BUILD_STATIC="yes"
            shift
            ;;
        --disable-static)
            BUILD_STATIC="no"
            shift
            ;;
        --jobs)
            PARALLEL_JOBS="$2"
            shift 2
            ;;
        --skip-deps)
            SKIP_DEPS="yes"
            shift
            ;;
        --run-tests)
            RUN_TESTS="yes"
            shift
            ;;
        --help)
            show_help
            exit 0
            ;;
        *)
            log_error "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Check prerequisites
check_prerequisites() {
    log_info "Checking prerequisites..."
    
    if ! command_exists make; then
        log_error "make is required but not found"
        return 1
    fi
    
    if ! command_exists gcc && ! command_exists clang; then
        log_error "C compiler (gcc or clang) is required but not found"
        return 1
    fi
    
    log_success "Prerequisites check passed"
    return 0
}

# Script entry point
if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    if check_prerequisites; then
        main
        exit_code=$?
    else
        exit_code=1
    fi
    
    exit $exit_code
fi
