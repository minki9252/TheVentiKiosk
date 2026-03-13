#include "pointinputview.h"
#include "ui_pointinputview.h"


// 생성자에 connectButtons() 호출 추가
PointInputView::PointInputView(QWidget *parent)
    : QWidget(parent), ui(new Ui::PointInputView)
{
    ui->setupUi(this);
    connectButtons(); // ← 이 줄 추가
}

PointInputView::~PointInputView()
{
    delete ui;
}

// ↓ 아래 두 함수 전체 추가

void PointInputView::connectButtons()
{
    connect(ui->pushButton, &QPushButton::clicked, this, [this](){ emit digitPressed("1"); });
    connect(ui->pushButton_2,    &QPushButton::clicked, this, [this](){ emit digitPressed("2"); });
    connect(ui->pushButton_3, &QPushButton::clicked, this, [this](){ emit digitPressed("3"); });
    connect(ui->pushButton_4,  &QPushButton::clicked, this, [this](){ emit digitPressed("4"); });
    connect(ui->pushButton_5, &QPushButton::clicked, this, [this](){ emit digitPressed("5"); });
    connect(ui->pushButton_6,  &QPushButton::clicked, this, [this](){ emit digitPressed("6"); });
    connect(ui->pushButton_7,  &QPushButton::clicked, this, [this](){ emit digitPressed("7"); });
    connect(ui->pushButton_8,  &QPushButton::clicked, this, [this](){ emit digitPressed("8"); });
    connect(ui->pushButton_9,  &QPushButton::clicked, this, [this](){ emit digitPressed("9"); });
    connect(ui->pushButton_11,  &QPushButton::clicked, this, [this](){ emit digitPressed("0"); });
    connect(ui->pushButton_10, &QPushButton::clicked, this, [this](){ emit digitPressed("010"); });
    connect(ui->pushButton_12,  &QPushButton::clicked, this, [this](){ emit deletePressed(); });
    // connect(ui->pushButton_15,  &QPushButton::clicked, this, [this](){ emit clearPressed(); });
    connect(ui->pushButton_15, &QPushButton::clicked, this, [this](){ emit confirmPressed(); });
    connect(ui->pushButton_14, &QPushButton::clicked, this, &PointInputView::cancelled);
}

void PointInputView::setDisplay(const QString &maskedText)
{
    ui->lineEdit->setText(maskedText);
}

