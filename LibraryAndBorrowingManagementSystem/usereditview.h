#ifndef USEREDITVIEW_H
#define USEREDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>
#include "idatabase.h"

namespace Ui {
class UserEditView;
}

class UserEditView : public QWidget
{
    Q_OBJECT

public:
    explicit UserEditView(QWidget *parent = nullptr,int index=0);
    ~UserEditView();


private slots:
    void on_btEdit_clicked();

    void on_btCancel_clicked();

private:
    Ui::UserEditView *ui;
     int m_userIndex; // 存储选中的用户行索引
    QDataWidgetMapper *dataMapper; // 数据映射器（简化数据与控件的绑定）
     QDataWidgetMapper *m_mapper; // 声明m_mapper成员
    void loadUserInfo(); // 加载用户信息到输入框

signals:
    void goBack();
};

#endif // USEREDITVIEW_H
