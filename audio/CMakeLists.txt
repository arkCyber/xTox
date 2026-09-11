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

set(SOURCE_FILES
    "include/audio/audio.h"
    "include/audio/iaudiocontrol.h"
    "include/audio/iaudiosettings.h"
    "include/audio/iaudiosink.h"
    "include/audio/iaudiosource.h"
    "src/iaudiosettings.cpp"
    "src/iaudiosink.cpp"
    "src/audio.cpp"
    "src/backend/alsink.cpp"
    "src/backend/alsink.h"
    "src/backend/alsource.cpp"
    "src/backend/alsource.h"
    "src/backend/openal.cpp"
    "src/backend/openal.h")

add_library(audio_library STATIC ${SOURCE_FILES} resources/audio_res.qrc)

# We need this directory, and users of our library will need it too
target_include_directories(audio_library PUBLIC include/)

target_link_libraries(audio_library util_library)
target_link_libraries(audio_library Qt5::Core)
target_link_libraries(audio_library ${OPENAL_LIBRARIES})
target_link_libraries(audio_library xtox::warnings)
