#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtGui>
#include "Unpack.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateTitle();
private slots:
    void on_menu_FileOpen_triggered();
    void on_menu_Save_triggered();
    void on_menu_SaveAss_triggered();

    void on_textEdit_textChanged();

private:
    QString file;
    QString editData;
    QString windowTitle;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
