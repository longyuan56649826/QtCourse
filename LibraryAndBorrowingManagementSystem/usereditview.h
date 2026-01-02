#ifndef USEREDITVIEW_H
#define USEREDITVIEW_H

#include <QWidget>

namespace Ui {
class UserEditView;
}

class UserEditView : public QWidget
{
    Q_OBJECT

public:
    explicit UserEditView(QWidget *parent = nullptr,int index=0);
    ~UserEditView();

private slots:
    void on_btEdit_clicked();

    void on_btCancel_clicked();

private:
    Ui::UserEditView *ui;

signals:
    void goBack();
};

#endif // USEREDITVIEW_H
