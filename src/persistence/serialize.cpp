/*
    Copyright © 2014-2019 by The xTox Project Contributors

    This file is part of xTox, a Qt-based graphical interface for Tox.

    xTox is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    xTox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xTox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "src/persistence/serialize.h"

/**
 * @file serialize.cpp
 * Most of functions in this file are unsafe unless otherwise specified.
 * @warning Do not use them on untrusted data (e.g. check a signature first).
 */

namespace {

/**
 * @brief Decode a base-128 variable length integer.
 * @param data Byte array to read from.
 * @param maxBytes Maximum number of bytes to consume (5 for a 32 bit value,
 *                 10 for a 64 bit one).
 * @param bytesRead Set to the number of bytes actually consumed.
 * @return The decoded value, or 0 if the array is empty.
 * @note Reading stops at the end of the array or after maxBytes bytes, so a
 *       truncated or malicious varint can neither read out of bounds nor shift
 *       by more than the width of the return type.
 */
uint64_t decodeVuint(const QByteArray& data, int maxBytes, int& bytesRead)
{
    uint64_t num = 0;
    int shift = 0;
    bytesRead = 0;
    for (int i = 0; i < maxBytes; ++i) {
        if (bytesRead >= data.size())
            break;
        const uint8_t byte = static_cast<uint8_t>(data.at(bytesRead));
        ++bytesRead;
        num |= static_cast<uint64_t>(byte & 0x7f) << shift;
        if ((byte & 0x80) == 0)
            break;
        shift += 7;
    }
    return num;
}

} // namespace

QString dataToString(QByteArray data)
{
    int bytesRead = 0;
    const uint64_t strlen = decodeVuint(data, 5, bytesRead);

    if (strlen == 0)
        return QString();

    // Remove the strlen
    data.remove(0, bytesRead);
    if (strlen < static_cast<uint64_t>(data.size()))
        data.truncate(static_cast<int>(strlen));

    return QString::fromUtf8(data);
}

uint64_t dataToUint64(const QByteArray& data)
{
    if (data.size() < 8)
        return 0;

    return static_cast<uint64_t>(data[0]) | (static_cast<uint64_t>(data[1]) << 8)
           | (static_cast<uint64_t>(data[2]) << 16) | (static_cast<uint64_t>(data[3]) << 24)
           | (static_cast<uint64_t>(data[4]) << 32) | (static_cast<uint64_t>(data[5]) << 40)
           | (static_cast<uint64_t>(data[6]) << 48) | (static_cast<uint64_t>(data[7]) << 56);
}

int dataToVInt(const QByteArray& data)
{
    int bytesRead = 0;
    return static_cast<int32_t>(decodeVuint(data, 5, bytesRead));
}

size_t dataToVUint(const QByteArray& data)
{
    int bytesRead = 0;
    return static_cast<size_t>(decodeVuint(data, 10, bytesRead));
}

unsigned getVUint32Size(QByteArray data)
{
    int bytesRead = 0;
    decodeVuint(data, 5, bytesRead);
    return static_cast<unsigned>(bytesRead);
}

QByteArray vintToData(int num)
{
    // A 32 bit varint needs at most 5 bytes; a smaller buffer would be overrun
    QByteArray data(5, 0);
    // Write the size in a Uint of variable lenght (8-32 bits)
    int i = 0;
    while (num >= 0x80) {
        data[i] = static_cast<char>(num | 0x80);
        ++i;
        num = num >> 7;
    }
    data[i] = static_cast<char>(num);
    data.resize(i + 1);
    return data;
}

QByteArray vuintToData(size_t num)
{
    // A 64 bit varint needs at most 10 bytes; a smaller buffer would be overrun
    QByteArray data(10, 0);
    // Write the size in a Uint of variable lenght (8-32 bits)
    int i = 0;
    while (num >= 0x80) {
        data[i] = static_cast<char>(num | 0x80);
        ++i;
        num = num >> 7;
    }
    data[i] = static_cast<char>(num);
    data.resize(i + 1);
    return data;
}
