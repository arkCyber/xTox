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


#pragma once

#include <QByteArray>
#include <QString>
#include <cstdint>

QString dataToString(QByteArray data);
uint64_t dataToUint64(const QByteArray& data);
int dataToVInt(const QByteArray& data);
size_t dataToVUint(const QByteArray& data);
unsigned getVUint32Size(QByteArray data);
QByteArray vintToData(int num);
QByteArray vuintToData(size_t num);
