/********************************************************************************
** Form generated from reading UI file 'bag.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BAG_H
#define UI_BAG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_Bag
{
public:
    QTableWidget *bag;

    void setupUi(QFrame *Bag)
    {
        if (Bag->objectName().isEmpty())
            Bag->setObjectName(QStringLiteral("Bag"));
        Bag->resize(270, 206);
        Bag->setFrameShape(QFrame::StyledPanel);
        Bag->setFrameShadow(QFrame::Raised);
        bag = new QTableWidget(Bag);
        bag->setObjectName(QStringLiteral("bag"));
        bag->setGeometry(QRect(7, 7, 256, 192));
        bag->horizontalHeader()->setVisible(false);
        bag->verticalHeader()->setVisible(false);

        retranslateUi(Bag);

        QMetaObject::connectSlotsByName(Bag);
    } // setupUi

    void retranslateUi(QFrame *Bag)
    {
        Bag->setWindowTitle(QApplication::translate("Bag", "Frame", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Bag: public Ui_Bag {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BAG_H
