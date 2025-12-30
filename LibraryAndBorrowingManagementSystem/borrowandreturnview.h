#ifndef BORROWANDRETURNVIEW_H
#define BORROWANDRETURNVIEW_H

#include <QWidget>

namespace Ui {
class BorrowAndReturnView;
}

class BorrowAndReturnView : public QWidget
{
    Q_OBJECT

public:
    explicit BorrowAndReturnView(QWidget *parent = nullptr);
    ~BorrowAndReturnView();

private:
    Ui::BorrowAndReturnView *ui;
};

#endif // BORROWANDRETURNVIEW_H
