#include "finder.h"
#include <QDirIterator>
#include <set>
#include <QCryptographicHash>
#include <QDir>
#include "mainwindow.h"
#include <iostream>

using namespace std;
const size_t BUFFER_SIZE = 128;


QList<std::set<QString>> sort_all_files(QDir dir) {
    QDirIterator it(dir.absolutePath(), QDirIterator::Subdirectories);
    std::map<long long, std::set<QString>> files;
    while (it.hasNext()) {
        QFile crnt(it.next());
        files[crnt.size()].insert(crnt.fileName());
    }
    QList<std::set<QString>> result;
    for (std::pair<long long, std::set<QString>> el : files) {
        result.append(el.second);
    }
    return result;
}

std::set<std::set<QString>> find_in_group(std::set<QString> path_group) {
    std::set<QFile*> group;
    for (QString path : path_group) {
        QFile* crnt = new QFile(path);
        if (crnt->open(QIODevice::ReadOnly)) {
            group.insert(crnt);
        }
    }
    if (group.size() < 2) {
        return {};
    }
    map<QString, set<QFile*>> subgroups;
    char buffer[BUFFER_SIZE];
    for (QFile* file_ptr : group) {

        long long buffLength;
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        while(!file_ptr->atEnd()) {
            buffLength = file_ptr->read(buffer, sizeof(buffer));
            string str(buffer, buffLength);
            crypto.addData(str.c_str(), str.length());
        }
        subgroups[crypto.result().toHex().data()].insert(file_ptr);
    }
    std::set<std::set<QString>> result;
    for (pair<QString,set<QFile*>> g: subgroups) {
        set<QString> equel_files;
        for (QFile* file_ptr: g.second) {
            equel_files.insert(file_ptr->fileName());
        }
        if (equel_files.size() > 1)
            result.insert(equel_files);
    }
    for (QFile* file_ptr : group) {
        file_ptr->close();
        delete file_ptr;
    }
    return result;
}

void concat_sets(std::set<std::set<QString>>& res, const std::set<std::set<QString>>& intermid) {
    for (std::set<QString> group : intermid) {
        res.insert(group);
    }
}

