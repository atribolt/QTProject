#ifndef UNPACK
#define UNPACK

#include <string>
#include <fstream>
#include <QString>

#include "lz4.h"
#include "crc32.h"

struct Data{
    char* unpack { nullptr };
    size_t size { 0 };
};

#if defined(__x86_64__)
constexpr size_t STEP = 8;
#elif
constexpr size_t STEP = 4;
#endif

bool Decode(const QString& path, QString& out);

bool Code(const QString& source, const QString& out);

#endif
