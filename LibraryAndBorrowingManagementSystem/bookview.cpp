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
    IDatabase::getInstance().searchBook(filter);
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

