#include "idatabase.h"
#include <QUuid>
#include <QMessageBox>

bool IDatabase::initUserModel()
{
    UserTabModel=new QSqlTableModel(this,database);
    UserTabModel->setTable("User");
    UserTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    UserTabModel->setSort(UserTabModel->fieldIndex("UserNo"),Qt::AscendingOrder);

    if(!(UserTabModel->select()))
        return false;
    theUserSelection=new QItemSelectionModel(UserTabModel);
    return true;
}

int IDatabase::addNewBook()
{
    BookTabModel->insertRow(BookTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex=BookTabModel->index(BookTabModel->rowCount()-1,1);

    int curRecNo=curIndex.row();
    QSqlRecord curRec=BookTabModel->record(curRecNo);
    curRec.setValue("BookNo",QUuid::createUuid().toString(QUuid::WithoutBraces));

    BookTabModel->setRecord(curRecNo,curRec);

    return curIndex.row();
}

bool IDatabase::searchUser(QString filter)
{
    UserTabModel->setFilter(filter);
    return UserTabModel->select();
}

bool IDatabase::deleteCurrentUser()
{
    QModelIndex curIndex=theUserSelection->currentIndex();
    UserTabModel->removeRow(curIndex.row());
    UserTabModel->submitAll();
    UserTabModel->select();
    return true;
}

bool IDatabase::submitUserEdit()
{
    return UserTabModel->submitAll();
}

void IDatabase::revertUserEdit()
{
    UserTabModel->revertAll();
}

bool IDatabase::initBookModel()
{
    BookTabModel=new QSqlTableModel(this,database);
    BookTabModel->setTable("Book");
    BookTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    BookTabModel->setSort(BookTabModel->fieldIndex("BookNo"),Qt::AscendingOrder);

    if(!(BookTabModel->select()))
        return false;
    theBookSelection=new QItemSelectionModel(BookTabModel);
    return true;
}

bool IDatabase::searchBook(QString filter)
{
    BookTabModel->setFilter(filter);
    return BookTabModel->select();
}

bool IDatabase::deleteCurrentBook()
{
    QModelIndex curIndex=theBookSelection->currentIndex();
    BookTabModel->removeRow(curIndex.row());
    BookTabModel->submitAll();
    BookTabModel->select();
    return true;
}

bool IDatabase::submitBookEdit()
{
    return BookTabModel->submitAll();
}

void IDatabase::revertBookEdit()
{
    BookTabModel->revertAll();
}

bool IDatabase::initBorrowModel()
{
    // 改用QSqlQueryModel实现多表关联查询
    BorrowTabModel = new QSqlQueryModel(this);

    // 关联Borrow + User + Book表，替换编号为名称
    QString sql = R"(
        SELECT
            b.BorrowNo AS 借阅编号,
            u.UserAccountName AS 用户账号,
            u.UserName AS 用户姓名,
            bo.BookName AS 书本名称,
            b.BorrowNum AS 借阅数量,
            b.BorrowTime AS 借阅时间,
            b.DueTime AS 到期时间,
            b."Case" AS 借阅状态,
            b.ReturnTime AS 归还时间
        FROM Borrow b
        LEFT JOIN User u ON b.UserNo = u.UserNo
        LEFT JOIN Book bo ON b.BookNo = bo.BookNo
        ORDER BY b.BorrowNo ASC
    )";

    BorrowTabModel->setQuery(sql, database);

    // 检查查询是否成功
    if (BorrowTabModel->lastError().isValid()) {
        qDebug() << "借阅模型初始化失败：" << BorrowTabModel->lastError().text();
        return false;
    }

    // 设置表格中文表头（前端可直接使用）
    BorrowTabModel->setHeaderData(0, Qt::Horizontal, "借阅编号");
    BorrowTabModel->setHeaderData(1, Qt::Horizontal, "用户账号");
    BorrowTabModel->setHeaderData(2, Qt::Horizontal, "用户姓名");
    BorrowTabModel->setHeaderData(3, Qt::Horizontal, "书本名称");
    BorrowTabModel->setHeaderData(4, Qt::Horizontal, "借阅数量");
    BorrowTabModel->setHeaderData(5, Qt::Horizontal, "借阅时间");
    BorrowTabModel->setHeaderData(6, Qt::Horizontal, "到期时间");
    BorrowTabModel->setHeaderData(7, Qt::Horizontal, "借阅状态");
    BorrowTabModel->setHeaderData(8, Qt::Horizontal, "归还时间");

    // 修复原代码的赋值错误（theBookSelection → theBorrowSelection）
    theBorrowSelection = new QItemSelectionModel(BorrowTabModel);
    return true;
}

bool IDatabase::searchBorrow(QString filter)
{
    // 基础关联SQL
    QString baseSql = R"(
        SELECT
            b.BorrowNo AS 借阅编号,
            u.UserAccountName AS 用户账号,
            u.UserName AS 用户姓名,
            bo.BookName AS 书本名称,
            b.BorrowNum AS 借阅数量,  -- 新增：借阅数量
            b.BorrowTime AS 借阅时间,
            b."Case" AS 借阅状态,
            b.ReturnTime AS 归还时间
        FROM Borrow b
        LEFT JOIN User u ON b.UserNo = u.UserNo
        LEFT JOIN Book bo ON b.BookNo = bo.BookNo
    )";

    // 拼接过滤条件（无过滤时查询所有）
    if (!filter.isEmpty()) {
        baseSql += " WHERE " + filter;
    }
    baseSql += " ORDER BY b.BorrowNo ASC";

    BorrowTabModel->setQuery(baseSql, database);

    // 返回查询是否成功
    bool success = !BorrowTabModel->lastError().isValid();
    if (!success) {
        qDebug() << "借阅记录搜索失败：" << BorrowTabModel->lastError().text();
    }
    return success;
}

bool IDatabase::initCurrentUserBorrowModel()
{
    if (currentUserAccount.isEmpty()) {
        qDebug() << "未登录用户，无法初始化当前用户借阅模型";
        return false;
    }

    // 获取当前用户的UserNo
    QString userNo = getCurrentUserNo();
    if (userNo.isEmpty()) {
        qDebug() << "获取当前用户UserNo失败";
        return false;
    }

    // 用QSqlQueryModel查询“当前用户的未归还借阅记录”（关联Book表）
    BorrowTabModel = new QSqlQueryModel(this);
    QString sql = R"(
        SELECT
            b.BorrowNo AS 借阅编号,
            bo.BookName AS 书本名字,
            b.BorrowNum AS 借书数量,
            b.BorrowTime AS 借书时间,
            b.DueTime AS 到期时间
        FROM Borrow b
        LEFT JOIN Book bo ON b.BookNo = bo.BookNo
        WHERE b.UserNo = :userNo AND b."Case" = '借出'  -- 只显示当前用户的未归还记录
        ORDER BY b.BorrowTime DESC
    )";

    QSqlQuery query(database);
    query.prepare(sql);
    query.bindValue(":userNo", userNo);
    query.exec();

    BorrowTabModel->setQuery(query);

    // 检查查询是否成功
    if (BorrowTabModel->lastError().isValid()) {
        qDebug() << "当前用户借阅模型初始化失败：" << BorrowTabModel->lastError().text();
        return false;
    }

    // 设置中文表头
    BorrowTabModel->setHeaderData(0, Qt::Horizontal, "借阅编号");
    BorrowTabModel->setHeaderData(1, Qt::Horizontal, "书本名字");
    BorrowTabModel->setHeaderData(2, Qt::Horizontal, "借书数量");
    BorrowTabModel->setHeaderData(3, Qt::Horizontal, "借书时间");
    BorrowTabModel->setHeaderData(4, Qt::Horizontal, "到期时间");

    theBorrowSelection = new QItemSelectionModel(BorrowTabModel);
    return true;
}

// 2. 执行归还操作（事务：更新状态+归还时间+恢复库存）
bool IDatabase::returnBook(const QString& borrowNo)
{
    if (borrowNo.isEmpty()) {
        qDebug() << "借阅编号为空，无法执行归还";
        return false;
    }

    // 开启事务（保证原子性）
    if (!database.transaction()) {
        qDebug() << "归还事务开启失败：" << database.lastError().text();
        return false;
    }

    QSqlQuery query;
    bool success = true;
    int borrowNum = 0;
    QString bookNo;

    // 步骤1：查询该借阅记录的“借阅数量”和“书本编号”（用于恢复库存）
    query.prepare("SELECT BorrowNum, BookNo FROM Borrow WHERE BorrowNo = :borrowNo");
    query.bindValue(":borrowNo", borrowNo);
    if (!query.exec() || !query.first()) {
        qDebug() << "查询借阅记录失败：" << query.lastError().text();
        success = false;
    } else {
        borrowNum = query.value("BorrowNum").toInt();
        bookNo = query.value("BookNo").toString();
    }

    // 步骤2：更新Borrow表（状态改为“已归还”+ 写入归还时间）
    if (success) {
        query.prepare("UPDATE Borrow SET \"Case\" = '已归还', ReturnTime = datetime('now') WHERE BorrowNo = :borrowNo");
        query.bindValue(":borrowNo", borrowNo);
        if (!query.exec()) {
            qDebug() << "更新借阅状态失败：" << query.lastError().text();
            success = false;
        }
    }

    // 步骤3：恢复Book表库存（Stock += 借阅数量）
    if (success && !bookNo.isEmpty() && borrowNum > 0) {
        query.prepare("UPDATE Book SET Stock = Stock + :num WHERE BookNo = :bookNo");
        query.bindValue(":num", borrowNum);
        query.bindValue(":bookNo", bookNo);
        if (!query.exec()) {
            qDebug() << "恢复库存失败：" << query.lastError().text();
            success = false;
        }
    }

    // 事务提交/回滚
    if (success) {
        database.commit();
    } else {
        database.rollback();
    }

    return success;
}

bool IDatabase::searchCurrentUserBorrowByBookName(const QString& bookName)
{
    if (currentUserAccount.isEmpty()) {
        qDebug() << "未登录用户，无法搜索借阅记录";
        return false;
    }

    // 获取当前用户UserNo
    QString userNo = getCurrentUserNo();
    if (userNo.isEmpty()) {
        qDebug() << "获取当前用户UserNo失败";
        return false;
    }

    // 拼接模糊查询条件（书本名+当前用户+未归还）
    QString sql = R"(
        SELECT
            b.BorrowNo AS 借阅编号,
            bo.BookName AS 书本名字,
            b.BorrowNum AS 借书数量,
            b.BorrowTime AS 借书时间
        FROM Borrow b
        LEFT JOIN Book bo ON b.BookNo = bo.BookNo
        WHERE b.UserNo = :userNo
          AND b."Case" = '借出'
    )";

    // 加入书本名模糊查询（非空时）
    if (!bookName.isEmpty()) {
        sql += " AND bo.BookName LIKE :bookName";
    }

    sql += " ORDER BY b.BorrowTime DESC";

    QSqlQuery query(database);
    query.prepare(sql);
    query.bindValue(":userNo", userNo);
    if (!bookName.isEmpty()) {
        query.bindValue(":bookName", "%" + bookName + "%");
    }
    query.exec();

    BorrowTabModel->setQuery(query);

    // 检查查询结果
    bool success = !BorrowTabModel->lastError().isValid();
    if (!success) {
        qDebug() << "搜索当前用户借阅记录失败：" << BorrowTabModel->lastError().text();
    }
    return success;
}

bool IDatabase::initBorrowBookAggregateModel()
{
    // 改用QSqlQueryModel实现聚合查询
    BorrowTabModel = new QSqlQueryModel(this);

    // 核心SQL：按BookNo分组，统计总借出数量，关联Book表获取书名，只查“借出”状态
    QString sql = R"(
        SELECT
            bo.BookName AS 书本名称,
            SUM(b.BorrowNum) AS 总借出数量
        FROM Borrow b
        LEFT JOIN Book bo ON b.BookNo = bo.BookNo
        WHERE b."Case" = '借出'  -- 只统计未归还的借出记录
        GROUP BY b.BookNo, bo.BookName  -- 按BookNo分组（确保相同书籍合并）
        ORDER BY SUM(b.BorrowNum) DESC  -- 按借出数量降序排列
    )";

    BorrowTabModel->setQuery(sql, database);

    // 检查查询是否成功
    if (BorrowTabModel->lastError().isValid()) {
        qDebug() << "借出书籍聚合模型初始化失败：" << BorrowTabModel->lastError().text();
        return false;
    }

    // 设置表格中文表头
    BorrowTabModel->setHeaderData(0, Qt::Horizontal, "书本名称");
    BorrowTabModel->setHeaderData(1, Qt::Horizontal, "总借出数量");

    theBorrowSelection = new QItemSelectionModel(BorrowTabModel);
    return true;
}

bool IDatabase::searchBorrowBookAggregateByBookName(const QString& bookName)
{
    // 改用QSqlQueryModel实现聚合查询（带书名过滤）
    BorrowTabModel = new QSqlQueryModel(this);

    // 核心SQL：按BookNo分组，统计总借出数量，关联Book表获取书名，只查“借出”状态
    QString sql = R"(
        SELECT
            bo.BookName AS 书本名称,
            SUM(b.BorrowNum) AS 总借出数量
        FROM Borrow b
        LEFT JOIN Book bo ON b.BookNo = bo.BookNo
        WHERE b."Case" = '借出'  -- 只统计未归还的借出记录
    )";

    // 加入书本名模糊查询（非空时）
    if (!bookName.isEmpty()) {
        sql += " AND bo.BookName LIKE :bookName";
    }

    sql += R"(
        GROUP BY b.BookNo, bo.BookName  -- 按BookNo分组（确保相同书籍合并）
        ORDER BY SUM(b.BorrowNum) DESC  -- 按借出数量降序排列
    )";

    QSqlQuery query(database);
    query.prepare(sql);
    if (!bookName.isEmpty()) {
        query.bindValue(":bookName", "%" + bookName + "%"); // 模糊匹配
    }
    query.exec();

    BorrowTabModel->setQuery(query);

    // 检查查询是否成功
    if (BorrowTabModel->lastError().isValid()) {
        qDebug() << "借出书籍聚合搜索失败：" << BorrowTabModel->lastError().text();
        return false;
    }

    // 设置表格中文表头
    BorrowTabModel->setHeaderData(0, Qt::Horizontal, "书本名称");
    BorrowTabModel->setHeaderData(1, Qt::Horizontal, "总借出数量");

    theBorrowSelection = new QItemSelectionModel(BorrowTabModel);
    return true;
}

QString IDatabase::getCurrentUserNo()
{
    if (currentUserAccount.isEmpty()) {
        qDebug() << "未登录用户，无法获取UserNo";
        return "";
    }

    QSqlQuery query;
    query.prepare("SELECT UserNo FROM User WHERE UserAccountName=:account");
    query.bindValue(":account", currentUserAccount);
    if (query.exec() && query.first()) {
        return query.value("UserNo").toString();
    }
    qDebug() << "查询当前用户UserNo失败";
    return "";
}

// 2. 实现“借阅操作（扣库存+写借阅记录）”
bool IDatabase::borrowBook(const QString& userNo, const QString& bookNo, int borrowNum)
{
    // 校验参数
    if (userNo.isEmpty() || bookNo.isEmpty() || borrowNum <= 0) {
        qDebug() << "借阅参数无效";
        return false;
    }

    // 开启事务（保证原子性）
    if (!database.transaction()) {
        qDebug() << "事务开启失败：" << database.lastError().text();
        return false;
    }

    QSqlQuery query;
    // 步骤1：扣减Book表库存
    query.prepare("UPDATE Book SET Stock = Stock - :num WHERE BookNo=:bookNo AND Stock >= :num");
    query.bindValue(":num", borrowNum);
    query.bindValue(":bookNo", bookNo);
    if (!query.exec()) {
        qDebug() << "库存扣减失败：" << query.lastError().text();
        database.rollback();
        return false;
    }
    // 检查是否有行被修改（防止库存不足/书籍不存在）
    if (query.numRowsAffected() == 0) {
        qDebug() << "库存不足或书籍不存在";
        database.rollback();
        return false;
    }

    // 步骤2：插入Borrow表（Case设为“借出”，ReturnTime留空）
    query.prepare("INSERT INTO Borrow (UserNo, BookNo, BorrowTime, \"Case\", BorrowNum) "
                  "VALUES (:userNo, :bookNo, datetime('now'), '借出', :num)");
    query.bindValue(":userNo", userNo);
    query.bindValue(":bookNo", bookNo);
    query.bindValue(":num", borrowNum);
    if (!query.exec()) {
        qDebug() << "借阅记录插入失败：" << query.lastError().text();
        database.rollback();
        return false;
    }

    // 提交事务
    if (!database.commit()) {
        qDebug() << "事务提交失败：" << database.lastError().text();
        database.rollback();
        return false;
    }

    return true;
}

QString IDatabase::userLogin(QString userName, QString password)
{
    QSqlQuery query;
    query.prepare("select UserAccountName,UserPassword,UserIdentity from user where UserAccountName=:User");
    query.bindValue(":User",userName);
    query.exec();
    if(query.first() &&query.value("UserAccountName").isValid()){
        QString passwd=query.value("UserPassword").toString();
        QString identity=query.value("UserIdentity").toString();

        if(passwd==password){
            if(identity=="管理员"){
                setCurrentUserAccount(userName);
                return "AdminloginOK";
            }
            else if(identity=="读者"){
                setCurrentUserAccount(userName);
                return "ReaderloginOK";
            }
        }
        else{
            qDebug()<<"wrong Password";
            return "wrongPassword";
        }
    }
    else{
        qDebug()<<"no such user";
        return "wrongUserAccountname";
    }
    query.first();

}

QString IDatabase::userRegister(QString userAccountName, QString password, QString passwordagain, QString userName, QString id,QString userIdentity)
{
    // 1. 先去除所有输入的首尾空格（避免用户只输入空格的情况）
    userAccountName = userAccountName.trimmed();
    password = password.trimmed();
    passwordagain = passwordagain.trimmed();
    userName = userName.trimmed();
    id = id.trimmed();
    userIdentity = userIdentity.trimmed();

    // 2. 校验所有字段是否为空，只要有一个为空就禁止注册
    if (userAccountName.isEmpty() ||
        password.isEmpty() ||
        passwordagain.isEmpty() ||
        userName.isEmpty() ||
        id.isEmpty() ||
        userIdentity.isEmpty())
    {
        qDebug() << "注册失败：所有信息必须填写，不能为空！";
        return "emptyField"; // 返回空字段标识，便于调用方提示用户
    }

    // ========== 第一步：校验两次输入的密码是否一致 ==========
    if (password != passwordagain) {
        qDebug() << "注册失败：两次输入的密码不一致";
        return "passwordNotMatch"; // 密码不一致
    }

    // ========== 第二步：检查数据库中是否存在相同的账号名或ID ==========
    QSqlQuery query;
    // 准备查询语句：检查user表中是否有相同的accountname（账号名）或相同的id
    // 注意：这里假设User表中存储账号名的字段是userAccountName，存储ID的字段是id
    // 如果你的字段名不同（比如账号名字段是account），请对应修改
    query.prepare("SELECT * FROM User WHERE UserAccountName=:UserAccountName OR UserId=:UserId");
    // 绑定账号名参数
    query.bindValue(":UserAccountName", userAccountName);
    // 绑定ID参数
    query.bindValue(":UserId", id);

    // 执行查询并检查是否成功
    if (!query.exec()) {
        qDebug() << "注册失败：数据库查询出错 -" << query.lastError().text();
        return "dbQueryError"; // 数据库查询错误
    }

    // 如果查询到记录，说明账号名或ID已存在
    if (query.first()) {
        qDebug() << "注册失败：账号名或ID已存在（账号名：" << userAccountName
                 << "，UserId：" << id << "）";
        return "accountOrIdExisted"; // 账号/ID已存在
    }

    // ========== 第三步：所有校验通过，插入新用户信息到数据库 ==========
    // 准备插入语句（参数化查询，防止SQL注入）
    query.prepare("INSERT INTO User (userAccountName, UserPassword, userName, UserId, userIdentity) "
                  "VALUES (:account, :pwd, :name, :id, :identity)");
    // 绑定所有用户信息参数
    query.bindValue(":account", userAccountName); // 账号名
    query.bindValue(":pwd", password);            // 只保存第一个密码（password）
    query.bindValue(":identity", userIdentity);   // 用户身份（如学生/教师）
    query.bindValue(":name", userName);           // 用户名
    query.bindValue(":id",id);                   // 用户ID

    // 执行插入操作
    if (query.exec()) {
        qDebug() << "注册成功：账号名" << userAccountName;
        return "registerOK"; // 注册成功
    } else {
        // 插入失败，输出错误信息
        qDebug() << "注册失败：插入数据出错 -" << query.lastError().text();
        return "dbInsertError"; // 数据库插入错误
    }
}

IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    initDatabase();
}


void IDatabase::initDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString aFile="D:/Qt6/Samples/QtCourse/LibraryAndBorrowingManagementSystem/dbLibrary.db";
    database.setDatabaseName(aFile);

    if(!database.open()){
        qDebug()<<"failed to open database";
    }else
        qDebug()<<"open database is OK"<<database.connectionName();

}

void IDatabase::setCurrentUserAccount(const QString& account) {
    currentUserAccount = account;
}

QString IDatabase::getCurrentUserAccount() {
    return currentUserAccount;
}

QSqlRecord IDatabase::getCurrentUserInfo() {
    if (currentUserAccount.isEmpty()) {
        return QSqlRecord(); // 未登录，返回空记录
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM User WHERE UserAccountName=:account");
    query.bindValue(":account", currentUserAccount);
    if (query.exec() && query.first()) {
        return query.record(); // 返回当前用户的记录
    }
    return QSqlRecord(); // 查询失败，返回空记录
}
