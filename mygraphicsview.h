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
    enum DrawMode {Normal, DrawLine, DrawRectangle, DrawSector};
    explicit MyGraphicsView(QWidget *parent = nullptr);
    ~MyGraphicsView() override;
    void setImage(const QImage &newImage);
    const QImage& getImage() const;
    bool isImageNull() const;
    void gentleZoom(double factor);
    void normalSize();
    void toggleDragMode();
    void setDrawMode(DrawMode mode);

signals:
    void pixelUnderCursorChanged(QPoint value);

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    Ui::MyGraphicsView *ui;

    QImage image;
    QGraphicsPixmapItem *pixmapItem;

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
    bool pan;
    bool panMode;
    QPoint panStart;

    /**
     * Draw line, rectangle, sector
     */
    DrawMode drawMode;
    /* Starting point of a line or the centerline of a rectangle. or center of a sector. */
    std::shared_ptr<QPointF> firstPoint = nullptr;
    /* End point of a line or the centerline of a rectangle or an arbitrary point at the
     * centerline of a ring. */
    std::shared_ptr<QPointF> secondPoint = nullptr;
    /* Width of a rectangle or a ring */
    int width = 20;
    /* angle between the centerline of the section and the vertical direction,
        in degree. By default the centerline is pointing to the south */
    double theta = 0;
    /* the angle of the whole sector (not half), in degree */
    double phi = 30;
    std::shared_ptr<QGraphicsItem> itemToDraw = nullptr;

    static QPainterPath createRing(QPointF center, double radius, double width, double theta, double phi);

signals:
    void zoomed();
};

#endif // MYGRAPHICSVIEW_H
