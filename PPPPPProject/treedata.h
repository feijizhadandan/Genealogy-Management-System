#ifndef TREEDATA_H
#define TREEDATA_H

#include <QString>
#include <QList>
#include <QHash>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QQueue>
#include "addmember.h"

#include <iostream>
using namespace std;


class TreeNode {    // 设计每个成员节点的信息
public:
    int ID;             // 唯一标识

    QString Name;
    QString Gender;     // 性别
    int Tall;         // 身高
    QString Education;  // 学历
    QString Job;        // 工作
    QString Birthday;   // 生日
    QString Deathday;   // 死亡日期
    QString Introduction;   // 个人简介
    TreeNode* FatherNode;   // 用来记录父亲节点
    TreeNode* PartnerNode;   // 用来记录伴侣节点

    QList<int> SonList; // 用来存储儿子节点的ID
};

class Tree {    // 用来设计家族树的功能
public:
    Tree();
    void addMember(int FatherID, int PartnerID, QStringList Date);  // 添加成员
    void updateMember(int ID, QStringList Data);    // 修改成员信息
    QList<int> deleteMember(int ID);  // 删除成员
    QList<int> searchRelation(int ID, QString relationship);    // 亲戚关系查询
    QList<int> keyWordSearch(QString kind, QString keyWord);
    QStringList queryMember(int ID);   // 查询成员信息
    void Statistics();          // 打印家族各种数据
    void outputDataToFile(QString path);    // 将信息写入文本文件
    bool inputDataFromFile(QString path);   // 从文本文件读入信息

    // 统计
    QString eduSta();

    // 信息表
    QMap<int, TreeNode*> idHash;   // 通过ID来获得对应的成员节点
    QMap<QString, int> genderCount; // 记录男女数量
    QMultiHash<QString, int> birthdayHash;  // 存储相同生日的成员ID
    QMultiHash<QString, int> educationHash; // 存储相同学历的成员ID
    QMultiHash<QString, int> genderHash;    // 存储两大性别的ID

    int MaxID;  // 记录当前最大序号
    int Total;  // 记录总人数
    int root;   // 记录祖宗节点的序号
};



#endif // TREEDATA_H
