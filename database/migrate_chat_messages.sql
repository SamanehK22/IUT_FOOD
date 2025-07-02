-- Migration: Add chat_messages table for order-based chat

CREATE TABLE IF NOT EXISTS chat_messages (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    order_id INTEGER NOT NULL,
    from_user_id TEXT NOT NULL,
    to_user_id TEXT NOT NULL,
    content TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    is_read INTEGER DEFAULT 0,
    FOREIGN KEY(order_id) REFERENCES orders(id)
);

CREATE INDEX IF NOT EXISTS idx_chat_messages_order_id ON chat_messages(order_id);
CREATE INDEX IF NOT EXISTS idx_chat_messages_from_user_id ON chat_messages(from_user_id);
CREATE INDEX IF NOT EXISTS idx_chat_messages_to_user_id ON chat_messages(to_user_id); 