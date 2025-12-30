#include "returnview.h"
#include "ui_returnview.h"

ReturnView::ReturnView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReturnView)
{
    ui->setupUi(this);
}

ReturnView::~ReturnView()
{
    delete ui;
}
