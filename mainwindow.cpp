#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finder.h"

#include <QCommonStyle>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTreeWidgetItem>

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

    scan_directory(QDir::homePath());
}

void main_window::find_copies() {
   QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
   if (items.size() != 1) {
       return;
   }
   for (QTreeWidgetItem * item: items) {
       QString fullFilePath = (item->data(0,Qt::UserRole)).toString();
       if(QFileInfo(fullFilePath).isDir()) {
           finder f = finder(QDir(fullFilePath));
           f.find_copies();
           std::set<std::set<QString>> copies = f.get_copies();
           for (std::set<QString> group : copies) {
               for (QString file : group) {
                   ui->listView->model()->removeRows(0, ui->listView->model()->rowCount());
                   // find haw show it
               }
           }
       }
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
