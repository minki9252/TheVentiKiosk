#include "goodbyedialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

GoodbyeDialog::GoodbyeDialog(QWidget *parent) : QDialog(parent) {
    // 1. 창 설정 (테두리 없음, 중앙 배치)
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setFixedSize(400, 250);
    setStyleSheet("background-color: white; border: 3px solid #5d2d91; border-radius: 20px;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 2. 인사 문구 및 아이콘
    QLabel *lblIcon = new QLabel("👋", this);
    lblIcon->setAlignment(Qt::AlignCenter);
    lblIcon->setStyleSheet("font-size: 50pt; border: none;");

    QLabel *lblMsg = new QLabel("이용해 주셔서 감사합니다.\n안녕히 가세요!", this);
    lblMsg->setAlignment(Qt::AlignCenter);
    lblMsg->setStyleSheet("font-size: 16pt; font-weight: bold; color: #333; border: none;");

    layout->addWidget(lblIcon);
    layout->addWidget(lblMsg);

    // 3. 2초(2000ms) 후에 자동으로 창이 닫히도록 설정
    QTimer::singleShot(2000, this, &QDialog::accept);
}
