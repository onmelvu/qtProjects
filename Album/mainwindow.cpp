#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "wizard.h"
#include "protree.h"
#include "picshow.h"
#include <QTreeWidget>
#include "protreewidget.h"
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建文件菜单栏
    QMenu *menuFile = menuBar()->addMenu(tr("文件(&F)"));

    //创建项目动作
    QAction *actCreatePro = new QAction(QIcon(":/icon/createpro.png"),tr("创建项目"),this);
    actCreatePro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menuFile->addAction(actCreatePro);
    //创建打开项目
    QAction *actOpenPro = new QAction(QIcon(":/icon/openpro.png"),tr("打开项目"),this);
    actOpenPro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menuFile->addAction(actOpenPro);

    //创建设置菜单栏
    QMenu *menuSet = menuBar()->addMenu(tr("设置(&S)"));
    QAction *actMusic = new QAction(QIcon(":/icon/music.png"),tr("背景音乐"),this);
    menuSet->addAction(actMusic);

    //连接创建项目槽函数
    connect(actCreatePro,&QAction::triggered,this,&MainWindow::slotCreatPro);
    //连接打开项目槽函数
    connect(actOpenPro,&QAction::triggered,this,&MainWindow::slotOpenPro);

    _proTree = new ProTree();
    ui->LeftLayout->addWidget(_proTree);

    _picShow = new picShow();
    ui->RightLayout->addWidget(_picShow);

    QTreeWidget* TreeWidget = dynamic_cast<ProTree*>(_proTree)->getTreeWidget();
    proTreeWidget *_proTreeWidget = dynamic_cast<proTreeWidget*>(TreeWidget);
    auto * proPicShow = dynamic_cast<picShow*>(_picShow);
    connect(this, &MainWindow::sigOpenPro, _proTreeWidget, &proTreeWidget::SlotOpenPro);

    connect(_proTreeWidget, &proTreeWidget::sigUpdateSelected,proPicShow,
            &picShow::SlotSelectItem);

    connect(proPicShow,&picShow::SigPreClicked,_proTreeWidget,&proTreeWidget::SlotPreShow);
    connect(proPicShow,&picShow::SigNextClicked,_proTreeWidget,&proTreeWidget::SlotNextShow);
    connect(_proTreeWidget,&proTreeWidget::SigUpdatePic,proPicShow,&picShow::SlotUpdatePic);
    connect(_proTreeWidget,&proTreeWidget::SigClearSelected,proPicShow,&picShow::SlotDeleteItem);
    connect(_proTreeWidget,&proTreeWidget::clearPic,proPicShow,&picShow::SlotClearPic);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotCreatPro(bool)
{
    qDebug()<<"创建项目成功";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    QWizardPage *page = wizard.page(0);
    page->setTitle(tr("设置项目配置"));


    //连接信号和槽
    connect(&wizard,&Wizard::SigProSettings,dynamic_cast<ProTree*>(_proTree),&ProTree::addProToTree);//降低耦合性

    wizard.show();
    wizard.exec();

    disconnect();
}

void MainWindow::slotOpenPro(bool)
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("选择导入的文件");

    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList listNames;
    if(fileDialog.exec()){
        listNames = fileDialog.selectedFiles();
    }

    if(listNames.length()<=0){
        return;
    }

    QString importPath = listNames.at(0);
    emit sigOpenPro(importPath);

}


