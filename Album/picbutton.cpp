#include "picbutton.h"

picButton::picButton(QWidget *parent) {}

void picButton::SetIcons(const QString &normal, const QString &hover, const QString &pressed)
{
    _normal = normal;
    _hover = hover;
    _pressed = pressed;
    QPixmap pixMap;
    pixMap.load(normal);
    this->resize(pixMap.size());
    this->setIcon(pixMap);
    this->setIconSize(pixMap.size());
}

bool picButton::event(QEvent *event)
{
    switch(event->type()){
    case QEvent::Enter:
        setHoverIcon();
        break;
    case QEvent::Leave:
        setNormalIcon();
        break;
    case QEvent::MouseButtonPress:
        setPressedIcon();
        break;
    case QEvent::MouseButtonRelease:
        setHoverIcon();
        break;
    default:
        break;
    }
    return QPushButton::event(event);

}

void picButton::setNormalIcon()
{
    QPixmap pixMap;
    pixMap.load(_normal);
    this->setIcon(pixMap);
}

void picButton::setHoverIcon()
{
    QPixmap pixMap;
    pixMap.load(_hover);
    this->setIcon(pixMap);
}

void picButton::setPressedIcon()
{
    QPixmap pixMap;
    pixMap.load(_pressed);
    this->setIcon(pixMap);
}
