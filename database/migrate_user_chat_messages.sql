-- Migration: Add user_chat_messages table for user-based chat

CREATE TABLE IF NOT EXISTS user_chat_messages (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    from_user_id TEXT NOT NULL,
    to_user_id TEXT NOT NULL,
    content TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    is_read INTEGER DEFAULT 0
);

CREATE INDEX IF NOT EXISTS idx_user_chat_from_user_id ON user_chat_messages(from_user_id);
CREATE INDEX IF NOT EXISTS idx_user_chat_to_user_id ON user_chat_messages(to_user_id); 