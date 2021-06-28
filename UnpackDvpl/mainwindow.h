#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateTitle();
private slots:
    void massDvplTool();
    void on_menu_FileOpen_triggered();
    void on_menu_Save_triggered();
    void on_menu_SaveAss_triggered();

private:
    QString _file;
    QString windowTitle;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
