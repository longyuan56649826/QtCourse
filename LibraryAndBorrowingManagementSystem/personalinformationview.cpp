#include "personalinformationview.h"
#include "ui_personalinformationview.h"

PersonalInformationView::PersonalInformationView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonalInformationView)
{
    ui->setupUi(this);
}

PersonalInformationView::~PersonalInformationView()
{
    delete ui;
}
