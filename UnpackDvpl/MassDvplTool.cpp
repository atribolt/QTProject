#include <QMutex>
#include <QRunnable>
#include <QMessageBox>
#include <QFileDialog>
#include <QLoggingCategory>

#include "ActionEncode.h"
#include "ActionDecode.h"
#include "Unpack.h"
#include "MassDvplTool.h"
#include "ui_PacketUnpack.h"

const QLoggingCategory massDvplTool("MassDvplTool");

MassDvplTool::MassDvplTool(QWidget *parent) 
  : QMainWindow(parent) 
  , ui(new Ui::MassDvplTool())
{
  ui->setupUi(this);
  init();
}

MassDvplTool::~MassDvplTool() {
  delete ui;
}

void MassDvplTool::changeSrcPath() {
  QString path = changePath();
  ui->tb_SrcPath->setText(path);
  
  if(ui->tb_DstPath->toPlainText().isEmpty()) {
    ui->tb_DstPath->setText(path + "/output");
  }
}

void MassDvplTool::changeDstPath() {
  QString path = changePath();
  ui->tb_DstPath->setText(path);
}

void MassDvplTool::stopProcess() {
  _stopFlag = true;
  _threadPool.waitForDone();
  
  activeWidgets();
}

void MassDvplTool::startProcess() {
  if (canBeStart()) {
    _stopFlag = false;
    
    if (!QDir(ui->tb_DstPath->toPlainText()).exists())
      QDir().mkpath(ui->tb_DstPath->toPlainText());
    
    ui->status->setValue(0);
    
    QVector<QRunnable*> actions = createActions();
    if (actions.size() != 0) {
      for (QRunnable* action : actions) {
        _threadPool.start(action);
      }
      
      blockWidgets();
      
      while (!_threadPool.waitForDone(100)) {
        qApp->processEvents(QEventLoop::AllEvents);
      }
      
      activeWidgets();
    }
    else {
      QMessageBox::critical(this, "Ошибка", "В папке нет файлов для обработки");
    }
  }
}

void MassDvplTool::blockWidgets() {
  for (QWidget* widget : _blockList) {
    widget->setEnabled(false);
  }
}

void MassDvplTool::activeWidgets() {
  for (QWidget* widget : _blockList) {
    widget->setEnabled(true);
  }
}

bool MassDvplTool::canBeStart() {
  QString srcPath = ui->tb_SrcPath->toPlainText();
  QString dstPath = ui->tb_DstPath->toPlainText();
  
  if (!QDir(srcPath).exists()) {
    QMessageBox::critical(this, "Ошибка", "Входной путь не задан или не существует");
    return false;
  }
  if (dstPath.isEmpty()) {
    QMessageBox::critical(this, "Ошибка", "Выходной путь не задан");
    return false;
  }
  
  return true;
}

void MassDvplTool::init() {
  QObject::connect(ui->pb_SrcChange, &QPushButton::clicked, this, &MassDvplTool::changeSrcPath);
  QObject::connect(ui->pb_DstChange, &QPushButton::clicked, this, &MassDvplTool::changeDstPath);
  QObject::connect(ui->btn_Stop, &QPushButton::clicked, this, &MassDvplTool::stopProcess);
  QObject::connect(ui->btn_Start, &QPushButton::clicked, this, &MassDvplTool::startProcess);
  
  _blockList.append(ui->btn_Start);
  _blockList.append(ui->pb_DstChange);
  _blockList.append(ui->pb_SrcChange);
  _blockList.append(ui->rb_Pack);
  _blockList.append(ui->rb_Unpack);
  
  ui->tb_DstPath->setLineWrapMode(QTextEdit::NoWrap);
  ui->tb_SrcPath->setLineWrapMode(QTextEdit::NoWrap);
}

QString MassDvplTool::changePath() const {
  return QFileDialog::getExistingDirectory(nullptr, "Выберите каталог");
}

QVector<QStringList> MassDvplTool::getFilesGroups(QString path, QStringList filter, int* fileCount) {
  QVector<QStringList> result;  
  
  QFileInfoList files = QDir(path).entryInfoList(filter, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
  int filesPerThread = QThread::idealThreadCount();

  int i = 0;
  for (; (i + filesPerThread) < files.size(); i += filesPerThread) {
    result.push_back({});
    for ( auto& file : files.mid(i, filesPerThread)) {
      result.back().push_back(file.absoluteFilePath());
    }
  }
  
  result.push_back({});
  for ( auto& file : files.mid(i)) {
    result.back().push_back(file.absoluteFilePath());
  }
  
  if (fileCount != nullptr) 
    *fileCount = files.size();
  
  return result;
}

QVector<QRunnable*> MassDvplTool::createActions() {
  QVector<QRunnable*> result;
  
  QString srcPath = ui->tb_SrcPath->toPlainText();
  QString dstPath = ui->tb_DstPath->toPlainText();
  
  auto makeActions = [&](auto actiontype, QStringList filter) mutable {
    int count = 0;
    QVector<QStringList> filesGroup = getFilesGroups(srcPath, filter, &count);
    
    if (count != 0) {
      ui->status->setMaximum(count);
      for (QStringList const& files : filesGroup) {
        auto action = new decltype(actiontype)();
        action->files = files;
        action->dstPath = dstPath;
        action->mutex = &_mutex;
        action->progress = ui->status;
        
        result.append(action);
      }
    }
  };
  
  if (ui->rb_Unpack->isChecked()) {
    makeActions(ActionDecode(), { "*.dvpl" });
  }
  else {
    makeActions(ActionEncode(), { "*" });
  }
  
  return result;
}
