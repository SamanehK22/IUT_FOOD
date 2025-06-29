@echo off
REM Database Management Script for IUTFood Project (Windows)
REM Usage: manage_db.bat [command]

set DB_FILE=database.sqlite
set SCHEMA_FILE=schema.sql

if "%1"=="init" (
    echo Initializing database from schema...
    sqlite3 "%DB_FILE%" < "%SCHEMA_FILE%"
    echo Database initialized successfully!
    goto :eof
)

if "%1"=="reset" (
    echo Resetting database...
    if exist "%DB_FILE%" del "%DB_FILE%"
    sqlite3 "%DB_FILE%" < "%SCHEMA_FILE%"
    echo Database reset successfully!
    goto :eof
)

if "%1"=="backup" (
    for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
    set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
    set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"
    set "BACKUP_FILE=database_backup_%YYYY%%MM%%DD%_%HH%%Min%%Sec%.sqlite"
    echo Creating backup: %BACKUP_FILE%
    copy "%DB_FILE%" "%BACKUP_FILE%"
    echo Backup created successfully!
    goto :eof
)

if "%1"=="migrate" (
    echo Applying migrations...
    if exist "migrate_ingredients.sql" (
        sqlite3 "%DB_FILE%" < migrate_ingredients.sql
        echo Applied ingredients migration
    )
    if exist "migration_add_usernames.sql" (
        sqlite3 "%DB_FILE%" < migration_add_usernames.sql
        echo Applied username migration
    )
    echo Migrations completed!
    goto :eof
)

if "%1"=="schema" (
    echo Database schema:
    sqlite3 "%DB_FILE%" ".schema"
    goto :eof
)

if "%1"=="tables" (
    echo Database tables:
    sqlite3 "%DB_FILE%" ".tables"
    goto :eof
)

if "%1"=="info" (
    echo Database information:
    echo File: %DB_FILE%
    for %%A in ("%DB_FILE%") do echo Size: %%~zA bytes
    echo Tables:
    sqlite3 "%DB_FILE%" ".tables"
    goto :eof
)

echo Usage: %0 [command]
echo.
echo Commands:
echo   init     - Initialize database from schema
echo   reset    - Reset database (delete and recreate)
echo   backup   - Create backup of current database
echo   migrate  - Apply all migration scripts
echo   schema   - Show database schema
echo   tables   - List all tables
echo   info     - Show database information 