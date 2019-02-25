#ifndef FINDER_H
#define FINDER_H
#include <map>
#include <QDir>
#include <set>
#include <QObject>


void concat_sets(std::set<std::set<QString>>& res, const std::set<std::set<QString>>& intermid);
QList<std::set<QString>> sort_all_files(QDir dir);
std::set<std::set<QString>> find_in_group(std::set<QString>);


#endif // FINDER_H
