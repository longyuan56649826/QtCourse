#ifndef BORROWANDRETURNDATAVIEW_H
#define BORROWANDRETURNDATAVIEW_H

#include <QWidget>

namespace Ui {
class BorrowAndReturnDataView;
}

class BorrowAndReturnDataView : public QWidget
{
    Q_OBJECT

public:
    explicit BorrowAndReturnDataView(QWidget *parent = nullptr);
    ~BorrowAndReturnDataView();

private:
    Ui::BorrowAndReturnDataView *ui;
};

#endif // BORROWANDRETURNDATAVIEW_H
