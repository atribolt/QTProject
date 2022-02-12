#include <QFile>
#include <QLoggingCategory>

#include "Dvpl.h"
#include "DvplData.h"


const QLoggingCategory dvpl("DvplData");


DvplData::DvplData()
{
}


bool DvplData::loadFromFile(const QString& path)
{
  bool result = false;

  QFile file(path);
  if (file.exists()) {
    if (file.open(QFile::ReadOnly)) {
      QByteArray data = file.readAll();
      file.close();

      result = loadFromDvplData(data);
    }
    else {
      qCritical(dvpl) << "File didn't open for read:" << file.errorString();
    }
  }
  else {
    qCritical(dvpl) << "File isn't exists:" << path;
  }

  return result;
}


bool DvplData::loadFromDvplData(const QByteArray& data)
{
  bool result = false;

  if (data.size() > 20) {
    _data = Dvpl::Decode(data);
    result = true;
  }
  else {
    qCritical(dvpl) << "Data invalid: 'Size less or equal then 20'";
  }

  return result;
}


void DvplData::setTextData(const QByteArray& data)
{
  _data = data;
}


QByteArray DvplData::asDvpl() const
{
  return Dvpl::Encode(_data);
}


const QByteArray& DvplData::asData() const
{
  return _data;
}
