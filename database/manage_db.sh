#!/bin/bash

# Database Management Script for IUTFood Project
# Usage: ./manage_db.sh [command] [options]

DB_FILE="database.sqlite"
SCHEMA_FILE="schema.sql"

case "$1" in
    "init")
        echo "Initializing database from schema..."
        sqlite3 "$DB_FILE" < "$SCHEMA_FILE"
        echo "Database initialized successfully!"
        ;;
    "reset")
        echo "Resetting database..."
        rm -f "$DB_FILE"
        sqlite3 "$DB_FILE" < "$SCHEMA_FILE"
        echo "Database reset successfully!"
        ;;
    "backup")
        BACKUP_FILE="database_backup_$(date +%Y%m%d_%H%M%S).sqlite"
        echo "Creating backup: $BACKUP_FILE"
        cp "$DB_FILE" "$BACKUP_FILE"
        echo "Backup created successfully!"
        ;;
    "migrate")
        echo "Applying migrations..."
        if [ -f "migrate_ingredients.sql" ]; then
            sqlite3 "$DB_FILE" < migrate_ingredients.sql
            echo "Applied ingredients migration"
        fi
        if [ -f "migration_add_usernames.sql" ]; then
            sqlite3 "$DB_FILE" < migration_add_usernames.sql
            echo "Applied username migration"
        fi
        echo "Migrations completed!"
        ;;
    "schema")
        echo "Database schema:"
        sqlite3 "$DB_FILE" ".schema"
        ;;
    "tables")
        echo "Database tables:"
        sqlite3 "$DB_FILE" ".tables"
        ;;
    "info")
        echo "Database information:"
        echo "File: $DB_FILE"
        echo "Size: $(du -h "$DB_FILE" | cut -f1)"
        echo "Tables:"
        sqlite3 "$DB_FILE" ".tables"
        ;;
    *)
        echo "Usage: $0 [command]"
        echo ""
        echo "Commands:"
        echo "  init     - Initialize database from schema"
        echo "  reset    - Reset database (delete and recreate)"
        echo "  backup   - Create backup of current database"
        echo "  migrate  - Apply all migration scripts"
        echo "  schema   - Show database schema"
        echo "  tables   - List all tables"
        echo "  info     - Show database information"
        ;;
esac 