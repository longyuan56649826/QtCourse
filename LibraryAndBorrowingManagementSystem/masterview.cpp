#include "masterview.h"
#include "ui_masterview.h"
#include <QDebug>
//#include "idatabase.h"

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    goLoginView();

    //IDatabase::getInstance();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count=ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count-1);//总是显示最新加入的view
    ui->labelTitle->setText(widget->windowTitle());
}

void MasterView::on_btBack_clicked()
{
    goPreviousView();
}


void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    int count=ui->stackedWidget->count();
    if(count>1)
        ui->btBack->setEnabled(true);
    else
        ui->btBack->setEnabled(false);

    QString title=ui->stackedWidget->currentWidget()->windowTitle();

    if(title=="登录"){
        ui->btBack->setEnabled(false);
    }
}


void MasterView::on_btLogout_clicked()
{
    close();
}

void MasterView::goBookView()
{
    qDebug()<<"goBookView";
    bookView =new BookView(this);

    pushWidgetToStackView(bookView);

}

void MasterView::goBorrowAndReturnView()
{
    qDebug()<<"goBorrowAndReturnView";
    borrowandreturnView =new BorrowAndReturnView(this);

    pushWidgetToStackView(borrowandreturnView);

}

void MasterView::goBorrowView()
{
    qDebug()<<"goBorrowView";
    borrowView =new BorrowView(this);

    pushWidgetToStackView(borrowView);

}

void MasterView::goDataView()
{
    qDebug()<<"goDataView";
    dataView =new DataView(this);

    pushWidgetToStackView(dataView);

}

void MasterView::goPreviousView()
{
    int count=ui->stackedWidget->count();
    if(count>1){
        ui->stackedWidget->setCurrentIndex(count-2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());

        QWidget *widget=ui->stackedWidget->widget(count-1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }

}

void MasterView::goLoginView()
{
    qDebug()<<"goLoginView";
    loginView =new LoginView(this);

    pushWidgetToStackView(loginView);


    connect(loginView,SIGNAL(loginAdminSuccess()),this,SLOT(goManagementView()));
    connect(loginView,SIGNAL(loginReaderSuccess()),this,SLOT(goReaderBorrowAndReturnView()));
    connect(loginView,SIGNAL(goregisterSuccess()),this,SLOT(goRegisterView()));
}

void MasterView::goManagementView()
{
    qDebug()<<"goManagementView";
    managementView =new ManagementView(this);

    pushWidgetToStackView(managementView);

    connect(managementView,SIGNAL(goBook()),this,SLOT(goBookView()));
    connect(managementView,SIGNAL(goReader()),this,SLOT(goReaderView()));
    connect(managementView,SIGNAL(goBorrowAndReturn()),this,SLOT(goBorrowAndReturnView()));
    connect(managementView,SIGNAL(goBorrowAndReturndata()),this,SLOT(goBorrowAndReturnDataView()));
    connect(managementView,SIGNAL(goBecomeReader()),this,SLOT(goReaderBorrowAndReturnView()));

}

void MasterView::goPersonalInformationView()
{
    qDebug()<<"goPersonalInformationView";
    personlinformationView =new PersonalInformationView(this);

    pushWidgetToStackView(personlinformationView);

}

void MasterView::goReaderBorrowAndReturnView()
{
    qDebug()<<"goReaderBorrowAndReturnView";
    readerborrowandreturnView =new ReaderBorrowAndReturnView(this);

    pushWidgetToStackView(readerborrowandreturnView);

    connect(readerborrowandreturnView,SIGNAL(goReturn()),this,SLOT(goReturnView()));
    connect(readerborrowandreturnView,SIGNAL(goBorrow()),this,SLOT(goBorrowView()));
    connect(readerborrowandreturnView,SIGNAL(goPersonalInformation()),this,SLOT(goPersonalInformationView()));

}



void MasterView::goReaderView()
{
    qDebug()<<"goReaderView";
    readerView =new ReaderView(this);

    pushWidgetToStackView(readerView);
    connect(readerView,SIGNAL(goUserEditView(int)),this,SLOT(goUserEditView(int)));

}

void MasterView::goRegisterView()
{
    qDebug()<<"goRegisterView";
    registerView =new RegisterView(this);

    pushWidgetToStackView(registerView);

    connect(registerView,SIGNAL(registerSuccess()),this,SLOT(goLoginView()));
    connect(registerView,SIGNAL(gologinSuccess()),this,SLOT(goLoginView()));

}

void MasterView::goReturnView()
{
    qDebug()<<"goReturnView";
    returnView =new ReturnView(this);

    pushWidgetToStackView(returnView);

}

void MasterView::goBorrowAndReturnDataView()
{
    qDebug()<<"goBorrowAndReturnDataView";
    borrowandreturndataView =new BorrowAndReturnDataView(this);

    pushWidgetToStackView(borrowandreturndataView);
}

void MasterView::goUserEditView(int rowNo)
{
    qDebug()<<"goUserEditView";
    usereditView =new UserEditView(this,rowNo);

    pushWidgetToStackView(usereditView);
}




