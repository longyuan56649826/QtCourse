#ifndef READERVIEW_H
#define READERVIEW_H

#include <QWidget>

namespace Ui {
class ReaderView;
}

class ReaderView : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderView(QWidget *parent = nullptr);
    ~ReaderView();

private:
    Ui::ReaderView *ui;
};

#endif // READERVIEW_H
