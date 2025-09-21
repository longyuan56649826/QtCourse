#ifndef SAMP2_4_H
#define SAMP2_4_H

#include <QWidget>

namespace Ui {
class samp2_4;
}

class samp2_4 : public QWidget
{
    Q_OBJECT

public:
    explicit samp2_4(QWidget *parent = nullptr);
    ~samp2_4();

private slots:
    void on_pushButton_clicked();

private:
    Ui::samp2_4 *ui;
};

#endif // SAMP2_4_H
