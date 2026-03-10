#ifndef COUPONDIALOG_H
#define COUPONDIALOG_H

#include <QDialog>

namespace Ui {
class CouponDialog;
}

class CouponDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CouponDialog(QWidget *parent = nullptr);
    ~CouponDialog();

private slots:
    // 페이지 이동 버튼들
    void on_btnUseCoupon_clicked();
    void on_btnSkipCoupon_clicked();
    void on_btnCancelCoupon_clicked();
    void on_btnConfirmCoupon_clicked();
    void on_btnResultOk_clicked();

    // 키패드 입력 슬롯
    void onNumButtonClicked();

private:
    Ui::CouponDialog *ui;
};

#endif // COUPONDIALOG_H
