#include "selectioncontrol.h"
#include "ui_selectioncontrol.h"
#include <QLayout>

SelectionControl::SelectionControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectionControl)
{
    ui->setupUi(this);
    setLayout(ui->horizontalLayout);
    // These are the default values
    ui->horizontalSlider->setValue(40); // width
    ui->horizontalSlider_3->setValue(0); // theta
    ui->horizontalSlider_4->setValue(60); // phi
    setViewMode(MyGraphicsView::ViewMode::Normal);
    auto getFirstPoint = [&](){
        emit firstPointChanged(QPoint(ui->lineEdit->text().toInt(),
                                      ui->lineEdit_2->text().toInt()));
    };
    connect(ui->lineEdit, &QLineEdit::textEdited, getFirstPoint);
    connect(ui->lineEdit_2, &QLineEdit::textEdited, getFirstPoint);
    auto getSecondPoint = [&](){
        emit secondPointChanged(QPoint(ui->lineEdit_3->text().toInt(),
                                       ui->lineEdit_4->text().toInt()));
    };
    connect(ui->lineEdit_3, &QLineEdit::textEdited, getSecondPoint);
    connect(ui->lineEdit_4, &QLineEdit::textEdited, getSecondPoint);
    connect(ui->horizontalSlider, &QAbstractSlider::valueChanged, [&](){
        emit widthChanged(ui->horizontalSlider->value());
    });
    connect(ui->horizontalSlider_2, &QAbstractSlider::valueChanged, [&](){
        emit radiusChanged(ui->horizontalSlider_2->value());
    });
    connect(ui->horizontalSlider_3, &QAbstractSlider::valueChanged, [&](){
        emit thetaChanged(ui->horizontalSlider_3->value());
    });
    connect(ui->horizontalSlider_4, &QAbstractSlider::valueChanged, [&](){
        emit phiChanged(ui->horizontalSlider_4->value());
    });
}

SelectionControl::~SelectionControl()
{
    delete ui;
}

void SelectionControl::setViewMode(MyGraphicsView::ViewMode mode)
{
    if (mode == MyGraphicsView::ViewMode::Normal || mode == MyGraphicsView::ViewMode::Pan) {
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
        ui->lineEdit_3->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        ui->horizontalSlider->setEnabled(false);
        ui->horizontalSlider_2->setEnabled(false);
        ui->horizontalSlider_3->setEnabled(false);
        ui->horizontalSlider_4->setEnabled(false);
    } else if (mode == MyGraphicsView::ViewMode::Line) {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
        ui->horizontalSlider->setEnabled(false);
        ui->horizontalSlider_2->setEnabled(false);
        ui->horizontalSlider_3->setEnabled(false);
        ui->horizontalSlider_4->setEnabled(false);
    } else if (mode == MyGraphicsView::ViewMode::Rectangle) {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        ui->horizontalSlider_2->setEnabled(false);
        ui->horizontalSlider_3->setEnabled(false);
        ui->horizontalSlider_4->setEnabled(false);
    } else if (mode == MyGraphicsView::ViewMode::Sector) {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_3->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        ui->horizontalSlider->setEnabled(true);
        ui->horizontalSlider_2->setEnabled(true);
        ui->horizontalSlider_3->setEnabled(true);
        ui->horizontalSlider_4->setEnabled(true);
    }
}

void SelectionControl::onFirstPointChange(QPoint newPoint)
{
    ui->lineEdit->setText(QString::number(newPoint.x()));
    ui->lineEdit_2->setText(QString::number(newPoint.x()));
}

void SelectionControl::onSecondPointChange(QPoint newPoint)
{
    ui->lineEdit_3->setText(QString::number(newPoint.x()));
    ui->lineEdit_4->setText(QString::number(newPoint.x()));
}

void SelectionControl::onRadiusChange(int newRadius)
{
    ui->horizontalSlider_2->setValue(newRadius);
}

void SelectionControl::adjustRadiusRange(int maxRadius)
{
    ui->horizontalSlider_2->setMaximum(maxRadius);
}
