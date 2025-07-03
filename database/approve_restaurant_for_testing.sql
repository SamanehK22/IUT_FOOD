-- Script to approve restaurant for testing purposes
-- This will approve the restaurant created by "sahar" so she can log in

-- First, let's see what restaurants we have
SELECT 'CURRENT RESTAURANTS:' as info;
SELECT id, name, owner_id, status FROM restaurants;

-- Find the restaurant owned by sahar
SELECT 'RESTAURANT OWNED BY SAHAR:' as info;
SELECT r.id, r.name, r.owner_id, r.status, ro.first_name, ro.last_name, ro.email
FROM restaurants r
JOIN restaurant_owners ro ON r.owner_id = ro.id
WHERE ro.email = 'sahar@gmail.com';

-- Approve the restaurant
UPDATE restaurants 
SET status = 'approved', 
    approved_at = CURRENT_TIMESTAMP
WHERE owner_id = (
    SELECT id FROM restaurant_owners WHERE email = 'sahar@gmail.com'
);

-- Verify the approval
SELECT 'AFTER APPROVAL:' as info;
SELECT r.id, r.name, r.owner_id, r.status, r.approved_at, ro.first_name, ro.last_name, ro.email
FROM restaurants r
JOIN restaurant_owners ro ON r.owner_id = ro.id
WHERE ro.email = 'sahar@gmail.com'; 