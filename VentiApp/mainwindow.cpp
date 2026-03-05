#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug> // 디버깅용 로그 출력

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // mainwindow.ui에서 파일 선택후 경로 설정
    QString imagePath = ":/g-dragon/벤티홍보.jpg";

    // 버튼의 배경으로 이미지 넣기 (CSS 이용)
    ui->introButton->setStyleSheet(QString("border-image: url(%1); border: none;").arg(imagePath));

    // 프로그램 시작 시 무조건 0번 페이지(홍보 모델)부터 보여줌
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 홍보 모델 이미지 버튼 클릭 시
void MainWindow::on_introButton_clicked()
{
    // 0번 페이지(초기화면)에서 1번 페이지(선택화면)로 전환
    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << "초기화면 -> 선택화면 이동 완료";
}

// 매장 버튼 클릭 시
void MainWindow::on_storeButton_clicked()
{
    // 1번 페이지(선택화면)에서 2번 페이지(메뉴화면)로 전환
    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "매장 선택 -> 메뉴판 이동 완료";
}

// 포장 버튼 클릭 시]
void MainWindow::on_takeoutButton_clicked()
{
    // 2번 페이지(메뉴화면)로 이동
    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "포장 선택 -> 메뉴판 이동 완료";
}
