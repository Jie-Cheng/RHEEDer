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
    ui->lineEdit_5->setText("40");
    ui->lineEdit_7->setText("0");
    ui->lineEdit_8->setText("60");
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
    connect(ui->lineEdit_5, &QLineEdit::textEdited, [&](){
        emit widthChanged(ui->lineEdit_5->text().toInt());
    });
    connect(ui->lineEdit_6, &QLineEdit::textEdited, [&](){
        emit radiusChanged(ui->lineEdit_6->text().toInt());
    });
    connect(ui->lineEdit_7, &QLineEdit::textEdited, [&](){
        emit thetaChanged(ui->lineEdit_7->text().toInt());
    });
    connect(ui->lineEdit_8, &QLineEdit::textEdited, [&](){
        emit phiChanged(ui->lineEdit_8->text().toInt());
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
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_6->setEnabled(false);
        ui->lineEdit_7->setEnabled(false);
        ui->lineEdit_8->setEnabled(false);
    } else if (mode == MyGraphicsView::ViewMode::Line) {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_6->setEnabled(false);
        ui->lineEdit_7->setEnabled(false);
        ui->lineEdit_8->setEnabled(false);
    } else if (mode == MyGraphicsView::ViewMode::Rectangle) {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
        ui->lineEdit_5->setEnabled(true);
        ui->lineEdit_6->setEnabled(false);
        ui->lineEdit_7->setEnabled(false);
        ui->lineEdit_8->setEnabled(false);
    } else if (mode == MyGraphicsView::ViewMode::Sector) {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
        ui->lineEdit_3->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_5->setEnabled(true);
        ui->lineEdit_6->setEnabled(true);
        ui->lineEdit_7->setEnabled(true);
        ui->lineEdit_8->setEnabled(true);
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
    ui->lineEdit_6->setText(QString::number(newRadius));
}
