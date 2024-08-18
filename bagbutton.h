#ifndef BAGBUTTON_H
#define BAGBUTTON_H
#include <QApplication>
#include <QPushButton>
#include <QIcon>
#include <QPainter>
class BagButton : public QPushButton {
public:
    bool ClearText;
    QIcon* icon_;
    size_t id;
    BagButton(QWidget* parent = nullptr);
    void setIcon(QIcon*icon,int id=0);
protected:
    void paintEvent(QPaintEvent* event) override;

};
#endif // BAGBUTTON_H
