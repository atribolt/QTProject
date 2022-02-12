#pragma once

#include <QRunnable>
#include <QStringList>

class ActionEncode
  : public QRunnable
{
public:
  virtual void run();

  bool* stopFlag = { nullptr };
  class QMutex* mutex = { nullptr };
  class QProgressBar* progress = { nullptr };

  QString dstPath;
  QStringList files;
};
