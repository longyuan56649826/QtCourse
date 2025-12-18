#include "patienteditview.h"
#include "ui_patienteditview.h"
#include <QSqlTableModel>
#include "idatabase.h"

PatientEditView::PatientEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
{
    ui->setupUi(this);

    dataMapper=new QDataWidgetMapper();
    QSqlTableModel *tableModel=IDatabase::getInstance().patientTabModel;
    dataMapper->setModel(IDatabase::getInstance().patientTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditID,tableModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName,tableModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDCard,tableModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbEditHeight,tableModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbEditWeight,tableModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbEditMobilephone,tableModel->fieldIndex("MOBLEPHONE"));
    dataMapper->addMapping(ui->dbEditDOB,tableModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dbEditSex,tableModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbEditCratedTimestamp,tableModel->fieldIndex("CREATEDTIMESTAMP"));

    dataMapper->setCurrentIndex(index);

}

PatientEditView::~PatientEditView()
{
    delete ui;
}

void PatientEditView::on_pushButton_clicked()
{
    IDatabase::getInstance().submitPatientEdit();

    emit goPreviousView();
}


void PatientEditView::on_pushButton_2_clicked()
{
    IDatabase::getInstance().revertPatientEdit();

    emit goPreviousView();
}

