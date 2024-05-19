#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>

namespace Ui {
class picShow;
}

class picShow : public QDialog
{
    Q_OBJECT
public:
    explicit picShow(QWidget *parent = nullptr);
    ~picShow();


private:
    Ui::picShow *ui;
    QString _selectedPath;
    QPixmap _pixMap;
public slots:
    void SlotSelectItem(const QString& path);
    void SlotUpdatePic(const QString& path);
    void SlotDeleteItem();
    void SlotClearPic();
signals:
    void SigNextClicked();
    void SigPreClicked();
};

#endif // PICSHOW_H
