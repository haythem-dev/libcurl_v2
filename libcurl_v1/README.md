# libcurl Build Guide & Automation

A comprehensive guide and automation toolkit for downloading, building, and installing libcurl from source code. This project provides both a user-friendly web interface and command-line scripts to streamline the entire build process.

## 🚀 Features

- **Web-based Interface**: Modern, responsive web UI with real-time build progress
- **Automated Scripts**: Complete shell scripts for download, build, verify, and cleanup
- **Cross-Platform Support**: Works on Linux, macOS, Windows 10+, and other Unix-like systems
- **Flexible Configuration**: Extensive build options and presets
- **Progress Tracking**: Real-time build status and log streaming
- **System Verification**: Comprehensive checks for dependencies and successful installation
- **Troubleshooting Guide**: Detailed solutions for common build issues
- **Complete Documentation**: Step-by-step guides for manual and automated builds

## 📋 Table of Contents

- [Quick Start](#quick-start)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Scripts Overview](#scripts-overview)
- [Web Interface](#web-interface)
- [Manual Building](#manual-building)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## ⚡ Quick Start

### Web Interface (Recommended)

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd libcurl-build-guide
   ```

2. **Install dependencies** (Python 3.6+ required):
   ```bash
   pip3 install flask
   ```

3. **Start the web application**:
   ```bash
   python3 app.py
   ```

4. **Open your browser** to `http://localhost:5000`

5. **Follow the web interface** to configure and build libcurl

### Command Line (Unix/Linux/macOS)

1. **Install system dependencies**:
   ```bash
   bash scripts/install_dependencies.sh
   ```

2. **Download libcurl source**:
   ```bash
   bash scripts/download_libcurl.sh
   ```

3. **Build libcurl**:
   ```bash
   bash scripts/build_libcurl.sh
   ```

4. **Verify installation**:
   ```bash
   bash scripts/verify_build.sh
   ```

### Command Line (Windows 10+)

1. **Open Command Prompt or PowerShell as Administrator**

2. **Install system dependencies**:
   ```cmd
   scripts\install_dependencies.bat
   ```

3. **Download libcurl source**:
   ```cmd
   scripts\download_libcurl.bat
   ```

4. **Build libcurl**:
   ```cmd
   scripts\build_libcurl.bat
   ```

5. **Verify installation**:
   ```cmd
   scripts\verify_build.bat
   ```

## 📦 Installation

### Prerequisites

#### For All Platforms
- **Python 3.6+** (for web interface)
- **Internet connection** (for downloading source code)

#### For Unix/Linux/macOS
- **Bash shell** (for shell scripts)
- **GCC or Clang compiler**
- **Make and build tools**

#### For Windows 10+
- **Command Prompt or PowerShell**
- **Visual Studio Build Tools 2019/2022** or **Visual Studio Community**
- **CMake 3.15+**
- **Git for Windows** (optional but recommended)
- **7-Zip or Windows 10 built-in tar** (for archive extraction)

### System Dependencies

The build process requires various system packages. Use our automated installer:

```bash
# Install all required dependencies
bash scripts/install_dependencies.sh

# Install only essential dependencies
bash scripts/install_dependencies.sh --skip-optional

# Dry run to see what would be installed
bash scripts/install_dependencies.sh --dry-run
