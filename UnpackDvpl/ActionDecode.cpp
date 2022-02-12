#include <QFile>
#include <QMutex>
#include <QFileInfo>
#include <QProgressBar>
#include <QLoggingCategory>

#include "DvplData.h"
#include "ActionDecode.h"


const QLoggingCategory decode("ActionDecode");


void ActionDecode::run()
{
  const bool& isStop = (stopFlag != nullptr) ? *stopFlag : false;

  for (int i = 0; i < files.size() && !isStop; ++i) {
    DvplData dvpl;
    dvpl.loadFromFile(files[i]);

    QByteArray data = dvpl.asData();
    if (data.isEmpty()) {
      qWarning(decode) << "File isn't DVPL:" << files[i];
    }

    QFileInfo fileOuput(files[i]);
    QString newName = fileOuput.fileName();
    newName.resize(newName.lastIndexOf('.'));

    fileOuput.setFile(dstPath + '/' + newName);

    QFile file(fileOuput.absoluteFilePath());
    if (file.open(QFile::WriteOnly)) {
      file.write(data);
      file.close();
    }

    if (mutex != nullptr) mutex->lock();

    if (progress != nullptr) {
      int value = progress->value();
      progress->setValue(value + 1);
    }

    if (mutex != nullptr) mutex->unlock();
  }
}
