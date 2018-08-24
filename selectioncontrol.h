#ifndef SELECTIONCONTROL_H
#define SELECTIONCONTROL_H

#include <QWidget>
#include "mygraphicsview.h"

namespace Ui {
class SelectionControl;
}

class SelectionControl : public QWidget
{
    Q_OBJECT

public:
    explicit SelectionControl(QWidget *parent = nullptr);
    ~SelectionControl();
    void setViewMode(MyGraphicsView::ViewMode);

public slots:
    void onFirstPointChange(QPoint);
    void onSecondPointChange(QPoint);
    void onRadiusChange(int);

signals:
    void firstPointChanged(QPoint);
    void secondPointChanged(QPoint);
    void widthChanged(int);
    void radiusChanged(int);
    void thetaChanged(int);
    void phiChanged(int);

private:
    Ui::SelectionControl *ui;
};

#endif // SELECTIONCONTROL_H
