#include "idatabase.h"
#include <QUuid>

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

int IDatabase::addNewUser()
{
    UserTabModel->insertRow(UserTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex=UserTabModel->index(UserTabModel->rowCount()-1,1);

    int curRecNo=curIndex.row();
    QSqlRecord curRec=UserTabModel->record(curRecNo);

    UserTabModel->setRecord(curRecNo,curRec);

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
            if(identity=="管理员")
                return "AdminloginOK";
            else if(identity=="读者")
                return "ReaderloginOK";
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

