/********************************************************************************
** Form generated from reading UI file 'ModelTab.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELTAB_H
#define UI_MODELTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelTab
{
public:
    QPushButton *openFileButton;
    QListView *mdlListView;
    QPushButton *removeButton;
    QCheckBox *rdMdCB;
    QDoubleSpinBox *posXSpin;
    QDoubleSpinBox *posYSpin;
    QDoubleSpinBox *posZSpin;
    QDoubleSpinBox *scaleSpin;
    QDoubleSpinBox *rotXSpin;
    QDoubleSpinBox *rotYSpin;
    QDoubleSpinBox *rotZSpin;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *matButton;
    QPushButton *ResetButton;

    void setupUi(QWidget *ModelTab)
    {
        if (ModelTab->objectName().isEmpty())
            ModelTab->setObjectName(QString::fromUtf8("ModelTab"));
        ModelTab->resize(400, 600);
        ModelTab->setMinimumSize(QSize(400, 600));
        ModelTab->setMaximumSize(QSize(400, 600));
        ModelTab->setFocusPolicy(Qt::StrongFocus);
        openFileButton = new QPushButton(ModelTab);
        openFileButton->setObjectName(QString::fromUtf8("openFileButton"));
        openFileButton->setGeometry(QRect(50, 30, 80, 28));
        mdlListView = new QListView(ModelTab);
        mdlListView->setObjectName(QString::fromUtf8("mdlListView"));
        mdlListView->setGeometry(QRect(47, 80, 311, 191));
        removeButton = new QPushButton(ModelTab);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        removeButton->setGeometry(QRect(159, 30, 80, 28));
        rdMdCB = new QCheckBox(ModelTab);
        rdMdCB->setObjectName(QString::fromUtf8("rdMdCB"));
        rdMdCB->setGeometry(QRect(50, 520, 91, 19));
        posXSpin = new QDoubleSpinBox(ModelTab);
        posXSpin->setObjectName(QString::fromUtf8("posXSpin"));
        posXSpin->setGeometry(QRect(50, 330, 60, 20));
        posXSpin->setFocusPolicy(Qt::WheelFocus);
        posXSpin->setKeyboardTracking(true);
        posXSpin->setMinimum(-99.000000000000000);
        posXSpin->setMaximum(99.000000000000000);
        posXSpin->setSingleStep(0.100000000000000);
        posXSpin->setValue(0.000000000000000);
        posYSpin = new QDoubleSpinBox(ModelTab);
        posYSpin->setObjectName(QString::fromUtf8("posYSpin"));
        posYSpin->setGeometry(QRect(170, 330, 60, 20));
        posYSpin->setFocusPolicy(Qt::WheelFocus);
        posYSpin->setKeyboardTracking(true);
        posYSpin->setMinimum(-99.000000000000000);
        posYSpin->setMaximum(99.000000000000000);
        posYSpin->setSingleStep(0.100000000000000);
        posYSpin->setValue(0.000000000000000);
        posZSpin = new QDoubleSpinBox(ModelTab);
        posZSpin->setObjectName(QString::fromUtf8("posZSpin"));
        posZSpin->setGeometry(QRect(290, 330, 60, 20));
        posZSpin->setFocusPolicy(Qt::WheelFocus);
        posZSpin->setKeyboardTracking(true);
        posZSpin->setMinimum(-99.000000000000000);
        posZSpin->setMaximum(99.000000000000000);
        posZSpin->setSingleStep(0.100000000000000);
        posZSpin->setValue(0.000000000000000);
        scaleSpin = new QDoubleSpinBox(ModelTab);
        scaleSpin->setObjectName(QString::fromUtf8("scaleSpin"));
        scaleSpin->setGeometry(QRect(50, 470, 60, 20));
        scaleSpin->setFocusPolicy(Qt::WheelFocus);
        scaleSpin->setKeyboardTracking(true);
        scaleSpin->setDecimals(3);
        scaleSpin->setMinimum(0.005000000000000);
        scaleSpin->setSingleStep(0.100000000000000);
        scaleSpin->setValue(1.000000000000000);
        rotXSpin = new QDoubleSpinBox(ModelTab);
        rotXSpin->setObjectName(QString::fromUtf8("rotXSpin"));
        rotXSpin->setGeometry(QRect(50, 400, 60, 20));
        rotXSpin->setFocusPolicy(Qt::WheelFocus);
        rotXSpin->setKeyboardTracking(true);
        rotXSpin->setDecimals(0);
        rotXSpin->setMinimum(-180.000000000000000);
        rotXSpin->setMaximum(180.000000000000000);
        rotXSpin->setSingleStep(5.000000000000000);
        rotXSpin->setValue(0.000000000000000);
        rotYSpin = new QDoubleSpinBox(ModelTab);
        rotYSpin->setObjectName(QString::fromUtf8("rotYSpin"));
        rotYSpin->setGeometry(QRect(170, 400, 60, 20));
        rotYSpin->setFocusPolicy(Qt::WheelFocus);
        rotYSpin->setKeyboardTracking(true);
        rotYSpin->setDecimals(0);
        rotYSpin->setMinimum(-180.000000000000000);
        rotYSpin->setMaximum(180.000000000000000);
        rotYSpin->setSingleStep(5.000000000000000);
        rotYSpin->setValue(0.000000000000000);
        rotZSpin = new QDoubleSpinBox(ModelTab);
        rotZSpin->setObjectName(QString::fromUtf8("rotZSpin"));
        rotZSpin->setGeometry(QRect(290, 400, 60, 20));
        rotZSpin->setFocusPolicy(Qt::WheelFocus);
        rotZSpin->setKeyboardTracking(true);
        rotZSpin->setDecimals(0);
        rotZSpin->setMinimum(-180.000000000000000);
        rotZSpin->setMaximum(180.000000000000000);
        rotZSpin->setSingleStep(5.000000000000000);
        rotZSpin->setValue(0.000000000000000);
        label = new QLabel(ModelTab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 300, 72, 15));
        label_2 = new QLabel(ModelTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 370, 72, 15));
        label_3 = new QLabel(ModelTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 440, 72, 15));
        matButton = new QPushButton(ModelTab);
        matButton->setObjectName(QString::fromUtf8("matButton"));
        matButton->setGeometry(QRect(265, 30, 93, 28));
        ResetButton = new QPushButton(ModelTab);
        ResetButton->setObjectName(QString::fromUtf8("ResetButton"));
        ResetButton->setGeometry(QRect(159, 515, 93, 28));

        retranslateUi(ModelTab);

        QMetaObject::connectSlotsByName(ModelTab);
    } // setupUi

    void retranslateUi(QWidget *ModelTab)
    {
        ModelTab->setWindowTitle(QApplication::translate("ModelTab", "\346\250\241\345\236\213\350\256\276\347\275\256", nullptr));
        openFileButton->setText(QApplication::translate("ModelTab", "\345\212\240\350\275\275\346\250\241\345\236\213", nullptr));
        removeButton->setText(QApplication::translate("ModelTab", "\345\210\240\351\231\244\346\250\241\345\236\213", nullptr));
        rdMdCB->setText(QApplication::translate("ModelTab", "\347\275\221\346\240\274\345\214\226", nullptr));
        label->setText(QApplication::translate("ModelTab", "\344\275\215\347\275\256", nullptr));
        label_2->setText(QApplication::translate("ModelTab", "\346\227\213\350\275\254", nullptr));
        label_3->setText(QApplication::translate("ModelTab", "\347\274\251\346\224\276", nullptr));
        matButton->setText(QApplication::translate("ModelTab", "\347\274\226\350\276\221\346\235\220\350\264\250", nullptr));
        ResetButton->setText(QApplication::translate("ModelTab", "\351\207\215\347\275\256\350\247\206\350\247\222", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelTab: public Ui_ModelTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELTAB_H
