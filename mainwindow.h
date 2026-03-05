#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum addr_page {  };   // 페이지 구분용

public:
    MainWindow(QWidget *parent = nullptr);

    // 메서드 선언
    void setup_db();
    ~MainWindow();



private:   //기능 추가 함수들...
    Ui::MainWindow *ui;


private slots:  // 슬롯, 데이터 수신
    void recv_data(addr_page addr, QString title, QStringList data);
};
#endif // MAINWINDOW_H
