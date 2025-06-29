-- Migration script to add ingredients and category columns to menu_items table
-- Run this script on existing databases to add ingredients and category support

-- Add ingredients column to menu_items table
ALTER TABLE menu_items ADD COLUMN ingredients TEXT;

-- Add category column to menu_items table
ALTER TABLE menu_items ADD COLUMN category TEXT;

-- Update existing menu items to have empty ingredients and category (optional)
-- UPDATE menu_items SET ingredients = '', category = '' WHERE ingredients IS NULL; 