#ifndef POINTINPUTVIEW_H
#define POINTINPUTVIEW_H

#include <QWidget>

namespace Ui {
class PointInputView;
}

class PointInputView : public QWidget
{
    Q_OBJECT

public:
    explicit PointInputView(QWidget *parent = nullptr);
    ~PointInputView();

private:
    Ui::PointInputView *ui;
};

#endif // POINTINPUTVIEW_H
