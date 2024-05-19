#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H
#include <QTreeWidget>
#include <QAction>
#include <QProgressDialog>
#include "protreethread.h"
#include "proopenthread.h"
class proTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit proTreeWidget(QWidget *parent = nullptr);
    void addProToTree(const QString& name, const QString& path);
    ~proTreeWidget();
signals:

private:
    QSet<QString> setPath;
    QTreeWidgetItem *rightBtnItem;
    QAction *actImport;
    QAction *actSetStart;
    QAction *actClosePro;
    QAction *actSlideShow;
    QProgressDialog *dialogProgress;
    std::shared_ptr<proTreeThread> proThreadCreate;//用智能指针管理线程的生命周期
    std::shared_ptr<proOpenThread> proThreadOpen;
    QTreeWidgetItem *selectedItem;
    QProgressDialog* openProgressdlg;
private slots:
    void slotItemPressed(QTreeWidgetItem *item,int column);
    void slotImport();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();
    void SlotDoubleClickItem(QTreeWidgetItem* doubleItem, int col);
    void SlotUpOpenProgress();
    void SlotFinishOpenProgress();
public slots:
    void SlotOpenPro(const QString& path);
    void SlotPreShow();
    void SlotNextShow();
    void SlotClosePro();
signals:
    void sigCancelProgress();
    void sigUpdateSelected(const QString& path);
    void SigUpdatePic(QString path);
    void SigClearSelected();
    void clearPic();
};

#endif // PROTREEWIDGET_H
