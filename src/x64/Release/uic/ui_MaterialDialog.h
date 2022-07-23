/********************************************************************************
** Form generated from reading UI file 'MaterialDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALDIALOG_H
#define UI_MATERIALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_MaterialDialog
{
public:
    QDoubleSpinBox *gSB;
    QSpinBox *sSBR;
    QLabel *label_5;
    QPushButton *mrB;
    QSpinBox *cSBR;
    QLabel *label;
    QSpinBox *sSBG;
    QSpinBox *cSBG;
    QSpinBox *sSBB;
    QLabel *label_6;
    QSpinBox *cSBB;
    QPushButton *moB;
    QLabel *label_11;
    QLabel *mCav;

    void setupUi(QDialog *MaterialDialog)
    {
        if (MaterialDialog->objectName().isEmpty())
            MaterialDialog->setObjectName(QString::fromUtf8("MaterialDialog"));
        MaterialDialog->resize(222, 407);
        gSB = new QDoubleSpinBox(MaterialDialog);
        gSB->setObjectName(QString::fromUtf8("gSB"));
        gSB->setGeometry(QRect(30, 200, 70, 22));
        gSB->setMinimum(1.000000000000000);
        gSB->setMaximum(32.000000000000000);
        sSBR = new QSpinBox(MaterialDialog);
        sSBR->setObjectName(QString::fromUtf8("sSBR"));
        sSBR->setGeometry(QRect(30, 120, 46, 22));
        sSBR->setMaximum(255);
        label_5 = new QLabel(MaterialDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 90, 100, 16));
        mrB = new QPushButton(MaterialDialog);
        mrB->setObjectName(QString::fromUtf8("mrB"));
        mrB->setGeometry(QRect(150, 330, 50, 28));
        mrB->setFocusPolicy(Qt::ClickFocus);
        cSBR = new QSpinBox(MaterialDialog);
        cSBR->setObjectName(QString::fromUtf8("cSBR"));
        cSBR->setGeometry(QRect(30, 40, 46, 22));
        cSBR->setMaximum(255);
        label = new QLabel(MaterialDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 250, 100, 16));
        sSBG = new QSpinBox(MaterialDialog);
        sSBG->setObjectName(QString::fromUtf8("sSBG"));
        sSBG->setGeometry(QRect(90, 120, 46, 22));
        sSBG->setMaximum(255);
        cSBG = new QSpinBox(MaterialDialog);
        cSBG->setObjectName(QString::fromUtf8("cSBG"));
        cSBG->setGeometry(QRect(90, 40, 46, 22));
        cSBG->setMaximum(255);
        sSBB = new QSpinBox(MaterialDialog);
        sSBB->setObjectName(QString::fromUtf8("sSBB"));
        sSBB->setGeometry(QRect(150, 120, 46, 22));
        sSBB->setMaximum(255);
        label_6 = new QLabel(MaterialDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 10, 100, 16));
        cSBB = new QSpinBox(MaterialDialog);
        cSBB->setObjectName(QString::fromUtf8("cSBB"));
        cSBB->setGeometry(QRect(150, 40, 46, 22));
        cSBB->setMaximum(255);
        moB = new QPushButton(MaterialDialog);
        moB->setObjectName(QString::fromUtf8("moB"));
        moB->setGeometry(QRect(150, 290, 50, 28));
        moB->setFocusPolicy(Qt::ClickFocus);
        label_11 = new QLabel(MaterialDialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(30, 170, 100, 16));
        mCav = new QLabel(MaterialDialog);
        mCav->setObjectName(QString::fromUtf8("mCav"));
        mCav->setGeometry(QRect(30, 280, 100, 100));

        retranslateUi(MaterialDialog);

        QMetaObject::connectSlotsByName(MaterialDialog);
    } // setupUi

    void retranslateUi(QDialog *MaterialDialog)
    {
        MaterialDialog->setWindowTitle(QApplication::translate("MaterialDialog", "\346\235\220\350\264\250\350\256\276\347\275\256", nullptr));
        label_5->setText(QApplication::translate("MaterialDialog", "Specular", nullptr));
        mrB->setText(QApplication::translate("MaterialDialog", "\347\247\273\351\231\244", nullptr));
        label->setText(QApplication::translate("MaterialDialog", "Main Texture", nullptr));
        label_6->setText(QApplication::translate("MaterialDialog", "Albedo", nullptr));
        moB->setText(QApplication::translate("MaterialDialog", "\346\211\223\345\274\200", nullptr));
        label_11->setText(QApplication::translate("MaterialDialog", "Glossy", nullptr));
        mCav->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MaterialDialog: public Ui_MaterialDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALDIALOG_H
