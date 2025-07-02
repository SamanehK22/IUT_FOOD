#ifndef MENUITEMMANAGERWIDGET_H
#define MENUITEMMANAGERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MenuItemManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuItemManagerWidget(QString name,
                                   QString category,
                                   double price,
                                   QString imagePath = "",
                                   QWidget *parent = nullptr);

    QString getName() const;
    QString getCategory() const;
    double getPrice() const;
    QString getImagePath() const;

    void setName(const QString &name);
    void setCategory(const QString &category);
    void setPrice(double price);
    void setImage(const QString &imagePath);

signals:
    void editClicked();
    void deleteClicked();

private:
    QLabel *imageLabel;
    QLabel *nameLabel;
    QLabel *categoryLabel;
    QLabel *priceLabel;

    QPushButton *editButton;
    QPushButton *deleteButton;

    QString currentImagePath;
};

#endif // MENUITEMMANAGERWIDGET_H
