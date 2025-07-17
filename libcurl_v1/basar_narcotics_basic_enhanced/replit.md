# Project Architecture Documentation

## Overview

This project has been transformed into a comprehensive technical analysis dashboard for evaluating the integration of libcurl into the BASAR framework as a replacement for QtFTP. The analysis examines the narcotics project's dependency issues and provides detailed implementation recommendations.

## User Preferences

Preferred communication style: Simple, everyday language.

## Recent Analysis (January 2025)

**BASAR Framework libcurl Integration Assessment**
- **Problem**: QtFTP removed from Qt 5.15.16+ breaking narcotics project
- **Solution**: Replace QtFTP with libcurl-based FTP client in BASAR framework
- **Status**: ✅ **IMPLEMENTATION COMPLETE** - All roadmap requirements delivered
- **Key Finding**: Perfect architectural fit with existing BASAR patterns
- **Risk Level**: Low - mature technology with proven track record

**Implementation Completed (January 2025)**
- ✅ **Phase 1**: Core libcurl FTP client in BASAR framework
  - Created `libbasarcmnutil_ftpclient.h/cpp` with full IFtpClient interface
  - Implemented CurlFtpClient class with all required operations
  - Added comprehensive error handling and progress callbacks
  - Integrated with BASAR exception system and logging
  
- ✅ **Phase 2**: Advanced features and error handling
  - SITE CHMOD support for file permissions
  - Timeout configuration and connection management
  - Passive/Active mode support
  - Binary/ASCII transfer modes
  - Comprehensive error mapping from libcurl codes
  
- ✅ **Phase 3**: Narcotics project migration
  - Updated `BasarFtpFileTransfer` base class using new FTP client
  - Implemented `NarcoticsFtpFileTransfer` concrete class
  - Maintained backward compatibility with existing interfaces
  - Added comprehensive logging and progress tracking
  - Preserved all existing functionality while removing QtFTP dependency

## System Architecture

### Frontend Architecture
- **Framework**: React 18 with TypeScript
- **Build Tool**: Vite for fast development and optimized builds
- **UI Framework**: shadcn/ui components built on Radix UI primitives
- **Styling**: Tailwind CSS with custom CSS variables for theming
- **State Management**: TanStack Query (React Query) for server state
- **Routing**: Wouter for lightweight client-side routing
- **Form Handling**: React Hook Form with Zod validation

### Backend Architecture
- **Runtime**: Node.js with Express.js framework
- **Language**: TypeScript with ES modules
- **Database ORM**: Drizzle ORM for type-safe database operations
- **Database**: PostgreSQL (configured via Neon serverless driver)
- **Session Management**: PostgreSQL-based sessions with connect-pg-simple
- **Development**: Hot reload with tsx

### Project Structure
```
├── client/          # Frontend React application
├── server/          # Backend Express application
├── shared/          # Shared types, schemas, and utilities
├── migrations/      # Database migration files
└── dist/           # Built application for production
```

## Key Components

### Database Layer
- **ORM**: Drizzle ORM with PostgreSQL dialect
- **Schema Definition**: Centralized in `shared/schema.ts` with Zod validation
- **Migrations**: Automated with drizzle-kit
- **Connection**: Neon serverless PostgreSQL driver for scalable connections

### API Layer
- **Framework**: Express.js with TypeScript
- **Request Logging**: Custom middleware for API request tracking
- **Error Handling**: Centralized error middleware
- **CORS**: Configured for cross-origin requests
- **JSON Parsing**: Built-in Express middleware

### Frontend Components
- **UI Library**: Complete shadcn/ui component system
- **Icons**: Lucide React for consistent iconography
- **Responsive Design**: Mobile-first approach with Tailwind breakpoints
- **Dark Mode**: CSS variable-based theming system
- **Accessibility**: Radix UI primitives ensure WCAG compliance

### Development Tools
- **Hot Reload**: Vite HMR for frontend, tsx for backend
- **Type Checking**: Strict TypeScript configuration
- **Code Quality**: ESLint and Prettier configurations
- **Build Process**: Separate builds for frontend (Vite) and backend (esbuild)

## Data Flow

### Request Flow
1. Client makes HTTP request through TanStack Query
2. Request goes through Express middleware stack
3. Route handlers process requests using storage interface
4. Database operations via Drizzle ORM
5. Response sent back through middleware chain
6. Client updates UI based on response

### Storage Abstraction
- **Interface**: `IStorage` defines CRUD operations
- **Implementation**: `MemStorage` for development (in-memory)
- **Production**: Can be swapped for database-backed storage
- **Methods**: Type-safe operations for user management

### State Management
- **Server State**: TanStack Query for caching and synchronization
- **Local State**: React's built-in useState and useContext
- **Form State**: React Hook Form for complex form handling
- **UI State**: Local component state for interactions

## External Dependencies

### Core Dependencies
- **@neondatabase/serverless**: PostgreSQL connection driver
- **drizzle-orm**: Type-safe ORM for database operations
- **@tanstack/react-query**: Server state management
- **react-hook-form**: Form handling and validation
- **zod**: Runtime type validation

### UI Dependencies
- **@radix-ui/***: Headless UI primitives
- **tailwindcss**: Utility-first CSS framework
- **class-variance-authority**: Type-safe component variants
- **clsx**: Conditional className utility

### Development Dependencies
- **vite**: Frontend build tool and dev server
- **tsx**: TypeScript execution for Node.js
- **esbuild**: Fast JavaScript bundler for production
- **typescript**: Type checking and compilation

## Deployment Strategy

### Build Process
1. **Frontend Build**: Vite builds React app to `dist/public`
2. **Backend Build**: esbuild bundles Express server to `dist/index.js`
3. **Database**: Migrations applied via `drizzle-kit push`

### Environment Configuration
- **Development**: Hot reload for both frontend and backend
- **Production**: Optimized builds with proper error handling
- **Database**: Environment variable-based connection string

### Hosting Considerations
- **Static Assets**: Frontend built to static files
- **Server**: Single Node.js process serving API and static files
- **Database**: External PostgreSQL instance via connection string
- **Sessions**: Persistent storage in PostgreSQL

### Scripts
- `dev`: Development mode with hot reload
- `build`: Production build for both frontend and backend
- `start`: Production server startup
- `db:push`: Apply database schema changes