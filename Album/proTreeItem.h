#ifndef PROTREEITEM_H
#define PROTREEITEM_H
#include <QTreeWidget>
#include <QTreeWidgetItem>
class proTreeItem :public QTreeWidgetItem
{
public:
    proTreeItem(QTreeWidget *view, const QString &name,
                const QString &path, int type = Type);
    proTreeItem(QTreeWidgetItem *parent, const QString &name,
                      const QString &path, QTreeWidgetItem* root,int type = Type);

    const QString &getPath();
    QTreeWidgetItem *getRoot();
    void setPreItem(QTreeWidgetItem *item);
    void setNextItem(QTreeWidgetItem *item);
    proTreeItem *getPreItem();
    proTreeItem *getNextItem();
    proTreeItem *getLastPicChild();
    proTreeItem *getFirstPicChild();

private:
    QString _path;
    QString _name;
    QTreeWidgetItem *_root ;
    QTreeWidgetItem *_preItem;
    QTreeWidgetItem *_nextItem;
};

#endif // PROTREEITEM_H
