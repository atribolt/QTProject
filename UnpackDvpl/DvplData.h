#pragma once

#include <QString>


class DvplData
{
public:
  DvplData();

  bool loadFromFile(QString const& path);
  bool loadFromDvplData(QByteArray const& data);
  void setTextData(QByteArray const& data);

  QByteArray asDvpl() const;
  const QByteArray& asData() const;

private:
  QByteArray _data;
};
