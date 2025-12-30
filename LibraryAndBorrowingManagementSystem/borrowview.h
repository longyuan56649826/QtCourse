#ifndef BORROWVIEW_H
#define BORROWVIEW_H

#include <QWidget>

namespace Ui {
class BorrowView;
}

class BorrowView : public QWidget
{
    Q_OBJECT

public:
    explicit BorrowView(QWidget *parent = nullptr);
    ~BorrowView();

private:
    Ui::BorrowView *ui;
};

#endif // BORROWVIEW_H
