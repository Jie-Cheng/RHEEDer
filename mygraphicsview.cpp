#include "mygraphicsview.h"
#include "ui_mygraphicsview.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QDebug>
#include <QScrollBar>
#include <QGraphicsPixmapItem>

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::MyGraphicsView),
    mode(ViewMode::Normal),
    zoomFactor(1.0),
    zoomFactorBase(1.0015)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

MyGraphicsView::~MyGraphicsView()
{
    delete ui;
}

void MyGraphicsView::setImage(const QImage& newImage)
{
    image = newImage;
    pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(newImage));
    pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    auto scene = new QGraphicsScene;
    scene->addItem(pixmapItem);
    setScene(scene);
    show();
}

const QImage& MyGraphicsView::getImage() const
{
    return image;
}

bool MyGraphicsView::isImageNull() const
{
    return image.isNull();
}

void MyGraphicsView::normalSize()
{
    gentleZoom(1.0 / zoomFactor);
    centerOn(QPointF(viewport()->width()/2.0,
                     viewport()->height()/2.0));
}

void MyGraphicsView::gentleZoom(double factor)
{
    scale(factor, factor);
    zoomFactor *= factor;
    centerOn(targetScenePos);
    QPointF deltaViewportPos = targetViewportPos - QPointF(viewport()->width()/2.0,
                                                           viewport()->height()/2.0);
    QPointF viewportCenter = mapFromScene(targetScenePos) - deltaViewportPos;
    centerOn(mapToScene(viewportCenter.toPoint()));
    emit zoomed();
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    targetViewportPos = event->pos();
    targetScenePos = mapToScene(event->pos());
    if (pan) {
        this->horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - panStart.x()));
        this->verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - panStart.y()));
        panStart = event->pos();
    }
    emit pixelUnderCursorChanged(targetScenePos.toPoint());

    if (event->buttons() == Qt::LeftButton && pixmapItem && pixmapItem->boundingRect().contains(targetScenePos)) {
        if (mode == ViewMode::Line) {
            QLineF line(firstPoint, targetScenePos);
            if (!itemToDraw) {
                itemToDraw = std::make_shared<QGraphicsLineItem>();
                auto lineToDraw = std::dynamic_pointer_cast<QGraphicsLineItem>(itemToDraw);
                lineToDraw->setPen(QPen(Qt::green, 1, Qt::SolidLine));
                lineToDraw->setLine(line);
                scene()->addItem(lineToDraw.get());
            } else {
                auto lineToDraw = std::dynamic_pointer_cast<QGraphicsLineItem>(itemToDraw);
                lineToDraw->setLine(line);
            }
        } else if (mode == ViewMode::Rectangle) {
            QVector<QPointF> vertices;
            QPointF tmp = targetScenePos - firstPoint;
            double dx = 1, dy = 0;
            if (abs(tmp.x()) > 1e-6) {
                // not vertical
                double k = -tmp.y() / tmp.x();
                dy = sqrt(1.0/(1 + k * k));
                dx = k * dy;
            }
            vertices.push_back(QPointF(firstPoint.x() + width/2 * dx, firstPoint.y() + width/2 * dy));
            vertices.push_back(QPointF(targetScenePos.x() + width/2 * dx, targetScenePos.y() + width/2 * dy));
            vertices.push_back(QPointF(targetScenePos.x() - width/2 * dx, targetScenePos.y() - width/2 * dy));
            vertices.push_back(QPointF(firstPoint.x() - width/2 * dx, firstPoint.y() - width/2 * dy));
            QPolygonF poly(vertices);
            if (!itemToDraw) {
                itemToDraw = std::make_shared<QGraphicsPolygonItem>();
                auto rectToDraw = std::dynamic_pointer_cast<QGraphicsPolygonItem>(itemToDraw);
                rectToDraw->setPen(QPen(Qt::green, 1, Qt::SolidLine));
                rectToDraw->setPolygon(poly);
                scene()->addItem(rectToDraw.get());
            } else {
                auto rectToDraw = std::dynamic_pointer_cast<QGraphicsPolygonItem>(itemToDraw);
                rectToDraw->setPolygon(poly);
            }
        } else if (mode == ViewMode::Sector) {
            double r = sqrt(qPow(targetScenePos.x() - firstPoint.x(), 2) +
                            qPow(targetScenePos.y() - firstPoint.y(), 2));
            QPainterPath path = createRing(firstPoint, r, width, theta, phi);
            if (!itemToDraw) {
                itemToDraw = std::make_shared<QGraphicsPathItem>();
                auto ringToDraw = std::dynamic_pointer_cast<QGraphicsPathItem>(itemToDraw);
                ringToDraw->setPen(QPen(Qt::green, 1, Qt::SolidLine));
                ringToDraw->setPath(path);
                scene()->addItem(ringToDraw.get());
            } else {
                auto ringToDraw = std::dynamic_pointer_cast<QGraphicsPathItem>(itemToDraw);
                ringToDraw->setPath(path);
            }
        }
    }
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        double angle = event->angleDelta().y();
        double factor = qPow(zoomFactorBase, angle);
        gentleZoom(factor);
    }
}


void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton || (mode == ViewMode::Pan && event->button() == Qt::LeftButton)) {
        pan = true;
        panStart = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    } else if ((mode == ViewMode::Line || mode == ViewMode::Rectangle || mode == ViewMode::Sector) &&
               event->button() == Qt::LeftButton) {
        if (itemToDraw) {
            // clear the existing item
            scene()->removeItem(itemToDraw.get());
            itemToDraw = nullptr;
        }
        if (scene()->sceneRect().contains(mapToScene(event->pos()))) {
            firstPoint = mapToScene(event->pos()).toPoint();
            emit firstPointChanged(firstPoint);
        }
    }
    event->ignore();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton || (mode == ViewMode::Pan && event->button() == Qt::LeftButton)) {
        pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    } else if (mode == ViewMode::Line || mode == ViewMode::Rectangle || mode == ViewMode::Sector) {
        // do nothing
    }
    event->ignore();
}

void MyGraphicsView::setViewMode(ViewMode mode)
{
    this->mode = mode;
    if (mode == ViewMode::Line || mode == ViewMode::Rectangle || mode == ViewMode::Sector) {
        if (itemToDraw) {
            scene()->removeItem(itemToDraw.get());
            itemToDraw = nullptr;
        }
    }
}

QPainterPath MyGraphicsView::createRing(QPointF center, double radius, double width, double theta, double phi)
{
    QPainterPath path(center);
    double alpha = (theta - phi / 2 - 90) * M_PI / 180;
    QRectF inner(QPointF(center.x() - (radius - width / 2), center.y() - (radius - width / 2)),
                 QPointF(center.x() + (radius - width / 2), center.y() + (radius - width / 2)));
    QRectF middle(QPointF(center.x() - radius, center.y() - radius),
                  QPointF(center.x() + radius, center.y() + radius));
    QRectF outer(QPointF(center.x() - (radius + width / 2), center.y() - (radius + width / 2)),
                 QPointF(center.x() + (radius + width / 2), center.y() + (radius + width / 2)));
    path.arcTo(outer, static_cast<int>(theta-phi/2-90), static_cast<int>(phi));
    path.closeSubpath();
    path.moveTo(QPointF(center.x() + (radius - width / 2) * cos(alpha), center.y() - (radius - width / 2) * sin(alpha)));
    path.arcTo(inner, static_cast<int>(theta-phi/2-90), static_cast<int>(phi));
    path.moveTo(QPointF(center.x() + radius * cos(alpha), center.y() - radius * sin(alpha)));
    path.arcTo(middle, static_cast<int>(theta-phi/2-90), static_cast<int>(phi));
    return path;
}

void MyGraphicsView::onFirstPointChange(QPoint newPoint)
{
   firstPoint = newPoint;
}

void MyGraphicsView::onSecondPointChange(QPoint newPoint)
{
   secondPoint = newPoint;
}

void MyGraphicsView::onWidthChange(int newWidth)
{
   width = newWidth;
}

void MyGraphicsView::onRadiusChange(int newRadius)
{
   radius = newRadius;
}

void MyGraphicsView::onThetaChange(int newTheta)
{
   theta = newTheta;
}

void MyGraphicsView::onPhiChange(int newPhi)
{
    phi = newPhi;
}
