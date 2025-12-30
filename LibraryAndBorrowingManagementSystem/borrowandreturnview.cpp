#include "borrowandreturnview.h"
#include "ui_borrowandreturnview.h"

BorrowAndReturnView::BorrowAndReturnView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowAndReturnView)
{
    ui->setupUi(this);
}

BorrowAndReturnView::~BorrowAndReturnView()
{
    delete ui;
}
