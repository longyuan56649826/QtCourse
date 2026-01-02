#ifndef BOOKEDITVIEW_H
#define BOOKEDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class BookEditView;
}

class BookEditView : public QWidget
{
    Q_OBJECT

public:
    explicit BookEditView(QWidget *parent = nullptr,int index=0);
    ~BookEditView();

private slots:
    void on_btEdit_clicked();

    void on_btCancel_clicked();

private:
    Ui::BookEditView *ui;

    QDataWidgetMapper *dataMapper;

signals:
    void goBack();
};

#endif // BOOKEDITVIEW_H
