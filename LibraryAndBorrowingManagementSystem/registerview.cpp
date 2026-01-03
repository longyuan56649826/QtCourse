#include "registerview.h"
#include "ui_registerview.h"
#include "idatabase.h"
#include <QMessageBox>

RegisterView::RegisterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterView)
{
    ui->setupUi(this);
}

RegisterView::~RegisterView()
{
    delete ui;
}

void RegisterView::on_btReturnLogin_clicked()
{
    emit gologinSuccess();
}


void RegisterView::on_btRegister_clicked()
{
    QString status=IDatabase::getInstance().userRegister(ui->InputUserName->text(),ui->InputUserPassword->text(),ui->InputUserPasswordAgain->text(),ui->InputName->text(),ui->InputId->text(),ui->dbeditIdentity->currentText());

    if(status=="registerOK")
    emit registerSuccess();
    else if(status=="emptyField")
        QMessageBox::critical(this, "注册失败", "所有信息必须填写，不能为空！", QMessageBox::Ok);
    else if(status=="passwordNotMatch")
        QMessageBox::critical(this, "注册失败", "两次输入的密码不一致！", QMessageBox::Ok);
    else if(status=="dbQueryError")
        QMessageBox::critical(this, "注册失败", "注册失败：数据库查询出错！", QMessageBox::Ok);
    else if(status=="accountOrIdExisted")
        QMessageBox::critical(this, "注册失败", "注册失败：账号名或ID已存在！", QMessageBox::Ok);

}

