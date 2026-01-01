#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>

#include "bookview.h"
#include "borrowandreturnview.h"
#include "borrowview.h"
#include "dataview.h"
#include "loginview.h"
#include "managementview.h"
#include "personalinformationview.h"
#include "readerborrowandreturnview.h"
#include "readerview.h"
#include "registerview.h"
#include "returnview.h"
#include "borrowandreturndataview.h"
#include "usereditview.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class MasterView : public QWidget
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

public slots:
    void goPreviousView();
    void goBookView();
    void goBorrowAndReturnView();
    void goBorrowView();
    void goDataView();;
    void goLoginView();
    void goManagementView();
    void goPersonalInformationView();
    void goReaderBorrowAndReturnView();
    void goReaderView();
    void goRegisterView();
    void goReturnView();
    void goBorrowAndReturnDataView();
    void goUserEditView(int rowNo);


private slots:
    void on_btBack_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_btLogout_clicked();

private:
    void pushWidgetToStackView(QWidget *widget);

    Ui::MasterView *ui;

    BookView *bookView;
    BorrowAndReturnView *borrowandreturnView;
    BorrowView *borrowView;
    DataView *dataView;
    LoginView *loginView;
    ManagementView *managementView;
    PersonalInformationView *personlinformationView;
    ReaderBorrowAndReturnView *readerborrowandreturnView;
    ReaderView *readerView;
    RegisterView *registerView;
    ReturnView *returnView;
    BorrowAndReturnDataView *borrowandreturndataView;
    UserEditView *usereditView;

};
#endif // MASTERVIEW_H
