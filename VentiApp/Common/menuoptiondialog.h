#ifndef MENUOPTIONDIALOG_H
#define MENUOPTIONDIALOG_H

#include <QDialog> // QDialog 클래스를 상속받기 위해 필수

namespace Ui {
class MenuOptionDialog;
}

class MenuOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MenuOptionDialog(QWidget *parent = nullptr);
    ~MenuOptionDialog();

private slots:
    void onConfirmClicked(); // 확인 버튼 슬롯
    void onCancelClicked();  // 취소 버튼 슬롯

private:
    Ui::MenuOptionDialog *ui;
};

#endif // MENUOPTIONDIALOG_H
