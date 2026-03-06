#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QSqlDatabase> 
#include "kioskevent.h"
#include "cartmanager.h"
#include "cart.h" // 🌟 새롭게 만든 Cart 클래스 포함

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_introButton_clicked();   
    void on_storeButton_clicked();   
    void on_takeoutButton_clicked(); 

private:
    Ui::MainWindow *ui;
    
    // 🌟 장바구니 데이터를 관리할 전담 객체
    Cart m_cart; 
    CartManager m_cartManager;
    QSqlDatabase db;
    void setup_db();
    void setup_connections();
    void handle(const KioskEvent &event);
    void refreshCartUI(); // UI만 담당하는 함수

    QSqlDatabase db;

};

#endif // MAINWINDOW_H
