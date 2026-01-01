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


private:
    explicit IDatabase(QObject *parent = nullptr);
    IDatabase(IDatabase const&)               = delete;
    void operator=(IDatabase const&)  = delete;

    QSqlDatabase database;

signals:

public:
public:
    // bool initPatientModel();
    // int addNewPatient();
    // bool searchPatient(QString filter);
    // bool deleteCurrentPatient();
    // bool submitPatientEdit();
    // void revertPatientEdit();

    int addNewUser();//添加新的用户信息
    bool initUserModel();//初始化用户信息模型
    bool searchUser(QString filter);//按条件搜索用户
    bool deleteCurrentUser();//删除当前选中的用户记录
    bool submitUserEdit();//提交用户信息的编辑修改（保存到数据库）
    void revertUserEdit();//撤销用户信息的编辑修改（放弃未提交的更改）

    // QSqlTableModel *patientTabModel;
    //QItemSelectionModel *thePatientSelection;

    QSqlTableModel *UserTabModel;//用户信息表模型
    QItemSelectionModel *theUserSelection;//用户信息表选择模型


};

#endif // IDATABASE_H
