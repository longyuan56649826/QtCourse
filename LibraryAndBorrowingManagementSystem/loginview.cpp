#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"

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

    if(status=="loginOK")
        emit loginSuccess();
}


void LoginView::on_btSignRegister_clicked()
{
    emit goregisterSuccess();
}

