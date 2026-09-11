#!/usr/bin/env bash

#   Copyright © 2019 by The xTox Project Contributors
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

PWD=`pwd`

echo "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>
<installer-gui-script minSpecVersion=\"1\">
    <pkg-ref id=\"chat.tox.xtox\"/>
    <options hostArchitectures=\"x86_64\"/>
    <title>xTox</title>
    <license file=\"$PWD/gplv3.rtf\"/>
    <welcome file=\"$PWD/welcome.txt\"/>
    <domains enable_currentUserHome=\"true\" enable_localSystem=\"false\" enable_anywhere=\"false\"/>
    <options customize=\"never\" require-scripts=\"true\"/>
    <choices-outline>
        <line choice=\"default\">
            <line choice=\"chat.tox.xtox\"/>
        </line>
    </choices-outline>
    <allowed-os-versions>
    <os-version min=\"10.7\"/>
</allowed-os-versions>
    <choice id=\"default\"/>
    <choice id=\"chat.tox.xtox\" visible=\"false\">
        <pkg-ref id=\"chat.tox.xtox\"/>
    </choice>
    <pkg-ref id=\"chat.tox.xtox\" version=\"1\" onConclusion=\"none\">xtox.pkg</pkg-ref>
</installer-gui-script>" > distribution.xml
