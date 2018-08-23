#include "selectioncontrol.h"
#include "ui_selectioncontrol.h"
#include <QLayout>

SelectionControl::SelectionControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectionControl)
{
    ui->setupUi(this);
    setLayout(ui->horizontalLayout);
}

SelectionControl::~SelectionControl()
{
    delete ui;
}

void SelectionControl::onViewModeChange(MyGraphicsView::ViewMode mode)
{
    if (mode == MyGraphicsView::ViewMode::Normal) {
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
