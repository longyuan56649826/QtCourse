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

private:
    Ui::ReaderBorrowAndReturnView *ui;
};

#endif // READERBORROWANDRETURNVIEW_H
