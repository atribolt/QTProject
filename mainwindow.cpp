#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <fstream>
#include <stdio.h>

void ShowError(const QString& message){
    QMessageBox::critical(nullptr, QObject::tr("Ошибка"), message);
}

void MainWindow::on_menu_FileOpen_triggered()
{
    windowTitle = "DVPL edit";
    file = QFileDialog::getOpenFileName(this, "Открыть", QDir::homePath(), "DVPL (*.dvpl)");

    if(!Decode(file, editData)) {
        ShowError("Файл не может быть открыт");
        editData.clear();
        file.clear(); // clear path
    }
    UpdateTitle();

    ui->textEdit->setText(editData);
}

void MainWindow::UpdateTitle(){
    if(!file.isEmpty())
        this->setWindowTitle(windowTitle + ": " + file);
    else
        this->setWindowTitle(windowTitle);
}

void MainWindow::on_menu_Save_triggered()
{
    if(!Code(editData, file)){
        ShowError("Файл не был соханен");
    }

    UpdateTitle();
}

void MainWindow::on_menu_SaveAss_triggered()
{
    QString save = QFileDialog::getSaveFileName(this, "Сохранить", QDir::homePath(), "All file (*.*)");

    if(!Code(editData, save.toStdString().data())){
        ShowError("Файл не был сохранен");
    } else {
        file = save;
    }
    UpdateTitle();
}

void MainWindow::on_textEdit_textChanged()
{
    editData = ui->textEdit->toPlainText();
}
