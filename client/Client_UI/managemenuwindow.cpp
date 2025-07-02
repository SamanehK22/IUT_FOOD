#include "managemenuwindow.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

ManageMenuWindow::ManageMenuWindow(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #EFE1C6; font-family: Segoe UI;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QPushButton *addButton = new QPushButton("Add Menu Item");
    addButton->setStyleSheet(
        "QPushButton { background-color: #F4A261; color: white; font-size: 16px; padding: 10px 20px; border: none; border-radius: 10px; }"
        "QPushButton:hover { background-color: #e07c3d; }"
    );
    connect(addButton, &QPushButton::clicked, this, &ManageMenuWindow::addMenuItem);
    mainLayout->addWidget(addButton);

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: transparent; border: none;");

    scrollWidget = new QWidget();
    itemsLayout = new QVBoxLayout(scrollWidget);
    itemsLayout->setSpacing(15);
    itemsLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);
}

void ManageMenuWindow::addMenuItem()
{
    QString name = QInputDialog::getText(this, "New Item", "Enter name:");
    if (name.isEmpty()) return;

    QString category = QInputDialog::getText(this, "Category", "Enter category:");
    if (category.isEmpty()) return;

    double price = QInputDialog::getDouble(this, "Price", "Enter price:", 0, 0, 1000, 2);

    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg)");

    addItemToList(name, category, price, imagePath);
}

void ManageMenuWindow::addItemToList(const QString &name, const QString &category, double price, const QString &imagePath)
{
    auto *item = new MenuItemManagerWidget(name, category, price, imagePath);
    itemsLayout->addWidget(item);

    connect(item, &MenuItemManagerWidget::editClicked, [=]() { handleEdit(item); });
    connect(item, &MenuItemManagerWidget::deleteClicked, [=]() { handleDelete(item); });
}

void ManageMenuWindow::handleEdit(MenuItemManagerWidget *item)
{
    QString name = QInputDialog::getText(this, "Edit Name", "Name:", QLineEdit::Normal, item->getName());
    if (!name.isEmpty())
        item->setName(name);

    QString category = QInputDialog::getText(this, "Edit Category", "Category:", QLineEdit::Normal, item->getCategory());
    if (!category.isEmpty())
        item->setCategory(category);

    double price = QInputDialog::getDouble(this, "Edit Price", "Price:", item->getPrice(), 0, 1000, 2);
    item->setPrice(price);

    QString imagePath = QFileDialog::getOpenFileName(this, "Change Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!imagePath.isEmpty())
        item->setImage(imagePath);
}

void ManageMenuWindow::handleDelete(MenuItemManagerWidget *item)
{
    if (QMessageBox::question(this, "Confirm", "Are you sure you want to delete this item?") == QMessageBox::Yes) {
        itemsLayout->removeWidget(item);
        item->deleteLater();
    }
}
