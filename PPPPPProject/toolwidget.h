#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QDate>
#include <QWidget>
#include <QMessageBox>
#include <treedata.h>
#include "addmember.h"

namespace Ui {
class toolWidget;
}

class toolWidget : public QWidget
{
    Q_OBJECT

public:
    explicit toolWidget(QWidget *parent = nullptr);

    void connection();  // 建立各种按钮的连接
    void changeTheMode(int index);  // 根据主页面的选项，修改工具栏的功能显示：显示详细信息 或 信息搜索 或 按关系查询
    // 详细信息
    void showDetail(int ID);    // 显示详细信息函数
    bool checkTheInput();   // 检查修改详细信息的格式
    void clearDetail();   // 清空详细信息

    // 按亲戚关系查询（看了某人的详细信息，才能根据他来按关系查询）
    void clearSearchRL();   // 清空搜索栏

    // 关键字查询
    void clearKeyWordSearch(); // 清空关键字查询界面

    // 更新统计数据
    void updateStatistics();

    ~toolWidget();

    addMember * addWidget;
    Tree * family;
    int Detail_ID;

signals:
    void reloadTable();

private:
    Ui::toolWidget *ui;
};

#endif // TOOLWIDGET_H
