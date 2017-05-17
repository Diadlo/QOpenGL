#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSpinBox>
#include <QCheckBox>
#include <QDoubleSpinBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinN->setValue(ui->opengl->getN());
    ui->spinTop->setValue(ui->opengl->getTopRaius());
    ui->spinBase->setValue(ui->opengl->getBottomRadius());
    ui->spinHeight->setValue(ui->opengl->getHeight());
    ui->cbTransparent->setChecked(ui->opengl->getTransparent());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinN_valueChanged(int val)
{
    ui->opengl->setN(val);
}

void MainWindow::on_spinTop_valueChanged(double val)
{
    ui->opengl->setTopRadius(val);
}

void MainWindow::on_spinBase_valueChanged(double val)
{
    ui->opengl->setBottomRadius(val);
}

void MainWindow::on_spinHeight_valueChanged(double val)
{
    ui->opengl->setHeight(val);
}

void MainWindow::on_cbTransparent_clicked(bool checked)
{
    ui->opengl->setTransparent(checked);
}
