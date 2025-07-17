#!/bin/bash

# libcurl Source Download Script
# Downloads the latest stable version of libcurl source code

set -e  # Exit on any error

# Configuration
CURL_VERSION="${CURL_VERSION:-8.5.0}"
DOWNLOAD_DIR="${DOWNLOAD_DIR:-./libcurl-build}"
CURL_URL="https://curl.se/download/curl-${CURL_VERSION}.tar.gz"
BACKUP_URL="https://github.com/curl/curl/releases/download/curl-${CURL_VERSION//./_}/curl-${CURL_VERSION}.tar.gz"

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

# Function to download file with fallback methods
download_file() {
    local url="$1"
    local output="$2"
    local description="$3"
    
    log_info "Downloading $description..."
    
    if command_exists curl; then
        if curl -L --progress-bar --fail "$url" -o "$output"; then
            return 0
        fi
    elif command_exists wget; then
        if wget --progress=bar:force:noscroll "$url" -O "$output"; then
            return 0
        fi
    else
        log_error "Neither curl nor wget is available for downloading"
        return 1
    fi
    
    return 1
}

# Function to verify download integrity
verify_download() {
    local file="$1"
    
    if [ ! -f "$file" ]; then
        log_error "Downloaded file not found: $file"
        return 1
    fi
    
    local file_size=$(stat -c%s "$file" 2>/dev/null || stat -f%z "$file" 2>/dev/null || echo "0")
    if [ "$file_size" -lt 1000000 ]; then  # Less than 1MB is suspicious
        log_warning "Downloaded file seems too small (${file_size} bytes). This might indicate a download error."
        return 1
    fi
    
    log_success "Download verification passed (${file_size} bytes)"
    return 0
}

# Function to extract archive
extract_archive() {
    local archive="$1"
    local extract_dir="$2"
    
    log_info "Extracting archive..."
    
    if command_exists tar; then
        if tar -xzf "$archive" -C "$extract_dir"; then
            log_success "Archive extracted successfully"
            return 0
        else
            log_error "Failed to extract archive with tar"
            return 1
        fi
    else
        log_error "tar command not available"
        return 1
    fi
}

# Function to cleanup on exit
cleanup_on_exit() {
    local exit_code=$?
    if [ $exit_code -ne 0 ]; then
        log_warning "Script interrupted or failed. Cleaning up..."
        if [ -f "$DOWNLOAD_DIR/curl-${CURL_VERSION}.tar.gz" ]; then
            rm -f "$DOWNLOAD_DIR/curl-${CURL_VERSION}.tar.gz"
        fi
    fi
}

# Set up cleanup trap
trap cleanup_on_exit EXIT

# Main execution
main() {
    log_info "Starting libcurl source download process"
    log_info "Version: $CURL_VERSION"
    log_info "Download directory: $DOWNLOAD_DIR"
    
    # Create download directory
    mkdir -p "$DOWNLOAD_DIR"
    cd "$DOWNLOAD_DIR"
    
    # Check if source already exists
    if [ -d "curl-${CURL_VERSION}" ]; then
        log_warning "Source directory curl-${CURL_VERSION} already exists"
        read -p "Do you want to remove it and re-download? [y/N]: " -r
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            rm -rf "curl-${CURL_VERSION}"
            log_info "Removed existing source directory"
        else
            log_info "Using existing source directory"
            return 0
        fi
    fi
    
    # Download source archive
    local archive_file="curl-${CURL_VERSION}.tar.gz"
    
    if [ -f "$archive_file" ]; then
        log_info "Archive already exists, verifying..."
        if verify_download "$archive_file"; then
            log_info "Using existing archive"
        else
            log_warning "Existing archive failed verification, re-downloading..."
            rm -f "$archive_file"
        fi
    fi
    
    if [ ! -f "$archive_file" ]; then
        log_info "Downloading from primary URL: $CURL_URL"
        if ! download_file "$CURL_URL" "$archive_file" "libcurl source"; then
            log_warning "Primary download failed, trying backup URL..."
            if ! download_file "$BACKUP_URL" "$archive_file" "libcurl source (backup)"; then
                log_error "All download attempts failed"
                return 1
            fi
        fi
        
        # Verify download
        if ! verify_download "$archive_file"; then
            log_error "Download verification failed"
            return 1
        fi
    fi
    
    # Extract archive
    if ! extract_archive "$archive_file" "."; then
        log_error "Failed to extract archive"
        return 1
    fi
    
    # Verify extraction
    if [ ! -d "curl-${CURL_VERSION}" ]; then
        log_error "Source directory not found after extraction"
        return 1
    fi
    
    # Display source information
    log_success "libcurl source downloaded and extracted successfully"
    log_info "Source location: $(pwd)/curl-${CURL_VERSION}"
    
    if [ -f "curl-${CURL_VERSION}/README" ]; then
        log_info "README file found"
    fi
    
    if [ -f "curl-${CURL_VERSION}/configure" ]; then
        log_info "Configure script found"
    fi
    
    if [ -f "curl-${CURL_VERSION}/CMakeLists.txt" ]; then
        log_info "CMake configuration found"
    fi
    
    # Show next steps
    echo
    log_info "Next steps:"
    echo "  1. cd curl-${CURL_VERSION}"
    echo "  2. Run the build script: ../build_libcurl.sh"
    echo "  3. Or configure manually: ./configure --help"
    
    log_success "Download process completed successfully"
}

# Check prerequisites
check_prerequisites() {
    log_info "Checking prerequisites..."
    
    if ! command_exists tar; then
        log_error "tar command is required but not found"
        return 1
    fi
    
    if ! command_exists curl && ! command_exists wget; then
        log_error "Either curl or wget is required for downloading"
        return 1
    fi
    
    log_success "Prerequisites check passed"
}

# Script entry point
if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    # Only run if script is executed directly, not sourced
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -v|--version)
                CURL_VERSION="$2"
                shift 2
                ;;
            -d|--dir)
                DOWNLOAD_DIR="$2"
                shift 2
                ;;
            -h|--help)
                echo "Usage: $0 [OPTIONS]"
                echo "Download libcurl source code"
                echo ""
                echo "OPTIONS:"
                echo "  -v, --version VERSION   Specify libcurl version (default: $CURL_VERSION)"
                echo "  -d, --dir DIRECTORY     Specify download directory (default: $DOWNLOAD_DIR)"
                echo "  -h, --help              Show this help message"
                exit 0
                ;;
            *)
                log_error "Unknown option: $1"
                exit 1
                ;;
        esac
    done
    
    # Run main function
    if check_prerequisites; then
        main
        exit_code=$?
    else
        exit_code=1
    fi
    
    exit $exit_code
fi
