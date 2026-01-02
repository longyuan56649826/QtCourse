#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class IDatabase : public QObject
{
    Q_OBJECT
public:


    static IDatabase &getInstance()
    {
        static IDatabase    instance;
        return instance;
    }

    QString userLogin(QString userName,QString password);
    QString userRegister(QString userAccountName,QString password,QString passwordagain,QString userName,QString id,QString userIdentity);

    void initDatabase();
    void setCurrentUserAccount(const QString& account); // 设置当前登录用户账号
    QString getCurrentUserAccount(); // 获取当前用户账号
    QSqlRecord getCurrentUserInfo();// 获取当前用户信息记录

    QSqlDatabase database;


private:
    explicit IDatabase(QObject *parent = nullptr);
    IDatabase(IDatabase const&)               = delete;
    void operator=(IDatabase const&)  = delete;


    QString currentUserAccount; // 当前登录用户账号

signals:

public:
    // bool initPatientModel();
    // int addNewPatient();
    // bool searchPatient(QString filter);
    // bool deleteCurrentPatient();
    // bool submitPatientEdit();
    // void revertPatientEdit();

    int addNewBook();//添加新的书本信息
    bool initUserModel();//初始化用户信息模型
    bool searchUser(QString filter);//按条件搜索用户
    bool deleteCurrentUser();//删除当前选中的用户记录
    bool submitUserEdit();//提交用户信息的编辑修改（保存到数据库）
    void revertUserEdit();//撤销用户信息的编辑修改（放弃未提交的更改）
    bool initBookModel();//初始化书本信息模型
    bool searchBook(QString filter);//按条件搜索书本
    bool deleteCurrentBook();//删除当前选中的书本记录
    bool submitBookEdit();//提交书本信息的编辑修改（保存到数据库）
    void revertBookEdit();//撤销书本信息的编辑修改（放弃未提交的更改）

    // 获取当前用户的UserNo
    QString getCurrentUserNo();
    // 执行借阅（用户No、书籍No、借阅数量）
    bool borrowBook(const QString& userNo, const QString& bookNo, int borrowNum);

    // QSqlTableModel *patientTabModel;
    //QItemSelectionModel *thePatientSelection;

    QSqlTableModel *UserTabModel;//用户信息表模型
    QItemSelectionModel *theUserSelection;//用户信息表选择模型
    QSqlTableModel *BookTabModel;//书本信息表模型
    QItemSelectionModel *theBookSelection;//书本信息表选择模型


};

#endif // IDATABASE_H
