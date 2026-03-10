#ifndef MENUOPTIONDIALOG_H
#define MENUOPTIONDIALOG_H

#include <QDialog>

namespace Ui { class MenuOptionDialog; }

struct SelectedOption {
    QString name;
    int basePrice;
    QString tempOption; // "HOT" 또는 "ICE"
    int extraPrice = 0;
    QStringList details; // "샷 추가" 등
};

class MenuOptionDialog : public QDialog {
    Q_OBJECT
public:
    explicit MenuOptionDialog(QString name, int price, QWidget *parent = nullptr);
    ~MenuOptionDialog();
    SelectedOption getSelectedOption();

private slots:
    void on_btnAddToOrder_clicked();

private:
    Ui::MenuOptionDialog *ui;
    SelectedOption result;
};

#endif // MENUOPTIONDIALOG_H
