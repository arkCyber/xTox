#   Copyright © 2020 by The xTox Project Contributors
#
#   This file is part of xTox, a Qt-based graphical interface for Tox.
#   xTox is libre software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   xTox is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with xTox.  If not, see <http://www.gnu.org/licenses/>


add_library(util_library STATIC
    "include/util/compatiblerecursivemutex.h"
    "include/util/interface.h"
    "include/util/strongtype.h"
    "include/util/display.h"
    "src/display.cpp"
    "include/util/toxcoreerrorparser.h"
    "src/toxcoreerrorparser.cpp")

# We need this directory, and users of our library will need it too
target_include_directories(util_library PUBLIC include/)
target_link_libraries(util_library PRIVATE Qt5::Core)
target_link_libraries(util_library PRIVATE xtox::warnings)
