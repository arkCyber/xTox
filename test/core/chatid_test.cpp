/*
    Copyright © 2017-2019 by The xTox Project Contributors

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

#include "src/core/chatid.h"
#include "src/core/toxpk.h"
#include "src/core/toxid.h"
#include "src/core/groupid.h"

#include <QtTest/QtTest>
#include <QByteArray>
#include <QString>

const uint8_t testPkArray[32] = {0xC7, 0x71, 0x9C, 0x68, 0x08, 0xC1, 0x4B, 0x77, 0x34, 0x80, 0x04,
                                 0x95, 0x6D, 0x1D, 0x98, 0x04, 0x6C, 0xE0, 0x9A, 0x34, 0x37, 0x0E,
                                 0x76, 0x08, 0x15, 0x0E, 0xAD, 0x74, 0xC3, 0x81, 0x5D, 0x30};

const QString testStr =
    QStringLiteral("C7719C6808C14B77348004956D1D98046CE09A34370E7608150EAD74C3815D30");
const QByteArray testPk = QByteArray::fromHex(testStr.toLatin1());

const QString echoStr =
    QStringLiteral("76518406F6A9F2217E8DC487CC783C25CC16A15EB36FF32E335A235342C48A39");
const QByteArray echoPk = QByteArray::fromHex(echoStr.toLatin1());

class TestChatId : public QObject
{
    Q_OBJECT
private slots:
    void toStringTest();
    void equalTest();
    void clearTest();
    void copyTest();
    void dataTest();
    void sizeTest();
    void hashableTest();
    void invalidLengthIsRejected();
};

void TestChatId::toStringTest()
{
    ToxPk pk(testPk);
    QVERIFY(testStr == pk.toString());
}

void TestChatId::equalTest()
{
    ToxPk pk1(testPk);
    ToxPk pk2(testPk);
    ToxPk pk3(echoPk);
    QVERIFY(pk1 == pk2);
    QVERIFY(pk1 != pk3);
    QVERIFY(!(pk1 != pk2));
}

void TestChatId::clearTest()
{
    ToxPk empty;
    ToxPk pk(testPk);
    QVERIFY(empty.isEmpty());
    QVERIFY(!pk.isEmpty());
}

void TestChatId::copyTest()
{
    ToxPk src(testPk);
    ToxPk copy = src;
    QVERIFY(copy == src);
}

void TestChatId::dataTest()
{
    ToxPk pk(testPk);
    QVERIFY(testPk == pk.getByteArray());
    for (int i = 0; i < pk.getSize(); i++) {
        QVERIFY(testPkArray[i] == pk.getData()[i]);
    }
}

void TestChatId::sizeTest()
{
    ToxPk pk;
    GroupId id;
    QVERIFY(pk.getSize() == ToxPk::size);
    QVERIFY(id.getSize() == GroupId::size);
}

void TestChatId::hashableTest()
{
    ToxPk pk1{testPkArray};
    ToxPk pk2{testPk};
    QVERIFY(qHash(pk1) == qHash(pk2));
    ToxPk pk3{echoPk};
    QVERIFY(qHash(pk1) != qHash(pk3));
}

/**
 * @brief Constructing an id from a byte array of the wrong length must yield an
 *        empty id instead of storing a short buffer.
 * @note Callers (and toxcore) rely on getData() pointing at exactly getSize()
 *       bytes. An attacker controlled byte array (corrupted profile database,
 *       drag and drop MIME data, ...) of the wrong length would otherwise cause
 *       an out of bounds read. Only the assert used to catch this, and asserts
 *       are compiled out in release builds.
 */
void TestChatId::invalidLengthIsRejected()
{
    QVERIFY(ToxPk{QByteArray()}.isEmpty());
    QVERIFY(ToxPk{QByteArray(1, 'a')}.isEmpty());
    QVERIFY(ToxPk{QByteArray(ToxPk::size - 1, 'a')}.isEmpty());
    QVERIFY(ToxPk{QByteArray(ToxPk::size + 1, 'a')}.isEmpty());

    QVERIFY(GroupId{QByteArray()}.isEmpty());
    QVERIFY(GroupId{QByteArray(1, 'a')}.isEmpty());
    QVERIFY(GroupId{QByteArray(GroupId::size - 1, 'a')}.isEmpty());
    QVERIFY(GroupId{QByteArray(GroupId::size + 1, 'a')}.isEmpty());

    // A correctly sized array must still be accepted
    QVERIFY(!ToxPk{QByteArray(ToxPk::size, 'a')}.isEmpty());
    QVERIFY(!GroupId{QByteArray(GroupId::size, 'a')}.isEmpty());
}

QTEST_GUILESS_MAIN(TestChatId)
#include "chatid_test.moc"
