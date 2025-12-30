#include "borrowview.h"
#include "ui_borrowview.h"

BorrowView::BorrowView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowView)
{
    ui->setupUi(this);
}

BorrowView::~BorrowView()
{
    delete ui;
}
