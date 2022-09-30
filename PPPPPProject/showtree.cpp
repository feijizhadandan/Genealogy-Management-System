#include "showtree.h"
#include "ui_showtree.h"

showTree::showTree(Tree * familyA, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showTree)
{
    ui->setupUi(this);

    family = familyA;
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);  // 自适应表头
    ui->treeWidget->header()->hide();
}

void showTree::startShow() {
    // 特殊处理根节点
    TreeNode * node = family->idHash[family->root];
    QTreeWidgetItem * root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0, node->Name);

    for(auto & child : node->SonList) {
        showTheTree(root, child);
    }
}

void showTree::showTheTree(QTreeWidgetItem * root, int id) {

    TreeNode * node = family->idHash[id];
    QTreeWidgetItem * child = new QTreeWidgetItem(root);

    if(node->PartnerNode != nullptr) {
        child->setText(0, node->Name + "    " + node->PartnerNode->Name);
    }
    else {
        child->setText(0, node->Name);
    }

    for(auto & i : node->SonList) {
        showTheTree(child, i);
    }
}

void showTree::clear() {
    ui->treeWidget->clear();
}

showTree::~showTree()
{
    delete ui;
}
