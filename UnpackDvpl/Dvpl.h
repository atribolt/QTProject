#pragma once

#include <QByteArray>

#include "DvplSign.h"


namespace Dvpl
{
  QByteArray Decode(const QByteArray& data);
  QByteArray Encode(const QByteArray& data);
}
