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

    void loadCurrentUserInfo();

private slots:
    void on_btBack_clicked();

    void on_btEdit_clicked();

private:
    Ui::PersonalInformationView *ui;

signals:
    void goBack();
    void goUserEditView(int index);
};

#endif // PERSONALINFORMATIONVIEW_H
