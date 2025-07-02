#include "ordercommand.h"
#include <QDebug>

OrderCommand::OrderCommand(Type type, QObject *parent)
    : Command(parent)
    , commandType(type)
    , dbManager(DatabaseManager::getInstance())
{
    // Set default priorities based on command type
    switch (type) {
        case Type::Create:
            setPriority(1); // High priority for new orders
            break;
        case Type::UpdateStatus:
            setPriority(2); // Medium priority for status updates
            break;
        case Type::Delete:
            setPriority(3); // Low priority for deletions
            break;
    }
}

OrderCommand::~OrderCommand()
{
}

void OrderCommand::setOrderData(const QString& orderId, const QString& customerId, const QString& restaurantId, const QVariantList& items)
{
    this->orderId = orderId;
    this->customerId = customerId;
    this->restaurantId = restaurantId;
    this->orderItems = items;
}

void OrderCommand::setStatus(const QString& status)
{
    this->status = status;
}

void OrderCommand::execute()
{
    switch (commandType) {
        case Type::Create:
            if (!dbManager->createOrder(customerId, restaurantId, orderItems)) {
                qDebug() << "Failed to create order";
                return;
            }
            qDebug() << "Order created successfully";
            break;

        case Type::UpdateStatus:
            if (!dbManager->updateOrderStatus(orderId, status)) {
                qDebug() << "Failed to update order status";
                return;
            }
            qDebug() << "Order status updated successfully";
            break;

        case Type::Delete:
            if (!dbManager->deleteOrder(orderId)) {
                qDebug() << "Failed to delete order";
                return;
            }
            qDebug() << "Order deleted successfully";
            break;
    }
}

int OrderCommand::getPriority() const
{
    return priority;
}

void OrderCommand::setPriority(int priority)
{
    this->priority = priority;
} 