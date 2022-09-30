#ifndef ADDMEMBER_H
#define ADDMEMBER_H

#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>
#include "treedata.h"

namespace Ui {
class addMember;
}

class addMember : public QWidget
{
    Q_OBJECT

public:
    explicit addMember(int mode, QWidget *parent = nullptr);
    ~addMember();

    int Mode;
    bool checkTheInput();
    void clear();

signals:
    void KeepTheData(QStringList data, int mode);

private:
    Ui::addMember *ui;
};

#endif // ADDMEMBER_H
