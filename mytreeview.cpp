#include "mytreeview.h"
#include <QDebug>

MyTreeView::MyTreeView(QWidget *parent) : QTreeView(parent)
{
    model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    setModel(model);
    setRootIndex(model->index(QDir::currentPath()));
    setAnimated(false);
    setIndentation(20);
    setSortingEnabled(true);
    setWindowTitle(QObject::tr("Dir View"));
    show();
    connect(this, &QTreeView::doubleClicked, this, &MyTreeView::onItemDoubleClicked);
}

void MyTreeView::onItemDoubleClicked(const QModelIndex& index)
{
    emit fileDoubleClicked(model->filePath(index));
}

void MyTreeView::setFileName(const QString& file)
{
    fileName = file;
    QFileInfo info(fileName);
    QDir dir = info.absoluteDir();
    model->setRootPath(dir.absolutePath());
    auto index = model->index(model->rootPath());
    setRootIndex(index);
    index = model->index(fileName);
    expand(index);
    scrollTo(index);
    setCurrentIndex(index);
    resizeColumnToContents(0);
}
