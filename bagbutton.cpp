#include "bagbutton.h"
BagButton::BagButton(QWidget* parent):QPushButton(parent)
{
    ClearText=0;
    icon_ = 0;
    id = 0;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void BagButton::setIcon(QIcon*icon,int id_)
{
    icon_ = icon;
    id = id_;
}


 void BagButton::paintEvent(QPaintEvent* event) {
     if(!ClearText)
     {
         ClearText=1;
         setText("");
     }
    QPushButton::paintEvent(event);
    if (icon_ == 0)return;
    // 获取按钮的矩形区域
    QRect rect = this->rect();
    // 创建绘制器
    QPainter painter(this);
    // 将图标绘制到按钮中，使其填充整个按钮
   icon_->paint(&painter, rect, Qt::AlignCenter, QIcon::Normal, QIcon::On);
}
