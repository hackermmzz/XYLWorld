/********************************************************************************
** Form generated from reading UI file 'itemboard.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ITEMBOARD_H
#define UI_ITEMBOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include "bagbutton.h"

QT_BEGIN_NAMESPACE

class Ui_ItemBoard
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    BagButton *item0;
    QLabel *num0;
    QVBoxLayout *verticalLayout_2;
    BagButton *item1;
    QLabel *num1;
    QVBoxLayout *verticalLayout_3;
    BagButton *item2;
    QLabel *num2;
    QVBoxLayout *verticalLayout_4;
    BagButton *item3;
    QLabel *num3;
    QVBoxLayout *verticalLayout_5;
    BagButton *item4;
    QLabel *num4;
    QVBoxLayout *verticalLayout_6;
    BagButton *item5;
    QLabel *num5;
    QVBoxLayout *verticalLayout_7;
    BagButton *item6;
    QLabel *num6;
    QVBoxLayout *verticalLayout_8;
    BagButton *item7;
    QLabel *num7;
    QVBoxLayout *verticalLayout_9;
    BagButton *item8;
    QLabel *num8;

    void setupUi(QFrame *ItemBoard)
    {
        if (ItemBoard->objectName().isEmpty())
            ItemBoard->setObjectName(QStringLiteral("ItemBoard"));
        ItemBoard->resize(770, 34);
        ItemBoard->setFrameShape(QFrame::StyledPanel);
        ItemBoard->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(ItemBoard);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        item0 = new BagButton(ItemBoard);
        item0->setObjectName(QStringLiteral("item0"));

        verticalLayout->addWidget(item0);

        num0 = new QLabel(ItemBoard);
        num0->setObjectName(QStringLiteral("num0"));

        verticalLayout->addWidget(num0);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        item1 = new BagButton(ItemBoard);
        item1->setObjectName(QStringLiteral("item1"));

        verticalLayout_2->addWidget(item1);

        num1 = new QLabel(ItemBoard);
        num1->setObjectName(QStringLiteral("num1"));

        verticalLayout_2->addWidget(num1);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        item2 = new BagButton(ItemBoard);
        item2->setObjectName(QStringLiteral("item2"));

        verticalLayout_3->addWidget(item2);

        num2 = new QLabel(ItemBoard);
        num2->setObjectName(QStringLiteral("num2"));

        verticalLayout_3->addWidget(num2);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        item3 = new BagButton(ItemBoard);
        item3->setObjectName(QStringLiteral("item3"));

        verticalLayout_4->addWidget(item3);

        num3 = new QLabel(ItemBoard);
        num3->setObjectName(QStringLiteral("num3"));

        verticalLayout_4->addWidget(num3);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        item4 = new BagButton(ItemBoard);
        item4->setObjectName(QStringLiteral("item4"));

        verticalLayout_5->addWidget(item4);

        num4 = new QLabel(ItemBoard);
        num4->setObjectName(QStringLiteral("num4"));

        verticalLayout_5->addWidget(num4);


        horizontalLayout->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        item5 = new BagButton(ItemBoard);
        item5->setObjectName(QStringLiteral("item5"));

        verticalLayout_6->addWidget(item5);

        num5 = new QLabel(ItemBoard);
        num5->setObjectName(QStringLiteral("num5"));

        verticalLayout_6->addWidget(num5);


        horizontalLayout->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        item6 = new BagButton(ItemBoard);
        item6->setObjectName(QStringLiteral("item6"));

        verticalLayout_7->addWidget(item6);

        num6 = new QLabel(ItemBoard);
        num6->setObjectName(QStringLiteral("num6"));

        verticalLayout_7->addWidget(num6);


        horizontalLayout->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        item7 = new BagButton(ItemBoard);
        item7->setObjectName(QStringLiteral("item7"));

        verticalLayout_8->addWidget(item7);

        num7 = new QLabel(ItemBoard);
        num7->setObjectName(QStringLiteral("num7"));

        verticalLayout_8->addWidget(num7);


        horizontalLayout->addLayout(verticalLayout_8);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        item8 = new BagButton(ItemBoard);
        item8->setObjectName(QStringLiteral("item8"));

        verticalLayout_9->addWidget(item8);

        num8 = new QLabel(ItemBoard);
        num8->setObjectName(QStringLiteral("num8"));

        verticalLayout_9->addWidget(num8);


        horizontalLayout->addLayout(verticalLayout_9);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(ItemBoard);

        QMetaObject::connectSlotsByName(ItemBoard);
    } // setupUi

    void retranslateUi(QFrame *ItemBoard)
    {
        ItemBoard->setWindowTitle(QApplication::translate("ItemBoard", "Frame", Q_NULLPTR));
        item0->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num0->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
        item1->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num1->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
        item2->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num2->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
        item3->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num3->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
        item4->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num4->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
        item5->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num5->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
        item6->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num6->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
        item7->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num7->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
        item8->setText(QApplication::translate("ItemBoard", "PushButton", Q_NULLPTR));
        num8->setText(QApplication::translate("ItemBoard", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ItemBoard: public Ui_ItemBoard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ITEMBOARD_H
