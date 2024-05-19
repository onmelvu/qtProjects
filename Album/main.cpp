
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss(":/style/style.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("qss open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("qss open failed");
    }
    MainWindow w;
    w.setWindowTitle("Ablum");
    //w.showMaximized();
    w.show();
    return a.exec();
}
