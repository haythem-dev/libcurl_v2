#!/bin/bash

# libcurl Build Verification Script
# Verifies that libcurl was built and installed correctly

set -e  # Exit on any error

# Configuration
INSTALL_PREFIX="${INSTALL_PREFIX:-/usr/local}"
VERBOSE="${VERBOSE:-no}"

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

log_verbose() {
    if [ "$VERBOSE" = "yes" ]; then
        echo -e "${BLUE}[VERBOSE]${NC} $1"
    fi
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Test counter
TESTS_TOTAL=0
TESTS_PASSED=0
TESTS_FAILED=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_command="$2"
    local expected_result="${3:-0}"
    
    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    
    log_info "Running test: $test_name"
    
    if [ "$VERBOSE" = "yes" ]; then
        log_verbose "Command: $test_command"
    fi
    
    if eval "$test_command" >/dev/null 2>&1; then
        local result=0
    else
        local result=1
    fi
    
    if [ "$result" -eq "$expected_result" ]; then
        log_success "PASS: $test_name"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        return 0
    else
        log_error "FAIL: $test_name"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        
        if [ "$VERBOSE" = "yes" ]; then
            log_verbose "Expected result: $expected_result, Got: $result"
            log_verbose "Command output:"
            eval "$test_command" 2>&1 | sed 's/^/  /'
        fi
        return 1
    fi
}

# Function to check file existence
check_file_exists() {
    local file_path="$1"
    local description="$2"
    
    run_test "File exists: $description" "[ -f '$file_path' ]"
}

# Function to check directory existence
check_dir_exists() {
    local dir_path="$1"
    local description="$2"
    
    run_test "Directory exists: $description" "[ -d '$dir_path' ]"
}

# Function to check command availability
check_command() {
    local command_name="$1"
    local description="$2"
    
    run_test "Command available: $description" "command_exists '$command_name'"
}

# Function to verify curl binary
verify_curl_binary() {
    log_info "Verifying curl binary..."
    
    # Check if curl exists
    check_command "curl" "curl command"
    
    if command_exists curl; then
        # Check curl version
        run_test "curl version check" "curl --version | grep -q 'curl [0-9]'"
        
        # Check curl help
        run_test "curl help output" "curl --help | grep -q 'Usage:'"
        
        # Check supported protocols
        run_test "curl supports HTTP" "curl --version | grep -q 'http'"
        run_test "curl supports HTTPS" "curl --version | grep -q 'https'"
        
        if [ "$VERBOSE" = "yes" ]; then
            log_verbose "curl version information:"
            curl --version | sed 's/^/  /'
        fi
    fi
}

# Function to verify libcurl library
verify_libcurl_library() {
    log_info "Verifying libcurl library..."
    
    # Check library files
    if [ -f "$INSTALL_PREFIX/lib/libcurl.so" ]; then
        check_file_exists "$INSTALL_PREFIX/lib/libcurl.so" "shared library (libcurl.so)"
    elif [ -f "$INSTALL_PREFIX/lib/libcurl.dylib" ]; then
        check_file_exists "$INSTALL_PREFIX/lib/libcurl.dylib" "shared library (libcurl.dylib)"
    else
        log_warning "Shared library not found (might be disabled)"
    fi
    
    if [ -f "$INSTALL_PREFIX/lib/libcurl.a" ]; then
        check_file_exists "$INSTALL_PREFIX/lib/libcurl.a" "static library"
    else
        log_warning "Static library not found (might be disabled)"
    fi
    
    # Check pkg-config file
    check_file_exists "$INSTALL_PREFIX/lib/pkgconfig/libcurl.pc" "pkg-config file"
    
    # Test pkg-config functionality
    if command_exists pkg-config; then
        run_test "pkg-config libcurl version" "pkg-config --modversion libcurl"
        run_test "pkg-config libcurl cflags" "pkg-config --cflags libcurl"
        run_test "pkg-config libcurl libs" "pkg-config --libs libcurl"
        
        if [ "$VERBOSE" = "yes" ]; then
            log_verbose "pkg-config information:"
            echo "  Version: $(pkg-config --modversion libcurl 2>/dev/null || echo 'N/A')"
            echo "  CFLAGS: $(pkg-config --cflags libcurl 2>/dev/null || echo 'N/A')"
            echo "  LIBS: $(pkg-config --libs libcurl 2>/dev/null || echo 'N/A')"
        fi
    fi
}

# Function to verify header files
verify_headers() {
    log_info "Verifying header files..."
    
    # Check main header directory
    check_dir_exists "$INSTALL_PREFIX/include/curl" "curl include directory"
    
    # Check essential headers
    check_file_exists "$INSTALL_PREFIX/include/curl/curl.h" "main curl header"
    check_file_exists "$INSTALL_PREFIX/include/curl/easy.h" "curl easy interface header"
    check_file_exists "$INSTALL_PREFIX/include/curl/multi.h" "curl multi interface header"
    
    if [ "$VERBOSE" = "yes" ]; then
        log_verbose "Available header files:"
        if [ -d "$INSTALL_PREFIX/include/curl" ]; then
            ls -la "$INSTALL_PREFIX/include/curl/" | sed 's/^/  /'
        fi
    fi
}

# Function to test basic functionality
test_basic_functionality() {
    log_info "Testing basic functionality..."
    
    if command_exists curl; then
        # Test simple HTTP request
        run_test "HTTP request test" "curl -s --max-time 10 --fail http://httpbin.org/get"
        
        # Test HTTPS request
        run_test "HTTPS request test" "curl -s --max-time 10 --fail https://httpbin.org/get"
        
        # Test following redirects
        run_test "Redirect following test" "curl -s --max-time 10 --fail -L http://httpbin.org/redirect/1"
        
        # Test user agent
        run_test "User agent test" "curl -s --max-time 10 --fail -A 'libcurl-test' http://httpbin.org/user-agent"
        
        # Test POST request
        run_test "POST request test" "curl -s --max-time 10 --fail -X POST -d 'test=data' http://httpbin.org/post"
    else
        log_warning "curl command not available, skipping functionality tests"
    fi
}

# Function to test compilation
test_compilation() {
    log_info "Testing compilation with libcurl..."
    
    if ! command_exists gcc && ! command_exists clang; then
        log_warning "No C compiler available, skipping compilation test"
        return 0
    fi
    
    local compiler="gcc"
    if ! command_exists gcc && command_exists clang; then
        compiler="clang"
    fi
    
    # Create temporary test program
    local test_dir="/tmp/libcurl_test_$$"
    mkdir -p "$test_dir"
    
    cat > "$test_dir/test_libcurl.c" << 'EOF'
#include <stdio.h>
#include <curl/curl.h>

int main() {
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://httpbin.org/get");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if(res == CURLE_OK) {
            printf("libcurl test successful\n");
            return 0;
        } else {
            printf("libcurl test failed: %s\n", curl_easy_strerror(res));
            return 1;
        }
    }
    
    printf("Failed to initialize libcurl\n");
    return 1;
}
EOF
    
    # Try to compile and run
    local compile_flags=""
    if command_exists pkg-config && pkg-config --exists libcurl; then
        compile_flags="$(pkg-config --cflags --libs libcurl)"
    else
        compile_flags="-I$INSTALL_PREFIX/include -L$INSTALL_PREFIX/lib -lcurl"
    fi
    
    run_test "Compile test program" "$compiler -o '$test_dir/test_libcurl' '$test_dir/test_libcurl.c' $compile_flags"
    
    if [ -f "$test_dir/test_libcurl" ]; then
        # Set library path if needed
        export LD_LIBRARY_PATH="$INSTALL_PREFIX/lib:${LD_LIBRARY_PATH:-}"
        export DYLD_LIBRARY_PATH="$INSTALL_PREFIX/lib:${DYLD_LIBRARY_PATH:-}"
        
        run_test "Run test program" "$test_dir/test_libcurl"
        
        if [ "$VERBOSE" = "yes" ]; then
            log_verbose "Test program output:"
            "$test_dir/test_libcurl" 2>&1 | sed 's/^/  /' || true
        fi
    fi
    
    # Cleanup
    rm -rf "$test_dir"
}

# Function to check library dependencies
check_dependencies() {
    log_info "Checking library dependencies..."
    
    local lib_file=""
    if [ -f "$INSTALL_PREFIX/lib/libcurl.so" ]; then
        lib_file="$INSTALL_PREFIX/lib/libcurl.so"
    elif [ -f "$INSTALL_PREFIX/lib/libcurl.dylib" ]; then
        lib_file="$INSTALL_PREFIX/lib/libcurl.dylib"
    fi
    
    if [ -n "$lib_file" ]; then
        if command_exists ldd; then
            run_test "Check library dependencies (ldd)" "ldd '$lib_file'"
            
            if [ "$VERBOSE" = "yes" ]; then
                log_verbose "Library dependencies:"
                ldd "$lib_file" 2>/dev/null | sed 's/^/  /' || true
            fi
        elif command_exists otool; then
            run_test "Check library dependencies (otool)" "otool -L '$lib_file'"
            
            if [ "$VERBOSE" = "yes" ]; then
                log_verbose "Library dependencies:"
                otool -L "$lib_file" 2>/dev/null | sed 's/^/  /' || true
            fi
        else
            log_warning "No tool available to check library dependencies"
        fi
    else
        log_warning "No shared library found to check dependencies"
    fi
}

# Function to display verification summary
display_summary() {
    echo
    echo "================================================"
    log_info "Verification Summary"
    echo "================================================"
    echo "Total tests: $TESTS_TOTAL"
    echo "Passed: $TESTS_PASSED"
    echo "Failed: $TESTS_FAILED"
    echo
    
    if [ "$TESTS_FAILED" -eq 0 ]; then
        log_success "All verification tests passed!"
        echo "libcurl appears to be correctly built and installed."
    else
        log_warning "$TESTS_FAILED out of $TESTS_TOTAL tests failed"
        echo "There may be issues with the libcurl installation."
        echo "Run with --verbose for more detailed information."
    fi
    
    echo
    log_info "Installation details:"
    echo "  Install prefix: $INSTALL_PREFIX"
    
    if command_exists curl; then
        echo "  curl version: $(curl --version | head -1)"
    fi
    
    if command_exists pkg-config && pkg-config --exists libcurl; then
        echo "  libcurl version: $(pkg-config --modversion libcurl 2>/dev/null || echo 'Unknown')"
    fi
    
    echo
    log_info "Quick usage examples:"
    echo "  curl --version"
    echo "  curl -I https://httpbin.org/get"
    echo "  pkg-config --cflags --libs libcurl"
}

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo "Verify libcurl build and installation"
    echo ""
    echo "OPTIONS:"
    echo "  --prefix PATH    Installation prefix to verify (default: $INSTALL_PREFIX)"
    echo "  --verbose        Enable verbose output"
    echo "  --help           Show this help message"
    echo ""
    echo "ENVIRONMENT VARIABLES:"
    echo "  INSTALL_PREFIX   Same as --prefix"
    echo "  VERBOSE          Enable verbose output (yes/no)"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        --verbose)
            VERBOSE="yes"
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

# Main verification function
main() {
    log_info "Starting libcurl verification process"
    log_info "Install prefix: $INSTALL_PREFIX"
    echo
    
    # Run all verification tests
    verify_curl_binary
    echo
    
    verify_libcurl_library
    echo
    
    verify_headers
    echo
    
    check_dependencies
    echo
    
    test_basic_functionality
    echo
    
    test_compilation
    echo
    
    # Display summary
    display_summary
    
    # Return appropriate exit code
    if [ "$TESTS_FAILED" -eq 0 ]; then
        return 0
    else
        return 1
    fi
}

# Script entry point
if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    main
    exit $?
fi
