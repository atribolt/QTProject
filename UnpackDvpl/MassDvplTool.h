#ifndef MASSDVPLTOOL_H
#define MASSDVPLTOOL_H

#include <QMutex>
#include <QMainWindow>
#include <QThreadPool>

QT_BEGIN_NAMESPACE
namespace Ui { class MassDvplTool; }
QT_END_NAMESPACE

class MassDvplTool final : public QMainWindow
{
  Q_OBJECT
public:
  MassDvplTool(QWidget *parent = nullptr);
  ~MassDvplTool();
  
private slots:
  void changeSrcPath();
  void changeDstPath();
  
  void stopProcess();
  void startProcess();
  
  void blockWidgets();
  void activeWidgets();
  
private:
  bool canBeStart();
  
  void init();
  QString changePath() const;
  QVector<QStringList> getFilesGroups(QString path, QStringList filter, int* files = nullptr);
  QVector<class QRunnable*> createActions();
  
private:
  bool _stopFlag;
  QMutex _mutex;
  QThreadPool _threadPool;
  Ui::MassDvplTool* ui;
  QVector<QWidget*> _blockList;
};

#endif // MASSDVPLTOOL_H
