#include "cartwidget.h"
#include "ui_cartwidget.h"

cartwidget::cartwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::cartwidget)
{
    ui->setupUi(this);
}

cartwidget::~cartwidget()
{
    delete ui;
}

void cartwidget::updateCart(QList<KioskData> list)
{
    qDebug() << "updateCart 호출됨, 아이템 수:" << list.size(); // 추가!
    ui->ListCart->clear(); // 기존 목록 비우기

    for (const KioskData &data : list) {
        qDebug() << "추가중:" << data.menuName; // 추가!
        QString itemText = QString("%1 | %2 | %3원")
                               .arg(data.menuName)
                               .arg(data.summaryText)
                               .arg(data.totalPrice);
        ui->ListCart->addItem(itemText);
    }
}
