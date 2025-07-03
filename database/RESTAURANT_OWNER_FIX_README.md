# Restaurant Owner Data Fix

## Issue Description

There were multiple critical bugs in the restaurant owner registration and login process:

### 1. Parameter Order Bug
The parameters were passed in the wrong order to the `createRestaurantOwner` function. This caused:
- **City data was stored in the `restaurant_id` field** instead of the `city` field
- **Location data was stored in the `city` field** instead of the `location` field
- **Empty string was stored in the `location` field**

### 2. Missing Owner Link
The `createRestaurant` function was missing the `owner_id` parameter, which meant restaurants were created without proper ownership links.

### 3. Admin Approval Mechanism
Restaurant owners couldn't log in because the login function wasn't checking if their restaurant was approved by an admin. Restaurants are created with `status = 'pending'` and need admin approval to change to `status = 'approved'`.

### 4. Phone Number Handling Inconsistency
Restaurant owner login queries weren't using `CAST(phone AS TEXT)` like customer queries, causing login failures.

## Root Cause

In `server/authsystem.cpp`, line 103, the function call was:
```cpp
// WRONG - parameters in wrong order
success = dbManager->createRestaurantOwner(firstName, lastName, username, email, hashedPassword, phone, city, location, "");
```

The correct function signature is:
```cpp
bool createRestaurantOwner(const QString& firstName, const QString& lastName, const QString& username, const QString& email,
                          const QString& passwordHash, const QString& phone, const QString& restaurantId, const QString& city, const QString& location);
```

## Fixes Applied

### 1. Fixed Parameter Order in `authsystem.cpp`
```cpp
// CORRECT - parameters in right order
success = dbManager->createRestaurantOwner(firstName, lastName, username, email, hashedPassword, phone, "", city, location);
```

### 2. Added Missing `owner_id` Parameter to `createRestaurant`
- Updated function signature in `databasemanager.h`
- Updated implementation in `databasemanager.cpp`
- Updated function call in `authsystem.cpp`

### 3. Added Admin Approval Check to Login
- Added `getRestaurantById` function to `DatabaseManager`
- Modified login function to check restaurant approval status
- Restaurant owners can only log in if their restaurant is approved

### 4. Fixed Phone Number Handling
- Updated `getRestaurantOwnerByLoginId` to use `CAST(phone AS TEXT)`
- Updated `ownerPhoneExists` to use `CAST(phone AS TEXT)`
- Updated `customerPhoneExists` for consistency

### 5. Created Migration Scripts
- `migrate_fix_restaurant_owner_data.sql` - Fixes existing data issues
- `approve_restaurant_for_testing.sql` - Approves restaurant for testing

## How to Apply the Fix

1. **Code Fix**: The code changes have been applied to the source files
2. **Database Fix**: Run the migration scripts on your database:
   ```bash
   sqlite3 database_clean.sqlite < migrate_fix_restaurant_owner_data.sql
   sqlite3 database_clean.sqlite < approve_restaurant_for_testing.sql
   ```

## Verification

After applying the fix, verify that:
1. Restaurant owners have proper `restaurant_id` values (numeric or NULL)
2. Restaurants have proper `owner_id` values
3. The relationships between owners and restaurants are correct
4. Restaurant owners can log in only after their restaurant is approved
5. Phone number login works correctly for restaurant owners

## Prevention

To prevent similar issues in the future:
1. Always verify parameter order when calling functions
2. Use named parameters or structs for complex function calls
3. Add unit tests to verify data integrity
4. Use database constraints to enforce relationships 