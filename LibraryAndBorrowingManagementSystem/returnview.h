#ifndef RETURNVIEW_H
#define RETURNVIEW_H

#include <QWidget>

namespace Ui {
class ReturnView;
}

class ReturnView : public QWidget
{
    Q_OBJECT

public:
    explicit ReturnView(QWidget *parent = nullptr);
    ~ReturnView();

private slots:
    void on_btSearch_clicked();

    void on_btReturn_clicked();

    void on_btBack_clicked();

private:
    Ui::ReturnView *ui;

signals:
    void goBack();
};

#endif // RETURNVIEW_H
