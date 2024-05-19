#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include <QEvent>
#include <QPixmap>
class picButton : public QPushButton
{
public:
    picButton(QWidget* parent = nullptr);
    void SetIcons(const QString& normal,const QString& hover, const QString& pressed);
protected:
    bool event(QEvent *event) override;
private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressedIcon();
    QString _normal;
    QString _hover;
    QString _pressed;
};

#endif // PICBUTTON_H
