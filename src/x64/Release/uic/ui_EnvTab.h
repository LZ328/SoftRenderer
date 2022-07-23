/********************************************************************************
** Form generated from reading UI file 'EnvTab.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENVTAB_H
#define UI_ENVTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EnvTab
{
public:
    QDoubleSpinBox *dirX;
    QLabel *label;
    QDoubleSpinBox *dirY;
    QDoubleSpinBox *dirZ;
    QLabel *label_2;
    QDoubleSpinBox *lcR;
    QDoubleSpinBox *lcB;
    QDoubleSpinBox *lcG;
    QLabel *label_3;
    QDoubleSpinBox *acR;
    QDoubleSpinBox *acB;
    QDoubleSpinBox *acG;
    QCheckBox *skyboxCB;
    QCheckBox *shadowCB;
    QListView *skyboxList;

    void setupUi(QWidget *EnvTab)
    {
        if (EnvTab->objectName().isEmpty())
            EnvTab->setObjectName(QString::fromUtf8("EnvTab"));
        EnvTab->resize(400, 600);
        EnvTab->setMinimumSize(QSize(400, 600));
        EnvTab->setMaximumSize(QSize(400, 600));
        EnvTab->setFocusPolicy(Qt::StrongFocus);
        dirX = new QDoubleSpinBox(EnvTab);
        dirX->setObjectName(QString::fromUtf8("dirX"));
        dirX->setGeometry(QRect(50, 60, 60, 22));
        dirX->setDecimals(0);
        dirX->setMinimum(-180.000000000000000);
        dirX->setMaximum(180.000000000000000);
        dirX->setSingleStep(1.000000000000000);
        label = new QLabel(EnvTab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 75, 15));
        dirY = new QDoubleSpinBox(EnvTab);
        dirY->setObjectName(QString::fromUtf8("dirY"));
        dirY->setGeometry(QRect(170, 60, 60, 22));
        dirY->setDecimals(0);
        dirY->setMinimum(-180.000000000000000);
        dirY->setMaximum(180.000000000000000);
        dirY->setSingleStep(1.000000000000000);
        dirZ = new QDoubleSpinBox(EnvTab);
        dirZ->setObjectName(QString::fromUtf8("dirZ"));
        dirZ->setGeometry(QRect(290, 60, 60, 22));
        dirZ->setDecimals(0);
        dirZ->setMinimum(-180.000000000000000);
        dirZ->setMaximum(180.000000000000000);
        dirZ->setSingleStep(1.000000000000000);
        label_2 = new QLabel(EnvTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 110, 75, 15));
        lcR = new QDoubleSpinBox(EnvTab);
        lcR->setObjectName(QString::fromUtf8("lcR"));
        lcR->setGeometry(QRect(50, 150, 60, 22));
        lcR->setDecimals(0);
        lcR->setMaximum(255.000000000000000);
        lcR->setValue(255.000000000000000);
        lcB = new QDoubleSpinBox(EnvTab);
        lcB->setObjectName(QString::fromUtf8("lcB"));
        lcB->setGeometry(QRect(290, 150, 60, 22));
        lcB->setDecimals(0);
        lcB->setMaximum(255.000000000000000);
        lcB->setValue(255.000000000000000);
        lcG = new QDoubleSpinBox(EnvTab);
        lcG->setObjectName(QString::fromUtf8("lcG"));
        lcG->setGeometry(QRect(170, 150, 60, 22));
        lcG->setDecimals(0);
        lcG->setMaximum(255.000000000000000);
        lcG->setValue(255.000000000000000);
        label_3 = new QLabel(EnvTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 200, 81, 16));
        acR = new QDoubleSpinBox(EnvTab);
        acR->setObjectName(QString::fromUtf8("acR"));
        acR->setGeometry(QRect(50, 240, 60, 22));
        acR->setDecimals(0);
        acR->setMaximum(255.000000000000000);
        acR->setValue(255.000000000000000);
        acB = new QDoubleSpinBox(EnvTab);
        acB->setObjectName(QString::fromUtf8("acB"));
        acB->setGeometry(QRect(290, 240, 60, 22));
        acB->setDecimals(0);
        acB->setMaximum(255.000000000000000);
        acB->setValue(255.000000000000000);
        acG = new QDoubleSpinBox(EnvTab);
        acG->setObjectName(QString::fromUtf8("acG"));
        acG->setGeometry(QRect(170, 240, 60, 22));
        acG->setDecimals(0);
        acG->setMaximum(255.000000000000000);
        acG->setValue(255.000000000000000);
        skyboxCB = new QCheckBox(EnvTab);
        skyboxCB->setObjectName(QString::fromUtf8("skyboxCB"));
        skyboxCB->setGeometry(QRect(30, 330, 91, 19));
        shadowCB = new QCheckBox(EnvTab);
        shadowCB->setObjectName(QString::fromUtf8("shadowCB"));
        shadowCB->setGeometry(QRect(30, 290, 91, 19));
        shadowCB->setChecked(true);
        skyboxList = new QListView(EnvTab);
        skyboxList->setObjectName(QString::fromUtf8("skyboxList"));
        skyboxList->setGeometry(QRect(30, 370, 341, 191));

        retranslateUi(EnvTab);

        QMetaObject::connectSlotsByName(EnvTab);
    } // setupUi

    void retranslateUi(QWidget *EnvTab)
    {
        EnvTab->setWindowTitle(QApplication::translate("EnvTab", "EnvTab", nullptr));
        label->setText(QApplication::translate("EnvTab", "\345\271\263\350\241\214\345\205\211\346\226\271\345\220\221", nullptr));
        label_2->setText(QApplication::translate("EnvTab", "\345\271\263\350\241\214\345\205\211\351\242\234\350\211\262", nullptr));
        label_3->setText(QApplication::translate("EnvTab", "\347\216\257\345\242\203\345\205\211\351\242\234\350\211\262", nullptr));
        skyboxCB->setText(QApplication::translate("EnvTab", "\345\244\251\347\251\272\347\233\222", nullptr));
        shadowCB->setText(QApplication::translate("EnvTab", "\346\270\262\346\237\223\351\230\264\345\275\261", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EnvTab: public Ui_EnvTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENVTAB_H
