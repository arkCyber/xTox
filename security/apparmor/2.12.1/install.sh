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

set -e -u pipefail

readonly SCRIPT_DIR=$(dirname $(readlink -f $0))

if [[ $(id -u) != 0 ]]
then
    >&2 echo "Please run as root."
    exit 1
fi

if [[ -z $(which apparmor_parser) ]]
then
   >&2 echo "AppArmor not found."
   exit 1
fi

echo "Copying AppArmor files..."
[[ ! -d "/etc/apparmor.d/tunables/usr.bin.xtox.d/" ]] && mkdir -v "/etc/apparmor.d/tunables/usr.bin.xtox.d/"
cp -v "${SCRIPT_DIR}/tunables/usr.bin.xtox" "/etc/apparmor.d/tunables/"
cp -v "${SCRIPT_DIR}/usr.bin.xtox" "/etc/apparmor.d/"
touch "/etc/apparmor.d/local/usr.bin.xtox"

echo "Restarting AppArmor..."
systemctl restart apparmor

echo "Done."

