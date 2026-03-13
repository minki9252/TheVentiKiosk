#include "loadingdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

LoadingDialog::LoadingDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); // 테두리 제거
    setFixedSize(350, 250);
    setStyleSheet("background-color: white; border: 3px solid #5d2d91; border-radius: 20px;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *lblIcon = new QLabel("💳", this); // 이모지가 안 나오면 텍스트로 대체 가능
    lblIcon->setAlignment(Qt::AlignCenter);
    lblIcon->setStyleSheet("font-size: 50pt;");

    QLabel *lblMsg = new QLabel("카드를 읽는 중입니다...\n잠시만 기다려주세요.", this);
    lblMsg->setAlignment(Qt::AlignCenter);
    lblMsg->setStyleSheet("font-size: 16pt; font-weight: bold; color: #333;");

    layout->addWidget(lblIcon);
    layout->addWidget(lblMsg);

    // [중요] 2.5초(2500ms) 뒤에 이 창을 닫으라는 명령입니다.
    QTimer::singleShot(2500, this, &QDialog::accept);
}
