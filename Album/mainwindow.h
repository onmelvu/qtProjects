#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *_proTree;
    QWidget *_picShow;
private slots:
    void slotCreatPro(bool );
    void slotOpenPro(bool );

signals:
    void sigOpenPro(QString path);
};
#endif // MAINWINDOW_H
