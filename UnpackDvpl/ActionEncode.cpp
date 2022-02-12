#include <QFile>
#include <QMutex>
#include <QFileInfo>
#include <QProgressBar>
#include <QLoggingCategory>

#include "DvplData.h"
#include "ActionEncode.h"


const QLoggingCategory encode("ActionEncode");


void ActionEncode::run()
{
  const bool& isStop = (stopFlag != nullptr) ? *stopFlag : false;

  for (int i = 0; i < files.size() && !isStop; ++i) {
    DvplData dvpl;

    {
      QFile file(files[i]);
      if (file.open(QFile::ReadOnly)) {
        dvpl.setTextData(file.readAll());
        file.close();
      }
      else {
        qCritical(encode) << "File didn't read:" << files[i];
        continue;
      }
    }

    QByteArray data = dvpl.asDvpl();

    QFileInfo fileOuput(files[i]);
    fileOuput.setFile(dstPath + '/' + fileOuput.fileName() + ".dvpl");

    if (mutex != nullptr) mutex->lock();
    QFile file(fileOuput.absoluteFilePath());
    if (file.open(QFile::WriteOnly)) {
      file.write(data);
      file.close();
    }


    if (progress != nullptr) {
      int value = progress->value();
      progress->setValue(value + 1);
    }

    if (mutex != nullptr) mutex->unlock();
  }
}
