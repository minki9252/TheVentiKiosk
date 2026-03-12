#ifndef COUPONINPUTDIALOG_H
#define COUPONINPUTDIALOG_H

#include <QDialog>
#include <QPushButton> // QPushButton 사용을 위해 추가

namespace Ui { class CouponInputDialog; }

class CouponInputDialog : public QDialog {
    Q_OBJECT
public:
    explicit CouponInputDialog(QWidget *parent = nullptr);
    ~CouponInputDialog();

private slots:
    void onKeyClicked();       // 숫자 키패드 클릭 시
    void on_btnConfirm_clicked(); // 확인 버튼

private:
    Ui::CouponInputDialog *ui;
    QString currentInput;      // 입력된 숫자를 저장할 변수
};

#endif
