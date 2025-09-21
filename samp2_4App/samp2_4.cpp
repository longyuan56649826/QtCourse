#include "samp2_4.h"
#include "ui_samp2_4.h"

samp2_4::samp2_4(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::samp2_4)
{
    ui->setupUi(this);
}

samp2_4::~samp2_4()
{
    delete ui;
}



void samp2_4::on_pushButton_clicked()
{
    close();
}

