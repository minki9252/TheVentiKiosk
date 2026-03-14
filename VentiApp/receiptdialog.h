#ifndef RECEIPTDIALOG_H
#define RECEIPTDIALOG_H

#include <QDialog>

class ReceiptDialog : public QDialog {
    Q_OBJECT
public:
    // 메뉴 내역(QString)을 추가로 받도록 수정
    explicit ReceiptDialog(int orderNum, const QString &menuDetails, QWidget *parent = nullptr);
};

#endif
