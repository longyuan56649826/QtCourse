#include "readerborrowandreturnview.h"
#include "ui_readerborrowandreturnview.h"

ReaderBorrowAndReturnView::ReaderBorrowAndReturnView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReaderBorrowAndReturnView)
{
    ui->setupUi(this);
}

ReaderBorrowAndReturnView::~ReaderBorrowAndReturnView()
{
    delete ui;
}

void ReaderBorrowAndReturnView::on_btBorrow_clicked()
{
    emit goBorrow();
}


void ReaderBorrowAndReturnView::on_btReturn_clicked()
{
    emit goReturn();
}


void ReaderBorrowAndReturnView::on_btPersonalInformation_clicked()
{
    emit goPersonalInformation();
}

