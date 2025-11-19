#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textChanged=false;

    on_actionNew_triggered();

    statusLabel.setMaximumWidth(200);
    statusLabel.setText("length:"+QString::number(0)+"   lines:"+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(200);
    statusCursorLabel.setText("Ln:"+QString::number(0)+"   Col:"+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author=new QLabel(ui->statusbar);
    author->setText(tr("何宇涵"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    ui->actionLineWrap->setCheckable(true);
    QPlainTextEdit::LineWrapMode mode=ui->textEdit->lineWrapMode();
    ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    ui->actionLineWrap->setChecked(false);  // 不选中状态

    ui->toolBar->setVisible(true);
    ui->statusbar->setVisible(true);


    ui->actionToolbar->setCheckable(true);
    ui->actionToolbar->setChecked(true);

    ui->actionStatusbar->setCheckable(true);
    ui->actionStatusbar->setChecked(true);
    ui->actionShowLineNumber->setChecked(false);
    on_actionShowLineNumber_triggered(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dig;
    dig.exec();
}

void MainWindow::on_actionFind_triggered()
{
    SearchDialog dig(this,ui->textEdit);
    dig.exec();
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dig(this,ui->textEdit);
    dig.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed()){
        return;
    }
    filePath="";
    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件--编辑器"));
    textChanged=false;
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed()){
        return;
    }
    QString filename=QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files(*.txt);;All(*.*)"));
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开文件失败");
        return;
    }
    filePath=filename;
    QTextStream in(&file);
    QString text=in.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
    textChanged=false;
}


void MainWindow::on_actionSave_triggered()
{
    if(filePath==""){
        QString filename=QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files(*.txt)"));
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this,"..","打开保存文件失败");
            return;
        }
        file.close();
        filePath=filename;
        }

    QFile file(filePath);

        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this,"..","打开保存文件失败");
            return;
        }

    QTextStream out(&file);
    QString text=ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged=false;
}


void MainWindow::on_actionSaveAs_triggered()
{
    QString filename=QFileDialog::getSaveFileName(this,"另存为文件",".",tr("Text files(*.txt)"));
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","另存为文件失败");
        return;
    }

    filePath=filename;
    QTextStream out(&file);
    QString text=ui->textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}


void MainWindow::on_textEdit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*"+this->windowTitle());
        textChanged=true;
    }

    statusLabel.setText("length:"+QString::number(ui->textEdit->toPlainText().length())+"   lines:"+QString::number(ui->textEdit->document()->lineCount()));
}

bool MainWindow::userEditConfirmed()
{
    if(textChanged){
        QString path=(filePath!="")?filePath:"无标题文件.txt";
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否更改保存到\n")+"\""+path+"\"?");
        msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

        int r=msg.exec();
        switch(r){
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged=false;
            break;
        case QMessageBox::Cancel:
            return false;
        }

    }

    return true;
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color=QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(!color.isValid()){
        return;
    }

    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format;
    format.setForeground(color);  // 设置字体颜色

    if(cursor.hasSelection()){
        // 有选中文本：仅修改选中部分
        cursor.mergeCharFormat(format);
    } else {
        // 无选中文本：修改全部文本
        cursor.select(QTextCursor::Document);  // 选中整个文档
        cursor.mergeCharFormat(format);
        cursor.clearSelection();  // 取消选中状态
        ui->textEdit->setTextCursor(cursor);   // 恢复光标位置
    }
}


void MainWindow::on_actionBackgroundColor_triggered()
{
    QColor color=QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit{background-color:%1}").arg(color.name()));
    }
}


void MainWindow::on_actionEditorBackgroundColor_triggered()
{

}


void MainWindow::on_actionLineWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode=ui->textEdit->lineWrapMode();
    if (ui->actionLineWrap->isChecked()) {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    }
    else {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    // 获取当前文本的字体（作为对话框初始值）
    QFont currentFont = ui->textEdit->font();
    // 弹出字体选择对话框（包含字体类型、大小、样式等）
    QFont font = QFontDialog::getFont(&ok, currentFont, this, "选择字体");

    if (ok) {  // 用户确认选择
        QTextCursor cursor = ui->textEdit->textCursor();
        QTextCharFormat format;
        // 设置字体（包含类型、大小、粗体、斜体等所有属性）
        format.setFont(font);

        if (cursor.hasSelection()) {
            // 有选中文本：仅修改选中部分
            cursor.mergeCharFormat(format);
        } else {
            // 无选中文本：修改全部文本
            cursor.select(QTextCursor::Document); // 选中整个文档
            cursor.mergeCharFormat(format);
            cursor.clearSelection(); // 取消选中，恢复光标位置
            ui->textEdit->setTextCursor(cursor);
        }
    }
}


void MainWindow::on_actionToolbar_triggered()
{
    bool visible=ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolbar->setChecked(!visible);
}


void MainWindow::on_actionStatusbar_triggered()
{
    bool visible=ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusbar->setChecked(!visible);
}


void MainWindow::on_actionExit_triggered()
{
    if(userEditConfirmed())
        exit(0);
    else{
        on_actionSaveAs_triggered();
    }
    if (!textChanged) {
        close();
    }
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_textEdit_cursorPositionChanged()
{
    int col=0;
    int ln=0;
    int flg=-1;
    int pos=ui->textEdit->textCursor().position();
    QString text=ui->textEdit->toPlainText();

    for(int i=0;i<pos;i++){
        if(text[i]=='\n'){
            ln++;
            flg=i;
        }
    }
    flg++;
    col=pos-flg;
    statusCursorLabel.setText("Ln:"+QString::number(ln+1)+"   Col:"+QString::number(col+1));
}



void MainWindow::on_actionShowLineNumber_triggered(bool checked)
{
    ui->textEdit->hideLineNumberArea(!checked);
}

