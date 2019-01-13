#include "finder.h"
#include <QDirIterator>
#include <set>
#include <QCryptographicHash>

using namespace std;
const size_t BUFFER_SIZE = 128;

void find_copies_in_group(set<QFile*>& group) {
    map<long long, set<QFile*>> subgroups;
    char buffer[BUFFER_SIZE];
    bool is_end = false;
    for (QFile* file_ptr : group) {
        long long buffLength = file_ptr->read(buffer, sizeof(buffer));
        if (buffLength > 0) {
            string str(buffer, buffLength);
            size_t hashData = hash<string>{}(str);
            subgroups[hashData].insert(file_ptr);
        } else {
            is_end = true;
            break;
        }
    }
    if (!is_end) {
        for (pair<long long, set<QFile*>> subgroup: subgroups) {
            if (subgroup.second.size() > 1) {
                find_copies_in_group(subgroup.second);
            }
        }
    } else {
        set<QString> equel_files;
        for (QFile* file_ptr: group) {
            equel_files.insert(file_ptr->fileName());
        }
    }
}
finder::finder(QDir dir): dir(dir) {
    QDirIterator it(dir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile* crntFile = new QFile(it.next());
        files[crntFile->size()].insert(crntFile);
    }
}
finder::~finder() {
    for (pair<long long, set<QFile*>> group: files) {
        for (QFile* file : group.second) {
            delete(file);
        }
    }
}
void finder::find_copies() {
    for (pair<long long, set<QFile*>> group: files) {
        if(group.second.size() > 0) {
            find_copies_in_group(group.second);
        }
    }
}

