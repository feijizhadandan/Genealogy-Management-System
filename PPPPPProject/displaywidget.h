#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QDebug>
#include "mainwindow.h"
#include "treedata.h"
#include "toolwidget.h"
#include "addmember.h"

namespace Ui {
class displayWidget;
}

class displayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit displayWidget(QWidget *parent = nullptr);

    void fillTheTable();
    void reloadTheTable();
    void connection();

    ~displayWidget();

    Tree * family;

signals:
    void showDetails(int ID);

private:
    Ui::displayWidget *ui;
};

#endif // DISPLAYWIDGET_H
