#include <QFile>

#include "Unpack.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MassDvplTool.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QObject::connect(ui->menu_MassDvplTool, &QAction::triggered, this, &MainWindow::massDvplTool);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_menu_FileOpen_triggered() {
  windowTitle = "DVPL edit";
  _file = QFileDialog::getOpenFileName(this, "Открыть", QDir::homePath(), "DVPL (*.dvpl)");
  
  DvplData dvpl;
  if (!dvpl.loadFromFile(_file)) {
    QMessageBox::critical(this, "Ошибка", "Файл не был загружен");
  }
  
  ui->textEdit->setText(dvpl.asData());
}

void MainWindow::UpdateTitle() {
  if(!_file.isEmpty())
    this->setWindowTitle(windowTitle + ": " + QFileInfo(_file).fileName());
  else
    this->setWindowTitle(windowTitle);
}

void MainWindow::massDvplTool() {
  MassDvplTool* window = new MassDvplTool();
  window->show();
  
  QObject::connect(window, &MassDvplTool::destroyed, window, &MassDvplTool::deleteLater);
}

void MainWindow::on_menu_Save_triggered()
{
  const QString EXT_TXT = "txt";
  const QString EXT_DVPL = "dvpl";
  
  QByteArray data; 
  QString extension = QFileInfo(_file).suffix();
  
  DvplData dvpl;
  dvpl.setTextData(ui->textEdit->toPlainText().toUtf8());
  
  if (extension.compare(EXT_TXT, Qt::CaseInsensitive) == 0) {
    data = dvpl.asData();
  }
  else if (QFileInfo(_file).suffix().compare(EXT_DVPL, Qt::CaseInsensitive) == 0) {
    CompressLevel lvl = CompressLevel::NoCompress;
    
    if (dvpl.asData().size() > 64)
      lvl = CompressLevel::SimpleCompress;
    
    data = dvpl.asDvpl(lvl);  
  }
  
  QFile file(_file);
  if (file.open(QFile::WriteOnly)) {
    file.write(data);
    file.close();
  }
  else {
    QMessageBox::critical(this, "Ошибка", "Файл не был сохранен:" + file.errorString());
  }
  
  UpdateTitle();
}

void MainWindow::on_menu_SaveAss_triggered() {
  QString fileForSave = QFileDialog::getSaveFileName(this, "Сохранить как", QDir::homePath(), "DVPL (*.dvpl);;Txt (*.txt)");
  
  if (!fileForSave.isEmpty()) {
    _file = fileForSave;
    on_menu_Save_triggered();
  }
  else {
    QMessageBox::critical(this, "Ошибка", "Файл не был сохранен: 'Пустое имя файла'");
  }
  
  UpdateTitle();
}
