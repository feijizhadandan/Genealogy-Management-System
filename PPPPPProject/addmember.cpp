#include "addmember.h"
#include "ui_addmember.h"

addMember::addMember(int mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMember)
{
    ui->setupUi(this);
    setWindowTitle("添加成员");
    setWindowIcon(QIcon(":/resource/logo.jpg"));

    Mode = mode;
    // 如果点击了保存，先判断数据有无错误，然后再打包数据返回给toolWidget，加进familyTree中
    connect(ui->Confirm, &QPushButton::clicked, [=]() {
        // 检查输入格式
        if(checkTheInput()) {   // 输入无误
            QStringList data;
            QString gender;
            if(ui->Man->isChecked())     // 如果选中了男的
                gender = "男";
            else
                gender = "女";
            data << ui->Name->text() << gender << QString::number(ui->Height->value()) << ui->Education->currentText() << ui->Job->text() << ui->Birthday->dateTime().toString(QStringView(QString("yyyy-MM-dd"))) << ui->Deathday->dateTime().toString(QStringView(QString("yyyy-MM-dd"))) << ui->Introduction->toPlainText();

            emit KeepTheData(data, Mode);   // 返回输入的数据和添加模式 到 toolwidget，然后在toolwidget中加入家族树
            clear();
            this->hide();
            QMessageBox::information(this, "info", "添加成功");
        }
        else {
            // 如果有输入错误，就弹出错误提示框
            QMessageBox::warning(this, "warning", "输入不能为空");
        }
    });

    connect(ui->Cancel, &QPushButton::clicked, [=]() {
       clear();
       this->hide();
    });
}

bool addMember::checkTheInput() {
    if(ui->Name->text() == "" || ui->Job->text() == "" || ui->Introduction->toPlainText() == "") {  // 如果有输入不规范
        return false;
    }
    return true;
}

void addMember::clear() {
    ui->Name->clear();
    ui->Man->setChecked(true);
    ui->Height->setValue(0);
    ui->Education->setCurrentIndex(0);
    ui->Job->clear();
    ui->Birthday->setDate(QDate::fromString("1999-01-01", "yyyy-MM-dd"));
    ui->Deathday->setDate(QDate::fromString("2000-01-01", "yyyy-MM-dd"));
    ui->Introduction->clear();
}

addMember::~addMember()
{
    delete ui;
}
