#include "removeprodialog.h"
#include "ui_removeprodialog.h"

removeProDialog::removeProDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::removeProDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        this->close();
    });
}

removeProDialog::~removeProDialog()
{
    delete ui;
}

bool removeProDialog::IsRemoved()
{
    bool bChecked = ui->pushButton->isChecked();
    return bChecked;
}
