#include "registerview.h"
#include "ui_registerview.h"
#include "idatabase.h"

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
}

