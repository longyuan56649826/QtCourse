#include "dataview.h"
#include "ui_dataview.h"

DataView::DataView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DataView)
{
    ui->setupUi(this);
}

DataView::~DataView()
{
    delete ui;
}
