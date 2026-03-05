#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->cart_each = 0;
    this->cart_price = 0;
    this->select_lang = 0;      // 언어 초기설정
    this->setup_db();      // DB 데이터 설정

    // 시작 페이지 추가
    this->page_start = new PageStart(this);
    ui->ui_page_start->layout()->addWidget(page_start);

    ui->stackedWidget->setCurrentIndex(PAGE_START);
}

void MainWindow::setup_db(){
    this->db.setHostName("10.10.20.106");
    this->db.setPort(3306);
    this->db.setUserName("kiosk");
    this->db.setPassword("1234");
    this->db.setDatabaseName("The_Chaos");
}

/////////////////////// 핸들 함수 시작 //////////////////////////////////////
void MainWindow::recv_data(){


    switch(){

        /////////////////// 카테고리 설정//////////////////////////////
    case :

        break;

    case :

        break;

    case :

        break;

        /////////////////// 카테고리 설정 끝 //////////////////////////////

        /////////////////// 메뉴 선택 설정//////////////////////////////
    case :

        break;

    case :

        break;

    case :

        break;

        /////////////////// 메뉴 선택 끝 //////////////////////////////
        /////////////////// 장바구니 설정//////////////////////////////
    case :

        break;

    case :

        break;

    case :

        break;

        /////////////////// 장바구니 설정 끝 //////////////////////////////




    }
}
////////////////////////////// 핸들 함수 끝 //////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}
