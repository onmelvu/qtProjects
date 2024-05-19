#include "protreewidget.h"
#include <QDir>
#include "const.h"
#include "proTreeItem.h"
#include <QHeaderView>
#include <QGuiApplication>
#include <QMenu>
#include <QFileDialog>
#include "removeprodialog.h"

proTreeWidget::proTreeWidget(QWidget *parent)
    : QTreeWidget{parent}
{
    qRegisterMetaType<QVector<int> >("QVector<int>");
    //隐藏表头
    this->header()->hide();
    connect(this,&proTreeWidget::itemPressed,this,&proTreeWidget::slotItemPressed);
    actImport = new QAction(QIcon(":/icon/import.png"),tr("导入文件"),this);
    //actSetStart = new QAction(QIcon(":/icon/core.png"),tr("设置活动项目"),this);
    actClosePro = new QAction(QIcon(":/icon/close.png"),tr("关闭项目"),this);
    //actSlideShow = new QAction(QIcon(":/icon/slideshow.png"),tr("轮换播放"),this);
    connect(actImport,&QAction::triggered,this,&proTreeWidget::slotImport);
    connect(this,&proTreeWidget::itemDoubleClicked,this,&proTreeWidget::SlotDoubleClickItem);
    connect(actClosePro,&QAction::triggered,this,&proTreeWidget::SlotClosePro);

}

proTreeWidget::~proTreeWidget()
{

}

void proTreeWidget::slotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::RightButton){
        QMenu menu(this);
        int itemType = pressedItem->type();
        if(itemType==TreeItemPro){
            rightBtnItem=pressedItem;

            menu.addAction(actImport);
            //menu.addAction(actSetStart);
            menu.addAction(actClosePro);
            //menu.addAction(actSlideShow);

            menu.exec(QCursor::pos());
        }
    }
}

void proTreeWidget::slotImport()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("选择导入的文件");
    QString filepath = "";
    if(!rightBtnItem){//判断是否为空
        qDebug()<<"rightBtnItem is empty";
        return;
    }
    filepath = dynamic_cast<proTreeItem*>(rightBtnItem)->getPath();//父子类转换 将rightBtnItem转换为proTreeItem
    fileDialog.setDirectory(filepath);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList listNames;
    if(fileDialog.exec()){
        listNames = fileDialog.selectedFiles();
    }

    if(listNames.length()<=0){
        return;
    }

    QString importPath = listNames.at(0);

    int fileCount = 0;

    dialogProgress = new QProgressDialog(this);

    proThreadCreate = std::make_shared<proTreeThread>(std::ref(importPath),std::ref(filepath),rightBtnItem,std::ref(fileCount),this,rightBtnItem,nullptr);

    connect(proThreadCreate.get(),&proTreeThread::sigUpdateProgress,this,&proTreeWidget::SlotUpdateProgress);
    connect(proThreadCreate.get(),&proTreeThread::sigFinishProgress,this,&proTreeWidget::SlotFinishProgress);
    connect(dialogProgress,&QProgressDialog::canceled,this,&proTreeWidget::SlotCancelProgress);
    connect(this,&proTreeWidget::sigCancelProgress,proThreadCreate.get(),&proTreeThread::SlotEndThread);

    proThreadCreate->start();

    dialogProgress->setWindowTitle("请耐心等待...");
    dialogProgress->setFixedWidth(PROGRESS_WIDTH);
    dialogProgress->setRange(0,PROGRESS_MAX);
    dialogProgress->exec();
}

void proTreeWidget::SlotUpdateProgress(int count)
{
    if(!dialogProgress){
        qDebug()<<"dialogProgress is empty";
        return;
    }
    if(count>=PROGRESS_MAX){
        dialogProgress->setValue(count%PROGRESS_MAX);
    }else{
        dialogProgress->setValue(count);
    }
}

void proTreeWidget::SlotFinishProgress()
{
    dialogProgress->setValue(PROGRESS_MAX);
    dialogProgress->deleteLater();
}

void proTreeWidget::SlotCancelProgress()
{
    emit sigCancelProgress();
    delete dialogProgress;
    dialogProgress=nullptr;
}

void proTreeWidget::SlotDoubleClickItem(QTreeWidgetItem *doubleItem, int col)
{
    if(QGuiApplication::mouseButtons()==Qt::LeftButton){
        proTreeItem *doubleTreeItem = dynamic_cast<proTreeItem*>(doubleItem);
        if(!doubleTreeItem){
            return;
        }
        int doubleTreeItemType = (int)doubleTreeItem->type();
        if(doubleTreeItemType==TreeItemPic){
            emit sigUpdateSelected(doubleTreeItem->getPath());
            selectedItem=doubleItem;
        }
    }
}

void proTreeWidget::SlotUpOpenProgress()
{
    if(!openProgressdlg){
        return;
    }
    openProgressdlg->setValue(PROGRESS_MAX);
    delete  openProgressdlg;
    openProgressdlg = nullptr;
}

void proTreeWidget::SlotFinishOpenProgress()
{
    if(!openProgressdlg){
        return;
    }
    openProgressdlg->setValue(PROGRESS_MAX);
    delete  openProgressdlg;
    openProgressdlg = nullptr;
}

void proTreeWidget::SlotOpenPro(const QString &path)
{
    if(setPath.find(path)!=setPath.end()){
        return;//已经存在
    }
    setPath.insert(path);
    int fileCount=0;
    QDir proDir(path);
    const QString& proName=proDir.dirName();
    proThreadOpen =  std::make_shared<proOpenThread>(path,fileCount,this,nullptr);
    proThreadOpen->start();

    openProgressdlg = new QProgressDialog(this);
    //连接更新进度框操作
    connect(proThreadOpen.get(), &proOpenThread::sigUpdateProgress,
            this, &proTreeWidget::SlotUpOpenProgress);
    connect(proThreadOpen.get(), &proOpenThread::sigFinishProgress, this,
            &proTreeWidget::SlotFinishOpenProgress);
    openProgressdlg->setWindowTitle("Please wait...");
    openProgressdlg->setFixedWidth(PROGRESS_WIDTH);
    openProgressdlg->setRange(0, PROGRESS_MAX);
    openProgressdlg->exec();
}

void proTreeWidget::SlotPreShow()
{
    if(!selectedItem){
        return;
    }
    proTreeItem *curItem = dynamic_cast<proTreeItem*>(selectedItem)->getPreItem();
    if(!curItem){
        return;
    }
    emit SigUpdatePic(curItem->getPath());
    selectedItem = curItem;
    this->setCurrentItem(curItem);
}

void proTreeWidget::SlotNextShow()
{
    if(!selectedItem){
        return;
    }
    proTreeItem *curItem = dynamic_cast<proTreeItem*>(selectedItem)->getNextItem();
    if(!curItem){
        return;
    }
    emit SigUpdatePic(curItem->getPath());
    selectedItem = curItem;//将选中的item设为下一个item
    this->setCurrentItem(curItem);

}

void proTreeWidget::SlotClosePro()
{
    removeProDialog _removeProDialog;
    _removeProDialog.show();
    _removeProDialog.exec();

    auto indexRightBtn = this->indexOfTopLevelItem(rightBtnItem);
    proTreeItem *pro_TreeItem = dynamic_cast<proTreeItem*>(rightBtnItem);
    auto * select_Item = dynamic_cast<proTreeItem*>(selectedItem);

    QString deletePath = pro_TreeItem->getPath();
    QDir dir(deletePath);
    QFileInfoList deleteInfo = dir.entryInfoList();
    if(deleteInfo.at(0).isDir()){
        QDir deleteDir(deletePath);
        deleteDir.removeRecursively();
    }
    setPath.remove(deletePath);//从容器中删除需要删除的文件路径
    if(select_Item && pro_TreeItem == select_Item->getRoot()){
        select_Item = nullptr;
        emit SigClearSelected();
    }
    delete this->takeTopLevelItem(indexRightBtn);
    rightBtnItem=nullptr;
    emit clearPic();
}

void proTreeWidget::addProToTree(const QString& name, const QString& path)
{
    QDir dir(path);
    QString filePath = dir.absoluteFilePath(name);
    if(setPath.find(filePath)!=setPath.end()){
        qDebug()<<"file has loaded";//如果文件路径和名称都一样，说明文件以及导入
        return;
    }

    QDir proDir(filePath);

    if(!proDir.exists()){
        bool enable = proDir.mkpath(filePath);
        if(!enable){
            qDebug()<<"proDir make failed";
            return;
        }
    }

    setPath.insert(filePath);

    //设置item信息
    auto* item = new proTreeItem(this,name,filePath,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,filePath);
    this->addTopLevelItem(item);


}
