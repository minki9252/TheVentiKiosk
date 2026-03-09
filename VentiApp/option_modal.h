#ifndef OPTION_MODAL_H
#define OPTION_MODAL_H

#include <QDialog>

namespace Ui {
class option_modal;
}

class option_modal : public QDialog
{
    Q_OBJECT

public:
    explicit option_modal(QWidget *parent = nullptr);
    ~option_modal();

private:
    Ui::option_modal *ui;
};

#endif // OPTION_MODAL_H
