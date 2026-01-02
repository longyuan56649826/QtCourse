#include "bookeditview.h"
#include "ui_bookeditview.h"
#include "idatabase.h"
#include <QSqlTableModel>

BookEditView::BookEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::BookEditView)
{
    ui->setupUi(this);

    dataMapper=new QDataWidgetMapper();
    QSqlTableModel *tableModel=IDatabase::getInstance().BookTabModel;
    dataMapper->setModel(IDatabase::getInstance().BookTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);


    dataMapper->addMapping(ui->InputBookName,tableModel->fieldIndex("BookName"));
    dataMapper->addMapping(ui->InputStock,tableModel->fieldIndex("Stock"));


    dataMapper->setCurrentIndex(index);
}

BookEditView::~BookEditView()
{
    delete ui;
}

void BookEditView::on_btEdit_clicked()
{
    IDatabase::getInstance().submitBookEdit();
    emit goBack();
}


void BookEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertBookEdit();

    emit goBack();
}

