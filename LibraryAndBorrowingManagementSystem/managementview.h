#ifndef MANAGEMENTVIEW_H
#define MANAGEMENTVIEW_H

#include <QWidget>

namespace Ui {
class ManagementView;
}

class ManagementView : public QWidget
{
    Q_OBJECT

public:
    explicit ManagementView(QWidget *parent = nullptr);
    ~ManagementView();

private slots:
    void on_btReader_clicked();

    void on_btBook_clicked();

    void on_btBorrowAndReturn_clicked();

    void on_btBorrowAndReturndata_clicked();

    void on_btBecomeReader_clicked();

signals:
    void goReader();
    void goBook();
    void goBorrowAndReturn();
    void goBorrowAndReturndata();
    void goBecomeReader();
private:
    Ui::ManagementView *ui;
};

#endif // MANAGEMENTVIEW_H
