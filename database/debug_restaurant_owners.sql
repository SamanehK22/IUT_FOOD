-- Debug script to check restaurant owners in the database

-- Check all restaurant owners
SELECT 'ALL RESTAURANT OWNERS:' as info;
SELECT id, first_name, last_name, email, phone, username, restaurant_id, city, location, status 
FROM restaurant_owners;

-- Check specifically for sahar
SELECT 'LOOKING FOR SAHAR:' as info;
SELECT id, first_name, last_name, email, phone, username, restaurant_id, city, location, status 
FROM restaurant_owners 
WHERE email LIKE '%sahar%' OR first_name LIKE '%sahar%' OR last_name LIKE '%sahar%';

-- Check for any restaurant owners with the email domain
SELECT 'RESTAURANT OWNERS WITH GMAIL:' as info;
SELECT id, first_name, last_name, email, phone, username, restaurant_id, city, location, status 
FROM restaurant_owners 
WHERE email LIKE '%@gmail.com%';

-- Check all restaurants
SELECT 'ALL RESTAURANTS:' as info;
SELECT id, name, owner_id, status, created_at 
FROM restaurants; 