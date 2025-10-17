#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon (":/resource/CalculatorMedTile.png"));

    digitBTNs={
        {Qt::Key_0,ui->btnNum0},
        {Qt::Key_1,ui->btnNum1},
        {Qt::Key_2,ui->btnNum2},
        {Qt::Key_3,ui->btnNum3},
        {Qt::Key_4,ui->btnNum4},
        {Qt::Key_5,ui->btnNum5},
        {Qt::Key_6,ui->btnNum6},
        {Qt::Key_7,ui->btnNum7},
        {Qt::Key_8,ui->btnNum8},
        {Qt::Key_9,ui->btnNum9},
        };
    foreach(auto btn,digitBTNs)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    BinaryoperatorBTNs={
        {Qt::Key_Plus,ui->btnPlus},
        {Qt::Key_Asterisk,ui->btnMultiple},
        {Qt::Key_Slash,ui->btnDivide},
        {Qt::Key_Minus,ui->btnMinus},
    };
    foreach(auto btn,BinaryoperatorBTNs)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    // connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnPercentage,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnPlusMinus,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result=0;
    QString m="";
    if(operands.size()==2&&opcodes.size()>0){
        //取操作数
        double operand1=operands.front().toDouble();
        operands.pop_front();
        double operand2=operands.front().toDouble();
        operands.pop_front();
        //取操作符
        QString op=opcodes.front();
        opcodes.pop_front();
            if(op=="+"){
                result=operand1+operand2;
            }
            else if(op=="-"){
                result=operand1-operand2;
            }
            else if(op=="/"){
                if(operand2!=0)
                    result=operand1/operand2;
                else
                    m="除数不能为0，请重新输入";
            }
            else if(op=="×"){
                result=operand1*operand2;
            }
            ui->statusbar->showMessage(QString("operands is %1,opcodes is %2").arg(operands.size()).arg(opcodes.size()));
    }
    else
        ui->statusbar->showMessage(QString("operands is %1,opcodes is %2").arg(operands.size()).arg(opcodes.size()));
    if(m=="")
        return QString::number(result);
    else
        return m;

}

void MainWindow::btnNumClicked()
{
    QString digit=qobject_cast<QPushButton*>(sender())->text();
    if(digit=="0"&&operand=="0")
        digit="";
    else if(digit!="0"&&operand=="0")
        operand="";
    else if(operand==".")
        operand="0.";
    operand+=digit;
    ui->display->setText(operand);
    ui->statusbar->showMessage(qobject_cast<QPushButton*>(sender())->text()+"btn clicked");

}


void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains("."))
        operand+=qobject_cast<QPushButton*>(sender())->text();
    ui->display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
    operand=operand.left(operand.length()-1);
    ui->display->setText(operand);
}


void MainWindow::on_btnClear_clicked()
{
    operand.clear();
    ui->display->setText(operand);
    while(1){
        if (operands.size()!=0)
            operands.pop_front();
        if (opcodes.size()!=0)
            opcodes.pop_front();
        if(operands.size()==0&&opcodes.size()==0)
            break;
    }

}


void MainWindow::btnBinaryOperatorClicked()
{
    opcode=qobject_cast<QPushButton*>(sender())->text();
    QString result;
    if(opcode=="+"||opcode=="-"||opcode=="/"||opcode=="×"){
        if(operand!=""){
            opcodes.push_back(opcode);
            operands.push_back(operand);
            operand="";
        }
        else if(operand=="除数不能为0，请重新输入"||operand=="分母不为0，请重新输入"||operand=="不允许为负数，请重新输入")
            opcodes.pop_front();
        else if(opcodes.size()==1){
            opcodes.pop_front();
            opcodes.push_back(opcode);
        }

    }
    if(operands.size()==2){
        result=calculation();
        ui->display->setText(result);
        operands.push_back(result);
    }

}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operand !=""){
        double result=operand.toDouble();

        operand="";

        QString op=qobject_cast<QPushButton*>(sender())->text();

        if(result>=0){
            if(op=="%")
                result/=100.0;
            else if(op=="1/x"&&result!=0)
                result=1/result;
            else if(op=="x^2")
                result *=result;
            else if(op=="√")
                result=sqrt(result);
            else if(op=="±")
                result=0-result;
            ui->display->setText(QString::number(result));
            operand=QString::number(result);
            if(result==0&&op=="1/x")
                ui->display->setText("分母不为0，请重新输入");
        }
        else if(result<0){
            if(op=="%"||op=="√")
                ui->display->setText("不允许为负数，请重新输入");
            else if(op=="x^2"){
                result=result*result;
            ui->display->setText(QString::number(result));
            operand=QString::number(result);
            }
            else if(op=="1/x"){
                result=1/result;
            ui->display->setText(QString::number(result));
            operand=QString::number(result);
            }
            else if(op=="±"){
                result=0-result;
                ui->display->setText(QString::number(result));
                operand=QString::number(result);
            }
        }
    }
}

void MainWindow::on_btnEqual_clicked()
{
    if(operand!=""){
        operands.push_back(operand);
        operand="";
    }
    QString result=calculation();
    ui->display->setText(result);
    operand=result;
}


void MainWindow::on_btnClearEntry_clicked()
{
    operand.clear();
    ui->display->setText(operand);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    foreach (auto btnKey,digitBTNs.keys()) {
        if(event->key()==btnKey)
            digitBTNs[btnKey]->animateClick();
    }
    foreach (auto btnKey,BinaryoperatorBTNs.keys())
        if(event->key()==btnKey)
            BinaryoperatorBTNs[btnKey]->animateClick();
    if(event->key()==Qt::Key_Period)
        ui->btnPeriod->animateClick();
    if(event->key()==Qt::Key_Equal)
        ui->btnEqual->animateClick();
    if(event->key()==Qt::Key_Backspace)
        ui->btnDel->animateClick();

}

