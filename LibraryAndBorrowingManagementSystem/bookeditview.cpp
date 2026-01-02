#include "bookeditview.h"
#include "ui_bookeditview.h"
#include "idatabase.h"
#include <QSqlTableModel>

BookEditView::BookEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::BookEditView)
    , dataMapper(new QDataWidgetMapper(this)) // 初始化时指定父对象，避免内存泄漏
{
    ui->setupUi(this);

    // 获取模型并检查有效性
    QSqlTableModel *tableModel = IDatabase::getInstance().BookTabModel;
    if (!tableModel) {
        qDebug() << "图书模型获取失败！";
        return;
    }

    // 检查索引有效性（防止越界）
    if (index < 0 || index >= tableModel->rowCount()) {
        qDebug() << "无效的图书索引:" << index;
        return;
    }

    // 绑定模型和映射器
    dataMapper->setModel(tableModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit); // 自动提交修改

    // 绑定控件与字段（根据实际UI控件和数据库字段名调整）
    dataMapper->addMapping(ui->InputBookName, tableModel->fieldIndex("BookName"));
    dataMapper->addMapping(ui->InputStock, tableModel->fieldIndex("Stock"));
    // 补充其他字段映射，例如：
    // dataMapper->addMapping(ui->InputAuthor, tableModel->fieldIndex("Author"));
    // dataMapper->addMapping(ui->InputPublisher, tableModel->fieldIndex("Publisher"));

    // 定位到指定行
    dataMapper->setCurrentIndex(index);

    dataMapper=new QDataWidgetMapper();
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

