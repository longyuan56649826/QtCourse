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

signals:
    void goBack();
    void goBookEditView(int index);
private slots:
    void on_btBack_clicked();
    void on_btSearch_clicked();
    void on_btBorrow_clicked();
};

#endif // BORROWVIEW_H
