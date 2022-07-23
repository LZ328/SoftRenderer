/********************************************************************************
** Form generated from reading UI file 'RenderWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERWIDGET_H
#define UI_RENDERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RenderWidgetClass
{
public:
    QTabWidget *tabWidget;

    void setupUi(QWidget *RenderWidgetClass)
    {
        if (RenderWidgetClass->objectName().isEmpty())
            RenderWidgetClass->setObjectName(QString::fromUtf8("RenderWidgetClass"));
        RenderWidgetClass->resize(1200, 600);
        RenderWidgetClass->setMinimumSize(QSize(1200, 600));
        RenderWidgetClass->setMaximumSize(QSize(1200, 600));
        RenderWidgetClass->setFocusPolicy(Qt::StrongFocus);
        tabWidget = new QTabWidget(RenderWidgetClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(800, 0, 400, 600));
        tabWidget->setMinimumSize(QSize(400, 600));
        tabWidget->setMaximumSize(QSize(400, 600));

        retranslateUi(RenderWidgetClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(RenderWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *RenderWidgetClass)
    {
        RenderWidgetClass->setWindowTitle(QApplication::translate("RenderWidgetClass", "SoftRenderQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RenderWidgetClass: public Ui_RenderWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERWIDGET_H
