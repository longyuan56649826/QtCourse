#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include <QWidget>

namespace Ui {
class RegisterView;
}

class RegisterView : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterView(QWidget *parent = nullptr);
    ~RegisterView();

private slots:
    void on_btReturnLogin_clicked();

    void on_btRegister_clicked();

private:
    Ui::RegisterView *ui;

signals:
    void loginSuccess();
    void registerFailed();
    void registerSuccess();
    void gologinSuccess();
};

#endif // REGISTERVIEW_H
