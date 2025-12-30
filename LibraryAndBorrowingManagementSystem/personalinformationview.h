#ifndef PERSONALINFORMATIONVIEW_H
#define PERSONALINFORMATIONVIEW_H

#include <QWidget>

namespace Ui {
class PersonalInformationView;
}

class PersonalInformationView : public QWidget
{
    Q_OBJECT

public:
    explicit PersonalInformationView(QWidget *parent = nullptr);
    ~PersonalInformationView();

private:
    Ui::PersonalInformationView *ui;
};

#endif // PERSONALINFORMATIONVIEW_H
