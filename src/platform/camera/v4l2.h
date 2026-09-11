/*
    Copyright © 2015-2019 by The xTox Project Contributors

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

#include "src/video/videomode.h"
#include <QPair>
#include <QString>
#include <QVector>

#if !(defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD))
#error "This file is only meant to be compiled for Linux or FreeBSD targets"
#endif

namespace v4l2 {
QVector<VideoMode> getDeviceModes(QString devName);
QVector<QPair<QString, QString>> getDeviceList();
QString getPixelFormatString(uint32_t pixel_format);
bool betterPixelFormat(uint32_t a, uint32_t b);
}
