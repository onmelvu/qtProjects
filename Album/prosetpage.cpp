#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QFileDialog>
ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proPath",ui->lineEdit);
    registerField("proName",ui->lineEdit_2);
    connect(ui->lineEdit,&QLineEdit::textEdited,this,&ProSetPage::completeChanged);
    connect(ui->lineEdit_2,&QLineEdit::textEdited,this,&ProSetPage::completeChanged);

    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());
    //设置lineedit的清除功能
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit_2->setClearButtonEnabled(true);


}

ProSetPage::~ProSetPage()
{
    delete ui;
}

bool ProSetPage::isComplete() const
{
    if(ui->lineEdit->text()==""||ui->lineEdit_2->text()==""){
        return false;
    }
    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists()){
        ui->tips->setText("projectPath is not exist!");
        return false;
    }
    QString absFilePath = dir.absoluteFilePath(ui->label->text());
    QDir distDir(absFilePath);
    if(distDir.exists()){
        ui->tips->setText("project has exist pls change path or name");
        return false;
    }

    ui->tips->setText("");
    return QWizardPage::isComplete();
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name=ui->lineEdit->text();
    path=ui->lineEdit_2->text();
}

void ProSetPage::on_pushButton_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);//以目录方式打开文件对话框
    fileDialog.setWindowTitle("选择导入文件夹");
    QString path = QDir::currentPath();//获取当前路径
    fileDialog.setDirectory(path);//将当前路径设为起始路径
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(fileDialog.exec()){//如果打开成功，则执行下列程序
        fileNames=fileDialog.selectedFiles();//将选择的文件保存到fileNames中
    }
    if(fileNames.length()<=0){
        return;
    }
    QString importPath = fileNames.at(0);
    qDebug()<<"import_path is "<<importPath;
    ui->lineEdit_2->setText(importPath);
}

