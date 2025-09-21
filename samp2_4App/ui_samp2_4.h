/********************************************************************************
** Form generated from reading UI file 'samp2_4.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMP2_4_H
#define UI_SAMP2_4_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_samp2_4
{
public:
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *samp2_4)
    {
        if (samp2_4->objectName().isEmpty())
            samp2_4->setObjectName("samp2_4");
        samp2_4->resize(400, 300);
        QFont font;
        font.setPointSize(15);
        samp2_4->setFont(font);
        label = new QLabel(samp2_4);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 120, 321, 61));
        QFont font1;
        font1.setPointSize(15);
        font1.setBold(true);
        label->setFont(font1);
        pushButton = new QPushButton(samp2_4);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(300, 250, 81, 31));

        retranslateUi(samp2_4);

        QMetaObject::connectSlotsByName(samp2_4);
    } // setupUi

    void retranslateUi(QWidget *samp2_4)
    {
        samp2_4->setWindowTitle(QCoreApplication::translate("samp2_4", "samp2_4", nullptr));
        label->setText(QCoreApplication::translate("samp2_4", "\345\274\200\345\217\221\344\272\272\345\221\230\345\247\223\345\220\215\357\274\232\344\275\225\345\256\207\346\266\265\n"
"\345\274\200\345\217\221\344\272\272\345\221\230\345\255\246\345\217\267\357\274\2322023414290109", nullptr));
        pushButton->setText(QCoreApplication::translate("samp2_4", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class samp2_4: public Ui_samp2_4 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMP2_4_H
