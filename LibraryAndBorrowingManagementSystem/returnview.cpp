#include "returnview.h"
#include "ui_returnview.h"
#include "idatabase.h"
#include <QMessageBox>

ReturnView::ReturnView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReturnView)
{

    ui->setupUi(this);

    // 表格基础设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDatabase &iDatabase = IDatabase::getInstance();
    // 初始化“当前用户的未归还借阅模型”
    if (iDatabase.initCurrentUserBorrowModel()) {
        ui->tableView->setModel(iDatabase.BorrowTabModel);
        ui->tableView->setSelectionModel(iDatabase.theBorrowSelection);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        qDebug() << "归还窗口：当前用户借阅记录加载完成";
    } else {
        QMessageBox::critical(this, "错误", "当前用户借阅记录加载失败！");
    }
}

ReturnView::~ReturnView()
{
    delete ui;
}

void ReturnView::on_btSearch_clicked()
{
    IDatabase &iDatabase = IDatabase::getInstance();
    // 获取输入的书本名字（去除首尾空格）
    QString bookName = ui->txtSearch->text().trimmed();

    // 执行搜索：按书本名筛选当前用户未归还的借阅记录
    bool success = iDatabase.searchCurrentUserBorrowByBookName(bookName);

    // 反馈搜索结果
    if (success) {
        if (bookName.isEmpty()) {
            QMessageBox::information(this, "提示", "已加载当前用户所有未归还借阅记录");
        } else {
            int recordCount = iDatabase.BorrowTabModel->rowCount();
            if (recordCount == 0) {
                QMessageBox::information(this, "提示", QString("未找到包含「%1」的未归还借阅记录").arg(bookName));
            } else {
                QMessageBox::information(this, "提示", QString("找到%1条包含「%2」的未归还借阅记录").arg(recordCount).arg(bookName));
            }
        }
        // 更新表格显示
        ui->tableView->setModel(iDatabase.BorrowTabModel);
    } else {
        QMessageBox::warning(this, "提示", "搜索失败，请检查输入或数据库连接");
    }
}


void ReturnView::on_btReturn_clicked()
{
    IDatabase &iDatabase = IDatabase::getInstance();
    // 获取选中行的借阅编号
    QModelIndex curIndex = iDatabase.theBorrowSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "提示", "请选择要归还的借阅记录！");
        return;
    }

    // 获取借阅编号（第0列）
    QString borrowNo = iDatabase.BorrowTabModel->data(iDatabase.BorrowTabModel->index(curIndex.row(), 0)).toString();
    if (borrowNo.isEmpty()) {
        QMessageBox::warning(this, "提示", "选中记录的借阅编号无效！");
        return;
    }

    // 执行归还操作
    if (iDatabase.returnBook(borrowNo)) {
        QMessageBox::information(this, "成功", "书籍归还成功！");
        // 重新加载当前用户的未归还记录（移除已归还的行）
        iDatabase.initCurrentUserBorrowModel();
        ui->tableView->setModel(iDatabase.BorrowTabModel);
    } else {
        QMessageBox::critical(this, "失败", "书籍归还失败！");
    }
}


void ReturnView::on_btBack_clicked()
{
    emit goBack();
}

