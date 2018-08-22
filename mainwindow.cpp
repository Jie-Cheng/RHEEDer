#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QDebug>
#include <QScreen>
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    view(new MyGraphicsView)
{
    ui->setupUi(this);
    view->setVisible(false);
    setCentralWidget(view);
    createActions();
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    setWindowTitle("RHEEDer");
    QDir::setCurrent(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last());
    connect(view, &MyGraphicsView::pixelUnderCursorChanged, this, &MainWindow::onPixelUnderCursorChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }
    view->setImage(newImage);
    updateActions();
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(view->getImage())) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), writer.errorString()));
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    openAct = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    openAct->setShortcut(QKeySequence::Open);
    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcut(QKeySequence::Save);
    saveAsAct->setEnabled(false);
    changeWorkingDirectoryAct = fileMenu->addAction(tr("&Chang working directory"), this, &MainWindow::changeWorkingDirectory);
    changeWorkingDirectoryAct->setShortcut(tr("Ctrl+C"));
    changeWorkingDirectoryAct->setEnabled(true);
    fileMenu->addSeparator();
    exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &MainWindow::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);
    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &MainWindow::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);
    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &MainWindow::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+N"));
    normalSizeAct->setEnabled(false);
    viewMenu->addSeparator();
    translateAct = viewMenu->addAction(tr("&Translate"));
    translateAct->setEnabled(false);
    translateAct->setCheckable(true);
    translateAct->setShortcut(tr("Ctrl+T"));
    connect(translateAct, &QAction::triggered, [=](const bool &state){
        updateActions();
        translateAct->setChecked(state);
        view->setViewMode(state ? MyGraphicsView::ViewMode::Pan : MyGraphicsView::ViewMode::Normal);
    });

    QMenu *toolMenu = menuBar()->addMenu(tr("&Tools"));
    drawLineAct = toolMenu->addAction(tr("Select &Line"));
    drawLineAct->setEnabled(false);
    drawLineAct->setCheckable(true);
    connect(drawLineAct, &QAction::triggered, [=](const bool &state){
        updateActions();
        drawLineAct->setChecked(state);
        view->setViewMode(state ? MyGraphicsView::ViewMode::Line : MyGraphicsView::ViewMode::Normal);
    });

    drawRectangleAct = toolMenu->addAction(tr("Select &Rectangle"));
    drawRectangleAct->setEnabled(false);
    drawRectangleAct->setCheckable(true);
    connect(drawRectangleAct, &QAction::triggered, [=](const bool &state){
        updateActions();
        drawRectangleAct->setChecked(state);
        view->setViewMode(state ? MyGraphicsView::ViewMode::Rectangle : MyGraphicsView::ViewMode::Normal);
    });

    drawSectorAct = toolMenu->addAction(tr("Select &Sector"));
    drawSectorAct->setEnabled(false);
    drawSectorAct->setCheckable(true);
    connect(drawSectorAct, &QAction::triggered, [=](const bool &state){
        updateActions();
        drawSectorAct->setChecked(state);
        view->setViewMode(state ? MyGraphicsView::ViewMode::Sector : MyGraphicsView::ViewMode::Normal);
    });

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);

    QToolBar *translateToolBar = addToolBar(tr("translate"));
    const QIcon translateIcon(":/images/translate.png");
    translateAct->setIcon(translateIcon);
    translateToolBar->addAction(translateAct);

    QToolBar *zoomInToolBar = addToolBar(tr("zoom in"));
    const QIcon zoomInIcon(":/images/zoom-in.png");
    zoomInAct->setIcon(zoomInIcon);
    zoomInToolBar->addAction(zoomInAct);

    QToolBar *zoomOutToolBar = addToolBar(tr("zoom out"));
    const QIcon zoomOutIcon(":/images/zoom-out.png");
    zoomOutAct->setIcon(zoomOutIcon);
    zoomOutToolBar->addAction(zoomOutAct);

    QToolBar *normalSizeToolBar = addToolBar(tr("normal size"));
    const QIcon normalSizeIcon(":/images/normal-size.png");
    normalSizeAct->setIcon(normalSizeIcon);
    normalSizeToolBar->addAction(normalSizeAct);

    QToolBar *drawLineToolBar = addToolBar(tr("draw line"));
    const QIcon drawLineIcon(":/images/line.png");
    drawLineAct->setIcon(drawLineIcon);
    drawLineToolBar->addAction(drawLineAct);

    QToolBar *drawRectangleToolBar = addToolBar(tr("draw rectangle"));
    const QIcon drawRectangleIcon(":/images/rectangle.png");
    drawRectangleAct->setIcon(drawRectangleIcon);
    drawRectangleToolBar->addAction(drawRectangleAct);

    QToolBar *drawSectorToolBar = addToolBar(tr("draw sector"));
    const QIcon drawSectorIcon(":/images/sector.png");
    drawSectorAct->setIcon(drawSectorIcon);
    drawSectorToolBar->addAction(drawSectorAct);
}

void MainWindow::updateActions()
{
    saveAsAct->setEnabled(view && !view->isImageNull());
    zoomInAct->setEnabled(view && !view->isImageNull());
    zoomOutAct->setEnabled(view && !view->isImageNull());
    normalSizeAct->setEnabled(view && !view->isImageNull());
    translateAct->setEnabled(view && !view->isImageNull());
    translateAct->setChecked(false);
    drawLineAct->setEnabled(view && !view->isImageNull());
    drawLineAct->setChecked(false);
    drawRectangleAct->setEnabled(view && !view->isImageNull());
    drawRectangleAct->setChecked(false);
    drawSectorAct->setEnabled(view && !view->isImageNull());
    drawSectorAct->setChecked(false);
}

void MainWindow::changeWorkingDirectory()
{
    QString directoryName = QFileDialog::getExistingDirectory(this,
                                                              tr("Change working directory"), ".",
                                                              QFileDialog::ShowDirsOnly
                                                                | QFileDialog::DontResolveSymlinks);
    QDir::setCurrent(directoryName);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open image"), QDir::currentPath(),
        tr("Images (*.png *.xpm *.jpg *.jpeg *.tiff *.svg *.bmp *.gif *.ppm)"));
    if (fileName.isEmpty()) {
        return;
    }
    loadFile(fileName);
    file = QFileInfo(fileName);
}

void MainWindow::saveAs()
{
    QString defaultName = "utitled.png";
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save image as"), defaultName,
        tr("Images (*.png *.xpm *.jpg *.tiff *.svg *.bmp *.gif *.ppm)"));
    if (fileName.isEmpty()) {
        return;
    }
    saveFile(fileName);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p><b>RHEEDer</b> is a Qt-based cross-platform software "
               "that is specialized to process images produced by "
               "Reflection High Energy Electron Diffraction (RHEED) systems. "
               "The basic functionalities include open and view image, select "
               "an area on the image and plot the gray level distribution in "
               "that area, and further fit the distribution using Gaussian functions. "
               "Icons are provided by the courtesy of David Gandy "
               "on www.flaticon.com"
               "</p>"));
}

void MainWindow::zoomIn()
{
    view->gentleZoom(1.25);
}

void MainWindow::zoomOut()
{
    view->gentleZoom(0.75);
}

void MainWindow::normalSize()
{
    view->normalSize();
}

void MainWindow::onPixelUnderCursorChanged(QPoint point)
{
    QString message = tr("Opened \"%1\", %2x%3, Depth: %4.")
        .arg(file.absoluteFilePath()).arg(view->getImage().width()).arg(view->getImage().height()).arg(view->getImage().depth());
    if (view && point.x() >= 0 && point.y() >= 0 && point.x() < view->getImage().width() && point.y() < view->getImage().height()) {
        message += QString(10, ' ');
        message += tr("Pixel location under cursor: X = %1, Y = %2").
                arg(QString::number(point.x()), QString::number(point.y()));
    }
    statusBar()->showMessage(message);
}
