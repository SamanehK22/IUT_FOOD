#include "custompopup.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

CustomPopup::CustomPopup(const QString &message, QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(300, 130);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);

    QLabel *label = new QLabel("🎉 " + message, this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(R"(
        QLabel {
            font-size: 17px;
            color: #5A3E2B;
            font-weight: bold;
        }
    )");

    QWidget *background = new QWidget(this);
    background->setStyleSheet(R"(
        background-color: #FFE8CC;
        border-radius: 20px;
        border: 2px solid #F5B97D;
    )");

    QVBoxLayout *innerLayout = new QVBoxLayout(background);
    innerLayout->addWidget(label);
    layout->addWidget(background);

    // ✨ افکت سایه
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(150, 100, 50, 160));
    shadow->setOffset(0, 5);
    background->setGraphicsEffect(shadow);

    // 🕒 تایمر برای بستن خودکار
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &QWidget::close);
    timer->start(2200); // دو ثانیه نمایش
}
