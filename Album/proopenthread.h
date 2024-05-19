#ifndef PROOPENTHREAD_H
#define PROOPENTHREAD_H

#include <QThread>
#include <QTreeWidget>
class proOpenThread : public QThread
{
    Q_OBJECT
public:
    explicit proOpenThread(const QString&src_path, int &file_count,
                            QTreeWidget* self,QObject *parent = nullptr);
    void OpenProTree(const QString &src_path,int &file_count, QTreeWidget *self);

protected:
    void run() override;


private:
    void RecursiveProTree(const QString &src_path,
                          int &file_count, QTreeWidget *self,
                          QTreeWidgetItem* root, QTreeWidgetItem* parent, QTreeWidgetItem* preitem);
    QString _srcPath;
    int _fileCount;
    QTreeWidget* _Right;
    bool _threadStop;
    QTreeWidgetItem* _root;
signals:
    void sigFinishProgress(int);
    void sigUpdateProgress(int);
};

#endif // PROOPENTHREAD_H
