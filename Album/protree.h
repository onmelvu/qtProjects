#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include <QTreeWidget>
namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();

private:
    Ui::ProTree *ui;
    QSet<QString> setPath;
public slots:
    void addProToTree(const QString& name,const QString& path);
    QTreeWidget * getTreeWidget();
};

#endif // PROTREE_H
