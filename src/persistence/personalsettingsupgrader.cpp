/*
    Copyright © 2022 by The xTox Project Contributors

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

#include "personalsettingsupgrader.h"
#include "settingsserializer.h"
#include "src/core/toxpk.h"

#include <QDebug>

#include <cassert>

namespace {
    bool version0to1(SettingsSerializer& ps)
    {
        ps.beginGroup("Friends");
        {
            int size = ps.beginReadArray("Friend");
            for (int i = 0; i < size; i++) {
                ps.setArrayIndex(i);
                const auto oldFriendAddr = ps.value("addr").toString();
                auto newFriendAddr = oldFriendAddr.left(ToxPk::numHexChars);
                ps.setValue("addr", newFriendAddr);
            }
            ps.endArray();
        }
        ps.endGroup();

        return true;
    }
} // namespace

bool PersonalSettingsUpgrader::doUpgrade(SettingsSerializer& settingsSerializer, int fromVer, int toVer)
{
    if (fromVer == toVer) {
        return true;
    }

    if (fromVer > toVer) {
        qWarning().nospace() << "Settings version (" << fromVer
                             << ") is newer than we currently support (" << toVer
                             << "). Please upgrade xTox";
        return false;
    }

    using SettingsUpgradeFn = bool (*)(SettingsSerializer&);
    std::vector<SettingsUpgradeFn> upgradeFns = {version0to1};

    // fromVer comes from the (possibly corrupted or attacker controlled) settings
    // file. The asserts below are compiled out in release builds, so a negative
    // version would index upgradeFns out of bounds. Validate it at runtime instead.
    if (fromVer < 0 || fromVer > static_cast<int>(upgradeFns.size())) {
        qCritical() << "Invalid settings version" << fromVer << ", aborting upgrade";
        return false;
    }

    assert(fromVer < static_cast<int>(upgradeFns.size()));
    assert(toVer == static_cast<int>(upgradeFns.size()));

    for (int i = fromVer; i < static_cast<int>(upgradeFns.size()); ++i) {
        auto const newSettingsVersion = i + 1;
        if (!upgradeFns[i](settingsSerializer)) {
            qCritical() << "Failed to upgrade settings to version " << newSettingsVersion << " aborting";
            return false;
        }
        qDebug() << "Settings upgraded incrementally to schema version " << newSettingsVersion;
    }

    qInfo() << "Settings upgrade finished (settingsVersion" << fromVer << "->"
            << toVer << ")";
    return true;
}
