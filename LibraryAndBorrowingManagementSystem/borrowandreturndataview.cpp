#include "borrowandreturndataview.h"
#include "ui_borrowandreturndataview.h"

BorrowAndReturnDataView::BorrowAndReturnDataView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowAndReturnDataView)
{
    ui->setupUi(this);
}

BorrowAndReturnDataView::~BorrowAndReturnDataView()
{
    delete ui;
}
