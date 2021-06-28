#pragma once

#include <QRunnable>
#include <QStringList>

class ActionDecode : public QRunnable
{
public:
  virtual void run();
  
  bool* stopFlag = { nullptr };               // all pointer wont be release
  class QMutex* mutex = { nullptr };          // 
  class QProgressBar* progress = { nullptr }; //
  
  QString dstPath;
  QStringList files;
};
