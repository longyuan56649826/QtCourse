#include "readerview.h"
#include "ui_readerview.h"
#include "idatabase.h"

ReaderView::ReaderView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReaderView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDatabase &iDatabase=IDatabase::getInstance();
    if(iDatabase.initUserModel()){
        ui->tableView->setModel(iDatabase.UserTabModel);
        ui->tableView->setSelectionModel(iDatabase.theUserSelection);

        // ========== 核心修改：设置表格中文表头 ==========
        QSqlTableModel *model = iDatabase.UserTabModel;
        // 1. 根据数据库字段名获取列索引，设置对应中文表头
        // 请根据你的实际数据库字段名和需要显示的中文调整！
        model->setHeaderData(model->fieldIndex("UserNo"), Qt::Horizontal, "用户编号");
        model->setHeaderData(model->fieldIndex("UserAccountName"), Qt::Horizontal, "账号");
        model->setHeaderData(model->fieldIndex("UserPassword"), Qt::Horizontal, "密码");
        model->setHeaderData(model->fieldIndex("UserName"), Qt::Horizontal, "姓名");
        model->setHeaderData(model->fieldIndex("UserId"), Qt::Horizontal, "身份证号");
        model->setHeaderData(model->fieldIndex("UserIdentity"), Qt::Horizontal, "用户身份");

        // 2. 可选：调整列宽自适应（避免中文表头显示不全）
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // 或指定某列自适应：ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

        qDebug() << "ReaderView：中文表头设置完成";
    }
}

ReaderView::~ReaderView()
{
    delete ui;
}


void ReaderView::on_btSearch_clicked()
{
    QString filter=QString("UserName like '%%1%'").arg(ui->txtSearch->text());
    IDatabase::getInstance().searchUser(filter);
}


void ReaderView::on_btDelete_clicked()
{
    IDatabase::getInstance().deleteCurrentUser();
}


void ReaderView::on_btEdit_clicked()
{
    QModelIndex curIndex=IDatabase::getInstance().theUserSelection->currentIndex();
    emit goUserEditView(curIndex.row());
}

