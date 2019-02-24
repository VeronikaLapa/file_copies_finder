#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finder.h"

#include <QCommonStyle>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTreeWidgetItem>
#include <QtConcurrent/QtConcurrent>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    QCommonStyle style;
    ui->actionScan_Directory->setIcon(style.standardIcon(QCommonStyle::SP_DialogOpenButton));
    ui->actionExit->setIcon(style.standardIcon(QCommonStyle::SP_DialogCloseButton));
    ui->actionAbout->setIcon(style.standardIcon(QCommonStyle::SP_DialogHelpButton));

    connect(ui->actionScan_Directory, &QAction::triggered, this, &main_window::select_directory);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, &main_window::show_about_dialog);
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(onTreeItemClicked(QTreeWidgetItem*)));
    connect(ui->findButton, SIGNAL(clicked()), this, SLOT(find_copies()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(delete_files()));
    scan_directory(QDir::homePath());
}

void main_window::delete_files() {
    QList<QTreeWidgetItem*> selected_items = {};
    for(int i = 0; i < ui->copiesTree->topLevelItemCount(); ++i ) {
       QTreeWidgetItem *item = ui->copiesTree->topLevelItem(i);
       for (int j = 0; j < item->childCount(); ++j) {
           if (item->child(j)->checkState(0) == Qt::Checked) {
               selected_items.append(item->takeChild(j));
           }
       }
       if (item->childCount() <= 1) {
           ui->copiesTree->takeTopLevelItem(i);
       } else {
           item->setText(0, QString::number(item->childCount()) + " copies of file");
       }
    }
    for (QTreeWidgetItem* file : selected_items) {
        QString fullFilePath = (file->data(0,Qt::UserRole)).toString();
        QFile(fullFilePath).remove();
    }
}
void main_window::show_copies(std::set<std::set<QString>>& copies) {
    for (std::set<QString> group : copies) {
        QTreeWidgetItem* parent_item = new QTreeWidgetItem(ui->copiesTree);
        parent_item->setText(0, QString::number(group.size()) + " copies of file");
        ui->copiesTree->addTopLevelItem(new QTreeWidgetItem);
        for (QString file : group) {
            QTreeWidgetItem* item = new QTreeWidgetItem(parent_item);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Unchecked);
            QFileInfo file_info(file);
            item->setText(1,file_info.fileName());
            item->setText(2, file_info.filePath());
            item->setText(3,  QString::number(file_info.size()));
            item->setData(0, Qt::UserRole, file);
            parent_item->addChild(item);
        }
    }
}

void concat_sets(std::set<std::set<QString>>& res, const std::set<std::set<QString>>& intermid) {
    for (std::set<QString> group : intermid) {
        res.insert(group);
    }
}
void main_window::find_copies() {
    QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
    QString fullFilePath;
    if (items.size() == 1) {
        QTreeWidgetItem * item = items[0];
        fullFilePath = (item->data(0,Qt::UserRole)).toString();
    } else {
        fullFilePath = crnt_dir;
    }
    if(QFileInfo(fullFilePath).isDir()) {
        ui->copiesTree->clear();
        //finder f = finder(QDir(fullFilePath));
        QList<std::set<QString>> files = sort_all_files(QDir(fullFilePath));
        QFutureWatcher<std::set<std::set<QString>>> watcher;
        QProgressDialog dialog;
        dialog.setLabelText(QString("Scaning..."));
        connect(&watcher, SIGNAL(finished()), &dialog, SLOT(reset()));
        connect(&dialog, SIGNAL(canceled()), &watcher, SLOT(cancel()));
        connect(&watcher, SIGNAL(progressRangeChanged(int, int)), &dialog, SLOT(setRange(int, int)));
        connect(&watcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));
        watcher.setFuture(QtConcurrent::mappedReduced(files, find_in_group, concat_sets));
        dialog.exec();
        watcher.waitForFinished();
        std::set<std::set<QString>> res = watcher.result();
        show_copies(res);
        //f.find_copies();
        //std::set<std::set<QString>> copies = f.get_copies();
        //show_copies(copies);
    }
}

void main_window::onTreeItemClicked(QTreeWidgetItem* item) {
    QVariant file = item->data(0,Qt::UserRole);
    QString fullFilePath = file.toString();
    if(QFileInfo(fullFilePath).isDir()) {
        scan_directory(fullFilePath);
    }
}
main_window::~main_window()
{}

void main_window::select_directory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory for Scanning",
                                                    QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    scan_directory(dir);
}

void main_window::scan_directory(QString const& dir)
{
    crnt_dir = dir;
    ui->treeWidget->clear();

    setWindowTitle(QString("Directory Content - %1").arg(dir));
    QDir d(dir);
    QFileInfoList list = d.entryInfoList();
    for (QFileInfo file_info : list)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, file_info.fileName());
        item->setText(1, QString::number(file_info.size()));
        item->setData(0, Qt::UserRole, file_info.path()+"/"+file_info.fileName());
        ui->treeWidget->addTopLevelItem(item);
    }
}

void main_window::show_about_dialog()
{
    QMessageBox::aboutQt(this);
}
