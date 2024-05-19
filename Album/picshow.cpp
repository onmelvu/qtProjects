#include "picshow.h"
#include "ui_picshow.h"

picShow::picShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::picShow)
{
    ui->setupUi(this);
    ui->preBtn->SetIcons(":/icon/previous.png",":/icon/previous_hover.png",":/icon/previous_press.png");
    ui->nextBtn->SetIcons(":/icon/next.png",":/icon/next_hover.png",":/icon/next_press.png");
    connect(ui->nextBtn,&QPushButton::clicked,this,&picShow::SigNextClicked);
    connect(ui->preBtn,&QPushButton::clicked,this,&picShow::SigPreClicked);

}

picShow::~picShow()
{
    delete ui;
}



void picShow::SlotSelectItem(const QString &path)
{
    _selectedPath = path;
    _pixMap.load(path);
    auto width = this->width()-20;
    auto height = this->height()-20;
    _pixMap = _pixMap.scaled(width,height,Qt::KeepAspectRatio);
    ui->picLab->setPixmap(_pixMap);
}

void picShow::SlotUpdatePic(const QString &path)
{
    _selectedPath=path;
    if(_selectedPath != ""){
        const auto &width = ui->gridLayout->geometry().width();
        const auto &height = ui->gridLayout->geometry().height();
        _pixMap.load(_selectedPath);
        _pixMap = _pixMap.scaled(width,height,Qt::KeepAspectRatio);
        ui->picLab->setPixmap(_pixMap);
    }
}

void picShow::SlotDeleteItem()
{
    _selectedPath="";
}

void picShow::SlotClearPic()
{
    ui->picLab->clear();
}

