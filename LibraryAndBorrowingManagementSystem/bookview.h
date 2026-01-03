#ifndef BOOKVIEW_H
#define BOOKVIEW_H

#include <QWidget>

namespace Ui {
class BookView;
}

class BookView : public QWidget
{
    Q_OBJECT

public:
    explicit BookView(QWidget *parent = nullptr);
    ~BookView();

private:
    Ui::BookView *ui;
    QThread *searchThread;

signals:
    void goBack();
    void goBookEditView(int index);
private slots:
    void on_btBack_clicked();
    void on_btAdd_clicked();
    void on_btDelete_clicked();
    void on_btEdit_clicked();
    void on_btSearch_clicked();
};

#endif // BOOKVIEW_H
