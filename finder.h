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
private:
    void find_copies_in_group(std::set<QFile*>&);
    std::map<long long, std::set<QFile*>> files;
    std::set<std::set<QString>> copies;
    QDir dir;
signals:
    void progress(int);
};

#endif // FINDER_H
