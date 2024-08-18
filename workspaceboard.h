#ifndef WORKSPACEBOARD_H
#define WORKSPACEBOARD_H

#include <QFrame>

namespace Ui {
class WorkSpaceBoard;
}

class WorkSpaceBoard : public QFrame
{
    Q_OBJECT

public:
    explicit WorkSpaceBoard(QWidget *parent = 0);
    ~WorkSpaceBoard();

private:
    Ui::WorkSpaceBoard *ui;
};

#endif // WORKSPACEBOARD_H
