#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>


namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private slots:


    void on_btSignLogin_clicked();

    void on_btSignRegister_clicked();

signals:
    void loginAdminSuccess();
    void loginReaderSuccess();
    void loginFailed();
    void goregisterSuccess();

private:
    Ui::LoginView *ui;
};

#endif // LOGINVIEW_H
