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

#include "src/persistence/settingsserializer.h"

#include <QtTest/QtTest>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QString>

class TestSettingsSerializer : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void defaultValueForMissingKey();
    void stringValueRoundTrip();
    void groupRoundTrip();
    void arrayRoundTrip();
    void malformedVarintDoesNotCrash();
    void truncatedFileDoesNotCrash();

private:
    QString testFile;
};

void TestSettingsSerializer::init()
{
    testFile = QDir::tempPath() + QStringLiteral("/xtox_settingsserializer_test.qts");
    QFile::remove(testFile);
}

void TestSettingsSerializer::cleanup()
{
    QFile::remove(testFile);
}

/**
 * @brief Reading a key that was never set must return the default value.
 */
void TestSettingsSerializer::defaultValueForMissingKey()
{
    SettingsSerializer serializer(testFile);
    QCOMPARE(serializer.value(QStringLiteral("missing"), QStringLiteral("fallback")).toString(),
             QStringLiteral("fallback"));
}

/**
 * @brief A value saved to disk must load back unchanged.
 */
void TestSettingsSerializer::stringValueRoundTrip()
{
    {
        SettingsSerializer serializer(testFile);
        serializer.setValue(QStringLiteral("key"), QStringLiteral("value"));
        serializer.save();
    }

    {
        SettingsSerializer serializer(testFile);
        serializer.load();
        QCOMPARE(serializer.value(QStringLiteral("key")).toString(), QStringLiteral("value"));
    }
}

/**
 * @brief Values in a nested group must load back into the same group.
 */
void TestSettingsSerializer::groupRoundTrip()
{
    {
        SettingsSerializer serializer(testFile);
        serializer.setValue(QStringLiteral("top"), QStringLiteral("topvalue"));
        serializer.beginGroup(QStringLiteral("group"));
        serializer.setValue(QStringLiteral("gkey"), QStringLiteral("gvalue"));
        serializer.endGroup();
        serializer.save();
    }

    {
        SettingsSerializer serializer(testFile);
        serializer.load();
        QCOMPARE(serializer.value(QStringLiteral("top")).toString(), QStringLiteral("topvalue"));
        serializer.beginGroup(QStringLiteral("group"));
        QCOMPARE(serializer.value(QStringLiteral("gkey")).toString(), QStringLiteral("gvalue"));
        serializer.endGroup();
    }
}

/**
 * @brief An array must preserve its size and per-index values.
 */
void TestSettingsSerializer::arrayRoundTrip()
{
    const int arraySize = 3;
    {
        SettingsSerializer serializer(testFile);
        serializer.beginWriteArray(QStringLiteral("array"), arraySize);
        for (int i = 0; i < arraySize; ++i) {
            serializer.setArrayIndex(i);
            serializer.setValue(QStringLiteral("value"), QString::number(i));
        }
        serializer.endArray();
        serializer.save();
    }

    {
        SettingsSerializer serializer(testFile);
        serializer.load();
        QCOMPARE(serializer.beginReadArray(QStringLiteral("array")), arraySize);
        for (int i = 0; i < arraySize; ++i) {
            serializer.setArrayIndex(i);
            QCOMPARE(serializer.value(QStringLiteral("value")).toString(), QString::number(i));
        }
        serializer.endArray();
    }
}

/**
 * @brief A length prefix with all continuation bits set must be bounded and must not over-read.
 */
void TestSettingsSerializer::malformedVarintDoesNotCrash()
{
    QByteArray data = QByteArray::fromHex("51544f58"); // "XTOX" magic
    data.append('\x01'); // GroupStart tag
    // A varint length that never terminates
    data.append('\xff');
    data.append('\xff');
    data.append('\xff');
    data.append('\xff');
    data.append('\xff');
    data.append('\xff');

    QFile file(testFile);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QCOMPARE(file.write(data), static_cast<qint64>(data.size()));
    file.close();

    SettingsSerializer serializer(testFile);
    serializer.load(); // must not read out of bounds
    QCOMPARE(serializer.value(QStringLiteral("missing"), QStringLiteral("fallback")).toString(),
             QStringLiteral("fallback"));
}

/**
 * @brief A record that promises more data than the file holds must not allocate or read it.
 */
void TestSettingsSerializer::truncatedFileDoesNotCrash()
{
    QByteArray data = QByteArray::fromHex("51544f58"); // "XTOX" magic
    data.append('\x02'); // ArrayStart tag
    data.append('\x01'); // length prefixed name ...
    data.append('a');    // ... "a"
    data.append('\x7f'); // array size 127, but nothing follows

    QFile file(testFile);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QCOMPARE(file.write(data), static_cast<qint64>(data.size()));
    file.close();

    SettingsSerializer serializer(testFile);
    serializer.load(); // must not read out of bounds
    QCOMPARE(serializer.value(QStringLiteral("missing"), QStringLiteral("fallback")).toString(),
             QStringLiteral("fallback"));
}

QTEST_GUILESS_MAIN(TestSettingsSerializer)
#include "settingsserializer_test.moc"
