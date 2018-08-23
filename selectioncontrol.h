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

public slots:
    void onViewModeChange(MyGraphicsView::ViewMode);

private:
    Ui::SelectionControl *ui;
};

#endif // SELECTIONCONTROL_H
