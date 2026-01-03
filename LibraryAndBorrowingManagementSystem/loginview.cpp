#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"
#include <QMessageBox>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}



void LoginView::on_btSignLogin_clicked()
{
    QString status=IDatabase::getInstance().userLogin(ui->InputUserName->text(),ui->InputUserPassword->text());



    if(status=="AdminloginOK")
        emit loginAdminSuccess();
    else if(status=="ReaderloginOK")
        emit loginReaderSuccess();
    else if(status=="wrongPassword")
         QMessageBox::critical(this, "登录失败", "密码错误，请重新输入！", QMessageBox::Ok);
    else if(status=="wrongUserAccountname")
        QMessageBox::critical(this, "登录失败", "未找到该账号，请重新输入！", QMessageBox::Ok);
}


void LoginView::on_btSignRegister_clicked()
{
    emit goregisterSuccess();
}

