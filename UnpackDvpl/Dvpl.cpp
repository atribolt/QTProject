#include "Dvpl.h"
#include "lz4-lite/crc32.h"
#include "lz4-lite/lz4-lite.h"


QByteArray Dvpl::Decode(const QByteArray& data)
{
  if (data.size() <= 20) {
    return {};
  }

  DvplSign sign;
  if (!sign.loadFromRawData(data)) {
    return {};
  }

  QByteArray compress = data.mid(0, sign.compressSize);
  QByteArray decompress(sign.originSize, '\0');

  if (sign.compressLevel != CompressLevel::NoCompress) {
    LZ4_decompress_safe(
          compress.data(),
          decompress.data(),
          compress.size(),
          decompress.size());
  }
  else {
    decompress = compress;
  }

  return decompress;
}

QByteArray Dvpl::Encode(const QByteArray& data)
{
  QByteArray compressed = data;
  CompressLevel cLevel = CompressLevel((data.size() > 64) * 2);

  if (cLevel != CompressLevel::NoCompress) {
    int size = LZ4_compress_default(
                    data.data(),
                    compressed.data(),
                    static_cast<int>(data.size()),
                    static_cast<int>(data.size()));
    compressed.resize(size);
  }

  compressed.append(DvplSign(data, compressed).toBytes());
  return compressed;
}
