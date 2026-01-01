#ifndef USEREDITVIEW_H
#define USEREDITVIEW_H

#include <QWidget>

namespace Ui {
class UserEditView;
}

class UserEditView : public QWidget
{
    Q_OBJECT

public:
    explicit UserEditView(QWidget *parent = nullptr,int index=0);
    ~UserEditView();

private:
    Ui::UserEditView *ui;
};

#endif // USEREDITVIEW_H
