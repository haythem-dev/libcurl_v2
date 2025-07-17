#!/bin/bash

# libcurl Cleanup Script
# Removes build artifacts, source code, and optionally installed files

set -e  # Exit on any error

# Configuration
INSTALL_PREFIX="${INSTALL_PREFIX:-/usr/local}"
DOWNLOAD_DIR="${DOWNLOAD_DIR:-./libcurl-build}"
REMOVE_INSTALLED="${REMOVE_INSTALLED:-no}"
REMOVE_SOURCE="${REMOVE_SOURCE:-yes}"
REMOVE_ARCHIVES="${REMOVE_ARCHIVES:-yes}"

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

# Function to remove directory safely
remove_directory() {
    local dir="$1"
    local description="$2"
    
    if [ -d "$dir" ]; then
        log_info "Removing $description: $dir"
        if rm -rf "$dir"; then
            log_success "Removed $description"
        else
            log_error "Failed to remove $description: $dir"
            return 1
        fi
    else
        log_info "$description not found: $dir"
    fi
    
    return 0
}

# Function to remove file safely
remove_file() {
    local file="$1"
    local description="$2"
    
    if [ -f "$file" ]; then
        log_info "Removing $description: $file"
        if rm -f "$file"; then
            log_success "Removed $description"
        else
            log_error "Failed to remove $description: $file"
            return 1
        fi
    else
        log_info "$description not found: $file"
    fi
    
    return 0
}

# Function to find and remove source directories
cleanup_source_directories() {
    log_info "Cleaning up source directories..."
    
    local found_dirs=()
    
    # Look for curl source directories
    for pattern in "curl-*" "libcurl-*"; do
        for dir in $pattern; do
            if [ -d "$dir" ] && [ -f "$dir/configure" ]; then
                found_dirs+=("$dir")
            fi
        done
    done
    
    # Check in download directory
    if [ -d "$DOWNLOAD_DIR" ]; then
        for pattern in "$DOWNLOAD_DIR/curl-*" "$DOWNLOAD_DIR/libcurl-*"; do
            for dir in $pattern; do
                if [ -d "$dir" ] && [ -f "$dir/configure" ]; then
                    found_dirs+=("$dir")
                fi
            done
        done
    fi
    
    if [ ${#found_dirs[@]} -eq 0 ]; then
        log_info "No source directories found"
        return 0
    fi
    
    for dir in "${found_dirs[@]}"; do
        if [ "$REMOVE_SOURCE" = "yes" ]; then
            remove_directory "$dir" "source directory"
        else
            log_info "Keeping source directory: $dir"
        fi
    done
}

# Function to clean up build artifacts in source directories
cleanup_build_artifacts() {
    log_info "Cleaning up build artifacts..."
    
    local found_dirs=()
    
    # Look for curl source directories that still exist
    for pattern in "curl-*" "libcurl-*"; do
        for dir in $pattern; do
            if [ -d "$dir" ] && [ -f "$dir/configure" ]; then
                found_dirs+=("$dir")
            fi
        done
    done
    
    # Check in download directory
    if [ -d "$DOWNLOAD_DIR" ]; then
        for pattern in "$DOWNLOAD_DIR/curl-*" "$DOWNLOAD_DIR/libcurl-*"; do
            for dir in $pattern; do
                if [ -d "$dir" ] && [ -f "$dir/configure" ]; then
                    found_dirs+=("$dir")
                fi
            done
        done
    fi
    
    for source_dir in "${found_dirs[@]}"; do
        if [ -d "$source_dir" ]; then
            log_info "Cleaning build artifacts in: $source_dir"
            
            cd "$source_dir"
            
            # Clean using make if Makefile exists
            if [ -f "Makefile" ]; then
                log_info "Running make clean..."
                make clean 2>/dev/null || true
                make distclean 2>/dev/null || true
            fi
            
            # Remove common build artifacts
            local artifacts=(
                "*.o" "*.lo" "*.la" "*.so" "*.so.*" "*.dylib" "*.a"
                ".libs" ".deps" "autom4te.cache" "config.log" "config.status"
                "Makefile" "libtool" "stamp-h1" "config.h"
            )
            
            for artifact in "${artifacts[@]}"; do
                if ls $artifact >/dev/null 2>&1; then
                    rm -rf $artifact
                fi
            done
            
            cd - >/dev/null
        fi
    done
}

# Function to remove downloaded archives
cleanup_archives() {
    log_info "Cleaning up downloaded archives..."
    
    local archive_patterns=(
        "curl-*.tar.gz"
        "curl-*.tar.bz2"
        "curl-*.tar.xz"
        "curl-*.zip"
    )
    
    # Check current directory
    for pattern in "${archive_patterns[@]}"; do
        for file in $pattern; do
            if [ -f "$file" ]; then
                if [ "$REMOVE_ARCHIVES" = "yes" ]; then
                    remove_file "$file" "archive file"
                else
                    log_info "Keeping archive file: $file"
                fi
            fi
        done
    done
    
    # Check download directory
    if [ -d "$DOWNLOAD_DIR" ]; then
        for pattern in "${archive_patterns[@]}"; do
            for file in "$DOWNLOAD_DIR"/$pattern; do
                if [ -f "$file" ]; then
                    if [ "$REMOVE_ARCHIVES" = "yes" ]; then
                        remove_file "$file" "archive file"
                    else
                        log_info "Keeping archive file: $file"
                    fi
                fi
            done
        done
    fi
}

# Function to remove download directory
cleanup_download_directory() {
    if [ -d "$DOWNLOAD_DIR" ]; then
        # Check if directory is empty or only contains files we're keeping
        local remaining_files=$(find "$DOWNLOAD_DIR" -type f | wc -l)
        local remaining_dirs=$(find "$DOWNLOAD_DIR" -type d -not -path "$DOWNLOAD_DIR" | wc -l)
        
        if [ "$remaining_files" -eq 0 ] && [ "$remaining_dirs" -eq 0 ]; then
            remove_directory "$DOWNLOAD_DIR" "download directory"
        else
            log_info "Download directory not empty, keeping: $DOWNLOAD_DIR"
        fi
    fi
}

# Function to remove installed files
cleanup_installed_files() {
    if [ "$REMOVE_INSTALLED" != "yes" ]; then
        log_info "Skipping removal of installed files (use --remove-installed to enable)"
        return 0
    fi
    
    log_warning "Removing installed libcurl files..."
    
    # Files and directories to remove
    local files_to_remove=(
        "$INSTALL_PREFIX/bin/curl"
        "$INSTALL_PREFIX/bin/curl-config"
        "$INSTALL_PREFIX/lib/libcurl.so"
        "$INSTALL_PREFIX/lib/libcurl.so.*"
        "$INSTALL_PREFIX/lib/libcurl.dylib"
        "$INSTALL_PREFIX/lib/libcurl.a"
        "$INSTALL_PREFIX/lib/pkgconfig/libcurl.pc"
        "$INSTALL_PREFIX/include/curl"
        "$INSTALL_PREFIX/share/man/man1/curl.1"
        "$INSTALL_PREFIX/share/man/man1/curl-config.1"
        "$INSTALL_PREFIX/share/man/man3/libcurl*"
        "$INSTALL_PREFIX/share/aclocal/libcurl.m4"
    )
    
    # Check if we need sudo
    local need_sudo="no"
    if [ ! -w "$INSTALL_PREFIX" ]; then
        need_sudo="yes"
        log_warning "Removal requires root privileges"
    fi
    
    for item in "${files_to_remove[@]}"; do
        # Handle glob patterns
        if [[ "$item" == *"*"* ]]; then
            for file in $item; do
                if [ -e "$file" ]; then
                    if [ "$need_sudo" = "yes" ]; then
                        sudo rm -rf "$file" 2>/dev/null || true
                    else
                        rm -rf "$file" 2>/dev/null || true
                    fi
                    log_info "Removed: $file"
                fi
            done
        else
            if [ -e "$item" ]; then
                if [ "$need_sudo" = "yes" ]; then
                    sudo rm -rf "$item" 2>/dev/null || true
                else
                    rm -rf "$item" 2>/dev/null || true
                fi
                log_info "Removed: $item"
            fi
        fi
    done
    
    # Update library cache
    if command_exists ldconfig && [ "$need_sudo" = "yes" ]; then
        log_info "Updating library cache..."
        sudo ldconfig 2>/dev/null || true
    fi
    
    log_success "Installed files removal completed"
}

# Function to remove temporary files
cleanup_temporary_files() {
    log_info "Cleaning up temporary files..."
    
    # Remove temporary test files
    rm -rf /tmp/libcurl_test_* 2>/dev/null || true
    rm -rf /tmp/custom_build.sh 2>/dev/null || true
    
    log_success "Temporary files cleaned up"
}

# Function to display cleanup summary
display_summary() {
    echo
    log_info "Cleanup Summary"
    echo "==============="
    
    if [ "$REMOVE_SOURCE" = "yes" ]; then
        echo "✓ Source directories removed"
    else
        echo "- Source directories kept"
    fi
    
    if [ "$REMOVE_ARCHIVES" = "yes" ]; then
        echo "✓ Archive files removed"
    else
        echo "- Archive files kept"
    fi
    
    if [ "$REMOVE_INSTALLED" = "yes" ]; then
        echo "✓ Installed files removed"
    else
        echo "- Installed files kept"
    fi
    
    echo "✓ Build artifacts cleaned"
    echo "✓ Temporary files cleaned"
    
    echo
    log_info "Cleanup locations:"
    echo "  Download directory: $DOWNLOAD_DIR"
    echo "  Install prefix: $INSTALL_PREFIX"
    
    if [ "$REMOVE_INSTALLED" != "yes" ]; then
        echo
        log_info "To remove installed files, run:"
        echo "  $0 --remove-installed"
    fi
}

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo "Clean up libcurl build artifacts and files"
    echo ""
    echo "OPTIONS:"
    echo "  --prefix PATH           Installation prefix (default: $INSTALL_PREFIX)"
    echo "  --download-dir PATH     Download directory (default: $DOWNLOAD_DIR)"
    echo "  --remove-installed      Remove installed files (dangerous!)"
    echo "  --keep-source           Keep source directories"
    echo "  --keep-archives         Keep downloaded archives"
    echo "  --artifacts-only        Only clean build artifacts, keep everything else"
    echo "  --help                  Show this help message"
    echo ""
    echo "ENVIRONMENT VARIABLES:"
    echo "  INSTALL_PREFIX          Same as --prefix"
    echo "  DOWNLOAD_DIR            Same as --download-dir"
    echo "  REMOVE_INSTALLED        Remove installed files (yes/no)"
    echo "  REMOVE_SOURCE           Remove source directories (yes/no)"
    echo "  REMOVE_ARCHIVES         Remove archive files (yes/no)"
    echo ""
    echo "WARNING:"
    echo "  Using --remove-installed will uninstall libcurl from your system!"
    echo "  This may break other software that depends on libcurl."
}

# Function to confirm dangerous operations
confirm_operation() {
    if [ "$REMOVE_INSTALLED" = "yes" ]; then
        echo
        log_warning "WARNING: This will remove libcurl from your system!"
        log_warning "This may break other software that depends on libcurl."
        echo
        read -p "Are you sure you want to continue? [y/N]: " -r
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            log_info "Operation cancelled"
            exit 0
        fi
    fi
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        --download-dir)
            DOWNLOAD_DIR="$2"
            shift 2
            ;;
        --remove-installed)
            REMOVE_INSTALLED="yes"
            shift
            ;;
        --keep-source)
            REMOVE_SOURCE="no"
            shift
            ;;
        --keep-archives)
            REMOVE_ARCHIVES="no"
            shift
            ;;
        --artifacts-only)
            REMOVE_SOURCE="no"
            REMOVE_ARCHIVES="no"
            REMOVE_INSTALLED="no"
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

# Main cleanup function
main() {
    log_info "Starting libcurl cleanup process"
    echo "Configuration:"
    echo "  Download directory: $DOWNLOAD_DIR"
    echo "  Install prefix: $INSTALL_PREFIX"
    echo "  Remove source: $REMOVE_SOURCE"
    echo "  Remove archives: $REMOVE_ARCHIVES"
    echo "  Remove installed: $REMOVE_INSTALLED"
    echo
    
    # Confirm dangerous operations
    confirm_operation
    
    # Perform cleanup operations
    cleanup_build_artifacts
    echo
    
    cleanup_source_directories
    echo
    
    cleanup_archives
    echo
    
    cleanup_download_directory
    echo
    
    cleanup_temporary_files
    echo
    
    if [ "$REMOVE_INSTALLED" = "yes" ]; then
        cleanup_installed_files
        echo
    fi
    
    # Display summary
    display_summary
    
    log_success "Cleanup process completed successfully"
}

# Script entry point
if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    main
    exit 0
fi
