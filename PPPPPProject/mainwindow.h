#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "treedata.h"

#include <QMainWindow>
#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include "toolwidget.h"
#include "treedata.h"
#include "displaywidget.h"
#include "addmember.h"
#include "showtree.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);


    void sendFamily();
    void connection();  // 初始化--连接控件函数
    void initNewFile(QString path); // 给新建的文件初始化一下（添加两行）


    Tree * family;
    showTree * showTreeWidget;

    QString fileName;
    QString newFilePath;
    ~MainWindow();

signals:
    void showDetail(int ID);
    void reloadTable();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
