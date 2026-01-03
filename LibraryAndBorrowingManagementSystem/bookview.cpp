#include "bookview.h"
#include "ui_bookview.h"
#include "idatabase.h"

BookView::BookView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BookView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDatabase &iDatabase=IDatabase::getInstance();
    if(iDatabase.initBookModel()){
        ui->tableView->setModel(iDatabase.BookTabModel);
        ui->tableView->setSelectionModel(iDatabase.theBookSelection);

        // ========== 核心修改：设置表格中文表头 ==========
        QSqlTableModel *model = iDatabase.BookTabModel;
        // 1. 根据数据库字段名获取列索引，设置对应中文表头
        // 请根据你的实际数据库字段名和需要显示的中文调整！
        model->setHeaderData(model->fieldIndex("BookNo"), Qt::Horizontal, "书本编号");
        model->setHeaderData(model->fieldIndex("BookName"), Qt::Horizontal, "书本名字");
        model->setHeaderData(model->fieldIndex("Stock"), Qt::Horizontal, "库存");

        // 2. 可选：调整列宽自适应（避免中文表头显示不全）
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // 或指定某列自适应：ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

        qDebug() << "ReaderView：中文表头设置完成";
    }
}

BookView::~BookView()
{
    delete ui;
}


void BookView::on_btSearch_clicked()
{
    QString filter=QString("BookName like '%%1%'").arg(ui->txtSearch->text());
    // 禁用按钮防止重复点击
    ui->btSearch->setEnabled(false);

    // 创建新线程
    searchThread = new QThread;

    // 创建一个临时对象用于在线程中执行任务
    QObject *worker = new QObject;
    connect(searchThread, &QThread::started, worker, [=]() {
        // 耗时操作放这里（数据库查询）
        IDatabase::getInstance().searchBook(filter);

        // 任务完成后切换回主线程更新界面
        QMetaObject::invokeMethod(this, [=]() {
            // 恢复按钮状态
            ui->btSearch->setEnabled(true);
            // 释放资源
            worker->deleteLater();
            searchThread->quit();
            searchThread->wait();
            searchThread->deleteLater();
        }, Qt::QueuedConnection);
    });

    // 启动线程
    worker->moveToThread(searchThread);
    searchThread->start();
}


void BookView::on_btDelete_clicked()
{
    IDatabase::getInstance().deleteCurrentBook();
}


void BookView::on_btEdit_clicked()
{
    QModelIndex curIndex=IDatabase::getInstance().theBookSelection->currentIndex();
    emit goBookEditView(curIndex.row());
}


void BookView::on_btBack_clicked()
{
    emit goBack();
}

void BookView::on_btAdd_clicked()
{
    int currow=IDatabase::getInstance().addNewBook();
    emit goBookEditView(currow);
}

