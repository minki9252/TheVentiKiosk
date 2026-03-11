#ifndef POINTRESULTVIEW_H
#define POINTRESULTVIEW_H

#include <QWidget>

namespace Ui {
class PointResultView;
}

class PointResultView : public QWidget
{
    Q_OBJECT

public:
    explicit PointResultView(QWidget *parent = nullptr);
    ~PointResultView();

private:
    Ui::PointResultView *ui;
};

#endif // POINTRESULTVIEW_H
