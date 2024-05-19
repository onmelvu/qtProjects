#include "protreethread.h"
#include <QDir>
#include "proTreeItem.h"
#include "const.h"
proTreeThread::proTreeThread(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QObject *parent):
    QThread(parent),_srcPath(src_path),_distPath(dist_path),_parentItem(parent_item),_root(root),_left(self),fileCount(file_count),_threadStop(false)
{

}

proTreeThread::~proTreeThread()
{

}

void proTreeThread::run()
{
    createProTree(_srcPath,_distPath,_parentItem,fileCount,_left,_root,nullptr);
    if(_threadStop){
        auto path =dynamic_cast<proTreeItem*>(_root)->getPath();
        int index = _left->indexOfTopLevelItem(_root);
        delete _left->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

    emit sigFinishProgress(fileCount);

}

void proTreeThread::createProTree(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self, QTreeWidgetItem *root,QTreeWidgetItem* preItem)
{
    if(_threadStop){
        return;
    }
    qDebug()<<"this is one test";
    bool needCopy =true;//判断是否需要copy
    if(src_path==dist_path){
        needCopy=false;
    }
    QDir importDir(src_path);
    qDebug() << "src_path is " << src_path << "dis_path is " << dist_path;
    QStringList nameFiltters;
    importDir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);//除了目录或文件，其他的都筛选掉
    importDir.setSorting(QDir::Name);//优先显示名字

    QFileInfoList list = importDir.entryInfoList();
    qDebug() << "list.size " << list.size() ;
    for(int i=0;i<list.size();i++){
        if(_threadStop){
            return;
        }
        QFileInfo fileInfo = list.at(i);// 遍历文件

        bool fileIsDir = fileInfo.isDir();
        if(fileIsDir){//如果文件是目录
            if(_threadStop){
                return;
            }
            file_count++;
            emit sigUpdateProgress(file_count);
            QDir distDir(dist_path);
            //构造子目录的路径
            QString subDistPath = distDir.absoluteFilePath(fileInfo.fileName());
            //子目录的目录
            QDir subDistDir(subDistPath);
            //不存在则可以创建
            if(!subDistDir.exists()){
                //创建多级目录
                bool isOk = subDistDir.mkpath(subDistPath);
                if(!isOk){
                    qDebug()<<"subDistDir mkpath failed!";
                    continue;
                }
            }
            auto *item = new proTreeItem(parent_item,fileInfo.fileName(),subDistPath,root,TreeItemDir);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole,subDistPath);

            createProTree(fileInfo.absoluteFilePath(),subDistPath,item,file_count,self,root,preItem);

        }else{
            if(_threadStop){
                return;
            }
            const QString& suffix = fileInfo.completeSuffix();
            if(suffix!="png"&&suffix!="jpeg"&&suffix!="jpg"){
                continue;
            }
            file_count++;
            emit sigUpdateProgress(file_count);

            if(!needCopy){
                return;
            }

            //复制文件到目标路径
            QDir distDir(dist_path);
            QString distFilePath = distDir.absoluteFilePath(fileInfo.fileName());//将目录的地址和目标文件的文件名拼接，形成一个新的目的地址
            if(!QFile::copy(fileInfo.absoluteFilePath(),distFilePath)){
                qDebug()<<"copy files was failed!";
                continue;
            }
            auto* item = new proTreeItem(parent_item,fileInfo.fileName(),distFilePath,root,TreeItemPic);
            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,distFilePath);

            if(preItem){
                proTreeItem* preProItem = dynamic_cast<proTreeItem*>(preItem);
                preProItem->setNextItem(item);
            }
            item->setPreItem(preItem);
            preItem=item;

        }

    }
    parent_item->setExpanded(true);
}

void proTreeThread::SlotEndThread()
{
    this->_threadStop=true;
}
