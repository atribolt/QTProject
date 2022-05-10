#include <limits>
#include <QtEndian>

#include "DvplSign.h"
#include "lz4-lite/crc32.h"

namespace
{
  const char MagicSignTitle[4] = {'D', 'V', 'P', 'L'};
  const int MagicSignIdeal = *(int*)(MagicSignTitle);
  const int SignLegth = 20;
}


DvplSign::DvplSign()
{
}

DvplSign::DvplSign(const QByteArray& originData, const QByteArray& compressedData)
{
  this->originSize = originData.size();
  this->compressSize = compressedData.size();
  this->compressLevel = CompressLevel((originSize > compressSize) * 2);
  this->crc32 = crc32_for_buffer(compressedData.data(), compressSize);
  this->magic = MagicSignIdeal;
}

bool DvplSign::loadFromRawData(const QByteArray& data)
{
  if (data.size() >= SignLegth) {
    const char* sign = data.data() + data.size() - SignLegth;
    memcpy(raw, sign, SignLegth);

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    sign.crc32        = qToBigEndian(sign.crc32);
    sign.originSize   = qToBigEndian(sign.originSize);
    sign.compressSize = qToBigEndian(sign.compressSize);
    sign.compress     = qToBigEndian(sign.compressSize);
#endif
  }

  return this->magic == MagicSignIdeal;
}

QByteArray DvplSign::toBytes() const
{
  QByteArray sign(20, '\0');

  for (int i = 0; i < SignLegth; i += 4) {
    *reinterpret_cast<uint32_t*>(sign.data() + i) = qToLittleEndian(*(uint32_t*)(raw + i));
  }

  return sign;
}
