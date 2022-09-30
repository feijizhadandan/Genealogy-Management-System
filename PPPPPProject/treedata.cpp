#include "treedata.h"

Tree::Tree() {
    genderCount.insert("男", 0);
    genderCount.insert("女", 0);

    root = 0;
    MaxID = 0;
    Total = 0;
}


void Tree::addMember(int FatherID, int PartnerID, QStringList Data) {
    QString name = Data.at(0);
    QString Gender = Data.at(1);
    int Tall = Data.at(2).toInt();
    QString Education = Data.at(3);
    QString Job = Data.at(4);
    QString Birthday = Data.at(5);
    QString Deathday = Data.at(6);
    QString Introduction = Data.at(7);

    // 初始化新节点
    TreeNode* newnode = new TreeNode();
    newnode->ID = ++MaxID;
    newnode->Name = name;
    newnode->Gender = Gender;
    newnode->Tall = Tall;
    newnode->Education = Education;
    newnode->Job = Job;
    newnode->Birthday = Birthday;
    newnode->Deathday = Deathday;
    newnode->Introduction = Introduction;

    // 将信息存储到各种查询表中
    idHash.insert(newnode->ID, newnode);    // 编号、节点表
    genderCount[Gender] ++; // 性别表
    birthdayHash.insert(newnode->Birthday, newnode->ID);    // 生日、编号表
    educationHash.insert(newnode->Education, newnode->ID);  // 学历、编号表
    genderHash.insert(newnode->Gender, newnode->ID);        // 性别、编号表
    Total ++;

    // 添加的时候有三种情况：1、有老爸没伴侣 2、有伴侣没老爸 3、没老爸没伴侣。
    // 如果要添加伴侣，必须在1先实现的情况下才能加2。3是祖宗节点专用
    if(FatherID > 0) {
        newnode->FatherNode = idHash.value(FatherID);   // 给新节点添加父亲
        newnode->FatherNode->SonList.append(newnode->ID);   // 把自己加入父亲节点的儿子节点集合中
    }
    else {  // 媳妇只有伴侣节点，没有父亲节点
        newnode->FatherNode = nullptr;
    }

    // 如果添加的是男性的伴侣
   if(PartnerID > 0) {
       newnode->PartnerNode = idHash.value(PartnerID);  // 给老婆节点添加老公
       newnode->PartnerNode->PartnerNode = newnode;     // 给老公节点添加老婆
   }
   else {
       newnode->PartnerNode = nullptr;
   }

   // 情况3 祖宗节点
   if(FatherID < 0 && PartnerID < 0) {
       root = newnode->ID;
   }
}


void Tree::updateMember(int ID, QStringList Data) {
    QString name = Data.at(0);
    QString Gender = Data.at(1);
    int Tall = Data.at(2).toInt();
    QString Education = Data.at(3);
    QString Job = Data.at(4);
    QString Birthday = Data.at(5);
    QString Deathday = Data.at(6);
    QString Introduction = Data.at(7);

    TreeNode * node = idHash.value(ID);
    node->Name = name;
    if(node->Gender != Gender) {    // 如果性别更换,需要修改两个表
        genderCount[node->Gender] --;
        genderCount[Gender] ++;

        genderHash.remove(node->Gender, node->ID);
        genderHash.insert(Gender, node->ID);

        node->Gender = Gender;
    }
    node->Tall = Tall;
    if(Education != node->Education) {  // 如果学历修改，那就修改哈希表中的内容
        educationHash.remove(node->Education, node->ID);
        educationHash.insert(Education, node->ID);
        node->Education = Education;
    }
    node->Job = Job;
    if(Birthday != node->Birthday) {
        birthdayHash.remove(node->Birthday, node->ID);
        birthdayHash.insert(Birthday, node->ID);
        node->Birthday = Birthday;
    }
    node->Deathday = Deathday;
    node->Introduction = Introduction;

}


QList<int> Tree::deleteMember(int ID) {
    // deletedIdList 存储被删除的元素id
    QList<int> deleteIdList;

    // 获取需要删除的节点指针
    TreeNode * node = idHash.value(ID);

    // 在他父亲的儿子列表中删除他（如果不是根节点的话）
    if(node->FatherNode != nullptr) {
        node->FatherNode->SonList.removeOne(ID);
    }

    //将他及其后代删除，用队列实现
    QQueue<TreeNode*> needToBeDeleted;
    needToBeDeleted.enqueue(node);
    while(!needToBeDeleted.isEmpty()) {
        node = needToBeDeleted.dequeue();
        deleteIdList.append(node->ID);
        if(node->Gender == "男") {   // 如果是男的，就要删除子孙和伴侣
            for(int i : node->SonList) {    // 删除儿子
                needToBeDeleted.enqueue(idHash.value(i));
            }
            if(node->PartnerNode != nullptr) {  // 如果有伴侣，需要删除(还需要修改统计表)
                deleteIdList.append(node->PartnerNode->ID);
                idHash.remove(node->PartnerNode->ID);
                genderCount["女"] --;
                birthdayHash.remove(node->PartnerNode->Birthday, node->PartnerNode->ID);
                educationHash.remove(node->PartnerNode->Education, node->PartnerNode->ID);
                genderHash.remove(node->PartnerNode->Gender, node->PartnerNode->ID);
                Total --;
                delete node->PartnerNode;
            }
        }
        else {  // 如果是女的（如果是一般的女性，用统一的删除就行了,因为没有什么下级关系牵连）
            if(node->FatherNode == nullptr) { // 这里特指媳妇,就将他老公的媳妇节点删掉
                node->PartnerNode->PartnerNode = nullptr;
            }
        }

        // 统一删除实现：
        idHash.remove(node->ID);
        genderCount[node->Gender] --;
        birthdayHash.remove(node->Birthday, node->ID);
        educationHash.remove(node->Education, node->ID);
        genderHash.remove(node->Gender, node->ID);
        Total --;
        delete node;
    }

    return deleteIdList;
}


QStringList Tree::queryMember(int ID) {
    TreeNode * node = idHash.value(ID);
    QStringList member;
    member << QString::number(node->ID) <<node->Name << node->Gender << QString::number(node->Tall) << node->Education << node->Job << node->Birthday << node->Deathday << node->Introduction;

    return member;
}


void Tree::Statistics() {
    qDebug() << "男性数量：" << genderCount["男"] << " " << "女性数量：" << genderCount["女"];
    qDebug() << "总人数：" << Total;
}


void Tree::outputDataToFile(QString path) {
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text| QIODevice::Truncate); // 以只写、文本、覆盖模式打开
    QTextStream out(&file);
    out << MaxID <<"\n";    // 文件的第一行写总人数

    QList<int> idList = idHash.keys();
    std::sort(idList.begin(), idList.end());
    for(int i : idList) {   // 按照成员编号依次写入文本文件
        TreeNode* node = idHash.value(i);

        int id = node->ID;

        QString name = node->Name;
        QString gender = node->Gender;
        int tall = node->Tall;
        QString Education = node->Education;
        QString job = node->Job;
        QString birthday = node->Birthday;
        QString Deathday = node->Deathday;
        QString Introduction = node->Introduction;
        int fatherID = node->FatherNode==nullptr? -1 : node->FatherNode->ID;
        int partnerID = node->PartnerNode==nullptr? -1: node->PartnerNode->ID;

        QString information = QString(QString("%1").arg(id) + "%" + name + "%" + gender + "%"
                                      + QString("%1").arg(tall) + "%" + Education + "%" + job + "%" + birthday + "%"
                                      + Deathday + "%" +Introduction + "%" + QString("%1").arg(fatherID) + "%" + QString("%1").arg(partnerID));
        out << information << "\n";
    }
    file.close();
}


bool Tree::inputDataFromFile(QString path) {
    QFile file(path);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {    // 以只读、文本形式打开。如果打开失败，就return false
        qDebug() << "File Open Failed";
        return false;
    }

    // 逐行读取（注意第一行是总人数，第二行是祖宗节点）
    QTextStream in(&file);
    QString line_in;    // 获得一整行的内容
    QStringList line_get;   // 存储line_in按照%分割后的结果

    line_in = in.readLine();
    if(line_in.isEmpty()) { // 如果文本为空，就返回错误
        qDebug() << "File Is Empty";
        return false;
    }
    MaxID = line_in.toInt();

    line_in = in.readLine();
    if(line_in.isEmpty()) { // 如果第二行为空，说明没有祖宗
        qDebug() << "No Root";
        return false;
    }

    line_get = line_in.split("%");
    // 初始化祖宗节点
    int id = line_get.at(0).toInt();
    QString name = line_get.at(1);
    QString Gender = line_get.at(2);
    int Tall = line_get.at(3).toInt();
    QString Education = line_get.at(4);
    QString Job = line_get.at(5);
    QString Birthday = line_get.at(6);
    QString Deathday = line_get.at(7);
    QString Introduction = line_get.at(8);
    // 初始化新节点
    TreeNode* newnode = new TreeNode();
    newnode->ID = id;
    newnode->Name = name;
    newnode->Gender = Gender;
    newnode->Tall = Tall;
    newnode->Education = Education;
    newnode->Job = Job;
    newnode->Birthday = Birthday;
    newnode->Deathday = Deathday;
    newnode->Introduction = Introduction;
    newnode->FatherNode = nullptr;
    newnode->PartnerNode = nullptr;
    // 将信息存储到各种查询表中
    idHash.insert(newnode->ID, newnode);    // 编号、节点表
    genderCount[Gender] ++; // 性别表
    birthdayHash.insert(newnode->Birthday, newnode->ID);    // 生日、编号表
    educationHash.insert(newnode->Education, newnode->ID);  // 学历、编号表
    genderHash.insert(newnode->Gender, newnode->ID);        // 性别、编号表
    Total ++;

    root = newnode->ID;
    // 载入其他节点
    while(!in.atEnd()) {
        line_in = in.readLine();
        line_get = line_in.split("%");

        int id = line_get.at(0).toInt();
        QString name = line_get.at(1);
        QString Gender = line_get.at(2);
        int Tall = line_get.at(3).toInt();
        QString Education = line_get.at(4);
        QString Job = line_get.at(5);
        QString Birthday = line_get.at(6);
        QString Deathday = line_get.at(7);
        QString Introduction = line_get.at(8);
        int FatherID = line_get.at(9).toInt();
        int PartnerID = line_get.at(10).toInt();
        // 初始化新节点
        newnode = new TreeNode();
        newnode->ID = id;
        newnode->Name = name;
        newnode->Gender = Gender;
        newnode->Tall = Tall;
        newnode->Education = Education;
        newnode->Job = Job;
        newnode->Birthday = Birthday;
        newnode->Deathday = Deathday;
        newnode->Introduction = Introduction;
        // 将信息存储到各种查询表中
        idHash.insert(newnode->ID, newnode);    // 编号、节点表
        genderCount[Gender] ++; // 性别表
        birthdayHash.insert(newnode->Birthday, newnode->ID);    // 生日、编号表
        educationHash.insert(newnode->Education, newnode->ID);  // 学历、编号表
        genderHash.insert(newnode->Gender, newnode->ID);        // 性别、编号表
        Total ++;

        if(FatherID > 0) {
            newnode->FatherNode = idHash.value(FatherID);   // 给新节点添加父亲
            newnode->FatherNode->SonList.append(newnode->ID);   // 把自己加入父亲节点的儿子节点集合中
        }
        else {  // 媳妇只有伴侣节点，没有父亲节点
            newnode->FatherNode = nullptr;
        }

        // 如果添加的是男性的伴侣
        if(Gender == "女" && PartnerID > 0) {
            newnode->PartnerNode = idHash.value(PartnerID);  // 给老婆节点添加老公
            newnode->PartnerNode->PartnerNode = newnode;     // 给老公节点添加老婆
        }
        else {
            newnode->PartnerNode = nullptr;
        }
    }
    file.close();
    return true;
}



QList<int> Tree::searchRelation(int ID, QString relationship) {
    QList<int> relation_ID;

    // 搜寻关系：如果是这个关系，就加入 列表
    TreeNode * node = idHash.value(ID);

    if(relationship == "父亲") {
        if(node->FatherNode != nullptr)
            relation_ID.append(node->FatherNode->ID);
    }
    else if(relationship == "母亲") {
        if((node->FatherNode != nullptr) && node->FatherNode->PartnerNode != nullptr)
            relation_ID.append(node->FatherNode->PartnerNode->ID);
    }
    else if(relationship == "伴侣") {
        if(node->PartnerNode != nullptr) {
            relation_ID.append(node->PartnerNode->ID);
        }
    }
    else if(relationship == "儿子") {
        TreeNode * son = nullptr;
        if(node->FatherNode == nullptr && node->PartnerNode != nullptr && node->ID != root) {   // 伴侣
            for(int id : node->PartnerNode->SonList) {
                son = idHash.value(id);
                if(son->Gender == "男") {
                    relation_ID.append(id);
                }
            }
        }
        else {
            for(int id : node->SonList) {
                son = idHash.value(id);
                if(son->Gender == "男") {
                    relation_ID.append(id);
                }
            }
        }
    }
    else if(relationship == "女儿") {
        TreeNode * son = nullptr;
        if(node->FatherNode == nullptr && node->PartnerNode != nullptr && node->ID != root) {
            for(int id : node->PartnerNode->SonList) {
                son = idHash.value(id);
                if(son->Gender == "女") {
                    relation_ID.append(id);
                }
            }
        }
        else {
            for(int id : node->SonList) {
                son = idHash.value(id);
                if(son->Gender == "女") {
                    relation_ID.append(id);
                }
            }
        }
    }
    else if(relationship == "爷爷") {
        if(node->FatherNode != nullptr && node->FatherNode->FatherNode != nullptr)
            relation_ID.append(node->FatherNode->FatherNode->ID);
    }
    else if(relationship == "奶奶") {
        if(node->FatherNode != nullptr &&
                node->FatherNode->FatherNode != nullptr &&
                node->FatherNode->FatherNode->PartnerNode != nullptr)
            relation_ID.append(node->FatherNode->FatherNode->PartnerNode->ID);
    }
    else if(relationship == "孙子") {
        TreeNode * son = nullptr;
        if(node->FatherNode == nullptr && node->PartnerNode != nullptr) {
            for(int i : node->PartnerNode->SonList) {
                son = idHash.value(i);
                TreeNode * grandson = nullptr;
                for(int j : son->SonList) {
                    grandson = idHash.value(j);
                    if(grandson->Gender == "男")
                        relation_ID.append(j);
                }
            }
        }
        else {
            for(int i : node->SonList) {
                son = idHash.value(i);
                TreeNode * grandson = nullptr;
                for(int j : son->SonList) {
                    grandson = idHash.value(j);
                    if(grandson->Gender == "男")
                        relation_ID.append(j);
                }
            }
        }
    }
    else if(relationship == "孙女") {
        TreeNode * son = nullptr;
        if(node->FatherNode == nullptr && node->PartnerNode != nullptr) {
            for(int i : node->PartnerNode->SonList) {
                son = idHash.value(i);
                TreeNode * granddaughter = nullptr;
                for(int j : son->SonList) {
                    granddaughter = idHash.value(j);
                    if(granddaughter->Gender == "女")
                        relation_ID.append(j);
                }
            }
        }
        else {
            for(int i : node->SonList) {
                son = idHash.value(i);
                TreeNode * granddaughter = nullptr;
                for(int j : son->SonList) {
                    granddaughter = idHash.value(j);
                    if(granddaughter->Gender == "女")
                        relation_ID.append(j);
                }
            }
        }
    }

    return relation_ID;
}


QList<int> Tree::keyWordSearch(QString kind, QString keyWord) {

    QList<int> keyWord_ID;
    TreeNode* node = nullptr;

    if(kind == "姓名") {
        QList<int> idList = idHash.keys();
        std::sort(idList.begin(), idList.end());
        for(int i : idList) {
            node = idHash.value(i);
            QString name = node->Name;

            if(name.contains(keyWord,Qt::CaseSensitive)) {    // 如果包含关键字
                keyWord_ID.append(i);
            }
        }
    }
    else if(kind == "职业") {
        QList<int> idList = idHash.keys();
        std::sort(idList.begin(), idList.end());
        for(int i : idList) {
            node = idHash.value(i);
            QString job = node->Job;

            if(job.contains(keyWord,Qt::CaseSensitive)) {    // 如果包含关键字
                keyWord_ID.append(i);
            }
        }
    }
    else if(kind == "生平事迹") {
        QList<int> idList = idHash.keys();
        std::sort(idList.begin(), idList.end());
        for(int i : idList) {
            node = idHash.value(i);
            QString introduction = node->Introduction;

            if(introduction.contains(keyWord,Qt::CaseSensitive)) {    // 如果包含关键字
                keyWord_ID.append(i);
            }
        }
    }
    else if(kind == "出生日期") {
        QList<int> idList = idHash.keys();
        std::sort(idList.begin(), idList.end());
        for(int i : idList) {
            node = idHash.value(i);
            QString birthday = node->Birthday;

            if(birthday.contains(keyWord,Qt::CaseSensitive)) {    // 如果包含关键字
                keyWord_ID.append(i);
            }
        }
    }
    else if(kind == "学历") {
        QList<int> idList = idHash.keys();
        std::sort(idList.begin(), idList.end());
        for(int i : idList) {
            node = idHash.value(i);
            QString education = node->Education;

            if(education.contains(keyWord,Qt::CaseSensitive)) {    // 如果包含关键字
                keyWord_ID.append(i);
            }
        }
    }
    return keyWord_ID;
}





