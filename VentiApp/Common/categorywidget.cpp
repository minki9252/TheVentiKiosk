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
            { emit categorySelected(CATEGORY_COFFEE); });

    connect(ui->category_tea_button, &QPushButton::clicked, this, [this]()
            { emit categorySelected(CATEGORY_TEA); });

    connect(ui->category_blended_button, &QPushButton::clicked, this, [this]()
            { emit categorySelected(CATEGORY_BLENDED); });

    connect(ui->category_beverage_button, &QPushButton::clicked, this, [this]()
            { emit categorySelected(CATEGORY_BEVERAGE); });

    connect(ui->category_ade_button, &QPushButton::clicked, this, [this]()
            { emit categorySelected(CATEGORY_ADE); });

    connect(ui->category_hotcoffee_button, &QPushButton::clicked, this, [this]()
            { emit categorySelected(CATEGORY_DESERT); });

    connect(ui->home_button, &QPushButton::clicked, this, [this]()
            { emit categorySelected(GO_HOME); });
}

categorywidget::~categorywidget()
{
    delete ui;
}
