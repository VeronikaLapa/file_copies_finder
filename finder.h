#ifndef FINDER_H
#define FINDER_H
#include <map>
#include <QDir>
#include <set>

class finder
{
public:
    finder(QDir dir);
    ~finder();
    void find_copies();
    std::set<std::set<QString>> get_copies();
private:
    void find_copies_in_group(std::set<QFile*>&);
    std::map<long long, std::set<QFile*>> files;
    std::set<std::set<QString>> copies;
    QDir dir;
};

#endif // FINDER_H
