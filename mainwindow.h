#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_spinN_valueChanged(int val);
    void on_spinTop_valueChanged(double val);
    void on_spinBase_valueChanged(double val);
    void on_spinHeight_valueChanged(double val);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
