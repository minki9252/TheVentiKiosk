#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:  //슬롯 설정
    void on_introButton_clicked();    // 페이지 0 -> 1 이동
    void on_storeButton_clicked();    // 페이지 1 -> 2 이동 (매장 선택)
    void on_takeoutButton_clicked();  // 페이지 1 -> 2 이동 (포장 선택)

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
