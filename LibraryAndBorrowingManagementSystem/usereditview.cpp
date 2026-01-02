#include "usereditview.h"
#include "ui_usereditview.h"
#include <QSqlTableModel>
#include "idatabase.h"
#include <QDebug>

UserEditView::UserEditView(QWidget *parent,int index)
    : QWidget(parent)
    , ui(new Ui::UserEditView)
    , m_userIndex(index)
    , m_mapper(new QDataWidgetMapper(this))
{
    ui->setupUi(this);
    loadUserInfo();

}

UserEditView::~UserEditView()
{
    delete ui;
}

void UserEditView::on_btEdit_clicked()
{
    dataMapper->submit();
    IDatabase::getInstance().submitUserEdit();
    emit goBack();
}


void UserEditView::on_btCancel_clicked()
{
    IDatabase::getInstance().revertUserEdit();

    emit goBack();
}

void UserEditView::loadUserInfo()
{
    // 从单例中获取用户模型
    QSqlTableModel *userModel = IDatabase::getInstance().UserTabModel;
    if (!userModel) {
        qDebug() << "用户模型未初始化";
        return;
    }

    // 检查索引有效性
    if (m_userIndex < 0 || m_userIndex >= userModel->rowCount()) {
        qDebug() << "无效的用户索引:" << m_userIndex;
        return;
    }

    // 绑定模型到数据映射器
    m_mapper->setModel(userModel);
    m_mapper->setCurrentIndex(m_userIndex); // 设置当前行

    // 关键：将数据库字段与UI输入框绑定（根据你的User表结构调整）
    // 格式：m_mapper->addMapping(输入框控件, 字段在表中的索引)
    // 请根据实际表字段顺序修改第二个参数！
    m_mapper->addMapping(ui->InputUserName, userModel->fieldIndex("UserAccountName")); // 账号
    m_mapper->addMapping(ui->InputName, userModel->fieldIndex("userName"));         // 用户名
    m_mapper->addMapping(ui->InputId, userModel->fieldIndex("UserId"));             // 用户ID
    m_mapper->addMapping(ui->InputUserPassword, userModel->fieldIndex("UserPassword"));     // 密码
    m_mapper->addMapping(ui->dbeditIdentity, userModel->fieldIndex("userIdentity"));     // 身份

    // 刷新显示（将数据加载到输入框）
    m_mapper->toFirst();
}
