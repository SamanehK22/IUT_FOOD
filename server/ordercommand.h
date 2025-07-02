#ifndef ORDERCOMMAND_H
#define ORDERCOMMAND_H

#include "command.h"
#include "databasemanager.h"
#include <QString>
#include <QVariantList>

class OrderCommand : public Command
{
    Q_OBJECT

public:
    enum class Type {
        Create,
        UpdateStatus,
        Delete
    };

    explicit OrderCommand(Type type, QObject *parent = nullptr);
    ~OrderCommand();

    void setOrderData(const QString& orderId, const QString& customerId, const QString& restaurantId, const QVariantList& items = QVariantList());
    void setStatus(const QString& status);
    void execute() override;
    int getPriority() const override;
    void setPriority(int priority) override;

private:
    Type commandType;
    QString orderId;
    QString customerId;
    QString restaurantId;
    QVariantList orderItems;
    QString status;
    DatabaseManager* dbManager;
};

#endif // ORDERCOMMAND_H 