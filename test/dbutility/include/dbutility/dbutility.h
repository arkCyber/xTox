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

#pragma once

#include <QString>

#include <array>
#include <memory>
#include <vector>

class RawDatabase;

namespace DbUtility
{
    struct SqliteMasterEntry {
        QString name;
        QString sql;
        bool operator==(const DbUtility::SqliteMasterEntry& rhs) const;
    };

    extern const std::array<QString, 11> testFileList;
    extern const std::vector<SqliteMasterEntry> schema0;
    extern const std::vector<SqliteMasterEntry> schema1;
    extern const std::vector<SqliteMasterEntry> schema2;
    extern const std::vector<SqliteMasterEntry> schema3;
    extern const std::vector<SqliteMasterEntry> schema4;
    extern const std::vector<SqliteMasterEntry> schema5;
    extern const std::vector<SqliteMasterEntry> schema6;
    extern const std::vector<SqliteMasterEntry> schema7;
    extern const std::vector<SqliteMasterEntry> schema9;
    extern const std::vector<SqliteMasterEntry> schema10;
    extern const std::vector<SqliteMasterEntry> schema11;

    void createSchemaAtVersion(std::shared_ptr<RawDatabase> db, const std::vector<DbUtility::SqliteMasterEntry>& schema);
    void verifyDb(std::shared_ptr<RawDatabase> db, const std::vector<DbUtility::SqliteMasterEntry>& expectedSql);
}
