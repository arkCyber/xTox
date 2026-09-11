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

#include "src/persistence/paths.h"

#include <ctime>

#include <QtTest/QtTest>
#include <QString>
#include <QStandardPaths>


class TestPaths : public QObject
{
    Q_OBJECT
private slots:
    void constructAuto();
    void constructPortable();
    void constructNonPortable();
#if PATHS_VERSION_TCS_COMPLIANT
    void checkPathsNonPortable();
    void checkPathsPortable();
#endif
private:
    static void verifyxToxPath(const QString& testPath, const QString& basePath, const QString& subPath);
};

namespace {
#if PATHS_VERSION_TCS_COMPLIANT
const QLatin1String globalSettingsFile{"xtox.ini"};
const QLatin1String profileFolder{"profiles"};
const QLatin1String themeFolder{"themes"};
const QLatin1String avatarsFolder{"avatars"};
const QLatin1String transfersFolder{"transfers"};
const QLatin1String screenshotsFolder{"screenshots"};
#endif // PATHS_VERSION_TCS_COMPLIANT
const QString sep{QDir::separator()};
}

/**
 * @brief Verifies construction in auto mode
 */
void TestPaths::constructAuto()
{
    Paths paths{Paths::Portable::Auto};
    // auto detection should succeed
    // the test environment should not provide a `xtox.ini`
    QVERIFY(paths.isPortable() == false);
}

/**
 * @brief Verifies construction in portable mode
 */
void TestPaths::constructPortable()
{
    Paths paths{Paths::Portable::Portable};
    // portable construction should succeed even though xtox.ini doesn't exist
    QVERIFY(paths.isPortable() == true);
}

/**
 * @brief Verifies construction in non-portable mode
 */
void TestPaths::constructNonPortable()
{
    Paths paths{Paths::Portable::NonPortable};
    // Non portable should succeed
    // the test environment should not provide a `xtox.ini`
    QVERIFY(paths.isPortable() == false);
}

/**
 * @brief Helper to verify xTox specific paths
 */
void TestPaths::verifyxToxPath(const QString& testPath, const QString& basePath, const QString& subPath)
{
    const QString expectPath = basePath % sep % subPath;
    QVERIFY(testPath == expectPath);
}

#if PATHS_VERSION_TCS_COMPLIANT
/**
 * @brief Check generated paths against expected values in non-portable mode
 */
void TestPaths::checkPathsNonPortable()
{
    Paths * paths = Paths::makePaths(Paths::Portable::NonPortable);
    QVERIFY(paths != nullptr);
    // Need non-portable environment to match our test cases
    QVERIFY(paths->isPortable() == false);

    // TODO(sudden6): write robust tests for Tox paths given by Tox Client Standard
    QString avatarDir{paths->getAvatarsDir()};
    QString toxSaveDir{paths->getToxSaveDir()};

    // avatars directory must be one level below toxsave to follow TCS
    QVERIFY(avatarDir.contains(toxSaveDir));

    const QString appData{QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};
    const QString appConfig{QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)};


    verifyxToxPath(paths->getProfilesDir(), appData, profileFolder % sep);
    verifyxToxPath(paths->getGlobalSettingsPath(), appConfig, globalSettingsFile);
    verifyxToxPath(paths->getScreenshotsDir(), appData, screenshotsFolder % sep);
    verifyxToxPath(paths->getTransfersDir(), appData, transfersFolder % sep);

    QStringList themeFolders{};
    themeFolders += QStandardPaths::locate(QStandardPaths::AppDataLocation, themeFolder,
                                           QStandardPaths::LocateDirectory);

    // look for themes beside the xTox binary with lowest priority
    const QString curPath{qApp->applicationDirPath()};
    themeFolders += curPath % sep % themeFolder % sep;

    QVERIFY(paths->getThemeDirs() == themeFolders);
}

/**
 * @brief Check generated paths against expected values in portable mode
 */
void TestPaths::checkPathsPortable()
{
    Paths * paths = Paths::makePaths(Paths::Portable::Portable);
    QVERIFY(paths != nullptr);
    // Need portable environment to match our test cases
    QVERIFY(paths->isPortable() == true);

    const QString basePath{qApp->applicationDirPath()};

    const QString avatarDir{paths->getAvatarsDir()};
    verifyxToxPath(avatarDir, basePath, profileFolder % sep % avatarsFolder % sep);

    const QString toxSaveDir{paths->getToxSaveDir()};
    verifyxToxPath(toxSaveDir, basePath, profileFolder % sep);

    // avatars directory must be one level below toxsave to follow TCS
    QVERIFY(avatarDir.contains(toxSaveDir));

    verifyxToxPath(paths->getProfilesDir(), basePath, profileFolder % sep);
    verifyxToxPath(paths->getGlobalSettingsPath(), basePath, globalSettingsFile);
    verifyxToxPath(paths->getScreenshotsDir(), basePath, screenshotsFolder % sep);
    verifyxToxPath(paths->getTransfersDir(), basePath, transfersFolder % sep);

    // look for themes beside the xTox binary with lowest priority
    const QString curPath{qApp->applicationDirPath()};
    QStringList themeFolders{curPath % sep % themeFolder % sep};

    QVERIFY(paths->getThemeDirs() == themeFolders);
}
#endif

QTEST_GUILESS_MAIN(TestPaths)
#include "paths_test.moc"
