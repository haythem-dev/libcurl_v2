# BASAR Framework Architecture

## Overview

The BASAR framework is a common framework used across the pharmaceutical software project that provides abstraction layers for various system functionalities including database operations, remote tasks, printing, email, and FTP operations. It follows a layered architecture pattern with interfaces and implementations.

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                    APPLICATION LAYER                            │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │   Narcotics     │  │   Other Apps    │  │   Future Apps   │ │
│  │   Application   │  │                 │  │                 │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                    BASAR FRAMEWORK                              │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │   Interface     │  │   Interface     │  │   Interface     │ │
│  │   Layer         │  │   Layer         │  │   Layer         │ │
│  │  (IFtpClient)   │  │  (IEmailSender) │  │  (IDBAccess)    │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │ Implementation  │  │ Implementation  │  │ Implementation  │ │
│  │   Layer         │  │   Layer         │  │   Layer         │ │
│  │ (FtpClient)     │  │ (EmailSender)   │  │ (DBAccess)      │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                  EXTERNAL LIBRARIES                             │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │    libcurl      │  │    libssh2      │  │   PostgreSQL    │ │
│  │   (FTP/HTTP)    │  │     (SSH)       │  │   (Database)    │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐ │
│  │   log4cplus     │  │      zlib       │  │      Qt5        │ │
│  │   (Logging)     │  │ (Compression)   │  │      (GUI)      │ │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

## BASAR Framework Components

### 1. libbasarcmnutil (Common Utilities)
- **Purpose**: Provides common utilities and abstractions
- **Key Components**:
  - FTP Client Interface and Implementation
  - Email functionality
  - Remote task execution
  - Credential management
  - Logging utilities

### 2. libbasarappl (Application Layer)
- **Purpose**: Application-level abstractions
- **Key Components**:
  - Application lifecycle management
  - Configuration management
  - Resource management

### 3. libbasardb* (Database Layer)
- **Purpose**: Database abstraction layer
- **Key Components**:
  - Database connection management
  - Query execution
  - Transaction management

### 4. libbasargui* (GUI Layer)
- **Purpose**: GUI abstractions for Qt applications
- **Key Components**:
  - Widget abstractions
  - Login dialogs
  - Common UI components

## Factory Pattern Implementation

The BASAR framework uses the Factory pattern extensively:

```cpp
// Factory functions in BASAR framework
namespace basar::cmnutil {
    std::unique_ptr<IFtpClient> createFtpClient();
    std::unique_ptr<IEmailSender> createEmailSender();
    std::unique_ptr<IRemoteTaskSubmitter> createRemoteTaskSubmitter();
}
```

## Benefits of BASAR Framework

1. **Abstraction**: Hides implementation details from applications
2. **Consistency**: Provides consistent APIs across different functionalities
3. **Maintainability**: Easy to update implementations without changing applications
4. **Testability**: Interfaces allow for easy mocking and testing
5. **Flexibility**: Easy to swap implementations (e.g., Qt5Ftp → libcurl)
6. **Regulatory Compliance**: Centralized validation and error handling

## Library Dependencies

The framework manages external library dependencies through:

1. **Property Sheets**: For Visual Studio builds
2. **Makefiles**: For Unix/Linux builds
3. **Ivy Configuration**: For dependency management
4. **Static Linking**: For pharmaceutical compliance requirements

## Error Handling Strategy

The framework implements consistent error handling:

```cpp
namespace basar::cmnutil {
    enum class ErrorCode {
        Success,
        ConnectionFailed,
        AuthenticationFailed,
        TransferFailed,
        NetworkError,
        InvalidParameter
    };
    
    class BasarRuntimeException {
        // Standardized exception handling
    };
}
```