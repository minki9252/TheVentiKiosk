#include "categorywidget.h"
#include "ui_categorywidget.h"
#include "mainwindow.h"

categorywidget::categorywidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::categorywidget)
{
    ui->setupUi(this);

    // 버튼을 누르면, 각각 알맞은 KioskEvent 액션 코드를 실어서 하나의 시그널만 쏜다
    connect(ui->category_new_button, &QPushButton::clicked, this, [this]()
            { emit categorySelected(CATEGORY_NEW_MENU); });

    connect(ui->category_icedcoffee_button, &QPushButton::clicked, this, [this]()
            { emit categorySelected(CATEGORY_ICED_COFFEE); });

    // (나머지 카테고리 버튼들도 여기에 쭉 추가하기.3/6)
}

categorywidget::~categorywidget()
{
    delete ui;
}
