# Database Files

This folder contains all database-related files for the IUTFood project.

## Files

### `schema.sql`
- **Purpose**: Main database schema with all tables
- **Contains**: Complete table definitions for customers, restaurants, orders, menus, etc.
- **Usage**: Run this to create a fresh database with all tables

### `database.sqlite`
- **Purpose**: Main SQLite database file
- **Contains**: All application data
- **Usage**: This is the actual database used by the application

### `migrate_ingredients.sql`
- **Purpose**: Migration script to add ingredients and category fields
- **Contains**: ALTER TABLE statements for menu_items table
- **Usage**: Run this to add new fields to existing database

### `migration_add_usernames.sql`
- **Purpose**: Migration script to add username fields
- **Contains**: ALTER TABLE statements for user tables
- **Usage**: Run this to add username fields to existing database

### `manage_db.sh` (Linux/Mac)
- **Purpose**: Database management script for Unix systems
- **Usage**: `./manage_db.sh [command]`

### `manage_db.bat` (Windows)
- **Purpose**: Database management script for Windows
- **Usage**: `manage_db.bat [command]`

## Quick Start

### Using Management Scripts

#### Linux/Mac
```bash
# Initialize database
./manage_db.sh init

# Create backup
./manage_db.sh backup

# Apply migrations
./manage_db.sh migrate

# Show database info
./manage_db.sh info
```

#### Windows
```cmd
# Initialize database
manage_db.bat init

# Create backup
manage_db.bat backup

# Apply migrations
manage_db.bat migrate

# Show database info
manage_db.bat info
```

### Manual Commands

#### Create Fresh Database
```bash
# Create new database from schema
sqlite3 database.sqlite < schema.sql
```

#### Apply Migrations
```bash
# Apply ingredients migration
sqlite3 database.sqlite < migrate_ingredients.sql

# Apply username migration
sqlite3 database.sqlite < migration_add_usernames.sql
```

#### Backup Database
```bash
# Create backup
cp database.sqlite database_backup_$(date +%Y%m%d_%H%M%S).sqlite
```

#### Reset Database
```bash
# Remove and recreate
rm database.sqlite
sqlite3 database.sqlite < schema.sql
```

## Management Script Commands

### `init`
- Creates a new database from the schema file
- Use when setting up the project for the first time

### `reset`
- Deletes the current database and creates a fresh one
- **Warning**: This will delete all data!

### `backup`
- Creates a timestamped backup of the current database
- Backup files are named: `database_backup_YYYYMMDD_HHMMSS.sqlite`

### `migrate`
- Applies all available migration scripts
- Safe to run multiple times (migrations are idempotent)

### `schema`
- Shows the current database schema
- Useful for debugging and verification

### `tables`
- Lists all tables in the database
- Quick overview of database structure

### `info`
- Shows database file information
- Displays file size, tables, and other details

## Database Schema Overview

### Core Tables
- `customers` - Customer user accounts
- `restaurant_owners` - Restaurant owner accounts
- `restaurants` - Restaurant information
- `menus` - Restaurant menus
- `menu_items` - Individual menu items
- `orders` - Customer orders
- `order_items` - Items within orders
- `order_feedback` - Customer feedback on orders
- `ratings` - Restaurant ratings
- `admin_logs` - Administrative actions

### Features
- ✅ User authentication and profiles
- ✅ Restaurant management and approval
- ✅ Menu and menu item management
- ✅ Order processing and status tracking
- ✅ Customer feedback and ratings
- ✅ Admin logging and oversight
- ✅ Image support for restaurants and menu items
- ✅ Order status management with validation

## Troubleshooting

### Database Connection Issues
- Ensure the database file exists: `ls -la database.sqlite`
- Check file permissions: `chmod 644 database.sqlite`
- Verify schema is applied: `./manage_db.sh schema`

### Migration Issues
- Check if migrations were applied: `sqlite3 database.sqlite ".schema menu_items"`
- Reapply migrations: `./manage_db.sh migrate`
- Reset database if needed: `./manage_db.sh reset`

### Backup and Restore
- Create backup before major changes: `./manage_db.sh backup`
- Restore from backup: `cp database_backup_YYYYMMDD_HHMMSS.sqlite database.sqlite` 