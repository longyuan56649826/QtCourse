#include "borrowandreturnview.h"
#include "ui_borrowandreturnview.h"
#include "idatabase.h"
#include <QMessageBox>

BorrowAndReturnView::BorrowAndReturnView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowAndReturnView)
{
    ui->setupUi(this);

    // 表格基础设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    // ========== 1. 初始化下拉框（SearchWay） ==========
    ui->SearchWay->clear();
    // 添加选项：显示文本 + 关联的数据库字段（用于生成过滤条件）
    ui->SearchWay->addItem("用户名字", "u.UserName");       // 关联User表的用户名
    ui->SearchWay->addItem("账号名字", "u.UserAccountName");// 关联User表的账号名
    ui->SearchWay->addItem("书本名字", "bo.BookName");      // 关联Book表的书本名
    // 默认选中第一个选项
    ui->SearchWay->setCurrentIndex(0);

    // ========== 2. 初始化借阅模型（多表关联） ==========
    IDatabase &iDatabase = IDatabase::getInstance();
    if (iDatabase.initBorrowModel()) {
        ui->tableView->setModel(iDatabase.BorrowTabModel);
        ui->tableView->setSelectionModel(iDatabase.theBorrowSelection);

        // 列宽自适应
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        qDebug() << "借阅归还窗口：表格初始化完成";
    } else {
        QMessageBox::critical(this, "错误", "借阅模型初始化失败！");
    }
}

BorrowAndReturnView::~BorrowAndReturnView()
{
    delete ui;
}


void BorrowAndReturnView::on_btSearch_clicked()
{
    IDatabase &iDatabase = IDatabase::getInstance();

    // 1. 获取搜索条件
    QString searchText = ui->txtSearch->text().trimmed();
    QString searchField = ui->SearchWay->currentData().toString(); // 获取绑定的数据库字段

    // 2. 生成过滤条件（模糊查询）
    QString filter;
    if (!searchText.isEmpty()) {
        // 模糊查询：字段 LIKE '%关键词%'
        filter = QString("%1 LIKE '%%2%'").arg(searchField).arg(searchText);
    } else {
        // 无输入时查询所有
        filter = "";
    }

    // 3. 执行搜索并反馈结果
    bool success = iDatabase.searchBorrow(filter);
    if (success && searchText.isEmpty()) {
        QMessageBox::information(this, "提示", "已加载所有借阅记录");
    } else if (success) {
        QMessageBox::information(this, "提示", QString("已搜索到包含「%1」的借阅记录").arg(searchText));
    } else {
        QMessageBox::warning(this, "提示", "搜索失败，请检查输入或数据库连接");
    }
}


void BorrowAndReturnView::on_btBack_clicked()
{
    emit goBack();
}

