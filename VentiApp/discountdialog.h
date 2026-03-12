#ifndef DISCOUNTDIALOG_H
#define DISCOUNTDIALOG_H

#include <QDialog>

namespace Ui { class DiscountDialog; }

class DiscountDialog : public QDialog {
    Q_OBJECT
public:
    explicit DiscountDialog(QWidget *parent = nullptr);
    ~DiscountDialog();
    int getSelectedType() const { return selectedType; } // 1: App, 2: 모바일

private slots:
    void on_btnAppCoupon_clicked();
    void on_btnMobileCoupon_clicked();
    void on_btnCancel_clicked();

private:
    Ui::DiscountDialog *ui;
    int selectedType = 0;
};

#endif
