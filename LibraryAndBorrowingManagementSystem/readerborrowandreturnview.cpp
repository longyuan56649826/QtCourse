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
