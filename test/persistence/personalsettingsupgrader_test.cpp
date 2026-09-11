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

#include "src/persistence/personalsettingsupgrader.h"
#include "src/persistence/settingsserializer.h"

#include <QtTest/QtTest>
#include <QDir>
#include <QFile>
#include <QString>

class TestPersonalSettingsUpgrader : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void equalVersionIsNoOp();
    void newerVersionIsRejected();
    void negativeVersionIsRejected();
    void upgradeFromZeroSucceeds();

private:
    QString testFile;
};

void TestPersonalSettingsUpgrader::init()
{
    testFile = QDir::tempPath() + QStringLiteral("/xtox_personalsettingsupgrader_test.qts");
    QFile::remove(testFile);
}

void TestPersonalSettingsUpgrader::cleanup()
{
    QFile::remove(testFile);
}

/**
 * @brief An already up to date version must be a no-op.
 */
void TestPersonalSettingsUpgrader::equalVersionIsNoOp()
{
    SettingsSerializer serializer(testFile);
    QVERIFY(PersonalSettingsUpgrader::doUpgrade(serializer, 1, 1));
}

/**
 * @brief A settings file from a newer xTox must be rejected.
 */
void TestPersonalSettingsUpgrader::newerVersionIsRejected()
{
    SettingsSerializer serializer(testFile);
    QVERIFY(!PersonalSettingsUpgrader::doUpgrade(serializer, 2, 1));
}

/**
 * @brief A negative version (corrupted settings file) must be rejected instead of
 *        indexing the upgrade table out of bounds.
 */
void TestPersonalSettingsUpgrader::negativeVersionIsRejected()
{
    SettingsSerializer serializer(testFile);
    QVERIFY(!PersonalSettingsUpgrader::doUpgrade(serializer, -1, 1));
}

/**
 * @brief A version zero settings file must upgrade to the current version.
 */
void TestPersonalSettingsUpgrader::upgradeFromZeroSucceeds()
{
    SettingsSerializer serializer(testFile);
    QVERIFY(PersonalSettingsUpgrader::doUpgrade(serializer, 0, 1));
}

QTEST_GUILESS_MAIN(TestPersonalSettingsUpgrader)
#include "personalsettingsupgrader_test.moc"
