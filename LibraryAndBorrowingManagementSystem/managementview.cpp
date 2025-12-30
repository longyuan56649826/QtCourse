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
