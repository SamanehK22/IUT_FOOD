-- Migration to fix restaurant_owners and restaurants table data issues
-- This migration should be run after fixing the code bugs

-- First, let's see what data we have before fixing
SELECT 'BEFORE FIX - restaurant_owners:' as info;
SELECT id, first_name, last_name, email, restaurant_id, city, location FROM restaurant_owners;

SELECT 'BEFORE FIX - restaurants:' as info;
SELECT id, name, address, city, location, type, owner_id FROM restaurants;

-- Step 1: Fix restaurant_owners where restaurant_id contains city name instead of restaurant ID
-- If restaurant_id contains text (city name) instead of a number, we need to fix it
UPDATE restaurant_owners 
SET restaurant_id = NULL 
WHERE restaurant_id IS NOT NULL 
AND restaurant_id != '' 
AND restaurant_id NOT GLOB '*[0-9]*';

-- Step 2: Fix restaurants that don't have proper owner_id
-- For restaurants without owner_id, try to find the owner by matching restaurant name with owner's city
UPDATE restaurants 
SET owner_id = (
    SELECT ro.id 
    FROM restaurant_owners ro 
    WHERE ro.city = restaurants.city 
    AND ro.restaurant_id IS NULL
    LIMIT 1
)
WHERE owner_id IS NULL;

-- Step 3: Update restaurant_owners with the correct restaurant_id
UPDATE restaurant_owners 
SET restaurant_id = (
    SELECT r.id 
    FROM restaurants r 
    WHERE r.owner_id = restaurant_owners.id
    LIMIT 1
)
WHERE restaurant_id IS NULL;

-- Step 4: Clean up any restaurants that still don't have an owner (orphaned restaurants)
DELETE FROM restaurants WHERE owner_id IS NULL;

-- Show the results after fixing
SELECT 'AFTER FIX - restaurant_owners:' as info;
SELECT id, first_name, last_name, email, restaurant_id, city, location FROM restaurant_owners;

SELECT 'AFTER FIX - restaurants:' as info;
SELECT id, name, address, city, location, type, owner_id FROM restaurants;

-- Verify the relationships are correct
SELECT 'VERIFICATION - Restaurant Owner Relationships:' as info;
SELECT 
    ro.id as owner_id,
    ro.first_name || ' ' || ro.last_name as owner_name,
    ro.restaurant_id,
    r.id as restaurant_id,
    r.name as restaurant_name,
    r.owner_id as restaurant_owner_id
FROM restaurant_owners ro
LEFT JOIN restaurants r ON ro.restaurant_id = r.id
ORDER BY ro.id; 