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

    void on_btBack_clicked();

private:
    Ui::ReaderView *ui;

signals:
    void goUserEditView(int index);
    void goBack();
};

#endif // READERVIEW_H
