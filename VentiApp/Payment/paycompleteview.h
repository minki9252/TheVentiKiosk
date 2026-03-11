#ifndef PAYCOMPLETEVIEW_H
#define PAYCOMPLETEVIEW_H

#include <QWidget>

namespace Ui {
class PayCompleteView;
}

class PayCompleteView : public QWidget
{
    Q_OBJECT

public:
    explicit PayCompleteView(QWidget *parent = nullptr);
    ~PayCompleteView();

private:
    Ui::PayCompleteView *ui;
};

#endif // PAYCOMPLETEVIEW_H
