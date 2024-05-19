#include "proopenthread.h"
#include <QDir>
#include "proTreeItem.h"
#include "const.h"
proOpenThread::proOpenThread(const QString&src_path, int &file_count,
                           QTreeWidget* self,QObject *parent ) :
    QThread (parent),_srcPath(src_path),_fileCount(file_count),
    _Right(self),_threadStop(false),_root(nullptr)
{

}

void proOpenThread::OpenProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    //创建根节点
    QDir dir(src_path);
    QString name = dir.dirName();
    proTreeItem *item = new proTreeItem(self,name,src_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,src_path);
    _root=item;//将item设为根

    RecursiveProTree(src_path,file_count,self,_root,item,nullptr);

}

void proOpenThread::run()
{
    OpenProTree(_srcPath,_fileCount,_Right);
    if(_threadStop&&_root){//如果线程停止并且创建了根节点，需要删除根节点
        QString path = dynamic_cast<proTreeItem*>(_root)->getPath();
        int index = _Right->indexOfTopLevelItem(_root);
        delete _Right->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }
    emit sigFinishProgress(_fileCount);
}

void proOpenThread::RecursiveProTree(const QString &src_path, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *preitem)
{
    //与创建项目步骤相同
    QDir srcDir(src_path);
    QStringList listNames;
    srcDir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    srcDir.setSorting(QDir::Name);//优先显示名字
    QFileInfoList list = srcDir.entryInfoList();
    for(int i=0;i<list.size();i++){
        if(_threadStop){
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool isDir = fileInfo.isDir();
        if(isDir){
            if(_threadStop){
                return;
            }
            file_count++;
            emit sigUpdateProgress(file_count);
            proTreeItem *item = new proTreeItem(parent,fileInfo.fileName(),fileInfo.absoluteFilePath(),root,TreeItemPic);//设置item
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());
            RecursiveProTree(fileInfo.absoluteFilePath(),file_count,self,root,item,preitem);//递归创建目录

        }else{
            if(_threadStop){
                return;
            }
            QString suffix = fileInfo.completeSuffix();
            if(suffix!= "png"&&suffix!="jpeg"&&suffix!="jpg"){
                continue;//跳过该文件
            }
            file_count++;
            emit sigUpdateProgress(file_count);
            proTreeItem *item = new proTreeItem(parent,fileInfo.fileName(),fileInfo.absoluteFilePath(),root,TreeItemPic);//设置item
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());
            if(preitem){
                proTreeItem* pre_proitem = dynamic_cast<proTreeItem*>(preitem);
                pre_proitem->setNextItem(item);
            }
            item->setPreItem(preitem);
            preitem=item;
        }
    }
    emit sigFinishProgress(file_count);
}
