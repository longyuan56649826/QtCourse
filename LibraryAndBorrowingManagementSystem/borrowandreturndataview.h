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

private slots:
    void on_btSearch_clicked();

    void on_btBack_clicked();

private:
    Ui::BorrowAndReturnDataView *ui;

signals:
    void goBack();
};

#endif // BORROWANDRETURNDATAVIEW_H
