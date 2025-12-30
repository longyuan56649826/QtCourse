#ifndef MANAGEMENTVIEW_H
#define MANAGEMENTVIEW_H

#include <QWidget>

namespace Ui {
class ManagementView;
}

class ManagementView : public QWidget
{
    Q_OBJECT

public:
    explicit ManagementView(QWidget *parent = nullptr);
    ~ManagementView();

private:
    Ui::ManagementView *ui;
};

#endif // MANAGEMENTVIEW_H
