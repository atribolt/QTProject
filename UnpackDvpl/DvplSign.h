#ifndef DVPLSIGN_H
#define DVPLSIGN_H

#include <QByteArray>


enum CompressLevel
  : uint32_t
{
  NoCompress = 0,
  SimpleCompress = 2
};


struct DvplSign
{
  union {
    struct {
      uint32_t originSize         {  0 };
      uint32_t compressSize       {  0 };
      uint32_t crc32              {  0 };
      CompressLevel compressLevel { CompressLevel::NoCompress };
      uint32_t magic;
    };
    char raw[20];
  };

  DvplSign();
  DvplSign(const QByteArray& originData,
           const QByteArray& compressedData);
  bool loadFromRawData(QByteArray const& data);

  QByteArray toBytes() const;
};

#endif // DVPLSIGN_H
