#ifndef RETURNVIEW_H
#define RETURNVIEW_H

#include <QWidget>

namespace Ui {
class ReturnView;
}

class ReturnView : public QWidget
{
    Q_OBJECT

public:
    explicit ReturnView(QWidget *parent = nullptr);
    ~ReturnView();

private:
    Ui::ReturnView *ui;
};

#endif // RETURNVIEW_H
