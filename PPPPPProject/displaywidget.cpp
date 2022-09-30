#include "displaywidget.h"
#include "ui_displaywidget.h"

displayWidget::displayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::displayWidget)
{
    ui->setupUi(this);

    // 初始化
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);//设置默认行高
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//列填充
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//选择整行
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->hide();  // 取消行号
    ui->tableWidget->setColumnWidth(0, 40);
    ui->tableWidget->setColumnWidth(2, 60);

    connection();
}


void displayWidget::fillTheTable() {
    ui->tableWidget->setRowCount(family->Total);
    int row = 0;
    for(auto iter = family->idHash.begin(); iter != family->idHash.end(); iter ++, row ++) {
        QStringList member = family->queryMember(iter.key());
        for(int col = 0; col < 8; col ++) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(member[col]));
        }
    }
}


void displayWidget::reloadTheTable() {
    ui->tableWidget->clearContents();
    fillTheTable();
}


void displayWidget::connection() {
    // 当表格中的某一行被点击时，显示详情页面。并且把成员ID传给工具栏
    connect(ui->tableWidget, &QTableWidget::itemClicked, [this](QTableWidgetItem* temp){
        emit showDetails(ui->tableWidget->item(temp->row(), 0)->text().toInt());
    });

}


displayWidget::~displayWidget()
{
    delete ui;
}
