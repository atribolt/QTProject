#include <QFile>
#include <QtEndian>
#include <QLoggingCategory>

#include "lz4.h"
#include "crc32.h"
#include "Unpack.h"

namespace {
  
template<class T>
  T toLittleEndian(T in) {
    T result = in;  
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    result = qToLittleEndian(in);
#endif  
    return result;
  }
}

struct Sign {
  uint32_t originSize    {  0 };
  uint32_t compressSize  {  0 };
  uint32_t crc32         {  0 };
  CompressLevel compress { CompressLevel::NoCompress };
  char magic[4]          { 'D', 'V', 'P', 'L' };
  
  bool loadFromRawData(QByteArray const& data) {
    if (data.size() >= 20) {
      const char* sign = data.data() + data.size() - 20;
      *this = *reinterpret_cast<const Sign*>(sign);
      
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
      sign.crc32        = qToBigEndian(sign.crc32);
      sign.originSize   = qToBigEndian(sign.originSize);
      sign.compressSize = qToBigEndian(sign.compressSize);
      sign.compress     = qToBigEndian(sign.compressSize);
#endif
    }
    
    std::string DVPL = "DVPL";
    return (std::equal(magic, magic + sizeof(magic), DVPL.begin()));
  }
  
  QByteArray toBytes() const {
    QByteArray sign(16, '\0');
    *reinterpret_cast<uint32_t*>(sign.data() +  0) = toLittleEndian(originSize);
    *reinterpret_cast<uint32_t*>(sign.data() +  4) = toLittleEndian(compressSize);
    *reinterpret_cast<uint32_t*>(sign.data() +  8) = toLittleEndian(crc32);
    *reinterpret_cast<uint32_t*>(sign.data() + 12) = toLittleEndian(compress);
    sign.append(magic);
    
    return sign;
  }
};
namespace DVPL {
  QByteArray Decode(const QByteArray& data) {
    if (data.size() <= 20) {
      return {};
    }
    
    Sign sign;
    if (!sign.loadFromRawData(data)) {
      return {};
    }
    
    QByteArray compress = data.mid(0, sign.compressSize);
    QByteArray decompress(sign.originSize, '\0');
    
    if (sign.compress != CompressLevel::NoCompress) {
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
  
  QByteArray Encode(const QByteArray& data, CompressLevel lvl = CompressLevel::NoCompress) {
    QByteArray pack = data;
    
    if (lvl != CompressLevel::NoCompress) {
      int size = LZ4_compress_default(
            data.data(),
            pack.data(),
            static_cast<int>(data.size()),
            static_cast<int>(data.size()));
      pack.resize(size);
    }
    
    Sign sign;
    sign.originSize = uint32_t(data.size());
    sign.compressSize = uint32_t(pack.size());
    sign.crc32 = crc32_for_buffer(pack.data(), pack.size());
    sign.compress = lvl;
  
    pack.append(sign.toBytes());
    return pack;
  }
}

const QLoggingCategory dvpl("DvplData");

DvplData::DvplData() {}

bool DvplData::loadFromFile(const QString& path) {
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

bool DvplData::loadFromDvplData(const QByteArray& data) {
  bool result = false;
  
  if (data.size() > 20) {
    _data = DVPL::Decode(data);
    result = true;
  }
  else {
    qCritical(dvpl) << "Data invalid: 'Size less or equal then 20'";
  }
  
  return result;
}

void DvplData::setTextData(const QByteArray& data) {
  _data = data;
}

QByteArray DvplData::asDvpl(CompressLevel lvl) const {
  return DVPL::Encode(_data, lvl);
}

const QByteArray& DvplData::asData() const {
  return _data;
}
