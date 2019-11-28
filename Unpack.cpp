#include "Unpack.h"

std::uint32_t crc32_for_buffer(const char* data, std::uint32_t size)
{
    std::uint32_t crc32 = 0xffffffff;
    for (std::uint32_t i = 0; i < size; ++i)
    {
        crc32 = (crc32 >> 8) ^ crc32_tab[(crc32 ^ static_cast<uint8_t>(data[i])) & 0xff];
    }
    crc32 ^= 0xffffffff;
    return crc32;
}

uint32_t ReadLittleEndian(const uint8_t* data) {
    uint32_t res = data[3];

    for(int i = 2; i >= 0; --i){
        res <<= STEP;
        res |= data[i];
    }

    return res;
}
uint32_t ReadBigEndian(const uint8_t* data) {
    uint32_t res = data[0];

    for(int i = 1; i < 4; ++i) {
        res <<= STEP;
        res |= data[i];
    }

    return res;
}

void ToLittleEndian(const uint32_t& data, char* out) {

    for(size_t i = 0; i < 4; ++i) {
        uint32_t res = data >> STEP * i ;
        out[i] = (uint8_t)res;
    }
}

void TpBigEndian(const uint32_t& data, char* out){

    for(int i = 3, o = 0; i >= 0; --i, ++o){
        uint32_t res = data >> STEP * i;
        out[o] = (uint8_t)res;
    }
}

bool Decode(const QString& path, QString& out) {
    using IS = std::ifstream;
    out.clear();

    IS in(path.toLocal8Bit(), std::ios_base::binary);

    if(!in) return false;

    in.seekg(-20, std::ios_base::end);

    uint8_t* sign = new uint8_t[20];
    in.read(reinterpret_cast<char*>(sign), 20);

    out.resize(ReadLittleEndian(sign), '\0');

    uint32_t sizePress = ReadLittleEndian(sign + 4);
    char* compress = new char[sizePress];
    char* decompress = new char[out.size()];

    in.seekg(0);
    in.read(compress, sizePress);

    LZ4_decompress_safe(
                compress,
                decompress,
                static_cast<int>(sizePress),
                static_cast<int>(out.size()));

    out = decompress;
    in.close();

    delete[] decompress;
    delete[] compress;
    delete[] sign;

    return true;
}


bool Code(const QString& source, const QString& out) {
    using OS = std::ofstream;

    OS result(out.toLocal8Bit().data(), std::ios_base::binary);

    if(!result) throw "Error";

    char* pack = new char[source.size()];
    auto size = LZ4_compress_default(
                source.toStdString().data(),
                pack,
                static_cast<int>(source.size()),
                static_cast<int>(source.size()));

    uint32_t crc32 = crc32_for_buffer(pack, static_cast<uint32_t>(size));
    result.write(pack, size);
    delete[] pack;

    pack = new char[20];

    ToLittleEndian(source.size(), pack);
    ToLittleEndian(size, pack + 4);
    ToLittleEndian(crc32, pack + 8);

    *(pack + 12    ) = 0;
    *(pack + 12 + 1) = 0;
    *(pack + 12 + 2) = 0;
    *(pack + 12 + 3) = 0;

    *(pack + 16    ) = 'D';
    *(pack + 16 + 1) = 'V';
    *(pack + 16 + 2) = 'P';
    *(pack + 16 + 3) = 'L';

    result.write(pack, 20);
    result.close();

    return true;
}

