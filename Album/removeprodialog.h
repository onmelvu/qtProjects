#ifndef REMOVEPRODIALOG_H
#define REMOVEPRODIALOG_H

#include <QDialog>

namespace Ui {
class removeProDialog;
}

class removeProDialog : public QDialog
{
    Q_OBJECT

public:
    explicit removeProDialog(QWidget *parent = nullptr);
    ~removeProDialog();
     bool IsRemoved();

private:
    Ui::removeProDialog *ui;

signals:
    void closePro();

};

#endif // REMOVEPRODIALOG_H
