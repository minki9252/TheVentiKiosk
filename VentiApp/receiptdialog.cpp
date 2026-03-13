#include "receiptdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

ReceiptDialog::ReceiptDialog(QWidget *parent) : QDialog(parent) {
    // 1. 창 설정 (테두리 없고 둥근 모서리)
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setFixedSize(400, 250);
    setStyleSheet("background-color: white; border: 2px solid #5d2d91; border-radius: 15px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // 2. 메시지 라벨
    QLabel *lblIcon = new QLabel("📄", this);
    lblIcon->setAlignment(Qt::AlignCenter);
    lblIcon->setStyleSheet("font-size: 40pt; border: none;");

    QLabel *lblMsg = new QLabel("결제가 완료되었습니다.\n영수증을 출력하시겠습니까?", this);
    lblMsg->setAlignment(Qt::AlignCenter);
    lblMsg->setStyleSheet("font-size: 15pt; font-weight: bold; color: #333; border: none;");

    // 3. 버튼 레이아웃 (예 / 아니오)
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);

    QPushButton *btnYes = new QPushButton("예 (출력)", this);
    QPushButton *btnNo = new QPushButton("아니오", this);

    // 버튼 스타일 (더벤티 테마)
    QString btnStyle = "QPushButton { font-size: 14pt; padding: 15px; border-radius: 10px; font-weight: bold; }";
    btnYes->setStyleSheet(btnStyle + "QPushButton { background-color: #5d2d91; color: white; }");
    btnNo->setStyleSheet(btnStyle + "QPushButton { background-color: #eee; color: #333; }");

    btnLayout->addWidget(btnNo); // '아니오'를 왼쪽에 배치
    btnLayout->addWidget(btnYes);

    mainLayout->addWidget(lblIcon);
    mainLayout->addWidget(lblMsg);
    mainLayout->addLayout(btnLayout);

    // 4. 이벤트 연결
    connect(btnYes, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnNo, &QPushButton::clicked, this, &QDialog::reject);
}
