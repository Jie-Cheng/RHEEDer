#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <memory>

/*!
 * Zoom in and out while keeping the point under cursor motionless when
 * the scene's size is larger than the viewport.
 * Adapted from [Stack Overflow answer]
 * (https://stackoverflow.com/questions/19113532/qgraphicsview-zooming-in-and-out-under-mouse-position-using-mouse-wheel)
 */

#include <QGraphicsView>

namespace Ui {
class MyGraphicsView;
}

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    enum ViewMode {Normal, Pan, Line, Rectangle, Sector};

    explicit MyGraphicsView(QWidget *parent = nullptr);
    ~MyGraphicsView() override;
    void setImage(const QImage &newImage);
    const QImage& getImage() const;
    bool isImageNull() const;
    void gentleZoom(double factor);
    void normalSize();
    void setViewMode(ViewMode mode);

public slots:
    void onFirstPointChange(QPoint);
    void onSecondPointChange(QPoint);
    void onWidthChange(int);
    void onRadiusChange(int);
    void onThetaChange(int);
    void onPhiChange(int);

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    Ui::MyGraphicsView *ui;

    QImage image;
    QGraphicsPixmapItem *pixmapItem;

    ViewMode mode;
    /**
     * These variables are used to enable "gentle zoom"
     */
    double zoomFactor; // current scale factor
    double zoomFactorBase; // factor to control the pace of zomming
    QPointF targetScenePos; // mouse coordinates w.r.t the scene
    QPointF targetViewportPos; // mouse coordinates w.r.t the viewport

    /**
     * These variables are to enable a pan mode: in which moving mouse with
     * left button pressed translates the scene; and a shortcut which does
     * the same thing with middle button pressed (but doesn't need to enter
     * a mode).
     */
    QPoint panStart;
    bool pan = false;

    /**
     * Draw line, rectangle, sector
     */
    /* Starting point of a line or the centerline of a rectangle. or center of a sector. */
    QPoint firstPoint;
    /* End point of a line or the centerline of a rectangle or an arbitrary point at the
     * centerline of a ring. */
    QPoint secondPoint;
    /* Radius of ring */
    int radius = 200;
    /* Width of a rectangle or a ring */
    int width = 40;
    /* angle between the centerline of the section and the vertical direction,
        in degree. By default the centerline is pointing to the south */
    int theta = 0;
    /* the angle of the whole sector (not half), in degree */
    int phi = 60;
    std::shared_ptr<QGraphicsItem> itemToDraw = nullptr;

    void drawLine(QPoint first, QPoint second);
    void drawRectangle(QPoint first, QPoint second, int width);
    void drawSector(QPoint center, int radius, int width, int theta, int phi);

signals:
    void pixelUnderCursorChanged(QPoint value);
    void zoomed();
    void firstPointChanged(QPoint);
    void secondPointChanged(QPoint);
};

#endif // MYGRAPHICSVIEW_H
