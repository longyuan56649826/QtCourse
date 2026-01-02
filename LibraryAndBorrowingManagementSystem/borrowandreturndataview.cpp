#include "borrowandreturndataview.h"
#include "ui_borrowandreturndataview.h"
#include "idatabase.h"
#include <QMessageBox>

BorrowAndReturnDataView::BorrowAndReturnDataView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowAndReturnDataView)
{
    ui->setupUi(this);

    // 表格基础设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    // 初始化“借出书籍聚合模型”（合并相同BookNo的借出数量）
    IDatabase &iDatabase = IDatabase::getInstance();
    if (iDatabase.initBorrowBookAggregateModel()) {
        ui->tableView->setModel(iDatabase.BorrowTabModel);
        ui->tableView->setSelectionModel(iDatabase.theBorrowSelection);

        // 列宽自适应
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        qDebug() << "借阅归还数据窗口：借出书籍聚合数据加载完成";

        // 反馈数据量
        int rowCount = iDatabase.BorrowTabModel->rowCount();
        if (rowCount == 0) {
            QMessageBox::information(this, "提示", "当前暂无未归还的借出书籍记录");
        } else {
            QMessageBox::information(this, "提示", QString("已加载%1种未归还的借出书籍").arg(rowCount));
        }
    } else {
        QMessageBox::critical(this, "错误", "借出书籍聚合数据加载失败！");
    }
}

BorrowAndReturnDataView::~BorrowAndReturnDataView()
{
    delete ui;
}

void BorrowAndReturnDataView::on_btSearch_clicked()
{
    IDatabase &iDatabase = IDatabase::getInstance();

    // 1. 获取输入的书本名（去除首尾空格）
    QString bookName = ui->txtSearch->text().trimmed();

    // 2. 执行搜索：按书本名筛选聚合数据
    bool success = iDatabase.searchBorrowBookAggregateByBookName(bookName);

    // 3. 更新表格显示
    if (success) {
        ui->tableView->setModel(iDatabase.BorrowTabModel);

        // 反馈搜索结果
        int recordCount = iDatabase.BorrowTabModel->rowCount();
        if (bookName.isEmpty()) {
            QMessageBox::information(this, "提示", "已加载所有未归还的借出书籍");
        } else if (recordCount == 0) {
            QMessageBox::information(this, "提示", QString("未找到包含「%1」的未归还借出书籍").arg(bookName));
        } else {
            QMessageBox::information(this, "提示", QString("找到%1种包含「%2」的未归还借出书籍").arg(recordCount).arg(bookName));
        }
    } else {
        QMessageBox::warning(this, "提示", "搜索失败，请检查输入或数据库连接");
    }
}


void BorrowAndReturnDataView::on_btBack_clicked()
{
    emit goBack();
}

