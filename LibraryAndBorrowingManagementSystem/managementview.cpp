#include "managementview.h"
#include "ui_managementview.h"

ManagementView::ManagementView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ManagementView)
{
    ui->setupUi(this);
}

ManagementView::~ManagementView()
{
    delete ui;
}

void ManagementView::on_btReader_clicked()
{
    emit goReader();

}


void ManagementView::on_btBook_clicked()
{
    emit goBook();
}


void ManagementView::on_btBorrowAndReturn_clicked()
{
    emit goBorrowAndReturn();

}


void ManagementView::on_btBorrowAndReturndata_clicked()
{
    emit goBorrowAndReturndata();
}


void ManagementView::on_btBecomeReader_clicked()
{
    emit goBecomeReader();
}

