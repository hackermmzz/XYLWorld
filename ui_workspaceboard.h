/********************************************************************************
** Form generated from reading UI file 'workspaceboard.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORKSPACEBOARD_H
#define UI_WORKSPACEBOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include "bagbutton.h"

QT_BEGIN_NAMESPACE

class Ui_WorkSpaceBoard
{
public:
    QVBoxLayout *verticalLayout_2;
    QGridLayout *Grid;
    BagButton *source0;
    BagButton *source3;
    QRadioButton *exit;
    BagButton *source6;
    BagButton *source7;
    BagButton *source2;
    QVBoxLayout *verticalLayout;
    BagButton *target;
    QLabel *number;
    BagButton *source4;
    BagButton *source8;
    BagButton *source5;
    BagButton *source1;
    QTableWidget *Bag;

    void setupUi(QFrame *WorkSpaceBoard)
    {
        if (WorkSpaceBoard->objectName().isEmpty())
            WorkSpaceBoard->setObjectName(QStringLiteral("WorkSpaceBoard"));
        WorkSpaceBoard->resize(441, 318);
        WorkSpaceBoard->setFrameShape(QFrame::StyledPanel);
        WorkSpaceBoard->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(WorkSpaceBoard);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        Grid = new QGridLayout();
        Grid->setObjectName(QStringLiteral("Grid"));
        source0 = new BagButton(WorkSpaceBoard);
        source0->setObjectName(QStringLiteral("source0"));

        Grid->addWidget(source0, 0, 0, 1, 1);

        source3 = new BagButton(WorkSpaceBoard);
        source3->setObjectName(QStringLiteral("source3"));

        Grid->addWidget(source3, 1, 0, 1, 1);

        exit = new QRadioButton(WorkSpaceBoard);
        exit->setObjectName(QStringLiteral("exit"));

        Grid->addWidget(exit, 0, 3, 1, 1);

        source6 = new BagButton(WorkSpaceBoard);
        source6->setObjectName(QStringLiteral("source6"));

        Grid->addWidget(source6, 2, 0, 1, 1);

        source7 = new BagButton(WorkSpaceBoard);
        source7->setObjectName(QStringLiteral("source7"));

        Grid->addWidget(source7, 2, 1, 1, 1);

        source2 = new BagButton(WorkSpaceBoard);
        source2->setObjectName(QStringLiteral("source2"));

        Grid->addWidget(source2, 0, 2, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        target = new BagButton(WorkSpaceBoard);
        target->setObjectName(QStringLiteral("target"));

        verticalLayout->addWidget(target);

        number = new QLabel(WorkSpaceBoard);
        number->setObjectName(QStringLiteral("number"));

        verticalLayout->addWidget(number);


        Grid->addLayout(verticalLayout, 1, 3, 1, 1);

        source4 = new BagButton(WorkSpaceBoard);
        source4->setObjectName(QStringLiteral("source4"));

        Grid->addWidget(source4, 1, 1, 1, 1);

        source8 = new BagButton(WorkSpaceBoard);
        source8->setObjectName(QStringLiteral("source8"));

        Grid->addWidget(source8, 2, 2, 1, 1);

        source5 = new BagButton(WorkSpaceBoard);
        source5->setObjectName(QStringLiteral("source5"));

        Grid->addWidget(source5, 1, 2, 1, 1);

        source1 = new BagButton(WorkSpaceBoard);
        source1->setObjectName(QStringLiteral("source1"));

        Grid->addWidget(source1, 0, 1, 1, 1);


        verticalLayout_2->addLayout(Grid);

        Bag = new QTableWidget(WorkSpaceBoard);
        Bag->setObjectName(QStringLiteral("Bag"));
        Bag->setRowCount(0);
        Bag->setColumnCount(0);

        verticalLayout_2->addWidget(Bag);


        retranslateUi(WorkSpaceBoard);

        QMetaObject::connectSlotsByName(WorkSpaceBoard);
    } // setupUi

    void retranslateUi(QFrame *WorkSpaceBoard)
    {
        WorkSpaceBoard->setWindowTitle(QApplication::translate("WorkSpaceBoard", "Frame", Q_NULLPTR));
        source0->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        source3->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        exit->setText(QApplication::translate("WorkSpaceBoard", "exit", Q_NULLPTR));
        source6->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        source7->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        source2->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        target->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        number->setText(QApplication::translate("WorkSpaceBoard", "Nums:0", Q_NULLPTR));
        source4->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        source8->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        source5->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
        source1->setText(QApplication::translate("WorkSpaceBoard", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WorkSpaceBoard: public Ui_WorkSpaceBoard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKSPACEBOARD_H
