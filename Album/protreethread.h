#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class proTreeThread : public QThread
{
    Q_OBJECT
public:
    proTreeThread(const QString &src_path, const QString &dist_path,
                  QTreeWidgetItem *parent_item, int &file_count,
                  QTreeWidget *self, QTreeWidgetItem *root, QObject *parent=nullptr);
    ~proTreeThread();

protected:
    virtual void run();
private:
    void createProTree(const QString &src_path, const QString &dist_path,
                       QTreeWidgetItem *parent_item, int &file_count,
                       QTreeWidget *self, QTreeWidgetItem *root,QTreeWidgetItem* preItem=nullptr);
    QString _srcPath;//当前路径
    QString _distPath;//目标路径
    QTreeWidgetItem *_parentItem;//当前节点的父节点
    QTreeWidgetItem *_root;//根节点
    QTreeWidget *_left;//所属的widget
    int fileCount;//文件数量
    bool _threadStop;//控制线程最初状态的变量

signals:
    void sigUpdateProgress(int);
    void sigFinishProgress(int);
public slots:
    void SlotEndThread();
};

#endif // PROTREETHREAD_H
