#include "toolwidget.h"
#include "ui_toolwidget.h"

toolWidget::toolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::toolWidget)
{
    ui->setupUi(this);
    clearDetail();    // 初始化清空
    addWidget = new addMember(1);   // 初始化时，先创建一个窗口，以后需要添加在show出来，添加完就hide
    connection();
    Detail_ID = 0;
    ui->SearchBtn->setEnabled(false);


    // 查询亲戚界面表格初始化
    ui->tableRL->verticalHeader()->setDefaultSectionSize(40);//设置默认行高
    ui->tableRL->horizontalHeader()->setStretchLastSection(true);//列填充
    ui->tableRL->setSelectionBehavior(QAbstractItemView::SelectRows);//选择整行
    ui->tableRL->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->tableRL->setShowGrid(false);
    ui->tableRL->verticalHeader()->hide();  // 取消行号
    ui->tableRL->setColumnWidth(0, 70);
    ui->tableRL->setColumnWidth(1, 100);


    // 按信息查询界面表格初始化
    ui->tableKW->verticalHeader()->setDefaultSectionSize(40);//设置默认行高
    ui->tableKW->horizontalHeader()->setStretchLastSection(true);//列填充
    ui->tableKW->setSelectionBehavior(QAbstractItemView::SelectRows);//选择整行
    ui->tableKW->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->tableKW->setShowGrid(false);
    ui->tableKW->verticalHeader()->hide();  // 取消行号
    ui->tableKW->setColumnWidth(0, 50);
    ui->tableKW->setColumnWidth(1, 100);
    ui->tableKW->setColumnWidth(2, 50);
}

void toolWidget::connection() {

    connect(ui->Save, &QPushButton::clicked, [=]() {    // 修改详细信息后的保存按钮
        // 弹出确认对话框
        if(QMessageBox::Save == QMessageBox::question(this, "question", "是否要保存信息", QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Cancel)) {
            if(checkTheInput()) {
                if(Detail_ID != 0) {
                    QStringList update_data;
                    QString gender;
                    if(ui->Man->isChecked())     // 如果选中了男的
                        gender = "男";
                    else
                        gender = "女";

                    update_data << ui->Name->text() << gender << QString::number(ui->Height->value()) << ui->Education->currentText() << ui->Job->text() << ui->Birthday->dateTime().toString(QStringView(QString("yyyy-MM-dd"))) << ui->Deathday->dateTime().toString(QStringView(QString("yyyy-MM-dd"))) << ui->Introduction->toPlainText();
                    family->updateMember(Detail_ID, update_data);
                    emit reloadTable();
                    QMessageBox::information(this, "info", "保存成功！");
                }
            }
            else {
                // 如果有输入错误，就弹出错误提示框
                QMessageBox::warning(this, "warning", "输入不能为空");
            }
        }

    });

    connect(ui->Delete, &QPushButton::clicked, [=]() {  // 删除成员按钮
       // 弹出确认删除对话框
       if(QMessageBox::Yes == QMessageBox::question(this, "question", "是否要删除该成员", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
           if(Detail_ID != 0) {
               family->deleteMember(Detail_ID);
               Detail_ID = 0;   // 删除后，当前ID归零
               clearDetail();
               clearSearchRL();
               clearKeyWordSearch();
               emit reloadTable();
               QMessageBox::information(this, "info", "删除成功");
           }
        }
    });

    // 添加子女：mode = 1；添加老婆：mode = 2；（点击后，展示输入信息的界面）
    connect(ui->addChild, &QPushButton::clicked, [=]() {    // 添加子女按钮
        // 添加子女
        addWidget->Mode = 1;
        addWidget->show();
    });

    connect(ui->addPartner, &QPushButton::clicked, [=]() {  // 添加老婆按钮
       // 添加老婆
       addWidget->Mode = 2;
       addWidget->show();
    });


    // 添加界面返回的信息会在这里监听，监听到之后，根据添加的模式判断是老婆还是子女，然后分情况添加到家族树中。
    // 通过当前详细信息显示的是谁，来判断往谁身上加东西
    connect(addWidget, &addMember::KeepTheData, [this](QStringList data, int mode) {
        int FatherId;
        int PartnerId;
        if(mode == 1) { // 如果添加的是子女
            FatherId = Detail_ID;
            PartnerId = -1;
        }
        else {
            FatherId = -1;
            PartnerId = Detail_ID;
        }
        family->addMember(FatherId, PartnerId, data);
        clearDetail();
        clearSearchRL();
        clearKeyWordSearch();
        emit reloadTable();
    });


    // 亲戚关系查询的“查询”按钮
    connect(ui->searchBtn, &QPushButton::clicked, [=]() {
        QString relation = ui->relKind->currentText();
        QList<int> relation_ID = family->searchRelation(Detail_ID, relation);
        ui->tableRL->setRowCount(relation_ID.length());
        TreeNode * tem = nullptr;
        int row = 0;
        for(auto id : relation_ID) {    // 遍历返回的节点
            tem = family->idHash.value(id);
            // qDebug() << id << " " << tem->Name << " " << tem->Education;
            ui->tableRL->setItem(row, 0, new QTableWidgetItem(QString::number(tem->ID)));
            ui->tableRL->setItem(row, 1, new QTableWidgetItem(tem->Name));
            ui->tableRL->setItem(row++, 2, new QTableWidgetItem(tem->Education));
        }
    });


    // 按信息搜索的“搜索”按钮
    connect(ui->SearchBtn, &QPushButton::clicked, [=]() {
        QString kind = ui->Kind->currentText();
        QString keyWord = ui->KeyWord->text();
        QString keyEdu = ui->KeyEdu->currentText();
        QString keyDate = ui->KeyDate->dateTime().toString(QStringView(QString("yyyy-MM-dd")));

        if(kind == "姓名" || kind == "职业" || kind == "生平事迹") {
            QList<int> keyWord_ID = family->keyWordSearch(kind, keyWord);
            ui->tableKW->setRowCount(keyWord_ID.length());
            TreeNode * tem = nullptr;
            int row = 0;
            for(auto id : keyWord_ID) {    // 遍历返回的节点
                tem = family->idHash.value(id);
                // qDebug() << id << " " << tem->Name << " " << tem->Gender;
                ui->tableKW->setItem(row, 0, new QTableWidgetItem(QString::number(tem->ID)));
                ui->tableKW->setItem(row, 1, new QTableWidgetItem(tem->Name));
                ui->tableKW->setItem(row, 2, new QTableWidgetItem(tem->Gender));
                ui->tableKW->setItem(row++, 3, new QTableWidgetItem(tem->Job));
            }
        }
        else if(kind == "出生日期") {
            QList<int> keyWord_ID = family->keyWordSearch(kind, keyDate);
            ui->tableKW->setRowCount(keyWord_ID.length());
            TreeNode * tem = nullptr;
            int row = 0;
            for(auto id : keyWord_ID) {    // 遍历返回的节点
                tem = family->idHash.value(id);
                // qDebug() << id << " " << tem->Name << " " << tem->Gender;
                ui->tableKW->setItem(row, 0, new QTableWidgetItem(QString::number(tem->ID)));
                ui->tableKW->setItem(row, 1, new QTableWidgetItem(tem->Name));
                ui->tableKW->setItem(row, 2, new QTableWidgetItem(tem->Gender));
                ui->tableKW->setItem(row++, 3, new QTableWidgetItem(tem->Job));
            }
        }
        else if(kind == "学历") {
            QList<int> keyWord_ID = family->keyWordSearch(kind, keyEdu);
            ui->tableKW->setRowCount(keyWord_ID.length());
            TreeNode * tem = nullptr;
            int row = 0;
            for(auto id : keyWord_ID) {    // 遍历返回的节点
                tem = family->idHash.value(id);
                // qDebug() << id << " " << tem->Name << " " << tem->Gender;
                ui->tableKW->setItem(row, 0, new QTableWidgetItem(QString::number(tem->ID)));
                ui->tableKW->setItem(row, 1, new QTableWidgetItem(tem->Name));
                ui->tableKW->setItem(row, 2, new QTableWidgetItem(tem->Gender));
                ui->tableKW->setItem(row++, 3, new QTableWidgetItem(tem->Job));
            }
        }
    });

}

void toolWidget::changeTheMode(int index) {
    ui->toolArea->setCurrentIndex(index);
}

// 在详情栏显示信息（根据点击displaywidget的人物ID来从树中获取人物信息）
void toolWidget::showDetail(int ID) {
    changeTheMode(2);
    TreeNode * node = family->idHash.value(ID);

    if(Detail_ID != node->ID) clearSearchRL();

    Detail_ID = node->ID;
    ui->Name->setText(node->Name);
    if(node->Gender == "男") {
        if(node->PartnerNode != nullptr) {  // 如果男的已经有了老婆，就不能添加伴侣了
            ui->addChild->setEnabled(true);
            ui->addPartner->setEnabled(false);
            ui->Man->setChecked(true);
        }
        else {
            ui->addChild->setEnabled(true);
            ui->addPartner->setEnabled(true);
            ui->Man->setChecked(true);
        }
    }
    else {
        ui->addChild->setEnabled(false);
        ui->addPartner->setEnabled(false);
        ui->Woman->setChecked(true);
    }
    ui->Height->setValue(node->Tall);
    ui->Education->setCurrentText(node->Education);
    ui->Job->setText(node->Job);
    ui->Birthday->setDate(QDate::fromString(node->Birthday, "yyyy-MM-dd"));
    ui->Deathday->setDate(QDate::fromString(node->Deathday, "yyyy-MM-dd"));
    ui->Introduction->setPlainText(node->Introduction);

    ui->Man->setEnabled(true);
    ui->Woman->setEnabled(true);
    ui->Save->setEnabled(true);
    ui->Delete->setEnabled(true);

    ui->searchBtn->setEnabled(true);
    ui->SearchBtn->setEnabled(true);

}

bool toolWidget::checkTheInput() {
    if(ui->Name->text() == "" || ui->Job->text() == "" || ui->Introduction->toPlainText() == "") {  // 如果有输入不规范
        return false;
    }
    return true;
}


// 进行删除操作后，详情表归零
void toolWidget::clearDetail() {
    Detail_ID = 0;
    ui->Name->clear();
    ui->Man->setChecked(true);
    ui->Height->setValue(0);
    ui->Education->setCurrentIndex(0);
    ui->Job->clear();
    ui->Birthday->setDate(QDate::fromString("1999-01-01", "yyyy-MM-dd"));
    ui->Deathday->setDate(QDate::fromString("2000-01-01", "yyyy-MM-dd"));
    ui->Introduction->clear();

    ui->Man->setEnabled(false);
    ui->Woman->setEnabled(false);
    ui->addChild->setEnabled(false);
    ui->addPartner->setEnabled(false);
    ui->Save->setEnabled(false);
    ui->Delete->setEnabled(false);

    // 关系查询界面，如果没有Detail_ID，就不能点
    ui->searchBtn->setEnabled(false);
}




void toolWidget::clearSearchRL() {
    ui->tableRL->clearContents();
    ui->relKind->setCurrentIndex(0);
}


void toolWidget::clearKeyWordSearch() {
    ui->KeyWord->clear();
    ui->tableKW->clearContents();
}

void toolWidget::updateStatistics() {
    ui->label_total->setText(QString::number(family->Total));
    ui->label_man->setText(QString::number(family->genderCount["男"]));
    ui->label_woman->setText(QString::number(family->genderCount["女"]));

    ui->label_primary->setText(QString::number(family->educationHash.count("小学")));
    ui->label_middle->setText(QString::number(family->educationHash.count("初中")));
    ui->label_high->setText(QString::number(family->educationHash.count("高中")));
    ui->label_university01->setText(QString::number(family->educationHash.count("大学专科")));
    ui->label_university02->setText(QString::number(family->educationHash.count("大学本科")));
    ui->label_university03->setText(QString::number(family->educationHash.count("研究生")));
}


toolWidget::~toolWidget()
{
    delete ui;
}
