#ifndef READERBORROWANDRETURNVIEW_H
#define READERBORROWANDRETURNVIEW_H

#include <QWidget>

namespace Ui {
class ReaderBorrowAndReturnView;
}

class ReaderBorrowAndReturnView : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderBorrowAndReturnView(QWidget *parent = nullptr);
    ~ReaderBorrowAndReturnView();

private slots:
    void on_btBorrow_clicked();

    void on_btReturn_clicked();

    void on_btPersonalInformation_clicked();

private:
    Ui::ReaderBorrowAndReturnView *ui;

signals:
    void goReturn();
    void goBorrow();
    void goPersonalInformation();
};

#endif // READERBORROWANDRETURNVIEW_H
