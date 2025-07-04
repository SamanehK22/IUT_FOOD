# System Architecture Overview

## High-Level Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Client App    │    │   Server App    │    │   Database      │
│   (Qt/C++)      │◄──►│   (C++)         │◄──►│   (SQLite)      │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## Component Architecture

### Client Side Components

```
┌─────────────────────────────────────────────────────────────┐
│                    Client Application                       │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐         │
│  │   UI Layer  │  │ Network     │  │   Models    │         │
│  │             │  │ Manager     │  │             │         │
│  │ • Login     │  │             │  │ • User      │         │
│  │ • Menu      │  │ • TCP       │  │ • Restaurant│         │
│  │ • Cart      │  │ • HTTP      │  │ • Order     │         │
│  │ • Profile   │  │ • JSON      │  │ • MenuItem  │         │
│  └─────────────┘  └─────────────┘  └─────────────┘         │
└─────────────────────────────────────────────────────────────┘
```

### Server Side Components

```
┌─────────────────────────────────────────────────────────────┐
│                    Server Application                       │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐         │
│  │   Network   │  │   Business  │  │   Database  │         │
│  │   Layer     │  │   Logic     │  │   Layer     │         │
│  │             │  │             │  │             │         │
│  │ • TCP Server│  │ • Auth      │  │ • SQLite    │         │
│  │ • HTTP      │  │ • Orders    │  │ • Manager   │         │
│  │ • WebSocket │  │ • Chat      │  │ • Migrations│         │
│  └─────────────┘  └─────────────┘  └─────────────┘         │
└─────────────────────────────────────────────────────────────┘
```

## Communication Flow

### Authentication Flow
```
Client ──HTTP──► Server ──SQL──► Database
   ▲                              │
   └──────────JSON Response───────┘
```

### Real-time Communication Flow
```
Client ──TCP──► Server ──SQL──► Database
   ▲                              │
   └──────────TCP Response────────┘
```

### Order Processing Flow
```
Customer ──Order──► Server ──Notify──► Restaurant Owner
    ▲                              │
    └──────────Status Update───────┘
```

