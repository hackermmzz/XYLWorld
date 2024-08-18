#ifndef ITEMBOARD_H
#define ITEMBOARD_H

#include <QFrame>
#include"ui_itemboard.h"
namespace Ui {
class ItemBoard;
}

class ItemBoard : public QFrame
{
    Q_OBJECT

public:
    explicit ItemBoard(QWidget *parent = 0);
    ~ItemBoard();
    Ui::ItemBoard *ui;
};

#endif // ITEMBOARD_H
