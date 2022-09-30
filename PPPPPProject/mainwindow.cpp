#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    family = new Tree();
    setWindowTitle("族谱管理器");
    setWindowIcon(QIcon(":/resource/logo.jpg"));
    connection();
    showTreeWidget = new showTree(family);

    // 默认显示详细信息
    ui->tool->changeTheMode(2);
}


void MainWindow::sendFamily() {
    ui->tool->family = family;  // 初始化工具栏的树
    ui->display->family = family;   // 初始化展示栏的树

}

void MainWindow::initNewFile(QString path) {
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text| QIODevice::Truncate); // 以只写、文本、覆盖模式打开
    QTextStream out(&file);
    out << "1" << "\n";  // 只有祖宗
    out << "1%祖宗%男%170%高中%工作%2000-01-01%2000-01-01%请添加简介%-1%-1" << "/n";
    file.close();
}


void MainWindow::connection() {

    // 新建一个家谱文档
    connect(ui->newOne, &QAction::triggered, [=]() {
        if(family->root == 0) {
            fileName = QFileDialog::getOpenFileName(this, "选择一个空文本文件", "D:\\Game\\QtProject\\PPPPPProject", "(*.txt)");
            initNewFile(fileName);
            family->inputDataFromFile(fileName);
            family->Statistics();
            sendFamily();   // 总页面初始化树之后，给其他工具栏也初始化一下
            ui->display->fillTheTable();
        }
        else {
            QMessageBox::warning(this, "warning", "请勿重复导入族谱");
        }
    });

    // 选择文件，并构造家族树
    connect(ui->action_Open, &QAction::triggered, [=]() {
        if(family->root == 0) {
            fileName = QFileDialog::getOpenFileName(this, "选择文件", "D:\\Game\\QtProject\\PPPPPProject", "(*.txt)");
            family->inputDataFromFile(fileName);
            family->Statistics();
            sendFamily();   // 总页面初始化树之后，给其他工具栏也初始化一下
            ui->display->fillTheTable();
        }
        else {
            QMessageBox::warning(this, "warning", "请勿重复导入族谱");
        }
    });

    // 保存文件，保存到原路径
    connect(ui->action_Save, &QAction::triggered, [=]() {
       if(family->root == 0) {
           QMessageBox::warning(this, "warning", "请先加载族谱文件");
       }
       else {
           family->outputDataToFile(fileName);
           QMessageBox::information(this, "info", "保存成功");
       }
    });

//    // 另存为，获取新路径，保存到新路径
//    connect(ui->action_newSave, &QAction::triggered, [=]() {
//        newFilePath = QFileDialog::getOpenFileName(this, "选择另存路径", "C:\\Users\\震\\Desktop");
//        family->outputDataToFile(newFilePath);
//    });

    // 功能1：显示详细信息
    connect(ui->actionDetail, &QAction::triggered, [=]() {
        ui->tool->changeTheMode(2);
    });

    // 功能2：信息搜索
    connect(ui->actionSearch, &QAction::triggered, [=]() {
        ui->tool->changeTheMode(0);
    });

    // 功能3：按亲戚关系查询
    connect(ui->actionRelative, &QAction::triggered, [=]() {
        ui->tool->changeTheMode(1);
    });

    // 功能4：刷新
//    connect(ui->actionReload, &QAction::triggered, [=]() {
//        ui->display->reloadTheTable();
//    });

    // 功能5：展示统计信息页面
    connect(ui->actionSta, &QAction::triggered, [=]() {
       if(family->root != 0) {
           ui->tool->changeTheMode(3);
           ui->tool->updateStatistics();
       }
    });

    // 功能6：展示家族树
    connect(ui->actionShowTree, &QAction::triggered, [=]() {
       if(family->root != 0) {
           showTreeWidget->clear();
           showTreeWidget->show();
           showTreeWidget->startShow();
       }
    });


    connect(ui->display, &displayWidget::showDetails, ui->tool, &toolWidget::showDetail);
    connect(ui->tool, &toolWidget::reloadTable, ui->display, &displayWidget::reloadTheTable);
}




MainWindow::~MainWindow()
{
    delete ui;
}

