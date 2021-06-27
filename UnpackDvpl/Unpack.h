#pragma once

#include <QString>
#include <QByteArray>

enum CompressLevel : uint32_t {
  NoCompress = 0,
  SimpleCompress = 2
};

class DvplData {
public:
  DvplData();
  
  bool loadFromFile(QString const& path);
  bool loadFromDvplData(QByteArray const& data);
  void setTextData(QByteArray const& data);
  
  QByteArray asDvpl(CompressLevel lvl = CompressLevel::NoCompress) const;
  const QByteArray& asData() const; 
  
private:
  QByteArray _data;
};
