#include "personalinformationview.h"
#include "ui_personalinformationview.h"
#include "idatabase.h"

PersonalInformationView::PersonalInformationView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonalInformationView)
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


        // 获取当前登录账号
        QString currentAccount = iDatabase.getCurrentUserAccount();
        if (!currentAccount.isEmpty()) {
            // 设置筛选条件：只显示当前账号的记录（字段名与数据库保持一致）
            model->setFilter(QString("UserAccountName = '%1'").arg(currentAccount));
            model->select(); // 应用筛选
        } else {
            qDebug() << "未获取到当前登录账号";
        }
        // 2. 可选：调整列宽自适应（避免中文表头显示不全）
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // 或指定某列自适应：ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

        qDebug() << "ReaderView：中文表头设置完成";
    }
}

PersonalInformationView::~PersonalInformationView()
{
    delete ui;
}

void PersonalInformationView::on_btBack_clicked()
{
    emit goBack();
}


void PersonalInformationView::on_btEdit_clicked()
{
    QModelIndex curIndex=IDatabase::getInstance().theUserSelection->currentIndex();
    emit goUserEditView(curIndex.row());

}

