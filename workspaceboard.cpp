#include "workspaceboard.h"
#include "ui_workspaceboard.h"

WorkSpaceBoard::WorkSpaceBoard(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WorkSpaceBoard)
{
    ui->setupUi(this);
}

WorkSpaceBoard::~WorkSpaceBoard()
{
    delete ui;
}
