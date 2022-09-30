#ifndef SHOWTREE_H
#define SHOWTREE_H

#include <QWidget>
#include "treedata.h"
#include <QQueue>
#include <QTreeWidget>
//#include "displaywidget.h"
//#include "mainwindow.h"

namespace Ui {
class showTree;
}

class showTree : public QWidget
{
    Q_OBJECT

public:
    explicit showTree(Tree * familyA, QWidget *parent = nullptr);
    ~showTree();

//    QQueue<int> showQue;

    Tree * family;
    void showTheTree(QTreeWidgetItem * root, int id);
    void startShow();
    void clear();

private:
    Ui::showTree *ui;
};

#endif // SHOWTREE_H
