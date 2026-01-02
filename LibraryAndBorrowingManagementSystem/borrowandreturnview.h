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

private slots:
    void on_btSearch_clicked();

    void on_btBack_clicked();

private:
    Ui::BorrowAndReturnView *ui;

signals:
    void goBack();
    void goBookEditView(int index);

};

#endif // BORROWANDRETURNVIEW_H
