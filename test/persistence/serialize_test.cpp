/*
    Copyright © 2019-2026 by The xTox Project Contributors

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

#include <QtTest/QtTest>
#include <QByteArray>
#include <QString>
#include <cstddef>
#include <tuple>

class TestSerialize : public QObject
{
    Q_OBJECT
private slots:
    void vintRoundTrip();
    void vuintRoundTrip();
    void vintEncodingIsMinimal();
    void dataToVIntEmpty();
    void dataToVIntTruncated();
    void dataToVUintTruncated();
    void dataToUint64TooShort();
    void dataToUint64Valid();
    // NOTE: these slots must not be named exactly like the free functions under
    // test (`getVUint32Size`, `dataToString`), otherwise the unqualified calls
    // inside the test members would resolve to the zero-argument slot and shadow
    // the global declaration.
    void getVUint32SizeValid();
    void getVUint32SizeTruncated();
    void dataToStringRoundTrip();
    void dataToStringEmpty();
    void dataToStringMalformed();
};

/**
 * @brief Every encodable 32 bit value must survive vintToData -> dataToVInt.
 */
void TestSerialize::vintRoundTrip()
{
    const int values[] = {0,          1,       0x7e,       0x7f,       0x80,
                          0x81,       0x3ffe,  0x3fff,     0x4000,     0x1fffff,
                          0x200000,   0x0fffffff, 0x10000000, 0x7ffffffe, 0x7fffffff};

    for (int value : values) {
        const QByteArray encoded = vintToData(value);
        QVERIFY(encoded.size() >= 1);
        QVERIFY(encoded.size() <= 5);
        QCOMPARE(dataToVInt(encoded), value);
    }
}

/**
 * @brief Every encodable 64 bit value must survive vuintToData -> dataToVUint.
 */
void TestSerialize::vuintRoundTrip()
{
    const size_t values[] = {0,
                             1,
                             0x7f,
                             0x80,
                             0x3fff,
                             0x4000,
                             0x1fffff,
                             0x200000,
                             0x0fffffff,
                             0x10000000,
                             static_cast<size_t>(0x7fffffffffffffffULL)};

    for (size_t value : values) {
        const QByteArray encoded = vuintToData(value);
        QVERIFY(encoded.size() >= 1);
        QVERIFY(encoded.size() <= 10);
        QCOMPARE(static_cast<qulonglong>(dataToVUint(encoded)), static_cast<qulonglong>(value));
    }
}

/**
 * @brief The encoding must not contain redundant continuation bytes.
 */
void TestSerialize::vintEncodingIsMinimal()
{
    // 0x7f fits in a single byte
    QCOMPARE(vintToData(0x7f), QByteArray::fromHex("7f"));
    // 0x80 needs a continuation byte
    QCOMPARE(vintToData(0x80), QByteArray::fromHex("8001"));
    // 0x3fff is the largest two byte value
    QCOMPARE(vintToData(0x3fff), QByteArray::fromHex("ff7f"));
    // 0x4000 needs a third byte
    QCOMPARE(vintToData(0x4000), QByteArray::fromHex("808001"));
}

/**
 * @brief Reading a varint out of an empty array must not read out of bounds.
 */
void TestSerialize::dataToVIntEmpty()
{
    QCOMPARE(dataToVInt(QByteArray()), 0);
    QCOMPARE(static_cast<int>(getVUint32Size(QByteArray())), 0);
}

/**
 * @brief A varint prefix that never terminates must not over-read or shift past 32 bits.
 */
void TestSerialize::dataToVIntTruncated()
{
    // All continuation bits set, but the array ends after four bytes
    const QByteArray truncated = QByteArray::fromHex("ffffffff");
    QVERIFY(static_cast<int>(getVUint32Size(truncated)) == truncated.size());
    // Must not crash; the exact value is unspecified for malformed input
    std::ignore = dataToVInt(truncated);
    std::ignore = dataToVUint(truncated);
}

/**
 * @brief dataToVUint must not shift by more than the width of size_t.
 */
void TestSerialize::dataToVUintTruncated()
{
    // Ten continuation bytes: the reader is capped at ten bytes
    const QByteArray truncated = QByteArray::fromHex("80808080808080808080");
    std::ignore = dataToVUint(truncated);
    QVERIFY(true);
}

/**
 * @brief dataToUint64 must return 0 rather than read past the end of a short array.
 */
void TestSerialize::dataToUint64TooShort()
{
    QCOMPARE(dataToUint64(QByteArray()), quint64(0));
    QCOMPARE(dataToUint64(QByteArray::fromHex("01020304050607")), quint64(0));
}

/**
 * @brief dataToUint64 must decode exactly eight bytes, little endian.
 */
void TestSerialize::dataToUint64Valid()
{
    const QByteArray data = QByteArray::fromHex("0102030405060708");
    QCOMPARE(dataToUint64(data), quint64(0x0807060504030201ULL));
}

/**
 * @brief getVUint32Size must return the number of bytes of a well formed prefix.
 */
void TestSerialize::getVUint32SizeValid()
{
    QCOMPARE(static_cast<int>(getVUint32Size(QByteArray::fromHex("00"))), 1);
    QCOMPARE(static_cast<int>(getVUint32Size(QByteArray::fromHex("7f"))), 1);
    QCOMPARE(static_cast<int>(getVUint32Size(QByteArray::fromHex("8001"))), 2);
    QCOMPARE(static_cast<int>(getVUint32Size(QByteArray::fromHex("808001"))), 3);
    // A 32 bit prefix never needs more than five bytes
    QCOMPARE(static_cast<int>(getVUint32Size(QByteArray::fromHex("8080808080"))), 5);
}

/**
 * @brief A truncated prefix must be bounded by the array length.
 */
void TestSerialize::getVUint32SizeTruncated()
{
    QCOMPARE(static_cast<int>(getVUint32Size(QByteArray::fromHex("80"))), 1);
    QCOMPARE(static_cast<int>(getVUint32Size(QByteArray::fromHex("8080"))), 2);
}

/**
 * @brief A well formed length prefixed string must round trip.
 */
void TestSerialize::dataToStringRoundTrip()
{
    QByteArray data = vintToData(5);
    data.append("hello", 5);
    QCOMPARE(dataToString(data), QStringLiteral("hello"));

    // A declared length longer than the payload must return what is available
    QByteArray longer = vintToData(100);
    longer.append("ab", 2);
    QCOMPARE(dataToString(longer), QStringLiteral("ab"));
}

/**
 * @brief Empty input and a zero length must both yield an empty string.
 */
void TestSerialize::dataToStringEmpty()
{
    QCOMPARE(dataToString(QByteArray()), QString());
    QCOMPARE(dataToString(vintToData(0)), QString());
}

/**
 * @brief A never terminating prefix must not read out of bounds.
 */
void TestSerialize::dataToStringMalformed()
{
    const QByteArray malformed = QByteArray::fromHex("ffffffff7f");
    QCOMPARE(dataToString(malformed), QString());
}

QTEST_GUILESS_MAIN(TestSerialize)
#include "serialize_test.moc"
