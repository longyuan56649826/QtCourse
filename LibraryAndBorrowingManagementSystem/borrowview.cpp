#include "borrowview.h"
#include "ui_borrowview.h"
#include "idatabase.h"
#include <QMessageBox>

BorrowView::BorrowView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDatabase &iDatabase=IDatabase::getInstance();
    if(iDatabase.initBookModel()){
        ui->tableView->setModel(iDatabase.BookTabModel);
        ui->tableView->setSelectionModel(iDatabase.theBookSelection);

        // ========== 核心修改：设置表格中文表头 ==========
        QSqlTableModel *model = iDatabase.BookTabModel;
        // 1. 根据数据库字段名获取列索引，设置对应中文表头
        // 请根据你的实际数据库字段名和需要显示的中文调整！
        model->setHeaderData(model->fieldIndex("BookNo"), Qt::Horizontal, "书本编号");
        model->setHeaderData(model->fieldIndex("BookName"), Qt::Horizontal, "书本名字");
        model->setHeaderData(model->fieldIndex("Stock"), Qt::Horizontal, "库存");

        // 2. 可选：调整列宽自适应（避免中文表头显示不全）
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // 或指定某列自适应：ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

        qDebug() << "ReaderView：中文表头设置完成";
    }
}

BorrowView::~BorrowView()
{
    delete ui;
}

void BorrowView::on_btBack_clicked()
{
    emit goBack();
}

void BorrowView::on_btSearch_clicked()
{
    QString filter=QString("BookName like '%%1%'").arg(ui->txtSearch->text());
    IDatabase::getInstance().searchBook(filter);
}

void BorrowView::on_btBorrow_clicked()
{
    // 直接复用已定义逻辑的数据库实例
    IDatabase &iDatabase=IDatabase::getInstance();

    // 1. 检查是否选中书籍
    QModelIndex curIndex = iDatabase.theBookSelection->currentIndex();
    if (!curIndex.isValid()) {
        QMessageBox::warning(this, "提示", "请先选择要借阅的书籍！");
        return;
    }

    // 2. 获取选中书籍的BookNo
    QSqlTableModel* bookModel = iDatabase.BookTabModel;
    QString bookNo = bookModel->data(bookModel->index(curIndex.row(), bookModel->fieldIndex("BookNo"))).toString();
    if (bookNo.isEmpty()) {
        QMessageBox::warning(this, "提示", "选中书籍的编号无效！");
        return;
    }

    // 3. 校验借阅数量（必须是正整数）
    bool ok;
    int borrowNum = ui->InputNum->text().toInt(&ok);
    if (!ok || borrowNum <= 0) {
        QMessageBox::warning(this, "提示", "请输入有效的借阅数量（正整数）！");
        return;
    }

    // 4. 获取当前用户的UserNo
    // 先检查是否登录
    if (iDatabase.getCurrentUserAccount().isEmpty()) {
        QMessageBox::warning(this, "提示", "请先登录！");
        return;
    }
    // 查询当前用户的UserNo
    QString userNo;
    QSqlQuery query;
    query.prepare("SELECT UserNo FROM User WHERE UserAccountName=:account");
    query.bindValue(":account", iDatabase.getCurrentUserAccount());
    if (query.exec() && query.first()) {
        userNo = query.value("UserNo").toString();
    }
    if (userNo.isEmpty()) {
        QMessageBox::warning(this, "提示", "未获取到当前用户的编号信息！");
        return;
    }

    // 5. 执行借阅核心逻辑（扣库存+插记录）
    // 开启事务保证原子性
    if (!iDatabase.database.transaction()) {
        QMessageBox::critical(this, "失败", "数据库事务开启失败！");
        return;
    }

    QSqlQuery borrowQuery;
    bool borrowSuccess = true;

    // 5.1 扣减库存
    borrowQuery.prepare("UPDATE Book SET Stock = Stock - :num WHERE BookNo=:bookNo AND Stock >= :num");
    borrowQuery.bindValue(":num", borrowNum);
    borrowQuery.bindValue(":bookNo", bookNo);
    if (!borrowQuery.exec()) {
        borrowSuccess = false;
        qDebug() << "库存扣减失败：" << borrowQuery.lastError().text();
    }
    // 检查是否有行被修改（库存不足/书籍不存在）
    if (borrowQuery.numRowsAffected() == 0) {
        borrowSuccess = false;
        QMessageBox::warning(this, "提示", "书籍库存不足或书籍不存在！");
    }

    // 5.2 插入借阅记录（Case加双引号避免关键字冲突）
    if (borrowSuccess) {
        // 把所有值都改成参数绑定（包括datetime和Case）
        QDateTime dueTime = QDateTime::currentDateTime().addDays(0);

        borrowQuery.prepare("INSERT INTO Borrow (UserNo, BookNo,BorrowNum,BorrowTime, DueTime, \"Case\") "
                            "VALUES (:userNo, :bookNo,:num,:borrowTime, :dueTime, :caseVal)");

        // 绑定参数（新增DueTime）
        borrowQuery.bindValue(":userNo", userNo);
        borrowQuery.bindValue(":bookNo", bookNo);
        borrowQuery.bindValue(":num", borrowNum);
        borrowQuery.bindValue(":borrowTime", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        borrowQuery.bindValue(":dueTime", dueTime.toString("yyyy-MM-dd HH:mm:ss")); // 添加到期时间
        borrowQuery.bindValue(":caseVal", "借出");

        if (!borrowQuery.exec()) {
            borrowSuccess = false;
            qDebug() << "借阅记录插入失败：" << borrowQuery.lastError().text();
        }
    }

    // 5.3 事务提交/回滚
    if (borrowSuccess) {
        iDatabase.database.commit();
        QMessageBox::information(this, "成功", "书籍借阅成功！");
        // 刷新书籍列表（更新库存）
        bookModel->select();
        // 清空输入框
        ui->InputNum->clear();
    } else {
        iDatabase.database.rollback();
        QMessageBox::critical(this, "失败", "书籍借阅失败！");
    }
}

