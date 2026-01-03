#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"
#include <QMessageBox>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::checkExpiredBorrows()
{
    IDatabase &iDatabase = IDatabase::getInstance();
    QString userNo = iDatabase.getCurrentUserNo(); // 需要实现获取当前用户编号的方法

    if (userNo.isEmpty()) return;

    QSqlQuery query;
    query.prepare("SELECT b.BookNo, bk.BookName, b.DueTime "
                  "FROM Borrow b "
                  "JOIN Book bk ON b.BookNo = bk.BookNo "
                  "WHERE b.UserNo = :userNo "
                  "AND b.\"Case\" = '借出' "
                  "AND b.DueTime <= :currentTime");

    query.bindValue(":userNo", userNo);
    query.bindValue(":currentTime", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    if (query.exec()) {
        QString expiredBooks;
        int count = 0;
        while (query.next()) {
            count++;
            QString bookNo = query.value("BookNo").toString();
            QString bookName = query.value("BookName").toString();
            QString dueTime = query.value("DueTime").toString();
            expiredBooks += QString("%1（编号：%2），到期时间：%3\n").arg(bookName).arg(bookNo).arg(dueTime);
        }

        if (count > 0) {
            QMessageBox::warning(nullptr, "到期提醒",
                                 QString("您有%1本图书已到期，请及时归还：\n%2").arg(count).arg(expiredBooks));
        }
    } else {
        qDebug() << "查询到期图书失败：" << query.lastError().text();
    }
}

void LoginView::checkSoonExpiredBorrows()
{
    IDatabase &iDatabase = IDatabase::getInstance();
    QString userNo = iDatabase.getCurrentUserNo();

    if (userNo.isEmpty()) return;

    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime threeDaysLater = currentTime.addDays(3);

    QSqlQuery query;
    query.prepare("SELECT b.BookNo, bk.BookName, b.DueTime "
                  "FROM Borrow b "
                  "JOIN Book bk ON b.BookNo = bk.BookNo "
                  "WHERE b.UserNo = :userNo "
                  "AND b.\"Case\" = '借出' "
                  "AND b.DueTime > :currentTime "
                  "AND b.DueTime <= :threeDaysLater");

    query.bindValue(":userNo", userNo);
    query.bindValue(":currentTime", currentTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":threeDaysLater", threeDaysLater.toString("yyyy-MM-dd HH:mm:ss"));

    if (query.exec()) {
        QString soonExpiredBooks;
        int count = 0;
        while (query.next()) {
            count++;
            QString bookNo = query.value("BookNo").toString();
            QString bookName = query.value("BookName").toString();
            QString dueTime = query.value("DueTime").toString();
            soonExpiredBooks += QString("%1（编号：%2），到期时间：%3\n").arg(bookName).arg(bookNo).arg(dueTime);
        }

        if (count > 0) {
            QMessageBox::information(nullptr, "即将到期提醒",
                                     QString("您有%1本图书将在3天内到期，请留意归还时间：\n%2").arg(count).arg(soonExpiredBooks));
        }
    } else {
        qDebug() << "查询即将到期图书失败：" << query.lastError().text();
    }
}



void LoginView::on_btSignLogin_clicked()
{
    QString status=IDatabase::getInstance().userLogin(ui->InputUserName->text(),ui->InputUserPassword->text());



    if(status=="AdminloginOK"){
        emit loginAdminSuccess();
        checkExpiredBorrows();
        checkSoonExpiredBorrows();
    }
    else if(status=="ReaderloginOK"){
        emit loginReaderSuccess();
        checkExpiredBorrows();
        checkSoonExpiredBorrows();
    }

    else if(status=="wrongPassword")
         QMessageBox::critical(this, "登录失败", "密码错误，请重新输入！", QMessageBox::Ok);
    else if(status=="wrongUserAccountname")
        QMessageBox::critical(this, "登录失败", "未找到该账号，请重新输入！", QMessageBox::Ok);
}


void LoginView::on_btSignRegister_clicked()
{
    emit goregisterSuccess();
}

