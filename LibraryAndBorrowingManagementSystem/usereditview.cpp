#include "usereditview.h"
#include "ui_usereditview.h"
#include <QSqlTableModel>
#include "idatabase.h"

UserEditView::UserEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::UserEditView)
{
    ui->setupUi(this);
}

UserEditView::~UserEditView()
{
    delete ui;
}
