#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QTreeView>
#include <QFileSystemModel>

class MyTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit MyTreeView(QWidget *parent = nullptr);
    void setFileName(const QString& file);

signals:
    void fileDoubleClicked(const QString& file);

public slots:
    void onItemDoubleClicked(const QModelIndex& index);

private:
    QFileSystemModel *model;
    QString fileName;
};

#endif // MYTREEVIEW_H
