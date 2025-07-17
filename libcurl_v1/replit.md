# libcurl Build Guide & Automation

## Overview

This project is a comprehensive cross-platform toolkit for downloading, building, and installing libcurl from source code. It provides both a web-based interface and command-line automation for building libcurl with flexible configuration options. The application supports both Unix/Linux systems with shell scripts (.sh) and Windows 10+ with batch files (.bat). Built using Flask for the web interface, it provides real-time build progress tracking, system verification, and troubleshooting guidance.

## User Preferences

Preferred communication style: Simple, everyday language.

## Recent Changes

**January 2025**: Added full Windows 10+ support with batch script equivalents
- Created Windows batch files (.bat) for all Unix shell scripts (.sh)
- Updated web interface with platform-specific script downloads
- Enhanced Flask application with cross-platform command execution
- Added Windows-specific build tools detection and configuration
- Implemented automated platform detection in the frontend JavaScript
- Fixed Visual Studio 2022 configuration issues with dependency management
- Created optimized build scripts that disable optional dependencies
- Added vcpkg integration option for seamless Visual Studio development

## System Architecture

### Frontend Architecture
- **Technology**: HTML5, CSS3, JavaScript (vanilla)
- **UI Framework**: Bootstrap 5.1.3 for responsive design
- **Icons**: Font Awesome 6.0.0
- **Real-time Updates**: Server-Sent Events (SSE) for build progress streaming
- **Design Pattern**: Single-page application with dynamic content loading

### Backend Architecture
- **Framework**: Flask (Python 3.6+)
- **Architecture Style**: Monolithic web application
- **Process Management**: Threading for background build processes
- **Command Execution**: Subprocess module for shell command execution
- **Session Management**: In-memory state tracking for build progress

### Key Design Decisions
- **Flask over Django**: Chosen for simplicity and lightweight nature of the application
- **In-memory state**: Build status tracked in global variables for simplicity
- **Server-Sent Events**: Used instead of WebSockets for unidirectional real-time updates
- **Shell script integration**: Leverages existing shell scripts rather than reimplementing build logic

## Key Components

### Web Application (`app.py`)
- **Purpose**: Main Flask application serving the web interface
- **Key Features**:
  - Real-time build progress tracking
  - System information detection
  - Command execution with timeout handling
  - File upload/download capabilities
  - RESTful API endpoints

### Frontend Interface
- **Home Page** (`index.html`): Landing page with feature overview
- **Build Guide** (`templates/guide.html`): Interactive build interface
- **Styling** (`static/style.css`): Custom CSS for enhanced UI
- **JavaScript** (`static/script.js`): Client-side logic for real-time updates

### Documentation System
- **Build Guide** (`docs/build-guide.md`): Comprehensive building instructions
- **Troubleshooting** (`docs/troubleshooting.md`): Common issues and solutions
- **README**: Project overview and quick start guide

## Data Flow

### Build Process Flow
1. **User Initiation**: User starts build through web interface
2. **System Check**: Application verifies system dependencies
3. **Configuration**: User selects build options and parameters
4. **Execution**: Background thread executes build commands
5. **Progress Updates**: Real-time status updates via SSE
6. **Completion**: Build results and logs presented to user

### API Endpoints
- `GET /`: Home page
- `GET /guide`: Build guide interface
- `GET /api/system-info`: System information retrieval
- `POST /api/start-build`: Initiate build process
- `GET /api/build-status`: Build progress streaming
- `POST /api/cleanup`: Clean up build artifacts

## External Dependencies

### Python Dependencies
- **Flask**: Web framework for the application
- **Standard Library**: subprocess, threading, json, tempfile, os

### System Dependencies
- **Build Tools**: GCC compiler, make, autotools
- **Network Tools**: wget, curl for downloading
- **SSL Libraries**: OpenSSL, GnuTLS (optional)
- **Platform Tools**: Various Unix utilities

### Frontend Dependencies
- **Bootstrap 5.1.3**: CSS framework (CDN)
- **Font Awesome 6.0.0**: Icon library (CDN)
- **Modern Browser**: Support for Server-Sent Events

## Deployment Strategy

### Local Development
- **Setup**: Simple pip install for Flask dependency
- **Execution**: Direct Python script execution (`python3 app.py`)
- **Port**: Default Flask development server on localhost:5000

### Production Considerations
- **WSGI Server**: Should use production WSGI server (Gunicorn, uWSGI)
- **Reverse Proxy**: Nginx or Apache for static file serving
- **Security**: File upload validation and secure directory handling
- **Scalability**: Current architecture uses in-memory state (not suitable for multi-instance deployment)

### Platform Support
- **Primary**: Linux, macOS, and Windows 10+ systems
- **Unix/Linux**: Shell scripts (.sh) with bash, autotools, make, GCC/Clang
- **Windows**: Batch scripts (.bat) with Visual Studio Build Tools, CMake, PowerShell
- **Requirements**: Python 3.6+ for web interface, platform-specific build tools

## Technical Limitations

### Current Architecture Constraints
- **Single Instance**: Global state variables prevent horizontal scaling
- **Memory Storage**: Build logs and status stored in memory only
- **No Persistence**: Build history not saved between application restarts
- **Timeout Handling**: Fixed 300-second timeout for build commands

### Security Considerations
- **Command Injection**: Uses shell=True in subprocess calls
- **File Upload**: Basic filename validation but limited security checks
- **No Authentication**: No user authentication or authorization system
- **Local Execution**: Designed for local/trusted environment use