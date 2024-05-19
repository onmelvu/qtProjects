#include "protree.h"
#include "ui_protree.h"
#include <QDir>

ProTree::ProTree(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProTree)
{
    ui->setupUi(this);
    this->setMinimumWidth(378);
    this->setMaximumWidth(378);


}

ProTree::~ProTree()
{
    delete ui;
}

void ProTree::addProToTree(const QString &name, const QString &path)
{
    ui->treeWidget->addProToTree(name,path);
}

QTreeWidget *ProTree::getTreeWidget()
{
    return ui->treeWidget;
}



