#include "proTreeItem.h"


proTreeItem::proTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type):
    QTreeWidgetItem(view,type),_path(path),_name(name),_root(this),_preItem(nullptr),_nextItem(nullptr)
{

}

proTreeItem::proTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type):
    QTreeWidgetItem(parent,type),_path(path),_name(name),_root(this),_preItem(nullptr),_nextItem(nullptr)
{

}

const QString &proTreeItem::getPath()
{
    return _path;
}

QTreeWidgetItem *proTreeItem::getRoot()
{
    return _root;
}

void proTreeItem::setPreItem(QTreeWidgetItem *item)
{
    _preItem = item;
}

void proTreeItem::setNextItem(QTreeWidgetItem *item)
{
    _nextItem = item;
}

proTreeItem *proTreeItem::getPreItem()
{
    return dynamic_cast<proTreeItem*>(_preItem);
}

proTreeItem *proTreeItem::getNextItem()
{
    return dynamic_cast<proTreeItem*>(_nextItem);
}

proTreeItem *proTreeItem::getLastPicChild()
{

}

proTreeItem *proTreeItem::getFirstPicChild()
{

}


