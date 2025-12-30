#include "readerview.h"
#include "ui_readerview.h"

ReaderView::ReaderView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReaderView)
{
    ui->setupUi(this);
}

ReaderView::~ReaderView()
{
    delete ui;
}
