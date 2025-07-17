# Replit.md

## Overview

This is a modern full-stack web application built with React, Express.js, and TypeScript. The project follows a monorepo structure with shared components between client and server, utilizing shadcn/ui for the frontend components and Drizzle ORM for database operations with PostgreSQL.

## User Preferences

Preferred communication style: Simple, everyday language.

## System Architecture

### Frontend Architecture
- **Framework**: React 18 with TypeScript
- **Styling**: Tailwind CSS with CSS variables for theming
- **UI Components**: shadcn/ui component library (Radix UI primitives)
- **State Management**: TanStack Query (React Query) for server state
- **Routing**: Wouter for client-side routing
- **Build Tool**: Vite for development and production builds

### Backend Architecture
- **Runtime**: Node.js with Express.js
- **Language**: TypeScript with ES modules
- **Database**: PostgreSQL with Drizzle ORM
- **Database Provider**: Neon Database (@neondatabase/serverless)
- **Session Management**: Uses connect-pg-simple for PostgreSQL session storage

### Project Structure
```
├── client/                 # Frontend React application
│   ├── src/
│   │   ├── components/ui/  # shadcn/ui components
│   │   ├── pages/          # Route components
│   │   ├── hooks/          # Custom React hooks
│   │   └── lib/            # Utilities and query client
├── server/                 # Backend Express application
│   ├── index.ts           # Main server entry point
│   ├── routes.ts          # API route definitions
│   ├── storage.ts         # Database abstraction layer
│   └── vite.ts            # Vite development integration
├── shared/                 # Shared code between client and server
│   └── schema.ts          # Database schema and types
└── migrations/            # Database migration files
```

## Key Components

### Database Layer
- **ORM**: Drizzle ORM with PostgreSQL dialect
- **Schema**: Centralized in `shared/schema.ts` for type sharing
- **Migrations**: Generated in `./migrations` directory
- **Storage Interface**: Abstracted through `IStorage` interface with in-memory implementation for development

### Authentication & Authorization
- **Current State**: Basic user schema with username/password fields
- **Session Management**: Configured for PostgreSQL sessions via connect-pg-simple
- **Validation**: Uses drizzle-zod for schema validation

### API Structure
- **Base Path**: All API routes prefixed with `/api`
- **Error Handling**: Centralized error middleware with status code handling
- **Request Logging**: Middleware for logging API requests with duration and response data

### Frontend Components
- **UI System**: Complete shadcn/ui component library implementation
- **Form Handling**: React Hook Form with resolvers
- **Responsive Design**: Mobile-first approach with Tailwind CSS
- **Dark Mode**: Built-in support via CSS variables

## Data Flow

1. **Client Requests**: Frontend makes HTTP requests to `/api/*` endpoints
2. **Server Processing**: Express routes handle requests using storage interface
3. **Database Operations**: Drizzle ORM performs PostgreSQL operations
4. **Response Handling**: TanStack Query manages client-side caching and state
5. **UI Updates**: React components re-render based on query state changes

## External Dependencies

### Core Dependencies
- **@neondatabase/serverless**: PostgreSQL database connection
- **drizzle-orm**: Type-safe database operations
- **@tanstack/react-query**: Server state management
- **@radix-ui/***: Headless UI components
- **react-hook-form**: Form handling
- **wouter**: Client-side routing

### Development Tools
- **Vite**: Frontend build tool and development server
- **TypeScript**: Type checking and compilation
- **Tailwind CSS**: Utility-first CSS framework
- **drizzle-kit**: Database schema management

## Deployment Strategy

### Development
- **Client**: Vite dev server with HMR
- **Server**: tsx for TypeScript execution with hot reloading
- **Database**: Environment variable `DATABASE_URL` for connection
- **Build Process**: Vite builds client, esbuild bundles server

### Production
- **Client Build**: Static files generated to `dist/public`
- **Server Build**: Bundled Node.js application in `dist/`
- **Database**: PostgreSQL connection via environment variables
- **Startup**: `node dist/index.js` for production server

### Environment Configuration
- **DATABASE_URL**: Required for PostgreSQL connection
- **NODE_ENV**: Controls development vs production behavior
- **Session Configuration**: Uses PostgreSQL for session storage

The architecture prioritizes type safety, developer experience, and modern web development practices while maintaining a clean separation between client and server concerns.