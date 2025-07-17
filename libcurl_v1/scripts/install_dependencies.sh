#!/bin/bash

# libcurl Dependencies Installation Script
# Automatically detects system and installs required build dependencies

set -e  # Exit on any error

# Configuration
INSTALL_OPTIONAL="${INSTALL_OPTIONAL:-yes}"
INSTALL_DEVELOPMENT="${INSTALL_DEVELOPMENT:-no}"
DRY_RUN="${DRY_RUN:-no}"

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

# Function to check if running as root
is_root() {
    [ "$(id -u)" -eq 0 ]
}

# Function to run command with appropriate privileges
run_with_sudo() {
    local cmd="$1"
    
    if [ "$DRY_RUN" = "yes" ]; then
        log_info "DRY RUN: Would execute: $cmd"
        return 0
    fi
    
    if is_root; then
        eval "$cmd"
    else
        if command_exists sudo; then
            sudo bash -c "$cmd"
        else
            log_error "This script requires root privileges, but sudo is not available"
            log_error "Please run as root or install sudo"
            return 1
        fi
    fi
}

# Function to detect Linux distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    elif [ -f /etc/redhat-release ]; then
        echo "rhel"
    elif [ -f /etc/debian_version ]; then
        echo "debian"
    elif [ -f /etc/alpine-release ]; then
        echo "alpine"
    else
        echo "unknown"
    fi
}

# Function to detect package manager
detect_package_manager() {
    if command_exists apt-get; then
        echo "apt"
    elif command_exists yum; then
        echo "yum"
    elif command_exists dnf; then
        echo "dnf"
    elif command_exists apk; then
        echo "apk"
    elif command_exists pacman; then
        echo "pacman"
    elif command_exists zypper; then
        echo "zypper"
    elif command_exists brew; then
        echo "brew"
    else
        echo "unknown"
    fi
}

# Function to update package lists
update_package_lists() {
    local pm="$1"
    
    log_info "Updating package lists..."
    
    case "$pm" in
        apt)
            run_with_sudo "apt-get update"
            ;;
        yum)
            run_with_sudo "yum check-update" || true
            ;;
        dnf)
            run_with_sudo "dnf check-update" || true
            ;;
        apk)
            run_with_sudo "apk update"
            ;;
        pacman)
            run_with_sudo "pacman -Sy"
            ;;
        zypper)
            run_with_sudo "zypper refresh"
            ;;
        brew)
            brew update
            ;;
        *)
            log_warning "Unknown package manager: $pm"
            return 1
            ;;
    esac
}

# Function to install packages
install_packages() {
    local pm="$1"
    shift
    local packages="$*"
    
    if [ -z "$packages" ]; then
        return 0
    fi
    
    log_info "Installing packages: $packages"
    
    case "$pm" in
        apt)
            run_with_sudo "apt-get install -y $packages"
            ;;
        yum)
            run_with_sudo "yum install -y $packages"
            ;;
        dnf)
            run_with_sudo "dnf install -y $packages"
            ;;
        apk)
            run_with_sudo "apk add $packages"
            ;;
        pacman)
            run_with_sudo "pacman -S --noconfirm $packages"
            ;;
        zypper)
            run_with_sudo "zypper install -y $packages"
            ;;
        brew)
            brew install $packages
            ;;
        *)
            log_error "Unknown package manager: $pm"
            return 1
            ;;
    esac
}

# Function to install build tools
install_build_tools() {
    local pm="$1"
    local packages=""
    
    log_info "Installing build tools..."
    
    case "$pm" in
        apt)
            packages="build-essential"
            ;;
        yum|dnf)
            packages="gcc gcc-c++ make"
            # Try to install development tools group
            if [ "$pm" = "yum" ]; then
                run_with_sudo "yum groupinstall -y 'Development Tools'" || true
            else
                run_with_sudo "dnf groupinstall -y 'Development Tools'" || true
            fi
            ;;
        apk)
            packages="build-base"
            ;;
        pacman)
            packages="base-devel"
            ;;
        zypper)
            packages="gcc gcc-c++ make"
            ;;
        brew)
            # Xcode command line tools
            if ! command_exists gcc; then
                log_info "Installing Xcode Command Line Tools..."
                xcode-select --install 2>/dev/null || true
            fi
            packages=""
            ;;
        *)
            log_error "Unknown package manager: $pm"
            return 1
            ;;
    esac
    
    if [ -n "$packages" ]; then
        install_packages "$pm" "$packages"
    fi
}

# Function to install autotools
install_autotools() {
    local pm="$1"
    local packages=""
    
    log_info "Installing autotools..."
    
    case "$pm" in
        apt)
            packages="autoconf automake libtool pkg-config"
            ;;
        yum|dnf)
            packages="autoconf automake libtool pkgconfig"
            ;;
        apk)
            packages="autoconf automake libtool pkgconfig"
            ;;
        pacman)
            packages="autoconf automake libtool pkgconf"
            ;;
        zypper)
            packages="autoconf automake libtool pkg-config"
            ;;
        brew)
            packages="autoconf automake libtool pkg-config"
            ;;
        *)
            log_error "Unknown package manager: $pm"
            return 1
            ;;
    esac
    
    install_packages "$pm" "$packages"
}

# Function to install SSL development libraries
install_ssl_dev() {
    local pm="$1"
    local packages=""
    
    log_info "Installing SSL development libraries..."
    
    case "$pm" in
        apt)
            packages="libssl-dev"
            ;;
        yum|dnf)
            packages="openssl-devel"
            ;;
        apk)
            packages="openssl-dev"
            ;;
        pacman)
            packages="openssl"
            ;;
        zypper)
            packages="openssl-devel"
            ;;
        brew)
            packages="openssl"
            ;;
        *)
            log_error "Unknown package manager: $pm"
            return 1
            ;;
    esac
    
    install_packages "$pm" "$packages"
}

# Function to install compression libraries
install_compression_dev() {
    local pm="$1"
    local packages=""
    
    log_info "Installing compression development libraries..."
    
    case "$pm" in
        apt)
            packages="zlib1g-dev"
            if [ "$INSTALL_OPTIONAL" = "yes" ]; then
                packages="$packages libbrotli-dev"
            fi
            ;;
        yum|dnf)
            packages="zlib-devel"
            if [ "$INSTALL_OPTIONAL" = "yes" ]; then
                packages="$packages brotli-devel"
            fi
            ;;
        apk)
            packages="zlib-dev"
            if [ "$INSTALL_OPTIONAL" = "yes" ]; then
                packages="$packages brotli-dev"
            fi
            ;;
        pacman)
            packages="zlib"
            if [ "$INSTALL_OPTIONAL" = "yes" ]; then
                packages="$packages brotli"
            fi
            ;;
        zypper)
            packages="zlib-devel"
            if [ "$INSTALL_OPTIONAL" = "yes" ]; then
                packages="$packages libbrotli-devel"
            fi
            ;;
        brew)
            packages="zlib"
            if [ "$INSTALL_OPTIONAL" = "yes" ]; then
                packages="$packages brotli"
            fi
            ;;
        *)
            log_error "Unknown package manager: $pm"
            return 1
            ;;
    esac
    
    install_packages "$pm" "$packages"
}

# Function to install SSH libraries
install_ssh_dev() {
    local pm="$1"
    local packages=""
    
    if [ "$INSTALL_OPTIONAL" != "yes" ]; then
        log_info "Skipping SSH libraries (optional dependencies disabled)"
        return 0
    fi
    
    log_info "Installing SSH development libraries..."
    
    case "$pm" in
        apt)
            packages="libssh2-1-dev"
            ;;
        yum|dnf)
            packages="libssh2-devel"
            ;;
        apk)
            packages="libssh2-dev"
            ;;
        pacman)
            packages="libssh2"
            ;;
        zypper)
            packages="libssh2-devel"
            ;;
        brew)
            packages="libssh2"
            ;;
        *)
            log_error "Unknown package manager: $pm"
            return 1
            ;;
    esac
    
    install_packages "$pm" "$packages"
}

# Function to install HTTP/2 libraries
install_http2_dev() {
    local pm="$1"
    local packages=""
    
    if [ "$INSTALL_OPTIONAL" != "yes" ]; then
        log_info "Skipping HTTP/2 libraries (optional dependencies disabled)"
        return 0
    fi
    
    log_info "Installing HTTP/2 development libraries..."
    
    case "$pm" in
        apt)
            packages="libnghttp2-dev"
            ;;
        yum|dnf)
            packages="libnghttp2-devel"
            ;;
        apk)
            packages="nghttp2-dev"
            ;;
        pacman)
            packages="libnghttp2"
            ;;
        zypper)
            packages="libnghttp2-devel"
            ;;
        brew)
            packages="nghttp2"
            ;;
        *)
            log_warning "HTTP/2 libraries not available for package manager: $pm"
            return 0
            ;;
    esac
    
    install_packages "$pm" "$packages"
}

# Function to install development tools
install_development_tools() {
    local pm="$1"
    local packages=""
    
    if [ "$INSTALL_DEVELOPMENT" != "yes" ]; then
        return 0
    fi
    
    log_info "Installing development tools..."
    
    case "$pm" in
        apt)
            packages="git wget curl vim gdb valgrind"
            ;;
        yum|dnf)
            packages="git wget curl vim gdb valgrind"
            ;;
        apk)
            packages="git wget curl vim gdb"
            ;;
        pacman)
            packages="git wget curl vim gdb valgrind"
            ;;
        zypper)
            packages="git wget curl vim gdb valgrind"
            ;;
        brew)
            packages="git wget curl vim"
            ;;
        *)
            log_warning "Development tools not configured for package manager: $pm"
            return 0
            ;;
    esac
    
    install_packages "$pm" "$packages"
}

# Function to install distribution-specific packages
install_distro_specific() {
    local distro="$1"
    local pm="$2"
    
    case "$distro" in
        ubuntu|debian)
            # Install CA certificates
            install_packages "$pm" "ca-certificates"
            ;;
        centos|rhel)
            # Install EPEL repository for additional packages
            if [ "$pm" = "yum" ]; then
                log_info "Installing EPEL repository..."
                run_with_sudo "yum install -y epel-release" || true
            elif [ "$pm" = "dnf" ]; then
                log_info "Installing EPEL repository..."
                run_with_sudo "dnf install -y epel-release" || true
            fi
            ;;
        fedora)
            # Fedora has most packages in main repos
            ;;
        alpine)
            # Install CA certificates
            install_packages "$pm" "ca-certificates"
            ;;
        arch)
            # Update keyring first
            run_with_sudo "pacman -S --noconfirm archlinux-keyring" || true
            ;;
        opensuse*)
            # Add development repository
            log_info "Adding development patterns..."
            run_with_sudo "zypper install -y -t pattern devel_basis" || true
            ;;
    esac
}

# Function to verify installation
verify_installation() {
    log_info "Verifying installation..."
    
    local missing_tools=()
    
    # Check essential tools
    local essential_tools=("gcc" "make" "pkg-config")
    for tool in "${essential_tools[@]}"; do
        if ! command_exists "$tool"; then
            missing_tools+=("$tool")
        fi
    done
    
    # Check optional tools
    if [ "$INSTALL_OPTIONAL" = "yes" ]; then
        local optional_tools=("autoconf" "automake" "libtool")
        for tool in "${optional_tools[@]}"; do
            if ! command_exists "$tool"; then
                missing_tools+=("$tool")
            fi
        done
    fi
    
    if [ ${#missing_tools[@]} -eq 0 ]; then
        log_success "All required tools are available"
        return 0
    else
        log_error "Missing tools: ${missing_tools[*]}"
        return 1
    fi
}

# Function to display installation summary
display_summary() {
    echo
    log_info "Installation Summary"
    echo "===================="
    
    local distro=$(detect_distro)
    local pm=$(detect_package_manager)
    
    echo "Operating System: $distro"
    echo "Package Manager: $pm"
    echo "Install Optional: $INSTALL_OPTIONAL"
    echo "Install Development: $INSTALL_DEVELOPMENT"
    
    echo
    echo "Installed Tools:"
    
    local tools=("gcc" "g++" "make" "pkg-config" "autoconf" "automake" "libtool")
    for tool in "${tools[@]}"; do
        if command_exists "$tool"; then
            echo "  ✓ $tool"
        else
            echo "  ✗ $tool"
        fi
    done
    
    echo
    echo "Next Steps:"
    echo "  1. Download libcurl source: bash scripts/download_libcurl.sh"
    echo "  2. Build libcurl: bash scripts/build_libcurl.sh"
    echo "  3. Verify installation: bash scripts/verify_build.sh"
}

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo "Install dependencies required for building libcurl from source"
    echo ""
    echo "OPTIONS:"
    echo "  --skip-optional      Skip optional dependencies (SSH, HTTP/2, etc.)"
    echo "  --with-development   Install additional development tools"
    echo "  --dry-run           Show what would be installed without actually installing"
    echo "  --help              Show this help message"
    echo ""
    echo "ENVIRONMENT VARIABLES:"
    echo "  INSTALL_OPTIONAL     Install optional dependencies (yes/no, default: yes)"
    echo "  INSTALL_DEVELOPMENT  Install development tools (yes/no, default: no)"
    echo "  DRY_RUN             Show commands without executing (yes/no, default: no)"
    echo ""
    echo "This script supports:"
    echo "  - Ubuntu/Debian (apt)"
    echo "  - CentOS/RHEL/Fedora (yum/dnf)"
    echo "  - Alpine Linux (apk)"
    echo "  - Arch Linux (pacman)"
    echo "  - openSUSE (zypper)"
    echo "  - macOS (brew)"
}

# Main installation function
main() {
    log_info "Starting libcurl dependencies installation"
    
    # Detect system
    local distro=$(detect_distro)
    local pm=$(detect_package_manager)
    
    log_info "Detected distribution: $distro"
    log_info "Detected package manager: $pm"
    
    if [ "$pm" = "unknown" ]; then
        log_error "Unsupported package manager"
        log_error "Please install dependencies manually or use a supported system"
        return 1
    fi
    
    # Check for network connectivity
    if ! curl -s --max-time 5 http://httpbin.org/get >/dev/null 2>&1; then
        log_warning "Network connectivity check failed"
        log_warning "Installation may fail if packages need to be downloaded"
    fi
    
    # Update package lists
    update_package_lists "$pm"
    
    # Install distribution-specific packages first
    install_distro_specific "$distro" "$pm"
    
    # Install core dependencies
    install_build_tools "$pm"
    install_autotools "$pm"
    install_ssl_dev "$pm"
    install_compression_dev "$pm"
    
    # Install optional dependencies
    if [ "$INSTALL_OPTIONAL" = "yes" ]; then
        install_ssh_dev "$pm"
        install_http2_dev "$pm"
    fi
    
    # Install development tools
    install_development_tools "$pm"
    
    # Verify installation
    if verify_installation; then
        log_success "Dependencies installation completed successfully"
    else
        log_warning "Some dependencies may be missing"
    fi
    
    # Display summary
    display_summary
    
    return 0
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --skip-optional)
            INSTALL_OPTIONAL="no"
            shift
            ;;
        --with-development)
            INSTALL_DEVELOPMENT="yes"
            shift
            ;;
        --dry-run)
            DRY_RUN="yes"
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

# Script entry point
if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    main
    exit $?
fi
