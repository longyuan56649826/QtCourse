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

private slots:
    void on_btSearch_clicked();

    void on_btDelete_clicked();

    void on_btEdit_clicked();

private:
    Ui::ReaderView *ui;

signals:
    void goUserEditView(int index);
};

#endif // READERVIEW_H
