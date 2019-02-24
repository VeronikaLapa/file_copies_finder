#ifndef FINDER_H
#define FINDER_H
#include <map>
#include <QDir>
#include <set>
#include <QObject>


class finder
{
public:
    finder(QDir dir);
    ~finder();
    void find_copies();
    std::set<std::set<QString>> get_copies();
    int files_count();
    std::map<long long, std::set<QFile*>> files;
private:
    void find_copies_in_group(std::set<QFile*>&);
    std::set<std::set<QString>> copies;
    QDir dir;
};
QList<std::set<QString>> sort_all_files(QDir dir);
std::set<std::set<QString>> find_in_group(std::set<QString>);

#endif // FINDER_H
