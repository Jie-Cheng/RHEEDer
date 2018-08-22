#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include "mygraphicsview.h"

class MyGraphicsView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool loadFile(const QString &);

public slots:
    void open();
    void saveAs();
    void changeWorkingDirectory();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void about();
    void drawLine();
    void drawRectangle();
    void drawSector();
    void onPixelUnderCursorChanged(QPoint point);

private:
    void createActions();
    void updateActions();
    bool saveFile(const QString &fileName);
    void updateViewDrawMode();

    Ui::MainWindow *ui;

    MyGraphicsView *view;

    QAction *openAct;
    QAction *exitAct;
    QAction *saveAsAct;
    QAction *changeWorkingDirectoryAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *translateAct;
    QAction *drawLineAct;
    QAction *drawRectangleAct;
    QAction *drawSectorAct;

    QFileInfo file;
};

#endif // MAINWINDOW_H
